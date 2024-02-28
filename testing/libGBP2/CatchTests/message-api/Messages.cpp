
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <libGBP2/MessageAPI/Messages.hpp>
#include <libGBP2/MessageAPI/Propagator.hpp>

#include "Messages.pb.h"

using namespace Catch;

TEST_CASE("Quantity message << operator")
{
  using namespace libGBP2;
  SECTION("With object")
  {
    msg::Quantity msg;

    msg << "10 mm";

    CHECK(msg.value() == Approx(10));
    CHECK(msg.unit() == "mm");
  }
  SECTION("With pointer")
  {
    msg::Quantity* msg = new msg::Quantity();

    *msg << "10 mm";

    CHECK(msg->value() == Approx(10));
    CHECK(msg->unit() == "mm");

    delete msg;
  }
  SECTION("As mutable field")
  {
    msg::CircularGaussianBeam msg;

    *msg.mutable_beam_waist_width() << "10 mm";

    CHECK(msg.beam_waist_width().value() == Approx(10));
    CHECK(msg.beam_waist_width().unit() == "mm");
  }
}

TEST_CASE("Quantity utility methods")
{
  using namespace libGBP2;
  msg::Quantity val;

  SECTION("widths")
  {
    val << "23 mm";

    SECTION("make_quantity")
    {
      auto w = msg::make_quantity<t::cm>(val);
      CHECK(w.value() == Approx(2.3));
    }

    SECTION("make_beam_width - 1/e2 radius")
    {
      msg::BeamWidthType type = msg::BEAM_WIDTH_TYPE_ONE_OVER_E_SQUARED_RADIUS;
      auto               w    = msg::make_beam_width<OneOverESquaredRadius, t::cm>(val, type);
      CHECK(w.get<OneOverESquaredRadius>().value() == Approx(2.3));
    }

    SECTION("make_beam_width - 1/e2 diameter")
    {
      msg::BeamWidthType type = msg::BEAM_WIDTH_TYPE_ONE_OVER_E_SQUARED_DIAMETER;
      auto               w    = msg::make_beam_width<OneOverESquaredRadius, t::cm>(val, type);
      CHECK(w.get<OneOverESquaredRadius>().value() == Approx(2.3 / 2));
    }

    SECTION("make_beam_width - 1/e radius")
    {
      msg::BeamWidthType type = msg::BEAM_WIDTH_TYPE_ONE_OVER_E_RADIUS;
      auto               w    = msg::make_beam_width<OneOverESquaredRadius, t::cm>(val, type);
      CHECK(w.get<OneOverESquaredRadius>().value() == Approx(2.3 * sqrt(2)));
    }

    SECTION("make_beam_width - 1/e diameter")
    {
      msg::BeamWidthType type = msg::BEAM_WIDTH_TYPE_ONE_OVER_E_DIAMETER;
      auto               w    = msg::make_beam_width<OneOverESquaredRadius, t::cm>(val, type);
      CHECK(w.get<OneOverESquaredRadius>().value() == Approx(2.3 * sqrt(2) / 2));
    }

    SECTION("make_beam_width - fwhm radius")
    {
      msg::BeamWidthType type = msg::BEAM_WIDTH_TYPE_FWHM_RADIUS;
      auto               w    = msg::make_beam_width<OneOverESquaredRadius, t::cm>(val, type);
      CHECK(w.get<OneOverESquaredRadius>().value() == Approx(2.3 * 0.8493218 * 2));
    }

    SECTION("make_beam_width - fwhm diameter")
    {
      msg::BeamWidthType type = msg::BEAM_WIDTH_TYPE_FWHM_DIAMETER;
      auto               w    = msg::make_beam_width<OneOverESquaredRadius, t::cm>(val, type);
      CHECK(w.get<OneOverESquaredRadius>().value() == Approx(2.3 * 0.8493218));
    }

    SECTION("make_beam_width - unspecified")
    {
      msg::BeamWidthType type = msg::BEAM_WIDTH_TYPE_UNSPECIFIED;
      CHECK_THROWS(msg::make_beam_width<OneOverESquaredRadius, t::cm>(val, type));
    }
  }

  SECTION("divergences")
  {
    val << "0.0023 rad";

    SECTION("make_quantity")
    {
      auto w = msg::make_quantity<t::rad>(val);
      CHECK(w.value() == Approx(0.0023));
    }

    SECTION("make_beam_divergence - 1/e2 half divergence")
    {
      msg::BeamDivergenceType type = msg::BEAM_DIVERGENCE_TYPE_ONE_OVER_E_SQUARED_HALF_ANGLE;
      auto                    w    = msg::make_beam_divergence<OneOverESquaredHalfAngle, t::mrad>(val, type);
      CHECK(w.get<OneOverESquaredHalfAngle>().value() == Approx(2.3));
    }

    SECTION("make_beam_divergence - 1/e2 full divergence")
    {
      msg::BeamDivergenceType type = msg::BEAM_DIVERGENCE_TYPE_ONE_OVER_E_SQUARED_FULL_ANGLE;
      auto                    w    = msg::make_beam_divergence<OneOverESquaredHalfAngle, t::mrad>(val, type);
      CHECK(w.get<OneOverESquaredHalfAngle>().value() == Approx(2.3 / 2));
    }

    SECTION("make_beam_divergence - 1/e half divergence")
    {
      msg::BeamDivergenceType type = msg::BEAM_DIVERGENCE_TYPE_ONE_OVER_E_HALF_ANGLE;
      auto                    w    = msg::make_beam_divergence<OneOverESquaredHalfAngle, t::mrad>(val, type);
      CHECK(w.get<OneOverESquaredHalfAngle>().value() == Approx(2.3 * sqrt(2)));
    }

    SECTION("make_beam_divergence - 1/e full divergence")
    {
      msg::BeamDivergenceType type = msg::BEAM_DIVERGENCE_TYPE_ONE_OVER_E_FULL_ANGLE;
      auto                    w    = msg::make_beam_divergence<OneOverESquaredHalfAngle, t::mrad>(val, type);
      CHECK(w.get<OneOverESquaredHalfAngle>().value() == Approx(2.3 * sqrt(2) / 2));
    }

    SECTION("make_beam_divergence - fwhm half divergence")
    {
      msg::BeamDivergenceType type = msg::BEAM_DIVERGENCE_TYPE_FWHM_HALF_ANGLE;
      auto                    w    = msg::make_beam_divergence<OneOverESquaredHalfAngle, t::mrad>(val, type);
      CHECK(w.get<OneOverESquaredHalfAngle>().value() == Approx(2.3 * 0.8493218 * 2));
    }

    SECTION("make_beam_divergence - fwhm full divergence")
    {
      msg::BeamDivergenceType type = msg::BEAM_DIVERGENCE_TYPE_FWHM_FULL_ANGLE;
      auto                    w    = msg::make_beam_divergence<OneOverESquaredHalfAngle, t::mrad>(val, type);
      CHECK(w.get<OneOverESquaredHalfAngle>().value() == Approx(2.3 * 0.8493218));
    }

    SECTION("make_beam_divergence - unspecified")
    {
      msg::BeamDivergenceType type = msg::BEAM_DIVERGENCE_TYPE_UNSPECIFIED;
      CHECK_THROWS(msg::make_beam_divergence<OneOverESquaredHalfAngle, t::mrad>(val, type));
    }
  }
}
