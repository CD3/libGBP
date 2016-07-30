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
    OpticalSystem<LengthUnitType> optics;
    MediaStack<LengthUnitType> media;
    GaussianBeam beam;


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
}

template<typename T>
void GBPCalc<T>::configure( const ptree& configTree )
{
  OpticalSystemBuilder<T> OSb;
  MediaStackBuilder<T> Mb;
  BeamBuilder Bb;

}

#endif // include protector
