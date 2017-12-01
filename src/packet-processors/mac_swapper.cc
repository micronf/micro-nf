#include "mac_swapper.h"

#include <algorithm>
#include <assert.h>
#include <google/protobuf/map.h>
#include <rte_ether.h>
#include <rte_prefetch.h>
#include <stdlib.h>
#include <rte_cycles.h>
#include <iostream>

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
   
   // Retrieve share cpu config
   share_core_ = pp_config.pp_parameters().find( 
         PacketProcessor::shareCoreFlag )->second;
   sem_enable_ = pp_config.pp_parameters().find(
         PacketProcessor::semaphoreFlag )->second;
   cpu_id_ = pp_config.pp_parameters().find( PacketProcessor::cpuId )->second;

   fprintf( stdout, "mac_swapper.cc: share_core_: %d. sem_enable_: %d. cpu_id_:%d\n", share_core_, sem_enable_, cpu_id_ );
   if ( sem_enable_ ) {
      std::string sname = "/SEM_CORE_" + std::to_string( cpu_id_ );
      std::cout << "!!!!!!!!!!!! sname: " << sname << std::endl;
      semaphore_ = sem_open( sname.c_str(), 0 );

      int val;
      sem_getvalue(semaphore_, &val);
      std::cout << this->instance_id_ << " MacSwapper Init Val: " << val << std::endl;

      if ( semaphore_ == SEM_FAILED ) {
         std::cerr << "sem_open failed!" << std::endl ;
         return;
      }
   } 
   else {
      std::cout << "!!!!!!!!!!!!!!!!! sem_enable is 0" << std::endl;
   }
   
   PacketProcessor::ConfigurePorts(pp_config, this);
  
}

inline void MacSwapper::Run() {
   rx_pkt_array_t rx_packets;
   register int16_t i = 0;
   const int16_t kNumPrefetch = 8;
   struct ether_hdr* eth_hdr = nullptr;
   uint16_t num_rx = 0;
   int res = 0;
//   int n = 0;
   while (true) {
      if ( sem_enable_ ) {
         res = sem_wait( semaphore_ );
      }
      num_rx = this->ingress_ports_[0]->RxBurst(rx_packets);
      // FIXME delete later
/*      if (!(n++ % 100000)) {
         if(this->instance_id_ != 3)
            printf( "[%d] num_rx: %d\n", this->instance_id_, num_rx );
      }
*/
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
      //rte_delay_us_block(32);
      this->egress_ports_[0]->TxBurst(rx_packets, num_rx);
      for (i=0; i < num_egress_ports_; i++){
         if (this->scale_bits->bits[this->instance_id_].test(i)){
            // TODO  Change port to smart port.
            this->scale_bits->bits[this->instance_id_].set(i, false);
         }
      }
    
      // Signal Semaphore and yield CPU
      if ( sem_enable_ ) {
         int val;
         sem_getvalue(semaphore_, &val);
         //std::cout << this->instance_id_ << " MacSwapper Before Val: " << val << std::endl;
         res = sem_post( semaphore_ );
         sem_getvalue(semaphore_, &val);
         //std::cout << this->instance_id_ << " MacSwapper After Val: " << val << std::endl;

      }
      if ( share_core_ ) {
         res = sched_yield();
         if ( res == -1 ) {
            std::cerr << "sched_yield failed! Exitting." << std::endl;
            return;
         }
         //rte_delay_us_block(32);
      }
   }
}

inline void MacSwapper::FlushState() {}
inline void MacSwapper::RecoverState() {}
