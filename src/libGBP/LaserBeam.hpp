#pragma once

/** @file LaserBeam.hpp
 * @brief
 * @author C.D. Clark III
 * @date 07/13/20
 */

#include <type_traits>

#include "Constants.hpp"
#include "Units.hpp"

class LaserBeam
{
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

  ADD_MEMBER(Wavelength, t::nm);
  ADD_MEMBER(Frequency, t::Hz);
  ADD_MEMBER(Power, t::W);
  ADD_MEMBER(WaistStandardDeviation, t::cm);
  ADD_MEMBER(WaistPosition, t::cm);

 protected:
  bool m_UseDiffractionLimitedDivergence = true;
 public:
  inline void setUseDiffractionLimitedDivergence(bool val){m_UseDiffractionLimitedDivergence=val;}
  inline bool getUseDiffractionLimitedDivergence() const{return m_UseDiffractionLimitedDivergence;}

 protected:
  boost::units::quantity<t::mrad,double> m_AngularSpreadStandardDeviation;
 public:
  template<typename R = t::mrad> 
  boost::units::quantity<R> getAngularSpreadStandardDeviation() const
  {                                                                 
    static_assert(std::is_same<typename R::dimension_type,         
                               typename t::mrad::dimension_type>::value,        
                  "Dimensions Error: Requested return type for getAngularSpreadStandardDeviation() " 
                  "method has wrong dimensions.");                          
    if(m_UseDiffractionLimitedDivergence)
    {
      return this->getDiffractionLimitedAngularSpreadStandardDeviation<R>();
    }
    return boost::units::quantity<R>(m_AngularSpreadStandardDeviation);                            
  }                                                                       
  template<typename A>                                                   
  void setAngularSpreadStandardDeviation(boost::units::quantity<A> a)                           
  {                                                                    
    static_assert(                                                        
        std::is_same<typename A::dimension_type,                         
                     typename t::mrad::dimension_type>::value,             
        "Dimensions Error: Argument type of setAngularSpreadStandardDeviation(...) method has "
        "wrong dimensions.");                                         
    m_AngularSpreadStandardDeviation = boost::units::quantity<t::mrad>(a);                      
    m_UseDiffractionLimitedDivergence = false;
  }

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

#define ADD_DERIVED_SETTER(NAME, UNIT, ...)                            \
  template<typename A = UNIT>                                          \
  void set##NAME(boost::units::quantity<A> arg)                        \
  {                                                                    \
    static_assert(std::is_same<typename A::dimension_type,             \
                               typename t::nm::dimension_type>::value, \
                  "Dimensions Error: Argument type of "                \
                  "set##NAME(...) method "                             \
                  "has wrong dimensions.");                            \
    __VA_ARGS__;                                                       \
  }

  ADD_DERIVED_GETTER(FreeSpaceWavelength, t::nm,
                     constants::SpeedOfLight / this->getFrequency<t::hertz>());
  ADD_DERIVED_SETTER(FreeSpaceWavelength, t::nm,
                     this->setFrequency(constants::SpeedOfLight / arg));
  ADD_DERIVED_GETTER(WaistFourSigmaDiameter, t::cm,
                     this->getWaistStandardDeviation() * 4.);
  ADD_DERIVED_SETTER(WaistFourSigmaDiameter, t::cm,
                     this->setWaistStandardDeviation(arg / 4.));
  /**
   * Computes the M^2 beam propagation factor (sometimes called the "beam
   * quality") from the beam divergence and beam waist size.
   */
  ADD_DERIVED_GETTER(
      BeamPropagationFactor, t::dimensionless,
      this->getAngularSpreadStandardDeviation<t::rad>() /
          this->getDiffractionLimitedAngularSpreadStandardDeviation<t::rad>());
#undef ADD_DERIVED_GETTER
#undef ADD_DERIVED_SETTER

  /**
   * Computes the diffraction limited second-moment divergence.
   *
   * Requires that WaistStandardDeviation and Wavelength are both set.
   */
  template<typename R = t::mrad>
  boost::units::quantity<R>
  getDiffractionLimitedAngularSpreadStandardDeviation() const
  {
    static_assert(
        std::is_same<typename R::dimension_type,
                     typename t::mrad::dimension_type>::value,
        "Dimensions Error: Requested return type for "
        "getDiffractionLimitedAngularSpreadStandardDeviation() method "
        "has wrong dimensions.");
    auto sigma_theta = boost::units::quantity<t::dimensionless>(
                           this->getWavelength() / (4 * M_PI) /
                           this->getWaistStandardDeviation()) *
                       i::rad;
    return boost::units::quantity<R>(sigma_theta);
  }

  /**
   * Computes the diffraction limited waist standard deviation.
   *
   * Requires that AngularSpreadStandardDeviation and Wavelength are both set.
   */
  template<typename R = t::cm>
  boost::units::quantity<R> getDiffractionLimitedWaistStandardDeviation() const
  {
    static_assert(std::is_same<typename R::dimension_type,
                               typename t::cm::dimension_type>::value,
                  "Dimensions Error: Requested return type for "
                  "getDiffractionLimitedWaistStandardDeviation() method "
                  "has wrong dimensions.");
    auto sigma_0 = this->getWavelength() / (4 * M_PI) /
                   this->getAngularSpreadStandardDeviation();
    return boost::units::quantity<R>(sigma_0);
  }

  /**
   * Computes the beam standard deviation (a measure of width) at a specified
   * position.
   */
  template<typename R = t::cm, typename A = t::cm>
  boost::units::quantity<R> getBeamStandardDeviation(
      boost::units::quantity<A> z) const
  {
    static_assert(
        std::is_same<typename R::dimension_type,
                     typename t::cm::dimension_type>::value,
        "Dimensions Error: Requested return type for "
        "getBeamStandardDeviation(...) method "
        "has wrong dimensions. Should have same dimensions as t::cm.");
    static_assert(
        std::is_same<typename A::dimension_type,
                     typename t::cm::dimension_type>::value,
        "Dimensions Error: argument to getBeamStandardDeviation(...) method "
        "has wrong dimensions.");
    auto sigma = root<2>(
        pow<2>(m_WaistStandardDeviation) +
        pow<2>(boost::units::quantity<t::rad>(this->getAngularSpreadStandardDeviation()))
                .value() *
            pow<2>(boost::units::quantity<t::cm>(z) - m_WaistPosition));
    return boost::units::quantity<R>(sigma);
  }

#define ADD_DERIVED_GETTER(NAME, UNIT, ...)                                 \
  template<typename R = UNIT, typename A = t::cm>                           \
  boost::units::quantity<R> get##NAME(boost::units::quantity<A> z) const    \
  {                                                                         \
    static_assert(                                                          \
        std::is_same<typename R::dimension_type,                            \
                     typename UNIT::dimension_type>::value,                 \
        "Dimensions Error: Requested return type for "                      \
        "get##NAME(...) method "                                            \
        "has wrong dimensions. Should have the same dimensions as " #UNIT); \
    static_assert(std::is_same<typename A::dimension_type,                  \
                               typename t::cm::dimension_type>::value,      \
                  "Dimensions Error: argument to get##NAME(...) method "    \
                  "has wrong dimensions.");                                 \
    return boost::units::quantity<R>(__VA_ARGS__);                          \
  }

  ADD_DERIVED_GETTER(FourSigmaDiameter, t::cm,
                     4. * this->getBeamStandardDeviation(z));

#undef ADD_DERIVED_GETTER
};
