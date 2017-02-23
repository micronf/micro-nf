#include "mac_swapper.h"
#include <algorithm>
#include <assert.h>
#include <rte_ether.h>
#include <stdlib.h>

void MacSwapper::Init(const PacketProcessorConfig& config) {
  this->num_ingress_ports_ = config.ingress_queue_ids().size();
  this->num_egress_ports_ = config.egress_queue_ids().size();
  assert(this->num_ingress_ports_ == 1);
  assert(this->num_egress_ports_ == 0);
  for (int i = 0; i < this->num_ingress_ports_; ++i) {
    auto& s = config.ingress_queue_ids()[i];
    this->ingress_ports_.emplace_back(i, s);
  }
  for (int i = 0; i < this->num_egress_ports_; ++i) {
    auto& s = config.egress_queue_ids()[i];
    this->egress_ports_.emplace_back(i, s);
  }
}

void MacSwapper::Run() {
  rx_pkt_array_t rx_packets;
  while(true) {
    int num_rx = this->ingress_ports_[0].RxBurst(rx_packets);
    int i = 0;
    for (i = 0; i < num_rx; ++i) {
      struct ether_hdr* eth_hdr = rte_pktmbuf_mtod(rx_packets[i], struct ether_hdr*);
      for (int j = 0; j < ETHER_ADDR_LEN; ++j) {
        std::swap(eth_hdr->s_addr.addr_bytes[j], eth_hdr->d_addr.addr_bytes[j]);
      }
      rte_pktmbuf_dump(stdout, rx_packets[i], rx_packets[i]->pkt_len);
    }
  }
}

void MacSwapper::FlushState() {}

void MacSwapper::RecoverState() {}
