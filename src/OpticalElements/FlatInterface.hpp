#ifndef OpticalElements_FlatInterface_hpp
#define OpticalElements_FlatInterface_hpp

/** @file FlatInterface.hpp
  * @brief A flat interface between two media.
  * @author C.D. Clark III
  * @date 07/01/16
  */

#include "Interface.hpp"

class FlatInterface : public Interface
{
  public:
    Eigen::Matrix<double,2,2> getRTMatrix() const;

};

inline
Eigen::Matrix<double,2,2>
FlatInterface::getRTMatrix() const
{
  Eigen::Matrix<double,2,2> mat;

  mat << 1., 0., 0, this->getWavelengthScaleFactor();

  return mat;
}


#endif // include protector
