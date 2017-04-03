#ifndef _NULL_EGRESS_PORT_H_
#define _NULL_EGRESS_PORT_H_

#include "port.h"
#include "../packet-processors/packet_processor.h"

class NullEgressPort : public EgressPort {
 public:
  NullEgressPort() {}
  void Init(std::map<std::string, std::string>& port_config,
              PacketProcessor* owner_pp) override;
  int TxBurst(tx_pkt_array_t& packets, uint16_t burst_size) override;
  virtual ~NullEgressPort() {}
};
#endif  // _NULL_EGRESS_PORT_H_
