#pragma once

#include <boost/units/quantity.hpp>

#include "./FreeSpace.hpp"
#include "./OpticalElement.hpp"
#include "./SphericalRefractiveSurface.hpp"

namespace libGBP2
{

template<typename LengthUnit = t::cm>
class ThickLens : public OpticalElement<LengthUnit>
{
 public:
  using L     = LengthUnit;
  ThickLens() = default;
  template<typename U1, typename U2, typename U3, typename U4>
  ThickLens(quantity<U1> a_refractive_index_scale, quantity<U2> a_front_radius_of_curvature, quantity<U3> a_thickness, quantity<U4> a_back_radius_of_curvature)
  {
    this->setLensParameters(a_refractive_index_scale, a_front_radius_of_curvature, a_thickness, a_back_radius_of_curvature);
  }
  template<typename U1, typename U2, typename U3, typename U4>
  void setLensParameters(quantity<U1> a_refractive_index_scale, quantity<U2> a_front_radius_of_curvature, quantity<U3> a_thickness, quantity<U4> a_back_radius_of_curvature)
  {
    SphericalRefractiveSurface<L> front(a_refractive_index_scale, a_front_radius_of_curvature);
    FreeSpace                     middle(a_thickness);
    SphericalRefractiveSurface<L> back(1 / a_refractive_index_scale, a_back_radius_of_curvature);
    this->setDisplacement(a_thickness);

    static_cast<OpticalElement<L>>(*this) = back * middle * front;
  }
};
}  // namespace libGBP2
