#ifndef _PACKET_PROCESSOR_H_
#define _PACKET_PROCESSOR_H_

#include <vector>

namespace micronf {
  template <class IngressPortType, class EgressPortType>
  class PacketProcessor {
    public:
      virtual void Init() = 0;
      virtual void Run() = 0;
      virtual void FlushState() = 0;
      virtual void RecoverState() = 0;
      const std::string& instance_id() const { return instance_id_; }
      virtual ~PacketProcessor();
    protected:
      std::string instance_id_;
      std::vector<std::unique_ptr<IngressPortType> > ingress_ports_;
      std::vector<std::unique_ptr<EgressPortType> > egress_ports_;
  };
}
#endif // _PACKET_PROCESSOR_H_
