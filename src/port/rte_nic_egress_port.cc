#include "rte_nic_egress_port.h"

#include <iostream>
#include <rte_ethdev.h>
#include <rte_mempool.h>

const std::string RteNICEgressPort::kConfNICPortId = "nic_port_id";
const std::string RteNICEgressPort::kConfNICQueueId = "nic_queue_id";

void RteNICEgressPort::Init(std::map<std::string, std::string>& port_config,
              PacketProcessor* owner_pp) {
  this->nic_port_id_ = std::stoi(port_config[RteNICEgressPort::kConfNICPortId]);
  this->nic_queue_id_ =
      std::stoi(port_config[RteNICEgressPort::kConfNICQueueId]);
}

int RteNICEgressPort::TxBurst(tx_pkt_array_t& packets, uint16_t burst_size) {
  int num_tx = rte_eth_tx_burst(this->nic_port_id_, this->nic_queue_id_,
                          packets.data(), burst_size);
	
  int idx = std::stoi(owner_packet_processor_->instance_id());
  printf("idx: %d\n", idx);
	printf("num_tx: %d\n", num_tx);
	if(num_tx < burst_size){
    this->micronf_stats->packet_drop[idx] += (burst_size - num_tx);
    for(int i=0; i < burst_size-num_tx; i++){
      rte_pktmbuf_free(packets[i]);
    }
  }
	
  return num_tx;
}
