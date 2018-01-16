// Author: Anthony Anthony.

#ifndef PROCESSORS_MAC_REWRITER_H
#define PROCESSORS_MAC_REWRITER_H

#include "../port/rte_egress_port.h"
#include "../port/rte_ingress_port.h"
#include "packet_processor.h"

class MacRewriter : public PacketProcessor {
 public:
  MacRewriter() {}
  void Init(const PacketProcessorConfig& pp_config) override;
  void Run() override;
  void FlushState() override;
  void RecoverState() override;
  virtual ~MacRewriter() {}

 private:
  std::string dest_mac_ = "";
  bool has_dest_mac_ = false;
  
};


#endif // PROCESSORS_MAC_REWRITER_H
