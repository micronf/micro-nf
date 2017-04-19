#include "port_factory.h"
#include <rte_malloc.h>

PortFactory *PortFactory::GetInstance() {
  static std::unique_ptr<PortFactory> port_ptr(nullptr);
  if (port_ptr.get() == nullptr) {
    port_ptr.reset(reinterpret_cast<PortFactory *>(
        rte_zmalloc("PortFactory", sizeof(PortFactory), RTE_CACHE_LINE_SIZE)));
  }
  return port_ptr.get();
}

std::unique_ptr<IngressPort> PortFactory::CreateIngressPort(
    const std::string &port_type) {
  std::unique_ptr<IngressPort> port_ptr(nullptr);
  if (port_type == "NullIngressPort") {
    port_ptr = CreatePortInternal<IngressPort, NullIngressPort>();
  } else if (port_type == "RteIngressPort") {
    port_ptr = CreatePortInternal<IngressPort, RteIngressPort>();
  } else if (port_type == "RteNICIngressPort") {
    port_ptr = CreatePortInternal<IngressPort, RteNICIngressPort>();
  } else if (port_type == "SyncIngressPort") {
    port_ptr = CreatePortInternal<IngressPort, SyncIngressPort>();
  }
  return std::move(port_ptr);
}

std::unique_ptr<EgressPort> PortFactory::CreateEgressPort(
    const std::string &port_type) {
  std::unique_ptr<EgressPort> port_ptr(nullptr);
  if (port_type == "NullEgressPort") {
    port_ptr = CreatePortInternal<EgressPort, NullEgressPort>();
  } else if (port_type == "RteEgressPort") {
    port_ptr = CreatePortInternal<EgressPort, RteEgressPort>();
  } else if (port_type == "RteNICEgressPort") {
    port_ptr = CreatePortInternal<EgressPort, RteNICEgressPort>();
  } else if (port_type == "BranchEgressPortZC") {
    port_ptr = CreatePortInternal<EgressPort, BranchEgressPortZC>();
  } else if (port_type == "MarkAndForwardEgressPort") {
    port_ptr = CreatePortInternal<EgressPort, MarkAndForwardEgressPort>();
  } else if (port_type == "SetBitmapEgressPort") {
    port_ptr = CreatePortInternal<EgressPort, SetBitmapEgressPort>();
  }
  return std::move(port_ptr);
}

template <class P, class T>
std::unique_ptr<P> PortFactory::CreatePortInternal() {
  auto port_ptr = std::unique_ptr<P>(new T());
  // size_t alloc_size = sizeof(T);
  // auto port_ptr = std::unique_ptr<P>(
  //    reinterpret_cast<T*>(rte_zmalloc(NULL, alloc_size,
  //    RTE_CACHE_LINE_SIZE)));
  return std::move(port_ptr);
}
