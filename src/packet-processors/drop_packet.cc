#include "drop_packet.h"

#include <rte_mbuf.h>

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
    for (auto& ingress_port : ingress_ports_) {
      num_rx = ingress_port->RxBurst(packets);
      for (i = 0; i < num_rx; ++i) {
        rte_pktmbuf_free(packets[i]);
      }
    }

		for(int i; i < num_egress_ports_; i++){
      if(this->scale_bits->bits[this->instance_id_].test(i)){
          // TODO 
          // Change port to smart port.

          this->scale_bits->bits[this->instance_id_].set(i, false);

      }
    }

  }
}

void DropPacket::FlushState() {}
void DropPacket::RecoverState() {}
