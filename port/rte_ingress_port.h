#ifndef _RTE_INGRESS_PORT_H_
#define _RTE_INGRESS_PORT_H_

#include "ingress_port.h"

#include <rte_mbuf.h>
#include <rte_ring.h>
#include <string>

class RteIngressPort : public IngressPort {
 public:
  RteIngressPort() : rx_ring_(nullptr) {}
  RteIngressPort(const std::string& ring_id, const unsigned int& port_id);
  void** RxBurst(int burst_size);
  virtual ~RteIngressPort() {}

 private:
  rte_ring* rx_ring_;
  unsigned int port_id_;
};

#endif  // _RTE_INGRESS_PORT_H_
