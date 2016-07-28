#ifndef Media_Absorber_hpp
#define Media_Absorber_hpp

/** @file Absorber.hpp
  * @brief 
  * @author C.D. Clark III
  * @date 07/27/16
  */

template<typename LengthUnitType>
class Absorber
{
  public:
    virtual double getTransmission( quantity<LengthUnitType> zi, quantity<LengthUnitType> zf ) const {return 1;} ///< returns percentage of power transmitted through absorber between positions zi and zf.

};

template<typename T>
using  Absorber_ptr = std::shared_ptr<Absorber<T> >;


#endif // include protector
