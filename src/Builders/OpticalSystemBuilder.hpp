#ifndef Builders_OpticalSystemBuilder_hpp
#define Builders_OpticalSystemBuilder_hpp

/** @file OpticalSystemBuilder.hpp
  * @brief 
  * @author C.D. Clark III
  * @date 07/25/16
  */

#include <boost/property_tree/ptree.hpp>
#include "OpticalSystem.hpp"
#include "Builder.hpp"

using boost::property_tree::ptree;

template<typename LengthUnitType>
class OpticalSystemBuilder : public Builder< OpticalSystem<LengthUnitType> >
{

  public:
    void configure( OpticalSystem<LengthUnitType>* elem, const ptree& configTree );
};

template<typename T>
void
OpticalSystemBuilder<T>::configure( OpticalSystem<T>* system, const ptree& configTree )
{

  system->clear();
  auto elementsConfig = configTree.get_child_optional("elements");
  if(!elementsConfig)
    return;

  OpticalElementBuilder<T> builder;
  quantity<T> position;

  for( auto &iter: elementsConfig.value() )
  {
    position = iter.second.get<double>("position", 0) * T();
    system->addElement( OpticalElement_ptr<T>( builder.build( iter.second ) ), position );
  }


}

#endif // include protector
