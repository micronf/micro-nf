#include "mac_swapper.h"
#include "../port/port_factory.h"
#include <google/protobuf/map.h>
#include <algorithm>
#include <assert.h>
#include <rte_ether.h>
#include <stdlib.h>

inline void MacSwapper::Init(const PacketProcessorConfig& pp_config) {
  this->num_ingress_ports_ = pp_config.num_ingress_ports();
  this->num_egress_ports_ = pp_config.num_egress_ports();

  // First, initialize the list of ingress and egress ports.
  int i = 0;
  for (i = 0; i < this->num_ingress_ports_; ++i) {
    this->ingress_ports_.emplace_back(nullptr);
  }
  for (i = 0; i < this->num_egress_ports_; ++i) {
    this->egress_ports_.emplace_back(nullptr);
  }
  PortFactory* port_factory = PortFactory::GetInstance();
  for (i = 0; i < pp_config.port_configs_size(); ++i) {
    const PortConfig& pconfig = pp_config.port_configs(i);
    std::map<std::string, std::string> port_params;
    const auto& port_parameters_map = pconfig.port_parameters();
    // Converting google::protobuf::Map to std::map. 
    // TODO: Change the function signature of IngressPort::Init and
    // EgressPort::Init and replace std::map with google::protobuf::Map.
    for (auto it = port_parameters_map.begin(); it != port_parameters_map.end(); ++it) {
      port_params[it->first] = it->second;
    }
    if (pconfig.port_type() == PortConfig::INGRESS_PORT) {
      auto iport = port_factory->CreateIngressPort(pconfig.port_class());  
      assert(iport.get() != nullptr);
      iport->Init(port_params);
      this->ingress_ports_[pconfig.port_index()] = std::move(iport);
    } else if (pconfig.port_type() == PortConfig::EGRESS_PORT) {
      auto eport = port_factory->CreateEgressPort(pconfig.port_class());
      assert(eport.get() != nullptr);
      eport->Init(port_params);
      this->egress_ports_[pconfig.port_index()] = std::move(eport);
    }
  }

  // Packet processor specific configuration (if any).
}

void MacSwapper::Run() {
  rx_pkt_array_t rx_packets;
  tx_pkt_array_t tx_packets;
  while (true) {
    int num_rx = this->ingress_ports_[0]->RxBurst(rx_packets);
    if (num_rx > 0) 
      printf("num_rx = %d\n", num_rx);
    int i = 0;
    for (i = 0; i < num_rx; ++i) {
      struct ether_hdr *eth_hdr =
          rte_pktmbuf_mtod(rx_packets[i], struct ether_hdr *);
      printf("BEFORE\n");
      rte_pktmbuf_dump(stdout, rx_packets[i], rx_packets[i]->pkt_len);
      for (int j = 0; j < ETHER_ADDR_LEN; ++j) {
        std::swap(eth_hdr->s_addr.addr_bytes[j], eth_hdr->d_addr.addr_bytes[j]);
      }
      tx_packets[i] = rx_packets[i];
      printf("AFTER\n");
      rte_pktmbuf_dump(stdout, rx_packets[i], rx_packets[i]->pkt_len);
    }
    this->egress_ports_[0]->TxBurst(tx_packets);
  }
}

inline void MacSwapper::FlushState() {}
inline void MacSwapper::RecoverState() {}
