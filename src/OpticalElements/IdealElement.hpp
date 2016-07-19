#ifndef OpticalElements_IdealElement_hpp
#define OpticalElements_IdealElement_hpp

#include "OpticalElementInterface.hpp"

/** @file IdealElement.hpp
  * @brief Base class for "ideal" elements. Ideal elements have zero power loss and do not change the wavelength.
  * @author C.D. Clark III
  * @date 07/01/16
  */

template<typename T>
class IdealElement : public OpticalElementInterface<T>
{
  public:
    virtual Eigen::Matrix<double,2,2> getRTMatrix() const = 0;
    virtual double                    getPowerLoss() const {return 0;}
    virtual double                    getWavelengthScaleFactor() const {return 1;}
    virtual quantity<T>               getPositionShift() const {return 0*T();}
};




#endif // include protector
