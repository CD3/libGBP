#ifndef OpticalElement_hpp
#define OpticalElement_hpp

/** @file OpticalElement.hpp
  * @brief Definition of the optical element interface.
  * @author C.D. Clark III
  * @date 06/29/16
  */

#include <Eigen/Dense>
#include "Units.hpp"

template<typename LengthUnitType>
class OpticalElementInterface
{

  public:

    virtual Eigen::Matrix<double,2,2> getRTMatrix() const = 0;              ///< return the Ray Transfer matrix for the element. NOTE: elements MUST be returned in units of centimeter or inverse centimeter.
    virtual double                    getPowerLoss() const = 0;             ///< return the power loss through the element
    virtual double                    getWavelengthScaleFactor() const = 0; ///< return the scaling factor through the element. i.e. if the wavelenght changes when alight passes through the element.
    virtual quantity<LengthUnitType>  getPositionShift() const = 0;         ///< return the difference in the position of a beam that passes through element

};

template<typename T, typename U>
class OpticalElementAdapter: public OpticalElementInterface<U>
{
  private:
    T &t;

  public:
    OpticalElementAdapter( T &ref ):t(ref) {}
    Eigen::Matrix<double,2,2> getRTMatrix() const {return t.getRTMatrix();}
    double                    getPowerLoss() const {return t.getPowerLoss();}
    double                    getWavelengthScaleFactor() const {return t.getWavelengthScaleFactor();}
    quantity<U>               getPositionShift() const {return t.getPositionShift();}
};


#endif // include protector
