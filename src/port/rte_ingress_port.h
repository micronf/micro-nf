#ifndef _RTE_INGRESS_PORT_H_
#define _RTE_INGRESS_PORT_H_

#include "port.h"

#include <string>

class RteIngressPort : public IngressPort {
public:
  RteIngressPort() : rx_ring_(nullptr) {}
  RteIngressPort(const unsigned int port_id, const std::string &ring_id);
  int RxBurst(rx_pkt_array_t &packets) override;
  virtual ~RteIngressPort() {}

private:
  rte_ring *rx_ring_;
};

#endif // _RTE_INGRESS_PORT_H_
