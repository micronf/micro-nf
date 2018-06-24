#ifndef _IMITATOR_PKTSIZE_H_
#define _IMITATOR_PKTSIZE_H_

#include "../port/rte_egress_port.h"
#include "../port/rte_ingress_port.h"
#include "packet_processor.h"

class ImitatorPktsize : public PacketProcessor {
 public:
  ImitatorPktsize() {}
  void Init(const PacketProcessorConfig& pp_config) override;
  void Run() override;
  void FlushState() override;
  void RecoverState() override;
  virtual ~ImitatorPktsize() {}
};

#endif
