#include "rte_nic_egress_port.h"

#include <rte_ethdev.h>
#include <rte_mempool.h>
#include <rte_log.h>


const std::string RteNICEgressPort::kConfNICPortId = "nic_port_id";
const std::string RteNICEgressPort::kConfNICQueueId = "nic_queue_id";

void RteNICEgressPort::Init(std::map<std::string, std::string>& port_config,
                            PacketProcessor* owner_pp) {
  this->nic_port_id_ = std::stoi(port_config[RteNICEgressPort::kConfNICPortId]);
  this->nic_queue_id_ =
      std::stoi(port_config[RteNICEgressPort::kConfNICQueueId]);
  this->stat_mz = rte_memzone_lookup(MZ_STAT);
  this->micronf_stats = (MSStats*)this->stat_mz->addr;
  this->owner_packet_processor_ = owner_pp;
}

int RteNICEgressPort::TxBurst(tx_pkt_array_t& packets, uint16_t burst_size) {
/* 
char str[ 2049 ];
str[2048] = '\0';
  for ( int i = 0; i < 2048; i++ ) 
     str[ i ] = rte_eth_tx_descriptor_status( this->nic_port_id_, this->nic_queue_id_, i ) + '0';

RTE_LOG( INFO, PMD, "Descriptor status: %s ", str );
*/
  int num_tx = rte_eth_tx_burst(this->nic_port_id_, this->nic_queue_id_,
                                packets.data(), burst_size);
//RTE_LOG( INFO, PMD, "\nrte_egress_nic rx: %d. tx: %d.\n", burst_size, num_tx );
    
  if (num_tx < burst_size) {
//this->micronf_stats->packet_drop[owner_packet_processor_->instance_id()][this->port_id_] += (burst_size - num_tx);
    for (int i = num_tx; i < burst_size; i++) {
      rte_pktmbuf_free(packets[i]);
    }
  }
  return num_tx;
}
