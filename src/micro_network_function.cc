#include <fcntl.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>
#include <pthread.h>
#include <rte_common.h>
#include <rte_eal.h>
#include <rte_ethdev.h>
#include <rte_malloc.h>
#include <string.h>
#include <map>
#include <memory>
#include <string>
#include "packet-processors/packet_processor_config.pb.h"
#include "packet-processors/packet_processor_factory.h"
#include "packet-processors/packet_processors.h"
#include "port/port_factory.h"

#include <sched.h>
#define MY_RT_PRIORITY 99

std::unique_ptr<std::map<std::string, std::string>> ParseArgs(int argc,
                                                              char *argv[]) {
  auto ret_map = std::unique_ptr<std::map<std::string, std::string>>(
      new std::map<std::string, std::string>());
  const std::string kDel = "=";
  for (int i = 0; i < argc; ++i) {
    char *key = strtok(argv[i] + 2, kDel.c_str());
    char *val = strtok(NULL, kDel.c_str());
    ret_map->insert(std::make_pair(key, val));
  }
  return std::move(ret_map);
}

int main(int argc, char *argv[]) {

  int args_processed = rte_eal_init(argc, argv);
  argc -= args_processed;
  argv += args_processed;
  auto arg_map = ParseArgs(argc - 1, argv + 1);

  std::string config_file_path = "";
  int real_core_id;
  for (auto it = arg_map->begin(); it != arg_map->end(); ++it) {
    printf("Key: %s, Val: %s\n", it->first.c_str(), it->second.c_str());
    if (it->first == "config-file") {
      config_file_path = it->second;
    }
    if (it->first == "real-core") {
       real_core_id = std::stoi(it->second);
    }
  }
	
  if (config_file_path == "")
    rte_exit(EXIT_FAILURE, "No configuration file provided\n");
  PacketProcessorConfig packet_processor_config;
  int fd = open(config_file_path.c_str(), O_RDONLY);
  if (fd < 0)
    rte_exit(EXIT_FAILURE, "Cannot open configuration file %s\n",
             config_file_path.c_str());

  // Read the configuration file into a protobuf object.
  google::protobuf::io::FileInputStream config_file_handle(fd);
  config_file_handle.SetCloseOnDelete(true);
  google::protobuf::TextFormat::Parse(&config_file_handle,
                                      &packet_processor_config);

  // For debugging purpose only.
  std::string str = "";
  google::protobuf::TextFormat::PrintToString(packet_processor_config, &str);
  printf("%s\n", str.c_str());

  // Change scheduler to RT Round Robin
  int rc, old_sched_policy;
  struct sched_param my_params;
  my_params.sched_priority = MY_RT_PRIORITY;
  old_sched_policy = sched_getscheduler(0);
  rc = sched_setscheduler(0, SCHED_RR, &my_params); 
  if (rc == -1) {
     printf("sched_setscheduler call is failed\n");
  } 
  else {
     printf("Old Scheduler: %d\n", old_sched_policy);
     printf("Current Scheduler: %d\n", sched_getscheduler( 0 ));
  }

  // First, pin the current thread to the CPU specified in CPU mask.
  // int ms_lcore_id = rte_lcore_id();
  int ms_lcore_id = real_core_id;
  pthread_t current_thread = pthread_self();
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(ms_lcore_id, &cpuset);
  pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
	
  // Initialize and run the packet processor.
  PacketProcessorFactory *pp_factory = PacketProcessorFactory::GetInstance();
  auto packet_processor = pp_factory->CreatePacketProcessor(
      packet_processor_config.packet_processor_class());
  assert(packet_processor.get() != nullptr);
  packet_processor->Init(packet_processor_config);
  packet_processor->Run();
  return 0;
}
