#include <string>

#include <google/protobuf/util/json_util.h>

#include "Messages.pb.h"
namespace libGBP2
{
namespace msg
{
using namespace libgbp2_message_api;
enum class serialization_format { UNKNOWN,
                                  PROTOBUF,
                                  JSON };
template<typename MESSAGE_TYPE>
std::string serialize_message(const MESSAGE_TYPE& a_protobuf_msg, serialization_format a_fmt = serialization_format::JSON)
{
  std::string serialized_msg;
  if(a_fmt == serialization_format::JSON) {
    google::protobuf::util::JsonPrintOptions print_options;
    print_options.preserve_proto_field_names = true;
    MessageToJsonString(a_protobuf_msg, &serialized_msg, print_options);
  } else {
    serialized_msg = a_protobuf_msg.SerializeAsString();
  }
  return serialized_msg;
}
template<typename MESSAGE_TYPE>
serialization_format deserialize_message(const std::string& a_serialized_msg, MESSAGE_TYPE& a_protobuf_msg)
{
  if(a_serialized_msg.size() > 0 && a_serialized_msg[0] == '{') {
    google::protobuf::util::JsonParseOptions parse_options;
    auto                                     status = JsonStringToMessage(a_serialized_msg, &a_protobuf_msg, parse_options);
    if(!status.ok()) {
      throw std::runtime_error(status.message().as_string());
    }
    return serialization_format::JSON;
  } else {
    auto status = a_protobuf_msg.ParseFromString(a_serialized_msg);
    return serialization_format::PROTOBUF;
  }
  return serialization_format::UNKNOWN;
}

}  // namespace msg
}  // namespace libGBP2
libgbp2_message_api::Quantity& operator<<(libgbp2_message_api::Quantity& a_quantity, std::string a_qstring);
