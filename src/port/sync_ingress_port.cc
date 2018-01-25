#include "sync_ingress_port.h"

#include <assert.h>
#include <rte_malloc.h>
#include <stdlib.h>
#include <string.h>

const std::string SyncIngressPort::kConfNumPrevMs = "num_prev_ms";

void SyncIngressPort::Init(std::map<std::string, std::string> &port_config) {
  this->num_prev_ms_ = std::stoi(port_config[SyncIngressPort::kConfNumPrevMs]);
  this->rx_ring_ =
      rte_ring_lookup(port_config[IngressPort::kConfRingId].c_str());
  assert(this->rx_ring_ != nullptr);
}

inline int SyncIngressPort::RxBurst(rx_pkt_array_t &packets) {
  int num_rx = 0;
  register uint32_t i = 0;
  // First check if there are packets that can be pulled from pending mbuf
  // queue.
  while (num_rx <= RX_BURST_SIZE && !pending_mbuf_q_.empty()) {
    auto mbuf = pending_mbuf_q_.front();
    char *mdata_ptr = MDATA_PTR(mbuf);
    if (likely(*reinterpret_cast<uint16_t *>(mdata_ptr) ==
               this->num_prev_ms_)) {
      packets[num_rx++] = mbuf;
      pending_mbuf_q_.pop();
    } else
      break;
  }

  // If the number of packets pulled from the pending queue is equal to
  // RX_BURST_SIZE return.
  if (unlikely(num_rx == RX_BURST_SIZE)) return num_rx;

  // If there are still packets left in pending_mbuf_q_ then return.
  if (unlikely(!this->pending_mbuf_q_.empty())) return num_rx;

  // If there is room for more packets and the pending_mbuf_q_ is empty, then
  // pull from the ring.
  rx_pkt_array_t rx_packets;
  int num_rx_from_ring = rte_ring_dequeue_burst(
      this->rx_ring_, reinterpret_cast<void **>(rx_packets.data()),
      RX_BURST_SIZE - num_rx, NULL);

  // Check if more packets can be returned, i.e., they are ready. If there are
  // packets that are not ready push them to pending_mbuf_q_.
  for (i = 0; i < num_rx_from_ring; ++i) {
    char *mdata_ptr = MDATA_PTR(rx_packets[i]);
    if (likely(*reinterpret_cast<uint16_t *>(mdata_ptr) ==
               this->num_prev_ms_)) {
      packets[num_rx++] = rx_packets[i];
    } else
      break;
  }

  // Push the rest of the packets in pending_mbuf_q_.
  for (; i < num_rx_from_ring; ++i) {
    this->pending_mbuf_q_.push(rx_packets[i]);
  }
  return num_rx;
}
