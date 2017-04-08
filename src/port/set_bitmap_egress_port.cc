#include "set_bitmap_egress_port.h"
#include <rte_mbuf.h>

void SetBitmapEgressPort::Init(
    std::map<std::string, std::string> &port_config,
              PacketProcessor* owner_pp) {
  this->port_id_ = std::stoi(port_config[EgressPort::kConfPortId]);
  this->bitmap_index_ = this->port_id_ >> 3;
  this->bitmap_offset_ = this->port_id_ & 8;
}

int SetBitmapEgressPort::TxBurst(tx_pkt_array_t &packets, uint16_t burst_size) {
  for (int i = 0; i < burst_size; ++i) {
    char *mdata_ptr = MDATA_PTR(packets[i]);
    mdata_ptr[this->bitmap_index_] |= (1 << this->bitmap_offset_);
  }
  return 0;
}
