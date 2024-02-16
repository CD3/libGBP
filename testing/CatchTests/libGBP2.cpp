#include "catch.hpp"

#include <BoostUnitDefinitions/Units.hpp>

#include <libGBP2/CircularLaserBeam.hpp>
#include <libGBP2/Conventions.hpp>
#include <libGBP2/MonochromaticSource.hpp>

TEST_CASE("Monchromatic Source")
{
  SECTION("MonochromaticSource")
  {
    libGBP2::MonochromaticSource source;

    SECTION("Setting wavelength")
    {
      source.setWavelength(0.532 * libGBP2::i::um);

      CHECK(source.getWavelength().value() == Approx(532));
      CHECK(source.getWavelength<libGBP2::t::um>().value() == Approx(0.532));

      CHECK(source.getRefractiveIndex().value() == Approx(1));
      CHECK(source.getFrequency<libGBP2::t::MHz>().value() == Approx(563519657.894737));

      SECTION("Setting frequency")
      {
        source.setFrequency(560000000000000. * libGBP2::i::Hz);
        CHECK(source.getFrequency<libGBP2::t::MHz>().value() == Approx(560000000));
        CHECK(source.getRefractiveIndex().value() == Approx(299792458 / (560000000000000 * 532e-9)));
      }

      SECTION("Setting refractive index")
      {
        source.setRefractiveIndex(1.5);
        CHECK(source.getFrequency<libGBP2::t::MHz>().value() == Approx(563519657.894737));
        CHECK(source.getWavelength<libGBP2::t::nm>().value() < 532);
        CHECK(source.getRefractiveIndex().value() == Approx(1.5));
      }
    }
  }
}

TEST_CASE("CircularLaserBeam")
{
  libGBP2::CircularLaserBeam beam;

  beam.setWavelength(633 * libGBP2::i::nm);
  beam.setSecondMomentBeamWaistWidth(100 * libGBP2::i::um);

  CHECK(beam.getSecondMomentBeamWaistWidth<libGBP2::t::cm>().value() == Approx(0.01));
  CHECK(beam.getD4SigmaBeamWaistWidth<libGBP2::t::cm>().value() == Approx(0.02));
  CHECK(beam.getBeamQualityFactor() == Approx(1));
  CHECK(beam.getBeamWaistPosition<libGBP2::t::m>().value() == Approx(0).scale(1));

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
  CHECK(beam.getDiffractionLimitedSecondMomentDivergence().value() == Approx(2.0149));
  CHECK(beam.getDiffractionLimitedD4SigmaDivergence().value() == Approx(2 * 2.0149));
  CHECK(beam.getBeamQualityFactor().value() == Approx(2));

  beam.setDiffractionLimitedSecondMomentDivergence(1.00745 * libGBP2::i::mrad);
  CHECK(beam.getSecondMomentDivergence().value() == Approx(2 * 1.00745));
  CHECK(beam.getD4SigmaDivergence().value() == Approx(2 * 2 * 1.00745));
  CHECK(beam.getDiffractionLimitedSecondMomentDivergence().value() == Approx(1.00745));
  CHECK(beam.getDiffractionLimitedD4SigmaDivergence().value() == Approx(2 * 1.00745));
  CHECK(beam.getBeamQualityFactor().value() == Approx(2));
  CHECK(beam.getSecondMomentBeamWaistWidth<libGBP2::t::mm>().value() == Approx(0.2));
  CHECK(beam.getD4SigmaBeamWaistWidth<libGBP2::t::mm>().value() == Approx(0.4));

  beam.setDiffractionLimitedD4SigmaDivergence(1.00745 * libGBP2::i::mrad);
  CHECK(beam.getSecondMomentDivergence().value() == Approx(2 * 1.00745 / 2));
  CHECK(beam.getD4SigmaDivergence().value() == Approx(2 * 1.00745));
  CHECK(beam.getDiffractionLimitedSecondMomentDivergence().value() == Approx(1.00745 / 2));
  CHECK(beam.getDiffractionLimitedD4SigmaDivergence().value() == Approx(1.00745));
  CHECK(beam.getBeamQualityFactor().value() == Approx(2));
  CHECK(beam.getSecondMomentBeamWaistWidth<libGBP2::t::mm>().value() == Approx(2 * 0.2));
  CHECK(beam.getD4SigmaBeamWaistWidth<libGBP2::t::mm>().value() == Approx(4 * 0.2));
}

TEST_CASE("Convention Conversions")
{
  using namespace libGBP2;

  SECTION("Misc")
  {
    auto d = Conventions::OneOverEDiameter{200 * i::um};

    CHECK(d.value() == Approx(0.02));

    // make sure these all compile
    Conventions::FromOneOverESquaredRadiusCF<Conventions::SecondMomentWidth>();
    Conventions::FromOneOverESquaredRadiusCF<Conventions::D4SigmaWidth>();
    Conventions::FromOneOverESquaredRadiusCF<Conventions::OneOverERadius>();
    Conventions::FromOneOverESquaredRadiusCF<Conventions::OneOverEDiameter>();
    Conventions::FromOneOverESquaredRadiusCF<Conventions::OneOverESquaredRadius>();
    Conventions::FromOneOverESquaredRadiusCF<Conventions::OneOverESquaredDiameter>();
    Conventions::FromOneOverESquaredRadiusCF<Conventions::FWHMRadius>();
    Conventions::FromOneOverESquaredRadiusCF<Conventions::FWHMDiameter>();
    // compiler error, no function specialization for this type
    /* Conventions::FromOneOverESquaredRadiusCF<Conventions::StrongQuantity<t::cm>>(); */
  }

  SECTION("Conversion Factors")
  {
    CHECK((Conventions::BeamWidthConversionFactor<Conventions::D4SigmaWidth, Conventions::D4SigmaWidth>() == Approx(1)));
    CHECK((Conventions::BeamWidthConversionFactor<Conventions::D4SigmaWidth, Conventions::OneOverESquaredRadius>() == Approx(0.5)));
    CHECK((Conventions::BeamWidthConversionFactor<Conventions::FWHMDiameter, Conventions::OneOverESquaredRadius>() == Approx(0.8493281)));
    CHECK((Conventions::BeamWidthConversionFactor<Conventions::OneOverESquaredRadius, Conventions::FWHMDiameter>() == Approx(1 / 0.8493281)));
    CHECK((Conventions::BeamWidthConversionFactor<Conventions::OneOverEDiameter, Conventions::FWHMDiameter>() == Approx(0.8325546111576977)));

    CHECK((Conventions::BeamDivergenceConversionFactor<Conventions::D4SigmaDivergence, Conventions::D4SigmaDivergence>() == Approx(1)));
    CHECK((Conventions::BeamDivergenceConversionFactor<Conventions::D4SigmaDivergence, Conventions::OneOverESquaredHalfAngleDivergence>() == Approx(0.5)));
    CHECK((Conventions::BeamDivergenceConversionFactor<Conventions::FWHMFullAngleDivergence, Conventions::OneOverESquaredHalfAngleDivergence>() == Approx(0.8493281)));
    CHECK((Conventions::BeamDivergenceConversionFactor<Conventions::OneOverESquaredHalfAngleDivergence, Conventions::FWHMFullAngleDivergence>() == Approx(1 / 0.8493281)));
    CHECK((Conventions::BeamDivergenceConversionFactor<Conventions::OneOverEFullAngleDivergence, Conventions::FWHMFullAngleDivergence>() == Approx(0.8325546111576977)));
  }

  SECTION("GaussianBeamWidth")
  {
    GaussianBeamWidth width(Conventions::OneOverEDiameter{2 * i::cm});

    CHECK(width.to<Conventions::OneOverERadius>().quant().value() == Approx(1));

    width = Conventions::OneOverEDiameter{4 * i::cm};
    CHECK(width.to<Conventions::OneOverERadius>().quant().value() == Approx(2));

    Conventions::FWHMDiameter D_fwhm = width.to<Conventions::FWHMDiameter>();
    CHECK(D_fwhm.quant<t::mm>().value() == Approx(sqrt(log(2)) * 40));
  }

  SECTION("GaussianBeamDivergence")
  {
    GaussianBeamDivergence div(Conventions::OneOverEFullAngleDivergence{2 * i::mrad});

    CHECK(div.to<Conventions::OneOverEHalfAngleDivergence>().quant().value() == Approx(1));

    div = Conventions::OneOverEFullAngleDivergence{4 * i::mrad};
    CHECK(div.to<Conventions::OneOverEHalfAngleDivergence>().quant().value() == Approx(2));
    CHECK(div.to<Conventions::OneOverEHalfAngleDivergence>().quant<t::rad>().value() == Approx(2e-3));

    Conventions::FWHMFullAngleDivergence theta_fwhm = div.to<Conventions::FWHMFullAngleDivergence>();
    CHECK(theta_fwhm.quant<t::mrad>().value() == Approx(sqrt(log(2)) * 4));
  }
}

// not sure if we want to allow this or not. if so, we will have to redesign a little
// bit.
/* Conventions::OneOverEDiameter D_1e = D_fwhm; */
/* CHECK(D_fwhm.quant<t::um>().value() == Approx(40000)); */
