#include "null_egress_port.h"

int NullEgressPort::TxBurst(void** packets, int burst_size) {
  return 0;
}
