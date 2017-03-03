#ifndef _RTE_NIC_INGRESS_PORT_H_
#define _RTE_NIC_INGRESS_PORT_H_

#include "port.h"

#include <string>

class RteNICIngressPort : public IngressPort {
 public:
  RteNICIngressPort() {}
  void Init(std::map<std::string, std::string> &port_config) override;
  int RxBurst(rx_pkt_array_t &packets) override;
  virtual ~RteNICIngressPort() {}
  static const std::string kConfNICPortId;
  static const std::string kConfNICQueueId;

 private:
  uint8_t nic_port_id_;
  uint8_t nic_queue_id_;
};

#endif  // _RTE_NIC_INGRESS_PORT_H_
