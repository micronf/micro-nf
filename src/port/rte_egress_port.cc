#include "rte_egress_port.h"

RteEgressPort::RteEgressPort(const unsigned int port_id,
                             const std::string &queue_id) {
  this->tx_ring_ = rte_ring_lookup(queue_id.c_str());
  this->port_id_ = port_id;
}

inline int
RteEgressPort::TxBurst(std::array<struct rte_mbuf *, TX_BURST_SIZE> &packets) {
  int num_tx = rte_ring_sp_enqueue_burst(
      this->tx_ring_, reinterpret_cast<void **>(packets.data()), TX_BURST_SIZE);
  return num_tx;
}
