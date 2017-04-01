#include "tcp_ip_classifier.h"
#include <rte_ether.h>
#include <rte_ip.h>
#include <rte_mbuf.h>
#include <rte_tcp.h>

void TCPIPClassifier::Init(const PacketProcessorConfig& pp_config) {
  num_ingress_ports_ = pp_config.num_ingress_ports();
  num_egress_ports_ = pp_config.num_egress_ports();
  for (int i = 0; i < num_ingress_ports_; ++i) {
    ingress_ports_.emplace_back(nullptr);
  }
  for (int i = 0; i < num_egress_ports_; ++i) {
    egress_ports_.emplace_back(nullptr);
  }
  PacketProcessor::ConfigurePorts(pp_config);
  acl_rules_.resize(num_ingress_ports_);
  const auto& pp_params = pp_config.pp_parameters();
  // FIXME: The following should not be hardcoded, rather it should be
  // configured using the configuration file.
  for (int i = 0; i < num_ingress_ports_; ++i) {
    allow_port_index_.push_back(i * 2);
    deny_port_index_.push_back(i * 2 + 1);
  }

  // FIXME: Hardcode some classification rules for testing.
  CIDRAddress src_addr_1("10.10.0.7/24");
  CIDRAddress dst_addr_1("10.10.0.10/24");
  ACLRule rule_1(src_addr_1, dst_addr_1, 1234, 5678, false);
  this->AddRule(0, rule_1);
}

void TCPIPClassifier::Run() {
  rx_pkt_array_t packets;
  tx_pkt_array_t allow_packets;
  tx_pkt_array_t deny_packets;
  uint16_t nb_allow = 0, nb_deny = 0, num_rx = 0;
  register int iport_idx = 0, i = 0;
  struct ether_hdr* ethernet = nullptr;
  struct ipv4_hdr* ipv4 = nullptr;
  struct tcp_hdr* tcp = nullptr;
  while (true) {
    for (iport_idx = 0; iport_idx < num_ingress_ports_; ++iport_idx) {
      num_rx = ingress_ports_[iport_idx]->RxBurst(packets);
      nb_allow = 0;
      nb_deny = 0;
      for (i = 0; i < num_rx; ++i) {
        // rte_prefetch0(rte_pktmbuf_mtod(packet, void*));
        ethernet = rte_pktmbuf_mtod(packets[i], struct ether_hdr*);
        ipv4 = reinterpret_cast<struct ipv4_hdr*>(ethernet + 1);
        tcp = reinterpret_cast<struct tcp_hdr*>(ipv4 + 1);
        for (auto& rule : acl_rules_[iport_idx]) {
          if (rule.Match(ipv4->src_addr, ipv4->dst_addr, tcp->src_port,
                         tcp->dst_port)) {
            if (rule.deny()) {
              deny_packets[nb_deny++] = packets[i];
              break;
            } 
            allow_packets[nb_allow++] = packets[i];
            break;
          }
        }
      }
      this->egress_ports_[allow_port_index_[iport_idx]]->TxBurst(
          allow_packets, nb_allow);
      this->egress_ports_[deny_port_index_[iport_idx]]->TxBurst(
          deny_packets, nb_deny);
    }
  }
}

void TCPIPClassifier::FlushState() {}
void TCPIPClassifier::RecoverState() {}
