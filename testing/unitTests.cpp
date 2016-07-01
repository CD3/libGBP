#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>


#include "GaussianBeam.hpp"
#include "OpticalElements/ThinLens.hpp"
#include "OpticalElements/FlatInterface.hpp"
#include "OpticalElements/SphericalInterface.hpp"

SCENARIO( "GaussianBeam can be configured", "[GaussianBeam]" )
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
        AND_THEN("the half divergence is 0.20149 mrad") {
          CHECK( beam.getDivergence<t::milliradian>().value()/2 == Approx(0.20149) );
        }
        AND_WHEN("the beam waist position is set to 2 m") {
          beam.setWaistPosition( 2*t::meter() );
          THEN("the diameter at 12 m is 4.498820 mm") {
            CHECK( beam.getDiameter<t::millimeter>( 12.*t::meter() ).value() == Approx(4.498820) );
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
        AND_WHEN("the waist position is set to 10 m") {
          beam.setWaistPosition( 10.*t::meter() );
          THEN("the real complex beam parameter at 18 m is 800 + 0.059052 i cm") {
            auto q = beam.getComplexBeamParameter(18*t::meter());
            CHECK( q.value().real() == Approx(800) );
            CHECK( q.value().imag() == Approx(0.059052) );

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

SCENARIO( "Thin lens class calculations", "[OpticalElements]" )
{
  WHEN("lens is created using centimeter as internal units") {
  ThinLens< t::centimeter > lens;

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

SCENARIO( "Flat interface class calculations", "[OpticalElements]" )
{
  WHEN("interface is created") {
  FlatInterface interface;

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

SCENARIO( "Spherical interface class calculations", "[OpticalElements]" )
{
  WHEN("interface is created with centimeter for as the default unit") {
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
