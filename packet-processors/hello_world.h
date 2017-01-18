#ifndef _HELLO_WORLD_H_
#define _HELLO_WORLD_H_

#include "../common/packet_processor.h"

class HelloWorld : public PacketProcessor <NullIngressPort, NullEgressPort> {
  public:
    void HelloWorld() {};
    void Init(const PacketProcessorConfig& config) override;
    void Run() override;
    void FlushState() override;
    void RecoverState() override;
}

#endif // _HELLO_WORLD_H_
