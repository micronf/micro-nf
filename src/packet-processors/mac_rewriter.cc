// Author: Anthony Anthony.

#include "mac_rewriter.h"

#include <algorithm>
#include <assert.h>
#include <google/protobuf/map.h>
#include <rte_ether.h>
#include <rte_prefetch.h>
#include <stdlib.h>
#include <iostream>
#include <inttypes.h>

inline void MacRewriter::Init(const PacketProcessorConfig& pp_config) {
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
   auto it = pp_param_map.find( PacketProcessor::compLoad );
   if ( it !=  pp_param_map.end() )
      comp_load_ = it->second;

   it = pp_param_map.find( PacketProcessor::shareCoreFlag );
   if ( it != pp_param_map.end() )
      share_core_ = it->second;
   
   it = pp_param_map.find( "debug" );
   if ( it !=  pp_param_map.end() )
      debug_ = it->second;

   it = pp_param_map.find( PacketProcessor::yieldAfterBatch );
   if ( it !=  pp_param_map.end() )
      yield_after_kbatch_ = it->second;

   this->has_dest_mac_ = pp_config.has_dest_mac(); 
   if ( this->has_dest_mac_ )
      this->dest_mac_ = pp_config.dest_mac();

   it = pp_param_map.find( PacketProcessor::kNumPrefetch );
   if ( it != pp_param_map.end() )
      k_num_prefetch_ = it->second;

   RTE_LOG( INFO, PMD, "rewritter k_num_prefetch_ : %d\n", k_num_prefetch_);

   PacketProcessor::ConfigurePorts(pp_config, this);
}

// Imitating work load.
void static inline imitate_processing( int load ) __attribute__((optimize("O0"))); 
void static inline imitate_processing( int load ) {   
   // Imitate extra processing
   int n = 500 * load;
   for ( int i = 0; i < n; i++ ) {
      int r = 0;
      int s = 999;
      r =  s * s;
   }
}

inline void MacRewriter::Run() {
   rx_pkt_array_t rx_packets;
   register int16_t i = 0;
   struct ether_hdr* eth_hdr = nullptr;
   uint16_t num_rx = 0;
   int res = 0;
   uint32_t counter = 1;

   uint64_t mac_addr = std::stoul( this->dest_mac_, nullptr, 16 );
      
   while ( true ) {
      
      num_rx = this->ingress_ports_[0]->RxBurst(rx_packets);

      for (i = 0; i < num_rx && i < k_num_prefetch_; ++i)
         rte_prefetch_non_temporal(rte_pktmbuf_mtod(rx_packets[i], void*));
      for (i = 0; i < num_rx - k_num_prefetch_; ++i) {
         rte_prefetch_non_temporal(rte_pktmbuf_mtod(rx_packets[i + k_num_prefetch_], void*));
         eth_hdr = rte_pktmbuf_mtod(rx_packets[i], struct ether_hdr*);
         memcpy((uint64_t*)  &eth_hdr->d_addr.addr_bytes[0], &mac_addr, sizeof(uint64_t));

      }
      for ( ; i < num_rx; ++i) {
         eth_hdr = rte_pktmbuf_mtod(rx_packets[i], struct ether_hdr*);
         memcpy( &eth_hdr->d_addr.addr_bytes[0], (struct ether_addr*) &mac_addr, sizeof(struct ether_addr));
      }
      
      // Do some extra work 
      // (desterministic and not compiler optimized.) 
      //imitate_processing( comp_load_ );
 
      this->egress_ports_[0]->TxBurst(rx_packets, num_rx);

      // If num_rx == 0 -> yield
      // Otherwise, try again and until k consecutive hits
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

inline void MacRewriter::FlushState() {}
inline void MacRewriter::RecoverState() {}
