#ifndef _DROP_PACKET_H_
#define _DROP_PACKET_H_

#include "packet_processor.h"

class DropPacket : public PacketProcessor {
  public:
    DropPacket() {}
    void Init(const PacketProcessorConfig& pp_config) override;
    void Run() override;
    void FlushState() override;
    void RecoverState() override;
    virtual ~DropPacket() {}
};

#endif // _DROP_PACKET_H_
