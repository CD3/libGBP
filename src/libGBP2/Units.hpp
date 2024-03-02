#pragma once

#include <concepts>
#include <type_traits>

#include <BoostUnitDefinitions/Units.hpp>
#include <boost/units/quantity.hpp>

namespace libGBP2
{
namespace t = boost::units::t;
namespace i = boost::units::i;
using boost::units::quantity;

// a namespace for concepts related to units
namespace c
{

// type trait  to see if two units have the same dimension
template<typename T, typename U>
struct have_same_dimensions {
  static const bool value = std::is_same<typename T::dimension_type,
                                         typename U::dimension_type>::value;
};

// concepts to check that units passed as template parameters have specific dimensions
template<typename U>
concept Length = have_same_dimensions<U, t::cm>::value;
template<typename U>
concept Area = have_same_dimensions<U, t::cm_p2>::value;
template<typename U>
concept Time = have_same_dimensions<U, t::s>::value;
template<typename U>
concept Speed = have_same_dimensions<U, decltype(i::m / i::s)>::value;
template<typename U>
concept Angle = have_same_dimensions<U, t::rad>::value;
template<typename U>
concept Frequency = have_same_dimensions<U, t::Hz>::value;
template<typename U>
concept InverseLength = have_same_dimensions<U, t::m_n1>::value;
template<typename U>
concept Dimensionless = have_same_dimensions<U, t::dimensionless>::value;
template<typename U>
concept Irradiance = have_same_dimensions<U, t::W_m_n2>::value;

}  // namespace c
}  // namespace libGBP2
