#include "mark_and_forward_egress_port.h"

#include <rte_mbuf.h>

MarkAndForwardEgressPort::MarkAndForwardEgressPort() {
  this->port_id_ = -1;
  this->bitmap_index_ = this->bitmap_offset_ = -1;
  this->tx_ring_ = nullptr;
}

void MarkAndForwardEgressPort::Init(
    std::map<std::string, std::string>& port_config) {
  this->port_id_ = std::stoi(port_config[EgressPort::kConfPortId]);
  this->tx_ring_ =
      rte_ring_lookup(port_config[EgressPort::kConfRingId].c_str());
  this->bitmap_index_ = this->port_id_ >> 3;
  this->bitmap_offset_ = this->port_id_ & 8;
}

int MarkAndForwardEgressPort::TxBurst(tx_pkt_array_t& packets) {
  for (int i = 0; i < packets.size(); ++i) {
    char* mdata_ptr = reinterpret_cast<char*>(
        reinterpret_cast<unsigned long>(packets[i]) + sizeof(struct rte_mbuf));
    mdata_ptr[this->bitmap_index_] |= (1 << this->bitmap_offset_);
  }
  int num_tx = rte_ring_sp_enqueue_burst(
      this->tx_ring_, reinterpret_cast<void**>(packets.data()), packets.size());
  return num_tx;
}
