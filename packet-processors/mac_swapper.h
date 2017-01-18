#ifndef _MAC_SWAPPER_H_
#define _MAC_SWAPPER_

#include "../common/packet_processor.h"
#include "../port/rte_ingress_port.h"
#include "../port/rte_egress_port.h"

class MacSwapper : public PacketProcessor <RteIngressPort, RteEgressPort> {
  public:
    MacSwapper() {}
    virtual void Init(PacketProcessorConfig& config);
    virtual void Run();
    virtual void FlushState();
    virtual void RecoverState();
    virtual ~MacSwapper();
}

#endif // _MAC_SWAPPER_H_
