cd ./src/packet-processors/
protoc --proto_path=./ --cpp_out=./ ./packet_processor_config.proto
cd ../../
make
cp build/micronf ../micro-nf-datapath/exec/
