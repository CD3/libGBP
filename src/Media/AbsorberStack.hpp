#ifndef Media_AbsorberStack_hpp
#define Media_AbsorberStack_hpp

/** @file AbsorberStack.hpp
  * @brief 
  * @author C.D. Clark III
  * @date 07/27/16
  */

#include "LinearAbsorber.hpp"
#include <queue>


template<typename LengthUnitType>
class AbsorberStack
{
  public:
    typedef std::pair< quantity<LengthUnitType>, Absorber_ptr<LengthUnitType> > BoundaryType;
    typedef std::list<BoundaryType> BoundariesType;

  protected:
    Absorber_ptr<LengthUnitType> backgroundAbsorber;
    BoundariesType boundaries;

  public:

    AbsorberStack();

    AbsorberStack<LengthUnitType>& setBackgroundAbsorber( Absorber_ptr<LengthUnitType> abs ) {this->backgroundAbsorber = abs;}

    template<typename U>
    AbsorberStack<LengthUnitType>& addBoundary( Absorber_ptr<LengthUnitType> abs, U position );

    const BoundariesType& getBoundaries() const;

    template<typename U, typename V>
    double getTransmission( U zi, V zf ) const;

    void clear() {boundaries.clear();}

};

template<typename T>
AbsorberStack<T>::AbsorberStack()
:backgroundAbsorber( new Absorber<T>() )
{
  
}

template<typename T>
template<typename U>
AbsorberStack<T>& AbsorberStack<T>::addBoundary( Absorber_ptr<T> abs, U position )
{
  boundaries.push_back( { quantity<T>(position), abs} );
  boundaries.sort();
}

template<typename T>
auto AbsorberStack<T>::getBoundaries() const -> const BoundariesType&
{
  return boundaries;
}

template<typename T>
template<typename U, typename V>
double AbsorberStack<T>::getTransmission( U zi, V zf ) const
{
  quantity<T> zi_ = quantity<T>(zi);
  quantity<T> zf_ = quantity<T>(zf);
  // . - initial point
  // x - final point
  //   . |          |   |     | x        // all boundaries are between zi and zf
  //   . |          |   |  x  |          // zi is before first boundary but zf is inside
  //     |  .       |   |  x  |          // both zi and zf are inside
  //     |    .  x  |   |     |          // no boundaries are between zi and zf

  // first, we need to figure out where zi and zf are w.r.t. the boundaries.
  // we determine what boundaries are between zi and zf
  std::queue<decltype(boundaries.begin())> incBoundaries;
  auto currentAbsorber = backgroundAbsorber; 
  for( auto it = boundaries.begin(); it != boundaries.end(); it++ )
  {
    if( zi_ < it->first && it->first < zf_ )
      incBoundaries.push( it );
    if( zi_ > it->first )
      currentAbsorber = it->second;
  }

  double transmission = 1;
  quantity<T> zi__, zf__;
  zi__ = zi_;
  while( incBoundaries.size() > 0 )
  {
    zf__ = incBoundaries.front()->first;
    transmission *= currentAbsorber->getTransmission(zi__, zf__);
    zi__ = zf__;
    currentAbsorber = incBoundaries.front()->second;
    incBoundaries.pop();
  }

  zf__ = zf_;
  transmission *= currentAbsorber->getTransmission(zi__, zf__);

  

  return transmission;
}



#endif // include protector
