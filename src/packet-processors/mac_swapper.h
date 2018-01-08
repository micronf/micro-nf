#ifndef _MAC_SWAPPER_H_
#define _MAC_SWAPPER_H_

#include "../port/rte_egress_port.h"
#include "../port/rte_ingress_port.h"
#include "packet_processor.h"

class MacSwapper : public PacketProcessor {
 public:
  MacSwapper() {}
  void Init(const PacketProcessorConfig& pp_config) override;
  void Run() override;
  void FlushState() override;
  void RecoverState() override;
  virtual ~MacSwapper() {}
};

#endif  // _MAC_SWAPPER_H_
