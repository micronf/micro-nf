#ifndef _PACKET_PROCESSOR_CONFIG_H_
#define _PACKET_PROCESSOR_CONFIG_H_

#include <string>
#include <vector>

class PacketProcessorConfig {
  public:
    inline const std::vector<std::string>& ingress_queue_ids () const {
      return this->ingress_queue_ids_;
    }
    inline const std::vector<std::string>& egress_queue_ids () const {
      return this->egress_queue_ids_;
    }
  private:
    std::vector<std::string> ingress_queue_ids_;
    std::vector<std::string> egress_queue_ids_;
};
#endif // _PACKET_PROCESSOR_CONFIG_H_
