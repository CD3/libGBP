#ifndef OpticalSystem_hpp
#define OpticalSystem_hpp

/** @file OpticalSystem.hpp
  * @brief An optical system consists of a set of optical elements.
  * @author C.D. Clark III
  * @date 06/30/16
  */

#include "OpticalElement.hpp"


class OpticalSystem
{
  public:
    void addElement( OpticalElementInterface *elem, U position );

    GaussianBeam transform( const GaussianBeam& beam );
}


#endif // include protector
