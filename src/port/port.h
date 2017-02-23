#ifndef _PORT_H_
#define _PORT_H_

#include "egress_port.h"
#include "ingress_port.h"

#include <rte_mbuf.h>
#include <rte_ring.h>

#define MDATA_PTR(mbuf)                                                        \
  reinterpret_cast<char *>(reinterpret_cast<unsigned long>((mbuf)) +           \
                           sizeof(struct rte_mbuf))
#endif // _PORT_H_
