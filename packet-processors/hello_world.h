#ifndef _HELLO_WORLD_H_
#define _HELLO_WORLD_H_

#include "../common/packet_processor.h"

class HelloWorld : public PacketProcessor <NullIngressPort, NullEgressPort> {
  public:
    HelloWorld() {};
    virtual void Init(const PacketProcessorConfig& config);
    virtual void Run();
    virtual void FlushState();
    virtual void RecoverState();
    virtual ~HelloWorld() {}
};

#endif // _HELLO_WORLD_H_
