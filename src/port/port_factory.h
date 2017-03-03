#ifndef _PORT_FACTORY_H_
#define _PORT_FACTORY_H_

#include "port.h"

#include <memory>

class PortFactory {
 public:
  static std::unique_ptr<IngressPort> CreateIngressPort(
      const std::string& port_type,
      std::map<std::string, std::string>& port_config);
  static std::unique_ptr<EgressPort> CreateEgressPort(
      const std::string& port_type,
      std::map<std::string, std::string>& port_config);
};

#endif  // _PORT_FACTORY_H_
