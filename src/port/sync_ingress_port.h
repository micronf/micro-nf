#ifndef _SYNC_INGRESS_PORT_H_
#define _SYNC_INGRESS_PORT_H_

#include "port.h"

#include <memory>
#include <queue>

class SyncIngressPort : public IngressPort {
 public:
  SyncIngressPort() {}
  void Init(std::map<std::string, std::string>& port_config) override;
  int RxBurst(rx_pkt_array_t& packets) override;
  const static std::string kConfNumPrevMs;

 private:
  int num_prev_ms_;
  std::queue<struct rte_mbuf*> pending_mbuf_q_;
  rte_ring* rx_ring_;
};
#endif  // _SYNC_INGRESS_PORT_H_
