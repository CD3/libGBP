#ifndef Propagator_hpp
#define Propagator_hpp

/** @file Propagator.hpp
  * @brief 
  * @author C.D. Clark III
  * @date 07/27/16
  */

#include <OpticalSystem.hpp>
#include <AbsorberStack.hpp>
#include <GaussianBeam.hpp>

class Propagator
{
  protected:
    OpticalSystem optics;
    AbsorberStack absorbers;
    std::vector<std::shared_ptr<GaussianBeam>> beams

  public:


};

#endif // include protector
