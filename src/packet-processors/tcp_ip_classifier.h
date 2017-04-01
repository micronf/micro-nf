#ifndef _TCP_IP_CLASSIFIER_H_
#define _TCP_IP_CLASSIFIER_H_

#include "../util/acl_rule.h"
#include "packet_processor.h"

class TCPIPClassifier : public PacketProcessor {
 public:
  void Init(const PacketProcessorConfig& pp_config) override;
  void Run() override;
  void FlushState() override;
  void RecoverState() override;
  void AddRule(uint16_t table_idx, const ACLRule& acl_rule) { acl_rules_[table_idx].push_back(acl_rule); }

 private:
  std::vector<std::vector<ACLRule>> acl_rules_;
  std::vector<uint16_t> deny_port_index_;
  std::vector<uint16_t> allow_port_index_;
};

#endif  // _TCP_IP_CLASSIFIER_H_
