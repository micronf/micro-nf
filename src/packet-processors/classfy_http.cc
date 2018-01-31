#include "classfy_http.h"
#include <rte_ether.h>
#include <rte_ip.h>
#include <rte_memcpy.h>
#include <rte_tcp.h>
#include <rte_udp.h>
#include <rte_byteorder.h>

ClassfyHTTP::~ClassfyHTTP(){
	delete _url_regex;
}

inline void ClassfyHTTP::Init(const PacketProcessorConfig& pp_config) {
  num_ingress_ports_ = pp_config.num_ingress_ports();
  num_egress_ports_ = pp_config.num_egress_ports();
  instance_id_ = pp_config.instance_id();
  assert(num_ingress_ports_ == 1);
  assert(num_egress_ports_ == 2);
  for (int i = 0; i < num_ingress_ports_; ++i)
    ingress_ports_.emplace_back(nullptr);
  for (int j = 0; j < num_egress_ports_; ++j)
    egress_ports_.emplace_back(nullptr);
  PacketProcessor::ConfigurePorts(pp_config, this);
  _cpu_ctr.setfname("ClassfyHTTP"+std::to_string(instance_id_));
//  _pattern = "(GET )(https?:\\/\\/)([a-z\\.\\/]*)( )(HTTP\\/1.[01] )(Host:)( )(.*)";
  std::string pattern = "(?:POST|GET|HEAD|OPTIONS|CONNECT)\\s[\\S]+\\s(HTTP\\/1.1)\\s[\\S](.*)";
  _url_regex = new std::regex(pattern);
}

inline bool
ClassfyHTTP::process(struct rte_mbuf *rx_packet) {
	struct ether_hdr* eth = nullptr;
    struct ipv4_hdr* ip = nullptr;
    struct tcp_hdr* tcp = nullptr;
    char* payload = nullptr;
    uint32_t offset = 0;
	bool _match = false;

	head = nullptr;
	tail = nullptr;
	eth = rte_pktmbuf_mtod(rx_packet, struct ether_hdr*);
	ip  = reinterpret_cast<ipv4_hdr*>(eth + 1);
	tcp = reinterpret_cast<tcp_hdr*>(ip + 1);
	payload = reinterpret_cast<char*>(tcp) + 8;
	offset = sizeof(struct ether_hdr) + sizeof(struct ipv4_hdr) + (tcp->data_off & 0xf0) >> 2;

	/* find the head and tail of the url in the packet here */
	head = payload;
	//tail = payload + rx_packets[i]->pkt_len - offset; // original 15
	//tail = payload + 50;
/*	std::string url(head);
	int length = url.size() > 170? 170 : url.size();
	url = std::string(head, length);
*/
	std::string url(head, strnlen(head, 200));

	/* ADDED FOR MEASURING CPU CYCLES */
        //_cpu_ctr.update();
    /*#################################*/
	_match = std::regex_match(url, *_url_regex);

	//_cpu_ctr.addone();

	/* added for counting cycles */ 
	if(_match) {
		return true;
	} else {
		return false;
	}
	/*############################*/


	//std::string url(head, tail);
/*	std::string url(head);
	if (std::regex_match(url, *_url_regex)) {
		return true;
	}

	// does not match
	return false;	
*/
}

void
ClassfyHTTP::Run() {
    rx_pkt_array_t rx_packets;
    rx_pkt_array_t tx_packets_1;
    rx_pkt_array_t tx_packets_2;
    uint16_t num_rx = 0;
    uint16_t num_tx_1 = 0;
    uint16_t num_tx_2 = 0;
    register int i = 0;
    
    while (true) {
       num_rx = ingress_ports_[0]->RxBurst(rx_packets);
       num_tx_1 = 0;
       num_tx_2 = 0;
		
      for (i = 0; i < num_rx && i < k_num_prefetch_; ++i)
         rte_prefetch_non_temporal(rte_pktmbuf_mtod(rx_packets[i], void*));

      for (i = 0; i < num_rx - k_num_prefetch_; ++i) {
         rte_prefetch_non_temporal(rte_pktmbuf_mtod(rx_packets[i + k_num_prefetch_], void*));
          if(process(rx_packets[i])){
             tx_packets_1[num_tx_1++] = rx_packets[i];
          } else {
             tx_packets_2[num_tx_2++] = rx_packets[i];
          }
       }

      for ( ; i < num_rx; ++i) {
         if(process(rx_packets[i])){
            tx_packets_1[num_tx_1++] = rx_packets[i];
         } else {
            // Packets to be dropped
            tx_packets_2[num_tx_2++] = rx_packets[i];
         }
      }

		
       num_tx_1 = egress_ports_[0]->TxBurst(tx_packets_1, num_tx_1);
       num_tx_2 = egress_ports_[1]->TxBurst(tx_packets_2, num_tx_2);
    }
}

void ClassfyHTTP::FlushState() {}
void ClassfyHTTP::RecoverState() {}
