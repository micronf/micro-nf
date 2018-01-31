#include "filt_udptcp.h"
#include "../common/protocol_id.h"
#include <rte_ether.h>
#include <rte_ip.h>
#include <rte_tcp.h>
#include <rte_udp.h>
#include "common/protocol_header_offset.h"

#include <rte_byteorder.h>

inline void 
FiltUDPTCP::Init(const PacketProcessorConfig& pp_config) {
  num_ingress_ports_ = pp_config.num_ingress_ports();
  num_egress_ports_ = pp_config.num_egress_ports();
  instance_id_ = pp_config.instance_id();
  assert(num_ingress_ports_ == 1);
  // one port goes to drop muNF (garbage collector) or macRewriter to send back
  assert(num_egress_ports_ == 2);
  for (int i = 0; i < num_ingress_ports_; ++i)
    ingress_ports_.emplace_back(nullptr);
  for (int j = 0; j < num_egress_ports_; ++j)
    egress_ports_.emplace_back(nullptr);
  PacketProcessor::ConfigurePorts(pp_config, this);
  _cpu_ctr.setfname("FiltUDPTCP"+std::to_string(instance_id_));
}

inline bool 
FiltUDPTCP::process(struct rte_mbuf *rx_packet) {
  struct ether_hdr* eth = nullptr;
  struct ipv4_hdr*  ip  = nullptr;
  struct tcp_hdr*   tcp = nullptr;
  struct udp_hdr*   udp = nullptr;
  struct ProtocolHeaderOffset header_offsets;
  uint16_t port = 0;

  tcp = nullptr;
  udp = nullptr;
  port = 0;

  //_cpu_ctr.update();

  eth = rte_pktmbuf_mtod(rx_packet, struct ether_hdr*);
  ip  = reinterpret_cast<struct ipv4_hdr*>(eth + 1);
     
  switch(ip->next_proto_id) {
      case PROTOCOL_TCP:
      case PROTOCOL_UDP:
         break;
      default:
    	RTE_LOG( WARNING, USER1, "filt_udptcp: strange traffic here!\n");
	
        // send to drop micronf
        //rte_pktmbuf_free(rx_packet);
        tcp = nullptr;
        udp = nullptr;
        port = 0;
		//_cpu_ctr.update();
        return false;
  }

  if (likely(ip->next_proto_id == IPPROTO_TCP)) {
     tcp = reinterpret_cast<struct tcp_hdr*>(ip + 1);
  } else if (ip->next_proto_id == IPPROTO_UDP) {
     udp = reinterpret_cast<struct udp_hdr*>(ip + 1);
  } else {
     printf("other traffic here!\n");
     fflush(stdout); 
  }

  port = tcp? tcp->dst_port : udp->dst_port;
  switch(rte_be_to_cpu_16(port)) {
     case 21:
     case 22:
       // send to drop micronf
       //rte_pktmbuf_free(rx_packet);
         tcp = nullptr;
         udp = nullptr;
         port = 0;
         return false;
     default:
	
        break;
  }
  return true;
}

void 
FiltUDPTCP::Run() {
  rx_pkt_array_t rx_packets;
  rx_pkt_array_t tx_packets;
  rx_pkt_array_t tx_packets_1;
  register uint16_t i = 0;
  uint16_t num_rx = 0;
  uint16_t num_tx = 0;
  uint16_t num_tx_1 = 0;

  while (true) {
     num_rx = ingress_ports_[0]->RxBurst(rx_packets);
     num_tx = 0;
     num_tx_1 = 0;
     
     for (i = 0; i < num_rx && i < k_num_prefetch_; ++i)
        rte_prefetch_non_temporal(rte_pktmbuf_mtod(rx_packets[i], void*));
    
     for (i = 0; i < num_rx - k_num_prefetch_; ++i) {    
        if(process(rx_packets[i]))
           tx_packets[num_tx++] = rx_packets[i];
        else
           tx_packets_1[num_tx_1++] = rx_packets[i];
     }
     
     for ( ; i < num_rx; ++i) {
        if( process(rx_packets[i]) )
           tx_packets[num_tx++] = rx_packets[i];
        else
           tx_packets_1[num_tx_1++] = rx_packets[i];
     }

     num_tx = egress_ports_[0]->TxBurst(tx_packets, num_tx);
     num_tx = egress_ports_[1]->TxBurst(tx_packets_1, num_tx_1);
  }
}

void FiltUDPTCP::FlushState() {}
void FiltUDPTCP::RecoverState() {}
