#ifndef _COUNT_SUBNET_PACKET_H_
#define _COUNT_SUBNET_PACKET_H_

#include "packet_processor.h"
#include "util/cidr_address.h"

class CountSubnetPackets : public PacketProcessor {
 public:
  CountSubnetPackets() {}
  void Init(const PacketProcessorConfig& pp_config) override;
  void Run() override;
  void FlushState() override;
  void RecoverState() override;
  void AddSubnet(const std::string& subnet_str);
  virtual ~CountSubnetPackets(){};

 private:
  std::vector<std::pair<CIDRAddress, uint64_t>> subnet_pkt_counter_;
  std::vector<uint16_t> i_to_e_port_map_;
};

#endif  // _COUNT_SUBNET_PACKET_H_
