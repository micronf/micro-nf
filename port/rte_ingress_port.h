#ifndef _RTE_INGRESS_PORT_H_
#define _RTE_INGRESS_PORT_H_

#include "ingress_port.h"

#include <rte_mbuf.h>
#include <rte_ring.h>

class RteIngressPort : public IngressPort {
  public:
    RteIngressPort() : rx_ring_(nullptr) {}
    RteIngressPort(const std::string& ring_id);
    inline struct rte_mbuf** RxBurst(int burst_size) override;
    virtual ~RteIngressPort() {}
  private:
    rte_ring* rx_ring_;
};

#endif // _RTE_INGRESS_PORT_H_
