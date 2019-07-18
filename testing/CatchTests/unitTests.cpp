#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#define WRITE2(a, b) std::cout << a << " " << b << std::endl
#define WRITE3(a, b, c) std::cout << a << " " << b << " " << c << std::endl

#include "GaussianBeam.hpp"
SCENARIO("GaussianBeam configuration", "[GaussianBeam]")
{
  GIVEN("An uninitialized GaussianBeam instance")
  {
    GaussianBeam beam;

    WHEN("the wavelength is set to 700 nm")
    {
      beam.setWavelength(700 * t::nanometer());
      THEN("the wavelength getter returns 700 nm")
      {
        CHECK(beam.getWavelength().value() == Approx(700));
      }

      AND_WHEN("the wavelength getter is called with nm")
      {
        THEN("the value of the returned quantity is 700")
        {
          CHECK(beam.getWavelength<t::nanometer>().value() == Approx(700));
        }
      }

      AND_WHEN("the wavelength getter is called with um")
      {
        THEN("the value of the returned quantity is 0.7")
        {
          CHECK(beam.getWavelength<t::micrometer>().value() == Approx(0.7));
        }
      }
    }

    WHEN("the wavelength is set to 0.7 um")
    {
      beam.setWavelength(0.7 * t::micrometer());
      THEN("the wavelength getter returns 700 nm")
      {
        CHECK(beam.getWavelength().value() == Approx(700));
      }
    }

    WHEN("the waist position is is set to 7 m")
    {
      beam.setWaistPosition(7 * t::meter());
      THEN("the waist position getter returns 700 cm")
      {
        CHECK(beam.getWaistPosition().value() == Approx(700));
      }
    }

    WHEN("the frequency is set to 100 GHz")
    {
      beam.setFrequency(100 * t::gigahertz());
      THEN("the freespace wavelength getter returns 2.997925 mm")
      {
        CHECK(beam.getFreeSpaceWavelength<t::millimeter>().value() ==
              Approx(2.997925));
      }
    }

    WHEN("the wavelength is set to 0.633 um")
    {
      beam.setWavelength(0.633 * t::micrometer());

      AND_WHEN("the beam waist diameter is set to 2 mm")
      {
        beam.setOneOverE2WaistDiameter(2 * t::millimeter());

        THEN("the rayleigh range is 4.96302157 m")
        {
          CHECK(beam.getRayleighRange<t::meter>().value() ==
                Approx(4.96302157));
        }
        THEN("the full angle divergence is 0.20149 mrad")
        {
          CHECK(beam.getOneOverE2FullAngleDivergence<t::milliradian>().value() /
                    2 ==
                Approx(0.20149));
        }
        THEN("the radius of curvature at 10 m is 12.46315831 m")
        {
          CHECK(beam.getRadiusOfCurvature<t::meter>(10. * t::meter()).value() ==
                Approx(12.46315831));
        }
        AND_WHEN("the beam waist position is set to 2 m")
        {
          beam.setWaistPosition(2 * t::meter());
          THEN("the diameter at 12 m is 4.498820 mm")
          {
            CHECK(beam.getOneOverE2Diameter<t::millimeter>(12. * t::meter())
                      .value() == Approx(4.498820));
            CHECK(beam.getOneOverE2Radius<t::millimeter>(12. * t::meter())
                      .value() == Approx(4.498820 / 2));
          }
          AND_THEN("the radius of curvature at 12 m is 12.46315831 m")
          {
            CHECK(
                beam.getRadiusOfCurvature<t::meter>(12. * t::meter()).value() ==
                Approx(12.46315831));
          }
        }
      }
    }

    WHEN("the wavelength is set to 532 nm")
    {
      beam.setWavelength(532 * t::nanometer());
      AND_WHEN("the waist diameter is set to 20 um")
      {
        beam.setOneOverE2WaistDiameter(20. * t::micrometer());
        THEN("the waist diameter is 20e-6 m")
        {
          CHECK(beam.getOneOverE2WaistDiameter<t::meter>().value() ==
                Approx(20e-6));
          CHECK(beam.getOneOverE2WaistDiameter<t::meter>().value() ==
                Approx(10e-6));
        }

        AND_WHEN("the waist position is set to 10 m")
        {
          beam.setWaistPosition(10. * t::meter());
          THEN("the complex beam parameter at 18 m is 800 + 0.059052 i cm")
          {
            auto q = beam.getComplexBeamParameter(18 * t::meter());
            CHECK(q.value().real() == Approx(800));
            CHECK(q.value().imag() == Approx(0.059052));
          }
          THEN("the beam divergence is 33.18 mrad")
          {
            CHECK(beam.getOneOverE2FullAngleDivergence<t::milliradian>()
                      .value() == Approx(33.86818));
          }
          THEN("the 1/e^2 beam diameter at 18 m is 270.94538 mm")
          {
            CHECK(beam.getOneOverE2Diameter<t::millimeter>(18 * m).value() ==
                  Approx(270.94538));
          }
          THEN("the 1/e^2 beam radius at 18 m is 270.94538/2 mm")
          {
            CHECK(beam.getOneOverE2Radius<t::millimeter>(18 * m).value() ==
                  Approx(270.94538 / 2));
          }
          THEN("the 1/e beam radius at 18 m is 270.94538/2/sqrt(2) mm")
          {
            CHECK(beam.getOneOverERadius<t::millimeter>(18 * m).value() ==
                  Approx(270.94538 / 2 / sqrt(2)));
          }
          THEN("the 1/e^2 beam area at 18 m is 576.571779025 cm^2")
          {
            CHECK(
                beam.getOneOverE2Area<t::centimeter_squared>(18 * m).value() ==
                Approx(576.571779025));
          }
          THEN("the 1/e beam area at 18 m is 576.571779025/2 cm^2")
          {
            CHECK(beam.getOneOverEArea<t::centimeter_squared>(18 * m).value() ==
                  Approx(576.571779025 / 2));
          }
        }
      }
    }

    WHEN("waist diameter is set to 2 cm")
    {
      beam.setOneOverE2WaistDiameter(2 * cm);
      THEN("1/e waist diameter is sqrt(2) cm")
      {
        CHECK(beam.getOneOverEWaistDiameter().value() == Approx(sqrt(2)));
      }
      THEN("1/e2 waist diameter is 2 cm")
      {
        CHECK(beam.getOneOverE2WaistDiameter().value() == Approx(2));
      }
      THEN("1/e waist radius is 1/sqrt(2) cm")
      {
        CHECK(beam.getOneOverEWaistRadius().value() == Approx(1 / sqrt(2)));
      }
      THEN("1/e2 waist radius is 1 cm")
      {
        CHECK(beam.getOneOverE2WaistRadius().value() == Approx(1));
      }
    }
  }

  GIVEN("a configured GaussianBeam instance")
  {
    GaussianBeam beam;

    beam.setWavelength(532 * nm);
    beam.setWaistPosition(5 * cm);
    beam.setOneOverE2WaistDiameter(10 * um);
    beam.setWaistPhase(0 * rad);
    beam.setPower(1 * W);

    WHEN("the current position is set to 10 cm")
    {
      auto z = 10 * cm;
      beam.setCurrentPosition(z);
      THEN(
          "the z-dependent getters agree when called with and without an "
          "argument.")
      {
#define CHECK_SAME(name) \
  CHECK(beam.get##name(z).value() == Approx(beam.get##name().value()))

        CHECK_SAME(OneOverE2Radius);
        CHECK_SAME(OneOverE2Diameter);
        CHECK_SAME(RadiusOfCurvature);
        CHECK_SAME(RelativeWaistPosition);
        CHECK_SAME(OneOverE2Area);
        CHECK_SAME(PeakIrradiance);
        CHECK_SAME(GouyPhase);

#undef CHECK_SAME
      }
    }
  }

  SECTION("Getter/Setter Overloads")
  {
    GaussianBeam beam;
    beam.setWavelength(500. * i::nm);
    beam.setOneOverE2WaistRadius(2. * i::cm);
    beam.setWaistPosition(0. * i::m);
    beam.setCurrentPosition(200. * i::m);

    CHECK(beam.getOneOverE2WaistRadius().value() == Approx(2));
    CHECK(beam.getOneOverE2WaistRadius<t::mm>().value() == Approx(20));
    CHECK(beam.getOneOverE2WaistRadius<t::cm>().value() == Approx(2));

    CHECK(beam.getOneOverE2WaistDiameter().value() == Approx(4));
    CHECK(beam.getOneOverE2WaistDiameter<t::mm>().value() == Approx(40));
    CHECK(beam.getOneOverE2WaistDiameter<t::cm>().value() == Approx(4));

    CHECK(beam.getOneOverEWaistRadius().value() == Approx(2 / sqrt(2)));
    CHECK(beam.getOneOverEWaistRadius<t::mm>().value() == Approx(20 / sqrt(2)));
    CHECK(beam.getOneOverEWaistRadius<t::cm>().value() == Approx(2 / sqrt(2)));

    CHECK(beam.getOneOverEWaistDiameter().value() == Approx(4 / sqrt(2)));
    CHECK(beam.getOneOverEWaistDiameter<t::mm>().value() ==
          Approx(40 / sqrt(2)));
    CHECK(beam.getOneOverEWaistDiameter<t::cm>().value() ==
          Approx(4 / sqrt(2)));

    double phi = (500e-9 / M_PI / 2e-2);
    CHECK(beam.getOneOverE2HalfAngleDivergence().value() == Approx(phi * 1000));
    CHECK(beam.getOneOverE2HalfAngleDivergence<t::mrad>().value() ==
          Approx(phi * 1000));
    CHECK(beam.getOneOverE2HalfAngleDivergence<t::rad>().value() ==
          Approx(phi));

    CHECK(beam.getRayleighRange().value() ==
          Approx((M_PI * 2e-2 * 2e-2 / 500e-9) * 100));
    CHECK(beam.getRayleighRange<t::cm>().value() ==
          Approx((M_PI * 2e-2 * 2e-2 / 500e-9) * 100));
    CHECK(beam.getRayleighRange<t::m>().value() ==
          Approx((M_PI * 2e-2 * 2e-2 / 500e-9)));

    CHECK(beam.getOneOverE2Radius(200. * i::m).value() ==
          Approx(sqrt(2 * 2 + 20000 * phi * 20000 * phi)));
    CHECK(beam.getOneOverE2Radius<t::cm>(200. * i::m).value() ==
          Approx(sqrt(2 * 2 + 20000 * phi * 20000 * phi)));
    CHECK(beam.getOneOverE2Radius<t::m>(200. * i::m).value() ==
          Approx(sqrt(0.02 * 0.02 + 200 * phi * 200 * phi)));
    CHECK(beam.getOneOverE2Radius().value() ==
          Approx(sqrt(2 * 2 + 20000 * phi * 20000 * phi)));
    CHECK(beam.getOneOverE2Radius<t::cm>().value() ==
          Approx(sqrt(2 * 2 + 20000 * phi * 20000 * phi)));
    CHECK(beam.getOneOverE2Radius<t::m>().value() ==
          Approx(sqrt(0.02 * 0.02 + 200 * phi * 200 * phi)));

    CHECK(beam.getRelativeWaistPosition(10 * i::cm).value() == Approx(-10));
    CHECK(beam.getRelativeWaistPosition(10 * i::m).value() == Approx(-1000));
    CHECK(beam.getRelativeWaistPosition<t::cm>(10 * i::cm).value() ==
          Approx(-10));
    CHECK(beam.getRelativeWaistPosition<t::cm>(10 * i::m).value() ==
          Approx(-1000));
    CHECK(beam.getRelativeWaistPosition<t::m>(10 * i::cm).value() ==
          Approx(-0.10));
    CHECK(beam.getRelativeWaistPosition<t::m>(10 * i::m).value() ==
          Approx(-10));
  }

  SECTION("1/e <-> 1/e2 Conversions")
  {
    GaussianBeam beam;
    beam.setWavelength(500 * i::nm);

    beam.setOneOverE2WaistRadius(10. * i::cm);
    CHECK(beam.getOneOverE2WaistRadius().value() == Approx(10));
    CHECK(beam.getOneOverE2WaistDiameter().value() == Approx(20));
    CHECK(beam.getOneOverEWaistRadius().value() == Approx(10 / sqrt(2)));
    CHECK(beam.getOneOverEWaistDiameter().value() == Approx(20 / sqrt(2)));

    beam.setOneOverE2WaistDiameter(10. * i::cm);
    CHECK(beam.getOneOverE2WaistRadius().value() == Approx(5));
    CHECK(beam.getOneOverE2WaistDiameter().value() == Approx(10));
    CHECK(beam.getOneOverEWaistRadius().value() == Approx(5 / sqrt(2)));
    CHECK(beam.getOneOverEWaistDiameter().value() == Approx(10 / sqrt(2)));

    beam.setOneOverEWaistRadius(10. * i::cm);
    CHECK(beam.getOneOverE2WaistRadius().value() == Approx(10 * sqrt(2)));
    CHECK(beam.getOneOverE2WaistDiameter().value() == Approx(20 * sqrt(2)));
    CHECK(beam.getOneOverEWaistRadius().value() == Approx(10));
    CHECK(beam.getOneOverEWaistDiameter().value() == Approx(20));

    beam.setOneOverEWaistDiameter(10. * i::cm);
    CHECK(beam.getOneOverE2WaistRadius().value() == Approx(5 * sqrt(2)));
    CHECK(beam.getOneOverE2WaistDiameter().value() == Approx(10 * sqrt(2)));
    CHECK(beam.getOneOverEWaistRadius().value() == Approx(5));
    CHECK(beam.getOneOverEWaistDiameter().value() == Approx(10));

    double phi = beam.getOneOverE2HalfAngleDivergence().value();
    CHECK(beam.getOneOverE2HalfAngleDivergence().value() == Approx(phi));
    CHECK(beam.getOneOverE2FullAngleDivergence().value() == Approx(2 * phi));
    CHECK(beam.getOneOverEHalfAngleDivergence().value() ==
          Approx(phi / sqrt(2)));
    CHECK(beam.getOneOverEFullAngleDivergence().value() ==
          Approx(sqrt(2) * phi));

    beam.setOneOverE2HalfAngleDivergence(0.02 * i::rad);
    CHECK(beam.getOneOverE2HalfAngleDivergence().value() == Approx(20));
    CHECK(beam.getOneOverE2HalfAngleDivergence<t::rad>().value() ==
          Approx(0.02));
    CHECK(beam.getOneOverEHalfAngleDivergence().value() ==
          Approx(20 / sqrt(2)));
    CHECK(beam.getOneOverEHalfAngleDivergence<t::rad>().value() ==
          Approx(20. / sqrt(2) / 1000));
    CHECK(beam.getOneOverE2FullAngleDivergence().value() == Approx(2 * 20));
    CHECK(beam.getOneOverE2FullAngleDivergence<t::rad>().value() ==
          Approx(2 * 20. / 1000));
    CHECK(beam.getOneOverEFullAngleDivergence().value() ==
          Approx(2 * 20 / sqrt(2)));
    CHECK(beam.getOneOverEFullAngleDivergence<t::rad>().value() ==
          Approx(2 * 20. / sqrt(2) / 1000));

    beam.setOneOverEHalfAngleDivergence(0.1 * i::rad);
    CHECK(beam.getOneOverE2HalfAngleDivergence().value() ==
          Approx(100 * sqrt(2)));
    CHECK(beam.getOneOverE2HalfAngleDivergence<t::rad>().value() ==
          Approx(0.1 * sqrt(2)));
    CHECK(beam.getOneOverEHalfAngleDivergence().value() == Approx(100));
    CHECK(beam.getOneOverEHalfAngleDivergence<t::rad>().value() == Approx(0.1));
    CHECK(beam.getOneOverE2FullAngleDivergence().value() ==
          Approx(2 * 100. * sqrt(2)));
    CHECK(beam.getOneOverE2FullAngleDivergence<t::rad>().value() ==
          Approx(2 * 100. * sqrt(2) / 1000));
    CHECK(beam.getOneOverEFullAngleDivergence().value() == Approx(2 * 100.));
    CHECK(beam.getOneOverEFullAngleDivergence<t::rad>().value() ==
          Approx(2 * 100. / 1000));

    beam.setOneOverE2FullAngleDivergence(0.1 * i::rad);
    CHECK(beam.getOneOverE2HalfAngleDivergence().value() == Approx(50));
    CHECK(beam.getOneOverE2HalfAngleDivergence<t::rad>().value() ==
          Approx(0.05));
    CHECK(beam.getOneOverEHalfAngleDivergence().value() ==
          Approx(50 / sqrt(2)));
    CHECK(beam.getOneOverEHalfAngleDivergence<t::rad>().value() ==
          Approx(0.05 / sqrt(2)));
    CHECK(beam.getOneOverE2FullAngleDivergence().value() == Approx(100));
    CHECK(beam.getOneOverE2FullAngleDivergence<t::rad>().value() ==
          Approx(100. / 1000));
    CHECK(beam.getOneOverEFullAngleDivergence().value() ==
          Approx(100. / sqrt(2)));
    CHECK(beam.getOneOverEFullAngleDivergence<t::rad>().value() ==
          Approx(100. / sqrt(2) / 1000));

    beam.setOneOverEFullAngleDivergence(0.05 * i::rad);
    CHECK(beam.getOneOverE2HalfAngleDivergence().value() ==
          Approx(sqrt(2) * 50 / 2));
    CHECK(beam.getOneOverE2HalfAngleDivergence<t::rad>().value() ==
          Approx(sqrt(2) * 0.05 / 2));
    CHECK(beam.getOneOverEHalfAngleDivergence().value() == Approx(50 / 2));
    CHECK(beam.getOneOverEHalfAngleDivergence<t::rad>().value() ==
          Approx(0.05 / 2));
    CHECK(beam.getOneOverE2FullAngleDivergence().value() ==
          Approx(sqrt(2) * 50));
    CHECK(beam.getOneOverE2FullAngleDivergence<t::rad>().value() ==
          Approx(sqrt(2) * 50. / 1000));
    CHECK(beam.getOneOverEFullAngleDivergence().value() == Approx(50));
    CHECK(beam.getOneOverEFullAngleDivergence<t::rad>().value() ==
          Approx(50. / 1000));
  }

  SECTION("Divergence Setting")
  {
    GaussianBeam beam;

    beam.setWavelength(532 * i::nm);
    beam.setOneOverE2WaistRadius(2 * i::mm);

    double phi = 532e-9 / M_PI / 2e-3;

    CHECK(beam.getDiffractionLimitedDivergence());
    CHECK(beam.getOneOverE2HalfAngleDiffractionLimitedDivergence<t::rad>()
              .value() == Approx(phi));
    CHECK(beam.getOneOverE2HalfAngleDivergence<t::rad>().value() ==
          Approx(phi));
    CHECK(beam.getBeamQualityFactor().value() == Approx(1));

    beam.setOneOverE2FullAngleDivergence(1. * i::mrad);
    CHECK(!beam.getDiffractionLimitedDivergence());
    CHECK(beam.getOneOverE2HalfAngleDiffractionLimitedDivergence<t::rad>()
              .value() == Approx(phi));
    CHECK(beam.getOneOverE2HalfAngleDivergence().value() == Approx(0.5));
    CHECK(beam.getBeamQualityFactor().value() > 1);
    CHECK(beam.getBeamQualityFactor().value() == Approx(0.0005 / phi));

    beam.setDiffractionLimitedDivergence(true);
    CHECK(beam.getOneOverE2HalfAngleDiffractionLimitedDivergence<t::rad>()
              .value() == Approx(phi));
    CHECK(beam.getOneOverE2HalfAngleDivergence<t::rad>().value() ==
          Approx(phi));
    CHECK(beam.getBeamQualityFactor().value() == Approx(1));

    beam.setDiffractionLimitedDivergence(false);
    CHECK(beam.getOneOverE2HalfAngleDiffractionLimitedDivergence<t::rad>()
              .value() == Approx(phi));
    CHECK(beam.getOneOverE2HalfAngleDivergence<t::rad>().value() ==
          Approx(0.0005));
    CHECK(beam.getBeamQualityFactor().value() == Approx(0.0005 / phi));

    beam.setOneOverE2FullAngleDivergence(
        phi * i::rad);  // divergence set to a value that is lower than
                        // physically possible
    CHECK(beam.getBeamQualityFactor().value() < 1);

    beam.setOneOverEFullAngleDivergence(0 * i::mrad);
    beam.setOneOverEWaistDiameter(2 * i::mm);

    CHECK(beam.getOneOverE2HalfAngleDivergence().value() == Approx(0));
    CHECK(beam.getOneOverEDiameter(0 * i::m).value() == Approx(0.2));
    CHECK(beam.getOneOverEDiameter(10 * i::m).value() == Approx(0.2));
    CHECK(beam.getOneOverEDiameter(100 * i::m).value() == Approx(0.2));
    CHECK(beam.getOneOverEDiameter(1000 * i::m).value() == Approx(0.2));
    CHECK(beam.getBeamQualityFactor().value() == Approx(0));

    beam.setOneOverEFullAngleDivergence(100 * i::mrad);
    CHECK(beam.getOneOverEDiameter<t::cm>(0 * i::m).value() == Approx(0.2));
    CHECK(beam.getOneOverEDiameter<t::cm>(10 * i::m).value() ==
          Approx(0.2 + 1000 * 0.1).epsilon(0.01));
    CHECK(beam.getOneOverEDiameter<t::cm>(100 * i::m).value() ==
          Approx(0.2 + 10000 * 0.1).epsilon(0.01));
    CHECK(beam.getOneOverEDiameter<t::cm>(1000 * i::m).value() ==
          Approx(0.2 + 100000 * 0.1).epsilon(0.01));
    CHECK(beam.getBeamQualityFactor().value() > 1);
  }
}

#include "GaussianBeam.hpp"
SCENARIO("Complex Beam Parameter Calculations", "[GaussianBeam]")
{
  GaussianBeam beam;
  WHEN("the beam is configured")
  {
    beam.setWavelength(532 * t::nanometer());
    beam.setOneOverE2WaistDiameter(10 * t::micrometer());
    beam.setWaistPosition(1.2 * t::meter());

    THEN("the real part of 1 / q @ z is equal to the radius of curvature @ z")
    {
      auto q = beam.getComplexBeamParameter<t::centimeter>(8 * t::meter());
      std::cout << "q: " << q << std::endl;
      auto one_over_q = complex<double>(1., 0.) / q;

      auto R = beam.getRadiusOfCurvature<t::centimeter>(8 * t::meter());
      auto one_over_R = 1. / R;

      auto w = beam.getOneOverE2Diameter<t::centimeter>(8 * t::meter()) / 2.;

      auto tmp = beam.getWavelength<t::centimeter>() / (M_PI * pow<2>(w));

      CHECK(one_over_q.value().real() == Approx(one_over_R.value()));
      CHECK(one_over_q.value().imag() == Approx(tmp.value()));
    }
  }
}

#include "OpticalElements/ThinLens.hpp"
SCENARIO("Thin lens class calculations", "[OpticalElements]")
{
  WHEN("lens is created")
  {
    ThinLens<t::centimeter> lens;

    AND_WHEN("focal length is set to 10 cm")
    {
      lens.setFocalLength(10 * t::centimeter());
      auto mat = lens.getRTMatrix();

      THEN("element (0,0) of the RT Matrix is 1")
      {
        CHECK(mat(0, 0) == Approx(1));
      }
      AND_THEN("element (0,1) of the RT Matrix is 0")
      {
        CHECK(mat(0, 1) == Approx(0));
      }
      AND_THEN("element (1,0) of the RT Matrix is 1/10")
      {
        CHECK(mat(1, 0) == Approx(-1. / 10));
      }
      AND_THEN("element (1,1) of the RT Matrix is 1")
      {
        CHECK(mat(1, 1) == Approx(1));
      }
    }

    AND_WHEN("focal length is set to 1 m")
    {
      lens.setFocalLength(1 * t::meter());
      auto mat = lens.getRTMatrix();

      THEN("element (0,0) of the RT Matrix is 1")
      {
        CHECK(mat(0, 0) == Approx(1));
      }
      AND_THEN("element (0,1) of the RT Matrix is 0")
      {
        CHECK(mat(0, 1) == Approx(0));
      }
      AND_THEN("element (1,0) of the RT Matrix is 1/100")
      {
        CHECK(mat(1, 0) == Approx(-1. / 100));
      }
      AND_THEN("element (1,1) of the RT Matrix is 1")
      {
        CHECK(mat(1, 1) == Approx(1));
      }
    }
  }
}

#include "OpticalElements/FlatInterface.hpp"
SCENARIO("Flat interface class calculations", "[OpticalElements]")
{
  WHEN("interface is created")
  {
    FlatInterface<t::centimeter> interface;

    AND_WHEN("the initial refractive index is set to 1")
    {
      interface.setInitialRefractiveIndex(1);

      AND_WHEN("the final refractive index is set to 2")
      {
        interface.setFinalRefractiveIndex(2);

        auto mat = interface.getRTMatrix();

        THEN("element (0,0) of the RT Matrix is 1")
        {
          CHECK(mat(0, 0) == Approx(1));
        }
        AND_THEN("element (0,1) of the RT Matrix is 0")
        {
          CHECK(mat(0, 1) == Approx(0));
        }
        AND_THEN("element (1,0) of the RT Matrix is 0")
        {
          CHECK(mat(1, 0) == Approx(0));
        }
        AND_THEN("element (1,1) of the RT Matrix is 1/2")
        {
          CHECK(mat(1, 1) == Approx(1. / 2));
        }
      }
    }

    AND_WHEN("the initial refractive index is set to 2")
    {
      interface.setInitialRefractiveIndex(2);

      AND_WHEN("the final refractive index is set to 1")
      {
        interface.setFinalRefractiveIndex(1);

        auto mat = interface.getRTMatrix();

        THEN("element (0,0) of the RT Matrix is 1")
        {
          CHECK(mat(0, 0) == Approx(1));
        }
        AND_THEN("element (0,1) of the RT Matrix is 0")
        {
          CHECK(mat(0, 1) == Approx(0));
        }
        AND_THEN("element (1,0) of the RT Matrix is 0")
        {
          CHECK(mat(1, 0) == Approx(0));
        }
        AND_THEN("element (1,1) of the RT Matrix is 2")
        {
          CHECK(mat(1, 1) == Approx(2));
        }
      }
    }
  }
}

#include "OpticalElements/SphericalInterface.hpp"
SCENARIO("Spherical interface class calculations", "[OpticalElements]")
{
  WHEN("interface is created")
  {
    SphericalInterface<t::centimeter> interface;

    AND_WHEN("the initial refractive index is set to 1")
    {
      interface.setInitialRefractiveIndex(1);

      AND_WHEN("the final refractive index is set to 2")
      {
        interface.setFinalRefractiveIndex(2);

        AND_WHEN("the radius of curvature is set to +10 cm")
        {
          interface.setRadiusOfCurvature(10 * t::centimeter());

          auto mat = interface.getRTMatrix();

          THEN("element (0,0) of the RT Matrix is 1")
          {
            CHECK(mat(0, 0) == Approx(1));
          }
          AND_THEN("element (0,1) of the RT Matrix is 0")
          {
            CHECK(mat(0, 1) == Approx(0));
          }
          AND_THEN("element (1,0) of the RT Matrix is -(2-1)/(10*2)")
          {
            CHECK(mat(1, 0) == Approx(-1. / 20));
          }
          AND_THEN("element (1,1) of the RT Matrix is 1/2")
          {
            CHECK(mat(1, 1) == Approx(1. / 2));
          }
        }

        AND_WHEN("the radius of curvature is set to -35 mm")
        {
          interface.setRadiusOfCurvature(-35 * t::millimeter());

          auto mat = interface.getRTMatrix();

          THEN("element (0,0) of the RT Matrix is 1")
          {
            CHECK(mat(0, 0) == Approx(1));
          }
          AND_THEN("element (0,1) of the RT Matrix is 0")
          {
            CHECK(mat(0, 1) == Approx(0));
          }
          AND_THEN("element (1,0) of the RT Matrix is +(2-1)/(3.5*2)")
          {
            CHECK(mat(1, 0) == Approx(+1. / 7));
          }
          AND_THEN("element (1,1) of the RT Matrix is 1/2")
          {
            CHECK(mat(1, 1) == Approx(1. / 2));
          }
        }
      }
    }
  }
}

#include "OpticalElements/Filter.hpp"
SCENARIO(
    "Filter class calculations"
    "[OpticalElements]")
{
  GIVEN("a filter")
  {
    Filter filter;

    WHEN("OD is 1")
    {
      filter.setOpticalDensity(1);
      THEN("power loss is 0.9") { CHECK(filter.getPowerLoss() == Approx(0.9)); }
    }
    WHEN("OD is 2")
    {
      filter.setOpticalDensity(2);
      THEN("power loss is 0.99")
      {
        CHECK(filter.getPowerLoss() == Approx(0.99));
      }
    }
  }
}

#include "Media/LinearAbsorber.hpp"
SCENARIO(
    "Linear absorber calculations"
    "[OpticalElements]")
{
  GIVEN("a linear absorber")
  {
    LinearAbsorber<t::cm> absorber;

    WHEN("mu_a is 1")
    {
      absorber.setAbsorptionCoefficient(1 / cm);
      THEN("transmission between z = 1 cm and z = 2 cm is e^{-1}")
      {
        CHECK(absorber.getTransmission(1 * cm, 2 * cm) == Approx(exp(-1)));
      }
      THEN("transmission between z = 1 m and z = 2 m is e^{-100}")
      {
        CHECK(absorber.getTransmission(1 * m, 2 * m) == Approx(exp(-100)));
      }
      THEN("transmission between z = 2 cm and z = 1 cm is e^{1}")
      {
        CHECK(absorber.getTransmission(2 * cm, 1 * cm) == Approx(exp(1)));
      }
      THEN("transmission between z = 2 m and z = 1 m is e^{100}")
      {
        CHECK(absorber.getTransmission(2 * m, 1 * m) == Approx(exp(100)));
      }
    }
  }
}

#include "Builders/BeamBuilder.hpp"
#include "GaussianBeam.hpp"
#include "OpticalElements/SphericalInterface.hpp"
#include "OpticalElements/ThinLens.hpp"
TEST_CASE("Gaussian Beam Transformations", "[OpticalElements,GuassianBeam]")
{
  GaussianBeam beam;
  BeamBuilder  config;

  SECTION("Thin lens examples")
  {
    beam.setWavelength(532 * nm);
    beam.setOneOverE2WaistDiameter(0.01 * mm);
    beam.setWaistPosition(0 * cm);

    SECTION("a 10 mm lens")
    {
      ThinLens<t::centimeter> lens;
      lens.setFocalLength(10 * mm);

      CHECK(beam.getWavelength().value() == Approx(532));
      CHECK(beam.getWaistPosition().value() == Approx(0));
      CHECK(beam.getOneOverE2WaistDiameter().value() == Approx(10e-4));
      CHECK(beam.getOneOverE2FullAngleDivergence().value() ==
            Approx(2 * 33.868));

      auto d = beam.getOneOverE2Diameter(100 * mm);

      beam.transform(&lens, 100 * mm);

      CHECK(beam.getOneOverE2Diameter(100 * mm).value() == Approx(d.value()));

      CHECK(beam.getWavelength().value() == Approx(532));
      CHECK(beam.getRelativeWaistPosition<t::millimeter>(100 * mm).value() ==
            Approx(11.111));
      CHECK(beam.getWaistPosition<t::millimeter>().value() ==
            Approx(100 + 11.111));
      CHECK(beam.getOneOverE2WaistDiameter<t::millimeter>().value() ==
            Approx(0.00111));
      CHECK(beam.getRayleighRange<t::millimeter>().value() == Approx(0.001823));
    }
  }

  SECTION("Spherical surface examples")
  {
    config.setWavelength(532 * nm);
    config.setPower(10 * mW);
    config.setPosition(0 * cm)
        .setDiameter(2 * 0.5 * mm)
        .setDivergence(2 * 0.75 * mrad);
    config.configure(beam);

    CHECK(beam.getWavelength<t::micrometer>().value() == Approx(0.532));
    CHECK(beam.getPower<t::watt>().value() == Approx(0.01));

    CHECK(beam.getOneOverE2Radius<t::millimeter>(0 * cm).value() ==
          Approx(0.5));
    CHECK(beam.getOneOverE2Radius<t::millimeter>(10 * cm).value() ==
          Approx(0.56793));

    CHECK(beam.getWaistPosition<t::millimeter>().value() == Approx(-594.822));
    CHECK(beam.getOneOverE2WaistRadius<t::millimeter>().value() ==
          Approx(0.225788));
    CHECK(beam.getRayleighRange<t::millimeter>().value() == Approx(301.05042));

    SECTION("a 10 mm surface")
    {
      SphericalInterface<t::centimeter> surface;
      surface.setRadiusOfCurvature(10 * mm);

      SECTION("a 1 <--> 1 refractive index")
      {
        surface.setInitialRefractiveIndex(1);
        surface.setFinalRefractiveIndex(1);

        beam.transform(&surface, 10 * cm);

        CHECK(beam.getWavelength<t::micrometer>().value() == Approx(0.532));
        CHECK(beam.getPower<t::watt>().value() == Approx(0.01));

        CHECK(beam.getOneOverE2Radius<t::millimeter>(0 * cm).value() ==
              Approx(0.5));
        CHECK(beam.getOneOverE2Radius<t::millimeter>(10 * cm).value() ==
              Approx(0.56793));

        CHECK(beam.getWaistPosition<t::millimeter>().value() ==
              Approx(-594.822));
        CHECK(beam.getOneOverE2WaistDiameter<t::millimeter>().value() / 2 ==
              Approx(0.225788));
      }

      SECTION("a 1 <--> 2 refractive index")
      {
        surface.setInitialRefractiveIndex(1);
        surface.setFinalRefractiveIndex(2);

        beam.transform(&surface, 100 * mm);

        CHECK(beam.getWaistPosition<t::millimeter>().value() ==
              Approx(100 + 20.2447486566));
        CHECK(beam.getRayleighRange<t::millimeter>().value() ==
              Approx(0.10759247802));

        CHECK(beam.getOneOverE2WaistRadius<t::millimeter>().value() ==
              Approx(0.00301826));
        CHECK(beam.getOneOverE2Radius<t::millimeter>(10 * cm).value() ==
              Approx(0.56793));  // beam diameter at position of the surface
                                 // should not change

        CHECK(beam.getWavelength<t::micrometer>().value() == Approx(0.532 / 2));
        CHECK(beam.getPower<t::watt>().value() == Approx(0.01));
      }
    }
  }

  SECTION("Ramrod example worksheet")
  {
    SECTION("Problem 1")
    {
      beam.setWavelength(444 * nm);
      beam.setOneOverE2WaistDiameter(2.5 * mm);
      beam.setWaistPosition(0 * cm);
      beam.setPower(800 * mW);

      ThinLens<t::centimeter> lens;
      lens.setFocalLength(120 * mm);

      quantity<t::centimeter> lensPosition = 15 * cm;

      WRITE2("waist diameter ", beam.getOneOverE2WaistDiameter());
      WRITE3("1.a ", beam.getPeakIrradiance(lensPosition).value(), " W/cm^2");
      WRITE2("1.b ", beam.getOneOverE2Diameter(lensPosition));
      WRITE2("1.c ", beam.getRadiusOfCurvature(lensPosition));
      beam.transform(&lens, 15 * cm);
      WRITE2("1.d ", beam.getRadiusOfCurvature(lensPosition));
      WRITE2("1.e ", beam.getWaistPosition());
      auto irr =
          beam.getPower() / ((M_PI / 4) * beam.getOneOverE2WaistDiameter() *
                             beam.getOneOverE2WaistDiameter());
      WRITE2("waist diameter ", beam.getOneOverE2WaistDiameter());
      WRITE2("1.f ", irr.value());
      WRITE3("1.f ", beam.getPeakIrradiance(beam.getWaistPosition()).value(),
             "W/cm^2");
    }

    SECTION("Problem 2")
    {
      beam.setPower(5 * mW);
      BeamBuilder config;

      config.setWavelength(532 * nm);
      config.setPosition(0 * cm).setDiameter(3 * mm).setDivergence(2 * mrad);

      config.configure(beam);

      SphericalInterface<t::centimeter> cornea;
      cornea.setRadiusOfCurvature(6.1 * mm);
      cornea.setInitialRefractiveIndex(1.0);
      cornea.setFinalRefractiveIndex(1.3369);

      std::cout << std::endl;
      WRITE2("2.a", beam.getWaistPosition());
      WRITE2("2.b", beam.getRadiusOfCurvature(0 * cm));

      beam.transform(&cornea, 0 * cm);

      WRITE2("--wavelength", beam.getWavelength());
      WRITE2("--RoC", beam.getRadiusOfCurvature(0 * cm));
      WRITE2("--Diam (at cornea)", beam.getOneOverE2Diameter(0 * cm));
      WRITE2("--z_0", beam.getWaistPosition());
      WRITE2("--w_0", beam.getOneOverE2WaistDiameter());
      WRITE2("2.c", beam.getOneOverE2Diameter(2.44 * cm));
      WRITE2("2.d", beam.getOneOverE2Diameter(2.44 * cm));
      WRITE2("2.e", beam.getRadiusOfCurvature(2.44 * cm));
      WRITE3("2.f", beam.getPeakIrradiance(2.44 * cm).value(), "W/cm^2");
    }

    SECTION("Problem 2 Details")
    {
      beam.setPower(5 * mW);
      BeamBuilder config;

      config.setWavelength(532 * nm);
      config.setPosition(0 * cm).setDiameter(3 * mm).setDivergence(2 * mrad);
      // config.setPosition(0*cm).setDiameter(3*mm).setDivergence(4*mrad); //
      // students assumed half-angle divergence.

      config.configure(beam);

      SphericalInterface<t::centimeter> cornea;
      cornea.setRadiusOfCurvature(6.1 * mm);
      cornea.setInitialRefractiveIndex(1.0);
      cornea.setFinalRefractiveIndex(1.3369);

      WRITE2("START", "--");
      WRITE2("L", beam.getWavelength());
      WRITE2("w(z)", beam.getOneOverE2Radius(0 * cm));
      WRITE2("w0", beam.getOneOverE2WaistRadius());
      WRITE2("R", beam.getRadiusOfCurvature(0 * cm));
      WRITE2("z", beam.getRelativeWaistPosition(0 * cm));
      WRITE2("zR", beam.getRayleighRange());
      WRITE2("q", beam.getComplexBeamParameter(0 * cm));
      WRITE2("--", "--");

      auto RTM = cornea.getRTMatrix();

      WRITE2("n1", cornea.getInitialRefractiveIndex());
      WRITE2("n2", cornea.getFinalRefractiveIndex());
      WRITE2("f", -1 / RTM(1, 0));
      WRITE2("A", RTM(0, 0));
      WRITE2("B", RTM(0, 1));
      WRITE2("C", RTM(1, 0));
      WRITE2("D", RTM(1, 1));
      WRITE2("--", "--");

      beam.transform(&cornea, 0 * cm);

      WRITE2("Lp", beam.getWavelength());
      WRITE2("w(z)p", beam.getOneOverE2Radius(0 * cm));
      WRITE2("w0p", beam.getOneOverE2WaistRadius());
      WRITE2("Rp", beam.getRadiusOfCurvature(0 * cm));
      WRITE2("zp", beam.getRelativeWaistPosition(0 * cm));
      WRITE2("zRp", beam.getRayleighRange());
      WRITE2("qp", beam.getComplexBeamParameter(0 * cm));

      WRITE2("d (distance to beam waist from retina)",
             beam.getRelativeWaistPosition(2.44 * cm));

      WRITE2("END", "--");
    }
  }
}

#include "Builders/BeamBuilder.hpp"
#include "GaussianBeam.hpp"
TEST_CASE("BeamBuilder Tests", "[Builders,GuassianBeam]")
{
  BeamBuilder config;

  SECTION("Internal Units")
  {
    config.setWavelength(532 * nm).setDivergence(2.5 * mrad);

    CHECK(config.getWavelength<t::nanometer>().value().value() == Approx(532));
    CHECK(config.getDivergence<t::milliradian>().value().value() ==
          Approx(2.5));
  }

  SECTION("Unit Conversions")
  {
    config.setWavelength(0.532 * um).setDivergence(0.0025 * rad);

    CHECK(config.getWavelength<t::nanometer>().value().value() == Approx(532));
    CHECK(config.getDivergence<t::milliradian>().value().value() ==
          Approx(2.5));

    config.setWavelength(0.444 * um).setDivergence(0.001 * rad);

    CHECK(config.getWavelength<t::nanometer>().value().value() == Approx(444));
    CHECK(config.getDivergence<t::milliradian>().value().value() == Approx(1));
  }

  SECTION("Arrays")
  {
    config.setWavelength(0.532 * um);
    config.setPosition(1.0 * cm).setDiameter(2.0 * mm);
    config.setPosition(10. * cm).setDiameter(4.0 * mm);

    CHECK(config.Wavelength.size() == 1);
    CHECK(config.Position.size() == 2);
    CHECK(config.Diameter.size() == 2);

    CHECK(config.getWavelength<t::nanometer>().value().value() == Approx(532));
    CHECK(config.getPosition<t::centimeter>().value().value() == Approx(1));
    CHECK(config.getPosition<t::centimeter>(0).value().value() == Approx(1));
    CHECK(config.getPosition<t::centimeter>(1).value().value() == Approx(10));
    CHECK(config.getDiameter<t::centimeter>().value().value() == Approx(0.2));
    CHECK(config.getDiameter<t::centimeter>(0).value().value() == Approx(0.2));
    CHECK(config.getDiameter<t::centimeter>(1).value().value() == Approx(0.4));
  }

  SECTION("Beam configuration")
  {
    GaussianBeam beam;

    config.setWavelength(532 * nm);
    config.setDivergence(10 * mrad);
    config.setDiameter(2.5000 * mm).setPosition(0.0 * meter);

    config.configure(beam);

    CHECK(beam.getWavelength().value() == Approx(532));
    CHECK(beam.getOneOverE2WaistDiameter<t::millimeter>().value() ==
          Approx(2 * 0.033868));
    CHECK(beam.getRayleighRange<t::millimeter>().value() == Approx(6.77357));
    CHECK(beam.getRadiusOfCurvature<t::millimeter>(0 * mm).value() ==
          Approx(250.09));
    CHECK(beam.getWaistPosition<t::millimeter>().value() == Approx(-249.908));
  }

  SECTION("Beam configuration, offset position")
  {
    GaussianBeam beam;

    config.setWavelength(532 * nm);
    config.setDivergence(10 * mrad);
    config.setDiameter(2.5000 * mm).setPosition(1.2 * meter);

    config.configure(beam);

    CHECK(beam.getWavelength().value() == Approx(532));
    CHECK(beam.getOneOverE2WaistDiameter<t::millimeter>().value() ==
          Approx(2 * 0.033868));
    CHECK(beam.getRayleighRange<t::millimeter>().value() == Approx(6.77357));
    CHECK(beam.getRadiusOfCurvature<t::millimeter>(1200 * mm).value() ==
          Approx(250.09));
    CHECK(beam.getWaistPosition<t::millimeter>().value() ==
          Approx(1200 - 249.908));
  }
}

#include "Builders/OpticalElementBuilder.hpp"
TEST_CASE("OpticalElementBuilder tests", "[Builders,OpticalElements]")
{
  OpticalElementBuilder<t::centimeter> OEBuilder;
  OpticalElement_ptr<t::centimeter>    elem;

  SECTION("Thin lens tests")
  {
    SECTION("External configuration")
    {
      for (auto k :
           std::vector<std::string>({"thinlens", "thin lens", "thin_lens"})) {
        elem.reset(OEBuilder.create(k));
        REQUIRE(elem != nullptr);

        ThinLens<t::centimeter>& lens =
            *std::dynamic_pointer_cast<ThinLens<t::centimeter>>(elem);

        lens.setFocalLength(10 * t::centimeter());
        auto mat = lens.getRTMatrix();

        CHECK(mat(0, 0) == Approx(1));
        CHECK(mat(0, 1) == Approx(0));
        CHECK(mat(1, 0) == Approx(-1. / 10));
        CHECK(mat(1, 1) == Approx(1));

        lens.setFocalLength(1 * t::meter());
        mat = lens.getRTMatrix();

        CHECK(mat(0, 0) == Approx(1));
        CHECK(mat(0, 1) == Approx(0));
        CHECK(mat(1, 0) == Approx(-1. / 100));
        CHECK(mat(1, 1) == Approx(1));
      }
    }

    SECTION("Internal configuration")
    {
      ptree configTree;
      configTree.put("type", "Thin Lens");
      configTree.put("focal_length", 10.);
      elem.reset(OEBuilder.build(configTree));
      REQUIRE(elem != nullptr);

      auto mat = elem->getRTMatrix();

      CHECK(mat(0, 0) == Approx(1));
      CHECK(mat(0, 1) == Approx(0));
      CHECK(mat(1, 0) == Approx(-1. / 10));
      CHECK(mat(1, 1) == Approx(1));
    }
  }

  SECTION("Spherical interface tests")
  {
    ptree configTree;
    configTree.put("type", "Spherical Interface");
    configTree.put("radius_of_curvature", 10.);
    configTree.put("refractive_index.initial", 1.);
    configTree.put("refractive_index.final", 2.);
    elem.reset(OEBuilder.build(configTree));
    REQUIRE(elem != nullptr);

    SphericalInterface<t::centimeter>& interface =
        *std::dynamic_pointer_cast<SphericalInterface<t::centimeter>>(elem);

    auto mat = elem->getRTMatrix();

    CHECK(mat(0, 0) == Approx(1));
    CHECK(mat(0, 1) == Approx(0));
    CHECK(mat(1, 0) == Approx(-1. / 20));
    CHECK(mat(1, 1) == Approx(1. / 2));

    configTree.put("radius_of_curvature", -3.5);
    elem.reset(OEBuilder.build(configTree));
    mat = elem->getRTMatrix();
    CHECK(mat(0, 0) == Approx(1));
    CHECK(mat(0, 1) == Approx(0));
    CHECK(mat(1, 0) == Approx(+1. / 7));
    CHECK(mat(1, 1) == Approx(1. / 2));
  }
}

#include "Builders/BeamBuilder.hpp"
#include "Builders/OpticalSystemBuilder.hpp"
#include "GaussianBeam.hpp"
#include "OpticalElements/SphericalInterface.hpp"
#include "OpticalElements/ThinLens.hpp"
#include "OpticalSystem.hpp"
TEST_CASE("Optical Systems", "[OpticalSystem]")
{
  SECTION("manual configuration")
  {
    GaussianBeam                 beam;
    OpticalSystem<t::centimeter> system;

    std::shared_ptr<ThinLens<t::centimeter>> lens;

    lens.reset(new ThinLens<t::centimeter>());
    lens->setFocalLength(100 * mm);

    system.addElement(lens, 0 * cm);

    lens.reset(new ThinLens<t::centimeter>());
    lens->setFocalLength(200 * mm);

    system.addElement(lens, 10 * cm);

    CHECK(system.getElements().size() == 2);
    auto elem = system.getElements().begin();
    CHECK(elem->first.value() == Approx(0));
    elem++;
    CHECK(elem->first.value() == Approx(10));

    lens.reset(new ThinLens<t::centimeter>());
    lens->setFocalLength(20 * mm);

    system.addElement(lens, 10 * mm);

    CHECK(system.getElements().size() == 3);
    elem = system.getElements().begin();
    CHECK(elem->first.value() == Approx(0));
    elem++;
    CHECK(elem->first.value() == Approx(1));
    elem++;
    CHECK(elem->first.value() == Approx(10));

    beam.setWavelength(0.532 * um);
    beam.setOneOverE2WaistDiameter(10 * um);
    beam.setWaistPosition(-10 * cm);

    GaussianBeam beam2 = system.transform(beam);
  }

  SECTION("ptree configuration")
  {
    ptree configTree;
    configTree.put("beam.wavelength", 532);
    configTree.put("beam.divergence", 2);
    configTree.put("beam.waist.position", -400);

    configTree.put("optical_system.elements.0.position", 100);
    configTree.put("optical_system.elements.0.type", "Thin Lens");
    configTree.put("optical_system.elements.0.focal_length", 200);

    configTree.put("optical_system.elements.1.position", 110);
    configTree.put("optical_system.elements.1.type", "Thin Lens");
    configTree.put("optical_system.elements.1.focal_length", 300);

    BeamBuilder                         beambuilder;
    OpticalSystemBuilder<t::centimeter> sysbuilder;

    std::shared_ptr<OpticalSystem<t::centimeter>> system(
        sysbuilder.build(configTree.get_child("optical_system")));
    std::shared_ptr<GaussianBeam> beam(
        beambuilder.build(configTree.get_child("beam")));

    CHECK(beam->getWavelength<t::nanometer>().value() == Approx(532));
    CHECK(beam->getOneOverE2FullAngleDivergence<t::milliradian>().value() ==
          Approx(2));
    CHECK(beam->getWaistPosition<t::centimeter>().value() == Approx(-400));

    system->transform(beam.get());

    std::shared_ptr<GaussianBeam> beam2(
        beambuilder.build(configTree.get_child("beam")));
    ThinLens<t::centimeter> lens1, lens2;
    lens1.setFocalLength(200 * cm);
    lens2.setFocalLength(300 * cm);

    beam2->transform(&lens1, 100 * cm);
    beam2->transform(&lens2, 110 * cm);

    // make sure the system produces the same results as manually applying the
    // lenses
    CHECK(beam->getWavelength().value() ==
          Approx(beam2->getWavelength().value()));
    CHECK(beam->getWaistPosition().value() ==
          Approx(beam2->getWaistPosition().value()));
    CHECK(beam->getOneOverE2WaistDiameter().value() ==
          Approx(beam2->getOneOverE2WaistDiameter().value()));
  }
}

#include "Builders/MediaStackBuilder.hpp"
#include "Media/LinearAbsorber.hpp"
#include "MediaStack.hpp"
TEST_CASE("Media Stack")
{
  SECTION("boundaries configuration")
  {
    MediaStack<t::centimeter> stack;

    std::shared_ptr<LinearAbsorber<t::centimeter>> abs;

    abs.reset(new LinearAbsorber<t::centimeter>());
    abs->setAbsorptionCoefficient(0.1 / cm);
    stack.setBackgroundMedia(abs);

    abs.reset(new LinearAbsorber<t::centimeter>());
    abs->setAbsorptionCoefficient(1 / cm);
    stack.addBoundary(abs, 0 * cm);

    abs.reset(new LinearAbsorber<t::centimeter>());
    abs->setAbsorptionCoefficient(10 / cm);
    stack.addBoundary(abs, 1 * cm);

    abs.reset(new LinearAbsorber<t::centimeter>());
    abs->setAbsorptionCoefficient(2 / cm);
    stack.addBoundary(abs, 1.001 * cm);

    // zi and zf are both in front of the first boundary.
    CHECK(stack.getTransmission(-2 * cm, -1 * cm) == Approx(exp(-0.1)));
    // all boundaries are between z1 and zf.
    CHECK(stack.getTransmission(-1 * cm, 0.1 * m) ==
          Approx(exp(-0.1) * exp(-1) * exp(-10 * 0.001) *
                 exp(-2 * (10 - 1.001))));
    // zi and zf are both between the 1st and 2nd boundaries.
    CHECK(stack.getTransmission(0.1 * cm, 0.9 * cm) == Approx(exp(-0.8)));
    // zi and zf are both beghind between the 1st and 2nd boundaries.
    CHECK(stack.getTransmission(2 * m, 2.1 * m) == Approx(exp(-2 * 10)));
  }

  SECTION("builder configuration")
  {
    ptree configTree;

    configTree.put("media_stack.media.background.type", "linear absorber");
    configTree.put("media_stack.media.background.absorption_coefficient", 0.1);

    configTree.put("media_stack.media.0.type", "linear absorber");
    configTree.put("media_stack.media.0.position", 0);
    configTree.put("media_stack.media.0.absorption_coefficient", 1);

    configTree.put("media_stack.media.1.type", "linear absorber");
    configTree.put("media_stack.media.1.absorption_coefficient", 10);
    configTree.put("media_stack.media.1.position", 1);

    configTree.put("media_stack.media.2.type", "linear absorber");
    configTree.put("media_stack.media.2.absorption_coefficient", 2);
    configTree.put("media_stack.media.2.position", 1.001);

    MediaStackBuilder<t::centimeter>           MSb;
    std::shared_ptr<MediaStack<t::centimeter>> stack(
        MSb.build(configTree.get_child("media_stack")));

    // zi and zf are both in front of the first boundary.
    CHECK(stack->getTransmission(-2 * cm, -1 * cm) == Approx(exp(-0.1)));
    // all boundaries are between z1 and zf.
    CHECK(stack->getTransmission(-1 * cm, 0.1 * m) ==
          Approx(exp(-0.1) * exp(-1) * exp(-10 * 0.001) *
                 exp(-2 * (10 - 1.001))));
    // zi and zf are both between the 1st and 2nd boundaries.
    CHECK(stack->getTransmission(0.1 * cm, 0.9 * cm) == Approx(exp(-0.8)));
    // zi and zf are both beghind between the 1st and 2nd boundaries.
    CHECK(stack->getTransmission(2 * m, 2.1 * m) == Approx(exp(-2 * 10)));
  }

  SECTION("builder configuration 2")
  {
    ptree configTree;

    configTree.put("media_stack.media.background.type", "linear absorber");
    configTree.put("media_stack.media.background.absorption_coefficient", 0.1);

    configTree.put("media_stack.media.0.type", "linear absorber");
    configTree.put("media_stack.media.0.position", 0);
    configTree.put("media_stack.media.0.absorption_coefficient", 1);
    configTree.put("media_stack.media.0.thickness", 1);

    configTree.put("media_stack.media.1.type", "linear absorber");
    configTree.put("media_stack.media.1.absorption_coefficient", 10);
    configTree.put("media_stack.media.1.thickness", 0.001);

    configTree.put("media_stack.media.2.type", "linear absorber");
    configTree.put("media_stack.media.2.absorption_coefficient", 2);

    MediaStackBuilder<t::centimeter>           MSb;
    std::shared_ptr<MediaStack<t::centimeter>> stack(
        MSb.build(configTree.get_child("media_stack")));

    // zi and zf are both in front of the first boundary.
    CHECK(stack->getTransmission(-2 * cm, -1 * cm) == Approx(exp(-0.1)));
    // all boundaries are between z1 and zf.
    CHECK(stack->getTransmission(-1 * cm, 0.1 * m) ==
          Approx(exp(-0.1) * exp(-1) * exp(-10 * 0.001) *
                 exp(-2 * (10 - 1.001))));
    // zi and zf are both between the 1st and 2nd boundaries.
    CHECK(stack->getTransmission(0.1 * cm, 0.9 * cm) == Approx(exp(-0.8)));
    // zi and zf are both beghind between the 1st and 2nd boundaries.
    CHECK(stack->getTransmission(2 * m, 2.1 * m) == Approx(exp(-2 * 10)));
  }

  SECTION("builder configuration 3")
  {
    ptree configTree;

    // two layers of media separated by a layer of background
    configTree.put("media_stack.media.0.type", "linear absorber");
    configTree.put("media_stack.media.0.position", 0);
    configTree.put("media_stack.media.0.absorption_coefficient", 1);
    configTree.put("media_stack.media.0.thickness", 0.1);

    configTree.put("media_stack.media.1.type", "linear absorber");
    configTree.put("media_stack.media.1.position", 1);
    configTree.put("media_stack.media.1.absorption_coefficient", 10);
    configTree.put("media_stack.media.1.thickness", 0.1);

    MediaStackBuilder<t::centimeter>           MSb;
    std::shared_ptr<MediaStack<t::centimeter>> stack(
        MSb.build(configTree.get_child("media_stack")));

    // zi and zf are both in front of the first boundary.
    CHECK(stack->getTransmission(-2 * cm, -1 * cm) == Approx(1));
    // all boundaries are between z1 and zf.
    CHECK(stack->getTransmission(-1 * cm, 3 * cm) ==
          Approx(exp(-0.1) * exp(-1)));
    // zi and zf are both in the air gap
    CHECK(stack->getTransmission(1.2 * cm, 1.9 * cm) == Approx(1));
    // zi and zf are both in behind the last boundary.
    CHECK(stack->getTransmission(3 * cm, 4 * cm) == Approx(1));
  }
}

#include "GBPCalc.hpp"
TEST_CASE("GBPCalc tests")
{
  ptree configTree;
  configTree.put("beam.wavelength", 444);
  configTree.put("beam.waist.position", 0);
  configTree.put("beam.waist.diameter", 0.25);
  configTree.put("beam.power", 0.800);

  configTree.put("optical_system.elements.0.position", 15);
  configTree.put("optical_system.elements.0.type", "Thin Lens");
  configTree.put("optical_system.elements.0.focal_length", 12);

  configTree.put("media_stack.media.0.type", "Linear Absorber");
  configTree.put("media_stack.media.0.position", 15);
  configTree.put("media_stack.media.0.thickness", 1);
  configTree.put("media_stack.media.0.absorption_coefficient", 2);

  GBPCalc<t::centimeter> calculator;
  calculator.configure(configTree);

  auto beam = calculator.getBeam(14.9999 * cm);

  CHECK(beam.getWavelength().value() == Approx(444));
  CHECK(beam.getPower().value() == Approx(0.800));
  CHECK(beam.getRadiusOfCurvature().value() == Approx(81501));
  CHECK(beam.getWaistPosition().value() == Approx(0));
  CHECK(beam.getOneOverE2WaistDiameter().value() == Approx(0.25));

  beam = calculator.getBeam(15 * cm);

  CHECK(beam.getWavelength().value() == Approx(444));
  CHECK(beam.getPower().value() == Approx(0.800 * exp(0)));
  CHECK(beam.getRadiusOfCurvature().value() == Approx(-12.0018));
  CHECK(beam.getWaistPosition().value() == Approx(27.0004));
  CHECK(beam.getOneOverE2WaistDiameter().value() == Approx(0.00271352));

  beam = calculator.getBeam(100 * cm);

  CHECK(beam.getWavelength().value() == Approx(444));
  CHECK(beam.getPower().value() == Approx(0.800 * exp(-2)));
  CHECK(beam.getRadiusOfCurvature().value() > 0);
  CHECK(beam.getWaistPosition().value() == Approx(27.0004));
  CHECK(beam.getOneOverE2WaistDiameter().value() == Approx(0.00271352));

  std::vector<quantity<t::centimeter>> z_vals;
  calculator.sig_calculatedBeam.connect([&z_vals](const GaussianBeam& beam) {
    z_vals.push_back(beam.getCurrentPosition());
  });

  configTree.put("evaluation_points.z.min", 0);
  configTree.put("evaluation_points.z.max", 100);
  configTree.put("evaluation_points.z.n", 2);

  calculator.configure(configTree);
  calculator.calculate();

  REQUIRE(z_vals.size() == 2);
  CHECK(z_vals[0].value() == Approx(0));
  CHECK(z_vals[1].value() == Approx(100));

  configTree.erase("evaluation_points");
  configTree.put("evaluation_points.z.0", 10);
  configTree.put("evaluation_points.z.1", 11);
  configTree.put("evaluation_points.z.2", 12);

  calculator.configure(configTree);
  calculator.calculate();

  CHECK(z_vals.size() == 5);
  CHECK(z_vals[0].value() == Approx(0));
  CHECK(z_vals[1].value() == Approx(100));

  CHECK(z_vals[2].value() == Approx(10));
  CHECK(z_vals[3].value() == Approx(11));
  CHECK(z_vals[4].value() == Approx(12));
}

#include "GaussianBeam.hpp"
#include "OpticalElements/ThinLens.hpp"
TEST_CASE("Gaussian Beam Examples", "[GuassianBeam,Examples]")
{
  SECTION("Basic Example")
  {
    GaussianBeam beam;

    beam.setOneOverEWaistDiameter(5 * i::mm);
    beam.setOneOverEFullAngleDivergence(1.5 * i::mrad);

    // using Catch2
    CHECK(beam.getOneOverEDiameter<t::cm>(10 * i::m).value() ==
          Approx(1.58).epsilon(0.01));
  }

  SECTION("Lens Example")
  {
    GaussianBeam beam;
    ThinLens<t::centimeter> lens;

    beam.setWavelength(532 * i::nm);
    beam.setOneOverE2WaistDiameter(5 * i::mm);
    lens.setFocalLength( 15*i::cm );

    beam.transform(&lens);


    CHECK( beam.getWaistPosition<t::centimeter>().value() == Approx(15) );
    CHECK( beam.getOneOverE2WaistRadius<t::micrometer>().value() == Approx(1e6 * 532e-9*15e-2/M_PI/(5e-3/2)) ); // lambda f / pi omega

  }
}
