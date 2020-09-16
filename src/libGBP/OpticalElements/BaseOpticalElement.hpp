#ifndef OpticalElements_BaseOpticalElement_hpp
#define OpticalElements_BaseOpticalElement_hpp

#include "OpticalElementInterface.hpp"

/** @file BaseOpticalElement.hpp
 * @brief
 * @author C.D. Clark III
 * @date 07/01/16
 */

/** @class BaseOpticalElement
 * @brief Base class for optical elements that provides default implementations
 * for the required methods.
 * @author C.D. Clark III
 */
template<typename T>
class BaseOpticalElement : public OpticalElementInterface<T>
{
 public:
  typedef Eigen::Matrix<double, 2, 2> MatrixType;
  virtual MatrixType  getRTMatrix() const { return MatrixType::Identity(); };
  virtual double      getPowerLoss() const { return 0; }
  virtual double      getWavelengthScaleFactor() const { return 1; }
  virtual boost::units::quantity<T> getPositionShift() const { return 0 * T(); }
};

#endif  // include protector
