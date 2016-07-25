#ifndef OpticalSystem_hpp
#define OpticalSystem_hpp

/** @file OpticalSystem.hpp
  * @brief An optical system consists of a set of optical elements.
  * @author C.D. Clark III
  * @date 06/30/16
  */

#include "OpticalElements/OpticalElementInterface.hpp"
#include "OpticalElements/OpticalElementBuilder.hpp"

#include<list>
#include<utility>
#include<memory>
#include<boost/property_tree/json_parser.hpp>


template<typename LengthUnitType>
class OpticalSystem
{
  public:
    typedef std::pair< quantity<LengthUnitType>, OpticalElement_ptr<LengthUnitType> > ElementType;
    typedef std::list<ElementType> ElementsType;

  protected:
    ElementsType elements;

  public:
    template<typename U>
    OpticalSystem<LengthUnitType>& addElement( OpticalElement_ptr<LengthUnitType> elem, U position );

    const ElementsType& getElements() const;

    void configure( const ptree& configTree );

    template<typename U, typename V>
    GaussianBeam transform( const GaussianBeam& beam, U zi, V zf );

    GaussianBeam transform( const GaussianBeam& beam );
};

template<typename T>
template<typename U>
OpticalSystem<T>& OpticalSystem<T>::addElement( OpticalElement_ptr<T> elem, U position )
{
  elements.push_back( { quantity<T>(position), elem } );
  elements.sort();
}

template<typename T>
auto OpticalSystem<T>::getElements() const -> const ElementsType&
{
  return elements;
}

template<typename T>
void OpticalSystem<T>::configure(const ptree& configTree)
{
  elements.clear();
  auto elementsConfig = configTree.get_child_optional("elements");
  if(!elementsConfig)
    return;

  OpticalElementBuilder<T> builder;
  quantity<T> position;

  for( auto &iter: elementsConfig.value() )
  {
    position = iter.second.get<double>("position", 0) * T();
    this->addElement( builder.build( iter.second ), position );
  }

}

template<typename T>
template<typename U, typename V>
GaussianBeam OpticalSystem<T>::transform( const GaussianBeam& beam, U zi, V zf )
{
  GaussianBeam beam2 = beam;

  for( auto it = elements.begin(); it != elements.end(); it++ )
  {
    // only apply elements that are between zi and zf
    if( it->first >= quantity<T>(zi) && it->first <= quantity<T>(zf) )
      beam2.transform( it->second.get(), it->first );
  }

  return beam2;
}

template<typename T>
GaussianBeam OpticalSystem<T>::transform( const GaussianBeam& beam )
{
  if(!elements.empty())
    return this->transform( beam, elements.front().first, elements.back().first );
  else
    return beam;
}


#endif // include protector
