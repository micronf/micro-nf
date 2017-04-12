#ifndef _PROTOCOL_HEADER_OFFSET_H_
#define _PROTOCOL_HEADER_OFFSET_H_

#include <rte_common.h>
#include <rte_memory.h>

// This struct contains the byte offset of the corresponding ''inner'' protocol
// header from the beginning of data area in an rte_mbuf. More specifically, for
// an rte_mbuf* mbuf, each offset corresponds to the starting offset of the
// header from (mbuf->buf_addr + mbuf->data_off).
struct ProtocolHeaderOffset {
  union {
    int8_t data[32];
    uint64_t eth_hdr_addr;
    uint64_t ip_hdr_addr;
    uint64_t transport_hdr_addr;
    uint64_t payload_addr;
  };
  int8_t __padding__[32];
} __attribute__((__packed__)) __rte_aligned(RTE_CACHE_LINE_SIZE);

#endif  // _PROTOCOL_HEADER_OFFSET_H_
