#ifndef _SYNC_INGRESS_PORT_H_
#define _SYNC_INGRESS_PORT_H_

#include "port.h"

#include <memory>
#include <queue>

class SyncIngressPort : public IngressPort {
  public:
    SyncIngressPort() {}
    SyncIngressPort(int num_prev_ms, const std::string& ring_id);
    int RxBurst(rx_pkt_array_t& packets) override;
  private:
    // Helper function to count the number of bits set in the bitmap pointed by
    // mdata_ptr. Number of set bitmaps indicate the number of microservices
    // that has completed processing this packet.
    int CountNumComplete(const char* mdata_ptr);

    int num_prev_ms_;

    // Num bitmap entries is derived from num_prev_ms_.
    int num_bitmap_entries_;
    std::unique_ptr<unsigned char> mask_;
    std::queue <struct rte_mbuf*> pending_mbuf_q_;
    rte_ring* rx_ring_;
};
#endif // _SYNC_INGRESS_PORT_H_
