#ifndef BeamBuilder_hpp
#define BeamBuilder_hpp

/** @file BeamBuilder.hpp
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
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "GaussianBeam.hpp"
#include "Builder.hpp"

using std::vector;

struct BeamBuilder : public Builder<GaussianBeam>
{
#define ADD_ATTRIBUTE(name, unit, n) \
  vector< quantity<unit> > name; \
  template<typename T> \
  BeamBuilder& set##name( T v, int i = -1 )\
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


  ADD_ATTRIBUTE(    Wavelength        , t::nanometer   , 1);
  ADD_ATTRIBUTE( FreeSpaceWavelength  , t::nanometer   , 1);
  ADD_ATTRIBUTE(            Frequency , t::nanometer   , 1);
  ADD_ATTRIBUTE(    Divergence        , t::milliradian , 1);
  ADD_ATTRIBUTE(      Diameter        , t::centimeter  , 2);
  ADD_ATTRIBUTE(      Position        , t::centimeter  , 2);
  ADD_ATTRIBUTE( WaistPosition        , t::centimeter  , 1);
  ADD_ATTRIBUTE( WaistDiameter        , t::centimeter  , 1);
  ADD_ATTRIBUTE(         Power        , t::watt        , 1);
  ADD_ATTRIBUTE( CurrentPosition      , t::centimeter  , 1);

  void configure( GaussianBeam* beam );
  void configure( GaussianBeam& beam ){ this->configure( &beam ); }

  void configure( GaussianBeam* beam, const ptree& configTree ); ///< required by the base class
  void configure( GaussianBeam& beam, const ptree& configTree ){ this->configure( &beam, configTree ); }
};

void
BeamBuilder::configure( GaussianBeam* beam )
{
  if(this->hasWavelength())
    beam->setWavelength( this->getWavelength<t::nanometer>().value() );

  if(this->hasPower())
    beam->setPower( this->getPower<t::watt>().value() );

  if(this->hasWaistDiameter())
    beam->setWaistDiameter( this->getWaistDiameter<t::centimeter>().value() );

  if(this->hasWaistPosition())
    beam->setWaistPosition( this->getWaistPosition<t::centimeter>().value() );

  if(this->hasWavelength())
  {
    if(this->hasDivergence())
    { // if the divergence is known, then we can determine the beam waist diameter

      // \Theta   = \frac{2\lambda}{\pi \omega_0}
      // \omega_0 = \frac{2\lambda}{\pi \Theta  }
      quantity<t::centimeter> waistRadius = 2.*beam->getWavelength<t::centimeter>() / quantity_cast<double>( M_PI*this->getDivergence<t::radian>().value() );
      beam->setWaistRadius( waistRadius );

      if(this->hasDiameter())
      { // if the diameter at some location is known, then we can determine where the beam waist is

        if(this->getDiameter<t::centimeter>() < 2.*waistRadius)
          throw std::runtime_error("BEAM CONFIGURATION ERROR: configured beam diameter is smaller than the beam waist diameter.");


        quantity<t::centimeter> pos = 0*cm;
        if( this->hasPosition() )
          pos = this->getPosition<t::centimeter>().value();

        quantity<t::centimeter> waistPosition = pos - beam->getRayleighRange<t::centimeter>() * sqrt( pow<2>(this->getDiameter<t::centimeter>().value()/beam->getWaistDiameter<t::centimeter>()) - 1. );
        beam->setWaistPosition( waistPosition );
      }


    }
  }

  if(!this->hasFrequency())
    beam->setFrequency( constants::SpeedOfLight / (this->hasFreeSpaceWavelength() ? this->getFreeSpaceWavelength<t::nanometer>().value() : this->getWavelength<t::nanometer>().value() ) );

  if(this->hasCurrentPosition())
    beam->setCurrentPosition( this->getCurrentPosition<t::centimeter>().value() );

}

void
BeamBuilder::configure( GaussianBeam* beam, const ptree& configTree )
{
  BeamBuilder builder;
  const ptree* tree;
  tree = &configTree;
#define SET( name, key, val )\
  {\
  auto param = tree->get_optional<double>( key );\
  if(param)\
  {\
    auto value = param.value();\
    builder.set##name( val );\
  }\
  }

  SET( Wavelength, "wavelength", value*nm );
  SET( Power, "power", value*W );
  SET( Divergence, "divergence", value*mrad );
  SET( WaistPosition, "waist.position", value*cm);
  SET( WaistDiameter, "waist.diameter", value*cm);
  SET( CurrentPosition, "current_position", value*cm);

  auto knowns = configTree.get_child_optional("knowns");
  if( knowns )
  {
    for( auto &iter: knowns.value() )
    {
      tree = &iter.second;
      SET( Position, "position", value*cm );
      SET( Diameter, "diameter", value*cm );
      SET( Diameter, "radius",   2.*value*cm );
    }

  }

  builder.configure( beam );

#undef SET
}

#endif // include protector
