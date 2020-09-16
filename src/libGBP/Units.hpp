#pragma once

/** @file Units.hpp
 * @brief Unit definitions.
 * @author C.D. Clark III
 * @date 06/28/16
 */

#include <BoostUnitDefinitions/Units.hpp>
#include <boost/units/quantity.hpp>

namespace units
{
  namespace t = boost::units::t;
  namespace i = boost::units::i;
}
#ifndef LIBGBP_NO_BACKWARD_COMPATIBLE_UNITS_NAMESPACES
using namespace boost::units;
using namespace boost::units::i;
#pragma message ( "WARNING: Future versions of libGBP library will no longer adds the boost::units or boost::units::i namespaces global scope. To maintain backward compatibility, they are being imported to the global namespace. To disable this, define LIBGBP_NO_BACKWARD_COMPATIBLE_UNITS_NAMESPACES before including the libGBP headers. This will require you to qualify names in the boost::units namespace (i.e. boost::units::quantity instead of quantity) and unit instances (i.e. boost::units::i::cm instead of cm)." )
#endif
