#ifndef _NULL_INGRESS_PORT_H_
#define _NULL_INGRESS_PORT_H_

#include "ingress_port.h"

class NullIngressPort : public IngressPort {
  public:
    NullIngressPort() {}
    void** RxBurst(int burst_size) override;
    virtual ~NullIngressPort() {}
};
#endif // _NULL_INGRESS_PORT_H_
