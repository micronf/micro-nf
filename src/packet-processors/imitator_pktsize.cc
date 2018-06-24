#include "imitator_pktsize.h"

#include <assert.h>
#include <google/protobuf/map.h>
#include <rte_ether.h>
#include <rte_prefetch.h>

#define uint32_t_to_char(ip, a, b, c, d) do {\
   *a = (unsigned char)(ip >> 24 & 0xff);\
   *b = (unsigned char)(ip >> 16 & 0xff);\
   *c = (unsigned char)(ip >> 8 & 0xff);\
   *d = (unsigned char)(ip & 0xff);\
   } while (0)

inline void ImitatorPktsize::Init(const PacketProcessorConfig& pp_config) {
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
   
   // Retrieve pp_params config
   auto pp_param_map = pp_config.pp_parameters();
   auto it = pp_param_map.find( PacketProcessor::compLoad );
   if ( it !=  pp_param_map.end() )
      comp_load_ = it->second;

   it = pp_param_map.find( PacketProcessor::shareCoreFlag );
   if ( it != pp_param_map.end() )
      share_core_ = it->second;
   
   it = pp_param_map.find( "debug" );
   if ( it !=  pp_param_map.end() )
      debug_ = it->second;

   it = pp_param_map.find( PacketProcessor::yieldAfterBatch );
   if ( it !=  pp_param_map.end() )
      yield_after_kbatch_ = it->second;

   it = pp_param_map.find( PacketProcessor::kNumPrefetch );
   if ( it != pp_param_map.end() )
      k_num_prefetch_ = it->second;

   it = pp_param_map.find( PacketProcessor::compLoad );
   if ( it !=  pp_param_map.end() )
      base_load_ = it->second;
	
   RTE_LOG( INFO, PMD, "k_num_prefetch_ : %d\n", k_num_prefetch_);   

   PacketProcessor::ConfigurePorts(pp_config, this);
}

inline void ImitatorPktsize::Run() {
   rx_pkt_array_t rx_packets;
   register int16_t i = 0;
   struct ether_hdr* eth_hdr = nullptr;
   struct ipv4_hdr* ip_hdr = nullptr;
   uint16_t num_rx = 0;
   int res = 0;
   uint32_t hit_count = 1;
   uint32_t sample_counter = 0;
   uint32_t mbuf_pkt_len;
   unsigned char a, b, c, d;
   uint16_t ethertype;

   while ( true ) {
      num_rx = this->ingress_ports_[0]->RxBurst(rx_packets);

      // If num_rx == 0 -> yield
      // Otherwise, try again and until k consecutive hits and then yield
      if ( share_core_ ) {
         if ( num_rx == 0 || hit_count == yield_after_kbatch_ ) {
            hit_count = 1;
            res = sched_yield();
            if ( unlikely( res == -1 ) ) {
               RTE_LOG( ERR, MICRONF, "sched_yield failed! Exitting." );
               return;
            }
         }
         else
            hit_count++;
      }

      for (i = 0; i < num_rx && i < k_num_prefetch_; ++i)
         rte_prefetch_non_temporal(rte_pktmbuf_mtod(rx_packets[i], void*));
      for (i = 0; i < num_rx - k_num_prefetch_; ++i) {
         rte_prefetch_non_temporal(rte_pktmbuf_mtod(rx_packets[i + k_num_prefetch_], void*));

         eth_hdr = rte_pktmbuf_mtod(rx_packets[i], struct ether_hdr*);
         ip_hdr = (ipv4_hdr*)(eth_hdr + 1);
         if ( rte_be_to_cpu_16(eth_hdr->ether_type ) == ETHER_TYPE_IPv4 ) {
            //RTE_LOG( INFO, MICRONF, "total_len: %u\n", rte_be_to_cpu_16(ip_hdr->total_length ) );
            imitate_processing_pktsize( rte_be_to_cpu_16(ip_hdr->total_length ) );
         }
         else {
            //RTE_LOG( INFO, MICRONF, "using mbuf_len: %u\n", rte_pktmbuf_pkt_len( rx_packets[ i ]  ) );
            imitate_processing_pktsize( rte_pktmbuf_pkt_len( rx_packets[ i ] ) );   
         }
      }
      for ( ; i < num_rx; ++i) {
         eth_hdr = rte_pktmbuf_mtod(rx_packets[i], struct ether_hdr*);
         ip_hdr = (ipv4_hdr*)(eth_hdr + 1);
         if ( rte_be_to_cpu_16(eth_hdr->ether_type ) == ETHER_TYPE_IPv4 ) {
            //RTE_LOG( INFO, MICRONF, "total_len: %u\n", rte_be_to_cpu_16(ip_hdr->total_length ) );
            imitate_processing_pktsize( rte_be_to_cpu_16(ip_hdr->total_length ) );
         }
         else {
            //RTE_LOG( INFO, MICRONF, "using mbuf_len\n" );
            imitate_processing_pktsize( rte_pktmbuf_pkt_len( rx_packets[ i ] ) );   
         }

      }
      
      this->egress_ports_[0]->TxBurst(rx_packets, num_rx);
   } 
}

inline void ImitatorPktsize::FlushState() {}
inline void ImitatorPktsize::RecoverState() {}

/*         uint32_t_to_char(rte_bswap32(ip_hdr->src_addr), &a, &b, &c, &d);         
         RTE_LOG( INFO, MICRONF, "IP Src: %hhu.%hhu.%hhu.%hhu ", a, b, c, d );
         RTE_LOG( INFO, MICRONF, "s_mac: %02X:%02X:%02X:%02X:%02X:%02X\n ", 
                  eth_hdr->s_addr.addr_bytes[ 0 ], 
                  eth_hdr->s_addr.addr_bytes[ 1 ],
                  eth_hdr->s_addr.addr_bytes[ 2 ],
                  eth_hdr->s_addr.addr_bytes[ 3 ],
                  eth_hdr->s_addr.addr_bytes[ 4 ],
                  eth_hdr->s_addr.addr_bytes[ 5 ]
            );

         RTE_LOG( INFO, MICRONF, "mbuf_pktlen: %u.  total_len: %u.  eth_type: %X.  inner eth_type: %X\n\n", \
                  mbuf_pkt_len, \
                  rte_be_to_cpu_16(ip_hdr->total_length ), \
                  rte_be_to_cpu_16(eth_hdr->ether_type ), \
                  rte_be_to_cpu_16(ip_hdr->time_to_live ) 
            );
*/
