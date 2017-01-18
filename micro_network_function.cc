#include "common/api_server.h"
#include "common/packet_processor.h"
#include "packet-processors/hello_world.h"

#include <rte_eal.h>

int main(int argc, char* argv[]) {
  rte_eal_init(argc, argv);
  PacketProcessorConfig config;
  HelloWorld hw_processor;
  hw_processor.Init(config);
  hw_processor.Run();
  return 0;
}
