#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <libGBP2/MessageAPI/Messages.hpp>
#include <libGBP2/MessageAPI/Propagator.hpp>

using namespace Catch;

TEST_CASE("Devel")
{
  using namespace libGBP2;
  Propagator propagator;

  msg::Propagator_run_Input  input_msg;
  msg::Propagator_run_Output output_msg;

  *input_msg.mutable_beam()->mutable_wavelength() << "532 nm";
  *input_msg.mutable_beam()->mutable_beam_waist_width() << "10 mm";
  input_msg.mutable_beam()->set_beam_waist_width_type(msg::BEAM_WIDTH_TYPE_ONE_OVER_E_SQUARED_RADIUS);
  *input_msg.mutable_beam()->mutable_beam_waist_position() << "0 cm";

  msg::deserialize_message(propagator.run(msg::serialize_message(input_msg)), output_msg);

  CHECK(output_msg.has_return_status());
  CHECK(!output_msg.return_status().has_error_message());
}
