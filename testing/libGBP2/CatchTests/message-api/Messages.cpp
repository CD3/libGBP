
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
