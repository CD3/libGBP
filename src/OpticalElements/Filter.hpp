#ifndef OpticalElements_Filter_hpp
#define OpticalElements_Filter_hpp

/** @file Filter.hpp
  * @brief 
  * @author C.D. Clark III
  * @date 07/26/16
  */

#include "BaseOpticalElement.hpp"

class Filter: public BaseOpticalElement<t::centimeter>
{
  public:
    // the required interface
    double getPowerLoss() const;
    

  protected:
    double absorbance;

  public:
    // absorbance getters and setters
    template<typename U>
    void setAbsorbance(U v) { this->absorbance = v; } ///< sets absorbance
    inline double getAbsorbance() const { return this->absorbance; } ///< returns absorbance
    template<typename U>
    void setOpticalDensity(U v) { this->setAbsorbance(v); } ///< sets absorbance
    inline double getOpticalDensity() const { return this->getAbsorbance(); } ///< returns absorbance
};

inline
double Filter::getPowerLoss() const
{
  // absorbance is the logarithm of the ratio of transmitted to incident power.
  // we need to return the percentage of power lost, which is 1 - this transmitted/incident power ratio
  
  double loss = 1. - 1./pow(10,this->absorbance);

  return loss;
}


#endif // include protector
