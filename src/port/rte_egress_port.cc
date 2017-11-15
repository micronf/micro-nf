#include "rte_egress_port.h"

void RteEgressPort::Init(std::map<std::string, std::string> &port_config,
											PacketProcessor* owner_pp) {
  this->tx_ring_ = 
      rte_ring_lookup(port_config[EgressPort::kConfRingId].c_str());
  assert(this->tx_ring_ != nullptr);
  this->port_id_ = std::stoi(port_config[EgressPort::kConfPortId]);
	this->stat_mz = rte_memzone_lookup(MZ_STAT);
  this->micronf_stats = (MSStats*) this->stat_mz->addr;

	this->owner_packet_processor_ = owner_pp;
}

inline int RteEgressPort::TxBurst(tx_pkt_array_t &packets, uint16_t burst_size) {
	// This function calls the multi-producer or the single-producer version 
	// depending on the default behavior that was specified at ring creation time.
  int num_tx = rte_ring_enqueue_burst(
        this->tx_ring_, reinterpret_cast<void **>(packets.data()), burst_size, NULL );

  if(unlikely(num_tx < burst_size)){
    this->micronf_stats->packet_drop[owner_packet_processor_->instance_id()][this->port_id_] += 
			burst_size - num_tx; 
    for(int i= num_tx; i < burst_size; i++){
      rte_pktmbuf_free(packets[i]);
    }
  }
	
  return num_tx;
}
