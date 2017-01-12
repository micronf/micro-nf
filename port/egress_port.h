#ifndef _EGRESS_PORT_H_
#define _EGRESS_PORT_H_

class EgressPort {
  public:
    void TxBatch() = 0;
};
#endif // _EGRESS_PORT_H_
