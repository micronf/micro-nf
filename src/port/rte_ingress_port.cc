#include "rte_ingress_port.h"

#include <assert.h>
#include <rte_malloc.h>
#include <rte_errno.h>
#include <stdlib.h>
#include <string>

RteIngressPort::RteIngressPort(const std::string& ring_id, const unsigned int& port_id) {
  this->rx_ring_ = rte_ring_lookup(ring_id.c_str());
  assert(this->rx_ring_ != nullptr);
  this->port_id_ = port_id;
}

inline void** RteIngressPort::RxBurst(int burst_size) {
  struct rte_mbuf** packets = 
    reinterpret_cast<struct rte_mbuf**>(
        calloc(burst_size, sizeof(struct mbuf*)));
  assert(packets != nullptr);
  int nb_rx = rte_ring_sc_dequeue_burst(this->rx_ring_, (void**)packets, burst_size);
  return reinterpret_cast<void**>(packets);
}
