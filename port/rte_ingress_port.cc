#include "rte_ingress_port.h"

RteIngressPort::RteIngressPort(const std::string& ring_id) {
  this->rx_ring_ = rte_ring_lookup(ring_id.c_str());
}

struct rte_mbuf** RteIngressPort::RxBurst(int burst_size) {
  struct rte_mbuf* packets[burst_size] = {nullptr};
  rte_ring_sc_dequeue_burst(this->rx_ring_, (void**)&packets, burst_size);
  return packets;
}
