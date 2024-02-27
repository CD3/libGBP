#pragma once

#include <string>

// #include "libgbp2-message-api_export.h"

namespace libGBP2
{

/**
 * A class to run propagation calculations based
 * on user configuration.
 */
class Propagator
{
 private:
  // using the PImple Pattern
  struct imp;
  imp* pImpl;

 public:
  /**
   * Run an propagation analysis based on a configuration
   * given by a serialized protobuf message.
   */
  std::string run(const std::string&);
};

}  // namespace libGBP2
