#include "packet_processor_factory.h"

#include <rte_malloc.h>

PacketProcessorFactory* PacketProcessorFactory::GetInstance() {
  static std::unique_ptr<PacketProcessorFactory> instance(nullptr);
  if (instance.get() == nullptr) {
    instance.reset(reinterpret_cast<PacketProcessorFactory*>(
        rte_zmalloc("PacketProcessorFactory", sizeof(PacketProcessorFactory),
                    RTE_CACHE_LINE_SIZE)));
  }
  return instance.get();
}

std::unique_ptr<PacketProcessor> PacketProcessorFactory::CreatePacketProcessor(
    const std::string& packet_processor_type) {
  std::unique_ptr<PacketProcessor> pp_ptr(nullptr);
  if (packet_processor_type == "MacSwapper") {
    pp_ptr = CreatePacketProcessorInternal<MacSwapper>();
  } else if (packet_processor_type == "TCPIPClassifier") {
    pp_ptr = CreatePacketProcessorInternal<TCPIPClassifier>();
  } else if (packet_processor_type == "ComputePktSzHist") {
    pp_ptr = CreatePacketProcessorInternal<ComputePktSzHist>();
  } else if (packet_processor_type == "CountSubnetPackets") {
    pp_ptr = CreatePacketProcessorInternal<CountSubnetPackets>();
  } else if (packet_processor_type == "DropPacket") {
    pp_ptr = CreatePacketProcessorInternal<DropPacket>();
  } else if (packet_processor_type == "HeaderParser") {
    pp_ptr = CreatePacketProcessorInternal<HeaderParser>();
  } else if (packet_processor_type == "Looper") {
    pp_ptr = CreatePacketProcessorInternal<Looper>();
  } else if (packet_processor_type == "Sleepy") {
    pp_ptr = CreatePacketProcessorInternal<Sleepy>();
  } else if (packet_processor_type == "CheckHeader") {
    pp_ptr = CreatePacketProcessorInternal<CheckHeader>();
  } else if (packet_processor_type == "ClassfyHTTP") {
    pp_ptr = CreatePacketProcessorInternal<ClassfyHTTP>();
  } else if (packet_processor_type == "ValidateURL") {
    pp_ptr = CreatePacketProcessorInternal<ValidateURL>();
  } else if (packet_processor_type == "FiltUDPTCP") {
     pp_ptr = CreatePacketProcessorInternal<FiltUDPTCP>();
  } else if (packet_processor_type == "CountURL") {
     pp_ptr = CreatePacketProcessorInternal<CountURL>();
  } else if (packet_processor_type == "MacRewriter") {
     pp_ptr = CreatePacketProcessorInternal<MacRewriter>();
  } else if (packet_processor_type == "IPTtlDecrementer") {
     pp_ptr = CreatePacketProcessorInternal<IPTtlDecrementer>();
  }
  return std::move(pp_ptr);
}

template <class T>
std::unique_ptr<PacketProcessor>
PacketProcessorFactory::CreatePacketProcessorInternal() {
  auto pp_ptr = std::unique_ptr<PacketProcessor>(new T());
  return std::move(pp_ptr);
}
