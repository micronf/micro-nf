# micro-nf
Microservices for building network functions.

## Kernel configurations

### Hugepage configuration

- Open /etc/grub
- Append the following at the end of GRUB_CMDLINE_LINUX_DEFAULT variable's value
  to allocate 8 1G hugepages.
```
default_hugepagesz=1G hugepagesz=1G hugepages=8
```
- Reboot the machine
- Mount hugetlbfs
```
$ mkdir /mnt/huge
$ mount -t hugetlbfs nodev /mnt/huge
```

The mount point can be made permanent across reboots, by adding the following 
line to the /etc/fstab file:
```
nodev /mnt/huge hugetlbfs defaults 0 0
```

For 1GB pages, the page size must be specified as a mount option:
```
nodev /mnt/huge_1GB hugetlbfs pagesize=1GB 0 0
```
### CPU Isolation

### Set CPU scaling governer to "performance"

### Disable Address Space Layout Randomization (ASLR)
To temporarily disable ASLR:
```
echo 0 > /proc/sys/kernel/randomize_va_space
```
To make the change permanent across boots add the following at the end of
/etc/sysctl.conf:
```
kernel.randomize_va_space=0
```
To immidiately apply changes run:
```
sysctl -p
```

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
