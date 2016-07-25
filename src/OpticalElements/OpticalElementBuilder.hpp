#ifndef OpticalElements_OpticalElementBuilder_h
#define OpticalElements_OpticalElementBuilder_h

/** @file OpticalElementBuilder.h
  * @brief 
  * @author C.D. Clark III
  * @date 07/24/16
  */

/** @class OpticalElementBuilder
  * @brief A factory for building optical elements
  * @author C.D. Clark III
  */

#include "OpticalElementInterface.hpp"

template<typename LengthUnitType>
class OpticalElementBuilder : public Builder< OpticalElementInterface<LengthUnitType> >
{

  public:
    OpticalElementBuilder();

    void configure( OpticalElement_ptr<LengthUnitType> elem, const ptree& configTree );

};

template<typename T>
OpticalElementBuilder<T>::OpticalElementBuilder()
{

  this->addNameMapping("thin[ _]*lens", "thinlens");  // allow "thinlens", "thin lens", and "thin_lens" (or even "thin_ _lens").

  this->addType("thinlens", createInstance<OpticalElementInterface<T>, ThinLens<T>>);
}


template<typename T>
void OpticalElementBuilder<T>::configure( OpticalElement_ptr<T> elem, const ptree& configTree )
{
  std::string typeName = this->getTypeName( configTree.get("type", "UNKNOWN") );

  if( typeName == "thinlens" )
  {
    ThinLens<T>& lens = *std::dynamic_pointer_cast<ThinLens<T>>(elem);
    lens.setFocalLength( configTree.get<double>("focal_length")*T() );

  }
}





#endif // include protector
