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

  this->stat_mz = rte_memzone_lookup(MZ_STAT);
  this->micronf_stats = (MSStats*) this->stat_mz->addr;
  this->owner_packet_processor_ = owner_pp;
	
  this->scale_bits_mz = rte_memzone_lookup(MZ_SCALE);
  this->scale_bits = (ScaleBitVector*) this->scale_bits_mz->addr;


}

int RteNICEgressPort::TxBurst(tx_pkt_array_t& packets, uint16_t burst_size) {
  int num_tx = rte_eth_tx_burst(this->nic_port_id_, this->nic_queue_id_,
                          packets.data(), burst_size);
	if(num_tx < burst_size){
    this->micronf_stats->packet_drop[owner_packet_processor_->instance_id()] += (burst_size - num_tx);
    for(int i=num_tx; i < burst_size; i++){
      rte_pktmbuf_free(packets[i]);
    }
  }
	
  if(this->scale_bits->bits.test(owner_packet_processor_->instance_id())){
      // TODO 
      // Change port to smart port. How?? need to change the caller of this function
	}
	

  return num_tx;
}
