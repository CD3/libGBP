#ifndef BeamConfigurator_hpp
#define BeamConfigurator_hpp

/** @file BeamConfigurator.hpp
  * @brief A class that can configure a GaussianBeam based on various scenarios.
  * @author C.D. Clark III
  * @date 07/04/16
  *
  * The beam configurator class manages configuring a GaussianBeam instance for a variety of situations so that the GaussianBeam
  * class can remain simple. The GaussianBeam class only allows the beam configuration to be specified by setting waist diameter,
  * the waist position, and the wavelength. Other common beam parameters (such as divergence) can be calculated, but they cannot
  * be set.
  *
  * Basically, three parameters must be known to characterize a Gaussian beam, and several sets of 3 parameters are commonly used.
  * For example, if the wavelength and divergence are known, then the beam waist diameter can be calculated. If the diameter at
  * a given position is also known, then the beam waist position can also be determined.
  *
  * Rather than allow for all of the configuration posibilities in the GaussianBeam class, they are handled here.
  */
#include <vector>
#include "GaussianBeam.hpp"

using std::vector;

struct BeamConfigurator
{
#define ADD_ATTRIBUTE(name, unit, n) \
  vector< quantity<unit> > name; \
  template<typename T> \
  BeamConfigurator& set##name( T v, int i = -1 )\
  {\
    if( i < 0 && this->name.size() < n )\
      this->name.push_back( quantity<unit>(v) );\
    if( i < 0 && this->name.size() >= n )\
      this->name[n-1] = quantity<unit>(v);\
    if( i >= 0 && i+1 < n )\
      this->name[i] = quantity<unit>(v);\
    return *this;\
  }\
  template<typename T> \
  optional< quantity<T> >    get##name( int i = 0 ){ if( i < name.size() ){ return quantity<T>(name[i]); } }\
  bool                       has##name( int i = 0 ){ if( i < name.size() ){ return true; }else{return false;} }\


  ADD_ATTRIBUTE(    Wavelength , t::nanometer   , 1);
  ADD_ATTRIBUTE(    Divergence , t::milliradian , 1);
  ADD_ATTRIBUTE(      Diameter , t::centimeter  , 2);
  ADD_ATTRIBUTE(      Position , t::centimeter  , 2);
  ADD_ATTRIBUTE( WaistPosition , t::centimeter  , 1);
  ADD_ATTRIBUTE( WaistDiameter , t::centimeter  , 1);
  ADD_ATTRIBUTE(         Power , t::watt        , 1);

  void configure( GaussianBeam* beam );
  void configure( GaussianBeam& beam ){ this->configure( &beam ); }
};

void
BeamConfigurator::configure( GaussianBeam* beam )
{
  if(this->hasPower())
  {
    beam->setPower( this->getPower<t::watt>().value() );
  }

  if(this->hasWavelength())
  {
    beam->setWavelength( this->getWavelength<t::nanometer>().value() );

    if(this->hasDivergence())
    { // if the divergence is known, then we can determine the beam waist diameter

      quantity<t::centimeter> waistDiameter = 4.*beam->getWavelength<t::centimeter>() / quantity_cast<double>( M_PI*this->getDivergence<t::radian>().value() );
      beam->setWaistDiameter( waistDiameter );

      if(this->hasDiameter())
      { // if the diameter at some location is known, then we can determine where the beam waist is

        quantity<t::centimeter> pos = 0*cm;
        if( this->hasPosition() )
          pos = this->getPosition<t::centimeter>().value();

        quantity<t::centimeter> waistPosition = pos - beam->getRayleighRange<t::centimeter>() * sqrt( pow<2>(this->getDiameter<t::centimeter>().value()/beam->getWaistDiameter<t::centimeter>()) - 1. );
        beam->setWaistPosition( waistPosition );
      }


    }
  }

}


#endif // include protector
