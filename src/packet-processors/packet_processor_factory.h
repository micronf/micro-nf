#ifndef _PACKET_PROCESSOR_FACTORY_H_
#define _PACKET_PROCESSOR_FACTORY_H_

#include "packet_processors.h"

#include <memory>

class PacketProcessorFactory {
  public:
    // Delete the copy constructor and assignment operator to ensure that an
    // object of this class cannot be instantiated from another object of the
    // same class.
    PacketProcessorFactory(const PacketProcessorFactory&) = delete;
    void operator=(const PacketProcessorFactory&) = delete;

    // Returns an instance of the class. If an instance does not exists already
    // it is created, otherwise the same instance is returned everytime
    // GetInstance is invoked.
    static PacketProcessorFactory* GetInstance();

    // Create an uninitialized packet processor object of type
    // packet_processor_type.
    std::unique_ptr<PacketProcessor> CreatePacketProcessor(
        const std::string& packet_processor_type);

  private:
    // The private constructor ensures that an object of this class cannot be
    // instantiated in either heap or stack.
    PacketProcessorFactory() { }

    // A helper class for instantiating a packet processor. The public interface
    // (the method without the Internal suffix), calls an appropriate template
    // specialization based on the type provided to it.
    template <class T>
    std::unique_ptr<PacketProcessor> CreatePacketProcessorInternal();
};

#endif // _PACKET_PROCESSOR_CONFIG_H_
