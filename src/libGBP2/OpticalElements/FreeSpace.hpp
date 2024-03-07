#pragma once

#include "./OpticalElement.hpp"

namespace libGBP2
{

template<c::Length LengthUnit = t::cm>
class FreeSpace : public OpticalElement<LengthUnit>
{
 public:
  FreeSpace() = default;
  template<c::Length U>
  FreeSpace(quantity<U> a_length)
  {
    this->setLength(a_length);
  }
  using L = LengthUnit;
  template<c::Length U>
  void setLength(quantity<U> a_length)
  {
    this->setDisplacement(a_length);
    this->setB(a_length);
  }
  template<c::Length U = L>
  quantity<U>
  getLength() const
  {
    return this->template getDisplacement<U>();
  }
};
}  // namespace libGBP2
