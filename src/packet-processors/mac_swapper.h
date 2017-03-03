#ifndef _MAC_SWAPPER_H_
#define _MAC_SWAPPER_

#include "packet_processor.h"
#include "../port/rte_egress_port.h"
#include "../port/rte_ingress_port.h"

class MacSwapper : public PacketProcessor {
public:
  MacSwapper() {}
  void Init(const PacketProcessorConfig &config) override;
  void Run() override;
  void FlushState() override;
  void RecoverState() override;
  virtual ~MacSwapper() {}
};

#endif // _MAC_SWAPPER_H_
