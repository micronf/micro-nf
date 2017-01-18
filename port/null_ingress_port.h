#ifndef _NULL_INGRESS_PORT_H_
#define _NULL_INGRESS_PORT_H_

class NullIngressPort : public IngressPort {
  public:
    NullIngressPort() {}
    inline void** RxBurst(int burst_size) override;
    virtual ~RteIngressPort() {}
};
#endif // _NULL_INGRESS_PORT_H_
