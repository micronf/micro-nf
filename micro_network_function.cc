#include "common/api_server.h"
#include "common/packet_processor.h"
#include "packet-processors/hello_world.h"

#include <rte_eal.h>

int main(int argc, char* argv[]) {
  rte_eal_init(argc, argv);
  PacketProcessorConfig config;
  config.add_ingress_queue_id("queue0");
  config.add_egress_queue_id("queue1");
  MacSwapper mac_swapper;
  mac_swapper.Init(config);
  mac_swapper.Run();
  return 0;
}
