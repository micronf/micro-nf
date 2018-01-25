#ifndef _SET_BITMAP_EGRESS_PORT_H_
#define _SET_BITMAP_EGRESS_PORT_H_

#include "egress_port.h"
#include "packet-processors/packet_processor.h"

#include <rte_ring.h>
#include <string>

class SetBitmapEgressPort : public EgressPort {
 public:
  SetBitmapEgressPort() : bitmap_index_(-1) {}
  SetBitmapEgressPort(int port_id, const std::string& ring_id);
  void Init(std::map<std::string, std::string>& port_config,
            PacketProcessor* owner_pp) override;
  int TxBurst(tx_pkt_array_t& packets, uint16_t burst_size) override;
};

#endif  // _SET_BITMAP_EGRESS_PORT_H_
