#include "mac_swapper.h"

#include <algorithm>
#include <assert.h>
#include <google/protobuf/map.h>
#include <rte_cycles.h>
#include <rte_ether.h>
#include <rte_prefetch.h>
#include <sched.h>
#include <stdlib.h>

inline void MacSwapper::Init(const PacketProcessorConfig& pp_config) {
  num_ingress_ports_ = pp_config.num_ingress_ports();
  num_egress_ports_ = pp_config.num_egress_ports();

  // First, initialize the list of ingress and egress ports.
  int i = 0;
  for (i = 0; i < this->num_ingress_ports_; ++i) {
    ingress_ports_.emplace_back(nullptr);
  }
  for (i = 0; i < this->num_egress_ports_; ++i) {
    egress_ports_.emplace_back(nullptr);
  }
  PacketProcessor::ConfigurePorts(pp_config, this);

  // Packet processor specific configuration (if any).
}

inline void MacSwapper::Run() {
  rx_pkt_array_t rx_packets;
  register uint16_t i = 0;
  struct ether_hdr* eth_hdr = nullptr;
  uint16_t num_rx = 0;
  const uint64_t kDrainTsc = (rte_get_tsc_hz() + US_PER_S - 1) / US_PER_S;
  uint64_t cur_tsc = 0, diff_tsc = 0, prev_tsc = rte_rdtsc(), timer_tsc = 0, 
           total_tx = 0, cur_tx = 0;
  const uint64_t kTimerPeriod = rte_get_timer_hz() * 10;
  while (true) {
    cur_tsc = rte_rdtsc();
    timer_tsc += (cur_tsc - prev_tsc);
    if (unlikely(timer_tsc > kTimerPeriod)) {
      printf("%lu\n", total_tx - cur_tx);
      cur_tx = total_tx;
      timer_tsc = 0;
    }
    prev_tsc = cur_tsc;
    num_rx = this->ingress_ports_[0]->RxBurst(rx_packets);
    for (i = 0; i < num_rx; ++i) {
      ether_hdr* eth_hdr = rte_pktmbuf_mtod(rx_packets[i], struct ether_hdr*);
      std::swap(eth_hdr->s_addr.addr_bytes, eth_hdr->d_addr.addr_bytes);
    }
    total_tx += this->egress_ports_[0]->TxBurst(rx_packets, num_rx);
  }
}

inline void MacSwapper::FlushState() {}
inline void MacSwapper::RecoverState() {}
