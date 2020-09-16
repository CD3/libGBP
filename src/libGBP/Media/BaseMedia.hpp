#ifndef Media_BaseMedia_hpp
#define Media_BaseMedia_hpp

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

#include "MediaInterface.hpp"

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

#endif  // include protector
