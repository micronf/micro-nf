#include "lb_egress_port.h"

const std::string LBEgressPort::kConfNumDestinationMs = "num_destination_ms";

inline void LBEgressPort::Init(std::map<std::string, std::string>& port_config,
                               PacketProcessor* owner_pp) {
  this->num_destination_ms_ =
      std::stoi(port_config[LBEgressPort::kConfNumDestinationMs]);
  for (int i = 0; i < this->num_destination_ms_; ++i) {
    auto& ring_name = port_config[std::to_string(i)];
    this->tx_rings_.push_back(rte_ring_lookup(ring_name.c_str()));
  }
}

inline int LBEgressPort::TxBurst(tx_pkt_array_t& packets, uint16_t burst_size) {
  uint16_t burst_per_ms = burst_size / num_destination_ms_;
  uint16_t packets_remaining = burst_size;
  register uint16_t i = 0;
  uint16_t num_tx = 0;
  for (i = 0; i < num_destination_ms_; ++i, packets_remaining -= burst_per_ms) {
    if (unlikely(packets_remaining < burst_per_ms))
      burst_per_ms = packets_remaining;
    num_tx += rte_ring_enqueue_burst(
        this->tx_rings_[i], reinterpret_cast<void**>(packets.data() + num_tx),
        burst_per_ms);
  }
  micronf_stats->packet_drop[owner_packet_processor_->instance_id()] +=
      burst_size - num_tx;
  for (i = num_tx; i < burst_size; ++i) rte_pktmbuf_free(packets[i]);
  return num_tx;
}

inline void LBEgressPort::AddDestinationMS(const std::string& ring_name) {
  this->tx_rings_.push_back(rte_ring_lookup(ring_name.c_str()));
  ++this->num_destination_ms_;
}
