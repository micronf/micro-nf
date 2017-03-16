#ifndef _NULL_EGRESS_PORT_H_
#define _NULL_EGRESS_PORT_H_

#include "port.h"

class NullEgressPort : public EgressPort {
 public:
  NullEgressPort() {}
  void Init(std::map<std::string, std::string>& port_config) override;
  int TxBurst(std::array<struct rte_mbuf*, TX_BURST_SIZE>& packets) override;
  virtual ~NullEgressPort() {}
};
#endif  // _NULL_EGRESS_PORT_H_
