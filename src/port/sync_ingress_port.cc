#include "sync_ingress_port.h"

#include <rte_malloc.h>

#include <cstring>

SyncIngressPort::SyncIngressPort(int num_prev_ms, const std::string& ring_id) {
  this->num_prev_ms_ = num_prev_ms;
  this->num_bitmap_entries_ =
      (num_prev_ms >> 3) + static_cast<int>((num_prev_ms & 8) != 0);
  this->mask_ = std::unique_ptr<unsigned char>(reinterpret_cast<unsigned char *>(
      rte_zmalloc(NULL, this->num_bitmap_entries_, 0)));
  memset(mask_.get(), 0xFF, this->num_bitmap_entries_);
  mask_.get()[this->num_bitmap_entries_ - 1] &=
      static_cast<unsigned char>(~(0xFF << (num_prev_ms & 8)));
  this->rx_ring_ = rte_ring_lookup(ring_id.c_str());
}

inline int SyncIngressPort::RxBurst(rx_pkt_array_t &packets) {
  // First check if there are packets that can be pulled from pending mbuf
  // queue.
  int num_rx = 0, i = 0;
  while (!pending_mbuf_q_.empty()) {
    auto mbuf = pending_mbuf_q_.front();
    char *mdata_ptr = MDATA_PTR(mbuf);
    if (likely(
            !std::memcmp(mdata_ptr, this->mask_.get(), this->num_bitmap_entries_))) {
      packets[num_rx++] = mbuf;
      pending_mbuf_q_.pop();
    } else break;
  }

  // If the number of packets pulled from the pending queue is equal to RX_BURST_SIZE return.
  if (unlikely(num_rx == RX_BURST_SIZE))
    return num_rx;

  // If there are still packets left in pending_mbuf_q_ then return.
  if (unlikely(!this->pending_mbuf_q_.empty()))
    return num_rx;

  // If there is room for more packets and the pending_mbuf_q_ is empty, then
  // pull from the ring.
  rx_pkt_array_t rx_packets;
  int num_rx_from_ring = rte_ring_sc_dequeue_burst(
      this->rx_ring_, reinterpret_cast<void **>(rx_packets.data()),
      RX_BURST_SIZE - num_rx);

  // Check if more packets can be returned, i.e., they are ready. If there are
  // packets that are not ready push them to pending_mbuf_q_.
  for (i = 0; i < num_rx_from_ring; ++i) {
    char *mdata_ptr = MDATA_PTR(rx_packets[i]);
    if (likely(
          !std::memcmp(mdata_ptr, this->mask_.get(), this->num_bitmap_entries_))) {
      packets[num_rx++] = rx_packets[i];
    } else break;
  }

  // Push the rest of the packets in pending_mbuf_q_.
  for (; i < num_rx_from_ring; ++i) {
    this->pending_mbuf_q_.push(rx_packets[i]);
  }
  return num_rx;
}

inline int SyncIngressPort::CountNumComplete(const char *mdata_ptr) {
  const char kOneBitsInByte[] = {0, 1, 1, 2, 1, 2, 2, 3};
  int i = 0, completed = 0;
  char x = 0;
  while (i < this->num_bitmap_entries_) {
    x = mdata_ptr[i];
    // The following magic counts the number of set bits in a byte.
    x = x - ((x >> 1) & 0x55);
    x = (x & 0x33333333) + ((x >> 2) & 0x33);
    x = (x + (x >> 4)) & 0x0F;
    completed += (x & 0x3F);
  }
  return completed;
}