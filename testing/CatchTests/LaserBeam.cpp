#include <libGBP/LaserBeam.hpp>

#include "catch.hpp"

using namespace libGBP;

SCENARIO("LaserBeam can be configured and compute beam width at range")
{
  GIVEN("An uninitialized LaserBeam instance")
  {
    LaserBeam beam;

    WHEN("the wavelength is set to 700 nm")
    {
      beam.setWavelength(700. * i::nm);
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
      beam.setWaistPosition(7. * t::meter());
      THEN("the waist position getter returns 700 cm")
      {
        CHECK(beam.getWaistPosition().value() == Approx(700));
      }
    }

    WHEN("the frequency is set to 100 GHz")
    {
      beam.setFrequency(100. * t::gigahertz());
      THEN("the freespace wavelength getter returns 2.997925 mm")
      {
        CHECK(beam.getFreeSpaceWavelength<t::millimeter>().value() ==
              Approx(2.997925));
      }
    }

    WHEN("the wavelength is set to 0.633 um")
    {
      beam.setWavelength(0.633 * t::micrometer());

      AND_WHEN("the beam waist four sigma diameter is set to 2 mm")
      {
        beam.setWaistFourSigmaDiameter(2. * t::millimeter());

        THEN(
            "the diffraction limited angular spread standard deviation is is "
            "0.20149/2 mrad")
        {
          CHECK(beam.getDiffractionLimitedAngularSpreadStandardDeviation<
                        t::milliradian>()
                    .value() == Approx(0.20149 / 2));
        }
        AND_WHEN("the beam waist position is set to 2 m")
        {
          beam.setAngularSpreadStandardDeviation(
              beam.getDiffractionLimitedAngularSpreadStandardDeviation());
          beam.setWaistPosition(2. * i::m);
          THEN("the beam standard deviation at 12 m is 4.498820/4 mm")
          {
            CHECK(beam.getBeamStandardDeviation<t::millimeter>(12. * t::meter())
                      .value() == Approx(4.498820 / 4));
          }
          THEN("the beam four sigma diameter at 12 m is 4.498820 mm")
          {
            CHECK(beam.getFourSigmaDiameter<t::millimeter>(12. * t::meter())
                      .value() == Approx(4.498820));
          }
        }
      }
    }

    WHEN("the wavelength is set to 532 nm")
    {
      beam.setWavelength(532. * i::nm);
      AND_WHEN("the waist four sigma diameter is set to 20 um")
      {
        beam.setWaistFourSigmaDiameter(20. * t::micrometer());
        THEN("the waist standard deviation is 20e-6/4 m")
        {
          CHECK(beam.getWaistStandardDeviation<t::meter>().value() ==
                Approx(20e-6 / 4));
        }
        AND_THEN(
            "the diffraction limited angular spread standard deviation is "
            "8.467042972488832 mrad")
        {
          CHECK(beam.getDiffractionLimitedAngularSpreadStandardDeviation<
                        t::milliradian>()
                    .value() == Approx(8.467042972488832));
        }
        AND_WHEN("the waist position is set to 10 m")
        {
          beam.setWaistPosition(10. * t::meter());
          AND_WHEN("the beam angular spread is set to its diffraction limit")
          {
            THEN("the beam four sigma diameter at 18 m is 270.94538 mm")
            {
              beam.setAngularSpreadStandardDeviation(
                  beam.getDiffractionLimitedAngularSpreadStandardDeviation<
                      t::milliradian>());
              CHECK(beam.getFourSigmaDiameter<t::millimeter>(18. * i::m)
                        .value() == Approx(270.94538));
            }
          }
        }
      }
    }
  }
}

SCENARIO("Beam quality")
{
  GIVEN(
      "A LaserBeam with a 500 nm wavelength, a 4 um beam waist four sigma "
      "diameter, and a 79.5 mrad angular spread standard deviation")
  {
    LaserBeam beam;
    WHEN("The wavelength is 500 nm")
    {
      beam.setWavelength(500. * i::nm);
      AND_WHEN("The beam waist four sigma diameter is 4 um")
      {
        beam.setWaistFourSigmaDiameter(4. * i::um);
        AND_WHEN("The angular spread standard deviation is 80 mrad")
        {
          beam.setAngularSpreadStandardDeviation(80. * i::mrad);
          THEN("The beam's M^2 factor is 2")
          {
            CHECK(beam.getBeamPropagationFactor().value() == Approx(2).epsilon(0.01));
          }
        }
        AND_WHEN("The angular spread standard deviation is 160 mrad")
        {
          beam.setAngularSpreadStandardDeviation(160. * i::mrad);
          THEN("The beam's M^2 factor is 2")
          {
            CHECK(beam.getBeamPropagationFactor().value() == Approx(4).epsilon(0.01));
          }
        }
        AND_WHEN("The angular spread standard deviation is 0 mrad")
        {
          beam.setAngularSpreadStandardDeviation(0. * i::mrad);
          THEN("The beam's M^2 factor is 0")
          {
            CHECK(beam.getBeamPropagationFactor().value() +1 == Approx(1).epsilon(0.01));
          }
        }
      }
    }
  }
}
