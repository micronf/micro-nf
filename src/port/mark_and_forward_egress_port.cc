#include "mark_and_forward_egress_port.h"

#include <rte_mbuf.h>

MarkAndForwardEgressPort::MarkAndForwardEgressPort() {
  this->port_id_ = -1;
  this->bitmap_index_ = this->bitmap_offset_ = -1;
  this->tx_ring_ = nullptr;
}

MarkAndForwardEgressPort::MarkAndForwardEgressPort(int port_id,
                                                   const std::string &ring_id) {
  this->port_id_ = port_id;
  this->tx_ring_ = rte_ring_lookup(ring_id.c_str());
  this->bitmap_index_ = port_id >> 3;
  this->bitmap_offset_ = port_id & 8;
}

int MarkAndForwardEgressPort::TxBurst(tx_pkt_array_t& packets) {
  for (int i = 0; i < packets.size(); ++i) {
    char *mdata_ptr = reinterpret_cast<char *>(
        reinterpret_cast<unsigned long>(packets[i]) + sizeof(struct rte_mbuf));
    mdata_ptr[this->bitmap_index_] |= (1 << this->bitmap_offset_);
  }
  int num_tx =
      rte_ring_sp_enqueue_burst(this->tx_ring_, reinterpret_cast<void**>(packets.data()), burst_size);
  return num_tx;
}
