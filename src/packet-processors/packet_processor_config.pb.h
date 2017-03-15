// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: packet_processor_config.proto

#ifndef PROTOBUF_packet_5fprocessor_5fconfig_2eproto__INCLUDED
#define PROTOBUF_packet_5fprocessor_5fconfig_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3000000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3000000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/map.h>
#include <google/protobuf/map_field_inl.h>
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_packet_5fprocessor_5fconfig_2eproto();
void protobuf_AssignDesc_packet_5fprocessor_5fconfig_2eproto();
void protobuf_ShutdownFile_packet_5fprocessor_5fconfig_2eproto();

class PacketProcessorConfig;
class PortConfig;

enum PortConfig_PortType {
  PortConfig_PortType_INGRESS_PORT = 0,
  PortConfig_PortType_EGRESS_PORT = 1
};
bool PortConfig_PortType_IsValid(int value);
const PortConfig_PortType PortConfig_PortType_PortType_MIN = PortConfig_PortType_INGRESS_PORT;
const PortConfig_PortType PortConfig_PortType_PortType_MAX = PortConfig_PortType_EGRESS_PORT;
const int PortConfig_PortType_PortType_ARRAYSIZE = PortConfig_PortType_PortType_MAX + 1;

const ::google::protobuf::EnumDescriptor* PortConfig_PortType_descriptor();
inline const ::std::string& PortConfig_PortType_Name(PortConfig_PortType value) {
  return ::google::protobuf::internal::NameOfEnum(
    PortConfig_PortType_descriptor(), value);
}
inline bool PortConfig_PortType_Parse(
    const ::std::string& name, PortConfig_PortType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<PortConfig_PortType>(
    PortConfig_PortType_descriptor(), name, value);
}
// ===================================================================

class PacketProcessorConfig : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:PacketProcessorConfig) */ {
 public:
  PacketProcessorConfig();
  virtual ~PacketProcessorConfig();

  PacketProcessorConfig(const PacketProcessorConfig& from);

  inline PacketProcessorConfig& operator=(const PacketProcessorConfig& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const PacketProcessorConfig& default_instance();

  void Swap(PacketProcessorConfig* other);

  // implements Message ----------------------------------------------

  inline PacketProcessorConfig* New() const { return New(NULL); }

  PacketProcessorConfig* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const PacketProcessorConfig& from);
  void MergeFrom(const PacketProcessorConfig& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(PacketProcessorConfig* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------


  // accessors -------------------------------------------------------

  // required string packet_processor_class = 1;
  bool has_packet_processor_class() const;
  void clear_packet_processor_class();
  static const int kPacketProcessorClassFieldNumber = 1;
  const ::std::string& packet_processor_class() const;
  void set_packet_processor_class(const ::std::string& value);
  void set_packet_processor_class(const char* value);
  void set_packet_processor_class(const char* value, size_t size);
  ::std::string* mutable_packet_processor_class();
  ::std::string* release_packet_processor_class();
  void set_allocated_packet_processor_class(::std::string* packet_processor_class);

  // required int32 num_ingress_ports = 2;
  bool has_num_ingress_ports() const;
  void clear_num_ingress_ports();
  static const int kNumIngressPortsFieldNumber = 2;
  ::google::protobuf::int32 num_ingress_ports() const;
  void set_num_ingress_ports(::google::protobuf::int32 value);

  // required int32 num_egress_ports = 3;
  bool has_num_egress_ports() const;
  void clear_num_egress_ports();
  static const int kNumEgressPortsFieldNumber = 3;
  ::google::protobuf::int32 num_egress_ports() const;
  void set_num_egress_ports(::google::protobuf::int32 value);

  // map<string, string> pp_parameters = 4;
  int pp_parameters_size() const;
  void clear_pp_parameters();
  static const int kPpParametersFieldNumber = 4;
  const ::google::protobuf::Map< ::std::string, ::std::string >&
      pp_parameters() const;
  ::google::protobuf::Map< ::std::string, ::std::string >*
      mutable_pp_parameters();

  // repeated .PortConfig port_configs = 5;
  int port_configs_size() const;
  void clear_port_configs();
  static const int kPortConfigsFieldNumber = 5;
  const ::PortConfig& port_configs(int index) const;
  ::PortConfig* mutable_port_configs(int index);
  ::PortConfig* add_port_configs();
  ::google::protobuf::RepeatedPtrField< ::PortConfig >*
      mutable_port_configs();
  const ::google::protobuf::RepeatedPtrField< ::PortConfig >&
      port_configs() const;

  // @@protoc_insertion_point(class_scope:PacketProcessorConfig)
 private:
  inline void set_has_packet_processor_class();
  inline void clear_has_packet_processor_class();
  inline void set_has_num_ingress_ports();
  inline void clear_has_num_ingress_ports();
  inline void set_has_num_egress_ports();
  inline void clear_has_num_egress_ports();

  // helper for ByteSize()
  int RequiredFieldsByteSizeFallback() const;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::internal::ArenaStringPtr packet_processor_class_;
  ::google::protobuf::int32 num_ingress_ports_;
  ::google::protobuf::int32 num_egress_ports_;
  typedef ::google::protobuf::internal::MapEntryLite<
      ::std::string, ::std::string,
      ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
      ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
      0 >
      PacketProcessorConfig_PpParametersEntry;
  ::google::protobuf::internal::MapField<
      ::std::string, ::std::string,
      ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
      ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
      0 > pp_parameters_;
  ::google::protobuf::RepeatedPtrField< ::PortConfig > port_configs_;
  friend void  protobuf_AddDesc_packet_5fprocessor_5fconfig_2eproto();
  friend void protobuf_AssignDesc_packet_5fprocessor_5fconfig_2eproto();
  friend void protobuf_ShutdownFile_packet_5fprocessor_5fconfig_2eproto();

  void InitAsDefaultInstance();
  static PacketProcessorConfig* default_instance_;
};
// -------------------------------------------------------------------

class PortConfig : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:PortConfig) */ {
 public:
  PortConfig();
  virtual ~PortConfig();

  PortConfig(const PortConfig& from);

  inline PortConfig& operator=(const PortConfig& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const PortConfig& default_instance();

  void Swap(PortConfig* other);

  // implements Message ----------------------------------------------

  inline PortConfig* New() const { return New(NULL); }

  PortConfig* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const PortConfig& from);
  void MergeFrom(const PortConfig& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(PortConfig* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------


  typedef PortConfig_PortType PortType;
  static const PortType INGRESS_PORT =
    PortConfig_PortType_INGRESS_PORT;
  static const PortType EGRESS_PORT =
    PortConfig_PortType_EGRESS_PORT;
  static inline bool PortType_IsValid(int value) {
    return PortConfig_PortType_IsValid(value);
  }
  static const PortType PortType_MIN =
    PortConfig_PortType_PortType_MIN;
  static const PortType PortType_MAX =
    PortConfig_PortType_PortType_MAX;
  static const int PortType_ARRAYSIZE =
    PortConfig_PortType_PortType_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  PortType_descriptor() {
    return PortConfig_PortType_descriptor();
  }
  static inline const ::std::string& PortType_Name(PortType value) {
    return PortConfig_PortType_Name(value);
  }
  static inline bool PortType_Parse(const ::std::string& name,
      PortType* value) {
    return PortConfig_PortType_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // required int32 port_index = 1;
  bool has_port_index() const;
  void clear_port_index();
  static const int kPortIndexFieldNumber = 1;
  ::google::protobuf::int32 port_index() const;
  void set_port_index(::google::protobuf::int32 value);

  // required .PortConfig.PortType port_type = 2;
  bool has_port_type() const;
  void clear_port_type();
  static const int kPortTypeFieldNumber = 2;
  ::PortConfig_PortType port_type() const;
  void set_port_type(::PortConfig_PortType value);

  // required string port_class = 3;
  bool has_port_class() const;
  void clear_port_class();
  static const int kPortClassFieldNumber = 3;
  const ::std::string& port_class() const;
  void set_port_class(const ::std::string& value);
  void set_port_class(const char* value);
  void set_port_class(const char* value, size_t size);
  ::std::string* mutable_port_class();
  ::std::string* release_port_class();
  void set_allocated_port_class(::std::string* port_class);

  // map<string, string> port_parameters = 4;
  int port_parameters_size() const;
  void clear_port_parameters();
  static const int kPortParametersFieldNumber = 4;
  const ::google::protobuf::Map< ::std::string, ::std::string >&
      port_parameters() const;
  ::google::protobuf::Map< ::std::string, ::std::string >*
      mutable_port_parameters();

  // @@protoc_insertion_point(class_scope:PortConfig)
 private:
  inline void set_has_port_index();
  inline void clear_has_port_index();
  inline void set_has_port_type();
  inline void clear_has_port_type();
  inline void set_has_port_class();
  inline void clear_has_port_class();

  // helper for ByteSize()
  int RequiredFieldsByteSizeFallback() const;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::int32 port_index_;
  int port_type_;
  ::google::protobuf::internal::ArenaStringPtr port_class_;
  typedef ::google::protobuf::internal::MapEntryLite<
      ::std::string, ::std::string,
      ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
      ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
      0 >
      PortConfig_PortParametersEntry;
  ::google::protobuf::internal::MapField<
      ::std::string, ::std::string,
      ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
      ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
      0 > port_parameters_;
  friend void  protobuf_AddDesc_packet_5fprocessor_5fconfig_2eproto();
  friend void protobuf_AssignDesc_packet_5fprocessor_5fconfig_2eproto();
  friend void protobuf_ShutdownFile_packet_5fprocessor_5fconfig_2eproto();

  void InitAsDefaultInstance();
  static PortConfig* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// PacketProcessorConfig

// required string packet_processor_class = 1;
inline bool PacketProcessorConfig::has_packet_processor_class() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void PacketProcessorConfig::set_has_packet_processor_class() {
  _has_bits_[0] |= 0x00000001u;
}
inline void PacketProcessorConfig::clear_has_packet_processor_class() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void PacketProcessorConfig::clear_packet_processor_class() {
  packet_processor_class_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_packet_processor_class();
}
inline const ::std::string& PacketProcessorConfig::packet_processor_class() const {
  // @@protoc_insertion_point(field_get:PacketProcessorConfig.packet_processor_class)
  return packet_processor_class_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PacketProcessorConfig::set_packet_processor_class(const ::std::string& value) {
  set_has_packet_processor_class();
  packet_processor_class_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:PacketProcessorConfig.packet_processor_class)
}
inline void PacketProcessorConfig::set_packet_processor_class(const char* value) {
  set_has_packet_processor_class();
  packet_processor_class_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:PacketProcessorConfig.packet_processor_class)
}
inline void PacketProcessorConfig::set_packet_processor_class(const char* value, size_t size) {
  set_has_packet_processor_class();
  packet_processor_class_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:PacketProcessorConfig.packet_processor_class)
}
inline ::std::string* PacketProcessorConfig::mutable_packet_processor_class() {
  set_has_packet_processor_class();
  // @@protoc_insertion_point(field_mutable:PacketProcessorConfig.packet_processor_class)
  return packet_processor_class_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* PacketProcessorConfig::release_packet_processor_class() {
  // @@protoc_insertion_point(field_release:PacketProcessorConfig.packet_processor_class)
  clear_has_packet_processor_class();
  return packet_processor_class_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PacketProcessorConfig::set_allocated_packet_processor_class(::std::string* packet_processor_class) {
  if (packet_processor_class != NULL) {
    set_has_packet_processor_class();
  } else {
    clear_has_packet_processor_class();
  }
  packet_processor_class_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), packet_processor_class);
  // @@protoc_insertion_point(field_set_allocated:PacketProcessorConfig.packet_processor_class)
}

// required int32 num_ingress_ports = 2;
inline bool PacketProcessorConfig::has_num_ingress_ports() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void PacketProcessorConfig::set_has_num_ingress_ports() {
  _has_bits_[0] |= 0x00000002u;
}
inline void PacketProcessorConfig::clear_has_num_ingress_ports() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void PacketProcessorConfig::clear_num_ingress_ports() {
  num_ingress_ports_ = 0;
  clear_has_num_ingress_ports();
}
inline ::google::protobuf::int32 PacketProcessorConfig::num_ingress_ports() const {
  // @@protoc_insertion_point(field_get:PacketProcessorConfig.num_ingress_ports)
  return num_ingress_ports_;
}
inline void PacketProcessorConfig::set_num_ingress_ports(::google::protobuf::int32 value) {
  set_has_num_ingress_ports();
  num_ingress_ports_ = value;
  // @@protoc_insertion_point(field_set:PacketProcessorConfig.num_ingress_ports)
}

// required int32 num_egress_ports = 3;
inline bool PacketProcessorConfig::has_num_egress_ports() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void PacketProcessorConfig::set_has_num_egress_ports() {
  _has_bits_[0] |= 0x00000004u;
}
inline void PacketProcessorConfig::clear_has_num_egress_ports() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void PacketProcessorConfig::clear_num_egress_ports() {
  num_egress_ports_ = 0;
  clear_has_num_egress_ports();
}
inline ::google::protobuf::int32 PacketProcessorConfig::num_egress_ports() const {
  // @@protoc_insertion_point(field_get:PacketProcessorConfig.num_egress_ports)
  return num_egress_ports_;
}
inline void PacketProcessorConfig::set_num_egress_ports(::google::protobuf::int32 value) {
  set_has_num_egress_ports();
  num_egress_ports_ = value;
  // @@protoc_insertion_point(field_set:PacketProcessorConfig.num_egress_ports)
}

// map<string, string> pp_parameters = 4;
inline int PacketProcessorConfig::pp_parameters_size() const {
  return pp_parameters_.size();
}
inline void PacketProcessorConfig::clear_pp_parameters() {
  pp_parameters_.Clear();
}
inline const ::google::protobuf::Map< ::std::string, ::std::string >&
PacketProcessorConfig::pp_parameters() const {
  // @@protoc_insertion_point(field_map:PacketProcessorConfig.pp_parameters)
  return pp_parameters_.GetMap();
}
inline ::google::protobuf::Map< ::std::string, ::std::string >*
PacketProcessorConfig::mutable_pp_parameters() {
  // @@protoc_insertion_point(field_mutable_map:PacketProcessorConfig.pp_parameters)
  return pp_parameters_.MutableMap();
}

// repeated .PortConfig port_configs = 5;
inline int PacketProcessorConfig::port_configs_size() const {
  return port_configs_.size();
}
inline void PacketProcessorConfig::clear_port_configs() {
  port_configs_.Clear();
}
inline const ::PortConfig& PacketProcessorConfig::port_configs(int index) const {
  // @@protoc_insertion_point(field_get:PacketProcessorConfig.port_configs)
  return port_configs_.Get(index);
}
inline ::PortConfig* PacketProcessorConfig::mutable_port_configs(int index) {
  // @@protoc_insertion_point(field_mutable:PacketProcessorConfig.port_configs)
  return port_configs_.Mutable(index);
}
inline ::PortConfig* PacketProcessorConfig::add_port_configs() {
  // @@protoc_insertion_point(field_add:PacketProcessorConfig.port_configs)
  return port_configs_.Add();
}
inline ::google::protobuf::RepeatedPtrField< ::PortConfig >*
PacketProcessorConfig::mutable_port_configs() {
  // @@protoc_insertion_point(field_mutable_list:PacketProcessorConfig.port_configs)
  return &port_configs_;
}
inline const ::google::protobuf::RepeatedPtrField< ::PortConfig >&
PacketProcessorConfig::port_configs() const {
  // @@protoc_insertion_point(field_list:PacketProcessorConfig.port_configs)
  return port_configs_;
}

// -------------------------------------------------------------------

// PortConfig

// required int32 port_index = 1;
inline bool PortConfig::has_port_index() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void PortConfig::set_has_port_index() {
  _has_bits_[0] |= 0x00000001u;
}
inline void PortConfig::clear_has_port_index() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void PortConfig::clear_port_index() {
  port_index_ = 0;
  clear_has_port_index();
}
inline ::google::protobuf::int32 PortConfig::port_index() const {
  // @@protoc_insertion_point(field_get:PortConfig.port_index)
  return port_index_;
}
inline void PortConfig::set_port_index(::google::protobuf::int32 value) {
  set_has_port_index();
  port_index_ = value;
  // @@protoc_insertion_point(field_set:PortConfig.port_index)
}

// required .PortConfig.PortType port_type = 2;
inline bool PortConfig::has_port_type() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void PortConfig::set_has_port_type() {
  _has_bits_[0] |= 0x00000002u;
}
inline void PortConfig::clear_has_port_type() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void PortConfig::clear_port_type() {
  port_type_ = 0;
  clear_has_port_type();
}
inline ::PortConfig_PortType PortConfig::port_type() const {
  // @@protoc_insertion_point(field_get:PortConfig.port_type)
  return static_cast< ::PortConfig_PortType >(port_type_);
}
inline void PortConfig::set_port_type(::PortConfig_PortType value) {
  assert(::PortConfig_PortType_IsValid(value));
  set_has_port_type();
  port_type_ = value;
  // @@protoc_insertion_point(field_set:PortConfig.port_type)
}

// required string port_class = 3;
inline bool PortConfig::has_port_class() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void PortConfig::set_has_port_class() {
  _has_bits_[0] |= 0x00000004u;
}
inline void PortConfig::clear_has_port_class() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void PortConfig::clear_port_class() {
  port_class_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_port_class();
}
inline const ::std::string& PortConfig::port_class() const {
  // @@protoc_insertion_point(field_get:PortConfig.port_class)
  return port_class_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PortConfig::set_port_class(const ::std::string& value) {
  set_has_port_class();
  port_class_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:PortConfig.port_class)
}
inline void PortConfig::set_port_class(const char* value) {
  set_has_port_class();
  port_class_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:PortConfig.port_class)
}
inline void PortConfig::set_port_class(const char* value, size_t size) {
  set_has_port_class();
  port_class_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:PortConfig.port_class)
}
inline ::std::string* PortConfig::mutable_port_class() {
  set_has_port_class();
  // @@protoc_insertion_point(field_mutable:PortConfig.port_class)
  return port_class_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* PortConfig::release_port_class() {
  // @@protoc_insertion_point(field_release:PortConfig.port_class)
  clear_has_port_class();
  return port_class_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PortConfig::set_allocated_port_class(::std::string* port_class) {
  if (port_class != NULL) {
    set_has_port_class();
  } else {
    clear_has_port_class();
  }
  port_class_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), port_class);
  // @@protoc_insertion_point(field_set_allocated:PortConfig.port_class)
}

// map<string, string> port_parameters = 4;
inline int PortConfig::port_parameters_size() const {
  return port_parameters_.size();
}
inline void PortConfig::clear_port_parameters() {
  port_parameters_.Clear();
}
inline const ::google::protobuf::Map< ::std::string, ::std::string >&
PortConfig::port_parameters() const {
  // @@protoc_insertion_point(field_map:PortConfig.port_parameters)
  return port_parameters_.GetMap();
}
inline ::google::protobuf::Map< ::std::string, ::std::string >*
PortConfig::mutable_port_parameters() {
  // @@protoc_insertion_point(field_mutable_map:PortConfig.port_parameters)
  return port_parameters_.MutableMap();
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::PortConfig_PortType> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::PortConfig_PortType>() {
  return ::PortConfig_PortType_descriptor();
}

}  // namespace protobuf
}  // namespace google
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_packet_5fprocessor_5fconfig_2eproto__INCLUDED
