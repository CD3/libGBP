#ifndef GaussianBeam_hpp
#define GaussianBeam_hpp

/** @file GaussianBeam.hpp
 * @brief Class representing a Guassian beam.
 * @author C.D. Clark III
 * @date 06/28/16
 */

#include <complex>
#include <iostream>

#include "Constants.hpp"
#include "OpticalElements/OpticalElementInterface.hpp"
#include "Units.hpp"

using std::complex;

static constexpr double sqrt2 = 1.41421356237;
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
class GaussianBeam
{
 public:
 protected:
  // Note on case: we are capitalizing member names here so that
  //               we can use macros to generate the accessor functions.

  quantity<t::hertz> Frequency;  ///< frequency of the light
  quantity<t::nanometer>
                          Wavelength;  ///< wavelength of light in the propagation medium
  quantity<t::centimeter> WaistPosition;  ///< position of the beam waist
  quantity<t::centimeter>
      OneOverE2WaistRadius;  ///< radius (1/e^2) of the beam waist
  quantity<t::radian>
                      WaistPhase;  ///< phase of the electric field at beam waist position.
  quantity<t::watt>   Power;  ///< total power in the beam
  quantity<t::radian> OneOverE2HalfAngleDivergence;  ///< beam divergence
  bool                DiffractionLimitedDivergence =
      true;  ///< flag indicating if the divergence should be compured from the
             ///< defraction limit or not
  quantity<t::centimeter>
      CurrentPosition;  ///< the current position in the beam.

 public:
  // MACROS

  /**
   * this macro creates one setter and two getters for a member quantity.
   * units are handled automatically by the compiler magic (requires C++11
   * compiler).
   */
  // clang-format off
#define ATTRIBUTE_SETTER_AND_GETTER(NAME)\
    typedef decltype(NAME) NAME##Type;\
    typedef NAME##Type::unit_type NAME##Unit;\
    typedef NAME##Type::value_type NAME##Value;\
    \
    template<typename T> void set##NAME(T v) { this->NAME = NAME##Type(v); }\
    template<typename T> quantity<T,NAME##Value> get##NAME() const       { return quantity<T,NAME##Value>(this->NAME); }\
    inline NAME##Type get##NAME() const { return this->get##NAME<NAME##Unit>(); }

  // clang-format on

  /**
   * declares setters and getters for derived parameters (parameters that are
   * not stored directly, but can be calculated from the state). user needs to
   * define non-template versions, and then templated versions that do automatic
   * unit conversion will be created. example:
   *
   * DERIVED_SETTER_AND_GETTER( OneOverE2WaistDiameter, quantity<t::cm> );
   * OneOverE2WaistDiameterType getOneOverE2WaistDiameter() const {...}
   * void setOneOverE2WaistDiameter( OneOverE2WaistDiameterType v ) {...}
   */
  // clang-format off
#define DERIVED_SETTER_AND_GETTER(NAME, Q)\
    typedef Q NAME##Type;\
    typedef NAME##Type::unit_type NAME##Unit;\
    typedef NAME##Type::value_type NAME##Value;\
    \
    template<typename T>\
    void set##NAME(T v) {this->set##NAME( NAME##Type(v) );}\
    template<typename T>\
    quantity<T,NAME##Value> get##NAME() const { return quantity<T,NAME##Value>(this->get##NAME()); }\
    \
    inline NAME##Type get##NAME() const; \
    inline void  set##NAME(NAME##Type v); \
  // clang-format on

  /** declares getter for a derived parameter (parameter that is calculated from
   * member variables). user needs to define non-template version (see
   * DERIVED_SETTER_AND_GETTER above).
   */
  // clang-format off
#define DERIVED_GETTER( NAME, Q )\
    typedef Q NAME##Type;\
    typedef NAME##Type::unit_type NAME##Unit;\
    typedef NAME##Type::value_type NAME##Value;\
    \
    template<typename T>\
    quantity<T,NAME##Value> get##NAME() const {return quantity<T,NAME##Value>(this->get##NAME()); } \
    \
    inline NAME##Type get##NAME() const; \
  // clang-format on

  /**
   * getter for derived parameters that depend on z.
   * user needs to define non-template version, and templated versions that
   * handle unit conversions will be created.
   *
   * example:
   *
   * Z_DEPENDENT_DERIVED_GETTER( OneOverE2Diameter, quantity<t::cm> );
   * OneOverE2DiameterType getOneOverE2Diameter(CurrentPositionType z) const
   * {...}
   *
   *
   */
  // clang-format off
#define Z_DEPENDENT_DERIVED_GETTER( NAME, Q )\
    typedef Q NAME##Type;\
    typedef NAME##Type::unit_type NAME##Unit;\
    typedef NAME##Type::value_type NAME##Value;\
    \
    template<typename T>\
    quantity<T,NAME##Value> get##NAME(   ) const {return this->get##NAME<T>( this->getCurrentPosition());}\
    NAME##Type  get##NAME(   ) const {return this->get##NAME(    this->getCurrentPosition());}\
    template<typename T, typename V>\
    quantity<T,NAME##Value> get##NAME(V z) const {return quantity<T,NAME##Value>( this->get##NAME( CurrentPositionType(z) ));}\
    template<typename V>\
    NAME##Type  get##NAME(V z) const {return NAME##Type(  this->get##NAME( CurrentPositionType(z) ));}\
    \
    inline NAME##Type get##NAME(CurrentPositionType z) const;
  // clang-format on

  // DECLARATIONS

  ATTRIBUTE_SETTER_AND_GETTER(Frequency);
  ATTRIBUTE_SETTER_AND_GETTER(Wavelength);
  ATTRIBUTE_SETTER_AND_GETTER(WaistPosition);
  ATTRIBUTE_SETTER_AND_GETTER(OneOverE2WaistRadius);
  ATTRIBUTE_SETTER_AND_GETTER(WaistPhase);
  ATTRIBUTE_SETTER_AND_GETTER(Power);
  ATTRIBUTE_SETTER_AND_GETTER(CurrentPosition);

  DERIVED_SETTER_AND_GETTER(OneOverE2WaistDiameter, OneOverE2WaistRadiusType);
  DERIVED_SETTER_AND_GETTER(OneOverEWaistRadius, OneOverE2WaistRadiusType);
  DERIVED_SETTER_AND_GETTER(OneOverEWaistDiameter, OneOverE2WaistRadiusType);

  DERIVED_SETTER_AND_GETTER(OneOverE2HalfAngleDivergence,
                            quantity<t::milliradian>);
  DERIVED_SETTER_AND_GETTER(OneOverE2FullAngleDivergence,
                            quantity<t::milliradian>);
  DERIVED_SETTER_AND_GETTER(OneOverEHalfAngleDivergence,
                            quantity<t::milliradian>);
  DERIVED_SETTER_AND_GETTER(OneOverEFullAngleDivergence,
                            quantity<t::milliradian>);

  DERIVED_GETTER(BeamQualityFactor, quantity<t::dimensionless>);
  DERIVED_GETTER(OneOverE2HalfAngleDiffractionLimitedDivergence,
                 quantity<t::milliradian>);

  DERIVED_GETTER(FreeSpaceWavelength, WavelengthType);
  DERIVED_GETTER(RayleighRange, OneOverE2WaistRadiusType);
  DERIVED_GETTER(DiffractionLimitedOneOverE2HalfAngleDivergence,
                 quantity<t::milliradian>);

  Z_DEPENDENT_DERIVED_GETTER(OneOverE2Radius, OneOverE2WaistRadiusType);
  Z_DEPENDENT_DERIVED_GETTER(OneOverERadius, OneOverE2WaistRadiusType);
  Z_DEPENDENT_DERIVED_GETTER(OneOverE2Diameter, OneOverE2WaistRadiusType);
  Z_DEPENDENT_DERIVED_GETTER(OneOverEDiameter, OneOverE2WaistRadiusType);
  Z_DEPENDENT_DERIVED_GETTER(RadiusOfCurvature, WaistPositionType);
  Z_DEPENDENT_DERIVED_GETTER(RelativeWaistPosition, WaistPositionType);
  Z_DEPENDENT_DERIVED_GETTER(OneOverE2Area, quantity<t::centimeter_squared>);
  Z_DEPENDENT_DERIVED_GETTER(OneOverEArea, quantity<t::centimeter_squared>);
  Z_DEPENDENT_DERIVED_GETTER(PeakIrradiance,
                             quantity<t::watt_per_centimeter_squared>);
  Z_DEPENDENT_DERIVED_GETTER(GouyPhase, quantity<t::radian>);
  typedef quantity<WaistPositionUnit, complex<double> > ComplexLengthType;
  Z_DEPENDENT_DERIVED_GETTER(ComplexBeamParameter, ComplexLengthType);
  // typedef quantity<t::volt_per_meter, complex<double> >
  // ComplexElectricFieldType; Z_DEPENDENT_DERIVED_GETTER( ElectricField,
  // ComplexElectricFieldType );

  void setDiffractionLimitedDivergence(bool val);
  bool getDiffractionLimitedDivergence() const;

  // OTHER METHODS

  template<typename T, typename U>
  void transform(OpticalElementInterface<T>* elem, U z);
  template<typename T>
  void transform(OpticalElementInterface<T>* elem)
  {
    this->transform(elem, this->getCurrentPosition());
  }
};

// the theory of Gaussian beams is almost always formulated in terms of the beam
// radius, but we almost always will want to work with the beam diameter. this
// setter allows the user to set the beam diameter, even through we are storing
// the radius.
void GaussianBeam::setOneOverE2WaistDiameter(OneOverE2WaistDiameterType v)
{
  this->setOneOverE2WaistRadius(OneOverE2WaistRadiusType(v) / 2.);
}

GaussianBeam::OneOverE2WaistDiameterType
GaussianBeam::getOneOverE2WaistDiameter() const
{
  return 2. * this->getOneOverE2WaistRadius();
}

void GaussianBeam::setOneOverEWaistRadius(OneOverEWaistRadiusType v)
{
  this->setOneOverE2WaistRadius(sqrt2 * OneOverE2WaistRadiusType(v));
}

GaussianBeam::OneOverEWaistRadiusType GaussianBeam::getOneOverEWaistRadius()
    const
{
  return this->getOneOverE2WaistRadius() / sqrt2;
}

void GaussianBeam::setOneOverEWaistDiameter(OneOverEWaistDiameterType v)
{
  this->setOneOverE2WaistRadius(OneOverE2WaistRadiusType(v) / sqrt2);
}

GaussianBeam::OneOverEWaistDiameterType GaussianBeam::getOneOverEWaistDiameter()
    const
{
  return this->getOneOverE2WaistRadius() * sqrt2;
}

void GaussianBeam::setDiffractionLimitedDivergence(bool val)
{
  this->DiffractionLimitedDivergence = val;
}

bool GaussianBeam::getDiffractionLimitedDivergence() const
{
  return this->DiffractionLimitedDivergence;
}

GaussianBeam::FreeSpaceWavelengthType GaussianBeam::getFreeSpaceWavelength()
    const
{
  auto val = constants::SpeedOfLight / this->getFrequency<t::hertz>();

  return FreeSpaceWavelengthType(val);
}

GaussianBeam::RayleighRangeType GaussianBeam::getRayleighRange() const
{
  auto val = this->getOneOverE2WaistRadius() /
             this->getOneOverE2HalfAngleDivergence<t::radian>().value();
  return RayleighRangeType(val);
}

GaussianBeam::BeamQualityFactorType GaussianBeam::getBeamQualityFactor() const
{
  if (this->DiffractionLimitedDivergence) {
    return BeamQualityFactorType::from_value(1);
  }

  return BeamQualityFactorType(
      OneOverE2HalfAngleDivergence /
      getOneOverE2HalfAngleDiffractionLimitedDivergence());
}

GaussianBeam::OneOverE2HalfAngleDivergenceType
GaussianBeam::getOneOverE2HalfAngleDiffractionLimitedDivergence() const
{
  auto val = getWavelength<t::nanometer>() /
             (M_PI * this->getOneOverE2WaistRadius<t::nanometer>()) *
             t::radian();

  return OneOverE2HalfAngleDiffractionLimitedDivergenceType(val);
}

void GaussianBeam::setOneOverE2HalfAngleDivergence(
    OneOverE2HalfAngleDivergenceType val)
{
  this->OneOverE2HalfAngleDivergence = quantity<t::radian>(val);
  this->DiffractionLimitedDivergence = false;
}

GaussianBeam::OneOverE2HalfAngleDivergenceType
GaussianBeam::getOneOverE2HalfAngleDivergence() const
{
  if (this->DiffractionLimitedDivergence)
    return this->getOneOverE2HalfAngleDiffractionLimitedDivergence();
  return OneOverE2HalfAngleDivergenceType(this->OneOverE2HalfAngleDivergence);
}

void GaussianBeam::setOneOverE2FullAngleDivergence(
    OneOverE2FullAngleDivergenceType val)
{
  this->setOneOverE2HalfAngleDivergence(val / 2.);
}

GaussianBeam::OneOverE2FullAngleDivergenceType
GaussianBeam::getOneOverE2FullAngleDivergence() const
{
  return 2. * getOneOverE2HalfAngleDivergence();
}

void GaussianBeam::setOneOverEHalfAngleDivergence(
    OneOverEHalfAngleDivergenceType val)
{
  this->setOneOverE2HalfAngleDivergence(val * sqrt2);
}

GaussianBeam::OneOverEHalfAngleDivergenceType
GaussianBeam::getOneOverEHalfAngleDivergence() const
{
  return getOneOverE2HalfAngleDivergence() / sqrt2;
}

void GaussianBeam::setOneOverEFullAngleDivergence(
    OneOverEFullAngleDivergenceType val)
{
  this->setOneOverEHalfAngleDivergence(val / 2.);
}

GaussianBeam::OneOverEFullAngleDivergenceType
GaussianBeam::getOneOverEFullAngleDivergence() const
{
  return getOneOverEHalfAngleDivergence() * 2.;
}

GaussianBeam::OneOverE2RadiusType GaussianBeam::getOneOverE2Radius(
    CurrentPositionType z) const
{
  auto dz = -getRelativeWaistPosition(z);

  auto val =
      getOneOverE2WaistRadius() *
      root<2>(1 +
              pow<2>(getOneOverE2HalfAngleDivergence<t::radian>().value() *
                     OneOverE2WaistRadiusType(dz) / getOneOverE2WaistRadius()));

  return OneOverE2RadiusType(val);
}

GaussianBeam::OneOverE2DiameterType GaussianBeam::getOneOverE2Diameter(
    CurrentPositionType z) const
{
  return OneOverE2DiameterType(2. * getOneOverE2Radius(z));
}

GaussianBeam::OneOverERadiusType GaussianBeam::getOneOverERadius(
    CurrentPositionType z) const
{
  return OneOverERadiusType(getOneOverE2Radius(z) / sqrt2);
}

GaussianBeam::OneOverEDiameterType GaussianBeam::getOneOverEDiameter(
    CurrentPositionType z) const
{
  return OneOverEDiameterType(2. * getOneOverERadius(z));
}

GaussianBeam::RadiusOfCurvatureType GaussianBeam::getRadiusOfCurvature(
    CurrentPositionType z) const
{
  auto dz = -getRelativeWaistPosition(z);

  auto val = dz * (1 + pow<2>(getRayleighRange() / dz));

  return RadiusOfCurvatureType(val);
}

GaussianBeam::ComplexBeamParameterType GaussianBeam::getComplexBeamParameter(
    CurrentPositionType z) const
{
  auto dz = -getRelativeWaistPosition<ComplexBeamParameterUnit>(z);

  double real, imag;

  real = dz.value();
  imag = getRayleighRange<ComplexBeamParameterUnit>().value();

  return ComplexBeamParameterType::from_value(complex<double>(real, imag));
}

GaussianBeam::RelativeWaistPositionType GaussianBeam::getRelativeWaistPosition(
    CurrentPositionType z) const
{
  auto val = RelativeWaistPositionType(getWaistPosition()) -
             RelativeWaistPositionType(z);

  return val;
}

GaussianBeam::OneOverE2AreaType GaussianBeam::getOneOverE2Area(
    CurrentPositionType z) const
{
  auto val = M_PI * pow<2>(getOneOverE2Radius(z));

  return OneOverE2AreaType(val);
}

GaussianBeam::OneOverEAreaType GaussianBeam::getOneOverEArea(
    CurrentPositionType z) const
{
  auto val = M_PI * pow<2>(getOneOverERadius(z));

  return OneOverEAreaType(val);
}

GaussianBeam::PeakIrradianceType GaussianBeam::getPeakIrradiance(
    CurrentPositionType z) const
{
  // Note: peak irradiance is P/A **when using 1/e beam diameter**.
  //       for 1/e2 diameter, it will be  2*P/A.
  auto val = getPower() / getOneOverEArea(z);

  return PeakIrradianceType(val);
}

GaussianBeam::GouyPhaseType GaussianBeam::getGouyPhase(
    CurrentPositionType z) const
{
  auto val = atan(z / getRayleighRange<CurrentPositionUnit>());

  return GouyPhaseType(val);
}

template<typename T, typename U>
void GaussianBeam::transform(OpticalElementInterface<T>* elem, U z)
{
  complex<double> qi  = this->getComplexBeamParameter<T>(z).value();
  auto            RTM = elem->getRTMatrix();
  double          A   = RTM(0, 0);
  double          B   = RTM(0, 1);
  double          C   = RTM(1, 0);
  double          D   = RTM(1, 1);

  complex<double> qf = (A * qi + B) / (C * qi + D);

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
  // \lambda
  //
  // So, \omega_0 = \sqrt{ z_R \lambda / \pi }
  //
  // CAREFUL! Make sure to get the units right.
  //

  this->setWavelength(this->getWavelength() * elem->getWavelengthScaleFactor());
  this->setPower(this->getPower() * (1. - elem->getPowerLoss()));

  this->setWaistPosition(quantity<T>(z) - qf.real() * T());
  //                             vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
  //                             this calculation will give us the 1/e^2 beam
  //                             radius
  this->setOneOverE2WaistRadius(
      sqrt(qf.imag() * this->getWavelength<T>().value() / M_PI) * T());
}

#endif
