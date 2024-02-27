#include "./Messages.hpp"

#include <boost/lexical_cast.hpp>

namespace libGBP2
{
namespace msg
{
}  // namespace msg
}  // namespace libGBP2
   //
libgbp2_message_api::Quantity& operator<<(libgbp2_message_api::Quantity& a_quantity, std::string a_qstring)
{
  auto        idx = a_qstring.find(' ');
  double      val;
  std::string unit;

  if(idx == std::string::npos) {
    val  = boost::lexical_cast<double>(a_qstring.substr(0, idx));
    unit = a_qstring.substr(idx + 1);

  } else {
    val  = boost::lexical_cast<double>(a_qstring.substr(0, idx));
    unit = a_qstring.substr(idx + 1);
  }
  a_quantity.set_value(val);
  a_quantity.set_unit(unit);
  return a_quantity;
}
