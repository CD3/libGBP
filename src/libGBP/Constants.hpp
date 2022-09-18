#include "./Units.hpp"

namespace libGBP {
namespace constants
{
const boost::units::quantity<units::t::meter_per_second> SpeedOfLight =
    299792458. * units::t::meter_per_second();
}
}
