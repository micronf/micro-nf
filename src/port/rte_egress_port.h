#ifndef _RTE_EGRESS_PORT_H_
#define _RTE_EGRESS_PORT_H_

#include "port.h"

#include <string>

class RteEgressPort : public EgressPort {
 public:
  RteEgressPort() : tx_ring_(nullptr) {}
  void Init(std::map<std::string, std::string> &port_config,
							const PacketProcessor* owner_pp) override;
  int TxBurst(tx_pkt_array_t &packets, uint16_t burst_size) override;
  virtual ~RteEgressPort() {}

 private:
  rte_ring *tx_ring_;
};
#endif  // _RTE_EGRESS_PORT_H_
