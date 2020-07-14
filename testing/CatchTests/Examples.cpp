#include "catch.hpp"

#include <libGBP/GaussianBeam.hpp>
#include <libGBP/OpticalElements/ThinLens.hpp>

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

TEST_CASE("The beam propagation (beam quality) factor increases diameter at range." )
{
  GaussianBeam beam;
  beam.setWavelength(500*i::nm);
  beam.setOneOverESquaredWaistDiameter(10*i::um);

  // by default, the diffraction limited divergence will be used.
  auto w_z_1 = beam.getOneOverESquaredDiameter(10*cm);

  beam.setOneOverESquaredHalfAngleDivergence( 2*beam.getOneOverESquaredHalfAngleDiffractionLimitedDivergence() );

  auto w_z_2 = beam.getOneOverESquaredDiameter(10*cm);

  CHECK( beam.getBeamPropagationFactor() == Approx(2) );
  CHECK( w_z_2.value() == Approx(2*w_z_1.value()) );

}


TEST_CASE("Non-ideal beams can be propagated with an 'embedded Gaussian'" )
{

  GaussianBeam beam;
  double M = sqrt(2);
  beam.setWavelength(500*i::nm);

  // embedded Gaussian
  beam.setOneOverESquaredWaistDiameter(10*i::um/M);
  auto w_z_1 = M*beam.getOneOverESquaredDiameter(10*cm);
  CHECK( beam.getBeamPropagationFactor() == Approx(1) );

  // actual Gaussian
  beam.setOneOverESquaredWaistDiameter(10*i::um);
  beam.setOneOverESquaredHalfAngleDivergence( M*M*beam.getOneOverESquaredHalfAngleDiffractionLimitedDivergence() );

  auto w_z_2 = beam.getOneOverESquaredDiameter(10*cm);

  CHECK( beam.getBeamPropagationFactor() == Approx(2) );
  CHECK( w_z_2.value() == Approx(w_z_1.value()) );


}

TEST_CASE("Non-ideal beams have a larger beam waist for the same divergence and a larger divergence for the same beam waist." )
{
  double w0 = 500e-9/M_PI/20e-3;
  double theta = 500e-9/M_PI/10e-6;

  GaussianBeam beam;
  beam.setWavelength(500*i::nm);

  beam.setOneOverESquaredHalfAngleDivergence(20*i::mrad);
  beam.adjustWaistSizeToBeamPropagationFactor(3*t::dimensionless{});
  CHECK( beam.getOneOverESquaredHalfAngleDivergence<t::rad>().value() == Approx(20e-3));
  CHECK( beam.getOneOverESquaredRadius<t::m>().value() == Approx(w0*3));

  beam.setOneOverESquaredWaistRadius(10*i::um);
  beam.adjustDivergenceToBeamPropagationFactor(3*t::dimensionless{});
  CHECK( beam.getOneOverESquaredWaistRadius<t::m>().value() == Approx(10e-6));
  CHECK( beam.getOneOverESquaredHalfAngleDivergence<t::rad>().value() == Approx(theta*3));


  
}
