#ifndef _NULL_EGRESS_PORT_H_
#define _NULL_EGRESS_PORT_H_

#include "egress_port.h"

class NullEgressPort : public EgressPort {
  public:
    NullEgressPort() {}
    virtual int TxBurst(void** packets, int burst_size);
    virtual ~NullEgressPort() {}
};
#endif // _NULL_EGRESS_PORT_H_
