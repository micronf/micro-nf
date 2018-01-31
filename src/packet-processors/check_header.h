#ifndef _CHECK_HEADER_H_
#define _CHECK_HEADER_H_

#include "../port/rte_egress_port.h"
#include "../port/rte_ingress_port.h"
#include "packet_processor.h"
#include "../util/cyclcounter.h"

class CheckHeader : public PacketProcessor {
  public:
    CheckHeader() {}
    ~CheckHeader() {} 
    void Init(const PacketProcessorConfig& pp_config) override;
    void Run() override;
	bool process(struct rte_mbuf *);
    void FlushState() override;
    void RecoverState() override;
  private:
	cyclcounter _cpu_ctr;
};

#endif // _CHECK_HEADER_H_
