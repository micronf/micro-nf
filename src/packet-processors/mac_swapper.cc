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
   cpu_id_ = pp_param_map.find( PacketProcessor::cpuId )->second;
   auto it = pp_param_map.find( PacketProcessor::compLoad );
   if ( it !=  pp_param_map.end() )
      comp_load_ = it->second;
   it = pp_param_map.find( "debug" );
   if ( it !=  pp_param_map.end() )
      debug_ = it->second;

   it = pp_param_map.find( PacketProcessor::yieldAfterBatch );
   if ( it !=  pp_param_map.end() )
      yield_after_kbatch_ = it->second;

   fprintf( stdout, "mac_swapper.cc: Id:%d. share_core_: %d. cpu_id_:%d\n"
            , instance_id_, share_core_, cpu_id_ );
   
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
   uint32_t counter = 1;

   while ( true ) {

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
    
      if ( share_core_ ) {
         if ( counter == yield_after_kbatch_ ) {
            counter = 0;
            res = sched_yield();
            if ( unlikely( res == -1 ) ) {
               std::cerr << "sched_yield failed! Exitting." << std::endl;
               return;
            }
         }
       }
      counter++;          
   } 
}

inline void MacSwapper::FlushState() {}
inline void MacSwapper::RecoverState() {}
