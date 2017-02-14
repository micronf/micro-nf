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
    this->ingress_ports_.emplace_back(s, i);
  }
  for (int i = 0; i < this->num_egress_ports_; ++i) {
    auto& s = config.egress_queue_ids()[i];
    this->egress_ports_.emplace_back(s, i);
  }
}

void MacSwapper::Run() {
  while(true) {
    struct rte_mbuf** rx_mbufs = reinterpret_cast<struct rte_mbuf**>(
                                  this->ingress_ports_[0].RxBurst(64));
    int i = 0;
    for (i = 0; i < 64; ++i) {
      if (!rx_mbufs[i]) break;
      struct ether_hdr* eth_hdr = rte_pktmbuf_mtod(rx_mbufs[i], struct ether_hdr*);
      for (int j = 0; j < ETHER_ADDR_LEN; ++j) {
        std::swap(eth_hdr->s_addr.addr_bytes[j], eth_hdr->d_addr.addr_bytes[j]);
      }
      rte_pktmbuf_dump(stdout, rx_mbufs[i], rx_mbufs[i]->pkt_len);
    }
    // this->egress_ports_[0].TxBurst(reinterpret_cast<void**>(rx_mbufs), i);
    free(rx_mbufs);
  }
}

void MacSwapper::FlushState() {}

void MacSwapper::RecoverState() {}
