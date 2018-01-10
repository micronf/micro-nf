#ifndef _COUNT_URL_H_
#define _COUNT_URL_H_

#include "packet_processor.h"
#include "../port/rte_egress_port.h"
#include "../port/rte_ingress_port.h"
#include <map>
//#include <string>

class count_url : public PacketProcessor {
public:
    count_url(){}
    void Init(const PacketProcessorConfig& pp_config) override;
    void Run() override;
    void FlushState() override;
    void RecoverState() override;
private:
    std::map<std::string, double> _urlmap;
};

#endif // _COUNT_URL_H_
