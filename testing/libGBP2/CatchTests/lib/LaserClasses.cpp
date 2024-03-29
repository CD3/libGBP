#include <complex>
#include <iostream>

#include <BoostUnitDefinitions/Units.hpp>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <libGBP2/CircularGaussianLaserBeam.hpp>
#include <libGBP2/CircularLaserBeam.hpp>
#include <libGBP2/Conventions.hpp>
#include <libGBP2/MonochromaticSource.hpp>
#include <libGBP2/Units.hpp>

using namespace Catch;

TEST_CASE("Monchromatic Source")
{
  using namespace libGBP2;
  SECTION("MonochromaticSource")
  {
    libGBP2::MonochromaticSource source;
    CHECK_COMPILE_FAILS(
        source.setWavelength(1 * i::s););
    CHECK_COMPILE_FAILS(
        source.setWavelength(1.););
    CHECK_COMPILE_FAILS(
        source.setFrequency(1 * i::s););
    CHECK_COMPILE_FAILS(
        source.setVacuumWavelength(1 * i::s););
    CHECK_COMPILE_FAILS(
        source.setRefractiveIndex(1 * i::s););
    SECTION("Setting Wavelenght, Refractive Index, and Frequency")
    {
      SECTION("wavelength -> refractive index")
      source.setWavelength(532 * i::nm);

      CHECK(source.getFrequency<t::MHz>().value() == Approx(563519657.894737));
      CHECK(source.getWavelength<t::nm>().value() == Approx(532));
      CHECK(source.getRefractiveIndex().value() == Approx(1));

      source.setRefractiveIndex(1.5 * i::dimensionless);

      CHECK(source.getFrequency<t::MHz>().value() == Approx(563519657.894737));
      CHECK(source.getWavelength<t::nm>().value() == Approx(532 / 1.5));
      CHECK(source.getRefractiveIndex().value() == Approx(1.5));

      source.setFrequency(source.getFrequency() / 2);

      CHECK(source.getFrequency<t::MHz>().value() == Approx(563519657.894737 / 2));
      CHECK(source.getWavelength<t::nm>().value() == Approx(2 * 532 / 1.5));
      CHECK(source.getRefractiveIndex().value() == Approx(1.5));

      source.setRefractiveIndex(1);

      CHECK(source.getFrequency<t::MHz>().value() == Approx(563519657.894737 / 2));
      CHECK(source.getWavelength<t::nm>().value() == Approx(2 * 532));
      CHECK(source.getRefractiveIndex().value() == Approx(1));
    }

    SECTION("Setting wavelength")
    {
      source.setWavelength(0.532 * libGBP2::i::um);

      CHECK(source.getWavelength().value() == Approx(532));
      CHECK(source.getWavelength<libGBP2::t::um>().value() == Approx(0.532));

      CHECK(source.getRefractiveIndex().value() == Approx(1));
      CHECK(source.getFrequency<libGBP2::t::MHz>().value() ==
            Approx(563519657.894737));

      SECTION("Setting frequency")
      {
        source.setFrequency(560000000000000. * libGBP2::i::Hz);
        CHECK(source.getFrequency<libGBP2::t::MHz>().value() == Approx(560000000));
        CHECK(source.getWavelength<t::m>().value() == Approx(299792458. / 560000000000000));
        CHECK(source.getRefractiveIndex().value() == Approx(1));
      }

      SECTION("Setting refractive index")
      {
        source.setRefractiveIndex(1.5);
        CHECK(source.getFrequency<libGBP2::t::MHz>().value() == Approx(563519657.894737));
        CHECK(source.getWavelength<libGBP2::t::nm>().value() < 532);
        CHECK(source.getRefractiveIndex().value() == Approx(1.5));
      }
    }
    SECTION("Copy construct and assignment")
    {
      // ORDER MATTERS HERE
      source.setRefractiveIndex(1.5 * libGBP2::i::dimensionless);
      source.setWavelength(532 * libGBP2::i::nm);

      libGBP2::MonochromaticSource source2(source);

      CHECK(source2.getWavelength().value() == Approx(532));
      CHECK(source2.getRefractiveIndex().value() == Approx(1.5));

      libGBP2::MonochromaticSource source3;

      source3 = source2;
      CHECK(source3.getWavelength().value() == Approx(532));
      CHECK(source3.getRefractiveIndex().value() == Approx(1.5));
    }
  }
}

TEST_CASE("CircularLaserBeam")
{
  libGBP2::CircularLaserBeam beam;
  beam.setWavelength(633 * libGBP2::i::nm);

  SECTION("Configuration")
  {
    beam.setSecondMomentBeamWaistWidth(100 * libGBP2::i::um);

    CHECK(beam.getSecondMomentBeamWaistWidth<libGBP2::t::cm>().value() ==
          Approx(0.01));
    CHECK(beam.getD4SigmaBeamWaistWidth<libGBP2::t::cm>().value() ==
          Approx(0.02));
    CHECK(beam.getBeamQualityFactor() == Approx(1));
    CHECK(beam.getBeamWaistPosition<libGBP2::t::m>().value() ==
          Approx(0).scale(1));

    beam.setBeamWaistPosition(200 * libGBP2::i::cm);

    CHECK(beam.getBeamWaistPosition<libGBP2::t::m>().value() == Approx(2));
    CHECK(beam.getSecondMomentDivergence().value() == Approx(2.0149));
    CHECK(beam.getD4SigmaDivergence().value() == Approx(2 * 2.0149));

    beam.setBeamQualityFactor(2 * libGBP2::i::dimensionless);
    CHECK(beam.getSecondMomentDivergence().value() == Approx(2 * 2.0149));
    CHECK(beam.getD4SigmaDivergence().value() == Approx(2 * 2 * 2.0149));

    beam.setBeamQualityFactor(1 * libGBP2::i::dimensionless);
    CHECK(beam.getSecondMomentDivergence().value() == Approx(2.0149));
    CHECK(beam.getD4SigmaDivergence().value() == Approx(2 * 2.0149));

    beam.adjustSecondMomentDivergence(2 * 2.0149 * libGBP2::i::mrad);
    CHECK(beam.getSecondMomentDivergence().value() == Approx(2 * 2.0149));
    CHECK(beam.getD4SigmaDivergence().value() == Approx(2 * 2 * 2.0149));
    CHECK(beam.getDiffractionLimitedSecondMomentDivergence().value() ==
          Approx(2.0149));
    CHECK(beam.getDiffractionLimitedD4SigmaDivergence().value() ==
          Approx(2 * 2.0149));
    CHECK(beam.getBeamQualityFactor().value() == Approx(2));

    beam.setDiffractionLimitedSecondMomentDivergence(1.00745 *
                                                     libGBP2::i::mrad);
    CHECK(beam.getSecondMomentDivergence().value() == Approx(2 * 1.00745));
    CHECK(beam.getD4SigmaDivergence().value() == Approx(2 * 2 * 1.00745));
    CHECK(beam.getDiffractionLimitedSecondMomentDivergence().value() ==
          Approx(1.00745));
    CHECK(beam.getDiffractionLimitedD4SigmaDivergence().value() ==
          Approx(2 * 1.00745));
    CHECK(beam.getBeamQualityFactor().value() == Approx(2));
    CHECK(beam.getSecondMomentBeamWaistWidth<libGBP2::t::mm>().value() ==
          Approx(0.2));
    CHECK(beam.getD4SigmaBeamWaistWidth<libGBP2::t::mm>().value() ==
          Approx(0.4));

    beam.setDiffractionLimitedD4SigmaDivergence(1.00745 * libGBP2::i::mrad);
    CHECK(beam.getSecondMomentDivergence().value() == Approx(2 * 1.00745 / 2));
    CHECK(beam.getD4SigmaDivergence().value() == Approx(2 * 1.00745));
    CHECK(beam.getDiffractionLimitedSecondMomentDivergence().value() ==
          Approx(1.00745 / 2));
    CHECK(beam.getDiffractionLimitedD4SigmaDivergence().value() ==
          Approx(1.00745));
    CHECK(beam.getBeamQualityFactor().value() == Approx(2));
    CHECK(beam.getSecondMomentBeamWaistWidth<libGBP2::t::mm>().value() ==
          Approx(2 * 0.2));
    CHECK(beam.getD4SigmaBeamWaistWidth<libGBP2::t::mm>().value() ==
          Approx(4 * 0.2));
  }

  SECTION("Range Equations")
  {
    beam.setSecondMomentBeamWaistWidth(0.002 * libGBP2::i::mm);
    beam.setBeamWaistPosition(100 * libGBP2::i::mm);

    CHECK(beam.getBeamQualityFactor().value() == Approx(1));
    CHECK(beam.getSecondMomentDivergence().value() == Approx(100.74508));
    CHECK(beam.getSecondMomentBeamWidth<libGBP2::t::mm>(100 * libGBP2::i::mm)
              .value() == Approx(0.002));
    CHECK(beam.getSecondMomentBeamWidth<libGBP2::t::mm>().value() ==
          Approx(10.07451));
  }
  SECTION("Copy construct and assignment")
  {
    beam.setRefractiveIndex(1.5 * libGBP2::i::dimensionless);
    beam.setVacuumWavelength(532 * libGBP2::i::nm);
    beam.setBeamWaistPosition(2 * libGBP2::i::m);
    beam.setSecondMomentBeamWaistWidth(100 * libGBP2::i::um);

    CHECK(beam.getWavelength().value() == Approx(532 / 1.5));
    CHECK(beam.getVacuumWavelength().value() == Approx(532));
    CHECK(beam.getRefractiveIndex().value() == Approx(1.5));
    CHECK(beam.getBeamWaistPosition().value() == Approx(200));
    CHECK(beam.getSecondMomentBeamWaistWidth().value() == Approx(0.01));

    libGBP2::CircularLaserBeam beam2(beam);

    CHECK(beam2.getWavelength().value() == Approx(532 / 1.5));
    CHECK(beam2.getVacuumWavelength().value() == Approx(532));
    CHECK(beam2.getRefractiveIndex().value() == Approx(1.5));
    CHECK(beam2.getBeamWaistPosition().value() == Approx(200));
    CHECK(beam2.getSecondMomentBeamWaistWidth().value() == Approx(0.01));

    libGBP2::CircularLaserBeam beam3;

    beam3 = beam2;
    CHECK(beam3.getWavelength().value() == Approx(532 / 1.5));
    CHECK(beam3.getVacuumWavelength().value() == Approx(532));
    CHECK(beam3.getRefractiveIndex().value() == Approx(1.5));
    CHECK(beam3.getBeamWaistPosition().value() == Approx(200));
    CHECK(beam3.getSecondMomentBeamWaistWidth().value() == Approx(0.01));
  }
}

TEST_CASE("Convention Conversions")
{
  using namespace libGBP2;

  SECTION("Misc")
  {
    // make sure these all compile
    FromOneOverESquaredRadiusCF<SecondMomentWidth>();
    FromOneOverESquaredRadiusCF<D4SigmaWidth>();
    FromOneOverESquaredRadiusCF<OneOverERadius>();
    FromOneOverESquaredRadiusCF<OneOverEDiameter>();
    FromOneOverESquaredRadiusCF<
        OneOverESquaredRadius>();
    FromOneOverESquaredRadiusCF<
        OneOverESquaredDiameter>();
    FromOneOverESquaredRadiusCF<FWHMRadius>();
    FromOneOverESquaredRadiusCF<FWHMDiameter>();
    // compiler error, no function specialization for this type
    // FromOneOverESquaredRadiusCF<t::cm>();
  }

  SECTION("Conversion Factors")
  {
    CHECK(
        (BeamWidthConversionFactor<D4SigmaWidth,
                                   D4SigmaWidth>() ==
         Approx(1)));
    CHECK(
        (BeamWidthConversionFactor<
             D4SigmaWidth, OneOverESquaredRadius>() ==
         Approx(0.5)));
    CHECK(
        (BeamWidthConversionFactor<
             FWHMDiameter, OneOverESquaredRadius>() ==
         Approx(0.8493281)));
    CHECK(
        (BeamWidthConversionFactor<
             OneOverESquaredRadius, FWHMDiameter>() ==
         Approx(1 / 0.8493281)));
    CHECK(
        (BeamWidthConversionFactor<OneOverEDiameter,
                                   FWHMDiameter>() ==
         Approx(0.8325546111576977)));

    CHECK((
        BeamDivergenceConversionFactor<
            D4SigmaDivergence, D4SigmaDivergence>() ==
        Approx(1)));
    CHECK((BeamDivergenceConversionFactor<
               D4SigmaDivergence,
               OneOverESquaredHalfAngle>() ==
           Approx(0.5)));
    CHECK((BeamDivergenceConversionFactor<
               FWHMFullAngle,
               OneOverESquaredHalfAngle>() ==
           Approx(0.8493281)));
    CHECK(
        (BeamDivergenceConversionFactor<
             OneOverESquaredHalfAngle,
             FWHMFullAngle>() == Approx(1 / 0.8493281)));
    CHECK((BeamDivergenceConversionFactor<
               OneOverEFullAngle,
               FWHMFullAngle>() ==
           Approx(0.8325546111576977)));
  }

  SECTION("GaussianBeamWidth")
  {
    GaussianBeamWidth<OneOverEDiameter, t::cm> width = 2 * i::cm;

    CHECK(width.get<OneOverERadius>().value() == Approx(1));
    CHECK((width.get<OneOverERadius, t::mm>().value() == Approx(10)));

    width = 4 * i::cm;
    CHECK(width.get<OneOverERadius>().value() == Approx(2));

    width = make_width<OneOverERadius>(4 * i::mm);
    CHECK(width.get<OneOverERadius>().value() == Approx(0.4));
    CHECK((width.get<OneOverERadius, t::m>().value() == Approx(0.004)));
    CHECK((width.get<OneOverEDiameter, t::cm>().value() == Approx(0.8)));

    width = 4 * i::cm;
    // quantity<t::mm> D_fwhm = width.get<FWHMDiameter>();  // compiler error, units don't match
    quantity<t::cm> D_fwhm = width.get<FWHMDiameter>();
    CHECK(D_fwhm.value() == Approx(sqrt(log(2)) * 4));
    CHECK(quantity<t::mm>(D_fwhm).value() == Approx(sqrt(log(2)) * 40));

    auto D_fwhm_2 = width.get<FWHMDiameter, t::m>();
    CHECK(D_fwhm_2.value() == Approx(sqrt(log(2)) * 0.04));
    CHECK(quantity<t::mm>(D_fwhm_2).value() == Approx(sqrt(log(2)) * 40));
  }

  SECTION("GaussianBeamDivergence")
  {
    GaussianBeamDivergence<OneOverEFullAngle, t::mrad> div = 2 * i::mrad;

    CHECK(div.get<OneOverEHalfAngle>().value() ==
          Approx(1));

    div = make_divergence<OneOverEFullAngle>(4 * i::mrad);
    CHECK(div.get<OneOverEHalfAngle>().value() == Approx(2));
    CHECK((div.get<OneOverEHalfAngle, t::rad>().value() == Approx(2e-3)));

    auto theta_fwhm = div.get<FWHMFullAngle>();
    CHECK(theta_fwhm.value() == Approx(sqrt(log(2)) * 4));
  }
}

TEST_CASE("CircularGaussianLaserBeam")
{
  using namespace libGBP2;
  CircularGaussianLaserBeam beam;

  beam.setWavelength(633 * i::nm);
  beam.setBeamWaistWidth(make_width<OneOverESquaredRadius>(2 * i::um));
  beam.setBeamWaistPosition(100 * i::mm);

  CHECK(beam.getDiffractionLimitedBeamDivergence<t::mrad>().get<OneOverESquaredHalfAngle>().value() == Approx(100.74508));
  CHECK(beam.getBeamDivergence<t::mrad>().get<OneOverESquaredHalfAngle>().value() == Approx(100.74508));
  CHECK(beam.getBeamQualityFactor().value() == Approx(1));
  CHECK(beam.getBeamWidth<t::mm>().get<OneOverESquaredRadius>().value() == Approx(10.07451));
  CHECK(beam.getBeamWidth<t::mm>().get<OneOverEDiameter>().value() == Approx(10.07451 * sqrt(2)));
  CHECK(beam.getBeamWidth<t::mm>(100 * i::mm).get<OneOverESquaredRadius>().value() == Approx(0.002));
  CHECK(beam.getBeamWidth<t::mm>(100 * i::mm).get<OneOverEDiameter>().value() == Approx(0.002 * sqrt(2)));
  CHECK(beam.getRayleighRange<t::mm>().value() == Approx(0.01985).epsilon(0.001));
  CHECK(beam.getRadiusOfCurvature<t::mm>().value() == Approx(-100));
  CHECK(beam.getRadiusOfCurvature<t::mm>(200 * i::mm).value() == Approx(100));
  CHECK(beam.getRadiusOfCurvature<t::mm>(100.1 * i::mm).value() == Approx(0.10394));
  CHECK(beam.getRadiusOfCurvature<t::mm>(99.9 * i::mm).value() == Approx(-0.10394));
  CHECK(beam.getGouyPhase<t::rad>(100 * i::mm).value() == Approx(0).scale(1));
  CHECK(beam.getGouyPhase<t::rad>().value() == Approx(atan(-100 / 0.01985)));

  CHECK(beam.getBeamWidth<t::mm>(beam.getBeamWaistPosition() + beam.getRayleighRange()).get<OneOverESquaredRadius>().value() == Approx(0.00283).epsilon(0.001));
  CHECK(beam.getBeamWidth<t::mm>(beam.getBeamWaistPosition() - beam.getRayleighRange()).get<OneOverESquaredRadius>().value() == Approx(0.00283).epsilon(0.001));

  beam.adjustBeamDivergence(make_divergence<OneOverESquaredHalfAngle>(2 * 100.74508 * i::mrad));

  CHECK(beam.getDiffractionLimitedBeamDivergence<t::mrad>().get<OneOverESquaredHalfAngle>().value() == Approx(100.74508));
  CHECK(beam.getBeamDivergence<t::mrad>().get<OneOverESquaredHalfAngle>().value() == Approx(2 * 100.74508));
  CHECK(beam.getBeamQualityFactor().value() == Approx(2));
  CHECK(beam.getBeamWaistWidth<t::mm>().get<OneOverESquaredRadius>().value() == Approx(0.002));

  CHECK(beam.getBeamWidth<t::mm>().get<OneOverESquaredRadius>().value() == Approx(2 * 10.07451));
  CHECK(beam.getBeamWidth<t::mm>().get<OneOverEDiameter>().value() == Approx(2 * 10.07451 * sqrt(2)));
  CHECK(beam.getBeamWidth<t::mm>(100 * i::mm).get<OneOverESquaredRadius>().value() == Approx(0.002));
  CHECK(beam.getBeamWidth<t::mm>(100 * i::mm).get<OneOverEDiameter>().value() == Approx(0.002 * sqrt(2)));

  CHECK(beam.getRayleighRange<t::mm>().value() == Approx(0.01985 / 2).epsilon(0.001));

  SECTION("Copy construct and assignment")
  {
    beam.setRefractiveIndex(1.5 * libGBP2::i::dimensionless);
    beam.setVacuumWavelength(532 * libGBP2::i::nm);
    beam.setBeamWaistPosition(2 * libGBP2::i::m);
    beam.setBeamWaistWidth(make_width<OneOverERadius>(100 * libGBP2::i::um));

    CHECK(beam.getWavelength().value() == Approx(532 / 1.5));
    CHECK(beam.getVacuumWavelength().value() == Approx(532));
    CHECK(beam.getRefractiveIndex().value() == Approx(1.5));
    CHECK(beam.getBeamWaistPosition().value() == Approx(200));
    CHECK(beam.getBeamWaistWidth().get<OneOverESquaredRadius>().value() == Approx(0.01 * sqrt(2)));

    libGBP2::CircularLaserBeam beam2(beam);

    CHECK(beam2.getWavelength().value() == Approx(532 / 1.5));
    CHECK(beam2.getVacuumWavelength().value() == Approx(532));
    CHECK(beam2.getRefractiveIndex().value() == Approx(1.5));
    CHECK(beam2.getBeamWaistPosition().value() == Approx(200));
    CHECK(beam.getBeamWaistWidth().get<OneOverESquaredRadius>().value() == Approx(0.01 * sqrt(2)));

    libGBP2::CircularLaserBeam beam3;

    beam3 = beam2;
    CHECK(beam3.getWavelength().value() == Approx(532 / 1.5));
    CHECK(beam3.getVacuumWavelength().value() == Approx(532));
    CHECK(beam3.getRefractiveIndex().value() == Approx(1.5));
    CHECK(beam3.getBeamWaistPosition().value() == Approx(200));
    CHECK(beam.getBeamWaistWidth().get<OneOverESquaredRadius>().value() == Approx(0.01 * sqrt(2)));
  }
}

TEST_CASE("Complex Beam Parameter")
{
  using namespace libGBP2;
  CircularGaussianLaserBeam beam;

  beam.setWavelength(633 * i::nm);
  beam.setBeamWaistWidth(make_width<OneOverESquaredRadius>(2 * i::um));
  beam.setBeamWaistPosition(100 * i::mm);

  SECTION("in default units (cm)")
  {
    auto q = beam.getComplexBeamParameter();

    CHECK(q.value().real() == Approx(-10));
    CHECK(q.value().imag() == Approx(0.001985).epsilon(0.001));
  }

  SECTION("in mm")
  {
    auto q = beam.getComplexBeamParameter<t::mm>();

    CHECK(q.value().real() == Approx(-100));
    CHECK(q.value().imag() == Approx(0.01985).epsilon(0.001));
  }

  SECTION("Setting q")
  {
    // q = z-z_0 + z_R i
    // the position z = 1 cm is 50 mm to the right of the waist, which means the waist is at -40 cm
    beam.setComplexBeamParameter(std::complex<double>{50, 0.00124} * i::mm, 1 * i::cm);

    CHECK(beam.getBeamWaistPosition<t::mm>().value() == Approx(-40));
    CHECK(beam.getBeamWaistWidth<t::mm>().get<OneOverESquaredRadius>().value() == Approx(0.0005).epsilon(0.001));
  }
}

TEST_CASE("Embedded Beam")
{
  using namespace libGBP2;

  CircularGaussianLaserBeam beam;
  beam.setWavelength(633 * i::nm);
  beam.setBeamWaistPosition(100 * i::mm);
  beam.setBeamWaistWidth(make_width<OneOverESquaredRadius>(2 * i::um));
  beam.adjustBeamDivergence(make_divergence<OneOverESquaredHalfAngle>(4 * 100.74508 * i::mrad));

  CHECK(beam.getBeamQualityFactor().value() == Approx(4));
  CHECK(beam.getBeamWaistWidth<t::um>().get<OneOverESquaredRadius>().value() == Approx(2));
  CHECK(beam.getBeamDivergence<t::mrad>().get<OneOverESquaredHalfAngle>().value() == Approx(4 * 100.74508));
  CHECK(beam.getDiffractionLimitedBeamDivergence<t::mrad>().get<OneOverESquaredHalfAngle>().value() == Approx(100.74508));
  CHECK(beam.getBeamWaistPosition<t::mm>().value() == Approx(100));

  double M2 = beam.getBeamDivergence<t::mrad>().get<OneOverESquaredHalfAngle>().value() / beam.getDiffractionLimitedBeamDivergence<t::mrad>().get<OneOverESquaredHalfAngle>().value();

  auto ebeam = beam.getEmbeddedBeam();

  CHECK(M2 == Approx(4));
  CHECK(ebeam.getBeamWaistWidth<t::um>().get<OneOverESquaredRadius>().value() == Approx(2 / 2));
  CHECK(ebeam.getBeamDivergence<t::mrad>().get<OneOverESquaredHalfAngle>().value() == Approx(2 * 100.74508));
  CHECK(ebeam.getBeamWaistPosition<t::mm>().value() == Approx(100));

  CircularGaussianLaserBeam beam2;

  beam2.setBeamQualityFactor(4 * i::dimensionless);
  beam2.setEmbeddedBeam(ebeam);

  CHECK(beam2.getBeamQualityFactor().value() == Approx(4));
  CHECK(beam2.getBeamWaistWidth<t::um>().get<OneOverESquaredRadius>().value() == Approx(2));
  CHECK(beam2.getBeamDivergence<t::mrad>().get<OneOverESquaredHalfAngle>().value() == Approx(4 * 100.74508));
  CHECK(beam2.getDiffractionLimitedBeamDivergence<t::mrad>().get<OneOverESquaredHalfAngle>().value() == Approx(100.74508));
  CHECK(beam2.getBeamWaistPosition<t::mm>().value() == Approx(100));
}

TEST_CASE("conventions interface")
{
  using namespace libGBP2;
  GaussianBeamWidth<OneOverESquaredRadius, t::cm> width;
  width = 2 * i::mm;

  CHECK((width.get<OneOverESquaredDiameter, t::mm>().value() == Approx(4)));
  CHECK((width.get<OneOverESquaredDiameter>().value() == Approx(0.4)));

  width = make_width<OneOverESquaredDiameter>(10 * i::mm);

  CHECK((width.get<OneOverESquaredDiameter, t::mm>().value() == Approx(10)));
  CHECK((width.get<OneOverESquaredDiameter>().value() == Approx(1)));
  CHECK((width.get<OneOverESquaredRadius, t::mm>().value() == Approx(5)));
  CHECK((width.get<OneOverESquaredRadius>().value() == Approx(0.5)));

  CircularGaussianLaserBeam beam;

  beam.setBeamWaistWidth(width);

  CHECK(beam.getBeamWaistWidth().get<OneOverESquaredRadius>().value() == Approx(0.5));
  CHECK((beam.getBeamWaistWidth().get<OneOverESquaredRadius, t::m>().value() == Approx(0.005)));
  CHECK((beam.getBeamWaistWidth<t::m>().get<OneOverESquaredRadius>().value() == Approx(0.005)));
}

TEST_CASE("Complex Numbers")
{
  std::complex<double> a(1, 1);

  auto b = conj(a) / norm(a);
  CHECK(b.real() == Approx(0.5));
  CHECK(b.imag() == Approx(-0.5));

  auto i = std::complex<double>(0, 1);

  a = 2. + 6. * i;

  b = conj(a) / norm(a);
  CHECK(b.real() == Approx(1. / 20));
  CHECK(b.imag() == Approx(-3. / 20));
}
