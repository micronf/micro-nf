#include "filt_udptcp.h"
#include "../common/protocol_id.h"
#include <rte_ether.h>
#include <rte_ip.h>
#include <rte_tcp.h>
#include <rte_udp.h>
#include "common/protocol_header_offset.h"


inline void 
filt_udptcp::Init(const PacketProcessorConfig& pp_config) {
  num_ingress_ports_ = pp_config.num_ingress_ports();
  num_egress_ports_ = pp_config.num_egress_ports();
  instance_id_ = pp_config.instance_id();
  assert(num_ingress_ports_ == 1);
  assert(num_egress_ports_ == 1);
  for (int i = 0; i < num_ingress_ports_; ++i)
    ingress_ports_.emplace_back(nullptr);
  for (int j = 0; j < num_egress_ports_; ++j)
    egress_ports_.emplace_back(nullptr);
  PacketProcessor::ConfigurePorts(pp_config, this);
}

void 
filt_udptcp::Run() {
  rx_pkt_array_t rx_packets;
  register uint16_t i = 0;
  uint16_t num_rx = 0;
  uint16_t num_tx = 0;
  uint16_t port = 0;

  struct ether_hdr* eth = nullptr;
  struct ipv4_hdr*  ip  = nullptr;
  struct tcp_hdr*   tcp = nullptr;
  struct udp_hdr*   udp = nullptr;
  struct ProtocolHeaderOffset header_offsets;

  while (true) {
    num_rx = ingress_ports_[0]->RxBurst(rx_packets);

    for (i = 0; i < num_rx; i++) {
        rte_prefetch0(rx_packets[i]->buf_addr);
        eth = rte_pktmbuf_mtod(rx_packets[i], struct ether_hdr*);
        header_offsets.eth_hdr_addr = reinterpret_cast<uint64_t>(eth);
        header_offsets.ip_hdr_addr =
                        header_offsets.eth_hdr_addr + sizeof(struct ether_hdr);
        ip = reinterpret_cast<struct ipv4_hdr*>(header_offsets.ip_hdr_addr);
        //ip = reinterpret_cast<struct ipv4_hdr*>((void*)eth + sizeof(struct ether_hdr));
   
     
        switch(ip->next_proto_id) {
            case PROTOCOL_TCP:
            case PROTOCOL_UDP:
                break;
            default:
                printf("wrong proto\n");
                fflush(stdout);
                rte_pktmbuf_free(rx_packets[i]);
                tcp = nullptr;
                udp = nullptr;
                continue;
        }

        if (likely(ip->next_proto_id == IPPROTO_TCP)) {
            //printf("tcp");
            //fflush(stdout); 
            tcp = reinterpret_cast<struct tcp_hdr*>(header_offsets.ip_hdr_addr + sizeof(struct ipv4_hdr));
        } else if (ip->next_proto_id == IPPROTO_UDP) {
            udp = reinterpret_cast<struct udp_hdr*>(header_offsets.ip_hdr_addr + sizeof(struct ipv4_hdr));
        } else {
            printf("WHAAAAAAAAAAAAAAT?\n");
            fflush(stdout); 
        }
        
        port = tcp? tcp->dst_port : udp->dst_port;
        switch(port) {
            case 21:
            case 22:
                rte_pktmbuf_free(rx_packets[i]);
                tcp = nullptr;
                udp = nullptr;
                continue;
            default:
                break;
        }
    }
    num_tx = egress_ports_[0]->TxBurst(rx_packets, num_rx);
    tcp = nullptr;
    udp = nullptr;
    printf("num_tx: %d\n", num_tx);
    fflush(stdout);
  }
}

void filt_udptcp::FlushState() {}
void filt_udptcp::RecoverState() {}
