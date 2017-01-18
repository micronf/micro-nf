#ifndef _INGRESS_PORT_H_
#define _INGRESS_PORT_H_

class IngressPort {
  public:
    virtual void** RxBurst(int burst_size) = 0;
};
#endif // _INGRESS_PORT_H_
