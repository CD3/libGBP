#pragma once

#include "./OpticalElement.hpp"

namespace libGBP2
{

template<typename LengthUnit = t::cm>
class ThinLens : public OpticalElement<LengthUnit>
{
 public:
  ThinLens() = default;
  template<typename U>
  ThinLens(quantity<U> a_focal_length)
  {
    this->setFocalLength(a_focal_length);
  }
  using L = LengthUnit;
  template<typename U>
  void setFocalLength(quantity<U> a_focal_length)
  {
    this->setC(-1 / a_focal_length);
  }
  template<typename U = L>
  quantity<U>
  getFocalLength() const
  {
    return quantity<U>(-1 / this->getC());
  }
};
}  // namespace libGBP2
