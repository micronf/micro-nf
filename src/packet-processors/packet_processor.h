#ifndef _PACKET_PROCESSOR_H_
#define _PACKET_PROCESSOR_H_

#include "../port/port.h"
#include "../port/port_factory.h"
#include "packet_processor_config.pb.h"

#include <memory>
#include <vector>

class PacketProcessor {
 public:
  PacketProcessor() : num_ingress_ports_(0), num_egress_ports_(0) {}

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
  std::vector<std::unique_ptr<IngressPort>>& ingress_ports() {
    return ingress_ports_;
  }
  std::vector<std::unique_ptr<EgressPort>>& egress_ports() {
    return egress_ports_;
  }

  virtual ~PacketProcessor(){};

 protected:
  // Configure the ports of this packet processor according to provided
  // configuration. This will be a method private to each PacketProcessor
  // implementation. 
  inline void ConfigurePorts(const PacketProcessorConfig& pp_config, 
									const PacketProcessor* owner_pp = nullptr) {
    PortFactory* port_factory = PortFactory::GetInstance();
    int i = 0;
    for (i = 0; i < pp_config.port_configs_size(); ++i) {
      const PortConfig& pconfig = pp_config.port_configs(i);
      std::map<std::string, std::string> port_params;
      const auto& port_parameters_map = pconfig.port_parameters();
      // Converting google::protobuf::Map to std::map.
      // TODO: Change the function signature of IngressPort::Init and
      // EgressPort::Init and replace std::map with google::protobuf::Map.
      for (auto it = port_parameters_map.begin(); it != port_parameters_map.end();
           ++it) {
        port_params[it->first] = it->second;
      }
      if (pconfig.port_type() == PortConfig::INGRESS_PORT) {
        auto iport = port_factory->CreateIngressPort(pconfig.port_class());
        assert(iport.get() != nullptr);
        iport->Init(port_params);
        this->ingress_ports_[pconfig.port_index()] = std::move(iport);
      } else if (pconfig.port_type() == PortConfig::EGRESS_PORT) {
        auto eport = port_factory->CreateEgressPort(pconfig.port_class());
        assert(eport.get() != nullptr);
        eport->Init(port_params, owner_pp);
        this->egress_ports_[pconfig.port_index()] = std::move(eport);
      }
    }
  }

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
#endif  // _PACKET_PROCESSOR_H_
