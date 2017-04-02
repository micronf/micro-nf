#ifndef _COMPUTE_PKT_SZ_HIST_H_
#define _COMPUTE_PKT_SZ_HIST_H_

#include "packet_processor.h"

#include <vector>

class ComputePktSzHist : public PacketProcessor {
 public:
  ComputePktSzHist() : kNumBuckets(1500 / 20 + 1), kBucketSize(20) {}
  void Init(const PacketProcessorConfig& pp_config) override;
  void Run() override;
  void FlushState() override;
  void RecoverState() override;
  const std::vector<unsigned long>& pkt_size_bucket() const {
    return pkt_size_bucket_;
  }

  virtual ~ComputePktSzHist() { }

 private:
  const int kNumBuckets;
  const int kBucketSize;
  std::vector<unsigned long> pkt_size_bucket_;
};
#endif  // _COMPUTE_PKT_SZ_HIST_H_
