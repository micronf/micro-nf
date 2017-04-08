#ifndef _MARK_AND_FORWARD_EGRESS_PORT_H_
#define _MARK_AND_FORWARD_EGRESS_PORT_H_

#include "egress_port.h"
#include "../packet-processors/packet_processor.h"

#include <rte_ring.h>
#include <string>

class MarkAndForwardEgressPort : public EgressPort {
 public:
  MarkAndForwardEgressPort() : 
    bitmap_index_(-1), bitmap_offset_(-1), tx_ring_(nullptr) {}
  void Init(std::map<std::string, std::string>& port_config,
              PacketProcessor* owner_pp) override;
  int TxBurst(tx_pkt_array_t& packets, uint16_t burst_size) override;

 private:
  // Index of the entry in the embedded bitmap inside an mbuf.
  int bitmap_index_;

  // Bit offset inside a bitmap entry.
  int bitmap_offset_;

  rte_ring* tx_ring_;
};

#endif  // _MARK_AND_FORWARD_EGRESS_PORT_H_
