#ifndef _CLASSFY_HTTP_H_
#define _CLASSFY_HTTP_H_

#include "packet_processor.h"
#include <regex>
#include <string>

class classfy_http : public PacketProcessor {
public:
    classfy_http() {}
    void Init(const PacketProcessorConfig& pp_config) override;
    void Run() override;
    void FlushState() override;
    void RecoverState() override;
private:
    char *head;
    char *tail;
};

#endif
