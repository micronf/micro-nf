#ifndef _PACKET_PROCESSOR_CONFIG_H_
#define _PACKET_PROCESSOR_CONFIG_H_

#include <string>
#include <vector>

class PacketProcessorConfig {
  public:
    inline void add_ingress_queue_id(const std::string& s) {
      this->ingress_queue_ids_.push_back(s);
    }
    inline const std::vector<std::string>& ingress_queue_ids () const {
      return this->ingress_queue_ids_;
    }
    inline void add_egress_queue_id(const std::string& s) {
      this->egress_queue_ids_.push_back(s);
    }
    inline const std::vector<std::string>& egress_queue_ids () const {
      return this->egress_queue_ids_;
    }
  private:
    std::vector<std::string> ingress_queue_ids_;
    std::vector<std::string> egress_queue_ids_;
};
#endif // _PACKET_PROCESSOR_CONFIG_H_
