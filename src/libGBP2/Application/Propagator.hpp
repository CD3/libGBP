#pragma once

#include "../CircularGaussianLaserBeam.hpp"
#include "../OpticalSystem.hpp"
#include<string>

namespace libGBP2
{

/**
 * A class to run propagation calculations based
 * on user configuration.
 */
class Propagator
{
 private:
  OpticalSystem<t::cm>      m_optical_system;
  CircularGaussianLaserBeam m_beam;
 public:

  /**
   * Run an propagation analysis based on a configuration
   * given by a serialized protobuf message.
   */
  std::string run(std::string);
};

}  // namespace libGBP2
