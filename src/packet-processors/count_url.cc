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
  _cpu_ctr.setfname("CountURL"+std::to_string(instance_id_));
}

inline bool
CountURL::process(struct rte_mbuf *rx_packet) {
	struct ProtocolHeaderOffset header_offsets;
    struct ether_hdr* eth = nullptr;
    struct ipv4_hdr* ip   = nullptr;
    struct tcp_hdr* tcp   = nullptr;
    struct udp_hdr* udp   = nullptr;
    char*  payload = nullptr;
    char*  head     = nullptr;
    char*  tail     = nullptr;
    std::string url;
    int offset = 0;

    std::map<std::string, double>::iterator it;
	
	eth = rte_pktmbuf_mtod(rx_packet, struct ether_hdr*);
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
    	return false;
	}

	if(strncmp(payload, "GET", 3) == 0) {
	    offset = 4;
	} else if(!strncmp(payload, "POST", 4) || !strncmp(payload, "HEAD", 4)) {
    	offset = 5;
	} else if(!strncmp(payload, "OPTIONS", 7) || !strncmp(payload, "CONNECT", 7)) {
    	offset = 8;
	} else {
    	return false;
	}   
/*
	const char *url = (payload + offset);
	const char *keyend = strchr(url, ' ');
	String key = String(url, keyend);
*/

	
	head = payload + offset;
	tail = strchr(head, ' ');
	url = std::string(head, tail);

	if ((it = _urlmap.find(url)) != _urlmap.end()) {
    	it->second += 1;
	} else {
    	_urlmap.insert(std::pair<std::string, double>(url, 1));
	}
	
	return true;
	
}

void
CountURL::Run() {
    rx_pkt_array_t rx_packets;
    uint16_t num_rx = 0;
    uint16_t num_tx = 0;
    register int i = 0;

    while (true) {
        num_rx = ingress_ports_[0]->RxBurst(rx_packets);

	    /* ADDED FOR MEASURING CPU CYCLES */
	    _cpu_ctr.update();
	    /*#################################*/

        for (i = 0; i < num_rx; ++i) {
			process(rx_packets[i]);
        }

		/* added for counting cycles */ 
		if(num_rx > 1) {
			_cpu_ctr.addmany(num_rx);
		} else {
			_cpu_ctr.end_rdtsc();
		}
		/*############################*/

        num_tx = egress_ports_[0]->TxBurst(rx_packets, num_rx);
 
    }
}

void CountURL::FlushState() {}
void CountURL::RecoverState() {} 
