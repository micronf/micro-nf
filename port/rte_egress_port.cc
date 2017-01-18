#include "rte_egress_port.h"

RteEgressPort::RteEgressPort(const std::string queue_id) {
  this->tx_ring_ = rte_ring_lookup(queue_id.c_str());
}

uint16_t RteEgressPort::TxBurst(struct mbuf** packets, uint16_t burst_size) {
  int num_tx = rte_ring_sp_enqueue_burst(this->tx_ring_, (void**)packets,
                                          burst_size); 
  return num_tx;
}
