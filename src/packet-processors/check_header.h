#ifndef _CHECK_HEADER_H_
#define _CHECK_HEADER_H_

#include "../port/rte_egress_port.h"
#include "../port/rte_ingress_port.h"
#include "packet_processor.h"

class CheckHeader : public PacketProcessor {
  public:
    CheckHeader() {}
    void Init(const PacketProcessorConfig& pp_config) override;
    void Run() override;
    void FlushState() override;
    void RecoverState() override;
    virtual ~CheckHeader() {}
};

#endif // _CHECK_HEADER_H_
