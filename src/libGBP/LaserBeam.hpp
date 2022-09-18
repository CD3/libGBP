#pragma once

/** @file LaserBeam.hpp
 * @brief
 * @author C.D. Clark III
 * @date 07/13/20
 */

#include <type_traits>

#include "Constants.hpp"
#include "Units.hpp"

namespace libGBP {

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

  ADD_MEMBER(Wavelength, units::t::nm);
  ADD_MEMBER(Frequency, units::t::Hz);
  ADD_MEMBER(Power, units::t::W);
  ADD_MEMBER(WaistStandardDeviation, units::t::cm);
  ADD_MEMBER(WaistPosition, units::t::cm);

 protected:
  bool m_UseDiffractionLimitedDivergence = true;
 public:
  inline void setUseDiffractionLimitedDivergence(bool val){m_UseDiffractionLimitedDivergence=val;}
  inline bool getUseDiffractionLimitedDivergence() const{return m_UseDiffractionLimitedDivergence;}

 protected:
  boost::units::quantity<units::t::mrad,double> m_AngularSpreadStandardDeviation;
 public:
  template<typename R = units::t::mrad> 
  boost::units::quantity<R> getAngularSpreadStandardDeviation() const
  {                                                                 
    static_assert(std::is_same<typename R::dimension_type,         
                               typename units::t::mrad::dimension_type>::value,        
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
                     typename units::t::mrad::dimension_type>::value,             
        "Dimensions Error: Argument type of setAngularSpreadStandardDeviation(...) method has "
        "wrong dimensions.");                                         
    m_AngularSpreadStandardDeviation = boost::units::quantity<units::t::mrad>(a);                      
    m_UseDiffractionLimitedDivergence = false;
  }

  /**
   * Adjusts (sets) the angular spread of the beam (divergence) to give the specified
   * propagation factor (M^2) for the current waist size.
   *
   * Requires wavelength and waist standard deviation to be set.
   */
  template<typename A>                                                   
  void adjustAngularSpreadStandardDeviationToBeamPropagationFactor(boost::units::quantity<A> a)                           
  {                                                                    
    static_assert(                                                        
        std::is_same<typename A::dimension_type,                         
                     typename units::t::dimensionless::dimension_type>::value,             
        "Dimensions Error: Argument type of adjustAngularSpreadStandardDeviationToBeamPropagationFactor(...) method has "
        "wrong dimensions.");                                         
    this->setAngularSpreadStandardDeviation(boost::units::quantity<units::t::dimensionless>(a).value()*this->getDiffractionLimitedAngularSpreadStandardDeviation());
  }

  /**
   * Adjusts (sets) the waist standard deviation (size) to give the specified
   * propagation factor (M^2) for the current divergence.
   *
   * Requires wavelength and angular spread standard deviation to be set.
   */
  template<typename A>                                                   
  void adjustWaistStandardDeviationToBeamPropagationFactor(boost::units::quantity<A> a)                           
  {                                                                    
    static_assert(                                                        
        std::is_same<typename A::dimension_type,                         
                     typename units::t::dimensionless::dimension_type>::value,             
        "Dimensions Error: Argument type of adjustAngularSpreadStandardDeviationToBeamPropagationFactor(...) method has "
        "wrong dimensions.");                                         
    this->setWaistStandardDeviation( boost::units::quantity<units::t::dimensionless>(a).value()*this->getDiffractionLimitedWaistStandardDeviation());
  }

  /**
   * This function is just an alias for adjustAngularSpreadStandardDeviationForBeamPropagationFactor(...)
   */
  template<typename A>                                                   
  void adjustDivergenceToBeamPropagationFactor(boost::units::quantity<A> a)                           
  {                                                                    
    this->adjustAngularSpreadStandardDeviationToBeamPropagationFactor(a);
  }

  /**
   * This function is just an alias for adjustWaistStandardDeviationForBeamPropagationFactor(...)
   */
  template<typename A>                                                   
  void adjustWaistSizeToBeamPropagationFactor(boost::units::quantity<A> a)                           
  {                                                                    
    this->adjustWaistStandardDeviationToBeamPropagationFactor(a);
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
                               typename units::t::nm::dimension_type>::value, \
                  "Dimensions Error: Argument type of "                \
                  "set##NAME(...) method "                             \
                  "has wrong dimensions.");                            \
    __VA_ARGS__;                                                       \
  }

  ADD_DERIVED_GETTER(FreeSpaceWavelength, units::t::nm,
                     constants::SpeedOfLight / this->getFrequency<units::t::hertz>());
  ADD_DERIVED_SETTER(FreeSpaceWavelength, units::t::nm,
                     this->setFrequency(constants::SpeedOfLight / arg));
  ADD_DERIVED_GETTER(WaistFourSigmaDiameter, units::t::cm,
                     this->getWaistStandardDeviation() * 4.);
  ADD_DERIVED_SETTER(WaistFourSigmaDiameter, units::t::cm,
                     this->setWaistStandardDeviation(arg / 4.));
  /**
   * Computes the M^2 beam propagation factor (sometimes called the "beam
   * quality") from the beam divergence and beam waist size.
   */
  ADD_DERIVED_GETTER(
      BeamPropagationFactor, units::t::dimensionless,
      this->getAngularSpreadStandardDeviation<units::t::rad>() /
          this->getDiffractionLimitedAngularSpreadStandardDeviation<units::t::rad>());
  ADD_DERIVED_GETTER(
      BeamParameterProduct, decltype(units::i::mm*units::i::mrad),
      this->getWaistStandardDeviation<units::t::mm>() *
          this->getAngularSpreadStandardDeviation<units::t::mrad>() );
#undef ADD_DERIVED_GETTER
#undef ADD_DERIVED_SETTER

  /**
   * Computes the diffraction limited second-moment divergence.
   *
   * Requires that WaistStandardDeviation and Wavelength are both set.
   */
  template<typename R = units::t::mrad>
  boost::units::quantity<R>
  getDiffractionLimitedAngularSpreadStandardDeviation() const
  {
    static_assert(
        std::is_same<typename R::dimension_type,
                     typename units::t::mrad::dimension_type>::value,
        "Dimensions Error: Requested return type for "
        "getDiffractionLimitedAngularSpreadStandardDeviation() method "
        "has wrong dimensions.");
    auto sigma_theta = boost::units::quantity<units::t::dimensionless>(
                           this->getWavelength() / (4 * M_PI) /
                           this->getWaistStandardDeviation()) *
                       units::i::rad;
    return boost::units::quantity<R>(sigma_theta);
  }

  /**
   * Computes the diffraction limited waist standard deviation.
   *
   * Requires that AngularSpreadStandardDeviation and Wavelength are both set.
   */
  template<typename R = units::t::cm>
  boost::units::quantity<R> getDiffractionLimitedWaistStandardDeviation() const
  {
    static_assert(std::is_same<typename R::dimension_type,
                               typename units::t::cm::dimension_type>::value,
                  "Dimensions Error: Requested return type for "
                  "getDiffractionLimitedWaistStandardDeviation() method "
                  "has wrong dimensions.");
    auto sigma_0 = this->getWavelength() / (4 * M_PI) /
                   this->getAngularSpreadStandardDeviation<units::t::rad>().value();
    return boost::units::quantity<R>(sigma_0);
  }

  /**
   * Computes the beam standard deviation (a measure of width) at a specified
   * position.
   */
  template<typename R = units::t::cm, typename A = units::t::cm>
  boost::units::quantity<R> getBeamStandardDeviation(
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
    auto sigma = boost::units::root<2>(
        boost::units::pow<2>(m_WaistStandardDeviation) +
        boost::units::pow<2>(boost::units::quantity<units::t::rad>(this->getAngularSpreadStandardDeviation()))
                .value() *
            boost::units::pow<2>(boost::units::quantity<units::t::cm>(z) - m_WaistPosition));
    return boost::units::quantity<R>(sigma);
  }

#define ADD_DERIVED_GETTER(NAME, UNIT, ...)                                 \
  template<typename R = UNIT, typename A = units::t::cm>                           \
  boost::units::quantity<R> get##NAME(boost::units::quantity<A> z) const    \
  {                                                                         \
    static_assert(                                                          \
        std::is_same<typename R::dimension_type,                            \
                     typename UNIT::dimension_type>::value,                 \
        "Dimensions Error: Requested return type for "                      \
        "get##NAME(...) method "                                            \
        "has wrong dimensions. Should have the same dimensions as " #UNIT); \
    static_assert(std::is_same<typename A::dimension_type,                  \
                               typename units::t::cm::dimension_type>::value,      \
                  "Dimensions Error: argument to get##NAME(...) method "    \
                  "has wrong dimensions.");                                 \
    return boost::units::quantity<R>(__VA_ARGS__);                          \
  }

  ADD_DERIVED_GETTER(FourSigmaDiameter, units::t::cm,
                     4. * this->getBeamStandardDeviation(z));

#undef ADD_DERIVED_GETTER
};

}
