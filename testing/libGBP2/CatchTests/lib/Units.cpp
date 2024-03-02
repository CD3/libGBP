#include <complex>
#include <iostream>

#include <BoostUnitDefinitions/Units.hpp>
#include <boost/units/quantity.hpp>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <libGBP2/Units.hpp>

using namespace Catch;
using namespace libGBP2;

template<typename R, typename U1, typename U2>
quantity<R> calc_speed_1(quantity<U1> a_distance, quantity<U2> a_time)
{
  // without concepts, this will fail when we try to convert to a speed unit,
  // but only if return type is a speed unit. really this function could be named "divide",
  // it does not enforce any unit dimensions
  return quantity<R>(a_distance / a_time);
}

template<typename R, typename U1, typename U2>
quantity<R> calc_speed_2(quantity<U1> a_distance, quantity<U2> a_time)
{
  // We can add static asserts, but this takes a lot of boilerplate
  // so its easy to make an error because we end up copy-and-pasting a lot.
  static_assert(c::have_same_dimensions<U1, t::cm>::value, "a_distance must be a length.");
  static_assert(c::have_same_dimensions<U2, t::s>::value, "a_time must be a time.");
  static_assert(c::have_same_dimensions<R, decltype(i::m / i::s)>::value, "return type must be a speed.");
  return quantity<R>(a_distance / a_time);
}

template<c::Speed R, c::Length U1, c::Time U2>
quantity<R> calc_speed_3(quantity<U1> a_distance, quantity<U2> a_time)
{
  // this does not require any boiler plate
  return quantity<R>(a_distance / a_time);
}

TEST_CASE("Unit Concepts")
{
  SECTION("Speed Calculation")
  {
    quantity<t::m> L = 4 * i::m;
    quantity<t::s> t = 10 * i::s;
    SECTION("No concepts")
    {
      SECTION("No checks")
      {
        // this works
        auto v1 = calc_speed_1<decltype(i::ft / i::ms)>(L, t);
        CHECK(v1.value() == Approx(0.4 * 0.0032808399));
        // and wont compile if we try to convert to a unit with different dimensions
        CHECK_COMPILE_FAILS(
            calc_speed_1<t::m>(L, t);)

        // but this also, and it shouldn't
        auto v2 = calc_speed_1<decltype(i::ms / i::ft)>(t, L);
      }

      SECTION("Static Asserts")
      {
        // this works
        auto v1 = calc_speed_2<decltype(i::ft / i::ms)>(L, t);
        CHECK(v1.value() == Approx(0.4 * 0.0032808399));
        // and wont compile if we try to convert to a unit with different dimensions
        CHECK_COMPILE_FAILS(
            calc_speed_2<t::m>(L, t);)
        // or if arguments don't have the correct dimensions'
        CHECK_COMPILE_FAILS(
            calc_speed_2<decltype(i::ms / i::ft)>(t, L);)
      }
    }
    SECTION("With concepts")
    {
      // this works
      auto v1 = calc_speed_3<decltype(i::ft / i::ms)>(L, t);
      CHECK(v1.value() == Approx(0.4 * 0.0032808399));
      // and wont compile if we try to convert to a unit with different dimensions
      CHECK_COMPILE_FAILS(
          calc_speed_3<t::m>(L, t);)
      // or if arguments don't have the correct dimensions'
      CHECK_COMPILE_FAILS(
          calc_speed_3<decltype(i::ms / i::ft)>(t, L);)
    }
  }
}
