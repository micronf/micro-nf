#ifndef _MARK_AND_FORWARD_EGRESS_PORT_H_
#define _MARK_AND_FORWARD_EGRESS_PORT_H_

#include "egress_port.h"

#include <rte_ring.h>
#include <string>

class MarkAndForwardEgressPort : public EgressPort {
  public:
    MarkAndForwardEgressPort();
    MarkAndForwardEgressPort(int port_id, const std::string& ring_id);
    int TxBurst(void** packets, int burst_size) override;
  private:
    // Identification of this port number. This does not have to be unique
    // across all ports. This is only required to be unique across the ports in
    // the same stage. Assumption: port_id_ is sequentially assigned starting
    // from 0.
    int port_id_;

    // Index of the entry in the embedded bitmap inside an mbuf.
    int bitmap_index_;

    // Bit offset inside a bitmap entry.
    int bitmap_offset_;
    rte_ring* tx_ring_;
};

#endif // _MARK_AND_FORWARD_EGRESS_PORT_H_
