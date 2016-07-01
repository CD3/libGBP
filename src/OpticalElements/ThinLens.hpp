#ifndef OpticalElements_ThinLens_hpp
#define OpticalElements_ThinLens_hpp

/** @file 
  * @brief Class representing a thin lens.
  * @author C.D. Clark III
  * @date 07/01/16
  */

#include "IdealElement.hpp"

template<typename LengthUnitType>
class ThinLens : IdealElement
{
  public:
    // the required interface
    Eigen::Matrix<double,2,2> getRTMatrix() const;

  protected:
    quantity<LengthUnitType> focalLength;

  public:
    // focalLength getters and setters
    template<typename T>
    void setFocalLength(T v) { this->focalLength = quantity<LengthUnitType>(v); } ///< performs unit conversion and sets focalLength
    template<typename T>
    quantity<T> getFocalLength() const { return quantity<T>(this->focalLength); } ///< returns focalLength in specified units
    inline quantity<LengthUnitType> getFocalLength() const { return this->getFocalLength<LengthUnitType>(); } ///< returns focalLength in internal units (LengthUnitType)
};

template<typename LengthUnitType>
Eigen::Matrix<double,2,2>
ThinLens<LengthUnitType>::getRTMatrix() const
{
  Eigen::Matrix<double,2,2> mat;

  mat << 1., 0., (-1./focalLength).value(), 1.;

  return mat;
}


#endif // include protector
