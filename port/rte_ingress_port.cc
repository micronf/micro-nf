#include "rte_ingress_port.h"

#include <string>

RteIngressPort::RteIngressPort(const std::string& ring_id, const unsigned int& port_id) {
  this->rx_ring_ = rte_ring_lookup(ring_id.c_str());
  this->port_id_ = port_id;
}

void** RteIngressPort::RxBurst(int burst_size) {
  std::string memzone_id = "rte_ingress_port_memzone_" + std::to_string(this->port_id_);
  struct rte_mbuf** packets =
      rte_malloc(memzone_id.c_str(), sizeof(struct rte_mbuf*), RTE_CACHE_LINE_SIZE);
  rte_ring_sc_dequeue_burst(this->rx_ring_, (void**)&packets, burst_size);
  return reinterpret_cast<void**>(packets);
}
