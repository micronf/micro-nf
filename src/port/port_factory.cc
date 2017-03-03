#include "port_factory.h"

#include "rte_egress_port.h"
#include "rte_ingress_port.h"

#include <rte_malloc.h>

std::unique_ptr<IngressPort> PortFactory::CreateIngressPort(
    const std::string& port_type,
    std::map<std::string, std::string>& port_config) {
  std::unique_ptr<IngressPort> port_ptr(nullptr);
  if (port_type == "RteIngressPort") {
    port_ptr = std::unique_ptr<IngressPort>(
        reinterpret_cast<RteIngressPort*>(rte_zmalloc(
            port_type.c_str(), sizeof(RteIngressPort), RTE_CACHE_LINE_SIZE)));
    port_ptr->Init(port_config);
  }
  return std::move(port_ptr);
}

std::unique_ptr<EgressPort> PortFactory::CreateEgressPort(
    const std::string& port_type,
    std::map<std::string, std::string>& port_config) {
  std::unique_ptr<EgressPort> port_ptr(nullptr);
  if (port_type == "RteEgressPort") {
    port_ptr = std::unique_ptr<EgressPort>(
        reinterpret_cast<RteEgressPort*>(rte_zmalloc(
            port_type.c_str(), sizeof(RteEgressPort), RTE_CACHE_LINE_SIZE)));
    port_ptr->Init(port_config);
  }
  return std::move(port_ptr);
}
