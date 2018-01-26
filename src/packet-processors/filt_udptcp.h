#ifndef _FILT_UDPTCP_H
#define _FILT_UDPTCP_H

#include "packet_processor.h"


class filt_udptcp : public PacketProcessor {
  public:
    filt_udptcp() {}
    void Init(const PacketProcessorConfig& config) override;
    void Run()          override;
    void FlushState()   override;
    void RecoverState() override;
    
};


#endif // _FILT_UDPTCP_H
