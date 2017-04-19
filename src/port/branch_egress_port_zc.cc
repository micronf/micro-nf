#include "branch_egress_port_zc.h"

#include <rte_mbuf.h>
#include <rte_memcpy.h>

const std::string BranchEgressPortZC::kConfNumBranches = "num_branches";

void BranchEgressPortZC::Init(std::map<std::string, std::string> &port_config,
              PacketProcessor* owner_pp){ 
	this->port_id_ = std::stoi(port_config[EgressPort::kConfPortId]);
  this->num_branches_ = std::stoi(port_config[BranchEgressPortZC::kConfNumBranches]);
  this->num_bitmap_entries_ = num_branches_ >> 3 + (num_branches_ & 8) ? 1 : 0;
  for (int i = 0; i < num_branches_; ++i) {
    rte_ring* ring = rte_ring_lookup(port_config[std::to_string(i)].c_str());
    assert(ring != nullptr);
    this->tx_rings_.push_back(ring);
  }
  this->stat_mz = rte_memzone_lookup(MZ_STAT);
  this->micronf_stats = (MSStats*) this->stat_mz->addr;
  this->owner_packet_processor_ = owner_pp;

}

inline int BranchEgressPortZC::TxBurst(tx_pkt_array_t &packets, uint16_t burst_size) {
  register uint16_t i = 0;
  uint16_t num_tx = 0;
  // Embed bitmap within the metadata area of each mbuf. i.e., set
  // kNumBitmapEntries bytes to 0.
  for (i = 0; i < burst_size; ++i) {
    char *mdata_ptr = reinterpret_cast<char *>(
        reinterpret_cast<unsigned long>(packets[i]) + sizeof(struct rte_mbuf));
    memset(mdata_ptr, 0, this->num_bitmap_entries_);
  }
  for (auto ring_ptr : this->tx_rings_) {
		num_tx = rte_ring_enqueue_burst(
      ring_ptr, reinterpret_cast<void **>(packets.data()), burst_size);
    this->micronf_stats->packet_drop[owner_packet_processor_->instance_id()][this->port_id_] += 
      (burst_size - num_tx);
		for(i = num_tx; i < burst_size; ++i) rte_pktmbuf_free(packets[i]);
  }
  // TODO: Not sure what exactly to return :/.
  return 0;
}
