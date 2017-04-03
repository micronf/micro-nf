#include "null_egress_port.h"

void NullEgressPort::Init(std::map<std::string, std::string>& port_config,
              const PacketProcessor* owner_pp) {}
int NullEgressPort::TxBurst(tx_pkt_array_t& packets, uint16_t burst_size) {
  return 0;
}
