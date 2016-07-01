#ifndef OpticalElement_hpp
#define OpticalElement_hpp

/** @file OpticalElement.hpp
  * @brief Definition of the optical element interface.
  * @author C.D. Clark III
  * @date 06/29/16
  */

#include <Eigen/Dense>
#include "Units.hpp"

class OpticalElementInterface
{

  public:

    virtual Eigen::Matrix<double,2,2> getRTMatrix() const = 0;              ///< return the Ray Transfer matrix for the element
    virtual double                    getPowerLoss() const = 0;             ///< return the power loss through the element
    virtual double                    getWavelengthScaleFactor() const = 0; ///< return the scaling factor through the element. i.e. if the wavelenght changes when alight passes through the element.

};

template<typename T>
class OpticalElementAdapter: public OpticalElementInterface
{
  private:
    T &t;

  public:
    OpticalElementAdapter( T &ref ):t(ref) {}
    Eigen::Matrix<double,2,2> getRTMatrix() const {return t.getRTMatrix();}
    double                    getPowerLoss() const {return t.getPowerLoss();}
    double                    getWavelengthScaleFactor() const {return t.getWavelengthScaleFactor();}
};


#endif // include protector
