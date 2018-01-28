#ifndef _VALIDATE_URL_H_
#define _VALIDATE_URL_H_
 
#include "packet_processor.h"
#include <regex>
#include <string>
 
class ValidateURL : public PacketProcessor {
public:
    ValidateURL() {}
    void Init(const PacketProcessorConfig& pp_config) override;
    void Run() override;
    void FlushState() override;
    void RecoverState() override;
private:
    char *head;
    char *tail;
};
 
#endif // _VALIDATE_URL_H_
