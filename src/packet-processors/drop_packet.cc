#include "drop_packet.h"

#include <rte_mbuf.h>
#include <rte_log.h>

void DropPacket::Init(const PacketProcessorConfig& pp_config) {
  num_ingress_ports_ = pp_config.num_ingress_ports();
  num_egress_ports_ = pp_config.num_egress_ports();
	this->instance_id_ = pp_config.instance_id();
  for (int i = 0; i < num_ingress_ports_; ++i) {
    ingress_ports_.emplace_back(nullptr);
  }
  for (int j = 0; j < num_egress_ports_; ++j) {
    egress_ports_.emplace_back(nullptr);
  }
  PacketProcessor::ConfigurePorts(pp_config, this);
}

void DropPacket::Run() {
  rx_pkt_array_t packets;
  uint16_t num_rx = 0;
  register uint16_t i = 0;
  while(true) {
        num_rx = ingress_ports_[0]->RxBurst(packets);
/*
        for (i = 0; i < num_rx && i < k_num_prefetch_; ++i)
           rte_prefetch_non_temporal(rte_pktmbuf_mtod(packets[i], void*));
        for (i = 0; i < num_rx - k_num_prefetch_; ++i) {
           rte_prefetch_non_temporal(rte_pktmbuf_mtod(packets[i + k_num_prefetch_], void*));         
           rte_pktmbuf_free(packets[i]); 
        }
        for ( ; i < num_rx; ++i) {
           rte_pktmbuf_free(packets[i]);  
        }

*/       //  without prefetch 
        for (i = 0; i < num_rx; ++i) {
           //RTE_LOG( WARNING, USER1, "@@@ drop\n");
           rte_pktmbuf_free(packets[i]);
        }
  }
}

void DropPacket::FlushState() {}
void DropPacket::RecoverState() {}
