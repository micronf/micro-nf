#ifndef _INGRESS_PORT_H_
#define _INGRESS_PORT_H_

#include <array>

#define RX_BURST_SIZE 64

class IngressPort {
  public:
    // Receive a burst of packets from the port. Maximum burst size is
    // determined by RX_BURST_SIZE macro defined above. The function returns the
    // number of actual packets read from the port. When the function returns,
    // packet will be populated with mbuf pointers.
    virtual int RxBurst(std::array<struct mbuf*, RX_BURST_SIZE>& packets) = 0;
    int port_id() const { return this->port_id_; }
  protected:
    // An identifier assigned to this port. The identifier is specific to port
    // implementation.
    unsigned int port_id_;
};
#endif // _INGRESS_PORT_H_
