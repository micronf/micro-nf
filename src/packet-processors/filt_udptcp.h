#ifndef _FILT_UDPTCP_H
#define _FILT_UDPTCP_H

#include "packet_processor.h"


class FiltUDPTCP : public PacketProcessor {
  public:
    FiltUDPTCP() {}
    void Init(const PacketProcessorConfig& config) override;
    void Run()          override;
    void FlushState()   override;
    void RecoverState() override;
    
};


#endif // _FILT_UDPTCP_H
