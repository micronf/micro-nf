#ifndef _EGRESS_PORT_H_
#define _EGRESS_PORT_H_

#include <vector>

class EgressPort {
  public:
    void TxBurst() = 0;
};
#endif // _EGRESS_PORT_H_
