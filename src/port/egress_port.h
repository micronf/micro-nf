#ifndef _EGRESS_PORT_H_
#define _EGRESS_PORT_H_

#include <vector>

class EgressPort {
  public:
    virtual int TxBurst(void** packets, int burst_size) = 0;
};
#endif // _EGRESS_PORT_H_
