#ifndef _MARK_AND_FORWARD_EGRESS_PORT_H_
#define _MARK_AND_FORWARD_EGRESS_PORT_H_

#include "packet-processors/packet_processor.h"
#include "egress_port.h"

#include <rte_ring.h>
#include <string>

class MarkAndForwardEgressPort : public EgressPort {
 public:
  MarkAndForwardEgressPort() : tx_ring_(nullptr) {}
  void Init(std::map<std::string, std::string>& port_config,
            PacketProcessor* owner_pp) override;
  int TxBurst(tx_pkt_array_t& packets, uint16_t burst_size) override;

 private:
  rte_ring* tx_ring_;
};

#endif  // _MARK_AND_FORWARD_EGRESS_PORT_H_
