#ifndef _RTE_EGRESS_PORT_H_
#define _RTE_EGRESS_PORT_H_

#include "port.h"

#include <string>

class RteEgressPort : public EgressPort {
public:
  RteEgressPort() : tx_ring_(nullptr) {}
  RteEgressPort(const unsigned int port_id, const std::string &ring_id);
  int TxBurst(tx_pkt_array_t &packets) override;

private:
  rte_ring *tx_ring_;
};
#endif // _RTE_EGRESS_PORT_H_
