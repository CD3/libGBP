#pragma once

/** @file Units.hpp
 * @brief Unit definitions.
 * @author C.D. Clark III
 * @date 06/28/16
 */

#include <BoostUnitDefinitions/Units.hpp>
#include <boost/units/quantity.hpp>

namespace libGBP {
  namespace units {
  namespace t = boost::units::t;
  namespace i = boost::units::i;
  using namespace boost::units::i;
  using boost::units::quantity;
  }
  using namespace units; // allow using t::<unit> and <unit> once libGBP namespace is open/used.
}
