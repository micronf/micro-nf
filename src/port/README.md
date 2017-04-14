Inside each .cc file look for variable names that start with "kConf". This
variable defines a configuration key specific to this port. Ports also inherit
configuration keys from their parent, i.e., IngressPort or EgressPort. If there
are additional configuration keys that is defined in an ad-hoc way, that will be
described as part of that corresponding port.

Additional note for parallel microservices. When a branching is created from a
microservice to a stage of parallel microservices then each egress port in the
parallel stage should have an id starting from 0 to the
number_of_microservices_that_are_parallel - 1. This is to ease the indexing in
the bitmap entry. So this note applies for MarkAndForwardEgressPort and
SetBitmapEgressPort.

## BranchEgressPortZC (branch_egress_port_zc.h)
An egress port that takes care of branching between parallel microservices.
Additional configuration keys are, "0", "1", "2", ..., "kConfNumBranches - 1",
where a key "i" corresponds to the name of the "i"-th ring.

## MarkAndForwardEgressPort (mark_and_forward_egress_port.h)
An egress port that sets the bitmap to mark that it's corresponding microservice
has done processing the packet. It also puts the packet in the ring shared with
the next microservice. This is used with parallel microservices.

## SetBitmapEgressPort (set_bitmap_egress_port.h)
This egress port only sets the bitmap to mark that it's corresponding
microservice has finished processing the packet. It does not put the packet into
the shared ring. It is used with parallel microservices. If there are n parallel
microservices then one of them will have MarkAndForwardEgressPort while the
others will have SetBitmapEgressPort to the next stage.

## RteEgressPort (rte_egress_port.h)
A regular egress port implemented using a shared memory ring. This is the most
common type of EgressPort in use. It is used to connect a pair of microservices
without parallelism.

## RteNICEgressPort (rte_nic_egress_port.h)
An egress port that is directly connected to the NIC's tx queue.

## RteIngressPort (rte_ingress_port.h)
A regular ingress port that pulls packets from a shared ring.

## RteNICIngressPort (rte_nic_ingress_port.h)
An ingress port that pulls packets directly from the NIC's rx queue.

## SyncIngressPort (sync_ingress_port.h)
This ingress port is connected to a microservice that is preceeded by some
parallel microservices. This port checks the bitmap to ensure that all
microservices have finished processing the packet.
