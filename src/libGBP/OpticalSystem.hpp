#pragma once

/** @file OpticalSystem.hpp
 * @brief An optical system consists of a set of optical elements.
 * @author C.D. Clark III
 * @date 06/30/16
 */

#include <list>
#include <memory>
#include <utility>

#include "Builders/OpticalElementBuilder.hpp"
#include "GaussianBeam.hpp"
#include "BeamTransformations/BeamTransformation_Interface.hpp"

namespace libGBP {

template<typename LengthUnitType>
class OpticalSystem
{
 public:
  typedef std::pair<boost::units::quantity<LengthUnitType>,
                    BeamTransformation_ptr<LengthUnitType> >
                                 ElementType;
  typedef std::list<ElementType> ElementsType;

 protected:
  ElementsType elements;

 public:
  template<typename U>
  OpticalSystem<LengthUnitType>& addElement(
      BeamTransformation_ptr<LengthUnitType> elem, U position);

  const ElementsType& getElements() const;

  template<typename U, typename V>
  GaussianBeam transform(const GaussianBeam& beam, U zi, V zf);
  GaussianBeam transform(const GaussianBeam& beam);

  template<typename U, typename V>
  void transform(GaussianBeam* beam, U zi, V zf);
  void transform(GaussianBeam* beam);

  void clear() { elements.clear(); }
};

template<typename T>
template<typename U>
OpticalSystem<T>& OpticalSystem<T>::addElement(BeamTransformation_ptr<T> elem,
                                               U                     position)
{
  elements.push_back({boost::units::quantity<T>(position), elem});
  elements.sort();
  return *this;
}

template<typename T>
auto OpticalSystem<T>::getElements() const -> const ElementsType&
{
  return elements;
}

template<typename T>
template<typename U, typename V>
GaussianBeam OpticalSystem<T>::transform(const GaussianBeam& beam, U zi, V zf)
{
  GaussianBeam beam2 = beam;
  this->transform(&beam2, zi, zf);

  return beam2;
}

template<typename T>
GaussianBeam OpticalSystem<T>::transform(const GaussianBeam& beam)
{
  return this->transform(beam, elements.front().first, elements.back().first);
}

template<typename T>
template<typename U, typename V>
void OpticalSystem<T>::transform(GaussianBeam* beam, U zi, V zf)
{
  for (auto it = elements.begin(); it != elements.end(); it++) {
    // only apply elements that are between zi and zf
    if (it->first >= boost::units::quantity<T>(zi) && it->first <= boost::units::quantity<T>(zf))
      beam->transform(it->second.get(), it->first);
  }
}

template<typename T>
void OpticalSystem<T>::transform(GaussianBeam* beam)
{
  this->transform(beam, elements.front().first, elements.back().first);
}

}
