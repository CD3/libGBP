#pragma once

#include <type_traits>

#include <boost/units/quantity.hpp>

#include "./Units.hpp"

namespace libGBP2
{

namespace Conventions
{

/**
 * A template to store a quantity
 * that we can use to derive from and
 * make strong types.
 */
template<typename U>
struct StrongQuantity {
 private:
  quantity<U> m_value;

 public:
  template<typename UU>
  StrongQuantity(quantity<UU> a_val)
  {
    m_value = quantity<U>(a_val);
  }
  quantity<U> value() const { return m_value; }
};

// define a set of types for the various beam with conventions we support.
struct SecondMomentWidth : public StrongQuantity<t::cm> {
};
struct D4SigmaWidth : public StrongQuantity<t::cm> {
};
struct OneOverERadius : public StrongQuantity<t::cm> {
};
struct OneOverESquaredRadius : public StrongQuantity<t::cm> {
};
struct FWHMRadius : public StrongQuantity<t::cm> {
};
struct OneOverEDiameter : public StrongQuantity<t::cm> {
};
struct OneOverESquaredDiameter : public StrongQuantity<t::cm> {
};
struct FWHMDiameter : public StrongQuantity<t::cm> {
};

/**
 * A function to compute width of some convention `Convention`
 * from the 1 over e squared radius. i.e.
 *
 * All we need to do is define how to compute each convention
 * from the 1/e squared radius (or any other convention of our choice)
 * and we can convert between all conventions.
 *
 * This function must be specialized for every convention we support.
 */
template<typename Convention>
double FromOneOverESquaredRadiusCF()
{
  static_assert(!std::is_same<Convention, Convention>::value, "No conversion factor defined for conversion.");
  return 0;
}

// define some numerical constant that we will use
template<class T>
constexpr T root_2 = T(1.4142135623730951L);
template<class T>
constexpr T one_over_root_2 = T(0.7071067811865475L);

template<class T>
constexpr T ln_2 = T(0.6931471805599453L);
template<class T>
constexpr T one_over_ln_2 = T(1.4426950408889634L);

template<class T>
constexpr T root_ln_2 = T(0.8325546111576977L);
template<class T>
constexpr T one_over_root_ln_2 = T(1.2011224087864498L);

template<>
inline double FromOneOverESquaredRadiusCF<SecondMomentWidth>()
{
  return 1;
}
template<>
inline double FromOneOverESquaredRadiusCF<D4SigmaWidth>()
{
  return 2;
}

template<>
inline double FromOneOverESquaredRadiusCF<OneOverESquaredRadius>()
{
  return 1;
}

template<>
inline double FromOneOverESquaredRadiusCF<OneOverESquaredDiameter>()
{
  return 2;
}

template<>
inline double FromOneOverESquaredRadiusCF<OneOverERadius>()
{
  return one_over_root_2<double>;
}

template<>
inline double FromOneOverESquaredRadiusCF<OneOverEDiameter>()
{
  return root_2<double>;
}

template<>
inline double FromOneOverESquaredRadiusCF<FWHMRadius>()
{
  static double cf = root_ln_2<double> / root_2<double>;
  return cf;
}

template<>
inline double FromOneOverESquaredRadiusCF<FWHMDiameter>()
{
  static double cf = root_ln_2<double> * root_2<double>;
  return cf;
}

template<typename C1, typename C2>
double ConversionFactor()
{
  // we have conversion factors for all conventions to 1/e squared radius.
  // to convert between any two conventions C1 and C2, we just need to convert C1 to 1/e
  // squared radius, and then convert that to C2.
  //
  // C1 = factor_1 \omega
  // C2 = factor_2 \omega
  //
  // C2 = factor_2 (C1 / factor_1) = (factor_2/factor_1) C1
  return FromOneOverESquaredRadiusCF<C2>() / FromOneOverESquaredRadiusCF<C1>();
}

}  // namespace Conventions

class GaussianBeamWidth
{
 private:
  quantity<t::cm> m_one_over_e_squared_radius;
 public:
};

}  // namespace libGBP2
