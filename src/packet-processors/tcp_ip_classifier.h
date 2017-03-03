#ifndef _TCP_IP_CLASSIFIER_H_
#define _TCP_IP_CLASSIFIER_H_

#include "../util/acl_rule.h"
#include "packet_processor.h"

class TCPIPClassifier : public PacketProcessor {
  public:
    void Init(const PacketProcessorConfig& config) override;
    void Run() override;
    void FlushState() override;
    void RecoverState() override;
    void AddRule(const ACLRule& acl_rule) {
      acl_rules_.push_back(acl_rule);
    }
  private:
    std::vector<ACLRule> acl_rules_;
    int deny_port_index_;
    int allow_port_index_;
};

#endif // _TCP_IP_CLASSIFIER_H_
