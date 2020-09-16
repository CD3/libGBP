#include "catch.hpp"

/**
 * This file just provides another translational unit that will be linked into the unit test
 * binary to check that we don't get multiply defined symbols.
 */

#include <libGBP/GaussianBeam.hpp>
SCENARIO("GaussianBeam ODR" )
{
  GaussianBeam beam;
  beam.setWavelength( 810*i::nm );
}
