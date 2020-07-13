#include "catch.hpp"

#include "GaussianBeam.hpp"
#include "OpticalElements/ThinLens.hpp"
SCENARIO("Gaussian beams can be focused by a lens" )
{
  GIVEN("An ideal, collimated Gaussian beam with wavelength 500 nm, D_L = 5 mm")
  {
    WHEN("Passing through a lens of focal length f = 12 cm")
    {
      THEN("The beam waist diameter is given by the classic equation: d = frac{4 lambda f}{ pi D_L}")
      {
        boost::units::quantity<t::nm> wavelength = 500*i::nm;
        boost::units::quantity<t::mm> diameter = 5*i::mm;
        boost::units::quantity<t::cm> focal_length = 12*i::cm;

        boost::units::quantity<t::um> waist_diameter(4.*wavelength*focal_length/M_PI/diameter);

        GaussianBeam beam;
        beam.setWavelength(wavelength);
        beam.setOneOverE2WaistDiameter(diameter);

        ThinLens<t::centimeter> lens;
        lens.setFocalLength(focal_length);

        beam.transform(&lens,0*i::cm);

        CHECK( beam.getOneOverE2WaistDiameter<t::um>().value() == Approx(waist_diameter.value()));

        


      }
    }
  }
}

SCENARIO("Beam quality factor increases diameter at range." )
{
  GIVEN("A Gaussian beam with M^2 value of 2")
  {
    WHEN("Measuring a 500 nm beam with 10 um waist 1 meter away")
    {
      THEN("The beam diameter will be larger by a factor of 2.")
      {
      }

    }
  }
}
