#include "compute_pkt_sz_hist.h"
#include <google/protobuf/map.h>

inline void ComputePktSzHist::Init(const PacketProcessorConfig& pp_config) {
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
  assert(this->num_egress_ports_ == 1);
  PacketProcessor::ConfigurePorts(pp_config, this);
  pkt_size_bucket_.resize(kNumBuckets, 0);

}

inline void ComputePktSzHist::Run() {
  rx_pkt_array_t rx_packets;
  uint16_t num_rx = 0;
  register int i = 0;
  while (true) {
    for (auto& iport : this->ingress_ports_) {
      num_rx = iport->RxBurst(rx_packets);
      for (i = 0; i < num_rx; ++i) 
        ++pkt_size_bucket_[rx_packets[i]->pkt_len / this->kBucketSize];
      this->egress_ports_[0]->TxBurst(rx_packets, num_rx);
    }
		
		//printf("bit: %d\n", this->scale_bits->bits.test(this->instance_id_));
    if(this->scale_bits->bits.test(this->instance_id_)){
        // TODO Change port to smart port. 
    }

  }
}

inline void ComputePktSzHist::FlushState() {}
inline void ComputePktSzHist::RecoverState() {}
