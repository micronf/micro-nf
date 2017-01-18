#include "common/api_server.h"
#include "common/packet_processor.h"
#include "packet_processors/hello_world.h"

int main(int argc, char*[] argv) {
  PacketProcessorConfig config;
  HelloWorld hw_processor;
  hw_processor.Init(config);
  hw_processor.Run();
  return 0;
}
