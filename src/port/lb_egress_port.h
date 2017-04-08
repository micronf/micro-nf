#ifndef _LB_EGRESS_PORT_H_
#define _LB_EGRESS_PORT_H_

#include <vector>
#include "egress_port.h"
#include "packet-processors/packet_processor.h"

class LBEgressPort : public EgressPort {
 public:
  LBEgressPort() {}
  void Init(std::map<std::string, std::string>& port_config,
            PacketProcessor* owner_pp) override;
  int TxBurst(tx_pkt_array_t& packets, uint16_t burst_size) override;
  void AddDestinationMS(const std::string& ring_name);
  virtual ~LBEgressPort() {}

  static const std::string kConfNumDestinationMs;

 private:
  int num_destination_ms_;
  std::vector<struct rte_ring*> tx_rings_;
};

#endif  // _LB_EGRESS_PORT_H_
