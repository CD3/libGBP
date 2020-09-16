#ifndef Media_LinearAbsorber_hpp
#define Media_LinearAbsorber_hpp

/** @file LinearAbsorber.hpp
 * @brief
 * @author C.D. Clark III
 * @date 07/27/16
 */

#include "BaseMedia.hpp"

template<typename LengthUnitType>
class LinearAbsorber : public BaseMedia<LengthUnitType>
{
 protected:
  typedef typename boost::units::divide_typeof_helper<units::t::dimensionless, LengthUnitType>::type
                              InvLengthUnitType;
  boost::units::quantity<InvLengthUnitType> absorptionCoefficient;

 public:
  template<typename U>
  void setAbsorptionCoefficient(U v)
  {
    this->absorptionCoefficient = boost::units::quantity<InvLengthUnitType>(v);
  }  ///< performs unit conversion and sets absorptionCoefficient
  template<typename U>
  boost::units::quantity<U> getAbsorptionCoefficient() const
  {
    return boost::units::quantity<U>(this->absorptionCoefficient);
  }  ///< returns absorptionCoefficient in specified units
  inline boost::units::quantity<InvLengthUnitType> getAbsorptionCoefficient() const
  {
    return this->getAbsorptionCoefficient<InvLengthUnitType>();
  }  ///< returns absorptionCoefficient in internal units (InvLengthUnitType)

  template<typename T, typename U>
  double getTransmission(
      T zi, U zf) const;  ///< returns percentage of power transmitted through
                          ///< absorber between positions zi and zf.

  virtual double getTransmission(boost::units::quantity<LengthUnitType> zi,
                                 boost::units::quantity<LengthUnitType> zf) const
  {
    return this->getTransmission<>(zi, zf);
  }
};

template<typename LengthUnitType>
template<typename T, typename U>
double LinearAbsorber<LengthUnitType>::getTransmission(T zi, U zf) const
{
  boost::units::quantity<LengthUnitType> dz =
      boost::units::quantity<LengthUnitType>(zf) - boost::units::quantity<LengthUnitType>(zi);
  double transmission = exp(-(this->absorptionCoefficient * dz).value());

  return transmission;
}

#endif  // include protector
