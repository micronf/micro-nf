#include "rte_egress_port.h"

RteEgressPort::RteEgressPort(const std::string queue_id, const unsigned int& port_id) {
  this->tx_ring_ = rte_ring_lookup(queue_id.c_str());
  this->port_id_ = port_id;
}

uint16_t RteEgressPort::TxBurst(void** packets, int burst_size) {
  struct mbuf** tx_mbufs = reinterpret_cast<struct mbuf**>(packets);
  int num_tx =
      rte_ring_sp_enqueue_burst(this->tx_ring_, tx_mbufs, burst_size);
  return num_tx;
}
