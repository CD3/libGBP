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

#include "OpticalElements/OpticalElementInterface.hpp"
#include "Builder.hpp"

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

  this->addNameMapping("^thin[ _]*lens$", "thinlens");  // allow "thinlens", "thin lens", and "thin_lens" (or even "thin_ _lens").
  this->addNameMapping("^spherical[ _]*interface$", "sphericalinterface");

  this->addType("thinlens", createInstance<OpticalElementInterface<T>, ThinLens<T>>);
  this->addType("sphericalinterface", createInstance<OpticalElementInterface<T>, SphericalInterface<T>>);
}


template<typename T>
void OpticalElementBuilder<T>::configure( OpticalElement_ptr<T> elem, const ptree& configTree )
{
  std::string typeName = this->getTypeName( configTree.get("type", "UNKNOWN") );

  if( typeName == "thinlens" )
  {
    typedef ThinLens<T> ElemType;
    ElemType& lens = *std::dynamic_pointer_cast<ElemType>(elem);
    lens.setFocalLength( configTree.get<double>("focal_length")*T() );
  }

  if( typeName == "sphericalinterface" )
  {
    typedef SphericalInterface<T> ElemType;
    ElemType& interface = *std::dynamic_pointer_cast<ElemType>(elem);
    interface.setRadiusOfCurvature(      configTree.get<double>("radius_of_curvature")*T() );
    interface.setInitialRefractiveIndex( configTree.get<double>("refractive_index.initial") );
    interface.setFinalRefractiveIndex(   configTree.get<double>("refractive_index.final") );
  }
}





#endif // include protector
