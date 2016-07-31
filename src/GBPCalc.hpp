#ifndef GBPCalc_hpp
#define GBPCalc_hpp

/** @file GBPCalc.hpp
  * @brief 
  * @author C.D. Clark III
  * @date 07/27/16
  */

#include <boost/property_tree/ptree.hpp>
#include "Builders/OpticalSystemBuilder.hpp"
#include "Builders/MediaStackBuilder.hpp"
#include "Builders/BeamBuilder.hpp"

/** @class GBPCalc
  * @brief Calculator class to peroform Gaussian Beam Propagation calculations.
  * @author C.D. Clark III
  */
template<typename LengthUnitType>
class GBPCalc
{
  protected:
    shared_ptr<OpticalSystem<LengthUnitType> > optics;
    shared_ptr<MediaStack<LengthUnitType> > media;
    shared_ptr<GaussianBeam> beam;


  public:

    void configure( const ptree& configTree );

    template<typename V>
    GaussianBeam getBeam( V z );

    void calculate();

};

/** Returns a Gaussian beam that corresponds to a given position. The current position of the beam
 * is set to z. WARNING: the beam's power is set to the power at the given z location. If the
 * location is inside of an absorbing material, then the beam's power will NOT be correct for any other location.
 */
template<typename T>
template<typename V>
GaussianBeam GBPCalc<T>::getBeam( V z )
{
  GaussianBeam beam = *(this->beam);

  double transmission = this->media->getTransmission( beam.getCurrentPosition(), z );
  beam.setPower( beam.getPower()*transmission );
  optics->transform( &beam, beam.getCurrentPosition(), z );
  beam.setCurrentPosition(z);

  return beam;
}

template<typename T>
void GBPCalc<T>::configure( const ptree& configTree )
{
  OpticalSystemBuilder<T> OSb;
  MediaStackBuilder<T> Mb;
  BeamBuilder Bb;


  this->beam.reset( Bb.build( configTree.get_child("beam") ) );
  this->media.reset( Mb.build( configTree.get_child("media_stack") ) );
  this->optics.reset( OSb.build( configTree.get_child("optical_system") ) );

}

#endif // include protector
