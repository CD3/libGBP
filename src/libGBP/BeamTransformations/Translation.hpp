#pragma once

/** @file Translation.hpp
 * @brief Transformation for a shift in the origin.
 * @author C.D. Clark III
 * @date 07/01/16
 */

#include "./BeamTransformation_Base.hpp"
namespace libGBP
{
template<typename T>
class Translation : public BeamTransformation_Base<T>
{
 public:
  // required by interface
  Eigen::Matrix<double, 2, 2> getRTMatrix() const;

 protected:
  boost::units::quantity<T> shift;

 public:
  template<typename U>
  void setShift(U v)
  {
    this->shift = boost::units::quantity<T>(v);
  }
  template<typename U>
  boost::units::quantity<U> getShift() const
  {
    return boost::units::quantity<U>(this->shift);
  }
  inline boost::units::quantity<T> getShift() const
  {
    return this->getShift<T>();
  }
};

template<typename T>
Eigen::Matrix<double, 2, 2> Translation<T>::getRTMatrix() const
{
  Eigen::Matrix<double, 2, 2> mat;

  mat << 1., this->shift.value(), 0., 1.;

  return mat;
}

}  // namespace libGBP
