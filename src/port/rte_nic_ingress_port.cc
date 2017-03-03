#include "rte_nic_ingress_port.h"

const std::string RteNICIngressPort::kConfPortId = "nic_port_id";
const std::string RteNICIngressPort::kConfQueueId = "nic_queue_id";

void RteNICIngressPort::Init(std::map<std::string, std::string> &port_config) {
  this->nic_port_id_ = std::stoi(port_config[RteNICIngressPort::kConfPortId]);
  this->nic_queue_id_ = std::stoi(port_config[RteNICIngressPort::kConfQueueId]);
}

int RteNICIngressPort::RxBurst(rx_pkt_array_t &packets) {
  return rte_eth_rx_burst(this->nic_port_id_, this->nic_queue_id_,
                          reinterpret_cast<void **>(packets.data()),
                          RX_BURST_SIZE);
}
