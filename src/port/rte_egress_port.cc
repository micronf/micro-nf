#include "rte_egress_port.h"

void RteEgressPort::Init(std::map<std::string, std::string> &port_config,
											PacketProcessor* owner_pp) {
  this->tx_ring_ = 
      rte_ring_lookup(port_config[EgressPort::kConfRingId].c_str());
  this->port_id_ = std::stoi(port_config[EgressPort::kConfPortId]);

	this->stat_mz = rte_memzone_lookup(MZ_STAT);
  this->micronf_stats = (MSStats*) this->stat_mz->addr;

	this->owner_packet_processor_ = owner_pp;
}

inline int RteEgressPort::TxBurst(tx_pkt_array_t &packets, uint16_t burst_size) {
	// This function calls the multi-producer or the single-producer version 
	// depending on the default behavior that was specified at ring creation time 
  int num_tx = rte_ring_enqueue_burst(
      this->tx_ring_, reinterpret_cast<void **>(packets.data()), burst_size);

  if(num_tx == -ENOBUFS){
		int idx = std::stoi(owner_packet_processor_->instance_id());
    this->micronf_stats->packet_drop[idx] += burst_size;
    for(int i=0; i < burst_size; i++){
      rte_pktmbuf_free(packets[i]);
    }
  }
	else if(num_tx == -EDQUOT){
		// high watermark exceeded
	}

  return num_tx;
}
