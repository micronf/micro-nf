#ifndef _PACKET_PROCESSOR_H_
#define _PACKET_PROCESSOR_H_

#include "../port/null_ingress_port.h"
#include "../port/null_egress_port.h"
#include "../port/rte_egress_port.h"
#include "../port/rte_ingress_port.h"
#include "packet_processor_config.h"
#include <vector>

template <class IngressPortType, class EgressPortType>
class PacketProcessor {
  public:
    PacketProcessor() num_ingress_ports_(0), num_egress_ports_(0) {}
    virtual void Init(const PacketProcessorConfig& config) = 0;
    virtual void Run() = 0;
    virtual void FlushState() = 0;
    virtual void RecoverState() = 0;
    const std::string& instance_id() const { return instance_id_; }
    inline const uint16_t& num_ingress_ports() const { return this->num_ingress_ports_; }
    inline const uint16_t& num_egress_ports() const { return this->num_egress_ports_; }
    virtual ~PacketProcessor();
  protected:
    std::string instance_id_;
    uint16_t num_ingress_ports_;
    uint16_t num_egress_ports_;
    std::vector<std::unique_ptr<IngressPortType> > ingress_ports_;
    std::vector<std::unique_ptr<EgressPortType> > egress_ports_;
};

template <> class PacketProcessor <RteIngressPort, RteEgressPort> {
};

template <> class PacketProcessor <NullIngressPort, NullEgressPort> {
};
#endif // _PACKET_PROCESSOR_H_
