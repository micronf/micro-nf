#include "ip_ttl_decrementer.h"

#include <algorithm>
#include <assert.h>
#include <google/protobuf/map.h>
#include <rte_ether.h>
#include <rte_ip.h>
#include <rte_prefetch.h>
#include <stdlib.h>
#include <rte_cycles.h>
#include <iostream>
#include <inttypes.h>
#include <rte_log.h>

inline void IPTtlDecrementer::Init(const PacketProcessorConfig& pp_config) {
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
   auto it = pp_param_map.find(PacketProcessor::compLoad);
   if (it !=  pp_param_map.end())
      comp_load_ = it->second;
   it = pp_param_map.find(PacketProcessor::shareCoreFlag);
   if ( it != pp_param_map.end() )
      share_core_ = it->second;
   it = pp_param_map.find( "debug" );
   if ( it !=  pp_param_map.end() )
      debug_ = it->second;
   it = pp_param_map.find(PacketProcessor::yieldAfterBatch);
   if ( it !=  pp_param_map.end() )
      yield_after_kbatch_ = it->second;
   it = pp_param_map.find(PacketProcessor::kNumPrefetch);
   if (it != pp_param_map.end())
      k_num_prefetch_ = it->second;
   it = pp_param_map.find(PacketProcessor::iterPayload);
   if (it != pp_param_map.end())
      iter_payload_ = it->second;
   RTE_LOG(INFO, PMD, "k_num_prefetch_ : %d\n", k_num_prefetch_);   
   PacketProcessor::ConfigurePorts(pp_config, this);
}

inline void IPTtlDecrementer::Run() {
   rx_pkt_array_t rx_packets;
   register int16_t i = 0;
   struct ether_hdr* eth_hdr = nullptr;
   struct ipv4_hdr* ip_hdr = nullptr;
   uint16_t num_rx = 0;
   int32_t res = 0;
   uint32_t hit_count = 1;
   register uint8_t current_tx_port_index = 0;
   while (true) {
      num_rx = this->ingress_ports_[0]->RxBurst(rx_packets);

      // If num_rx == 0 -> yield
      // Otherwise, try again and until k consecutive hits and then yield
      if (share_core_) {
         if (unlikely(num_rx == 0 || hit_count == yield_after_kbatch_)) {
            hit_count = 1;
            res = sched_yield();
            if (unlikely(res == -1)) {
               std::cerr << "sched_yield failed! Exitting." << std::endl;
               return;
            }
         }
         else
            ++hit_count;
      }

      for (i = 0; i < num_rx && i < k_num_prefetch_; ++i)
         rte_prefetch_non_temporal(rte_pktmbuf_mtod(rx_packets[i], void*));
      for (i = 0; i < num_rx - k_num_prefetch_; ++i) {
         rte_prefetch_non_temporal(rte_pktmbuf_mtod(rx_packets[i + k_num_prefetch_], void*));
         eth_hdr = rte_pktmbuf_mtod(rx_packets[i], struct ether_hdr*);
         ip_hdr = reinterpret_cast<ipv4_hdr*>(eth_hdr + 1);
         std::swap(eth_hdr->s_addr.addr_bytes, eth_hdr->d_addr.addr_bytes);
         --ip_hdr->time_to_live;
      }
      for ( ; i < num_rx; ++i) {
         eth_hdr = rte_pktmbuf_mtod(rx_packets[i], struct ether_hdr*);
         ip_hdr = reinterpret_cast<ipv4_hdr*>(eth_hdr + 1);
         std::swap(eth_hdr->s_addr.addr_bytes, eth_hdr->d_addr.addr_bytes);
         --ip_hdr->time_to_live;
      }
      this->egress_ports_[current_tx_port_index]->TxBurst(rx_packets, num_rx);
      current_tx_port_index = (current_tx_port_index + 1) % this->num_egress_ports_;
  }
}

inline void IPTtlDecrementer::FlushState() {}
inline void IPTtlDecrementer::RecoverState() {}
