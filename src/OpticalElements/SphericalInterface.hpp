#ifndef OpticalElements_SphericalInterface_hpp
#define OpticalElements_SphericalInterface_hpp

/** @file SphericalInterface.hpp
  * @brief A spherical interface between two media.
  * @author C.D. Clark III
  * @date 07/01/16
  */

#include "Interface.hpp"

template<typename LengthUnitType>
class SphericalInterface : public Interface
{
  public:
    // required by interface
    Eigen::Matrix<double,2,2> getRTMatrix() const;

  protected:
    quantity<LengthUnitType> radiusOfCurvature;

  public:

    // radiusOfCurvature getters and setters
    template<typename T>
    void setRadiusOfCurvature(T v) { this->radiusOfCurvature = quantity<LengthUnitType>(v); } ///< performs unit conversion and sets radiusOfCurvature
    template<typename T>
    quantity<T> getRadiusOfCurvature() const { return quantity<T>(this->radiusOfCurvature); } ///< returns radiusOfCurvature in specified units
    inline quantity<LengthUnitType> getRadiusOfCurvature() const { return this->getRadiusOfCurvature<LengthUnitType>(); } ///< returns radiusOfCurvature in internal units (LengthUnitType)

};

template<typename T>
Eigen::Matrix<double,2,2>
SphericalInterface<T>::getRTMatrix() const
{
  Eigen::Matrix<double,2,2> mat;

  auto f = (finalRefractiveIndex - initialRefractiveIndex)/(radiusOfCurvature*finalRefractiveIndex);

  mat << 1., 0., -f.value(), this->getWavelengthScaleFactor();

  return mat;
}


#endif // include protector

