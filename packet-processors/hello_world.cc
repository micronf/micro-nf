#include "hello_world.h"


void HelloWorld::Init(const PacketProcessorConfig& config) {
  this->num_ingress_ports_ = config.ingress_queue_ids_.size();
  this->num_egress_ports_ = config.egress_queue_ids_.size();
}

void HelloWorld::Run() {
  printf("Hello World\n");
}

void HelloWorld::FlushState() {
}

void HelloWorld::RecoverState() {
}
