#include "rte_ingress_port.h"

#include <assert.h>
#include <rte_errno.h>
#include <rte_malloc.h>
#include <stdlib.h>

inline void RteIngressPort::Init(
    std::map<std::string, std::string>& port_config) {
  this->port_id_ = std::stoi(port_config[IngressPort::kConfPortId]);
  this->rx_ring_ = rte_ring_lookup(port_config[IngressPort::kConfRingId].c_str());
}

inline int
RteIngressPort::RxBurst(rx_pkt_array_t &packets) {
  int num_rx = rte_ring_sc_dequeue_burst(
      this->rx_ring_, reinterpret_cast<void **>(packets.data()), RX_BURST_SIZE);
  return num_rx;
}
