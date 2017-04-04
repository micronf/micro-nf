# micro-nf
Microservices for building network functions.

## BUILD and RUN
```
$ make clean && make
$ sudo ./build/micronf -c <cpumask> -n <num-memory-channels> --proc-type <primary|secondary|auto> -- --config-file=<config-file-path>``
```

Example:
```
$ sudo ./build/micronf -c 0x20 -n 2 --proc-type secondary -- --config-file=./confs/mac_swapper.conf
```
