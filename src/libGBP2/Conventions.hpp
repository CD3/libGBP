#pragma once

#include <type_traits>

#include <boost/units/quantity.hpp>

#include "./Units.hpp"

/**
 * This file contains some classes and functions to work with
 * conventions. We want to support getting and setting a laser
 * beam width and divergence using different conventions. i.e.
 * setting the 1/e squared radius and getting the 1/e diameter.
 *
 * Rather than provide a separte method for each one of these conventions
 * (as we did in v1 of the library), we create a type that can
 * hold the width and knows how to convert to/from the different conventions.
 *
 * To accomplish this we:
 *
 * 1. Define an empty strict for each convention. These will be used as template arguments.
 * 2. Define functions to compute the conversion factor for each convention _from_ 1/e squared radius.
 *     - In other words, for an convention C = \alpha \omega, define \alpha.
 * 3. Define a function to compute the conversion factor between any two conventions.
 *     - This function can just use the conversion factors we defined above.
 * 4. Create a class template that stores a beam width, keeping track of the convention and units.
 *     - This class stores the width internally using some convention and unit, but can "get" the width in
 *       any other convention and unit.
 *
 * The Gaussian laser beam classes can then return the beam width class instead of quantities, so the
 * user can convert to any convention they want.
 *
 */
namespace libGBP2
{

/**
 * A namespace for defining constants that will be used. i.e. sqrt(2), log(2), etc.
 */
namespace Constants
{

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
}  // namespace Constants

/**
 * A template to store a quantity
 * that we can use to derive from and
 * make strong types.
 */
template<typename U>
struct StrongQuantity {
 private:
  quantity<U> m_quantity;

 public:
  template<typename UU>
  StrongQuantity(quantity<UU> a_val)
  {
    m_quantity = quantity<U>(a_val);
  }
  template<typename V = U>
  quantity<V> quant() const
  {
    return quantity<V>(m_quantity);
  }
  auto value() const -> decltype(m_quantity.value()) { return m_quantity.value(); }
};

////////////////////////////////////////////////////////////////////////////////
// define a set of types for the various beam width conventions we support.   //
////////////////////////////////////////////////////////////////////////////////
struct SecondMomentWidth {
};
struct D4SigmaWidth {
};
struct OneOverERadius {
};
struct OneOverESquaredRadius {
};
struct FWHMRadius {
};
struct OneOverEDiameter {
};
struct OneOverESquaredDiameter {
};
struct FWHMDiameter {
};

/**
 * A function to compute conversion factor for some convention `Convention`
 * from the 1 over e squared radius. i.e., for any convention C, we have
 *
 * C = \alpha \omega
 *
 * where \omega is the usual 1/e squared radius. This function returns \alpha.
 *
 * All we need to do is define how to compute each convention
 * from the 1/e squared radius (we could have chose any other convention)
 * and we can convert between all conventions.
 *
 * This function must be specialized for every convention we support.
 */
template<typename C>
double FromOneOverESquaredRadiusCF()
{
  static_assert(!std::is_same<C, C>::value, "No conversion factor defined for conversion.");
  return 0;
}

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
  return Constants::one_over_root_2<double>;
}

template<>
inline double FromOneOverESquaredRadiusCF<OneOverEDiameter>()
{
  return Constants::root_2<double>;
}

template<>
inline double FromOneOverESquaredRadiusCF<FWHMRadius>()
{
  static double cf = Constants::root_ln_2<double> / Constants::root_2<double>;
  return cf;
}

template<>
inline double FromOneOverESquaredRadiusCF<FWHMDiameter>()
{
  static double cf = Constants::root_ln_2<double> * Constants::root_2<double>;
  return cf;
}

/**
 * A function for computing the conversion factor between two beam width conventions
 */
template<typename C1, typename C2>
double BeamWidthConversionFactor()
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

// define a set of types for the various beam divergence conventions we support.
struct SecondMomentDivergence : public StrongQuantity<t::mrad> {
};
struct D4SigmaDivergence : public StrongQuantity<t::mrad> {
};
struct OneOverEHalfAngleDivergence : public StrongQuantity<t::mrad> {
};
struct OneOverESquaredHalfAngleDivergence : public StrongQuantity<t::mrad> {
};
struct FWHMHalfAngleDivergence : public StrongQuantity<t::mrad> {
};
struct OneOverEFullAngleDivergence : public StrongQuantity<t::mrad> {
};
struct OneOverESquaredFullAngleDivergence : public StrongQuantity<t::mrad> {
};
struct FWHMFullAngleDivergence : public StrongQuantity<t::mrad> {
};

/**
 * A function to compute divergence of some convention `Convention`
 * from the 1 over e squared halt angle. i.e.
 *
 * All we need to do is define how to compute each convention
 * from the 1/e squared half-angle divergence
 * and we can convert between all conventions.
 *
 * This function must be specialized for every convention we support.
 */
template<typename Convention>
double FromOneOverESquaredHalfAngleDivergenceCF()
{
  static_assert(!std::is_same<Convention, Convention>::value, "No conversion factor defined for conversion.");
  return 0;
}

template<>
inline double FromOneOverESquaredHalfAngleDivergenceCF<SecondMomentDivergence>()
{
  return 1;
}
template<>
inline double FromOneOverESquaredHalfAngleDivergenceCF<D4SigmaDivergence>()
{
  return 2;
}

template<>
inline double FromOneOverESquaredHalfAngleDivergenceCF<OneOverESquaredHalfAngleDivergence>()
{
  return 1;
}

template<>
inline double FromOneOverESquaredHalfAngleDivergenceCF<OneOverESquaredFullAngleDivergence>()
{
  return 2;
}

template<>
inline double FromOneOverESquaredHalfAngleDivergenceCF<OneOverEHalfAngleDivergence>()
{
  return Constants::one_over_root_2<double>;
}

template<>
inline double FromOneOverESquaredHalfAngleDivergenceCF<OneOverEFullAngleDivergence>()
{
  return Constants::root_2<double>;
}

template<>
inline double FromOneOverESquaredHalfAngleDivergenceCF<FWHMHalfAngleDivergence>()
{
  static double cf = Constants::root_ln_2<double> / Constants::root_2<double>;
  return cf;
}

template<>
inline double FromOneOverESquaredHalfAngleDivergenceCF<FWHMFullAngleDivergence>()
{
  static double cf = Constants::root_ln_2<double> * Constants::root_2<double>;
  return cf;
}

/**
 * A function for computing the conversion factor between two beam divergence conventions
 */
template<typename C1, typename C2>
double BeamDivergenceConversionFactor()
{
  // we have conversion factors for all conventions to 1/e squared half angle divergence.
  // to convert between any two conventions C1 and C2, we just need to convert C1 to 1/e
  // squared half-angle divergence, and then convert that to C2.
  //
  // C1 = factor_1 \omega
  // C2 = factor_2 \omega
  //
  // C2 = factor_2 (C1 / factor_1) = (factor_2/factor_1) C1
  return FromOneOverESquaredHalfAngleDivergenceCF<C2>() / FromOneOverESquaredHalfAngleDivergenceCF<C1>();
}

/**
 * A class that allows a beam "width" to be returned from a function
 * without specifying the specific convention to use. Later, the caller
 * can ask for the width in the convention they want.
 *
 * GaussianBeamWidth width = my_laser.getBeamWidth();
 *
 * quantity<t::cm> D = width.get<OneOverEDiameter>(); // get the 1/e diameter
 *
 * This keeps us from having to provide separate function calls for every width.
 * i.e.
 * quantity<t::cm> D = my_laser.getOneOverEDiameter();
 * quantity<t::cm> omega = my_laser.getOneOverESquaredRadius();
 * ...etc...
 * and allows us to add new conventiones without modifying the class.
 */
template<typename C, typename U>
class GaussianBeamWidth
{
 private:
  using CONVENTION = C;
  quantity<U> m_width;

 public:
  GaussianBeamWidth()                                            = default;
  GaussianBeamWidth(const GaussianBeamWidth& a_other)            = default;
  GaussianBeamWidth& operator=(const GaussianBeamWidth& a_other) = default;

  template<typename UU>
  GaussianBeamWidth(quantity<UU> a_width)
      : m_width(quantity<U>(a_width))
  {
  }

  template<typename CC, typename UU = U>
  quantity<UU> get() const
  {
    return quantity<UU>(
               m_width) *
           BeamWidthConversionFactor<C, CC>();
  }

  template<typename UU>
  GaussianBeamWidth& operator=(quantity<UU> a_width)
  {
    m_width = quantity<U>(a_width);
    return *this;
  }
  template<typename CC, typename UU>
  GaussianBeamWidth& operator=(GaussianBeamWidth<CC, UU> a_width)
  {
    m_width = a_width.template get<C, U>();
    return *this;
  }

  // allow constructing from a strong quantity
};

template<typename C, typename U>
GaussianBeamWidth<C, U> make_width(quantity<U> a_width)
{
  return GaussianBeamWidth<C, U>(a_width);
}

/**
 * A class that allows a beam "divergence" to be returned from a function
 * without specifying the specific convention to use. Later, the caller
 * can ask for the divergence in the convention they want.
 *
 * GaussianBeamDivergence div = my_laser.getBeamDivergence();
 *
 * quantity<t::mrad> D = div.get<OneOverEFullAngleDivergence>(); // get the 1/e full angle div
 *
 * This keeps us from having to provide separate function calls for every divergence.
 * i.e.
 * quantity<t::mrad> D = my_laser.getOneOverEFullAngleDivergence();
 * quantity<t::mrad> omega = my_laser.getOneOverESquaredHalfAngleDivergence();
 * ...etc...
 */
template<typename C, typename U>
class GaussianBeamDivergence
{
 private:
  using CONVENTION = C;
  quantity<U> m_divergence;
  //
 public:
  GaussianBeamDivergence()                                                 = default;
  GaussianBeamDivergence(const GaussianBeamDivergence& a_other)            = default;
  GaussianBeamDivergence& operator=(const GaussianBeamDivergence& a_other) = default;

  template<typename UU>
  GaussianBeamDivergence(quantity<UU> a_divergence)
      : m_divergence(quantity<U>(a_divergence))
  {
  }

  template<typename CC, typename UU = U>
  quantity<UU> get() const
  {
    return quantity<UU>(
               m_divergence) *
           BeamDivergenceConversionFactor<C, CC>();
  }

  template<typename UU>
  GaussianBeamDivergence& operator=(quantity<UU> a_divergence)
  {
    m_divergence = quantity<U>(a_divergence);
    return *this;
  }
  template<typename CC, typename UU>
  GaussianBeamDivergence& operator=(GaussianBeamDivergence<CC, UU> a_divergence)
  {
    m_divergence = a_divergence.template get<C, U>();
    return *this;
  }
};
template<typename C, typename U>
GaussianBeamDivergence<C, U> make_divergence(quantity<U> a_divergence)
{
  return GaussianBeamDivergence<C, U>(a_divergence);
}

}  // namespace libGBP2
