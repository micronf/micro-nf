#include "tcp_ip_classifier.h"

#include <rte_ether.h>
#include <rte_ip.h>
#include <rte_mbuf.h>
#include <rte_tcp.h>

void TCPIPClassifier::Init(const PacketProcessorConfig& config) {}

void TCPIPClassifier::Run() {
  rx_pkt_array_t packets;
  tx_pkt_array_t allow_packets;
  tx_pkt_array_t deny_packets;
  int nb_allow = 0, nb_deny = 0;
  while(true) {
    for (auto& ingress_port : ingress_ports_) {
      ingress_port->RxBurst(packets);
      nb_allow = 0; nb_deny = 0;
      for (auto packet : packets) {
        struct ether_hdr* ethernet = rte_pktmbuf_mtod(packet, struct ether_hdr*);
        struct ipv4_hdr* ipv4 = reinterpret_cast<struct ipv4_hdr*>(ethernet + 1);
        struct tcp_hdr* tcp = reinterpret_cast<struct tcp_hdr*>(ipv4 + 1);
        for (auto& rule : acl_rules_) {
          if (rule.Match(ipv4->src_addr, ipv4->dst_addr, tcp->src_port, tcp->dst_port)) {
            if (rule.deny()) {
              deny_packets[nb_deny++] = packet;
              break;
            } else {
              allow_packets[nb_allow++] = packet;
              break;
            }
          }
        }
      }
      this->egress_ports_[allow_port_index_]->TxBurst(allow_packets);
      this->egress_ports_[deny_port_index_]->TxBurst(deny_packets);
    }
  }
}

void TCPIPClassifier::FlushState() {}
void TCPIPClassifier::RecoverState() {}
