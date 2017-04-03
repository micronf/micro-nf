#include "compute_pkt_sz_hist.h"
#include <google/protobuf/map.h>

inline void ComputePktSzHist::Init(const PacketProcessorConfig& pp_config) {
  this->num_ingress_ports_ = pp_config.num_ingress_ports();
  this->num_egress_ports_ = pp_config.num_egress_ports();
  assert(this->num_egress_ports_ == 1);
  PacketProcessor::ConfigurePorts(pp_config, this);
  pkt_size_bucket_.resize(kNumBuckets, 0);
}

inline void ComputePktSzHist::Run() {
  rx_pkt_array_t rx_packets;
  tx_pkt_array_t tx_packets;
  uint16_t num_rx = 0;
  register int i = 0;
  while (true) {
    for (auto& iport : this->ingress_ports_) {
      num_rx = iport->RxBurst(rx_packets);
      for (i = 0; i < num_rx; ++i) {
        ++pkt_size_bucket_[rx_packets[i]->pkt_len / this->kBucketSize];
        tx_packets[i] = rx_packets[i];
      }
      this->egress_ports_[0]->TxBurst(tx_packets);
    }
  }
}

inline void ComputePktSzHist::FlushState() {}
inline void ComputePktSzHist::RecoverState() {}
