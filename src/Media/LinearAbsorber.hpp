#ifndef Media_LinearAbsorber_hpp
#define Media_LinearAbsorber_hpp

/** @file LinearAbsorber.hpp
  * @brief 
  * @author C.D. Clark III
  * @date 07/27/16
  */

#include "Absorber.hpp"

template<typename LengthUnitType>
class LinearAbsorber : public Absorber<LengthUnitType>
{
  protected:
    typedef typename divide_typeof_helper < t::dimensionless, LengthUnitType>::type InvLengthUnitType;
    quantity<InvLengthUnitType> absorptionCoefficient;

  public:
    template<typename U>
    void setAbsorptionCoefficient(U v) { this->absorptionCoefficient = quantity<InvLengthUnitType>(v); } ///< performs unit conversion and sets absorptionCoefficient
    template<typename U>
    quantity<U> getAbsorptionCoefficient() const { return quantity<U>(this->absorptionCoefficient); } ///< returns absorptionCoefficient in specified units
    inline quantity<InvLengthUnitType> getAbsorptionCoefficient() const { return this->getAbsorptionCoefficient<InvLengthUnitType>(); } ///< returns absorptionCoefficient in internal units (InvLengthUnitType)


    template<typename T, typename U>
    double getTransmission( T zi, U zf ) const; ///< returns percentage of power transmitted through absorber between positions zi and zf.

    virtual double getTransmission( quantity<LengthUnitType> zi, quantity<LengthUnitType> zf ) const { return this->getTransmission<>(zi,zf); }
};

template<typename LengthUnitType>
template<typename T, typename U>
double LinearAbsorber<LengthUnitType>::getTransmission( T zi, U zf ) const
{
  quantity<LengthUnitType> dz = quantity<LengthUnitType>(zf) - quantity<LengthUnitType>(zi);
  double transmission = exp( - (this->absorptionCoefficient * dz).value() );

  return transmission;
}

#endif // include protector
