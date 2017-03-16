#include "null_egress_port.h"

void NullEgressPort::Init(std::map<std::string, std::string>& port_config) {}
int NullEgressPort::TxBurst(
    std::array<struct rte_mbuf*, TX_BURST_SIZE>& packets) {
  return 0;
}
