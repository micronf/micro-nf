#ifndef _PORT_FACTORY_H_
#define _PORT_FACTORY_H_

#include "port.h"
#include "ingress_ports.h"
#include "egress_ports.h"

#include <memory>

class PortFactory {
public:
  // Delete copy constructor and assignment operator to prevent duplication of
  // the same instance through copy and assignment.
  PortFactory(const PortFactory &) = delete;
  void operator=(const PortFactory &) = delete;

  // Returns an instance of this class. If an instance does not already exist
  // then it is created, otherwise the same instance is returned everytime
  // GetInstance is invoked.
  static PortFactory *GetInstance();

  // Create an uninitialized ingress port object of type port_type.
  std::unique_ptr<IngressPort> CreateIngressPort(const std::string &port_type);

  // Create an uninitialized egress port object of type port_type.
  std::unique_ptr<EgressPort> CreateEgressPort(const std::string &port_type);

  // A helper method for instantiating an ingress port object. The first
  // template type parameter is the type of the parent class, i.e., IngressPort
  // or EgressPort. The second type parameter is the type of actual port being
  // created, e.g., RteIngressPort or RteNICEgressPort etc.
  template <class P, class T> std::unique_ptr<P> CreatePortInternal();
};

#endif // _PORT_FACTORY_H_
