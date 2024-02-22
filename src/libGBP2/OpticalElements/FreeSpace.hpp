#pragma once

#include "./OpticalElement.hpp"

namespace libGBP2
{

template<typename LengthUnit = t::cm>
class FreeSpace : public OpticalElement<LengthUnit>
{
 public:
  FreeSpace() = default;
  template<typename U>
  FreeSpace(quantity<U> a_length)
  {
    this->setLength(a_length);
  }
  using L = LengthUnit;
  template<typename U>
  void setLength(quantity<U> a_length)
  {
    this->setDisplacement(a_length);
    this->setB(a_length);
  }
  template<typename U = L>
  quantity<U>
  getLength() const
  {
    return this->template getDisplacement<U>();
  }
};
}  // namespace libGBP2
