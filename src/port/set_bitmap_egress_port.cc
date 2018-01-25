#include "set_bitmap_egress_port.h"
#include <rte_atomic.h>
#include <rte_mbuf.h>

void SetBitmapEgressPort::Init(std::map<std::string, std::string> &port_config,
                               PacketProcessor *owner_pp) {
  this->port_id_ = std::stoi(port_config[EgressPort::kConfPortId]);
}

int SetBitmapEgressPort::TxBurst(tx_pkt_array_t &packets, uint16_t burst_size) {
  // For each packet increase the shared counter in the meta-data area.
  for (int i = 0; i < burst_size; ++i) {
    char *mdata_ptr = MDATA_PTR(packets[i]);
    rte_atomic16_inc(reinterpret_cast<rte_atomic16_t *>(mdata_ptr));
  }
  return 0;
}
