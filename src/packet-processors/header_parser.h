#ifndef _HEADER_PARSER_H_
#define _HEADER_PARSER_H_

#include "packet_processor.h"

class HeaderParser : public PacketProcessor {
  private:
    HeaderParser() {}
    void Init(const PacketProcessorConfig& pp_config) override;
    void Run() override;
    void FlushState() override;
    void RecoverState() override;
    virtual ~HeaderParser() {}
};

#endif // _HEADER_PARSER_H_
