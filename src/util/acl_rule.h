#ifndef _ACL_RULE_H_
#define _ACL_RULE_H_

#include "cidr_address.h"

class ACLRule {
  public:
    ACLRule() {}
    ACLRule(const CIDRAddress& src_addr, const CIDRAddress& dst_addr, uint16_t src_port, uint16_t dst_port, bool deny):
      src_addr_(src_addr), dst_addr_(dst_addr), src_port_(src_port), dst_port_(dst_port), deny_(deny) { }
    bool Match(uint32_t src_ip, uint32_t dst_ip, uint16_t src_port, uint16_t dst_port) {
      return src_addr_.Match(src_ip) && dst_addr_.Match(dst_ip) && src_port == src_port_ && dst_port == dst_port_;
    }
    bool deny() const { return deny_; }
  private:
    CIDRAddress src_addr_;
    CIDRAddress dst_addr_;
    uint16_t src_port_;
    uint16_t dst_port_;
    bool deny_;
};
#endif // _ACL_RULE_H_
