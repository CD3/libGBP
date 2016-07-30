#ifndef Builders_MediaBuilder_hpp
#define Builders_MediaBuilder_hpp

/** @file MediaBuilder.hpp
  * @brief 
  * @author C.D. Clark III
  * @date 07/29/16
  */

#include "Media/MediaInterface.hpp"
#include "Builder.hpp"

/** @class MediaBuilder
  * @brief 
  * @author C.D. Clark III
  */
template<typename LengthUnitType>
class MediaBuilder : public Builder< MediaInterface<LengthUnitType> >
{
  public:
    MediaBuilder ();

    void configure( MediaInterface<LengthUnitType>* media, const ptree& configTree );

};

template<typename T>
MediaBuilder<T>::MediaBuilder()
{

  this->addNameMapping("^linear[ _]*absorber$", "linearabsorber");

  this->addType("linearabsorber", createInstance<MediaInterface<T>, LinearAbsorber<T>>);
}

template<typename T>
void MediaBuilder<T>::configure( MediaInterface<T>* media, const ptree& configTree )
{
  {
    auto ptr = dynamic_cast<LinearAbsorber<T>*>(media);
    if( ptr != nullptr )
    {
      ptr->setAbsorptionCoefficient( configTree.get<double>("absorption_coefficient")/T() );
    }
  }

}

#endif // include protector
