#include "validate_url.h"
#include <rte_ether.h>
#include <rte_ip.h>
#include <rte_memcpy.h>
#include <rte_tcp.h>
#include <rte_udp.h>
#include <string>
#include <iostream>
#include <rte_byteorder.h>

ValidateURL::~ValidateURL() {
	delete _url_regex1;
	delete _url_regex2;
}

inline void ValidateURL::Init(const PacketProcessorConfig& pp_config) {
  num_ingress_ports_ = pp_config.num_ingress_ports();
  num_egress_ports_ = pp_config.num_egress_ports();
  instance_id_ = pp_config.instance_id();
  for (int i = 0; i < num_ingress_ports_; ++i)
    ingress_ports_.emplace_back(nullptr);
  for (int j = 0; j < num_egress_ports_; ++j)
    egress_ports_.emplace_back(nullptr);
  PacketProcessor::ConfigurePorts(pp_config, this);

  std::string _pattern1 = "(.*)(((%)[A-Z,a-z,0-9][A-Z,a-z,0-9]){5,})(.*)";
  std::string _pattern2 = "(.*)(admin)(.*)(notexist)(.*)";
  _url_regex1 = new std::regex(_pattern1);
  _url_regex2 = new std::regex(_pattern2);
  _cpu_ctr.setfname("ValidateURL"+std::to_string(instance_id_));
}

inline bool
ValidateURL::process(struct rte_mbuf *rx_packet) {
	struct ether_hdr* eth = nullptr;
    struct ipv4_hdr* ip   = nullptr;
    struct tcp_hdr* tcp   = nullptr;
    char* payload         = nullptr;
    char* payload1         = nullptr;
    char* payload2         = nullptr;
	int offset = 0;

	head = nullptr;
	tail = nullptr;

	eth = rte_pktmbuf_mtod(rx_packet, struct ether_hdr*);
	ip  = reinterpret_cast<ipv4_hdr*>(eth + 1);
	tcp = reinterpret_cast<tcp_hdr*>(ip + 1);
	payload = reinterpret_cast<char *>(tcp) + 8;

	/* find the head and tail of the url in the packet here */
	if(strncmp(payload, "GET", 3) == 0) {
    	offset = 4;
	} else if(!strncmp(payload, "POST", 4) || !strncmp(payload, "HEAD", 4)) {
    	offset = 5;
	} else if(!strncmp(payload, "OPTIONS", 7) || !strncmp(payload, "CONNECT", 7)) {
    	offset = 8;
	} else {
    	return false;
	}   

	head = payload + offset;
	tail = strchr(head, ' '); 
	std::string url(head, tail);

	if (std::regex_match(url, *_url_regex1) || std::regex_match(url, *_url_regex2)) {
	    return false;
	}

	return true;
}

void
ValidateURL::Run() {
    rx_pkt_array_t rx_packets;
    uint16_t num_rx = 0;
    uint16_t num_tx = 0;
    register int i = 0;

    while (true) {
       num_rx = ingress_ports_[0]->RxBurst(rx_packets);

       /* ADDED FOR MEASURING CPU CYCLES */
       //_cpu_ctr.update();
       /*#################################*/

       for (i = 0; i < num_rx && i < k_num_prefetch_; ++i)
          rte_prefetch_non_temporal(rte_pktmbuf_mtod(rx_packets[i], void*));      
       for (i = 0; i < num_rx - k_num_prefetch_; ++i) {
          rte_prefetch_non_temporal(rte_pktmbuf_mtod(rx_packets[i + k_num_prefetch_], void*));         
          process(rx_packets[i]);
       }
       for ( ; i < num_rx; ++i) {
          process(rx_packets[i]);
       }

       /* added for counting cycles 
          if(num_rx > 1) {
          _cpu_ctr.addmany(num_rx);
          } else {
          _cpu_ctr.end_rdtsc();
          }
       ############################*/

       num_tx = egress_ports_[0]->TxBurst(rx_packets, num_rx);
    }
}

void ValidateURL::FlushState() {}
void ValidateURL::RecoverState() {}
