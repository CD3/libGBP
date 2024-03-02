#pragma once

#include <utility>
#include <vector>

#include "./OpticalElements/FreeSpace.hpp"
#include "./OpticalElements/OpticalElement.hpp"
namespace libGBP2
{

/**
 * A class for building an optical system.
 */
template<typename LengthUnit = t::cm>
class OpticalSystem
{
 public:
  using L = LengthUnit;

 private:
  std::vector<std::pair<quantity<L>, OpticalElement<L>>> m_elements;

 public:
  /**
   * Add an element to the system at a given position.
   */
  template<typename U1, typename U2>
  void add(quantity<U1> a_z, OpticalElement<U2> a_element)
  {
    bool sorted = false;
    if(m_elements.size() < 1 || quantity<L>(a_z) > m_elements[m_elements.size() - 1].first)
      sorted = true;

    m_elements.push_back(std::make_pair(std::move(quantity<L>(a_z)), OpticalElement<L>(std::move(a_element))));
    if(!sorted) {
      std::sort(m_elements.begin(), m_elements.end(), [](const auto &left, const auto &right) { return left.first < right.first; });
    }
  }

  /**
   * Build an optical element that will propagate a beam
   * from a position a_z_start to a position a_z_end in the system.
   */
  template<typename UR = L, typename UA1 = L, typename UA2 = L>
  OpticalElement<UR> build(quantity<UA1> a_z_start, quantity<UA2> a_z_end) const
  {
    quantity<L>        l_z = quantity<L>(a_z_start);
    OpticalElement<UR> system;
    for(const auto &elem : m_elements) {
      // if the element is past a_z_end, we are done and can break out of the loop

      if(elem.first > quantity<L>(a_z_end)) {
        break;
      }
      // only add elements that are *at-or-after* current position
      if(elem.first >= l_z) {
        // add a free space propagation to get to the element
        system = elem.second * FreeSpace(elem.first - l_z) * system;
        l_z    = elem.first;
      }
    }
    // add a free space propagation to the a_z_end position
    system = FreeSpace(quantity<L>(a_z_end) - l_z) * system;
    return system;
  }
  /**
   * Build an optical element that will propagate a beam
   * from a position of the first element in the system
   * to a position a_z_end in the system.
   */
  template<typename UR = L, typename UA = L>
  OpticalElement<UR> build(quantity<UA> a_z_end) const
  {
    return this->build(m_elements.size() > 0 ? m_elements[0].first : 0 * i::cm, a_z_end);
  }
  /**
   * Build an optical element that will propagate a beam
   * from a position of the first element in the system
   * to the position last element in the system.
   */
  template<typename UR = L>
  OpticalElement<UR> build() const
  {
    return this->build(m_elements.size() > 0 ? m_elements[m_elements.size() - 1].first : 0 * i::cm);
  }
};
}  // namespace libGBP2
