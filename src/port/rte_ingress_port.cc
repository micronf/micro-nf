#include "rte_ingress_port.h"

#include <assert.h>
#include <rte_errno.h>
#include <rte_malloc.h>
#include <stdlib.h>

RteIngressPort::RteIngressPort(const unsigned int port_id,
                               const std::string &ring_id) {
  this->port_id_ = port_id;
  this->rx_ring_ = rte_ring_lookup(ring_id.c_str());
}

inline int
RteIngressPort::RxBurst(rx_pkt_array_t &packets) {
  int num_rx = rte_ring_sc_dequeue_burst(
      this->rx_ring_, reinterpret_cast<void **>(packets.data()), RX_BURST_SIZE);
  return num_rx;
}
