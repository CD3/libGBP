#pragma once
#include <cmath>
#include <complex>

#include "./CircularLaserBeam.hpp"
#include "./Conventions.hpp"

namespace libGBP2
{

/**
 * A class for describing circular Gaussian beams.
 *
 * Multiple beam width and divergence conventions are supported
 * by using the libGBP2::GaussianBeamWidth and libGBP2::GaussianBeamDivergence
 * classes.
 *
 */
class CircularGaussianLaserBeam : public CircularLaserBeam
{
 private:
 public:
  template<typename U, typename C>
  void setBeamWaistWidth(GaussianBeamWidth<C, U> a_width)
  {
    this->setSecondMomentBeamWaistWidth(
        a_width.template get<OneOverESquaredRadius>());
  }
  template<typename U = t::cm, typename C = OneOverESquaredRadius>
  GaussianBeamWidth<C, U> getBeamWaistWidth() const
  {
    return GaussianBeamWidth<OneOverESquaredRadius, U>(this->getSecondMomentBeamWaistWidth());
  }
  template<typename U, typename C>
  void adjustBeamDivergence(GaussianBeamDivergence<C, U> a_div)
  {
    this->adjustSecondMomentDivergence(a_div.template get<OneOverESquaredHalfAngleDivergence>());
  }
  template<typename U = t::mrad, typename C = OneOverESquaredHalfAngleDivergence>
  GaussianBeamDivergence<C, U> getBeamDivergence() const
  {
    // the GaussianBeamDivergence assignemnt constructor will convert to the return type convention
    return GaussianBeamDivergence<OneOverESquaredHalfAngleDivergence, U>(this->getSecondMomentDivergence());
  }
  template<typename U = t::mrad, typename C = OneOverESquaredHalfAngleDivergence>
  GaussianBeamDivergence<C, U> getDiffractionLimitedBeamDivergence() const
  {
    return GaussianBeamDivergence<OneOverESquaredHalfAngleDivergence, U>(this->getDiffractionLimitedSecondMomentDivergence());
  }

  /**
   * Return the beam width at a given position a_z.
   */
  template<typename UR = t::cm, typename C = OneOverESquaredRadius, typename UA = t::cm>
  GaussianBeamWidth<C, UR> getBeamWidth(quantity<UA> a_z) const
  {
    return GaussianBeamWidth<OneOverESquaredRadius, UR>(this->getSecondMomentBeamWidth(a_z));
  }

  /**
   * Return the beam width at z = 0.
   */
  template<typename U = t::cm, typename C = OneOverESquaredRadius>
  GaussianBeamWidth<C, U> getBeamWidth() const
  {
    return GaussianBeamWidth<OneOverESquaredRadius, U>(this->getSecondMomentBeamWidth());
  }

  template<typename U = t::cm>
  quantity<U> getRayleighRange() const
  {
    return quantity<U>(this->getSecondMomentBeamWaistWidth<U>() /
                       this->getSecondMomentDivergence<t::rad>().value());
  }

  template<typename UR = t::cm, typename UA = t::cm>
  quantity<UR> getRadiusOfCurvature(quantity<UA> a_z) const
  {
    quantity<UR> dz = quantity<UR>(a_z) - this->getBeamWaistPosition<UR>();
    return dz * (1 + boost::units::pow<2>(this->getRayleighRange<UR>() / dz).value());
  }

  template<typename UR = t::cm>
  quantity<UR> getRadiusOfCurvature() const
  {
    return this->getRadiusOfCurvature<UR>(0 * i::cm);
  }

  template<typename UR = t::rad, typename UA = t::cm>
  quantity<UR> getGouyPhase(quantity<UA> a_z) const
  {
    quantity<UA> dz = a_z - this->getBeamWaistPosition<UA>();
    return quantity<UR>(atan((dz / this->getRayleighRange<UA>()).value()) * i::rad);
  }

  template<typename UR = t::rad>
  quantity<UR> getGouyPhase() const
  {
    quantity<t::cm> dz = -this->getBeamWaistPosition<t::cm>();
    return quantity<UR>(atan((dz / this->getRayleighRange<t::cm>()).value()) * i::rad);
  }

  /**
   * Compute and return the complex beam parameter at a given position a_z.
   */
  template<typename UR = t::cm, typename UA = t::cm>
  quantity<UR, std::complex<double>>
  getComplexBeamParameter(quantity<UA> a_z) const
  {
    double real, imag;

    real = quantity<UR>(a_z).value() - this->getBeamWaistPosition<UR>().value();
    imag = this->getRayleighRange<UR>().value();

    std::complex<double> val(real, imag);
    return quantity<UR, std::complex<double>>::from_value(val);
  }
  /**
   * Compute and return the complex beam parameter at z = 0.
   */
  template<typename U = t::cm>
  quantity<U, std::complex<double>>
  getComplexBeamParameter() const
  {
    return this->getComplexBeamParameter<U>(0 * i::cm);
  }
  /**
   * Set the complex beam parameter at a given z position a_z.
   *
   * The complex beam parameter at a single position does not uniquely determine
   * a laser beam. There are multiple beams that all have the same q-parameter at a given position.
   *
   * So this function assumes that:
   *
   * 1. the beam width at the given position a_z should be the same before and after the q-parameter is set.
   * 2. the beam quality factor of the laser is fixed.
   *
   * We'll see if this is actually useful...
   *
   */
  template<typename U1, typename U2>
  void setComplexBeamParameter(quantity<U1, std::complex<double>> a_q, quantity<U2> a_z)
  {
    // real part of q is z - z0, so z0 = z - Re{q}
    this->setBeamWaistPosition(quantity<U1>(a_z) - quantity<U1>::from_value(a_q.value().real()));
    // imag part of q is z_R, which is \pi \omega_0^2 / M^2 \lambda, so \omega_0 = sqrt(M^2\lambda Im{q} / \pi)
    // NOTE: for Gaussian beam, \omega == second moment width
    this->setSecondMomentBeamWaistWidth(boost::units::root<2>(this->getBeamQualityFactor<t::dimensionless>().value() * this->getWavelength() * quantity<U1>::from_value(a_q.value().imag()) / M_PI));
  }
  template<typename U>
  void setComplexBeamParameter(quantity<U, std::complex<double>> a_q)
  {
    this->setComplexBeamParameter(a_q, 0 * i::cm);
  }

  /**
   * Return the "embedded" Gaussian beam for this beam.
   *
   * The embedded beam is an ideal Gaussian beam that propagates similar to the
   * real beam, and can be used with ABCD transfer matrices to propagate the real
   * beam through an optical system.
   *
   * For real laser beams, the embedded beam has a beam waist and divergence that are M times smaller
   * than the real beam.
   */
  inline CircularGaussianLaserBeam
  getEmbeddedBeam() const
  {
    CircularGaussianLaserBeam embedded(*this);
    embedded.setSecondMomentBeamWaistWidth(this->getSecondMomentBeamWaistWidth() / boost::units::root<2>(this->getBeamQualityFactor()));
    embedded.setBeamQualityFactor(quantity<t::dimensionless>::from_value(1));
    return embedded;
  }
};

}  // namespace libGBP2
   //
