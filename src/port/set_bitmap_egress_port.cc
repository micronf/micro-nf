#include "set_bitmap_egress_port.h"

#include <rte_mbuf.h>

SetBitmapEgressPort::SetBitmapEgressPort() {
  this->port_id_ = -1;
  this->bitmap_index_ = -1;
  this->bitmap_offset_ = -1;
}

SetBitmapEgressPort::SetBitmapEgressPort(int port_id,
                                         const std::string &ring_id) {
  this->port_id_ = port_id;
  this->bitmap_index_ = port_id >> 3;
  this->bitmap_offset_ = port_id & 8;
}

int SetBitmapEgressPort::TxBurst(void **packets, int burst_size) {
  struct rte_mbuf **tx_mbufs = reinterpret_cast<rte_mbuf **>(packets);
  for (int i = 0; i < burst_size; ++i) {
    char *mdata_ptr = reinterpret_cast<char *>(
        reinterpret_cast<unsigned long>(tx_mbufs[i]) + sizeof(struct rte_mbuf));
    mdata_ptr[this->bitmap_index_] |= (1 << this->bitmap_offset_);
  }
  return 0;
}
