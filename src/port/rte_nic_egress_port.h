#ifndef _RTE_NIC_EGRESS_PORT_H_
#define _RTE_NIC_EGRESS_PORT_H_

#include "port.h"

#include <string>

class RteNICEgressPort : public EgressPort {
 public:
  RteNICEgressPort() {}
  void Init(std::map<std::string, std::string>& port_config) override;
  int TxBurst(tx_pkt_array_t& packets) override;
  virtual ~RteNICEgressPort() {}
  static const std::string kConfNICPortId;
  static const std::string kConfNICQueueId;

 private:
  uint8_t nic_port_id_;
  uint8_t nic_queue_id_;
};

#endif  // _RTE_NIC_EGRESS_PORT_H_
