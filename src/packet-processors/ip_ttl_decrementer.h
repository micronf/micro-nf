#ifndef IP_TTL_DECREMENTER_H_
#define IP_TTL_DECREMENTER_H_

#include "../port/rte_egress_port.h"
#include "../port/rte_ingress_port.h"
#include "packet_processor.h"

class IPTtlDecrementer : public PacketProcessor {
 public:
  IPTtlDecrementer() {}
  void Init(const PacketProcessorConfig& pp_config) override;
  void Run() override;
  void FlushState() override;
  void RecoverState() override;
  virtual ~IPTtlDecrementer() {}
};

#endif  // IP_TTL_DECREMENTER_H_
