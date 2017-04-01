#include "compute_pkt_sz_hist.h"
#include <google/protobuf/map.h>
#include "../port/port_factory.h"

inline void ComputePktSzHist::Init(const PacketProcessorConfig& pp_config) {
  this->num_ingress_ports_ = pp_config.num_ingress_ports();
  this->num_egress_ports_ = pp_config.num_egress_ports();

  assert(this->num_egress_ports_ == 1);

  ConfigurePorts(pp_config);
  for (int i = 0; i < pkt_size_bucket_.size(); ++i) pkt_size_bucket_[i] = 0;
}

inline void ComputePktSzHist::Run() {
  rx_pkt_array_t rx_packets;
  tx_pkt_array_t tx_packets;
  while (true) {
    for (auto& iport : this->ingress_ports_) {
      int num_rx = iport->RxBurst(rx_packets);
      int i = 0;
      for (; i < num_rx; ++i) {
        ++pkt_size_bucket_[rx_packets[i]->pkt_len / this->kBucketSize];
        tx_packets[i] = rx_packets[i];
      }
      this->egress_ports_[0].TxBurst(tx_packets);
    }
  }
}

inline void ComputePktSzHist::FlushState() {}
inline void ComputePktSzHist::RecoverState() {}
