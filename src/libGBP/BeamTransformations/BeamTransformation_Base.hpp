#pragma once

#include "./BeamTransformation_Interface.hpp"

/** @file BeamTransformation_Base.hpp
 * @brief
 * @author C.D. Clark III
 * @date 07/01/16
 */

/** @class BeamTransformation_Base
 * @brief Base class that implements the BeamTransformation_Interface and provides default implementations
 * for the required methods.
 * @author C.D. Clark III
 */
template<typename T>
class BeamTransformation_Base: public BeamTransformation_Interface<T>
{
 public:
  typedef Eigen::Matrix<double, 2, 2> MatrixType;
  virtual MatrixType  getRTMatrix() const { return MatrixType::Identity(); };
  virtual double      getPowerLoss() const { return 0; }
  virtual double      getWavelengthScaleFactor() const { return 1; }
  virtual boost::units::quantity<T> getPositionShift() const { return 0 * T(); }
};
