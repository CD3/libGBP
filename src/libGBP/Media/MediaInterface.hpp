#ifndef Media_MediaInterface_hpp
#define Media_MediaInterface_hpp

/** @file Media.hpp
 * @brief
 * @author C.D. Clark III
 * @date 07/27/16
 */

template<typename LengthUnitType>
class MediaInterface
{
 public:
  virtual double getTransmission(boost::units::quantity<LengthUnitType> zi,
                                 boost::units::quantity<LengthUnitType> zf)
      const = 0;  ///< returns percentage of power transmitted through absorber
                  ///< between positions zi and zf.
};

template<typename T>
using Media_ptr = std::shared_ptr<MediaInterface<T> >;

#endif  // include protector
