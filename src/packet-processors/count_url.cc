#include "count_url.h"
#include <rte_ether.h>
#include <rte_ip.h>
#include <rte_memcpy.h>
#include <rte_tcp.h>
#include <rte_udp.h>
#include "common/protocol_header_offset.h"
#include <string>

void
CountURL::Init(const PacketProcessorConfig& pp_config) {
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
CountURL::Run() {
    rx_pkt_array_t rx_packets;
    uint16_t num_rx = 0;
    uint16_t num_tx = 0;
    register int i = 0;
    struct ProtocolHeaderOffset header_offsets;
    struct ether_hdr* eth = nullptr;
    struct ipv4_hdr* ip   = nullptr;
    struct tcp_hdr* tcp   = nullptr;
    struct udp_hdr* udp   = nullptr;
    char*  payload = nullptr;
    char*  head     = nullptr;
    char*  tail     = nullptr;
    std::string url;


    std::map<std::string, double>::iterator it;

    while (true) {
        num_rx = ingress_ports_[0]->RxBurst(rx_packets);
        for (i = 0; i < num_rx; ++i) {
            eth = rte_pktmbuf_mtod(rx_packets[i], struct ether_hdr*);
			ip = reinterpret_cast<struct ipv4_hdr*>(eth + 1);

			if (likely(ip->next_proto_id == 0x06)) {
                tcp = reinterpret_cast<struct tcp_hdr*>(
												ip + 1);
                payload = reinterpret_cast<char *>(tcp) + ((tcp->data_off & 0xf0) >> 2);

            } else if (ip->next_proto_id == IPPROTO_UDP) {
                udp = reinterpret_cast<struct udp_hdr*>(
												ip + 1);
                payload = reinterpret_cast<char *>(udp) + sizeof(struct udp_hdr);
            } else {
				// don't check other traffic
				continue;
            }
    
            /* find the head and tail of the url in the packet here */
            // targeting specifically to http get traffic
			head = strstr(payload, "GET") + 4;
			tail = strchr(head, ' ');
			url = std::string(head, tail);
    
            if ((it = _urlmap.find(url)) != _urlmap.end()) {
                it->second += 1;
            } else {
                _urlmap.insert(std::pair<std::string, double>(url, 1));
            }

        }

        num_tx = egress_ports_[0]->TxBurst(rx_packets, num_rx);
 
    }
}

void CountURL::FlushState() {}
void CountURL::RecoverState() {} 
