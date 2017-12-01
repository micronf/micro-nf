#include "sleepy.h"

#include <rte_cycles.h>
#include <rte_ether.h>

const std::string Sleepy::kConfSleepDurationUs = "sleep_duration_us";

inline void Sleepy::Init(const PacketProcessorConfig& pp_config) {
  this->num_ingress_ports_ = pp_config.num_ingress_ports();
  this->num_egress_ports_ = pp_config.num_egress_ports();
  this->instance_id_ = pp_config.instance_id();
  int i = 0;
  for (i = 0; i < this->num_ingress_ports_; ++i)
    this->ingress_ports_.emplace_back(nullptr);
  for (i = 0; i < this->num_egress_ports_; ++i)
    this->egress_ports_.emplace_back(nullptr);
  PacketProcessor::ConfigurePorts(pp_config);
  this->sleep_duration_us_ = pp_config.pp_parameters().find(Sleepy::kConfSleepDurationUs)->second;
}

inline void Sleepy::Run() {
  rx_pkt_array_t rx_packets;
  uint16_t num_rx = 0, num_tx = 0;
  register uint16_t i = 0, j = 0;
  struct ether_hdr* eth_hdr = nullptr;
  while (true) {
    num_rx = ingress_ports_[0]->RxBurst(rx_packets);
    for (i = 0; i < num_rx; ++i) {
      eth_hdr = rte_pktmbuf_mtod(rx_packets[i], struct ether_hdr*);
      rte_delay_us(this->sleep_duration_us_);
    }
    num_tx = egress_ports_[0]->TxBurst(rx_packets, num_rx);
    for (i = num_tx; i < num_rx; ++i) {
      rte_pktmbuf_free(rx_packets[i]);
    }
  }
}

inline void Sleepy::FlushState() {}
inline void Sleepy::RecoverState() {}
