#include "null_egress_port.h"

int NullEgressPort::TxBurst(std::array<struct rte_mbuf*, TX_BURST_SIZE>& packets) {
  return 0;
}
