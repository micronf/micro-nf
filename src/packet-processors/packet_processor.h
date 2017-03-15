#ifndef _PACKET_PROCESSOR_H_
#define _PACKET_PROCESSOR_H_

#include "../port/port.h"
#include "packet_processor_config.pb.h"

#include <memory>
#include <vector>

class PacketProcessor {
  public:
    PacketProcessor(): num_ingress_ports_(0), num_egress_ports_(0) {}

    // Initialize the packet processor object with the provided configuration.
    virtual void Init(const PacketProcessorConfig& pp_config) = 0;

    // Run is expected to be a blocking method that iterates over the ingress
    // ports, reads packet batches, performs processing and then writes them to
    // appropriate egress ports if applicable.
    virtual void Run() = 0;

    virtual void FlushState() = 0;
    virtual void RecoverState() = 0;
    const std::string& instance_id() const { return instance_id_; }
    inline uint16_t num_ingress_ports() const { return this->num_ingress_ports_; }
    inline uint16_t num_egress_ports() const { return this->num_egress_ports_; }
    std::vector<std::unique_ptr<IngressPort>>& ingress_ports() { return ingress_ports_; }
    std::vector<std::unique_ptr<EgressPort>>& egress_ports() { return egress_ports_; }
    virtual ~PacketProcessor() {};

  protected:
    std::string instance_id_;
    uint16_t num_ingress_ports_;
    uint16_t num_egress_ports_;
    std::vector<std::unique_ptr<IngressPort>> ingress_ports_;
    std::vector<std::unique_ptr<EgressPort>> egress_ports_;
};

// template <> class PacketProcessor <RteIngressPort, RteEgressPort> {
// };

// template <> class PacketProcessor <NullIngressPort, NullEgressPort> {
// };
#endif // _PACKET_PROCESSOR_H_
