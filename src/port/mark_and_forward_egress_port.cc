#include "mark_and_forward_egress_port.h"

#include <rte_mbuf.h>

MarkAndForwardEgressPort::MarkAndForwardEgressPort() {
  this->port_id_ = -1;
  this->bitmap_index_ = this->bitmap_offset_ = -1;
  this->tx_ring_ = nullptr;
}

void MarkAndForwardEgressPort::Init(
    std::map<std::string, std::string>& port_config,
              PacketProcessor* owner_pp) {
  this->port_id_ = std::stoi(port_config[EgressPort::kConfPortId]);
  this->tx_ring_ =
      rte_ring_lookup(port_config[EgressPort::kConfRingId].c_str());
  this->bitmap_index_ = this->port_id_ >> 3;
  this->bitmap_offset_ = this->port_id_ & 8;

  this->stat_mz = rte_memzone_lookup(MZ_STAT);
  this->micronf_stats = (MSStats*) this->stat_mz->addr;
  this->owner_packet_processor_ = owner_pp;
}

int MarkAndForwardEgressPort::TxBurst(tx_pkt_array_t& packets, uint16_t burst_size) {
  for (int i = 0; i < burst_size; ++i) {
    char* mdata_ptr = reinterpret_cast<char*>(
        reinterpret_cast<unsigned long>(packets[i]) + sizeof(struct rte_mbuf));
    mdata_ptr[this->bitmap_index_] |= (1 << this->bitmap_offset_);
  }
	
	int num_tx = rte_ring_enqueue_burst(
      this->tx_ring_, reinterpret_cast<void **>(packets.data()), burst_size);

  if(num_tx == -ENOBUFS){
    this->micronf_stats->packet_drop[owner_packet_processor_->instance_id()] += burst_size;
    for(int i=0; i < burst_size; i++){
      rte_pktmbuf_free(packets[i]);
    }
  }
  else if(num_tx == -EDQUOT){
    // high watermark exceeded
  }
	

  return num_tx;
}
