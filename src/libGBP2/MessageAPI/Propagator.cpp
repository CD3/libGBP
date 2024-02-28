#include "libGBP2/Conventions.hpp"
#define UNITCONVERT_NO_BACKWARD_COMPATIBLE_NAMESPACE
#include <iostream>
#include <string>
#include <vector>

#include <UnitConvert.hpp>
#include <UnitConvert/GlobalUnitRegistry.hpp>

#include "../CircularGaussianLaserBeam.hpp"
#include "../OpticalSystem.hpp"
#include "./Messages.hpp"
#include "Messages.pb.h"
#include "libGBP2/MessageAPI/Propagator.hpp"

namespace libGBP2
{
struct Propagator::imp {
  CircularGaussianLaserBeam build_laser(const msg::CircularGaussianBeam& a_laser_msg) const
  {
    // validate
    if(a_laser_msg.has_beam_quality_factor() && a_laser_msg.has_beam_divergence()) {
      throw std::runtime_error("Both beam quality factor and beam divergence were given. Only one or the other can be set.");
    }
    if(!a_laser_msg.has_beam_quality_factor() && !a_laser_msg.has_beam_divergence()) {
      throw std::runtime_error("Neither beam quality factor or beam divergence were given. At least one must be set.");
    }

    auto&                     ureg = UnitConvert::getGlobalUnitRegistry();
    CircularGaussianLaserBeam beam;
    beam.setWavelength(msg::make_quantity<t::cm>(a_laser_msg.wavelength()));
    beam.setBeamWaistPosition(msg::make_quantity<t::cm>(a_laser_msg.beam_waist_position()));
    beam.setBeamWaistWidth(msg::make_beam_width<OneOverESquaredRadius, t::cm>(a_laser_msg.beam_waist_width(), a_laser_msg.beam_waist_width_type()));
    if(a_laser_msg.has_beam_quality_factor()) {
      beam.setBeamQualityFactor(msg::make_quantity<t::dimensionless>(a_laser_msg.beam_quality_factor()));
    }
    if(a_laser_msg.has_beam_divergence()) {
      beam.adjustBeamDivergence(msg::make_beam_divergence<OneOverESquaredHalfAngle, t::mrad>(a_laser_msg.beam_divergence(), a_laser_msg.beam_divergence_type()));
    }

    return beam;
  }
  void run(const msg::Propagator_run_Input& a_input, msg::Propagator_run_Output* a_output) const
  {
    auto beam = this->build_laser(a_input.beam());
    // build the system
    //
    msg::BeamWidthType output_beam_width_type = a_input.beam().beam_waist_width_type();
    if(a_input.has_output_beam_width_type()) {
      output_beam_width_type = a_input.output_beam_width_type();
    }
    for(int i = 0; i < a_input.position_size(); i++) {
      // propagate through system
      // get a beam width object for the given position
      auto beam_width = beam.getBeamWidth(msg::make_quantity<t::cm>(a_input.position(i)));
      // extract the width to msg::Quantity using the specified beam width convention
      auto width = msg::get_beam_width(beam_width, output_beam_width_type);
      // convert the width to the specified units
      if(a_input.has_output_beam_width_unit()) {
        msg::convert(width, a_input.output_beam_width_unit());
      }
      // copy the width to the output message
      *a_output->add_beam_width() << width;
    }
  }
};

/**
 * A macro to write the boiler plate for forwarding a function call to the private implementation.
 * Message deserialization/serialization happens here, the implementation only needs to
 * take the input message reference and output message pointer as arguments.
 *
 * Notes:
 *
 * This method **requires** the output message to contain a return_status message.
 * This is how error/sucess is communicated to the caller.
 *
 * If implementation did not set anything in the return_status
 * we want to create an empty one to return here. That way return_status will
 * exist for the caller and they can check to see if error_message
 * set to indicate an error. The non-existence of error_message indicates sucess... because we are optimists.
 */
#define DEFINE_FORWARDING_METHOD(NAME)                                                                    \
  auto Propagator::NAME(const std::string& a_input_str)->std::string                                      \
  {                                                                                                       \
    std::string                                     output_str;                                           \
    libgbp2_message_api::Propagator_##NAME##_Input  input_msg;                                            \
    libgbp2_message_api::Propagator_##NAME##_Output output_msg;                                           \
    try {                                                                                                 \
      msg::deserialize_message(a_input_str, input_msg);                                                   \
      pImpl->NAME(input_msg, &output_msg);                                                                \
    } catch(std::runtime_error & err) {                                                                   \
      std::string error_message = "There was an error in `" #NAME "` method: " + std::string(err.what()); \
      output_msg.mutable_return_status()->set_error_message(error_message);                               \
    } catch(...) {                                                                                        \
      std::string error_message = "There was an error in `" #NAME "` method.";                            \
      output_msg.mutable_return_status()->set_error_message(error_message);                               \
    }                                                                                                     \
    if(!output_msg.has_return_status()) {                                                                 \
      output_msg.set_allocated_return_status(new libgbp2_message_api::ReturnStatus());                    \
    }                                                                                                     \
    output_str = msg::serialize_message(output_msg, msg::serialization_format::JSON);                     \
    return output_str;                                                                                    \
  }

DEFINE_FORWARDING_METHOD(run)

}  // namespace libGBP2
