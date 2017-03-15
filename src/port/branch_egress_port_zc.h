#ifndef _BRANCH_EGRESS_PORT_ZC_H_
#define _BRANCH_EGRESS_PORT_ZC_H_

#include "egress_port.h"

#include <string>
#include <vector>

class BranchEgressPortZC : public EgressPort {
 public:
  BranchEgressPortZC();
  BranchEgressPortZC(int num_branches,
                     const std::vector<std::string> &ring_ids);
  void Init(std::map<std::string, std::string>& port_config) override;
  int TxBurst(tx_pkt_array_t &packets) override;

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
