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

/* __inline__ uint64_t start_rdtsc() {
   unsigned int lo,hi;
   //preempt_disable();
   //raw_local_irq_save(_flags);

   __asm__ __volatile__ ("CPUID\n\t"
                         "RDTSC\n\t"
                         "mov %%edx, %0\n\t"
                         "mov %%eax, %1\n\t": "=r" (hi), "=r" (lo):: "%rax", "%rbx", "%rcx", "%rdx");
   return ((uint64_t)hi << 32) | lo;
}
*/

__inline__ uint64_t get_rdtscp() {
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
  
   uint32_t volatile counter = 0;
   bool volatile sample_flag, dump_flag;
   uint64_t start_cycles = 0;
   uint64_t end_cycles = 0;
   int64_t comp_cycles = 0;
   int64_t yield_cycles = 0;

   uint32_t volatile  q = 0;
   int64_t acc_comp[1000] = {};
   int64_t acc_yield[1000] = {};

   while ( true ) {

      //  printf after certain interval
      sample_flag = ( (counter % 100000) == 0);
      dump_flag   = ( (counter % 2000000) == 0);

      if ( likely( debug_ ) ) {
         start_cycles = get_rdtscp(); 
         // Calculating running average of cycles spent from sched_yield() until rescheduled.
         // Only applicable for non-share core uNF. Ignore the first round value.
         yield_cycles = ( start_cycles - end_cycles ) ;

         if ( sample_flag ) {
            acc_yield[q] = yield_cycles;
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
      
      if ( likely ( debug_ ) ) {
         end_cycles =  get_rdtscp();
         // Measure the running average of cycles spent in packet retrieval, 
         // processing, extra work, and push back to next ring.
         // This makes sense with SCHED_RR (RT) because process won't be 
         // preempted during processing.
         comp_cycles = ( end_cycles - start_cycles );
       
         if ( sample_flag ) {
            acc_comp[q] = comp_cycles;
            q++; 
         }
      }
      
      if ( dump_flag ) {
         //   int64_t comp_avg = 0;
         //int64_t yield_avg = 0;
         for ( int i=0; i < q; i++ ){
               printf( "ID: %d.   Comp_cycles: %lu.   Yield_cycles: %lu.\n", 
                    this->instance_id_, acc_comp[i], acc_yield[i] );
            //yield_avg += acc_yield[i];
            //comp_avg += acc_comp[i];
         }
         //comp_avg /= q;
         //yield_avg /= q;
         //printf( "ID: %d.   Comp_avg: %lu.   Yield_avg: %lu.\n", 
         //      this->instance_id_, comp_avg, yield_avg );

         //printf( "q: %u. counter: %u\n", q, counter );
         q = 0;
         counter = 0; 
      }

      counter++;

      if ( likely( share_core_ ) ) {
         res = sched_yield();
         if ( unlikely( res == -1 ) ) {
            std::cerr << "sched_yield failed! Exitting." << std::endl;
            return;
         }
      }
   } 
}

inline void MacSwapper::FlushState() {}
inline void MacSwapper::RecoverState() {}
