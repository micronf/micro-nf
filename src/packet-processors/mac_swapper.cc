#include "mac_swapper.h"

#include <algorithm>
#include <assert.h>
#include <google/protobuf/map.h>
#include <rte_ether.h>
#include <rte_prefetch.h>
#include <stdlib.h>
#include <rte_cycles.h>
#include <iostream>
#include <inttypes.h>

__inline__ uint64_t start_rdtsc() {
   unsigned int lo,hi;
   //preempt_disable();
   //raw_local_irq_save(_flags);

   __asm__ __volatile__ ("CPUID\n\t"
                         "RDTSC\n\t"
                         "mov %%edx, %0\n\t"
                         "mov %%eax, %1\n\t": "=r" (hi), "=r" (lo):: "%rax", "%rbx", "%rcx", "%rdx");
   return ((uint64_t)hi << 32) | lo;
}

__inline__ uint64_t end_rdtsc() {
   unsigned int lo, hi;

   __asm__ __volatile__ ("RDTSCP\n\t"
                         "mov %%edx, %0\n\t"
                         "mov %%eax, %1\n\t"
                         "CPUID\n\t": "=r" (hi), "=r" (lo):: "%rax", "%rbx", "%rcx", "%rdx");
   //raw_local_irq_save(_flags);
   //preempt_enable();
   return ((uint64_t)hi << 32) | lo;
}

inline void MacSwapper::Init(const PacketProcessorConfig& pp_config) {
   num_ingress_ports_ = pp_config.num_ingress_ports();
   num_egress_ports_ = pp_config.num_egress_ports();
   instance_id_ = pp_config.instance_id();

   // First, initialize the list of ingress and egress ports.
   int i = 0;
   for (i = 0; i < this->num_ingress_ports_; ++i) {
      ingress_ports_.emplace_back(nullptr);
   }
   for (i = 0; i < this->num_egress_ports_; ++i) {
      egress_ports_.emplace_back(nullptr);
   }
   
   // Retrieve pp_params config
   auto pp_param_map = pp_config.pp_parameters();
   share_core_ = pp_param_map.find( PacketProcessor::shareCoreFlag )->second;
   sem_enable_ = pp_param_map.find( PacketProcessor::semaphoreFlag )->second;
   cpu_id_ = pp_param_map.find( PacketProcessor::cpuId )->second;
   auto it = pp_param_map.find( "comp_load" );
   if ( it !=  pp_param_map.end() )
      comp_load_ = it->second;
   it = pp_param_map.find( "debug" );
   if ( it !=  pp_param_map.end() )
      debug_ = it->second;

   fprintf( stdout, "mac_swapper.cc: Id:%d. share_core_: %d. sem_enable_: %d. \
cpu_id_:%d\n", instance_id_, share_core_, sem_enable_, cpu_id_ );

   if ( sem_enable_ ) {
      sem_set_id_ = PacketProcessor::lookup_vsem();

      share_p_idx_ = instance_id_ - 1;
      share_np_idx_= ( share_p_idx_ + 1 ) % n_share_core_x;

      std::cout << "sem_enable is 1 " \
                << "p_idx: " << share_p_idx_ << " np_idx: " \
                << share_np_idx_ << std::endl;
   } 
   else {
      std::cout << "sem_enable is 0" << std::endl;
   }
   
   PacketProcessor::ConfigurePorts(pp_config, this);
}

// Imitating work load.
void imitate_processing( int load ) __attribute__((optimize("O0"))); 
void imitate_processing( int load ) {   
   // Imitate extra processing
   int n = 500 * load;
   for ( int i = 0; i < n; i++ ) {
      int r = 0;
      int s = 999;
      r =  s * s;
   }
}

inline void MacSwapper::Run() {
   rx_pkt_array_t rx_packets;
   register int16_t i = 0;
   const int16_t kNumPrefetch = 8;
   struct ether_hdr* eth_hdr = nullptr;
   uint16_t num_rx = 0;
   int res = 0;
   uint32_t counter = 0;
   uint64_t start_cycles = 0;
   uint64_t end_cycles = 0;
   int64_t avg_cycles = 0;
   int64_t yield_avg = 0;

   while ( true ) {

      //  printf after certain interval
      bool print_flag = ( (counter ^ 100000) == 0);

      if ( likely( debug_ ) ) {
         start_cycles = start_rdtsc(); 
         // Calculating running average of cycles spent from sched_yield() until rescheduled.
         // Only applicable for non-share core uNF
         yield_avg = ( start_cycles - end_cycles ) ;

         if ( print_flag ) {
            printf( "%d: yield cycles: %" PRId64 "\n", this->instance_id_, yield_avg );
            counter = 0;
         }
      }

      // Wait for my turn to use the CPU
      if ( unlikely( sem_enable_ ) ) {
         res = PacketProcessor::wait_vsem( sem_set_id_, share_p_idx_ );
         if ( res < 0 )
            perror( "ERROR: wait_vsem()." );
      }

      num_rx = this->ingress_ports_[0]->RxBurst(rx_packets);
      for (i = 0; i < num_rx && i < kNumPrefetch; ++i)
         rte_prefetch0(rte_pktmbuf_mtod(rx_packets[i], void*));
      for (i = 0; i < num_rx - kNumPrefetch; ++i) {
         rte_prefetch0(rte_pktmbuf_mtod(rx_packets[i + kNumPrefetch], void*));
         eth_hdr = rte_pktmbuf_mtod(rx_packets[i], struct ether_hdr*);
         std::swap(eth_hdr->s_addr.addr_bytes, eth_hdr->d_addr.addr_bytes);
      }
      for ( ; i < num_rx; ++i) {
         eth_hdr = rte_pktmbuf_mtod(rx_packets[i], struct ether_hdr*);
         std::swap(eth_hdr->s_addr.addr_bytes, eth_hdr->d_addr.addr_bytes);
      }
      
      // Do some extra work 
      // (desterministic and not compiler optimized.) 
      imitate_processing( comp_load_ );
 
      this->egress_ports_[0]->TxBurst(rx_packets, num_rx);
      for (i=0; i < num_egress_ports_; i++){
         if (this->scale_bits->bits[this->instance_id_].test(i)){
            // TODO  Change port to smart port.
            this->scale_bits->bits[this->instance_id_].set(i, false);
         }
      }
    
      // Signal Semaphore and yield CPU
      if ( unlikely( sem_enable_ ) ) {
         res = PacketProcessor::post_vsem( sem_set_id_, share_np_idx_ );
         if ( unlikely( res < 0 ) )
            perror( "ERROR: post_vsem()." );
      }
 
      if ( print_flag )
         printf( "%d: avg_cycles: %" PRId64 "\n", this->instance_id_, avg_cycles );
      
      if ( likely ( debug_ ) ) {
         end_cycles =  end_rdtsc();
         // Measure the running average of cycles spent in packet retrieval, 
         // processing, extra work, and push back to next ring.
         // This makes sense with SCHED_RR (RT) because process won't be 
         // preempted during processing.
         avg_cycles = ( end_cycles - start_cycles );
      }

      if ( likely( share_core_ ) ) {
         res = sched_yield();
         if ( unlikely( res == -1 ) ) {
            std::cerr << "sched_yield failed! Exitting." << std::endl;
            return;
         }
      }
           
      counter++;
   } 
}

inline void MacSwapper::FlushState() {}
inline void MacSwapper::RecoverState() {}
