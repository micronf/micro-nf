#include "rte_nic_ingress_port.h"

#include <rte_ethdev.h>

const std::string RteNICIngressPort::kConfNICPortId = "nic_port_id";
const std::string RteNICIngressPort::kConfNICQueueId = "nic_queue_id";

void RteNICIngressPort::Init(std::map<std::string, std::string> &port_config) {
  this->nic_port_id_ = std::stoi(port_config[RteNICIngressPort::kConfNICPortId]);
  this->nic_queue_id_ = std::stoi(port_config[RteNICIngressPort::kConfNICQueueId]);
}

int RteNICIngressPort::RxBurst(rx_pkt_array_t &packets) {
  return rte_eth_rx_burst(this->nic_port_id_, this->nic_queue_id_,
                          packets.data(), RX_BURST_SIZE);
}
