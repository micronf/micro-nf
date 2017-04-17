#ifndef _SLEEPY_H_
#define _SLEEPY_H_

#include <string>
#include "packet_processor.h"

class Sleepy : public PacketProcessor {
 public:
  Sleepy() : sleep_duration_us_(0) { }
  void Init(const PacketProcessorConfig& pp_config) override;
  void Run() override;
  void FlushState() override;
  void RecoverState() override;
  static const std::string kConfSleepDurationUs;

 private:
  uint64_t sleep_duration_us_;
};
#endif  // _SLEEPY_H_
