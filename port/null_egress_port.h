#ifndef _NULL_EGRESS_PORT_H_
#define _NULL_EGRESS_PORT_H_

class NullEgressPort : public EgressPort {
  public:
    NullEgressPort() {}
    inline uint16_t TxBurst(void** packets, uint16_t burst_size) override;
    virtual ~NullEgressPort() {}
};
#endif // _NULL_EGRESS_PORT_H_
