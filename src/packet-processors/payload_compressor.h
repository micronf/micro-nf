#ifndef _PAYLOAD_COMPRESSOR_H_
#define _PAYLOAD_COMPRESSOR_H_

class PayloadCompressor : public PacketProcessor {
 public:
  void Init(const PacketProcessorConfig& config) override;
  void Run() override;
  void FlushState() override;
  void RecoverState() override;
};
#endif  // _PAYLOAD_COMPRESSOR_H_
