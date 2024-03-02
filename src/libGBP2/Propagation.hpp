#pragma once

#include "./CircularGaussianLaserBeam.hpp"
#include "./OpticalSystem.hpp"
#include "./Units.hpp"
namespace libGBP2
{

template<typename U1, typename U2>
CircularGaussianLaserBeam propagate_beam_through_system(const CircularGaussianLaserBeam& a_beam, const OpticalSystem<U1>& a_system, const quantity<U2>& a_position, bool a_fixed_coordinate_system = false)
{
  return transform_beam(a_beam, a_system.template build<t::cm>(0 * i::cm, a_position), a_fixed_coordinate_system);
}

/**
 * Transform a Gaussian beam through an optical element.
 *
 * @param a_beam : the input beam
 * @param a_element : the optical element
 * @param a_fix_coordinate_system : if true, return in the same coordinate system as the input beam.
 * @return the output beam
 *
 * The a_fixed_coordinate_system parameter is only used when an optical element includes a spatial dispacement.
 * For example:
 *
 * - For a thick lens, the input beam waist position is with respect
 *   to the front surface of the lens, but the output beam waist is with respect to the back surface.
 * - For a telescope, the input beam waist is with respect to the objective lens and the output beam waist is with
 *   respect to the eyepiece.
 *
 * If a_fixed_coordinate_system is True, the output beam waist will be with respec to the front surface/objective lens
 * in these two cases.
 *
 */
template<typename U1>
CircularGaussianLaserBeam transform_beam(CircularGaussianLaserBeam a_beam, const OpticalElement<U1>& a_element, bool a_fixed_coordinate_system = false)
{
  // get embedded beam
  // propagate embedded beam through optical element
  // convert back to real beam
  CircularGaussianLaserBeam ebeam = a_beam.getEmbeddedBeam();
  auto                      q     = ebeam.getComplexBeamParameter();
  q                               = a_element * q;
  // need to set refractive index FIRST

  ebeam.setRefractiveIndex(ebeam.getRefractiveIndex() * a_element.getRefractiveIndexScale());
  ebeam.setComplexBeamParameter(q);
  // IF we want to reset coordinate system, we need to do this AFTER
  if(a_fixed_coordinate_system) {
    ebeam.setBeamWaistPosition(ebeam.getBeamWaistPosition() + a_element.getDisplacement());
  }
  a_beam.setEmbeddedBeam(ebeam);

  return a_beam;
}

}  // namespace libGBP2
