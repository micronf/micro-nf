#ifndef _COUNT_URL_H_
#define _COUNT_URL_H_

#include "packet_processor.h"
#include "../port/rte_egress_port.h"
#include "../port/rte_ingress_port.h"
#include "../util/cyclcounter.h"
#include <map>
#include <string>

class CountURL : public PacketProcessor {
public:
    CountURL(){}
    void Init(const PacketProcessorConfig& pp_config) override;
    void Run() override;
    bool process(struct rte_mbuf *);
    void FlushState() override;
    void RecoverState() override;
private:
    std::map<std::string, double> _urlmap;
	cyclcounter _cpu_ctr;
};

#endif // _COUNT_URL_H_
