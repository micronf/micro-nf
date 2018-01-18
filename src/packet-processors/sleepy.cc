#include "sleepy.h"

#include <rte_cycles.h>
#include <rte_ether.h>

const std::string Sleepy::kConfSleepDurationUs = "sleep_duration_us";

inline void Sleepy::Init(const PacketProcessorConfig& pp_config) {
  this->num_ingress_ports_ = pp_config.num_ingress_ports();
  this->num_egress_ports_ = pp_config.num_egress_ports();
  this->instance_id_ = pp_config.instance_id();
  int i = 0;
  for (i = 0; i < this->num_ingress_ports_; ++i)
    this->ingress_ports_.emplace_back(nullptr);
  for (i = 0; i < this->num_egress_ports_; ++i)
    this->egress_ports_.emplace_back(nullptr);
  
  PacketProcessor::ConfigurePorts(pp_config);

  auto pp_param_map = pp_config.pp_parameters();
  auto it = pp_param_map.find( PacketProcessor::shareCoreFlag );
  if ( it != pp_param_map.end() )
     this->share_core_ = it->second;

  it = pp_param_map.find( PacketProcessor::yieldAfterBatch );
  if ( it !=  pp_param_map.end() )
     yield_after_kbatch_ = it->second;
  
  it = pp_param_map.find(Sleepy::kConfSleepDurationUs);
  if ( it != pp_param_map.end() )
     this->sleep_duration_us_ = it->second;
} 

inline void Sleepy::Run() {
   rx_pkt_array_t rx_packets;
   uint16_t num_rx = 0, num_tx = 0;
   register uint16_t i = 0, j = 0;
   struct ether_hdr* eth_hdr = nullptr;
   uint32_t hit_count = 1;
   int res = 0;
   
   while (true) {
      num_rx = ingress_ports_[0]->RxBurst(rx_packets);
      
      // If num_rx == 0 -> yield
      // Otherwise, try again and until k consecutive hits and then yield
      if ( share_core_ ) {
         if ( num_rx == 0 || hit_count == yield_after_kbatch_ ) {
            hit_count = 1;
            res = sched_yield();
            if ( unlikely( res == -1 ) ) {
               std::cerr << "sched_yield failed! Exitting." << std::endl;
               return;
            }
         }
         else
            hit_count++;
      }
      
      for (i = 0; i < num_rx; ++i) {
         eth_hdr = rte_pktmbuf_mtod(rx_packets[i], struct ether_hdr*);
      }
      
      rte_delay_us(this->sleep_duration_us_);

      num_tx = egress_ports_[0]->TxBurst(rx_packets, num_rx);
   }
}

inline void Sleepy::FlushState() {}
inline void Sleepy::RecoverState() {}
