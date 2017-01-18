#ifndef _PACKET_PROCESSOR_CONFIG_H_
#define _PACKET_PROCESSOR_CONFIG_H_

class PacketProcessorConfig {
  public:
    inline std::vector<std::string>& ingress_queue_ids_ () const {
      return this->ingress_queue_ids_;
    }
    inline std::vector<std::string>& egress_queue_ids_ () const {
      return this->egress_queue_ids_;
    }
  private:
    std::vector<std::string> ingress_queue_ids_;
    std::vector<std::string> egress_queue_ids_;
};
#endif // _PACKET_PROCESSOR_CONFIG_H_
