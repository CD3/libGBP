#ifndef OpticalSystem_hpp
#define OpticalSystem_hpp

/** @file OpticalSystem.hpp
  * @brief An optical system consists of a set of optical elements.
  * @author C.D. Clark III
  * @date 06/30/16
  */

#include "OpticalElements/OpticalElementInterface.hpp"

#include<list>
#include<utility>
#include<memory>


template<typename LengthUnitType>
class OpticalSystem
{
  public:
    typedef std::shared_ptr<OpticalElementInterface<LengthUnitType> > OpticalElement_ptr;
    typedef std::pair< quantity<LengthUnitType>, OpticalElement_ptr > ElementType;
    typedef std::list<ElementType> ElementsType;

  protected:
    ElementsType elements;

  public:
    template<typename U>
    OpticalSystem<LengthUnitType>& addElement( OpticalElement_ptr elem, U position );

    const ElementsType& getElements() const;

    template<typename U, typename V>
    GaussianBeam transform( const GaussianBeam& beam, U zi, V zf );

    GaussianBeam transform( const GaussianBeam& beam );
};

template<typename T>
template<typename U>
OpticalSystem<T>& OpticalSystem<T>::addElement( OpticalElement_ptr elem, U position )
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
    return this->transform( beam, elements.front().first - 1*T(), elements.back().first + 1*T() );
  else
    return beam;
}


#endif // include protector
