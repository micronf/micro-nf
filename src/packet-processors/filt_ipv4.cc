#include "filt_ipv4.h"
#include <rte_ether.h>
#include <rte_ip.h>

inline void filt_ipv4::Init(const PacketProcessorConfig& pp_config) {
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

   // Packet processor specific configuration (if any).
}

void
filt_ipv4::Run() {
    uint16_t num_rx = 0;
    register uint16_t i = 0;
    rx_pkt_array_t rx_packets;
    struct ether_hdr* eth = nullptr;
    struct ipv4_hdr* ip = nullptr;

    while (true) {
        num_rx = ingress_ports_[0]->RxBurst(rx_packets);
        for (i = 0; i < num_rx; i++) {
            rte_prefetch0(rx_packets[i]->buf_addr);
            eth = rte_pktmbuf_mtod(rx_packets[i], struct ether_hdr*);
            printf("%" PRIu16 "\n",eth->ether_type);
            switch (eth->ether_type) {
                case ETHER_TYPE_IPv4:
                    printf("ipv4!\n");
                    break;
                case ETHER_TYPE_ARP:
                case ETHER_TYPE_RARP:
                case ETHER_TYPE_IPv6:
                default:
                    rte_pktmbuf_free(rx_packets[i]);
                    continue;
            }
        }
        egress_ports_[0]->TxBurst(rx_packets, num_rx);
    }
}
inline void filt_ipv4::FlushState() {}
inline void filt_ipv4::RecoverState() {}
