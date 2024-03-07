#include "./Messages.hpp"

#include <boost/lexical_cast.hpp>

namespace libGBP2
{
namespace msg
{
/**
 * Convert a Quantity message to a different unit.
 */
void convert(msg::Quantity& a_quantity, std::string a_unit)
{
  if(a_quantity.unit() == a_unit)
    return;

  static auto& ureg = UnitConvert::getGlobalUnitRegistry();
  auto         q    = ureg.makeQuantity<double>(a_quantity.value(), a_quantity.unit()).to(a_unit);
  a_quantity.set_value(q.value());
  a_quantity.set_unit(a_unit);
}

}  // namespace msg
}  // namespace libGBP2
   //
libgbp2_message_api::Quantity& operator<<(libgbp2_message_api::Quantity& a_quantity, std::string a_qstring)
{
  auto        idx = a_qstring.find(' ');
  double      val;
  std::string unit;

  if(idx == std::string::npos) {
    val  = boost::lexical_cast<double>(a_qstring);
    unit = "dimensionless";

  } else {
    val  = boost::lexical_cast<double>(a_qstring.substr(0, idx));
    unit = a_qstring.substr(idx + 1);
  }
  a_quantity.set_value(val);
  a_quantity.set_unit(unit);
  return a_quantity;
}

libgbp2_message_api::Quantity& operator<<(libgbp2_message_api::Quantity& a_quantity, const libgbp2_message_api::Quantity& a_quantity2)
{
  a_quantity.set_value(a_quantity2.value());
  a_quantity.set_unit(a_quantity2.unit());
  return a_quantity;
}

libgbp2_message_api::Quantity* operator<<(libgbp2_message_api::Quantity* a_quantity, const libgbp2_message_api::Quantity& a_quantity2)
{
  a_quantity->set_value(a_quantity2.value());
  a_quantity->set_unit(a_quantity2.unit());
  return a_quantity;
}

std::ostream& operator<<(std::ostream& a_os, const libgbp2_message_api::Quantity& a_quantity)
{
  a_os << a_quantity.value() << " " << a_quantity.unit();
  return a_os;
}
