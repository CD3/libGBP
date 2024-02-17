#pragma once

#include "./MonochromaticSource.hpp"

namespace libGBP2
{
/**
 * A class for describing arbitrary circular beams.
 * Internally, This class uses the "second moment beam width" to quantify
 * the beam size.
 *
 * It is useful because it obeys a universal propagation law
 * (see "How to (Maybe) Measure Laser Beam Quality" by A. Siegman).
 *
 * Seigman calls it a "width", but the second moment
 * beam width is equivalent to the 1/e SQUARED beam RADIUS for a Gaussian beam.
 * So, all of the propagation equations involving Siegman's second moment width
 * are identical to the standard physics Gaussian Beam Propgation equations.
 *
 * In general, it is half of the D4\sigma width, which is an ISO standard for
 * beam width.
 *
 *
 */
class CircularLaserBeam : public MonochromaticSource
{
 private:
  quantity<t::cm>            m_second_moment_beam_waist_width;
  quantity<t::cm>            m_beam_waist_position = 0 * i::cm;
  quantity<t::dimensionless> m_beam_quality_factor = 1 * i::dimensionless;

 public:
  /**
   * Set the second moment width of the beam. This is twice the beam variance
   *
   * $$W = 2\sigma$$
   *
   * and is equivalent to the 1/e SQUARED beam RADIUS for a Gaussian beam.
   *
   * See Siegman "How to (Maybe) Measure Laser Beam Quality" for details.
   */
  template<typename U>
  void
  setSecondMomentBeamWaistWidth(quantity<U> a_val)
  {
    m_second_moment_beam_waist_width = quantity<t::cm>(a_val);
  }

  /**
   * Return the second moment width of the beam. This is twice the beam variance
   *
   * $$W = 2\sigma$$
   *
   * and is equivalent to the 1/e SQUARED beam RADIUS for a Gaussian beam.
   *
   * See Siegman "How to (Maybe) Measure Laser Beam Quality" for details.
   */
  template<typename U = t::cm>
  quantity<U>
  getSecondMomentBeamWaistWidth() const
  {
    return quantity<U>(m_second_moment_beam_waist_width);
  }

  template<typename U>
  void
  setBeamWaistPosition(quantity<U> a_val)
  {
    m_beam_waist_position = quantity<t::cm>(a_val);
  }
  template<typename U = t::cm>
  quantity<U>
  getBeamWaistPosition() const
  {
    return quantity<U>(m_beam_waist_position);
  }

  /**
   * Set the beam quality factor (M^2).
   */
  template<typename U>
  void
  setBeamQualityFactor(quantity<U> a_val)
  {
    m_beam_quality_factor = quantity<t::dimensionless>(a_val);
  }

  /**
   * Return the beam quality factor (M^2).
   */
  template<typename U = t::dimensionless>
  quantity<U>
  getBeamQualityFactor() const
  {
    return quantity<U>(m_beam_quality_factor);
  }

  /**
   * Set the second moment divergence
   * of the beam. This will adjust the beam quality factor
   * to give the specified divergence for the current beam
   * waist size. So, this must be called _after_ the beam waist
   * has been set.
   */
  template<typename U>
  void
  adjustSecondMomentDivergence(quantity<U> a_val)
  {
    this->setBeamQualityFactor(a_val / this->getDiffractionLimitedSecondMomentDivergence());
  }

  /**
   * Return the second moment divergence
   * of the beam. See Siegman for details.
   */
  template<typename U = t::mrad>
  quantity<U>
  getSecondMomentDivergence() const
  {
    return quantity<U>(this->getBeamQualityFactor() * this->getDiffractionLimitedSecondMomentDivergence<U>());
  }

  /**
   * Return the _diffraction limited_ second moment divergence
   * of the beam. The actual divergence will be larger by a factor of M^2.
   */
  template<typename U = t::mrad>
  quantity<U>
  getDiffractionLimitedSecondMomentDivergence() const
  {
    auto val = quantity<t::dimensionless>(this->getWavelength<t::cm>() / this->getSecondMomentBeamWaistWidth<t::cm>() / M_PI) * i::rad;
    return quantity<U>(val);
  }

  /**
   * Set the _diffraction limited_ second moment divergence
   * of the beam. The actual divergence will be larger by a factor of M^2.
   */
  template<typename U = t::mrad>
  void
  setDiffractionLimitedSecondMomentDivergence(quantity<U> a_val)
  {
    m_second_moment_beam_waist_width = this->getWavelength<t::cm>() / quantity<t::rad>(a_val).value() / M_PI;
  }

  /**
   * Set the D4\sigma width of the beam.
   */
  template<typename U>
  void
  setD4SigmaBeamWaistWidth(quantity<U> a_val)
  {
    this->setSecondMomentBeamWaistWidth(a_val / 2);
  }

  /**
   * Return the D4\sigma width of the beam.
   */
  template<typename U = t::cm>
  quantity<U>
  getD4SigmaBeamWaistWidth() const
  {
    return 2 * this->getSecondMomentBeamWaistWidth<U>();
  }

  /**
   * Set the D4\sigma divergence of the beam.
   */
  template<typename U>
  void
  adjustD4SigmaDivergence(quantity<U> a_val)
  {
    this->adjustSecondMomentDivergence(a_val / 2);
  }

  /**
   * Return the D4\sigma divergence of the beam.
   */
  template<typename U = t::mrad>
  quantity<U>
  getD4SigmaDivergence() const
  {
    return 2 * this->getSecondMomentDivergence<U>();
  }

  /**
   * Return the D4\sigma diffraction limited divergence of the beam.
   */
  template<typename U = t::mrad>
  quantity<U>
  getDiffractionLimitedD4SigmaDivergence() const
  {
    return 2 * this->getDiffractionLimitedSecondMomentDivergence<U>();
  }

  /**
   * Set the D4\sigma diffraction limited divergence of the beam.
   */
  template<typename U>
  void
  setDiffractionLimitedD4SigmaDivergence(quantity<U> a_val)
  {
    this->setDiffractionLimitedSecondMomentDivergence(a_val / 2);
  }

  template<typename UR = t::cm, typename UA = t::cm>
  quantity<UR> getSecondMomentBeamWidth(quantity<UA> a_z) const
  {
    return quantity<UR>(

        boost::units::root<2>(
            boost::units::pow<2>(this->getSecondMomentBeamWaistWidth<UA>()) +
            boost::units::pow<2>(this->getSecondMomentDivergence<t::rad>().value()) *
                boost::units::pow<2>(a_z - this->getBeamWaistPosition<UA>())

                ));
  }

  template<typename UR = t::cm>
  quantity<UR> getSecondMomentBeamWidth() const
  {
    return this->getSecondMomentBeamWidth<UR>(0 * i::cm);
  }
};
}  // namespace libGBP2
