#include "branch_egress_port_zc.h"

#include <rte_mbuf.h>
#include <rte_memcpy.h>

BranchEgressPortZC::BranchEgressPortZC() { this->num_branches_ = 0; }

BranchEgressPortZC::BranchEgressPortZC(
    int num_branches, const std::vector<std::string> &ring_ids) {
  this->num_branches_ = num_branches;
  this->num_bitmap_entries_ = num_branches >> 3 + (num_branches & 8) ? 1 : 0;
  for (auto &ring_id : ring_ids) {
    this->tx_rings_.push_back(rte_ring_lookup(ring_id.c_str()));
  }
}

void BranchEgressPortZC::Init(std::map<std::string, std::string> &port_config,
              PacketProcessor* owner_pp) {

  this->stat_mz = rte_memzone_lookup(MZ_STAT);
  this->micronf_stats = (MSStats*) this->stat_mz->addr;
  this->owner_packet_processor_ = owner_pp;

}

inline int BranchEgressPortZC::TxBurst(tx_pkt_array_t &packets, uint16_t burst_size) {
  // Embed bitmap within the metadata area of each mbuf. i.e., set
  // kNumBitmapEntries bytes to 0.
  for (int i = 0; i < burst_size; ++i) {
    char *mdata_ptr = reinterpret_cast<char *>(
        reinterpret_cast<unsigned long>(packets[i]) + sizeof(struct rte_mbuf));
    memset(mdata_ptr, 0, this->num_bitmap_entries_);
  }
  for (auto ring_ptr : this->tx_rings_) {
    //rte_ring_sp_enqueue_burst(
    //    ring_ptr, reinterpret_cast<void **>(packets.data()), packets.size());
		int num_tx = rte_ring_enqueue_burst(
      ring_ptr, reinterpret_cast<void **>(packets.data()), burst_size);

		if(num_tx == -ENOBUFS){
			this->micronf_stats->packet_drop[owner_packet_processor_->instance_id()] += burst_size;
			for(int i=0; i < burst_size; i++){
				rte_pktmbuf_free(packets[i]);
			}
		}
		else if(num_tx == -EDQUOT){
			// high watermark exceeded
		}


  }
  // TODO: Not sure what exactly to return :/.
  return 0;
}
