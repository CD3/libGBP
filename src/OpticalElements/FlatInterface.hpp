#ifndef OpticalElements_FlatInterface_hpp
#define OpticalElements_FlatInterface_hpp

/** @file FlatInterface.hpp
  * @brief A flat interface between two media.
  * @author C.D. Clark III
  * @date 07/01/16
  */

#include "Interface.hpp"

template<typename T>
class FlatInterface : public Interface<T>
{
  public:
    Eigen::Matrix<double,2,2> getRTMatrix() const;

};

template<typename T>
Eigen::Matrix<double,2,2>
FlatInterface<T>::getRTMatrix() const
{
  Eigen::Matrix<double,2,2> mat;

  mat << 1., 0., 0, this->getWavelengthScaleFactor();

  return mat;
}


#endif // include protector
