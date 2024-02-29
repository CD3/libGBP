
#include <sstream>

#include <boost/lexical_cast.hpp>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <libGBP2/MessageAPI/Messages.hpp>
#include <libGBP2/MessageAPI/Propagator.hpp>

#include "Messages.pb.h"
#include "libGBP2/Conventions.hpp"

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
    SECTION("getting static library beam width type from dynamic protobuf message types")
    {
      // these test check that we can extract a GaussianBeamWidth<C,U> object from a
      // msg::BeamWidthType
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
    SECTION("getting dynamic protobuf message object from static library beam width object")
    {
      GaussianBeamWidth<OneOverESquaredRadius, t::cm> width = 25 * i::mm;

      msg::Quantity width_msg = msg::get_beam_width(width, msg::BEAM_WIDTH_TYPE_ONE_OVER_E_SQUARED_RADIUS);

      CHECK(width_msg.value() == Approx(2.5));
      CHECK(width_msg.unit() == "cm");

      width_msg = msg::get_beam_width(width, msg::BEAM_WIDTH_TYPE_ONE_OVER_E_SQUARED_DIAMETER);

      CHECK(width_msg.value() == Approx(5));
      CHECK(width_msg.unit() == "cm");

      width_msg = msg::get_beam_width(width, msg::BEAM_WIDTH_TYPE_ONE_OVER_E_RADIUS);

      CHECK(width_msg.value() == Approx(2.5 / sqrt(2)));
      CHECK(width_msg.unit() == "cm");

      width_msg = msg::get_beam_width(width, msg::BEAM_WIDTH_TYPE_ONE_OVER_E_DIAMETER);

      CHECK(width_msg.value() == Approx(5 / sqrt(2)));
      CHECK(width_msg.unit() == "cm");

      width_msg = msg::get_beam_width(width, msg::BEAM_WIDTH_TYPE_FWHM_RADIUS);

      CHECK(width_msg.value() == Approx(1.25 / 0.8493218));
      CHECK(width_msg.unit() == "cm");

      width_msg = msg::get_beam_width(width, msg::BEAM_WIDTH_TYPE_FWHM_DIAMETER);

      CHECK(width_msg.value() == Approx(2.5 / 0.8493218));
      CHECK(width_msg.unit() == "cm");
    }
  }

  SECTION("divergences")
  {
    SECTION("getting static library beam divergence type from dynamic protobuf message types")
    {
      // these test check that we can extract a GaussianBeamDivergence<C,U> object from a
      // msg::BeamDivergenceType
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
    SECTION("getting dynamic protobuf message object from static library beam divergence object")
    {
      GaussianBeamDivergence<OneOverESquaredHalfAngle, t::crad> divergence = 25 * i::mrad;

      msg::Quantity divergence_msg = msg::get_beam_divergence(divergence, msg::BEAM_DIVERGENCE_TYPE_ONE_OVER_E_SQUARED_HALF_ANGLE);

      CHECK(divergence_msg.value() == Approx(2.5));
      CHECK(divergence_msg.unit() == "crad");

      divergence_msg = msg::get_beam_divergence(divergence, msg::BEAM_DIVERGENCE_TYPE_ONE_OVER_E_SQUARED_FULL_ANGLE);

      CHECK(divergence_msg.value() == Approx(5));
      CHECK(divergence_msg.unit() == "crad");

      divergence_msg = msg::get_beam_divergence(divergence, msg::BEAM_DIVERGENCE_TYPE_ONE_OVER_E_HALF_ANGLE);

      CHECK(divergence_msg.value() == Approx(2.5 / sqrt(2)));
      CHECK(divergence_msg.unit() == "crad");

      divergence_msg = msg::get_beam_divergence(divergence, msg::BEAM_DIVERGENCE_TYPE_ONE_OVER_E_FULL_ANGLE);

      CHECK(divergence_msg.value() == Approx(5 / sqrt(2)));
      CHECK(divergence_msg.unit() == "crad");

      divergence_msg = msg::get_beam_divergence(divergence, msg::BEAM_DIVERGENCE_TYPE_FWHM_HALF_ANGLE);

      CHECK(divergence_msg.value() == Approx(1.25 / 0.8493218));
      CHECK(divergence_msg.unit() == "crad");

      divergence_msg = msg::get_beam_divergence(divergence, msg::BEAM_DIVERGENCE_TYPE_FWHM_FULL_ANGLE);

      CHECK(divergence_msg.value() == Approx(2.5 / 0.8493218));
      CHECK(divergence_msg.unit() == "crad");
    }
  }

  SECTION("Convert quantity to string")
  {
    quantity<t::cm> L = 12 * i::cm;
    CHECK(boost::lexical_cast<std::string>(L) == "12 cm");
  }

  SECTION("Stream Quantity")
  {
    std::stringstream out;
    msg::Quantity     L;
    L << "2.54 cm";

    out << L;

    CHECK(out.str() == "2.54 cm");
  }

  SECTION("Converting Quantity")
  {
    msg::Quantity L;
    L << "2.54 cm";

    msg::convert(L, "in");

    std::stringstream out;

    out << L;

    CHECK(out.str() == "1 in");
  }
}

TEST_CASE("Optical Element Messages")
{
  using namespace libGBP2;
  msg::OpticalElement element;

  element.mutable_lens()->mutable_focal_length()->set_value(10);
  element.mutable_lens()->mutable_focal_length()->set_unit("mm");

  CHECK(element.has_lens());
  CHECK(!element.has_spherical_refractive_surface());
  CHECK(!element.has_flat_refractive_surface());
  CHECK(!element.has_custom());

  element.mutable_spherical_refractive_surface()->mutable_radius_of_curvature()->set_value(10);
  element.mutable_spherical_refractive_surface()->mutable_radius_of_curvature()->set_unit("mm");
  element.mutable_spherical_refractive_surface()->mutable_refractive_index()->set_value(1.5);
  element.mutable_spherical_refractive_surface()->mutable_refractive_index()->set_unit("dimensionless");

  CHECK(!element.has_lens());
  CHECK(element.has_spherical_refractive_surface());
  CHECK(!element.has_flat_refractive_surface());
  CHECK(!element.has_custom());

  /* std::cout << msg::serialize_message(element) << std::endl; */
}
