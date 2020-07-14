#ifndef Units_hpp
#define Units_hpp

/** @file Units.hpp
 * @brief Unit definitions.
 * @author C.D. Clark III
 * @date 06/28/16
 */

#include <boost/units/cmath.hpp>
#include <boost/units/conversion.hpp>
#include <boost/units/io.hpp>
#include <boost/units/make_scaled_unit.hpp>
#include <boost/units/systems/si.hpp>
#include <boost/units/systems/si/io.hpp>

namespace boost
{
namespace units
{
/** @brief namespace for unit types */
namespace t
{
typedef si::dimensionless dimensionless;

typedef si::time second;
typedef second   s;

typedef si::frequency hertz;
typedef hertz         Hz;
typedef make_scaled_unit<hertz, scale<10, static_rational<3> > >::type
                  kilohertz;
typedef kilohertz kHz;
typedef make_scaled_unit<hertz, scale<10, static_rational<6> > >::type
                  megahertz;
typedef megahertz MHz;
typedef make_scaled_unit<hertz, scale<10, static_rational<9> > >::type
                  gigahertz;
typedef gigahertz GHz;
typedef make_scaled_unit<hertz, scale<10, static_rational<12> > >::type
                  terahertz;
typedef terahertz THz;

typedef si::length meter;
typedef meter      m;
typedef make_scaled_unit<meter, scale<10, static_rational<-2> > >::type
                   centimeter;
typedef centimeter cm;
typedef make_scaled_unit<meter, scale<10, static_rational<-3> > >::type
                   millimeter;
typedef millimeter mm;
typedef make_scaled_unit<meter, scale<10, static_rational<-6> > >::type
                   micrometer;
typedef micrometer um;
typedef make_scaled_unit<meter, scale<10, static_rational<-9> > >::type
                  nanometer;
typedef nanometer nm;
typedef make_scaled_unit<meter, scale<10, static_rational<3> > >::type
                  kilometer;
typedef kilometer km;

typedef divide_typeof_helper<meter, second>::type      meter_per_second;
typedef divide_typeof_helper<centimeter, second>::type centimeter_per_second;

typedef si::area meter_squared;
typedef make_scaled_unit<meter_squared, scale<10, static_rational<-4> > >::type
    centimeter_squared;

typedef si::plane_angle radian;
typedef radian          rad;
typedef make_scaled_unit<radian, scale<10, static_rational<-3> > >::type
                    milliradian;
typedef milliradian mrad;

typedef si::energy joule;
typedef joule      J;

typedef si::power watt;
typedef watt      W;
typedef make_scaled_unit<watt, scale<10, static_rational<-3> > >::type
                  milliwatt;
typedef milliwatt mW;

typedef divide_typeof_helper<joule, centimeter_squared>::type radiant_exposure;
typedef radiant_exposure joule_per_centimeter_squared;

typedef divide_typeof_helper<watt, centimeter_squared>::type irradiance;
typedef irradiance watt_per_centimeter_squared;

typedef si::electric_potential volt;
typedef volt                   V;

typedef divide_typeof_helper<volt, meter>::type volt_per_meter;
typedef volt_per_meter                          V_p_m;

}  // namespace t
/** @brief namespace for unit instances*/
namespace i
{
BOOST_UNITS_STATIC_CONSTANT(hertz, t::hertz);
BOOST_UNITS_STATIC_CONSTANT(Hz, t::hertz);

BOOST_UNITS_STATIC_CONSTANT(nanometer, t::nanometer);
BOOST_UNITS_STATIC_CONSTANT(nm, t::nanometer);

BOOST_UNITS_STATIC_CONSTANT(micrometer, t::micrometer);
BOOST_UNITS_STATIC_CONSTANT(um, t::micrometer);

BOOST_UNITS_STATIC_CONSTANT(millimeter, t::millimeter);
BOOST_UNITS_STATIC_CONSTANT(mm, t::millimeter);

BOOST_UNITS_STATIC_CONSTANT(centimeter, t::centimeter);
BOOST_UNITS_STATIC_CONSTANT(cm, t::centimeter);

BOOST_UNITS_STATIC_CONSTANT(meter, t::meter);
BOOST_UNITS_STATIC_CONSTANT(m, t::meter);

BOOST_UNITS_STATIC_CONSTANT(milliwatt, t::milliwatt);
BOOST_UNITS_STATIC_CONSTANT(mW, t::milliwatt);

BOOST_UNITS_STATIC_CONSTANT(watt, t::watt);
BOOST_UNITS_STATIC_CONSTANT(W, t::watt);

BOOST_UNITS_STATIC_CONSTANT(milliradian, t::milliradian);
BOOST_UNITS_STATIC_CONSTANT(mrad, t::milliradian);
BOOST_UNITS_STATIC_CONSTANT(radian, t::radian);
BOOST_UNITS_STATIC_CONSTANT(rad, t::radian);

}  // namespace i

/** @brief namespace for unit conversions*/
namespace c
{
typedef quantity<t::cm> to_cm;
}

}  // namespace units
}  // namespace boost

using namespace boost;
using namespace boost::units;
using namespace boost::units::i;


// define multiplication between int's and quantity<unit>'s
// otherwise, we can't do this
// quantity<t::m> L(3.*m);
// quantity<t::m> L2 = 3*L; // error
// quantity<t::m> L2 = 3.*L; // OK
template <typename U, typename T>
boost::units::quantity<U, T> operator*(int a, boost::units::quantity<U, T> b)
{
  return static_cast<T>(a) * b;
}

template <typename U, typename T>
boost::units::quantity<U, T> operator*(boost::units::quantity<U, T> b, int a)
{
  return static_cast<T>(a) * b;
}

// define multiplication between int's and units, so that template type
// deduction will work.
// otherwise, we can't do this
//
// template<typename U>
// void Func( quantity<U> q );
// ...
// Func(3*m); // error
// Func(3.*m); // OK
template <typename U, typename T>
boost::units::quantity<boost::units::unit<U, T> > operator*(
    int a, boost::units::unit<U, T> u)
{
  return static_cast<double>(a) * u;
}

template <typename U, typename T>
boost::units::quantity<boost::units::unit<U, T> > operator*(
    boost::units::unit<U, T> u, int a)
{
  return static_cast<double>(a) * u;
}

// define division of quantity<unit>'s by ints
// otherwise, we can't do this
// quantity<t::m> L(3.*m);
// quantity<t::m> L2 = L/3; // error
// quantity<t::m> L2 = L/3.; // OK
template <typename U, typename T>
boost::units::quantity<U, T>
operator/(boost::units::quantity<U, T> b, int a)
{
  return b / static_cast<T>(a);
}

// define division of int by quantity<unit>
// otherwise, we can't do this
// quantity<t::s> T(2.*s);
// quantity<t::Hz> L2 = 1/T; // error
// quantity<t::Hz> L2 = 1./T.; // OK
template <typename U, typename T>
boost::units::quantity<typename boost::units::divide_typeof_helper<t::dimensionless, U>::type, T>
operator/(int a, boost::units::quantity<U, T> b)
{
  return static_cast<T>(a)/b;
}


#endif  // include protector