#include "catch.hpp"

#include <libGBP/GaussianBeam.hpp>
#include <libGBP/OpticalElements/ThinLens.hpp>
#include <libGBP/OpticalElements/SphericalInterface.hpp>

SCENARIO("Gaussian beams can be focused by a lens" )
{
  GIVEN("An ideal, collimated Gaussian beam with wavelength 500 nm, D_L = 5 mm")
  {
    WHEN("Passing through a lens of focal length f = 12 cm")
    {
      THEN("The beam waist diameter is given by the classic equation: d = frac{4 lambda f}{ pi D_L}")
      {
        boost::units::quantity<units::t::nm> wavelength = 500*units::i::nm;
        boost::units::quantity<units::t::mm> diameter = 5*units::i::mm;
        boost::units::quantity<units::t::cm> focal_length = 12*units::i::cm;

        boost::units::quantity<units::t::um> waist_diameter(4.*wavelength*focal_length/M_PI/diameter);

        GaussianBeam beam;
        beam.setWavelength(wavelength);
        beam.setOneOverE2WaistDiameter(diameter);

        ThinLens<units::t::centimeter> lens;
        lens.setFocalLength(focal_length);

        beam.transform(&lens,0*units::i::cm);

        CHECK( beam.getOneOverE2WaistDiameter<units::t::um>().value() == Approx(waist_diameter.value()));

        


      }
    }
  }
}

TEST_CASE("The beam propagation (beam quality) factor increases diameter at range." )
{
  GaussianBeam beam;
  beam.setWavelength(500*units::i::nm);
  beam.setOneOverESquaredWaistDiameter(10*units::i::um);

  // by default, the diffraction limited divergence will be used.
  auto w_z_1 = beam.getOneOverESquaredDiameter(10*units::i::cm);

  beam.setOneOverESquaredHalfAngleDivergence( 2*beam.getOneOverESquaredHalfAngleDiffractionLimitedDivergence() );

  auto w_z_2 = beam.getOneOverESquaredDiameter(10*units::i::cm);

  CHECK( beam.getBeamPropagationFactor() == Approx(2) );
  CHECK( w_z_2.value() == Approx(2*w_z_1.value()) );

}


TEST_CASE("Non-ideal beams can be propagated with an 'embedded Gaussian'" )
{

  GaussianBeam beam;
  double M = sqrt(2);
  beam.setWavelength(500*units::i::nm);

  // embedded Gaussian
  beam.setOneOverESquaredWaistDiameter(10*units::i::um/M);
  auto w_z_1 = M*beam.getOneOverESquaredDiameter(10*units::i::cm);
  CHECK( beam.getBeamPropagationFactor() == Approx(1) );

  // actual Gaussian
  beam.setOneOverESquaredWaistDiameter(10*units::i::um);
  beam.setOneOverESquaredHalfAngleDivergence( M*M*beam.getOneOverESquaredHalfAngleDiffractionLimitedDivergence() );

  auto w_z_2 = beam.getOneOverESquaredDiameter(10*units::i::cm);

  CHECK( beam.getBeamPropagationFactor() == Approx(2) );
  CHECK( w_z_2.value() == Approx(w_z_1.value()) );


}

TEST_CASE("Non-ideal beams have a larger beam waist for the same divergence and a larger divergence for the same beam waist." )
{
  double w0 = 500e-9/M_PI/20e-3;
  double theta = 500e-9/M_PI/10e-6;

  GaussianBeam beam;
  beam.setWavelength(500*units::i::nm);

  beam.setOneOverESquaredHalfAngleDivergence(20*units::i::mrad);
  beam.adjustWaistSizeToBeamPropagationFactor(3*units::t::dimensionless{});
  CHECK( beam.getOneOverESquaredHalfAngleDivergence<units::t::rad>().value() == Approx(20e-3));
  CHECK( beam.getOneOverESquaredRadius<units::t::m>().value() == Approx(w0*3));

  beam.setOneOverESquaredWaistRadius(10*units::i::um);
  beam.adjustDivergenceToBeamPropagationFactor(3*units::t::dimensionless{});
  CHECK( beam.getOneOverESquaredWaistRadius<units::t::m>().value() == Approx(10e-6));
  CHECK( beam.getOneOverESquaredHalfAngleDivergence<units::t::rad>().value() == Approx(theta*3));


  
}

TEST_CASE("Retinal image size")
{
  double A = 1;
  double B1 = 7.516e-1;
  double B2 = -4.484e-3;
  double B3 = -1.503e1;
  double C1 = 1.641e-2;
  double C2 = 8.596e-2;
  double C3 = -1.028e3;
  double l;

  GaussianBeam beam;
  beam.setOneOverESquaredWaistDiameter(4.24*units::i::mm);

  SphericalInterface<units::t::centimeter> cornea;
  cornea.setRadiusOfCurvature(6.1*units::i::mm);
  cornea.setInitialRefractiveIndex(1);

  SECTION("589 nm reference")
  {
    beam.setWavelength(589*units::i::nm);

    l = beam.getWavelength<units::t::um>().value();
    double n = sqrt(A + B1*l*l/(l*l - C1)
                      + B2*l*l/(l*l - C2)
                      + B3*l*l/(l*l - C3));
    cornea.setFinalRefractiveIndex(n);

    beam.transform(&cornea);

    CHECK(beam.getWaistPosition<units::t::mm>().value() == Approx(24.4).epsilon(0.01));
  }
  SECTION("1300 nm")
  {
    beam.setWavelength(1300*units::i::nm);

    l = beam.getWavelength<units::t::um>().value();
    double n = sqrt(A + B1*l*l/(l*l - C1)
                      + B2*l*l/(l*l - C2)
                      + B3*l*l/(l*l - C3));
    cornea.setFinalRefractiveIndex(n);

    beam.transform(&cornea);

    CHECK(beam.getOneOverESquaredDiameter<units::t::um>(2.44*units::i::cm).value() == Approx(180).epsilon(0.5));
  }


}
