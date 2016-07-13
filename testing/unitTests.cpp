#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>


#include "GaussianBeam.hpp"

#define WRITE2(a,b) std::cout<< a << " " << b << std::endl
#define WRITE3(a,b,c) std::cout<< a << " " << b << " " << c << std::endl

SCENARIO( "GaussianBeam configuration", "[GaussianBeam]" )
{
  GIVEN("An uninitialized GaussianBeam instance")
  {
    GaussianBeam beam;

    WHEN("the wavelength is set to 700 nm") {
      beam.setWavelength( 700*t::nanometer() );
      THEN("the wavelength getter returns 700 nm") {
        CHECK( beam.getWavelength().value() == Approx(700) );
      }

      AND_WHEN("the wavelength getter is called with nm") {
        THEN("the value of the returned quantity is 700") {
          CHECK( beam.getWavelength<t::nanometer>().value() == Approx(700) );
        }
      }

      AND_WHEN("the wavelength getter is called with um") {
        THEN("the value of the returned quantity is 0.7") {
          CHECK( beam.getWavelength<t::micrometer>().value() == Approx(0.7) );
        }
      }
    }

    WHEN("the wavelength is set to 0.7 um") {
      beam.setWavelength( 0.7*t::micrometer() );
      THEN("the wavelength getter returns 700 nm") {
        CHECK( beam.getWavelength().value() == Approx(700) );
      }
    }

    WHEN("the waist position is is set to 7 m") {
      beam.setWaistPosition( 7*t::meter() );
      THEN("the waist position getter returns 700 cm") {
        CHECK( beam.getWaistPosition().value() == Approx(700) );
      }
    }

    WHEN("the frequency is set to 100 GHz") {
      beam.setFrequency( 100*t::gigahertz() );
      THEN("the freespace wavelength getter returns 2.997925 mm") {
        CHECK( beam.getFreeSpaceWavelength<t::millimeter>().value() == Approx(2.997925) );
      }
    }

    WHEN("the wavelength is set to 0.633 um") {
      beam.setWavelength( 0.633*t::micrometer() );

      AND_WHEN("the beam waist diameter is set to 2 mm") {
        beam.setWaistDiameter( 2*t::millimeter() );


        THEN("the rayleigh range is 4.96302157 m") {
          CHECK( beam.getRayleighRange<t::meter>().value() == Approx(4.96302157) );
        }
        THEN("the half divergence is 0.20149 mrad") {
          CHECK( beam.getDivergence<t::milliradian>().value()/2 == Approx(0.20149) );
        }
        THEN("the radius of curvature at 10 m is 12.46315831 m") {
              CHECK( beam.getRadiusOfCurvature<t::meter>( 10.*t::meter() ).value() == Approx(12.46315831) );
        }
        AND_WHEN("the beam waist position is set to 2 m") {
          beam.setWaistPosition( 2*t::meter() );
          THEN("the diameter at 12 m is 4.498820 mm") {
            CHECK( beam.getDiameter<t::millimeter>( 12.*t::meter() ).value() == Approx(4.498820) );
            CHECK( beam.getRadius<t::millimeter>( 12.*t::meter() ).value() == Approx(4.498820/2) );
          }
          AND_THEN("the radius of curvature at 12 m is 12.46315831 m") {
            CHECK( beam.getRadiusOfCurvature<t::meter>( 12.*t::meter() ).value() == Approx(12.46315831) );
          }
        }
      }
    }
    
    WHEN("the wavelength is set to 532 nm") {
      beam.setWavelength( 532*t::nanometer() );
      AND_WHEN("the waist diameter is set to 20 um") {
        beam.setWaistDiameter( 20.*t::micrometer() );
        THEN("the waist diameter is 20e-6 m"){
          CHECK( beam.getWaistDiameter<t::meter>().value() == Approx(20e-6) );
          CHECK( beam.getWaistDiameter<t::meter>().value() == Approx(10e-6) );
        }

        AND_WHEN("the waist position is set to 10 m") {
          beam.setWaistPosition( 10.*t::meter() );
          THEN("the complex beam parameter at 18 m is 800 + 0.059052 i cm") {
            auto q = beam.getComplexBeamParameter(18*t::meter());
            CHECK( q.value().real() == Approx(800) );
            CHECK( q.value().imag() == Approx(0.059052) );
          }
          THEN("the beam divergence is 33.18 mrad") {
            CHECK( beam.getDivergence<t::milliradian>( ).value() == Approx(33.86818) );
          }
          THEN("the beam diameter at 18 m is 270.94538 mm") {
            CHECK( beam.getDiameter<t::millimeter>( 18*m ).value() == Approx(270.94538) );
            CHECK( beam.getRadius<t::millimeter>( 18*m ).value() == Approx(270.94538/2) );
          }
          THEN("the beam area at 18 m is 576.571779025 cm^2") {
            CHECK( beam.getArea<t::centimeter_squared>( 18*m ).value() == Approx(576.571779025) );
          }
        }
      }
    }
  }
}

SCENARIO( "Complex Beam Parameter Calculations", "[GaussianBeam]" )
{
  GaussianBeam beam;
  WHEN("the beam is configured")
  {
    beam.setWavelength( 532*t::nanometer() );
    beam.setWaistDiameter( 10*t::micrometer() );
    beam.setWaistPosition( 1.2*t::meter() );

    THEN("the real part of 1 / q @ z is equal to the radius of curvature @ z")
    {
      auto q = beam.getComplexBeamParameter<t::centimeter>(8*t::meter());
      auto one_over_q = complex<double>(1.,0.)/q;

      auto R = beam.getRadiusOfCurvature<t::centimeter>(8*t::meter());
      auto one_over_R = 1./R;

      auto w = beam.getDiameter<t::centimeter>(8*t::meter())/2.;

      auto tmp = beam.getWavelength<t::centimeter>() / (M_PI*pow<2>(w) );

      CHECK( one_over_q.value().real() == Approx( one_over_R.value() ) );
      CHECK( one_over_q.value().imag() == Approx( tmp.value()        ) );
    }
  }
}

#include "OpticalElements/ThinLens.hpp"

SCENARIO( "Thin lens class calculations", "[OpticalElements]" )
{
  WHEN("lens is created") {
  ThinLens<t::centimeter> lens;

    AND_WHEN("focal length is set to 10 cm") {
      lens.setFocalLength( 10*t::centimeter() );
      auto mat = lens.getRTMatrix();

      THEN("element (0,0) of the RT Matrix is 1") {
        CHECK( mat(0,0) == Approx(1) );
      }
      AND_THEN("element (0,1) of the RT Matrix is 0") {
        CHECK( mat(0,1) == Approx(0) );
      }
      AND_THEN("element (1,0) of the RT Matrix is 1/10") {
        CHECK( mat(1,0) == Approx(-1./10) );
      }
      AND_THEN("element (1,1) of the RT Matrix is 1") {
        CHECK( mat(1,1) == Approx(1) );
      }
    }

    AND_WHEN("focal length is set to 1 m") {
      lens.setFocalLength( 1*t::meter() );
      auto mat = lens.getRTMatrix();

      THEN("element (0,0) of the RT Matrix is 1") {
        CHECK( mat(0,0) == Approx(1) );
      }
      AND_THEN("element (0,1) of the RT Matrix is 0") {
        CHECK( mat(0,1) == Approx(0) );
      }
      AND_THEN("element (1,0) of the RT Matrix is 1/100") {
        CHECK( mat(1,0) == Approx(-1./100) );
      }
      AND_THEN("element (1,1) of the RT Matrix is 1") {
        CHECK( mat(1,1) == Approx(1) );
      }
    }
  }
}

#include "OpticalElements/FlatInterface.hpp"

SCENARIO( "Flat interface class calculations", "[OpticalElements]" )
{
  WHEN("interface is created") {
  FlatInterface<t::centimeter> interface;

    AND_WHEN("the initial refractive index is set to 1") {
      interface.setInitialRefractiveIndex(1);

      AND_WHEN("the final refractive index is set to 2") {
        interface.setFinalRefractiveIndex(2);

        auto mat = interface.getRTMatrix();

        THEN("element (0,0) of the RT Matrix is 1") {
          CHECK( mat(0,0) == Approx(1) );
        }
        AND_THEN("element (0,1) of the RT Matrix is 0") {
          CHECK( mat(0,1) == Approx(0) );
        }
        AND_THEN("element (1,0) of the RT Matrix is 0") {
          CHECK( mat(1,0) == Approx(0) );
        }
        AND_THEN("element (1,1) of the RT Matrix is 1/2") {
          CHECK( mat(1,1) == Approx(1./2) );
        }
      }
    }



    AND_WHEN("the initial refractive index is set to 2") {
      interface.setInitialRefractiveIndex(2);

      AND_WHEN("the final refractive index is set to 1") {
        interface.setFinalRefractiveIndex(1);

        auto mat = interface.getRTMatrix();

        THEN("element (0,0) of the RT Matrix is 1") {
          CHECK( mat(0,0) == Approx(1) );
        }
        AND_THEN("element (0,1) of the RT Matrix is 0") {
          CHECK( mat(0,1) == Approx(0) );
        }
        AND_THEN("element (1,0) of the RT Matrix is 0") {
          CHECK( mat(1,0) == Approx(0) );
        }
        AND_THEN("element (1,1) of the RT Matrix is 2") {
          CHECK( mat(1,1) == Approx(2) );
        }
      }
    }
  }
}

#include "OpticalElements/SphericalInterface.hpp"

SCENARIO( "Spherical interface class calculations", "[OpticalElements]" )
{
  WHEN("interface is created") {
  SphericalInterface<t::centimeter> interface;

    AND_WHEN("the initial refractive index is set to 1") {
      interface.setInitialRefractiveIndex(1);

      AND_WHEN("the final refractive index is set to 2") {
        interface.setFinalRefractiveIndex(2);

        AND_WHEN("the radius of curvature is set to +10 cm") {
          interface.setRadiusOfCurvature( 10*t::centimeter() );

          auto mat = interface.getRTMatrix();

          THEN("element (0,0) of the RT Matrix is 1") {
            CHECK( mat(0,0) == Approx(1) );
          }
          AND_THEN("element (0,1) of the RT Matrix is 0") {
            CHECK( mat(0,1) == Approx(0) );
          }
          AND_THEN("element (1,0) of the RT Matrix is -(2-1)/(10*2)") {
            CHECK( mat(1,0) == Approx(-1./20) );
          }
          AND_THEN("element (1,1) of the RT Matrix is 1/2") {
            CHECK( mat(1,1) == Approx(1./2) );
          }
        }

        AND_WHEN("the radius of curvature is set to -35 mm") {
          interface.setRadiusOfCurvature( -35*t::millimeter() );

          auto mat = interface.getRTMatrix();

          THEN("element (0,0) of the RT Matrix is 1") {
            CHECK( mat(0,0) == Approx(1) );
          }
          AND_THEN("element (0,1) of the RT Matrix is 0") {
            CHECK( mat(0,1) == Approx(0) );
          }
          AND_THEN("element (1,0) of the RT Matrix is +(2-1)/(3.5*2)") {
            CHECK( mat(1,0) == Approx(+1./7) );
          }
          AND_THEN("element (1,1) of the RT Matrix is 1/2") {
            CHECK( mat(1,1) == Approx(1./2) );
          }
        }
      }
    }
  }
}

#include "GaussianBeam.hpp"
#include "OpticalElements/ThinLens.hpp"
#include "OpticalElements/SphericalInterface.hpp"
#include "BeamConfigurator.hpp"

TEST_CASE( "Gaussian Beam Transformations", "[OpticalElements,GuassianBeam]" )
{
  GaussianBeam beam;
  BeamConfigurator config;

  SECTION("Thin lens examples") {

    beam.setWavelength(    532*nm );
    beam.setWaistDiameter( 0.01*mm );
    beam.setWaistPosition( 0*cm );

    SECTION("a 10 mm lens") {

      ThinLens<t::centimeter> lens;
      lens.setFocalLength( 10*mm );

      CHECK( beam.getWavelength().value() == Approx(532) );
      CHECK( beam.getWaistPosition().value() == Approx(0) );
      CHECK( beam.getWaistDiameter().value() == Approx(10e-4) );
      CHECK( beam.getDivergence().value() == Approx(2*33.868) );

      auto d = beam.getDiameter(100*mm);

      beam.transform( &lens, 100*mm );

      CHECK( beam.getDiameter(100*mm).value() == Approx( d.value() ) );

      CHECK( beam.getWavelength().value() == Approx(532) );
      CHECK( beam.getRelativeWaistPosition<t::millimeter>(100*mm).value() == Approx(-11.111) );
      CHECK( beam.getWaistPosition<t::millimeter>().value() == Approx(100 + 11.111) );
      CHECK( beam.getWaistDiameter<t::millimeter>().value() == Approx(0.00111) );
      CHECK( beam.getRayleighRange<t::millimeter>().value() == Approx(0.001823) );
    }
  }

  SECTION("Spherical surface examples") {

    config.setWavelength( 532*nm );
    config.setPower( 10*mW );
    config.setPosition( 0*cm ).setDiameter( 2*0.5*mm ).setDivergence( 2*0.75*mrad);
    config.configure(beam);

    CHECK( beam.getWavelength<t::micrometer>().value()      == Approx(0.532) );
    CHECK( beam.getPower<t::watt>().value()                 == Approx(0.01) );

    CHECK( beam.getRadius<t::millimeter>(0*cm).value()      == Approx(0.5) );
    CHECK( beam.getRadius<t::millimeter>(10*cm).value()     == Approx(0.56793) );

    CHECK( beam.getWaistPosition<t::millimeter>().value()   == Approx(-594.822) );
    CHECK( beam.getWaistRadius<t::millimeter>().value()     == Approx(0.225788) );
    CHECK( beam.getRayleighRange<t::millimeter>().value()   == Approx(301.05042) );

    SECTION("a 10 mm surface") {

      SphericalInterface<t::centimeter> surface;
      surface.setRadiusOfCurvature( 10*mm );

      SECTION("a 1 <--> 1 refractive index") {

        surface.setInitialRefractiveIndex( 1 );
        surface.setFinalRefractiveIndex( 1 );

        beam.transform( &surface, 10*cm );

        CHECK( beam.getWavelength<t::micrometer>().value()      == Approx(0.532) );
        CHECK( beam.getPower<t::watt>().value()                 == Approx(0.01) );

        CHECK( beam.getRadius<t::millimeter>(0*cm).value()      == Approx(0.5) );
        CHECK( beam.getRadius<t::millimeter>(10*cm).value()     == Approx(0.56793) );

        CHECK( beam.getWaistPosition<t::millimeter>().value()   == Approx(-594.822) );
        CHECK( beam.getWaistDiameter<t::millimeter>().value()/2 == Approx(0.225788) );

      }

      SECTION("a 1 <--> 2 refractive index") {

        surface.setInitialRefractiveIndex( 1 );
        surface.setFinalRefractiveIndex( 2 );

        beam.transform( &surface, 100*mm );

        CHECK( beam.getWaistPosition<t::millimeter>().value()   == Approx(100+20.2447486566) );
        CHECK( beam.getRayleighRange<t::millimeter>().value()   == Approx(0.10759247802) );

        CHECK( beam.getWaistRadius<t::millimeter>().value()     == Approx(0.00301826) );
        CHECK( beam.getRadius<t::millimeter>(10*cm).value()     == Approx(0.56793) );   // beam diameter at position of the surface should not change

        CHECK( beam.getWavelength<t::micrometer>().value()      == Approx(0.532/2) );
        CHECK( beam.getPower<t::watt>().value()                 == Approx(0.01) );

      }
    }
  }

  SECTION( "Ramrod example worksheet" )
  {

    SECTION( "Problem 1" )
    {
      beam.setWavelength(    444*nm );
      beam.setWaistDiameter( 2.5*mm );
      beam.setWaistPosition(   0*cm );
      beam.setPower(         800*mW );

      ThinLens<t::centimeter> lens;
      lens.setFocalLength( 120*mm );

      quantity<t::centimeter> lensPosition = 15*cm;

      WRITE2( "waist diameter ", beam.getWaistDiameter() );
      WRITE3( "1.a ",beam.getPeakIrradiance(lensPosition).value()," W/cm^2" );
      WRITE2( "1.b ",beam.getDiameter(lensPosition) );
      WRITE2( "1.c ",beam.getRadiusOfCurvature(lensPosition) );
      beam.transform( &lens, 15*cm );
      WRITE2( "1.d ", beam.getRadiusOfCurvature(lensPosition) );
      WRITE2( "1.e ", beam.getWaistPosition() );
      auto irr = beam.getPower() / ((M_PI/4)*beam.getWaistDiameter()*beam.getWaistDiameter());
      WRITE2( "waist diameter ",beam.getWaistDiameter() );
      WRITE2( "1.f ", irr.value() );
      WRITE3( "1.f ", beam.getPeakIrradiance( beam.getWaistPosition() ).value(), "W/cm^2" );


    }

    SECTION( "Problem 2" )
    {

      
      beam.setPower(         5*mW );
      BeamConfigurator config;

      config.setWavelength( 532*nm );
      config.setPosition(0*cm).setDiameter(3*mm).setDivergence(2*mrad);

      config.configure( beam );


      SphericalInterface<t::centimeter> cornea;
      cornea.setRadiusOfCurvature( 6.1*mm );
      cornea.setInitialRefractiveIndex( 1.0    );
      cornea.setFinalRefractiveIndex(   1.3369 );

      std::cout<<std::endl;
      WRITE2( "2.a", beam.getWaistPosition() );
      WRITE2( "2.b", beam.getRadiusOfCurvature(0*cm) );

      beam.transform( &cornea, 0*cm );

      WRITE2( "--wavelength", beam.getWavelength() );
      WRITE2( "--RoC", beam.getRadiusOfCurvature(0*cm) );
      WRITE2( "--Diam (at cornea)", beam.getDiameter(0*cm) );
      WRITE2( "--z_0", beam.getWaistPosition() );
      WRITE2( "--w_0", beam.getWaistDiameter() );
      WRITE2( "2.c", beam.getDiameter(2.44*cm) );
      WRITE2( "2.d", beam.getDiameter(2.44*cm) );
      WRITE2( "2.e", beam.getRadiusOfCurvature(2.44*cm) );
      WRITE3( "2.f", beam.getPeakIrradiance(2.44*cm).value(), "W/cm^2" );


      


    }
  }
}

#include "GaussianBeam.hpp"
#include "BeamConfigurator.hpp"

TEST_CASE( "BeamConfigurator Tests" )
{
  BeamConfigurator config;

  SECTION("Internal Units")
  {
    config.setWavelength(532*nm).setDivergence(2.5*mrad);

    CHECK( config.getWavelength<t::nanometer>().value().value() == Approx(532) );
    CHECK( config.getDivergence<t::milliradian>().value().value() == Approx(2.5) );
  }

  SECTION("Unit Conversions")
  {
    config.setWavelength(0.532*um).setDivergence(0.0025*rad);

    CHECK( config.getWavelength<t::nanometer>().value().value() == Approx(532) );
    CHECK( config.getDivergence<t::milliradian>().value().value() == Approx(2.5) );

    config.setWavelength(0.444*um).setDivergence(0.001*rad);

    CHECK( config.getWavelength<t::nanometer>().value().value() == Approx(444) );
    CHECK( config.getDivergence<t::milliradian>().value().value() == Approx(1) );
  }

  SECTION("Arrays")
  {
    config.setWavelength(0.532*um);
    config.setPosition(1.0*cm).setDiameter(2.0*mm);
    config.setPosition(10.*cm).setDiameter(4.0*mm);

    CHECK( config.Wavelength.size() == 1 );
    CHECK( config.Position.size() == 2 );
    CHECK( config.Diameter.size() == 2 );

    CHECK( config.getWavelength<t::nanometer>().value().value() == Approx(532) );
    CHECK( config.getPosition<t::centimeter>().value().value() == Approx(1) );
    CHECK( config.getPosition<t::centimeter>(0).value().value() == Approx(1) );
    CHECK( config.getPosition<t::centimeter>(1).value().value() == Approx(10) );
    CHECK( config.getDiameter<t::centimeter>().value().value() == Approx(0.2) );
    CHECK( config.getDiameter<t::centimeter>(0).value().value() == Approx(0.2) );
    CHECK( config.getDiameter<t::centimeter>(1).value().value() == Approx(0.4) );
  }

  SECTION("Beam configuration")
  {
    GaussianBeam beam;

    config.setWavelength(532*nm);
    config.setDivergence(10*mrad);
    config.setDiameter(2.5000*mm).setPosition(0.0*meter);

    config.configure(beam);

    CHECK( beam.getWavelength().value() == Approx(532) );
    CHECK( beam.getWaistDiameter<t::millimeter>().value() == Approx( 2*0.033868 ) );
    CHECK( beam.getRayleighRange<t::millimeter>().value() == Approx( 6.77357) );
    CHECK( beam.getRadiusOfCurvature<t::millimeter>(0*mm).value() == Approx( 250.09 ) );
    CHECK( beam.getWaistPosition<t::millimeter>().value() == Approx( -249.908 ) );

  }

  SECTION("Beam configuration, offset position")
  {
    GaussianBeam beam;

    config.setWavelength(532*nm);
    config.setDivergence(10*mrad);
    config.setDiameter(2.5000*mm).setPosition(1.2*meter);

    config.configure(beam);

    CHECK( beam.getWavelength().value() == Approx(532) );
    CHECK( beam.getWaistDiameter<t::millimeter>().value() == Approx( 2*0.033868 ) );
    CHECK( beam.getRayleighRange<t::millimeter>().value() == Approx( 6.77357) );
    CHECK( beam.getRadiusOfCurvature<t::millimeter>(1200*mm).value() == Approx( 250.09 ) );
    CHECK( beam.getWaistPosition<t::millimeter>().value() == Approx( 1200-249.908 ) );

  }
}



