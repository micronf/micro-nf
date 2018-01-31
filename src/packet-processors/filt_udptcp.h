#ifndef _FILT_UDPTCP_H
#define _FILT_UDPTCP_H

#include "packet_processor.h"
#include "../util/cyclcounter.h"


class FiltUDPTCP : public PacketProcessor {
  public:
    FiltUDPTCP() {}
    void Init(const PacketProcessorConfig& config) override;
    void Run()          override;
	static bool process(struct rte_mbuf *);
    void FlushState()   override;
    void RecoverState() override;
  private:
	cyclcounter _cpu_ctr;
};


#endif // _FILT_UDPTCP_H
