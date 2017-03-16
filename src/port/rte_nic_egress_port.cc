#include "rte_nic_egress_port.h"

#include <rte_ethdev.h>

const std::string RteNICEgressPort::kConfNICPortId = "nic_port_id";
const std::string RteNICEgressPort::kConfNICQueueId = "nic_queue_id";

void RteNICEgressPort::Init(std::map<std::string, std::string>& port_config) {
  this->nic_port_id_ = std::stoi(port_config[RteNICEgressPort::kConfNICPortId]);
  this->nic_queue_id_ =
      std::stoi(port_config[RteNICEgressPort::kConfNICQueueId]);
}

int RteNICEgressPort::TxBurst(tx_pkt_array_t& packets) {
  return rte_eth_tx_burst(this->nic_port_id_, this->nic_queue_id_,
                          packets.data(), TX_BURST_SIZE);
}
