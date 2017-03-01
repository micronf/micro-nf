#ifndef _RTE_INGRESS_PORT_H_
#define _RTE_INGRESS_PORT_H_

#include "port.h"

#include <string>

class RteIngressPort : public IngressPort {
public:
  RteIngressPort() : rx_ring_(nullptr) {}
  void Init(std::map<std::string, std::string>& port_config) override;
  int RxBurst(rx_pkt_array_t &packets) override;
  virtual ~RteIngressPort() {}

private:
  rte_ring *rx_ring_;
};

#endif // _RTE_INGRESS_PORT_H_
