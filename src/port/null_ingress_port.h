#ifndef _NULL_INGRESS_PORT_H_
#define _NULL_INGRESS_PORT_H_

#include "ingress_port.h"

class NullIngressPort : public IngressPort {
 public:
  NullIngressPort() {}
  void Init(std::map<std::string, std::string>& port_config) override;
  int RxBurst(rx_pkt_array_t& packets) override;
  virtual ~NullIngressPort() {}
};
#endif  // _NULL_INGRESS_PORT_H_
