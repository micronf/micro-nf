# micro-nf
Microservices for building network functions.

## BUILD and RUN
```
$ make clean && make
$ sudo ./build/micronf -c <cpumask> -n <num-memory-channels> -- --config-file=<config-file-path>``
```

Example:
```
$ sudo ./build/micronf -c 0x20 -n 2 -- --config-file=./confs/mac_swapper.conf
```
