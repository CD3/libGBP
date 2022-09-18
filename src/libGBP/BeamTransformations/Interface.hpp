#pragma once

/** @file Interface.hpp
 * @brief Base class for media interfaces, i.e. a change in refractive index.
 * This causes a change in the wavelength when the interface is crossed.
 * @author C.D. Clark III
 * @date 07/01/16
 */

#include "./BeamTransformation_Base.hpp"

template<typename T>
class Interface : public BeamTransformation_Base<T>
{
 public:
  // reimplement the wavelength scale factor
  double getWavelengthScaleFactor() const
  {
    return this->initialRefractiveIndex / this->finalRefractiveIndex;
  }

 protected:
  double initialRefractiveIndex;
  double finalRefractiveIndex;

 public:
  // initialRefractiveIndex getters and setters
  void setInitialRefractiveIndex(double v)
  {
    this->initialRefractiveIndex = v;
  }  ///< sets initialRefractiveIndex
  double getInitialRefractiveIndex() const
  {
    return this->initialRefractiveIndex;
  }  ///< returns initialRefractiveIndex

  // finalRefractiveIndex getters and setters
  void setFinalRefractiveIndex(double v)
  {
    this->finalRefractiveIndex = v;
  }  ///< sets finalRefractiveIndex
  double getFinalRefractiveIndex() const
  {
    return this->finalRefractiveIndex;
  }  ///< returns finalRefractiveIndex
};
