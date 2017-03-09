#include "common/api_server.h"
#include "packet-processors/packet_processor.h"
#include "packet-processors/tcp_ip_classifier.h"
#include "packet-processors/mac_swapper.h"
#include "port/rte_ingress_port.h"
#include "port/rte_egress_port.h"
#include "port/rte_nic_ingress_port.h"

#include <rte_eal.h>
#include <rte_ethdev.h>

int main(int argc, char* argv[]) {
  rte_eal_init(argc, argv);
/*  PacketProcessor* pp = new MacSwapper();
  PacketProcessorConfig pconfig;
  std::unique_ptr<IngressPort> iport(new RteNICIngressPort());
  std::unique_ptr<EgressPort> eport(new RteEgressPort());
  std::map<std::string, std::string> iport_config;
  std::map<std::string, std::string> eport_config;
  
  iport_config["nic_port_id"] = "1";
  iport_config["nic_queue_id"] = "1";
  eport_config["ring_id"] = "r1";
  eport_config["port_id"] = "1";

  iport->Init(iport_config);
  eport->Init(eport_config);

  pp->ingress_ports().push_back(std::move(iport));
  pp->egress_ports().push_back(std::move(eport));

  pp->Run();
  */
  return 0;
}
