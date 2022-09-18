#pragma once

/** @file MediaStack.hpp
 * @brief
 * @author C.D. Clark III
 * @date 07/27/16
 */

#include <queue>

#include "Media/MediaInterface.hpp"

namespace libGBP
{
template<typename LengthUnitType>
class MediaStack
{
 public:
  typedef std::pair<boost::units::quantity<LengthUnitType>, Media_ptr<LengthUnitType> >
      BoundaryType;
  typedef std::list<BoundaryType> BoundariesType;

 protected:
  Media_ptr<LengthUnitType> backgroundMedia;
  BoundariesType            boundaries;

 public:
  MediaStack();

  MediaStack<LengthUnitType>& setBackgroundMedia(Media_ptr<LengthUnitType> abs)
  {
    this->backgroundMedia = abs;
    return *this;
  }
  Media_ptr<LengthUnitType> getBackgroundMedia()
  {
    return this->backgroundMedia;
  }

  template<typename U>
  MediaStack<LengthUnitType>& addBoundary(Media_ptr<LengthUnitType> abs,
                                          U                         position);

  const BoundariesType& getBoundaries() const;

  template<typename U, typename V>
  double getTransmission(U zi, V zf) const;

  void clear() { boundaries.clear(); }
};

template<typename T>
MediaStack<T>::MediaStack() : backgroundMedia(new BaseMedia<T>())
{
}

template<typename T>
template<typename U>
MediaStack<T>& MediaStack<T>::addBoundary(Media_ptr<T> abs, U position)
{
  boundaries.push_back({boost::units::quantity<T>(position), abs});
  boundaries.sort();
  return *this;
}

template<typename T>
auto MediaStack<T>::getBoundaries() const -> const BoundariesType&
{
  return boundaries;
}

template<typename T>
template<typename U, typename V>
double MediaStack<T>::getTransmission(U zi, V zf) const
{
  boost::units::quantity<T> zi_ = boost::units::quantity<T>(zi);
  boost::units::quantity<T> zf_ = boost::units::quantity<T>(zf);
  // . - initial point
  // x - final point
  //   . |          |   |     | x        // all boundaries are between zi and zf
  //   . |          |   |  x  |          // zi is before first boundary but zf
  //   is inside
  //     |  .       |   |  x  |          // both zi and zf are inside
  //     |    .  x  |   |     |          // no boundaries are between zi and zf

  // first, we need to figure out where zi and zf are w.r.t. the boundaries.
  // we determine what boundaries are between zi and zf
  std::queue<decltype(boundaries.begin())> incBoundaries;
  auto                                     currentMedia = backgroundMedia;
  for(auto it = boundaries.begin(); it != boundaries.end(); it++) {
    if(zi_ < it->first && it->first < zf_) incBoundaries.push(it);
    if(zi_ > it->first) currentMedia = it->second;
  }

  double                    transmission = 1;
  boost::units::quantity<T> zi__, zf__;
  zi__ = zi_;
  while(incBoundaries.size() > 0) {
    zf__ = incBoundaries.front()->first;
    transmission *= currentMedia->getTransmission(zi__, zf__);
    zi__         = zf__;
    currentMedia = incBoundaries.front()->second;
    incBoundaries.pop();
  }

  zf__ = zf_;
  transmission *= currentMedia->getTransmission(zi__, zf__);

  return transmission;
}

}  // namespace libGBP
