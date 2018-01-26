#include "branch_egress_port_zc.h"

#include <rte_mbuf.h>
#include <rte_memcpy.h>
#include <algorithm>

const std::string BranchEgressPortZC::kConfNumBranches = "num_branches";

void BranchEgressPortZC::Init(std::map<std::string, std::string> &port_config,
                              PacketProcessor *owner_pp) {
  this->port_id_ = std::stoi(port_config[EgressPort::kConfPortId]);
  this->num_branches_ =
      std::stoi(port_config[BranchEgressPortZC::kConfNumBranches]);
  for (int i = 0; i < this->num_branches_; ++i) {
    rte_ring *ring = rte_ring_lookup(port_config[std::to_string(i)].c_str());
    assert(ring != nullptr);
    this->tx_rings_.push_back(ring);
  }
  this->stat_mz = rte_memzone_lookup(MZ_STAT);
  this->micronf_stats = (MSStats *)this->stat_mz->addr;
  this->owner_packet_processor_ = owner_pp;
}

inline int BranchEgressPortZC::TxBurst(tx_pkt_array_t &packets,
                                       uint16_t burst_size) {
  register uint16_t i = 0;
  uint16_t num_tx = 0;
  // Embed a 16-bit counter within the metadata area of each mbuf. Initialize
  // the counter to zero.
  for (i = 0; i < burst_size; ++i) {
    char *mdata_ptr = MDATA_PTR(packets[i]);
    memset(mdata_ptr, 0, sizeof(uint16_t));
  }

  // Determine the number of free spaces available in the next stage of rings.
  uint16_t min_sz = static_cast<uint16_t>(~(1 << 15));
  uint16_t ring_sz = 0;
  for (auto ring_ptr : this->tx_rings_) {
    ring_sz = rte_ring_free_count(ring_ptr);
    min_sz = std::min(min_sz, ring_sz);
  }

  auto final_burst_size = std::min(min_sz, burst_size);

  for (auto ring_ptr : this->tx_rings_) {
    num_tx = rte_ring_enqueue_burst(ring_ptr,
                                    reinterpret_cast<void **>(packets.data()),
                                    final_burst_size, NULL);
  }

  // if( unlikely( ( num_tx < burst_size ) ) ) {
  //    this->micronf_stats->packet_drop[owner_packet_processor_->instance_id()][this->port_id_]
  //    +=
  //       ( burst_size - num_tx );
  // }

  // Drop the mbufs that could not be pushed to the next stage.
  for (i = num_tx; i < burst_size; ++i) rte_pktmbuf_free(packets[i]);

  return num_tx;
}
