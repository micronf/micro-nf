#ifndef _RTE_EGRESS_PORT_H_
#define _RTE_EGRESS_PORT_H_

#include "egress_port.h"

#include <rte_mbuf.h>
#include <rte_ring.h>

#include <string>

class RteEgressPort : public EgressPort {
 public:
  RteEgressPort() : tx_ring_(nullptr) {}
  RteEgressPort(const std::string& queue_id, const unsigned int& port_id);
  int TxBurst(void** packets, int burst_size) override;

 private:
  rte_ring* tx_ring_;
  unsigned int port_id_;
};
#endif  // _RTE_EGRESS_PORT_H_
