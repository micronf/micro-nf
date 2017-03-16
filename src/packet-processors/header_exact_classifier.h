#ifndef _HEADER_EXACT_CLASSIFIER_H_
#define _HEADER_EXACT_CLASSIFIER_H_

#include "../common/packet_processor.h"

class HeaderExactClassifier : public PacketProcessor {
 public:
  void Init() {}
  void Run() {}
  void FlushState() {}
  void RecoverState() {}
};

#endif  // _HEADER_EXACT_CLASSIFIER_H_
