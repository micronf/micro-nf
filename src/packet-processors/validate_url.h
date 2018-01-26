 #ifndef _VALIDATE_URL_H_
 #define _VALIDATE_URL_H_
 
 #include "packet_processor.h"
 #include <regex>
 #include <string>
 
class validate_url : public PacketProcessor {
public:
    validate_url() {}
    void Init(const PacketProcessorConfig& pp_config) override;
    void Run() override;
    void FlushState() override;
    void RecoverState() override;
private:
    char *head;
    char *tail;
};
 
 #endif

