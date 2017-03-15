#ifndef _INGRESS_PORT_H_
#define _INGRESS_PORT_H_

#include <rte_mbuf.h>

#include <array>
#include <map>

#define RX_BURST_SIZE 64

typedef std::array<struct rte_mbuf*, RX_BURST_SIZE> rx_pkt_array_t;

class IngressPort {
 public:
  // Initialize a port from a set of key-value strings. Since initialization
  // is port specific, each implementation of IngressPort should have their
  // own Init method.
  virtual void Init(std::map<std::string, std::string>& port_config) = 0;

  // Receive a burst of packets from the port. Maximum burst size is
  // determined by RX_BURST_SIZE macro defined above. The function returns the
  // number of actual packets read from the port. When the function returns,
  // packet will be populated with mbuf pointers.
  virtual int RxBurst(rx_pkt_array_t& packets) = 0;

  unsigned int port_id() const { return this->port_id_; }
  virtual ~IngressPort() { }
  // The following constants define the keys corresponding to relevant
  // configuration.
  static const std::string kConfPortId;
  static const std::string kConfRingId;

 protected:
  // An identifier assigned to this port. The identifier is specific to port
  // implementation.
  unsigned int port_id_;
};
#endif  // _INGRESS_PORT_H_
