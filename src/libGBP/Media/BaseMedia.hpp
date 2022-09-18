#pragma once

/** @file BaseMedia.hpp
 * @brief
 * @author C.D. Clark III
 * @date 07/29/16
 */

/** @class BaseMedia
 * @brief Base class for media classes that provides default implementations for
 * the required methods.
 * @author C.D. Clark III
 */

#include "./MediaInterface.hpp"

namespace libGBP {

template<typename LengthUnitType>
class BaseMedia : public MediaInterface<LengthUnitType>
{
 public:
  virtual double getTransmission(boost::units::quantity<LengthUnitType> zi,
                                 boost::units::quantity<LengthUnitType> zf) const
  {
    return 1;
  }
};

}
