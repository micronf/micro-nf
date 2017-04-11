#include "mac_swapper.h"

#include <algorithm>
#include <assert.h>
#include <google/protobuf/map.h>
#include <rte_ether.h>
#include <rte_prefetch.h>
#include <stdlib.h>

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
  PacketProcessor::ConfigurePorts(pp_config, this);

//	this->scale_bits_mz = rte_memzone_lookup(MZ_SCALE);
//  this->scale_bits = (ScaleBitVector*) this->scale_bits_mz->addr;

  // Packet processor specific configuration (if any).
	
}

inline void MacSwapper::Run() {
  rx_pkt_array_t rx_packets;
  register uint16_t i = 0;
  struct ether_hdr* eth_hdr = nullptr;
  uint16_t num_rx = 0;
  while (true) {
    num_rx = this->ingress_ports_[0]->RxBurst(rx_packets);
    for (i = 0; i < num_rx; ++i) {
      ether_hdr* eth_hdr = rte_pktmbuf_mtod(rx_packets[i], struct ether_hdr*);
      std::swap(eth_hdr->s_addr.addr_bytes, eth_hdr->d_addr.addr_bytes);
    }
    this->egress_ports_[0]->TxBurst(rx_packets, num_rx);

		for(i=0; i < num_egress_ports_; i++){
      if(this->scale_bits->bits[this->instance_id_].test(i)){
          // TODO  Change port to smart port.
					
					this->scale_bits->bits[this->instance_id_].set(i, false);
      }
    }
	
  }
}

inline void MacSwapper::FlushState() {}
inline void MacSwapper::RecoverState() {}
