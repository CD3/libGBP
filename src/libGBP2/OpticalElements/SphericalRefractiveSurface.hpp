#pragma once

#include "./OpticalElement.hpp"

namespace libGBP2
{

template<typename LengthUnit = t::cm>
class SphericalRefractiveSurface : public OpticalElement<LengthUnit>
{
 public:
  using L                      = LengthUnit;
  SphericalRefractiveSurface() = default;
  template<typename U1, typename U2>
  SphericalRefractiveSurface(quantity<U1> a_scale, quantity<U2> a_radius_of_curvature)
  {
    this->setRefractiveIndexScaleFactorAndRadiusOfCurvature(a_scale, a_radius_of_curvature);
  }
  template<typename U1, typename U2>
  void setRefractiveIndexScaleFactorAndRadiusOfCurvature(quantity<U1> a_scale, quantity<U2> a_radius_of_curvature)
  {
    OpticalElement<LengthUnit>::setRefractiveIndexScale(a_scale);
    this->setC(((1 / a_scale) - 1) / a_radius_of_curvature);
    this->setD(1 / a_scale);
  }
};
}  // namespace libGBP2
