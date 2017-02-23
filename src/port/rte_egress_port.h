#ifndef _RTE_EGRESS_PORT_H_
#define _RTE_EGRESS_PORT_H_

#include "port.h"

#include <string>

class RteEgressPort : public EgressPort {
public:
  RteEgressPort() : tx_ring_(nullptr) {}
  RteEgressPort(const unsigned int port_id, const std::string &queue_id);
  int TxBurst(std::array<struct rte_mbuf *, TX_BURST_SIZE> &packets) override;

private:
  rte_ring *tx_ring_;
};
#endif // _RTE_EGRESS_PORT_H_
