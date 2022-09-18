#ifndef GaussianBeam_hpp
#define GaussianBeam_hpp

/** @file GaussianBeam.hpp
 * @brief Class representing a Guassian beam.
 * @author C.D. Clark III
 * @date 06/28/16
 */

#include <complex>
#include <iostream>

#include "./BeamTransformations/BeamTransformation_Interface.hpp"
#include "./LaserBeam.hpp"

/** @class
 * @brief A class implementing Gaussian Beam calculations.
 * @author C.D. Clark III
 *
 * NOTE: This class uses the same convension as "Lasers" by Minlonni and Eberly
 * in which the beam radius/diameter is defined by the 1/e^2 points. This
 * differs from the ANSI standard.
 *
 * @todo: implement methods to get electric field and irradiance at given (r,z)
 * coordinate.
 */
class GaussianLaserBeam : public LaserBeam
{
 protected:
  constexpr const double sqrt2() const { return 1.41421356237; }
  constexpr const double sqrt_ln2() const { return 0.8325546111576977; }

#define ADD_MEMBER(NAME, UNIT)                                               \
 protected:                                                                  \
  boost::units::quantity<UNIT, double> m_##NAME;                             \
                                                                             \
 public:                                                                     \
  template<typename R = UNIT>                                                \
  boost::units::quantity<R> get##NAME() const                                \
  {                                                                          \
    static_assert(std::is_same<typename R::dimension_type,                   \
                               typename UNIT::dimension_type>::value,        \
                  "Dimensions Error: Requested return type for get##NAME() " \
                  "method has wrong dimensions.");                           \
    return boost::units::quantity<R>(m_##NAME);                              \
  }                                                                          \
  template<typename A>                                                       \
  void set##NAME(boost::units::quantity<A> a)                                \
  {                                                                          \
    static_assert(                                                           \
        std::is_same<typename A::dimension_type,                             \
                     typename UNIT::dimension_type>::value,                  \
        "Dimensions Error: Argument type of set##NAME(...) method has "      \
        "wrong dimensions.");                                                \
    m_##NAME = boost::units::quantity<UNIT>(a);                              \
  }

  ADD_MEMBER(CurrentPosition, units::t::cm);
  ADD_MEMBER(WaistPhase, units::t::rad);

#undef ADD_MEMBER
#define ADD_DERIVED_GETTER(NAME, UNIT, ...)                           \
 public:                                                              \
  template<typename R = UNIT>                                         \
  boost::units::quantity<R> get##NAME() const                         \
  {                                                                   \
    static_assert(std::is_same<typename R::dimension_type,            \
                               typename UNIT::dimension_type>::value, \
                  "Dimensions Error: Requested return type for "      \
                  "get##NAME() method "                               \
                  "has wrong dimensions.");                           \
    return boost::units::quantity<R>(__VA_ARGS__);                    \
  }

#define ADD_DERIVED_SETTER(NAME, UNIT, ...)                           \
  template<typename A = UNIT>                                         \
  void set##NAME(boost::units::quantity<A> arg)                       \
  {                                                                   \
    static_assert(std::is_same<typename A::dimension_type,            \
                               typename UNIT::dimension_type>::value, \
                  "Dimensions Error: Argument type of "               \
                  "set##NAME(...) method "                            \
                  "has wrong dimensions.");                           \
    __VA_ARGS__;                                                      \
  }

  // provide getters and setters for all of the common beam width conventions
  //
  // the theory of Gaussian beams is almost always formulated in terms of the beam
  // radius, but we almost always will want to work with the beam diameter. this
  // setter allows the user to set the beam diameter, even through we are storing
  // the radius.
  //
  // Siegman shows that if we use the beam standard deviation, the usual beam propagation equations
  // can be used.
  ADD_DERIVED_GETTER(OneOverESquaredWaistRadius, units::t::cm, 2. * this->getWaistStandardDeviation());
  ADD_DERIVED_SETTER(OneOverESquaredWaistRadius, units::t::cm, this->setWaistStandardDeviation(arg / 2.));
  ADD_DERIVED_GETTER(OneOverESquaredWaistDiameter, units::t::cm, 2. * this->getOneOverESquaredWaistRadius());
  ADD_DERIVED_SETTER(OneOverESquaredWaistDiameter, units::t::cm, this->setOneOverESquaredWaistRadius(arg / 2.));
  ADD_DERIVED_GETTER(OneOverE2WaistRadius, units::t::cm, this->getOneOverESquaredWaistRadius());
  ADD_DERIVED_SETTER(OneOverE2WaistRadius, units::t::cm, this->setOneOverESquaredWaistRadius(arg));
  ADD_DERIVED_GETTER(OneOverE2WaistDiameter, units::t::cm, this->getOneOverESquaredWaistDiameter());
  ADD_DERIVED_SETTER(OneOverE2WaistDiameter, units::t::cm, this->setOneOverESquaredWaistDiameter(arg));

  ADD_DERIVED_GETTER(OneOverEWaistRadius, units::t::cm, this->getOneOverESquaredWaistRadius() / sqrt2());
  ADD_DERIVED_SETTER(OneOverEWaistRadius, units::t::cm, this->setOneOverESquaredWaistRadius(sqrt2() * arg));
  ADD_DERIVED_GETTER(OneOverEWaistDiameter, units::t::cm, this->getOneOverESquaredWaistDiameter() / sqrt2());
  ADD_DERIVED_SETTER(OneOverEWaistDiameter, units::t::cm, this->setOneOverESquaredWaistDiameter(sqrt2() * arg));

  ADD_DERIVED_GETTER(FullWidthHalfMaximumWaistRadius, units::t::cm, this->getOneOverESquaredWaistRadius() * sqrt_ln2() / sqrt2());
  ADD_DERIVED_SETTER(FullWidthHalfMaximumWaistRadius, units::t::cm, this->setOneOverESquaredWaistRadius(arg / sqrt2() / sqrt_ln2()));
  ADD_DERIVED_GETTER(FullWidthHalfMaximumWaistDiameter, units::t::cm, this->getOneOverESquaredWaistDiameter() * sqrt_ln2() / sqrt2());
  ADD_DERIVED_SETTER(FullWidthHalfMaximumWaistDiameter, units::t::cm, this->setOneOverESquaredWaistDiameter(arg* sqrt2() / sqrt_ln2()));

  ADD_DERIVED_GETTER(OneOverESquaredHalfAngleDivergence, units::t::mrad, 2. * this->getAngularSpreadStandardDeviation());
  ADD_DERIVED_SETTER(OneOverESquaredHalfAngleDivergence, units::t::mrad, this->setAngularSpreadStandardDeviation(arg / 2.));
  ADD_DERIVED_GETTER(OneOverESquaredFullAngleDivergence, units::t::mrad, 2. * this->getOneOverESquaredHalfAngleDivergence());
  ADD_DERIVED_SETTER(OneOverESquaredFullAngleDivergence, units::t::mrad, this->setOneOverESquaredHalfAngleDivergence(arg / 2.));

  ADD_DERIVED_GETTER(OneOverE2HalfAngleDivergence, units::t::mrad, 2. * this->getAngularSpreadStandardDeviation());
  ADD_DERIVED_SETTER(OneOverE2HalfAngleDivergence, units::t::mrad, this->setAngularSpreadStandardDeviation(arg / 2.));
  ADD_DERIVED_GETTER(OneOverE2FullAngleDivergence, units::t::mrad, 2. * this->getOneOverESquaredHalfAngleDivergence());
  ADD_DERIVED_SETTER(OneOverE2FullAngleDivergence, units::t::mrad, this->setOneOverESquaredHalfAngleDivergence(arg / 2.));

  ADD_DERIVED_GETTER(OneOverEHalfAngleDivergence, units::t::mrad, this->getOneOverESquaredHalfAngleDivergence() / sqrt2());
  ADD_DERIVED_SETTER(OneOverEHalfAngleDivergence, units::t::mrad, this->setOneOverESquaredHalfAngleDivergence(arg* sqrt2()));
  ADD_DERIVED_GETTER(OneOverEFullAngleDivergence, units::t::mrad, this->getOneOverESquaredFullAngleDivergence() / sqrt2());
  ADD_DERIVED_SETTER(OneOverEFullAngleDivergence, units::t::mrad, this->setOneOverESquaredFullAngleDivergence(arg* sqrt2()));

  ADD_DERIVED_GETTER(OneOverESquaredHalfAngleDiffractionLimitedDivergence, units::t::mrad, 2. * this->getDiffractionLimitedAngularSpreadStandardDeviation());
  ADD_DERIVED_GETTER(OneOverE2HalfAngleDiffractionLimitedDivergence, units::t::mrad, 2. * this->getDiffractionLimitedAngularSpreadStandardDeviation());
  ADD_DERIVED_GETTER(OneOverESquaredFullAngleDiffractionLimitedDivergence, units::t::mrad, 2. * this->getOneOverESquaredHalfAngleDiffractionLimitedDivergence());
  ADD_DERIVED_GETTER(OneOverE2FullAngleDiffractionLimitedDivergence, units::t::mrad, 2. * this->getOneOverE2HalfAngleDiffractionLimitedDivergence());
  ADD_DERIVED_GETTER(OneOverEHalfAngleDiffractionLimitedDivergence, units::t::mrad, this->getOneOverESquaredHalfAngleDiffractionLimitedDivergence() / sqrt2());
  ADD_DERIVED_GETTER(OneOverEFullAngleDiffractionLimitedDivergence, units::t::mrad, this->getOneOverESquaredFullAngleDiffractionLimitedDivergence() / sqrt2());

  ADD_DERIVED_GETTER(RayleighRange, units::t::cm, this->getOneOverESquaredWaistRadius() / this->getOneOverESquaredHalfAngleDivergence<units::t::radian>().value());

#undef ADD_DERIVED_GETTER
#undef ADD_DERIVED_SETTER

#define ADD_DERIVED_GETTER(NAME, UNIT, ...)                                   \
  template<typename R = UNIT, typename A = units::t::cm>                      \
  boost::units::quantity<R> get##NAME(boost::units::quantity<A> z) const      \
  {                                                                           \
    static_assert(                                                            \
        std::is_same<typename R::dimension_type,                              \
                     typename UNIT::dimension_type>::value,                   \
        "Dimensions Error: Requested return type for "                        \
        "get##NAME(...) method "                                              \
        "has wrong dimensions. Should have the same dimensions as " #UNIT);   \
    static_assert(std::is_same<typename A::dimension_type,                    \
                               typename units::t::cm::dimension_type>::value, \
                  "Dimensions Error: argument to get##NAME(...) method "      \
                  "has wrong dimensions.");                                   \
    return boost::units::quantity<R>(__VA_ARGS__);                            \
  }

  ADD_DERIVED_GETTER(OneOverESquaredDiameter, units::t::cm, this->getFourSigmaDiameter(z));
  ADD_DERIVED_GETTER(OneOverESquaredRadius, units::t::cm, this->getOneOverESquaredDiameter(z) / 2);
  ADD_DERIVED_GETTER(OneOverE2Diameter, units::t::cm, this->getOneOverESquaredDiameter(z));
  ADD_DERIVED_GETTER(OneOverE2Radius, units::t::cm, this->getOneOverESquaredRadius(z));
  ADD_DERIVED_GETTER(OneOverEDiameter, units::t::cm, this->getOneOverESquaredDiameter(z) / sqrt2());
  ADD_DERIVED_GETTER(OneOverERadius, units::t::cm, this->getOneOverESquaredRadius(z) / sqrt2());
  ADD_DERIVED_GETTER(FullWidthHalfMaxDiameter, units::t::cm, this->getOneOverESquaredDiameter(z) * sqrt_ln2() / sqrt2());
  ADD_DERIVED_GETTER(FullWidthHalfMaxRadius, units::t::cm, this->getOneOverESquaredRadius(z) * sqrt_ln2() / sqrt2());

  ADD_DERIVED_GETTER(OneOverESquaredArea, decltype(units::i::cm * units::i::cm), M_PI* pow<2>(getOneOverESquaredRadius(z)));
  ADD_DERIVED_GETTER(OneOverE2Area, decltype(units::i::cm * units::i::cm), M_PI* pow<2>(getOneOverE2Radius(z)));
  ADD_DERIVED_GETTER(OneOverEArea, decltype(units::i::cm * units::i::cm), M_PI* pow<2>(getOneOverERadius(z)));
  ADD_DERIVED_GETTER(FullWidthHalfMaxArea, decltype(units::i::cm * units::i::cm), M_PI* pow<2>(getFullWidthHalfMaxRadius(z)));

  ADD_DERIVED_GETTER(RelativeWaistPosition, units::t::cm, this->template getWaistPosition<units::t::cm>() - boost::units::quantity<units::t::cm>(z));
  ADD_DERIVED_GETTER(RadiusOfCurvature, units::t::cm, -getRelativeWaistPosition(z) * (1 + pow<2>(getRayleighRange() / -getRelativeWaistPosition(z))));
  ADD_DERIVED_GETTER(PeakIrradiance, decltype(units::i::W / units::i::cm / units::i::cm), this->getPower() / this->getOneOverEArea(z));
  ADD_DERIVED_GETTER(GouyPhase, units::t::dimensionless, units::t::dimensionless() * atan(boost::units::quantity<units::t::dimensionless>(z / this->getRayleighRange())).value());

#undef ADD_DERIVED_GETTER

#define FORWARD_POSITION_DEPENDENT_METHODS(NAME, UNIT)     \
  template<typename R = UNIT>                              \
  boost::units::quantity<R> get##NAME()                    \
      const                                                \
  {                                                        \
    return this->get##NAME<R>(this->getCurrentPosition()); \
  }

  FORWARD_POSITION_DEPENDENT_METHODS(OneOverESquaredDiameter, units::t::cm);
  FORWARD_POSITION_DEPENDENT_METHODS(OneOverESquaredRadius, units::t::cm);
  FORWARD_POSITION_DEPENDENT_METHODS(OneOverE2Diameter, units::t::cm);
  FORWARD_POSITION_DEPENDENT_METHODS(OneOverE2Radius, units::t::cm);
  FORWARD_POSITION_DEPENDENT_METHODS(OneOverEDiameter, units::t::cm);
  FORWARD_POSITION_DEPENDENT_METHODS(OneOverERadius, units::t::cm);
  FORWARD_POSITION_DEPENDENT_METHODS(FullWidthHalfMaxDiameter, units::t::mrad);
  FORWARD_POSITION_DEPENDENT_METHODS(FullWidthHalfMaxRadius, units::t::mrad);
  FORWARD_POSITION_DEPENDENT_METHODS(OneOverESquaredArea, decltype(units::i::cm * units::i::cm));
  FORWARD_POSITION_DEPENDENT_METHODS(OneOverE2Area, decltype(units::i::cm * units::i::cm));
  FORWARD_POSITION_DEPENDENT_METHODS(OneOverEArea, decltype(units::i::cm * units::i::cm));
  FORWARD_POSITION_DEPENDENT_METHODS(FullWidthHalfMaxArea, decltype(units::i::cm * units::i::cm));
  FORWARD_POSITION_DEPENDENT_METHODS(RadiusOfCurvature, units::t::cm);
  FORWARD_POSITION_DEPENDENT_METHODS(RelativeWaistPosition, units::t::cm);
  FORWARD_POSITION_DEPENDENT_METHODS(PeakIrradiance, decltype(units::i::W / units::i::cm / units::i::cm));
  FORWARD_POSITION_DEPENDENT_METHODS(GouyPhase, units::t::dimensionless);

#undef FORWARD_POSITION_DEPENDENT_METHODS

  /**
   * Computes the complex beam parameter at the position specified.
   */
  template<typename R = units::t::cm, typename A = units::t::cm>
  boost::units::quantity<R, std::complex<double>> getComplexBeamParameter(
      boost::units::quantity<A> z) const
  {
    static_assert(
        std::is_same<typename R::dimension_type,
                     typename units::t::cm::dimension_type>::value,
        "Dimensions Error: Requested return type for "
        "getBeamStandardDeviation(...) method "
        "has wrong dimensions. Should have same dimensions as units::t::cm.");
    static_assert(
        std::is_same<typename A::dimension_type,
                     typename units::t::cm::dimension_type>::value,
        "Dimensions Error: argument to getBeamStandardDeviation(...) method "
        "has wrong dimensions.");
    auto dz = -this->getRelativeWaistPosition<R>(z);

    double real, imag;

    real = dz.value();
    imag = this->getRayleighRange<R>().value();

    return boost::units::quantity<R, std::complex<double>>::from_value(std::complex<double>(real, imag));
  }

  /**
   * Computes the complex beam parameter at the current position.
   */
  template<typename R = units::t::cm, typename A = units::t::cm>
  boost::units::quantity<R, std::complex<double>> getComplexBeamParameter() const
  {
    return this->getComplexBeamParameter<R>(this->getCurrentPosition());
  }

  // OTHER METHODS

  template<typename T, typename U>
  void transform(const BeamTransformation_Interface<T>& elem, U z)
  {
    std::complex<double> qi  = this->getComplexBeamParameter<T>(z).value();
    auto                 RTM = elem.getRTMatrix();
    double               A   = RTM(0, 0);
    double               B   = RTM(0, 1);
    double               C   = RTM(1, 0);
    double               D   = RTM(1, 1);

    std::complex<double> qf = (A * qi + B) / (C * qi + D);

    // q = x + i y = z - i z_R
    //
    // z : distance to the beam waist
    // z_R : the rayleigh range
    //
    // The real part of q is the distance to the beam waist, so its actually
    // \Delta z = z - z_0, where z is the position that q_i is evaluated at.
    //
    // So, z_0 = z - Re{q}
    //
    // The imaginary part of q is the rayliegh range, z_R = \pi \omega_0^2 /
    // \LAMBda
    //
    // So, \omega_0 = \sqrt{ z_R \lambda / \pi }
    //
    // CAREFUL! Make sure to get the units right.
    //

    this->setWavelength(this->getWavelength() * elem.getWavelengthScaleFactor());
    this->setPower(this->getPower() * (1. - elem.getPowerLoss()));

    this->setWaistPosition(boost::units::quantity<T>(z) - qf.real() * T());
    //                             vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    //                             this calculation will give us the 1/e^2 beam
    //                             radius
    this->setOneOverE2WaistRadius(
        sqrt(qf.imag() * this->getWavelength<T>().value() / M_PI) * T());
  }
  template<typename T>
  void transform(const BeamTransformation_Interface<T>& elem)
  {
    this->transform(elem, this->getCurrentPosition());
  }

  template<typename T, typename U>
  void transform(const BeamTransformation_Interface<T>* elem, U z)
  {
    this->transform(*elem, z);
  }

  template<typename T, typename U>
  void transform(const BeamTransformation_Interface<T>* elem)
  {
    this->transform(*elem);
  }
};

using GaussianBeam = GaussianLaserBeam;

#endif
