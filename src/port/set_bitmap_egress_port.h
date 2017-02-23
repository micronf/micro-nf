#ifndef _SET_BITMAP_EGRESS_PORT_H_
#define _SET_BITMAP_EGRESS_PORT_H_

#include "egress_port.h"

#include <rte_ring.h>
#include <string>

class SetBitmapEgressPort : public EgressPort {
public:
  SetBitmapEgressPort();
  SetBitmapEgressPort(int port_id, const std::string &ring_id);
  int TxBurst(void **packets, int burst_size);

private:
  int port_id_;
  int bitmap_index_;
  int bitmap_offset_;
};

#endif // _SET_BITMAP_EGRESS_PORT_H_
