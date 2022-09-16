#include "catch.hpp"

#include <libGBP/GaussianBeam.hpp>
#include <libGBP/OpticalElements/SphericalInterface.hpp>
#include <libGBP/OpticalElements/ThinLens.hpp>
#include <libGBP/OpticalElements/Translation.hpp>

TEST_CASE("ABCD Matrix Transormations")
{
  boost::units::quantity<t::nm> wavelength     = 500 * i::nm;
  boost::units::quantity<t::um> waist_radius  = 10 * i::um;

  GaussianBeam beam;
  beam.setWavelength(wavelength);
  beam.setOneOverE2WaistRadius(waist_radius);

  SECTION("Translation")
  {
    Translation<t::cm> Translation;
    Translation.setShift(11 * i::cm);

    CHECK(beam.getCurrentPosition().value() == Approx(0).scale(1));
    CHECK(beam.getWaistPosition().value() == Approx(0).scale(1));
    CHECK(beam.getOneOverE2WaistRadius().value() == Approx(10e-4));
    CHECK(beam.getOneOverE2Radius().value() == Approx(10e-4));

    beam.transform( &Translation );

    CHECK(beam.getCurrentPosition().value() == Approx(0).scale(1));
    CHECK(beam.getWaistPosition().value() == Approx(-11));
    CHECK(beam.getOneOverE2WaistRadius().value() == Approx(10e-4));
    auto div = beam.getOneOverESquaredHalfAngleDivergence<t::rad>().value();
    CHECK(beam.getOneOverE2Radius().value() == Approx( sqrt( pow(10e-4,2) + pow(div*11,2) ) ));



  }
}
