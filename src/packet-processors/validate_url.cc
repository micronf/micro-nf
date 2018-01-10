#include "validate_url.h"
#include <rte_ether.h>
#include <rte_ip.h>
#include <rte_memcpy.h>
#include <rte_tcp.h>
#include <rte_udp.h>
 
inline void validate_url::Init(const PacketProcessorConfig& pp_config) {
  num_ingress_ports_ = pp_config.num_ingress_ports();
  num_egress_ports_ = pp_config.num_egress_ports();
  instance_id_ = pp_config.instance_id();
  //assert(num_ingress_ports_ == 1);
  //assert(num_egress_ports_ == 1);
  for (int i = 0; i < num_ingress_ports_; ++i)
    ingress_ports_.emplace_back(nullptr);
  for (int j = 0; j < num_egress_ports_; ++j)
    egress_ports_.emplace_back(nullptr);
  PacketProcessor::ConfigurePorts(pp_config, this);
}


void
validate_url::Run() {
    rx_pkt_array_t rx_packets;
    uint16_t num_rx = 0;
    uint16_t num_tx = 0;
    register int i = 0;
    struct ether_hdr* eth = nullptr;
    struct ipv4_hdr* ip   = nullptr;
    struct tcp_hdr* tcp   = nullptr;
    char* payload         = nullptr;


    //std::string pattern = "^(https?:\\/\\/)?([\\da-z\\.-]+)\\.([a-z\\.]{2,6})([\\/\\w \\.-]*)*([a-    z\\!\\@\\#\\$\\%\\^\\&\\*])*\\/?$";
	std::string pattern = "^(https?:\\/\\/)?([\\da-z\\.-]+)\\.([a-z\\.]{2,6})([\\/\\w \\.-]*)*([a-z\\!\\@\\#\\$\\%\\^\\&\\*])*\\/?$";
	std::regex url_regex(pattern);

    while (true) {
        //for (auto& iport : this->ingress_ports_) {
        num_rx = ingress_ports_[0]->RxBurst(rx_packets);
        for (i = 0; i < num_rx; ++i) {
			head = nullptr;
			tail = nullptr;

			rte_prefetch0(rx_packets[i]->buf_addr);
            eth = rte_pktmbuf_mtod(rx_packets[i], struct ether_hdr*);
            ip  = reinterpret_cast<ipv4_hdr*>(eth + 1);
            tcp = reinterpret_cast<tcp_hdr*>(ip + 1);
            payload = reinterpret_cast<char*>(tcp + 1);
		
            //++pkt_size_bucket_[rx_packets[i]->pkt_len / this->kBucketSize];
            /* find the head and tail of the url in the packet here */
    
			head = payload + 1;
			tail = head + 10;
            std::string url(head, tail);
            if (!std::regex_match(url, url_regex)) {
				//printf("invalid url!\n");
				//fflush(stdout);
            }
        }
        num_tx = egress_ports_[0]->TxBurst(rx_packets, num_rx);
        //}
 
	}
}

void validate_url::FlushState() {}
void validate_url::RecoverState() {}
