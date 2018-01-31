#ifndef _VALIDATE_URL_H_
#define _VALIDATE_URL_H_
 
#include "packet_processor.h"
#include "../util/cyclcounter.h"
#include <regex>
#include <string>
 
class ValidateURL : public PacketProcessor {
public:
    ValidateURL() {}
    ~ValidateURL();
    void Init(const PacketProcessorConfig& pp_config) override;
    void Run() override;
    bool process(struct rte_mbuf *);
    void FlushState() override;
    void RecoverState() override;
private:
    char *head;
    char *tail;
    //std::string _pattern;
    std::regex *_url_regex1;
    std::regex *_url_regex2;
	cyclcounter _cpu_ctr;

};
 
#endif // _VALIDATE_URL_H_
