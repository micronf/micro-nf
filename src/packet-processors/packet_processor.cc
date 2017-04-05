#include  "packet_processor.h"
#include  "../port/port_factory.h"

void PacketProcessor::ConfigurePorts(const PacketProcessorConfig& pp_config, 
									PacketProcessor* owner_pp) {
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

