# micro-nf
Microservices for building network functions.

## BUILD and RUN
```
$ make clean && make
$ sudo ./build/micronf -c <cpumask> -n <num-memory-channels> --proc-type <primary|secondary|auto> -- --config-file=<config-file-path>``
```
## BUILD protobuf   
```
$ cd micro-nf/src/packet-processors
$ protoc --proto_path=./ --cpp_out=./ ./packet_processor_config.proto    
```
Example:
```
$ sudo ./build/micronf -c 0x20 -n 2 --proc-type secondary -- --config-file=./confs/mac_swapper.conf
```
