#include "count_subnet_packets.h"
#include <rte_ether.h>
#include <rte_ip.h>

inline void CountSubnetPackets::Init(const PacketProcessorConfig& pp_config) {
  num_ingress_ports_ = pp_config.num_ingress_ports();
  num_egress_ports_ = pp_config.num_egress_ports();
	instance_id_ = pp_config.instance_id();
  // First, initialize the list of ingress and egress ports.
  int i = 0;
  for (i = 0; i < this->num_ingress_ports_; ++i) {
    ingress_ports_.emplace_back(nullptr);
  }
  for (i = 0; i < this->num_egress_ports_; ++i) {
    egress_ports_.emplace_back(nullptr);
  }
  PacketProcessor::ConfigurePorts(pp_config, this);
  subnet_pkt_counter_.clear();
  i_to_e_port_map_.resize(num_ingress_ports_);

  // Hardcoded configuration for convenience.
  for (int i = 0; i < num_ingress_ports_; ++i) i_to_e_port_map_.push_back(i);
  AddSubnet("10.10.0.0/24");
}

inline void CountSubnetPackets::AddSubnet(const std::string& subnet_str) {
  CIDRAddress subnet(subnet_str);
  subnet_pkt_counter_.push_back(std::make_pair(subnet, 0));
}

inline void CountSubnetPackets::Run() {
  rx_pkt_array_t rx_packets;
  uint16_t num_rx = 0;
  register int j = 0, k = 0;
  int i = 0, out_port = -1;
  struct ipv4_hdr* ipv4 = nullptr;
  while (true) {
    for (i = 0; i < num_ingress_ports_; ++i) {
      num_rx = ingress_ports_[i]->RxBurst(rx_packets);
      if (unlikely(num_rx <= 0)) continue;
      for (j = 0; j < num_rx; ++j) {
        rte_prefetch0(rx_packets[j]);
        ipv4 = reinterpret_cast<ipv4_hdr*>(
            rte_pktmbuf_mtod(rx_packets[i], struct ether_hdr*) + 1);
        for (k = 0; k < subnet_pkt_counter_.size(); ++k) {
          if (subnet_pkt_counter_[k].first.Match(ipv4->dst_addr)) {
            ++subnet_pkt_counter_[k].second;
            break;
          }
        }
      }
      out_port = i_to_e_port_map_[i];
      egress_ports_[out_port]->TxBurst(rx_packets, num_rx);
    }


		for(int i; i < num_egress_ports_; i++){
      if(this->scale_bits->bits[this->instance_id_].test(i)){
          // TODO 
          // Change port to smart port.
      }
    }
  }
}

inline void CountSubnetPackets::FlushState() {}
inline void CountSubnetPackets::RecoverState() {}
