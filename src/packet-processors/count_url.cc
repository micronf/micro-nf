#include "count_url.h"
#include <rte_ether.h>
#include <rte_ip.h>
#include <rte_memcpy.h>
#include <rte_tcp.h>
#include <rte_udp.h>
#include "common/protocol_header_offset.h"
#include <string>

void
count_url::Init(const PacketProcessorConfig& pp_config) {
   num_ingress_ports_ = pp_config.num_ingress_ports();
   num_egress_ports_  = pp_config.num_egress_ports();
   instance_id_       = pp_config.instance_id();

   assert(num_ingress_ports_ == 1);
   assert(num_egress_ports_ == 1);

   for (int i = 0; i < num_ingress_ports_; ++i)
     ingress_ports_.emplace_back(nullptr);
   for (int j = 0; j < num_egress_ports_; ++j)
     egress_ports_.emplace_back(nullptr);
   PacketProcessor::ConfigurePorts(pp_config, this);
}


void
count_url::Run() {
    rx_pkt_array_t rx_packets;
    uint16_t num_rx = 0;
    uint16_t num_tx = 0;
    register int i = 0;
    struct ProtocolHeaderOffset header_offsets;
    struct ether_hdr* eth = nullptr;
    struct ipv4_hdr* ip   = nullptr;
    struct tcp_hdr* tcp   = nullptr;
    char*  payload = nullptr;
    char*  url     = nullptr;
    std::string key;


    std::map<std::string, double>::iterator it;

    while (true) {
        num_rx = ingress_ports_[0]->RxBurst(rx_packets);
        for (i = 0; i < num_rx; ++i) {
            rte_prefetch0(rx_packets[i]->buf_addr);
            eth = rte_pktmbuf_mtod(rx_packets[i], struct ether_hdr*);
            header_offsets.eth_hdr_addr = reinterpret_cast<uint64_t>(eth);
            header_offsets.ip_hdr_addr =
                    header_offsets.eth_hdr_addr + sizeof(struct ether_hdr);
            header_offsets.transport_hdr_addr =
                    header_offsets.ip_hdr_addr + sizeof(struct ipv4_hdr);
            ip = reinterpret_cast<struct ipv4_hdr*>(header_offsets.ip_hdr_addr);

            //if (likely(ip->next_proto_id == IPPROTO_TCP)) {
            //printf("the next proto id is: %d\n", ip->next_proto_id);
            if (likely(ip->next_proto_id == 0x06)) {
                tcp = reinterpret_cast<struct tcp_hdr*>(
                        header_offsets.transport_hdr_addr);
                header_offsets.payload_addr =
                    header_offsets.transport_hdr_addr + tcp->data_off;

            } else if (ip->next_proto_id == IPPROTO_UDP) {
                header_offsets.payload_addr = 
                    header_offsets.transport_hdr_addr + sizeof(struct udp_hdr);
            }
            
    
            //++pkt_size_bucket_[rx_packets[i]->pkt_len / this->kBucketSize];
            /* find the head and tail of the url in the packet here */
            payload = (char*)(header_offsets.payload_addr);
            url = payload + 4;
            key = std::string(url);
    
            //std::string key(head, tail);
            if ((it = _urlmap.find(key)) != _urlmap.end()) {
                it->second += 1;
            } else {
                _urlmap.insert(std::pair<std::string, double>(key, 1));
            }
        }

        num_tx = egress_ports_[0]->TxBurst(rx_packets, num_rx);
 
    }
}

void count_url::FlushState() {}
void count_url::RecoverState() {} 
