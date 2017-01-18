#include "null_egress_port.h"

uint16_t NullEgressPort::TxBurst(void** packets, uint16_t burst_size) {
  return 0;
}
