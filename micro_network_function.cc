#include "common/api_server.h"
#include "common/packet_processor.h"
#include "packet-processors/mac_swapper.h"
#include "port/rte_ingress_port.h"
#include "port/rte_egress_port.h"

#include <rte_eal.h>

int main(int argc, char* argv[]) {
  rte_eal_init(argc, argv);
  PacketProcessorConfig config;
  config.add_ingress_queue_id("MProc_Userv_1_RX");
  PacketProcessor<RteIngressPort, RteEgressPort>* mac_swapper = new MacSwapper();
  mac_swapper->Init(config);
  mac_swapper->Run();
  return 0;
}
