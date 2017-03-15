#include "null_ingress_port.h"

void NullIngressPort::Init(std::map<std::string, std::string>& port_config) {}
int NullIngressPort::RxBurst(rx_pkt_array_t& packets) { return 0; }
