#ifndef Builders_OpticalSystemBuilder_hpp
#define Builders_OpticalSystemBuilder_hpp

/** @file OpticalSystemBuilder.hpp
 * @brief
 * @author C.D. Clark III
 * @date 07/25/16
 */

#include "Builder.hpp"
#include "../OpticalSystem.hpp"

template<typename LengthUnitType>
class OpticalSystemBuilder : public Builder<OpticalSystem<LengthUnitType> >
{
 public:
  void configure(OpticalSystem<LengthUnitType>* system,
                 const ptree&                   configTree);
};

template<typename T>
void OpticalSystemBuilder<T>::configure(OpticalSystem<T>* system,
                                        const ptree&      configTree)
{
  system->clear();
  auto elementsConfig = configTree.get_child_optional("elements");
  if (!elementsConfig) return;

  OpticalElementBuilder<T> builder;
  boost::units::quantity<T>              position;

  for (auto& iter : elementsConfig.value()) {
    position = iter.second.get<double>("position", 0) * T();
    system->addElement(BeamTransformation_ptr<T>(builder.build(iter.second)),
                       position);
  }
}

#endif  // include protector
