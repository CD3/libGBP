#pragma once

#include "./OpticalElement.hpp"

namespace libGBP2
{

template<c::Length LengthUnit = t::cm>
class ThinLens : public OpticalElement<LengthUnit>
{
 public:
  ThinLens() = default;
  template<c::Length U>
  ThinLens(quantity<U> a_focal_length)
  {
    this->setFocalLength(a_focal_length);
  }
  using L = LengthUnit;
  template<c::Length U>
  void setFocalLength(quantity<U> a_focal_length)
  {
    this->setC(-1 / a_focal_length);
  }
  template<c::Length U = L>
  quantity<U>
  getFocalLength() const
  {
    return quantity<U>(-1 / this->getC());
  }
};
}  // namespace libGBP2
