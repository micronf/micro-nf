#include "common/api_server.h"
#include "packet-processors/packet_processor.h"
#include "packet-processors/tcp_ip_classifier.h"
#include "packet-processors/mac_swapper.h"
#include "port/rte_ingress_port.h"
#include "port/rte_egress_port.h"

#include <rte_eal.h>
#include <rte_ethdev.h>

int main(int argc, char* argv[]) {
  rte_eal_init(argc, argv);
  int num_dev = rte_eth_dev_count();
  printf("Num devices = %d\n", num_dev);
  return 0;
}
