#ifndef _RTE_EGRESS_PORT_H_
#define _RTE_EGRESS_PORT_H_

#include "port.h"

#include <string>

class RteEgressPort : public EgressPort {
public:
  RteEgressPort() : tx_ring_(nullptr) {}
  void Init(std::map<std::string, std::string>& port_config) override;
  int TxBurst(tx_pkt_array_t &packets) override;

private:
  rte_ring *tx_ring_;
};
#endif // _RTE_EGRESS_PORT_H_
