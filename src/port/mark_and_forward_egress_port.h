#ifndef _MARK_AND_FORWARD_EGRESS_PORT_H_
#define _MARK_AND_FORWARD_EGRESS_PORT_H_

#include "egress_port.h"

#include <rte_ring.h>
#include <string>

class MarkAndForwardEgressPort : public EgressPort {
 public:
  MarkAndForwardEgressPort();
  void Init(std::map<std::string, std::string>& port_config) override;
  int TxBurst(tx_pkt_array_t& packets) override;

 private:
  // Index of the entry in the embedded bitmap inside an mbuf.
  int bitmap_index_;

  // Bit offset inside a bitmap entry.
  int bitmap_offset_;
  rte_ring* tx_ring_;
};

#endif  // _MARK_AND_FORWARD_EGRESS_PORT_H_
