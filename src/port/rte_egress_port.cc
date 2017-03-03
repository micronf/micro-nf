#include "rte_egress_port.h"

void RteEgressPort::Init(std::map<std::string, std::string> &port_config) {
  this->tx_ring_ =
      rte_ring_lookup(port_config[EgressPort::kConfRingId].c_str());
  this->port_id_ = std::stoi(port_config[EgressPort::kConfPortId]);
}

inline int RteEgressPort::TxBurst(tx_pkt_array_t &packets) {
  int num_tx = rte_ring_sp_enqueue_burst(
      this->tx_ring_, reinterpret_cast<void **>(packets.data()), TX_BURST_SIZE);
  return num_tx;
}
