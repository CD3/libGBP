#pragma once
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
  template<typename U>
  void setBeamWaistWidth(GaussianBeamWidth<U> a_width)
  {
    this->setSecondMomentBeamWaistWidth(
        a_width.template to<Conventions::SecondMomentWidth>().quant());
  }
  /* template<typename C> */
  /* auto setBeamWaistWidth(C a_width) -> decltype(a_width.quant<t::cm>(), void()) */
  /* { */
  /*   this->setSecondMomentBeamWaistWidth( */
  /*       a_width.template to<Conventions::SecondMomentWidth>().quant()); */
  /* } */
  template<typename U = t::cm>
  GaussianBeamWidth<U> getBeamWaistWidth() const
  {
    return GaussianBeamWidth<U>(
        Conventions::SecondMomentWidth{this->getSecondMomentBeamWaistWidth()});
  }
  template<typename U = t::mrad>
  void setBeamDivergence(GaussianBeamDivergence<U> a_div)
  {
  }
  template<typename U = t::mrad>
  GaussianBeamDivergence<U> getBeamDivergence() const
  {
    return GaussianBeamDivergence<U>(
        Conventions::OneOverESquaredHalfAngleDivergence{
            this->getSecondMomentDivergence<t::mrad>()});
  }
  template<typename U = t::mrad>
  GaussianBeamDivergence<U> getDiffractionLimitedBeamDivergence() const
  {
    return GaussianBeamDivergence<t::mrad>(
        Conventions::OneOverESquaredHalfAngleDivergence{
            this->getDiffractionLimitedSecondMomentDivergence<t::mrad>()});
  }

  /**
   * Return the beam width at a given position a_z.
   */
  template<typename UR = t::cm, typename UA = t::cm>
  GaussianBeamWidth<UR> getBeamWidth(quantity<UA> a_z) const
  {
    return GaussianBeamWidth<UR>(Conventions::OneOverESquaredRadius{
        this->getSecondMomentBeamWidth(a_z)});
  }

  /**
   * Return the beam width at z = 0.
   */
  template<typename U = t::cm>
  GaussianBeamWidth<U> getBeamWidth() const
  {
    return GaussianBeamWidth<U>(
        Conventions::OneOverESquaredRadius{this->getSecondMomentBeamWidth()});
  }

  template<typename U = t::cm>
  quantity<U> getRayleighRange() const
  {
    return quantity<U>(this->getSecondMomentBeamWaistWidth<U>() /
                       this->getSecondMomentDivergence<t::rad>().value());
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
   */
  template<typename U1, typename U2>
  void
  setComplexBeamParameter(quantity<U1, std::complex<double>> a_q, quantity<U2> a_z)
  {
    // real part of q is z - z0, so z0 = z - Re{q}
    this->setBeamWaistPosition(quantity<U1>(a_z) - quantity<U1>::from_value(a_q.value().real()));
    // imag part of q is z_R, which is \pi \omega_0^2 / \lambda, so \omega_0 = sqrt(\lambda Im{q} / \pi)
    // NOTE: for Gaussian beam, \omega == second moment width
    this->setSecondMomentBeamWaistWidth(boost::units::root<2>(this->getWavelength() * quantity<U1>::from_value(a_q.value().imag()) / M_PI));
  }
};

}  // namespace libGBP2
