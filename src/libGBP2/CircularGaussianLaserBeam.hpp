#pragma once

#include "./CircularLaserBeam.hpp"
#include "./Conventions.hpp"

namespace libGBP2
{

/**
 * A class for describing circular Gaussian beams.
 *
 * Multiple beam width and divergence conventions are supported
 * by using the libGBP2::GaussianBeamWidth and libGBP2::GaussianBeamDivergence classes.
 *
 */
class CircularGaussianLaserBeam : public CircularLaserBeam
{
 private:
 public:
  inline void setBeamWaistWidth(GaussianBeamWidth a_width)
  {
    this->setSecondMomentBeamWaistWidth(a_width.to<Conventions::SecondMomentWidth>().quant());
  }
  inline GaussianBeamWidth getBeamWaistWidth() const
  {
    return GaussianBeamWidth(Conventions::SecondMomentWidth{this->getSecondMomentBeamWaistWidth()});
  }
  inline void                   setBeamDivergence(GaussianBeamDivergence a_div) {}
  inline GaussianBeamDivergence getBeamDivergence() const
  {
    return GaussianBeamDivergence(Conventions::OneOverESquaredHalfAngleDivergence{this->getSecondMomentDivergence<t::mrad>()});
  }
  inline GaussianBeamDivergence getDiffractionLimitedBeamDivergence() const
  {
    return GaussianBeamDivergence(Conventions::OneOverESquaredHalfAngleDivergence{this->getDiffractionLimitedSecondMomentDivergence<t::mrad>()});
  }

  /**
   * Return the beam width at a given position a_z
   */
  template<typename U>
  GaussianBeamWidth getBeamWidth(quantity<U> a_z) const
  {
    return GaussianBeamWidth(Conventions::OneOverESquaredRadius{this->getSecondMomentBeamWidth(a_z)});
  }

  /**
   * Return the beam width at z = 0.
   */
  inline GaussianBeamWidth getBeamWidth() const
  {
    return GaussianBeamWidth(Conventions::OneOverESquaredRadius{this->getSecondMomentBeamWidth()});
  }
};

}  // namespace libGBP2
