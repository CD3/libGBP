#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include <fstream>


#include <libGBP/GaussianBeam.hpp>
#include <libGBP/BeamTransformations/SphericalInterface.hpp>
#include <libGBP/Builders/BeamBuilder.hpp>

using namespace libGBP;

TEST_CASE( "Retinal diameter vs Corneal diameter", "[Applications]" )
{

  GaussianBeam beam;

  SphericalInterface<t::centimeter> cornea;
  cornea.setRadiusOfCurvature( 6.1*mm );
  cornea.setInitialRefractiveIndex( 1.0    );
  cornea.setFinalRefractiveIndex(   1.3369 );

  beam.setPower(         5*mW );


  int N = 100;
  auto minDiam = quantity<t::centimeter>(10*um);
  auto maxDiam = quantity<t::centimeter>(1*cm);
  auto dDiam   = (maxDiam-minDiam)/(N-1.);
  auto minDiv  = quantity<t::milliradian>(1*mrad);
  auto maxDiv  = quantity<t::milliradian>(100*mrad);
  auto dDiv    = (maxDiv-minDiv)/(N-1.);
  auto z = 2.44*cm;

  std::ofstream out("Dr_vs_Dc.txt");
  for( int i = 0; i < N; i++)
  {
    auto Div = minDiv + i*1.*dDiv;
    for( int j = 0; j < N; j++)
    {
      auto Diam = minDiam + j*1.*dDiam;
      BeamBuilder config;

      config.setWavelength( 532*nm );
      config.setPosition(0*cm).setOneOverE2Diameter(Diam).setOneOverE2FullAngleDivergence(Div);
      try {
      config.configure( beam );
      } catch (const std::runtime_error& e) {
        continue;
      }

      out << beam.getOneOverE2Diameter<t::centimeter>() << " ";
      out << beam.getOneOverE2HalfAngleDivergence<t::milliradian>() << " ";
      beam.transform( &cornea, 0*cm );
      out << beam.getOneOverE2Diameter<t::centimeter>(z) << " ";
      out << beam.getOneOverE2HalfAngleDivergence<t::milliradian>() << " ";
      out<<"\n";

    }
    out<<"\n";
  }
  out.close();

}

#include <libGBP/GaussianBeam.hpp>
#include <libGBP/Builders/BeamBuilder.hpp>

TEST_CASE( "BeamBuilder Tests" )
{
  BeamBuilder config;

  SECTION("Internal Units")
  {
    config.setWavelength(532*nm).setOneOverE2FullAngleDivergence(2.5*mrad);

    CHECK( config.getWavelength<t::nanometer>().value().value() == Approx(532) );
    CHECK( config.getOneOverE2FullAngleDivergence<t::milliradian>().value().value() == Approx(2.5) );
  }

  SECTION("Unit Conversions")
  {
    config.setWavelength(0.532*um).setOneOverE2FullAngleDivergence(0.0025*rad);

    CHECK( config.getWavelength<t::nanometer>().value().value() == Approx(532) );
    CHECK( config.getOneOverE2FullAngleDivergence<t::milliradian>().value().value() == Approx(2.5) );

    config.setWavelength(0.444*um).setOneOverE2FullAngleDivergence(0.001*rad);

    CHECK( config.getWavelength<t::nanometer>().value().value() == Approx(444) );
    CHECK( config.getOneOverE2FullAngleDivergence<t::milliradian>().value().value() == Approx(1) );
  }

  SECTION("Arrays")
  {
    config.setWavelength(0.532*um);
    config.setPosition(1.0*cm).setOneOverE2Diameter(2.0*mm);
    config.setPosition(10.*cm).setOneOverE2Diameter(4.0*mm);

    CHECK( config.Wavelength.size() == 1 );
    CHECK( config.Position.size() == 2 );
    CHECK( config.OneOverE2Diameter.size() == 2 );

    CHECK( config.getWavelength<t::nanometer>().value().value() == Approx(532) );
    CHECK( config.getPosition<t::centimeter>().value().value() == Approx(1) );
    CHECK( config.getPosition<t::centimeter>(0).value().value() == Approx(1) );
    CHECK( config.getPosition<t::centimeter>(1).value().value() == Approx(10) );
    CHECK( config.getOneOverE2Diameter<t::centimeter>().value().value() == Approx(0.2) );
    CHECK( config.getOneOverE2Diameter<t::centimeter>(0).value().value() == Approx(0.2) );
    CHECK( config.getOneOverE2Diameter<t::centimeter>(1).value().value() == Approx(0.4) );
  }

  SECTION("Beam configuration")
  {
    GaussianBeam beam;

    config.setWavelength(532*nm);
    config.setOneOverE2FullAngleDivergence(10*mrad);
    config.setOneOverE2Diameter(2.5000*mm).setPosition(0.0*meter);

    config.configure(beam);

    CHECK( beam.getWavelength().value() == Approx(532) );
    CHECK( beam.getOneOverE2WaistDiameter<t::millimeter>().value() == Approx( 2*0.033868 ) );
    CHECK( beam.getRayleighRange<t::millimeter>().value() == Approx( 6.77357) );
    CHECK( beam.getRadiusOfCurvature<t::millimeter>(0*mm).value() == Approx( 250.09 ) );
    CHECK( beam.getWaistPosition<t::millimeter>().value() == Approx( -249.908 ) );

  }

  SECTION("Beam configuration, offset position")
  {
    GaussianBeam beam;

    config.setWavelength(532*nm);
    config.setOneOverE2FullAngleDivergence(10*mrad);
    config.setOneOverE2Diameter(2.5000*mm).setPosition(1.2*meter);

    config.configure(beam);

    CHECK( beam.getWavelength().value() == Approx(532) );
    CHECK( beam.getOneOverE2WaistDiameter<t::millimeter>().value() == Approx( 2*0.033868 ) );
    CHECK( beam.getRayleighRange<t::millimeter>().value() == Approx( 6.77357) );
    CHECK( beam.getRadiusOfCurvature<t::millimeter>(1200*mm).value() == Approx( 250.09 ) );
    CHECK( beam.getWaistPosition<t::millimeter>().value() == Approx( 1200-249.908 ) );

  }
}



