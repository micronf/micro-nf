#ifndef _BRANCH_EGRESS_PORT_ZC_H_
#define _BRANCH_EGRESS_PORT_ZC_H_

#include "egress_port.h"
#include "packet-processors/packet_processor.h"
#include <string>
#include <vector>

class BranchEgressPortZC : public EgressPort {
 public:
  BranchEgressPortZC() : num_branches_(0) {};
  void Init(std::map<std::string, std::string> &port_config,
              PacketProcessor* owner_pp) override;
  int TxBurst(tx_pkt_array_t &packets, uint16_t burst_size) override;
  virtual ~BranchEgressPortZC() {}

  static const std::string kConfNumBranches;
 private:
  // Number of microservices this egress port is connected to.
  int num_branches_;

  // Number of entries required in a char array to represent the bitmap.
  unsigned int num_bitmap_entries_;

  // Vector of rte_rings shared with the ingress ports of next stage of
  // microservices.
  std::vector<rte_ring *> tx_rings_;
};
#endif  // _BRANCH_EGRESS_PORT_ZC_H_
