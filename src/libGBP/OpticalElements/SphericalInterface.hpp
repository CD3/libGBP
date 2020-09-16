#ifndef OpticalElements_SphericalInterface_hpp
#define OpticalElements_SphericalInterface_hpp

/** @file SphericalInterface.hpp
 * @brief A spherical interface between two media.
 * @author C.D. Clark III
 * @date 07/01/16
 */

#include "Interface.hpp"

template<typename T>
class SphericalInterface : public Interface<T>
{
 public:
  // required by interface
  Eigen::Matrix<double, 2, 2> getRTMatrix() const;

 protected:
  boost::units::quantity<T> radiusOfCurvature;

 public:
  // radiusOfCurvature getters and setters
  template<typename U>
  void setRadiusOfCurvature(U v)
  {
    this->radiusOfCurvature = boost::units::quantity<T>(v);
  }  ///< performs unit conversion and sets radiusOfCurvature
  template<typename U>
  boost::units::quantity<U> getRadiusOfCurvature() const
  {
    return boost::units::quantity<U>(this->radiusOfCurvature);
  }  ///< returns radiusOfCurvature in specified units
  inline boost::units::quantity<T> getRadiusOfCurvature() const
  {
    return this->getRadiusOfCurvature<T>();
  }  ///< returns radiusOfCurvature in internal units (T)
};

template<typename T>
Eigen::Matrix<double, 2, 2> SphericalInterface<T>::getRTMatrix() const
{
  Eigen::Matrix<double, 2, 2> mat;

  auto f = (this->finalRefractiveIndex - this->initialRefractiveIndex) /
           (radiusOfCurvature * this->finalRefractiveIndex);

  mat << 1., 0., -f.value(), this->getWavelengthScaleFactor();

  return mat;
}

#endif  // include protector
