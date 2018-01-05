#ifndef FILT_IPV4_H
#define FILT_IPV4_H

#include "packet_processor.h"

class filt_ipv4 : public PacketProcessor {
    public:
        filt_ipv4() {}
        void Init(const PacketProcessorConfig& config) override;
        void Run()          override;
        void FlushState()   override;
        void RecoverState() override;
};

#endif
