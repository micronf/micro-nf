#ifndef _CLASSFY_HTTP_H_
#define _CLASSFY_HTTP_H_

#include "packet_processor.h"
#include <regex>
#include <string>
#include "../util/cyclcounter.h"

class ClassfyHTTP : public PacketProcessor {
public:
    ClassfyHTTP() {}
    ~ClassfyHTTP();
    void Init(const PacketProcessorConfig& pp_config) override;
    void Run() override;
	bool process(struct rte_mbuf *);
    void FlushState() override;
    void RecoverState() override;
private:
    char *head;
    char *tail;
	cyclcounter _cpu_ctr;
    //std::string _pattern;
    std::regex *_url_regex;
};

#endif // _CLASSFY_HTTP_H_
