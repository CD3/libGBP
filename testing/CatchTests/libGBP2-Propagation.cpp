#include "catch.hpp"

#include <BoostUnitDefinitions/Units.hpp>

#include <libGBP2/CircularGaussianLaserBeam.hpp>
#include <libGBP2/CircularLaserBeam.hpp>
#include <libGBP2/Conventions.hpp>
#include <libGBP2/MonochromaticSource.hpp>
#include <libGBP2/OpticalElements/FlatRefractiveSurface.hpp>
#include <libGBP2/OpticalElements/FreeSpace.hpp>
#include <libGBP2/OpticalElements/OpticalElement.hpp>
#include <libGBP2/OpticalElements/SphericalRefractiveSurface.hpp>
#include <libGBP2/OpticalElements/ThickLens.hpp>
#include <libGBP2/OpticalElements/ThinLens.hpp>

TEST_CASE("OpticalElement")
{
  using namespace libGBP2;
  SECTION("Ray Transfer Matrix")
  {
    OpticalElement<t::mm> element1;
    auto                  mat = element1.getRayTransferMatrix();
    CHECK(mat(0, 0) == Approx(1));
    CHECK(mat(0, 1) == Approx(0).scale(1));
    CHECK(mat(1, 0) == Approx(0).scale(1));
    CHECK(mat(1, 1) == Approx(1));

    mat = element1.getRayTransferMatrix<t::m>();
    CHECK(mat(0, 0) == Approx(1));
    CHECK(mat(0, 1) == Approx(0).scale(1));
    CHECK(mat(1, 0) == Approx(0).scale(1));
    CHECK(mat(1, 1) == Approx(1));

    element1.setA(element1.getA());
    element1.setB(2 * i::m);
    element1.setC(2 * i::cm_n1);
    element1.setD(element1.getD());

    mat = element1.getRayTransferMatrix();
    CHECK(mat(0, 0) == Approx(1));
    CHECK(mat(0, 1) == Approx(2000));
    CHECK(mat(1, 0) == Approx(0.2));
    CHECK(mat(1, 1) == Approx(1));

    mat = element1.getRayTransferMatrix<t::m>();
    CHECK(mat(0, 0) == Approx(1));
    CHECK(mat(0, 1) == Approx(2));
    CHECK(mat(1, 0) == Approx(200));
    CHECK(mat(1, 1) == Approx(1));

    mat = element1.getRayTransferMatrix<t::cm>();
    CHECK(mat(0, 0) == Approx(1));
    CHECK(mat(0, 1) == Approx(200));
    CHECK(mat(1, 0) == Approx(2));
    CHECK(mat(1, 1) == Approx(1));
  }

  SECTION("Multiply and Assign")
  {
    SECTION("Homogeneous Units")
    {
      OpticalElement<t::cm> element1, element2, element3, element4;
      element1.setRefractiveIndexScale(1.33 * i::dimensionless);
      element2.setDisplacement(2 * i::cm);
      element3.setRefractiveIndexScale((1.5 / 1.33) * i::dimensionless);
      element3.setDisplacement(3.5 * i::mm);
      element4.setRefractiveIndexScale((1 / 1.5) * i::dimensionless);

      element1.setB(1 * i::m);

      OpticalElement<t::cm> system = element1 * element2 * element3 * element4;

      CHECK(system.getDisplacement<t::cm>().value() == Approx(2.35));
      CHECK(system.getDisplacement<t::mm>().value() == Approx(23.5));
      CHECK(system.getRefractiveIndexScale().value() == Approx(1));

      auto mat = system.getRayTransferMatrix();
      CHECK(mat(0, 0) == Approx(1));
      CHECK(mat(0, 1) == Approx(100));
      CHECK(mat(1, 0) == Approx(0).scale(1));
      CHECK(mat(1, 1) == Approx(1));
      mat = system.getRayTransferMatrix<t::m>();
      CHECK(mat(0, 0) == Approx(1));
      CHECK(mat(0, 1) == Approx(1));
      CHECK(mat(1, 0) == Approx(0).scale(1));
      CHECK(mat(1, 1) == Approx(1));

      SECTION("Assignment")
      {
        element2 = element1;

        CHECK(element2.getDisplacement().value() == Approx(0).scale(1));
        CHECK(element2.getRefractiveIndexScale().value() ==
              Approx(1.33));
        CHECK(element2.getA().value() == Approx(1));
        CHECK(element2.getD().value() == Approx(1));
        CHECK(element2.getB().value() == Approx(100));
        CHECK(element2.getC().value() == Approx(0).scale(1));
      }
    }
    SECTION("Mixed Units")
    {
      OpticalElement<t::mm> element1, element2;
      OpticalElement<t::m>  element3, element4;
      element1.setRefractiveIndexScale(1.33 * i::dimensionless);
      element2.setDisplacement(2 * i::cm);
      element3.setRefractiveIndexScale((1.5 / 1.33) * i::dimensionless);
      element3.setDisplacement(3.5 * i::mm);
      element4.setRefractiveIndexScale((1 / 1.5) * i::dimensionless);

      element1.setB(1 * i::m);

      OpticalElement<t::cm> system = element1 * element2 * element3 * element4;

      CHECK(system.getDisplacement<t::cm>().value() == Approx(2.35));
      CHECK(system.getDisplacement<t::mm>().value() == Approx(23.5));
      CHECK(system.getRefractiveIndexScale().value() == Approx(1));

      auto mat = system.getRayTransferMatrix();
      CHECK(mat(0, 0) == Approx(1));
      CHECK(mat(0, 1) == Approx(100));
      CHECK(mat(1, 0) == Approx(0).scale(1));
      CHECK(mat(1, 1) == Approx(1));
      /* mat = system.getRayTransferMatrix<t::m>(); */
      /* CHECK(mat(0, 0) == Approx(1)); */
      /* CHECK(mat(0, 1) == Approx(1)); */
      /* CHECK(mat(1, 0) == Approx(0).scale(1)); */
      /* CHECK(mat(1, 1) == Approx(1)); */

      SECTION("Assignment")
      {
        element3 = element1;

        CHECK(element3.getDisplacement().value() == Approx(0).scale(1));
        CHECK(element3.getRefractiveIndexScale().value() ==
              Approx(1.33));
        CHECK(element3.getA().value() == Approx(1));
        CHECK(element3.getD().value() == Approx(1));
        CHECK(element3.getB().value() == Approx(1));
        CHECK(element3.getC().value() == Approx(0).scale(1));
      }
    }
  }
}

TEST_CASE("Optical Element Transformations")
{
  using namespace libGBP2;

  CircularGaussianLaserBeam beam;

  SECTION("Focused Beam")
  {
    beam.setWavelength(532 * i::nm);
    beam.setBeamWaistWidth(make_width<OneOverESquaredRadius>(20 * i::um));
    beam.setBeamWaistPosition(1.5 * i::m);

    CHECK(beam.getBeamWidth<t::mm>().get<OneOverEDiameter>().value() == Approx(12.7005 * sqrt(2)));
    CHECK(beam.getBeamWidth().get<OneOverEDiameter>().value() == Approx(1.27005 * sqrt(2)));

    SECTION("Free Space")
    {
      FreeSpace free_space(1.5 * i::m);

      CHECK(free_space.getLength<t::mm>().value() == Approx(1500));

      auto q = beam.getComplexBeamParameter();
      q      = free_space * q;
      beam.setComplexBeamParameter(q);

      CHECK(beam.getBeamWidth().get<OneOverEDiameter>().value() == Approx(20e-4 * sqrt(2)));
      CHECK(beam.getBeamWidth().get<OneOverESquaredRadius>().value() == Approx(20e-4));
      CHECK(beam.getBeamWidth(1.5 * i::m).get<OneOverESquaredRadius>().value() == Approx(1.27005));
      CHECK(beam.getBeamWidth(-1.5 * i::m).get<OneOverESquaredRadius>().value() == Approx(1.27005));
    }

    SECTION("Flat refractive surface")
    {
      FlatRefractiveSurface surface(1.5 * i::dimensionless);

      CHECK(beam.getBeamWaistWidth().get<OneOverESquaredRadius>().value() == Approx(20e-4));
      auto div = beam.getBeamDivergence().get<OneOverESquaredHalfAngleDivergence>().value();

      SECTION("No free space")
      {
        auto q = beam.getComplexBeamParameter();
        q      = surface * q;
        beam.setRefractiveIndex(1.5 * i::dimensionless);
        beam.setComplexBeamParameter(q);

        CHECK(beam.getBeamWaistWidth().get<OneOverESquaredRadius>().value() == Approx(20e-4));
        CHECK(beam.getBeamDivergence().get<OneOverESquaredHalfAngleDivergence>().value() == Approx(div / 1.5));
        CHECK(beam.getBeamWaistPosition<t::m>().value() == Approx(1.5 * 1.5));
      }
      SECTION("with free space")
      {
        FreeSpace free_space(1.5 * 1.5 * i::m);

        SECTION("With shifted reference frame")
        {
          auto q = beam.getComplexBeamParameter();
          q      = free_space * surface * q;
          beam.setRefractiveIndex(beam.getRefractiveIndex() * surface.getRefractiveIndexScale());
          beam.setComplexBeamParameter(q);

          CHECK(beam.getBeamWaistWidth().get<OneOverESquaredRadius>().value() == Approx(20e-4));
          CHECK(beam.getBeamWidth().get<OneOverESquaredRadius>().value() == Approx(20e-4));
          CHECK(beam.getBeamDivergence().get<OneOverESquaredHalfAngleDivergence>().value() == Approx(div / 1.5));
          CHECK(beam.getBeamWaistPosition().value() == Approx(0).scale(1));
        }
        SECTION("Without shifted reference frame")
        {
          auto q = beam.getComplexBeamParameter();
          q      = free_space * surface * q;
          beam.setRefractiveIndex(beam.getRefractiveIndex() * surface.getRefractiveIndexScale());  // this needs to be set BEFORE setting q
          beam.setComplexBeamParameter(q);
          beam.setBeamWaistPosition(beam.getBeamWaistPosition<t::m>() + free_space.getDisplacement<t::m>());  // this needs to be set AFTER settign Q
                                                                                                              // it will move the reference position *back* to where it was
                                                                                                              // it is kind of pointless here, but we might want to do it for a thick lens,
                                                                                                              // or multiple surfaces separated by some distance.

          CHECK(beam.getBeamWaistWidth().get<OneOverESquaredRadius>().value() == Approx(20e-4));
          CHECK(beam.getBeamDivergence().get<OneOverESquaredHalfAngleDivergence>().value() == Approx(div / 1.5));
          CHECK(beam.getBeamWaistPosition<t::m>().value() == Approx(1.5 * 1.5));
        }
      }
    }
  }

  SECTION("Collimated Beam")
  {
    beam.setWavelength(532 * i::nm);
    beam.setBeamWaistWidth(make_width<OneOverESquaredDiameter>(2 * i::mm));
    beam.setBeamWaistPosition(1.5 * i::m);

    SECTION("Through Thin Lens")
    {
      ThinLens thin_lens(50 * i::mm);
      CHECK(thin_lens.getFocalLength<t::cm>().value() == Approx(5));
      CHECK(thin_lens.getRayTransferMatrix()(0, 0) == Approx(1));
      CHECK(thin_lens.getRayTransferMatrix()(0, 1) == Approx(0).scale(1));
      CHECK(thin_lens.getRayTransferMatrix()(1, 0) == Approx(-0.2));
      CHECK(thin_lens.getRayTransferMatrix()(1, 1) == Approx(1));

      auto q = beam.getComplexBeamParameter(1.5 * i::m);  // other side of beam waist
      q      = thin_lens * q;
      beam.setComplexBeamParameter(q, 1.5 * i::m);

      CHECK(beam.getBeamWaistWidth<t::um>().get<OneOverESquaredDiameter>().value() == Approx(16.93).epsilon(0.01));
      CHECK(beam.getBeamWaistPosition<t::mm>().value() == Approx(1550));
    }

    SECTION("free space -> thin lens -> free space")
    {
      ThinLens  thin_lens(50 * i::mm);
      FreeSpace free_space_1(1.5 * i::m);
      FreeSpace free_space_2(50 * i::mm);

      auto q = beam.getComplexBeamParameter();
      q      = (free_space_2 * thin_lens * free_space_1) * q;
      beam.setComplexBeamParameter(q);

      CHECK(beam.getBeamWaistWidth<t::um>().get<OneOverESquaredDiameter>().value() == Approx(16.93).epsilon(0.01));
      CHECK(beam.getBeamWidth<t::um>().get<OneOverESquaredDiameter>().value() == Approx(16.93).epsilon(0.01));
      CHECK(beam.getBeamWaistPosition<t::mm>().value() == Approx(0).scale(1).epsilon(0.005));
    }

    SECTION("Spherical refractive surface")
    {
      SphericalRefractiveSurface front_surface(1.5 * i::dimensionless, 10 * i::inch);
      SphericalRefractiveSurface back_surface(1 / 1.5 * i::dimensionless, -10 * i::inch);

      auto element = back_surface * front_surface;
      auto mat     = element.getRayTransferMatrix<t::inch>();

      CHECK(mat(0, 0) == Approx(1));
      CHECK(mat(0, 1) == Approx(0).scale(1));
      CHECK(mat(1, 0) == Approx(-0.1));
      CHECK(mat(1, 1) == Approx(1));

      front_surface.setRefractiveIndexScaleFactorAndRadiusOfCurvature(1.5 * i::dimensionless, 10 * i::cm);  // f = 30 cm
      auto q = beam.getComplexBeamParameter(1.5 * i::m);
      q      = front_surface * q;
      beam.setRefractiveIndex(beam.getRefractiveIndex() * front_surface.getRefractiveIndexScale());  // this needs to be set BEFORE setting q
      beam.setComplexBeamParameter(q, 1.5 * i::m);

      CHECK(beam.getBeamWaistPosition<t::cm>().value() == Approx(150 + 30).epsilon(0.001));
    }

    SECTION("Thick lens")
    {
      ThickLens thick_lens(1.5 * i::dimensionless, 10 * i::cm, 0 * i::mm, 100 * i::mm);
      ThinLens  thin_lens(10 * i::cm);

      auto mat_thick = thick_lens.getRayTransferMatrix();
      auto mat_thin  = thin_lens.getRayTransferMatrix();

      CHECK(mat_thick(0, 0) == Approx(mat_thick(0, 0)));
      CHECK(mat_thick(0, 1) == Approx(mat_thick(0, 1)).scale(1));
      CHECK(mat_thick(1, 0) == Approx(mat_thick(1, 0)));
      CHECK(mat_thick(1, 1) == Approx(mat_thick(1, 1)));

      CHECK(thin_lens.getRefractiveIndexScale().value() == Approx(1));
      CHECK(thick_lens.getRefractiveIndexScale().value() == Approx(1));

      CHECK(thin_lens.getDisplacement().value() == Approx(0).scale(1));
      CHECK(thick_lens.getDisplacement().value() == Approx(0).scale(1));

      thick_lens.setLensParameters(1.5 * i::dimensionless, 10 * i::cm, 1 * i::in, 100 * i::mm);
      CHECK(thick_lens.getDisplacement().value() == Approx(2.54));
    }
  }
}
