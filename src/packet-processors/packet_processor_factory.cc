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
  }
  return std::move(pp_ptr);
}

template <class T>
std::unique_ptr<PacketProcessor>
PacketProcessorFactory::CreatePacketProcessorInternal() {
  auto pp_ptr = std::unique_ptr<PacketProcessor>(new T());
  return std::move(pp_ptr);
}
