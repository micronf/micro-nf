#include "looper.h"
#include <rte_ether.h>

const std::string Looper::kConfLoopCounter = "loop_counter";

inline void Looper::Init(const PacketProcessorConfig& pp_config) {
  num_ingress_ports_ = pp_config.num_ingress_ports();
  num_egress_ports_ = pp_config.num_egress_ports();
  instance_id_ = pp_config.instance_id();
  int i = 0;
  for (i = 0; i < this->num_ingress_ports_; ++i) 
    ingress_ports_.emplace_back(nullptr);
  for (i = 0; i < this->num_egress_ports_; ++i) 
    egress_ports_.emplace_back(nullptr);
  PacketProcessor::ConfigurePorts(pp_config, this);
  this->loop_counter_ = pp_config.pp_parameters().find(Looper::kConfLoopCounter)->second;
}

inline void Looper::Run() {
  rx_pkt_array_t rx_packets;
  register int16_t i = 0, j = 0;
  uint16_t num_rx = 0, num_tx = 0;
  struct ether_hdr* eth_hdr = nullptr;
  while(true) {
    num_rx = this->ingress_ports_[0]->RxBurst(rx_packets);
    for (i = 0; i < num_rx; ++i) {
      // Access inside the mbuf struct to make sure there is cache warm up
      // operation.
      eth_hdr = rte_pktmbuf_mtod(rx_packets[i], struct ether_hdr*);

      for (j = 0; j < this->loop_counter_; ++j);
    }
    num_tx = this->egress_ports_[0]->TxBurst(rx_packets, num_rx);
    for (i = num_tx; i < num_rx; ++i)
      rte_pktmbuf_free(rx_packets[i]);
  }
}

inline void Looper::FlushState() {}
inline void Looper::RecoverState() {}
