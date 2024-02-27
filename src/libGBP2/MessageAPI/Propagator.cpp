#include "libGBP2/MessageAPI/Propagator.hpp"

#include <iostream>

#include "../CircularGaussianLaserBeam.hpp"
#include "../OpticalSystem.hpp"
#include "./Messages.hpp"
#include "Messages.pb.h"

namespace libGBP2
{
struct Propagator::imp {
  CircularGaussianLaserBeam build_laser(const msg::CircularGaussianBeam& a_laser_msg) const
  {
    CircularGaussianLaserBeam beam;

    return beam;
  }
  void run(const msg::Propagator_run_Input& a_input, msg::Propagator_run_Output* a_output) const
  {
    auto beam = this->build_laser(a_input.beam());
  }
};

/**
 * A macro to write the boiler plate for forwarding a function call to the private implementation.
 * Message deserialization/serialization happens here, the implementation only needs to
 * take the input and output message pointer as arguments.
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
#define DEFINE_FORWARDING_METHOD(NAME)                                                                  \
  auto Propagator::NAME(const std::string& a_input_str)->std::string                                    \
  {                                                                                                     \
    std::string                                     output_str;                                         \
    libgbp2_message_api::Propagator_##NAME##_Input  input_msg;                                          \
    libgbp2_message_api::Propagator_##NAME##_Output output_msg;                                         \
    try {                                                                                               \
      msg::deserialize_message(a_input_str, input_msg);                                                 \
      pImpl->NAME(input_msg, &output_msg);                                                              \
    } catch(std::runtime_error & err) {                                                                 \
      std::string error_message = "There was an error in " #NAME " method: " + std::string(err.what()); \
      output_msg.mutable_return_status()->set_error_message(error_message);                             \
    } catch(...) {                                                                                      \
      std::string error_message = "There was an error in " #NAME " method.";                            \
      output_msg.mutable_return_status()->set_error_message(error_message);                             \
    }                                                                                                   \
    if(!output_msg.has_return_status()) {                                                               \
      output_msg.set_allocated_return_status(new libgbp2_message_api::ReturnStatus());                  \
    }                                                                                                   \
    output_str = msg::serialize_message(output_msg, msg::serialization_format::JSON);                   \
    return output_str;                                                                                  \
  }

DEFINE_FORWARDING_METHOD(run)

}  // namespace libGBP2
