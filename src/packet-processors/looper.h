#ifndef _LOOPER_H_
#define _LOOPER_H_

#include "packet_processor.h"
#include <string>

class Looper : public PacketProcessor {
  public:
    Looper() {}
    void Init(const PacketProcessorConfig& pp_config) override;
    void Run() override;
    void FlushState() override;
    void RecoverState() override;
    virtual ~Looper() {}
    static const std::string kConfLoopCounter;
  private:
    uint32_t loop_counter_;
};

#endif // _LOOPER_H_
