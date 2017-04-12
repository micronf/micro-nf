#include "header_parser.h"
#include <rte_ether.h>
#include <rte_ip.h>
#include <rte_memcpy.h>
#include <rte_tcp.h>
#include <rte_udp.h>
#include "common/protocol_header_offset.h"

inline void HeaderParser::Init(const PacketProcessorConfig& pp_config) {
  num_ingress_ports_ = pp_config.num_ingress_ports();
  num_egress_ports_ = pp_config.num_egress_ports();
  instance_id_ = pp_config.instance_id();
  assert(num_ingress_ports_ == 1);
  assert(num_egress_ports_ == 1);
  for (int i = 0; i < num_ingress_ports_; ++i)
    ingress_ports_.emplace_back(nullptr);
  for (int j = 0; j < num_egress_ports_; ++j)
    egress_ports_.emplace_back(nullptr);
  PacketProcessor::ConfigurePorts(pp_config, this);
}

inline void HeaderParser::Run() {
  rx_pkt_array_t rx_packets;
  register uint16_t i = 0;
  uint16_t num_rx = 0;
  uint8_t* mdata_ptr = nullptr;
  struct ProtocolHeaderOffset header_offsets;
  struct ether_hdr* eth = nullptr;
  struct ipv4_hdr* ip = nullptr;
  struct tcp_hdr* tcp = nullptr;
  const uint8_t* kHeaderOffsetPtr =
      reinterpret_cast<const uint8_t*>(header_offsets.data);
  const uint16_t kEtherTypeIPv4 = rte_cpu_to_be_16(0x0800);
  while (true) {
    num_rx = ingress_ports_[0]->RxBurst(rx_packets);
    for (i = 0; i < num_rx; ++i) {
      rte_prefetch0(rx_packets[i]->buf_addr);
      eth = rte_pktmbuf_mtod(rx_packets[i], struct ether_hdr*);
      header_offsets.eth_hdr_addr = reinterpret_cast<uint64_t>(eth);
      header_offsets.ip_hdr_addr =
          header_offsets.eth_hdr_addr + sizeof(struct ether_hdr);
      ip = reinterpret_cast<struct ipv4_hdr*>(header_offsets.ip_hdr_addr);
      header_offsets.transport_hdr_addr =
          header_offsets.ip_hdr_addr + sizeof(struct ipv4_hdr);
      if (likely(ip->next_proto_id == IPPROTO_TCP)) {
        tcp = reinterpret_cast<struct tcp_hdr*>(
            header_offsets.transport_hdr_addr);
        header_offsets.payload_addr =
            header_offsets.transport_hdr_addr + tcp->data_off;
      } else if (ip->next_proto_id == IPPROTO_UDP) {
        header_offsets.payload_addr =
            header_offsets.transport_hdr_addr + sizeof(struct udp_hdr);
      } else {
        header_offsets.payload_addr = 0;
      }
      mdata_ptr = reinterpret_cast<uint8_t*>(
          reinterpret_cast<unsigned long>(rx_packets[i]) +
          sizeof(struct rte_mbuf));
      rte_mov32(mdata_ptr, kHeaderOffsetPtr);
    }
    egress_ports_[0]->TxBurst(rx_packets);
  }
}

void HeaderParser::FlushState() {}
void HeaderParser::RecoverState() {}
