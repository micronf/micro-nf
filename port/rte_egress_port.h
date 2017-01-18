#ifndef _RTE_EGRESS_PORT_H_
#define _RTE_EGRESS_PORT_H_

#include "egress_port.h"

#include <rte_mbuf.h>
#include <rte_ring.h>

#include <string>

class RteEgressPort : public EgressPort {
  public:
    RteEgressPort() : tx_ring_(nullptr) {}
    RteEgressPort(const std::string& queue_id);
    uint32_t TxBurst(struct mbuf** packets, uint32_t burst_size) override;
  private:
    rte_ring* tx_ring_;
};
#endif // _RTE_EGRESS_PORT_H_
