#pragma once

/** @file OpticalElementBuilder.h
 * @brief
 * @author C.D. Clark III
 * @date 07/24/16
 */

/** @class OpticalElementBuilder
 * @brief A factory for building optical elements
 * @author C.D. Clark III
 */

#include "./Builder.hpp"
#include "../BeamTransformations/BeamTransformation_Interface.hpp"
#include "../BeamTransformations/SphericalInterface.hpp"
#include "../BeamTransformations/ThinLens.hpp"

namespace libGBP {

template<typename LengthUnitType>
class OpticalElementBuilder
    : public Builder<BeamTransformation_Interface<LengthUnitType>>
{
 public:
  OpticalElementBuilder();

  void configure(BeamTransformation_Interface<LengthUnitType>* elem,
                 const ptree&                             configTree);
};

template<typename T>
OpticalElementBuilder<T>::OpticalElementBuilder()
{
  this->addNameMapping("^thin[ _]*lens$",
                       "thinlens");  // allow "thinlens", "thin lens", and
                                     // "thin_lens" (or even "thin_ _lens").
  this->addNameMapping("^spherical[ _]*interface$", "sphericalinterface");

  this->addType("thinlens",
                createInstance<BeamTransformation_Interface<T>, ThinLens<T>>);
  this->addType(
      "sphericalinterface",
      createInstance<BeamTransformation_Interface<T>, SphericalInterface<T>>);
}

template<typename T>
void OpticalElementBuilder<T>::configure(BeamTransformation_Interface<T>* elem,
                                         const ptree&                configTree)
{
  // runtime type checking. dynamic_cast will return a nullptr is the elem
  // cannot be cast to a given pointer type.

  {  // these brackets allow us to reuse the name ptr for each call
    auto ptr = dynamic_cast<ThinLens<T>*>(elem);
    if (ptr != nullptr) {
      ptr->setFocalLength(configTree.get<double>("focal_length") * T());
    }
  }

  {
    auto ptr = dynamic_cast<Interface<T>*>(elem);
    if (ptr != nullptr) {
      ptr->setInitialRefractiveIndex(
          configTree.get<double>("refractive_index.initial"));
      ptr->setFinalRefractiveIndex(
          configTree.get<double>("refractive_index.final"));
    }
  }

  {
    auto ptr = dynamic_cast<SphericalInterface<T>*>(elem);
    if (ptr != nullptr) {
      ptr->setRadiusOfCurvature(configTree.get<double>("radius_of_curvature") *
                                T());
    }
  }
}

}
