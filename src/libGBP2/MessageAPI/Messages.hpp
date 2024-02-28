#include "libGBP2/Conventions.hpp"
#define UNITCONVERT_NO_BACKWARD_COMPATIBLE_NAMESPACE
#include <exception>
#include <string>

#include <UnitConvert.hpp>
#include <UnitConvert/GlobalUnitRegistry.hpp>
#include <google/protobuf/util/json_util.h>

#include "../CircularGaussianLaserBeam.hpp"
#include "../Units.hpp"
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

/**
 * Create a boost::unit::quantity from a msg::Quantity (protobuf message)
 */
template<typename U>
quantity<U> make_quantity(const msg::Quantity& a_Quantity)
{
  static auto& ureg = UnitConvert::getGlobalUnitRegistry();
  return ureg.makeQuantity<double>(a_Quantity.value(), a_Quantity.unit()).to<U>();
}

/**
 * Create a GaussianBeamWidth<C,T> object from a length and width type.
 * This function is for turning runtime data that came from a deserialized message
 * to the compile-time type used for calculations.
 */
template<typename C, typename U>
GaussianBeamWidth<C, U> make_beam_width(const msg::Quantity& a_width, const msg::BeamWidthType a_type)
{
  GaussianBeamWidth<C, U> width;
  switch(a_type) {
    case msg::BEAM_WIDTH_TYPE_ONE_OVER_E_SQUARED_RADIUS:
      width = make_width<OneOverESquaredRadius>(msg::make_quantity<t::cm>(a_width));
      break;
    case msg::BEAM_WIDTH_TYPE_ONE_OVER_E_SQUARED_DIAMETER:
      width = make_width<OneOverESquaredDiameter>(msg::make_quantity<t::cm>(a_width));
      break;
    case msg::BEAM_WIDTH_TYPE_ONE_OVER_E_RADIUS:
      width = make_width<OneOverERadius>(msg::make_quantity<t::cm>(a_width));
      break;
    case msg::BEAM_WIDTH_TYPE_ONE_OVER_E_DIAMETER:
      width = make_width<OneOverEDiameter>(msg::make_quantity<t::cm>(a_width));
      break;
    case msg::BEAM_WIDTH_TYPE_FWHM_RADIUS:
      width = make_width<FWHMRadius>(msg::make_quantity<t::cm>(a_width));
      break;
    case msg::BEAM_WIDTH_TYPE_FWHM_DIAMETER:
      width = make_width<FWHMDiameter>(msg::make_quantity<t::cm>(a_width));
      break;
    case msg::BEAM_WIDTH_TYPE_UNSPECIFIED:
      throw std::runtime_error("Beam width type was not set");
      break;
    default:
      throw std::runtime_error("Unknown beam width type. Did you add an additional convention to the message without adding it to the library?");
      break;
  }
  return width;
}

/**
 * Create a GaussianBeamDivergence<C,T> object from an angle and divergence type.
 * This function is for turning runtime data that came from a deserialized message
 * to the compile-time type used for calculations.
 */
template<typename C, typename U>
GaussianBeamDivergence<C, U> make_beam_divergence(const msg::Quantity& a_divergence, const msg::BeamDivergenceType a_type)
{
  GaussianBeamDivergence<C, U> divergence;
  switch(a_type) {
    case msg::BEAM_DIVERGENCE_TYPE_ONE_OVER_E_SQUARED_HALF_ANGLE:
      divergence = make_divergence<OneOverESquaredHalfAngle>(msg::make_quantity<t::mrad>(a_divergence));
      break;
    case msg::BEAM_DIVERGENCE_TYPE_ONE_OVER_E_SQUARED_FULL_ANGLE:
      divergence = make_divergence<OneOverESquaredFullAngle>(msg::make_quantity<t::mrad>(a_divergence));
      break;
    case msg::BEAM_DIVERGENCE_TYPE_ONE_OVER_E_HALF_ANGLE:
      divergence = make_divergence<OneOverEHalfAngle>(msg::make_quantity<t::mrad>(a_divergence));
      break;
    case msg::BEAM_DIVERGENCE_TYPE_ONE_OVER_E_FULL_ANGLE:
      divergence = make_divergence<OneOverEFullAngle>(msg::make_quantity<t::mrad>(a_divergence));
      break;
    case msg::BEAM_DIVERGENCE_TYPE_FWHM_HALF_ANGLE:
      divergence = make_divergence<FWHMHalfAngle>(msg::make_quantity<t::mrad>(a_divergence));
      break;
    case msg::BEAM_DIVERGENCE_TYPE_FWHM_FULL_ANGLE:
      divergence = make_divergence<FWHMFullAngle>(msg::make_quantity<t::mrad>(a_divergence));
      break;
    case msg::BEAM_DIVERGENCE_TYPE_UNSPECIFIED:
      throw std::runtime_error("Beam divergence type was not set");
      break;
    default:
      throw std::runtime_error("Unknown beam divergence type. Did you add an additional convention to the message without adding it to the library?");
      break;
  }
  return divergence;
}

}  // namespace msg
}  // namespace libGBP2
libgbp2_message_api::Quantity& operator<<(libgbp2_message_api::Quantity& a_quantity, std::string a_qstring);
