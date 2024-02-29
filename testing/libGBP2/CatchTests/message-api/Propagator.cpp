#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <libGBP2/MessageAPI/Messages.hpp>
#include <libGBP2/MessageAPI/Propagator.hpp>

#include "Messages.pb.h"

using namespace Catch;

TEST_CASE("Devel")
{
  using namespace libGBP2;
  Propagator propagator;

  msg::Propagator_run_Input  input_msg;
  msg::Propagator_run_Output output_msg;
  msg::Quantity              quant;

  *input_msg.mutable_beam()->mutable_wavelength() << "532 nm";
  *input_msg.mutable_beam()->mutable_beam_waist_width() << "10 um";
  input_msg.mutable_beam()->set_beam_waist_width_type(msg::BEAM_WIDTH_TYPE_ONE_OVER_E_SQUARED_RADIUS);
  *input_msg.mutable_beam()->mutable_beam_waist_position() << "0 cm";

  CHECK(input_msg.positions_size() == 0);

  *input_msg.add_positions() << "0 cm";
  CHECK(input_msg.positions_size() == 1);

  *input_msg.add_positions() << "10 cm";
  CHECK(input_msg.positions_size() == 2);

  CHECK(input_msg.positions(0).value() == Approx(0).scale(1));
  CHECK(input_msg.positions(0).unit() == "cm");
  CHECK(input_msg.positions(1).value() == Approx(10));
  CHECK(input_msg.positions(1).unit() == "cm");

  msg::deserialize_message(propagator.run(msg::serialize_message(input_msg)), output_msg);

  CHECK(output_msg.has_return_status());
  CHECK(output_msg.return_status().has_error_message());
  CHECK(output_msg.return_status().error_message() == "There was an error in `run` method: Neither beam quality factor or beam divergence were given. At least one must be set.");

  *input_msg.mutable_beam()->mutable_beam_quality_factor() << "1";

  msg::deserialize_message(propagator.run(msg::serialize_message(input_msg)), output_msg);

  CHECK(output_msg.has_return_status());
  CHECK(!output_msg.return_status().has_error_message());
}

TEST_CASE("running propagation analysis")
{
  using namespace libGBP2;
  Propagator propagator;

  msg::Propagator_run_Input  input_msg;
  msg::Propagator_run_Output output_msg;
  msg::Quantity              quant;

  *input_msg.mutable_beam()->mutable_wavelength() << "532 nm";
  *input_msg.mutable_beam()->mutable_beam_waist_width() << "10 um";
  input_msg.mutable_beam()->set_beam_waist_width_type(msg::BEAM_WIDTH_TYPE_ONE_OVER_E_SQUARED_RADIUS);
  *input_msg.mutable_beam()->mutable_beam_waist_position() << "0 cm";
  *input_msg.mutable_beam()->mutable_beam_quality_factor() << "1";

  *input_msg.add_positions() << "0 cm";
  *input_msg.add_positions() << "1 cm";
  *input_msg.add_positions() << "2 cm";
  *input_msg.add_positions() << "3 cm";
  *input_msg.add_positions() << "4 cm";
  CHECK(input_msg.positions_size() == 5);

  SECTION("No optical system")
  {
    SECTION("Use input width type and units")
    {
      msg::deserialize_message(propagator.run(msg::serialize_message(input_msg)), output_msg);

      CHECK(!output_msg.return_status().has_error_message());

      REQUIRE(output_msg.beam_widths_size() == 5);
      CHECK(output_msg.beam_widths(0).value() == Approx(10));
      CHECK(output_msg.beam_widths(0).unit() == "um");
      CHECK(output_msg.beam_widths(1).value() == Approx(169.64).epsilon(0.001));
      CHECK(output_msg.beam_widths(1).unit() == "um");
      CHECK(output_msg.beam_widths(2).value() == Approx(338.93).epsilon(0.001));
      CHECK(output_msg.beam_widths(2).unit() == "um");
      CHECK(output_msg.beam_widths(3).value() == Approx(508.12).epsilon(0.001));
      CHECK(output_msg.beam_widths(3).unit() == "um");
      CHECK(output_msg.beam_widths(4).value() == Approx(677.44).epsilon(0.001));
      CHECK(output_msg.beam_widths(4).unit() == "um");
    }

    SECTION("Specify output unit")
    {
      input_msg.set_output_beam_width_unit("mm");
      msg::deserialize_message(propagator.run(msg::serialize_message(input_msg)), output_msg);

      CHECK(!output_msg.return_status().has_error_message());

      REQUIRE(output_msg.beam_widths_size() == 5);
      CHECK(output_msg.beam_widths(0).value() == Approx(10e-3));
      CHECK(output_msg.beam_widths(0).unit() == "mm");
      CHECK(output_msg.beam_widths(1).value() == Approx(0.16964).epsilon(0.001));
      CHECK(output_msg.beam_widths(1).unit() == "mm");
      CHECK(output_msg.beam_widths(2).value() == Approx(0.33893).epsilon(0.001));
      CHECK(output_msg.beam_widths(2).unit() == "mm");
      CHECK(output_msg.beam_widths(3).value() == Approx(0.50812).epsilon(0.001));
      CHECK(output_msg.beam_widths(3).unit() == "mm");
      CHECK(output_msg.beam_widths(4).value() == Approx(0.67744).epsilon(0.001));
      CHECK(output_msg.beam_widths(4).unit() == "mm");
    }
  }
  SECTION("With optical system")
  {
    SECTION("Single Lens")
    {
      /* input_msg.mutable_system(); */
    }
  }
}
