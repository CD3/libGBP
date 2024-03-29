#pragma once

#include "./OpticalElement.hpp"

namespace libGBP2
{

template<c::Length LengthUnit = t::cm>
class FlatRefractiveSurface : public OpticalElement<LengthUnit>
{
 public:
  using L                 = LengthUnit;
  FlatRefractiveSurface() = default;
  template<c::Dimensionless U>
  FlatRefractiveSurface(quantity<U> a_scale)
  {
    this->setRefractiveIndexScaleFactor(a_scale);
  }
  template<c::Dimensionless U>
  void setRefractiveIndexScaleFactor(quantity<U> a_scale)
  {
    OpticalElement<LengthUnit>::setRefractiveIndexScale(a_scale);
    this->setD(1 / a_scale);
  }
};
}  // namespace libGBP2
