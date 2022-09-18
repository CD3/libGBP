#pragma once

/** @file
 * @brief Class representing a thin lens.
 * @author C.D. Clark III
 * @date 07/01/16
 */

#include "./BeamTransformation_Base.hpp"

template<typename T>
class ThinLens : public BeamTransformation_Base<T>
{
 public:
  Eigen::Matrix<double, 2, 2> getRTMatrix() const;

 protected:
  boost::units::quantity<T> focalLength;

 public:
  // focalLength getters and setters
  template<typename U>
  void setFocalLength(U v)
  {
    this->focalLength = boost::units::quantity<T>(v);
  }  ///< performs unit conversion and sets focalLength
  template<typename U>
  boost::units::quantity<U> getFocalLength() const
  {
    return boost::units::quantity<U>(this->focalLength);
  }  ///< returns focalLength in specified units
  inline boost::units::quantity<T> getFocalLength() const
  {
    return this->getFocalLength<T>();
  }  ///< returns focalLength in internal units (T)
};

template<typename T>
Eigen::Matrix<double, 2, 2> ThinLens<T>::getRTMatrix() const
{
  Eigen::Matrix<double, 2, 2> mat;

  mat << 1., 0., (-1. / focalLength).value(), 1.;

  return mat;
}
