#ifndef _NULL_EGRESS_PORT_H_
#define _NULL_EGRESS_PORT_H_

#include "egress_port.h"

class NullEgressPort : public EgressPort {
  public:
    NullEgressPort() {}
    int TxBurst(void** packets, int burst_size) override;
    virtual ~NullEgressPort() {}
};
#endif // _NULL_EGRESS_PORT_H_
