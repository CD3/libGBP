#ifndef Units_hpp
#define Units_hpp

/** @file Units.hpp
  * @brief Unit definitions.
  * @author C.D. Clark III
  * @date 06/28/16
  */

#include <boost/units/systems/si.hpp>
#include <boost/units/make_scaled_unit.hpp>
#include <boost/units/cmath.hpp>
#include <boost/units/io.hpp>
#include <boost/units/conversion.hpp>

namespace boost
{
namespace units
{
  /** @brief namespace for unit types */
  namespace t
  {
    typedef si::dimensionless dimensionless;

    typedef si::time second;

    typedef si::frequency hertz;
    typedef make_scaled_unit< hertz, scale<10, static_rational< 3> > >::type kilohertz;
    typedef make_scaled_unit< hertz, scale<10, static_rational< 6> > >::type megaertz;
    typedef make_scaled_unit< hertz, scale<10, static_rational< 9> > >::type gigahertz;
    typedef make_scaled_unit< hertz, scale<10, static_rational<12> > >::type terahertz;

    typedef si::length meter;
    typedef make_scaled_unit< meter, scale<10, static_rational<-2> > >::type centimeter;
    typedef make_scaled_unit< meter, scale<10, static_rational<-3> > >::type millimeter;
    typedef make_scaled_unit< meter, scale<10, static_rational<-6> > >::type micrometer;
    typedef make_scaled_unit< meter, scale<10, static_rational<-9> > >::type  nanometer;
    typedef make_scaled_unit< meter, scale<10, static_rational< 3> > >::type	kilometer;

    typedef divide_typeof_helper <      meter, second>::type meter_per_second;
    typedef divide_typeof_helper < centimeter, second>::type centimeter_per_second;

    typedef si::area meter_squared;
    typedef make_scaled_unit< meter_squared,   scale<10, static_rational<-4> > >::type centimeter_squared;

    typedef si::plane_angle radian;
    typedef make_scaled_unit< radian, scale<10, static_rational<-3> > >::type	milliradian;

    typedef si::energy    joule;
    typedef si::power     watt;

    typedef divide_typeof_helper < joule, centimeter_squared >::type radiant_exposure;
    typedef divide_typeof_helper < watt,  centimeter_squared >::type irradiance;      
  }
  /** @brief namespace for unit instances*/
  namespace i
  {
    BOOST_UNITS_STATIC_CONSTANT( nanometer, t::nanometer );
    BOOST_UNITS_STATIC_CONSTANT( nm,         t::nanometer );

    BOOST_UNITS_STATIC_CONSTANT( micrometer, t::micrometer );
    BOOST_UNITS_STATIC_CONSTANT( um,         t::micrometer );

    BOOST_UNITS_STATIC_CONSTANT( millimeter, t::millimeter );
    BOOST_UNITS_STATIC_CONSTANT( mm,         t::millimeter );

    BOOST_UNITS_STATIC_CONSTANT( centimeter, t::centimeter );
    BOOST_UNITS_STATIC_CONSTANT( cm,         t::centimeter );

  }
}
}


using namespace boost;
using namespace boost::units;
using namespace boost::units::i;
 


#endif // include protector
