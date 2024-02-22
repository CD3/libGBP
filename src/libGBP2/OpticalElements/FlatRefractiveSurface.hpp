#pragma once

#include "./OpticalElement.hpp"

namespace libGBP2
{

template<typename LengthUnit = t::cm>
class FlatRefractiveSurface : public OpticalElement<LengthUnit>
{
 public:
  FlatRefractiveSurface() = default;
  template<typename U>
  FlatRefractiveSurface(quantity<U> a_scale)
  {
    this->setRefractiveIndexScaleFactor(a_scale);
  }
  using L = LengthUnit;
  template<typename U>
  void setRefractiveIndexScaleFactor(quantity<U> a_scale)
  {
    OpticalElement<LengthUnit>::setRefractiveIndexScale(a_scale);
    this->setD(1 / a_scale);
  }
};
}  // namespace libGBP2
