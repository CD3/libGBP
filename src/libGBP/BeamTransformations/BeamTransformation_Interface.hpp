#pragma once

/** @file BeamTransformation_Interface.hpp
 * @brief Definition of the beam transformation interface
 * @author C.D. Clark III
 * @date 06/29/16
 */

#include <memory>

#include <Eigen/Dense>

#include "../Units.hpp"
namespace libGBP
{
/** @class BeamTransformation_Interface
 * @brief Abstract class that defines the interface a beam transform must
 * implement.
 * @author C.D. Clark III
 *
 * An beam transform tranforms a Gaussian beam. Before a beam transform is applied
 * the beam has a beam waist size and position. After the transform is applied,
 * it has a new waist size and position. The transformation can be described
 * with a Ray Transfer Matrix (RTM). Ultimately, implementations of this
 * class will construct a RTM.
 *
 * The theory of Gaussian beam propagation
 * based on the paraxial wave equation accounts for the action of optical
 * elements using an RTM (https://en.wikipedia.org/wiki/Ray_transfer_matrix_analysis).
 * These are the  same matricies that are used to transform optical rays when ray tracing
 * through an optical system and they are convienent because the action of
 * multiple elements can be accounted for my simply multiplying their RTM
 * together.
 *
 * _Most_ classes that implement this interface will represent optical elements (lenses,
 * refractive surfaces, etc) but a few will not. In the discussion below, we will
 * refer to the transformation as an element.
 *
 * This method is often referred to as the "ABCD" law for Gaussian beams.
 * Basically, the RTM is a \f$2\times2\f$ matrix that can be written as \f[
 * \left(
 * \begin{matrix}
 * A & B \\
 * C & D
 * \end{matrix}
 * \right)
 * \f]
 * The action of the element is then given by its affect on the complex beam
 * parameter (https://en.wikipedia.org/wiki/Complex_beam_parameter) \f[ q_f =
 * \frac{Aq_i + B}{Cq_i + D} \f]
 *
 * Where $q_i$ and $q_f$ are the complex beam parameters of the beam before and
 * after it passes through the lens. The complex beam parameter itself is
 * defined as \f$q = z + iz_R\f$ where \f$z\f$ is the distance from the beam
 * waist and \f$z_R\f$ is the Rayleigh range (\f$z_R = \frac{\pi
 * \omega_0^2}{\lambda}\f$, where \f$\omega_0\f$ is the beam waist radius
 * (\f$1/e^2\f$) and \f$\lambda\f$ is the wavelength.).
 *
 * One important point to make here is that the usual treatment of this method
 * assumes that the beam waist is always located at \f$z = 0\f$. In other words,
 * the origin of the coordinate system is always taken to be at the beam waist
 * which means that when the beam waist position changes (after it goes through
 * an optical element), the coordinate system changes.
 *
 * This library performs calculations in a fixed coordinate system. The beam
 * waist position is explicitly tracked, and when the beam passes through an
 * optical element, the beam waist position is updated. This introduces a few
 * subtle details that must be handled correctly. Some of these only affect the
 * GaussianBeam class. However, a few also affect optical elements.
 *
 * The optical element class is intended to encapsulate all of the information
 * necessary for a GaussianBeam instance to transform itself. In order to do
 * that, the beam will need the RTM representing the element. No surprise there.
 * However, it is possible for an element to separate two different media (more
 * specifically, two materials with different refractive indices. When this is
 * the case, the wavelength of the beam will change (since \f$\lambda =
 * \frac{\lambda_0}{n}\f$) and this change must be known in order for the beam
 * to update the beam waist size since the imaginary portion of the complex beam
 * parameter contains both the wavelength and beam waist radius. So, an optical
 * element must provide a method that the GaussianBeam instance can use to
 * determine the new wavelength.
 *
 * The second detail that arises from the uses of a fixed coordinate system is
 * the possibility that the z position of the complex beam parameter may shift
 * when it passes through the element. In other words, the z position of the
 * initial parameter may not coincide with the position of the final parameter.
 * When the coordinate system is always centered on the beam waist, this does
 * not matter. The real part of the complex beam parameter gives the distance to
 * the beam waist. However, for a fixed coordinate system, this possible shift
 * needs to be accounted for. The best example of this is a thick lens. The RTM
 * for a thick lens includes the refraction across the front surface,
 * propagation through the lens, and refraction across the back surface. When
 * this matrix is used to transform the complex beam parameter, the initial
 * parameter is evaluated at the front surface of the lens and the new parameter
 * actually corresponds to the position of the back surface of the lens.
 *
 * This shift needs to be known in order for the beam waist position to be
 * determined, so an optical element must provide a method to the
 * GaussianBeam instance can use to determine the new beam waist position.
 *
 * @tparam LengthUnitType the length unit that will be used for calculating
 * elements of the RTM and position shifts.
 */

template<typename LengthUnitType>
class BeamTransformation_Interface
{
 public:
  virtual Eigen::Matrix<double, 2, 2> getRTMatrix()
      const = 0;  ///< return the Ray Transfer matrix for the element. NOTE:
                  ///< elements MUST be returned in units of LengthUnitType or
                  ///< inverse LengthUnitType.
  virtual boost::units::quantity<LengthUnitType> getPositionShift()
      const = 0;  ///< return the difference in the position (z coordinate) that
                  ///< the complex beam parameter corresponds to after it passes
                  ///< through the element.
  virtual double getPowerLoss()
      const = 0;  ///< return the power loss (fraction) through the element
  virtual double getWavelengthScaleFactor()
      const = 0;  ///< return the scaling factor through the element. i.e. if
                  ///< the wavelenght changes when light passes through the
                  ///< element.
};

template<typename T, typename U>
class BeamTransformAdapter : public BeamTransformation_Interface<U>
{
 private:
  T &t;

 public:
  BeamTransformAdapter(T &ref) : t(ref) {}
  Eigen::Matrix<double, 2, 2> getRTMatrix() const { return t.getRTMatrix(); }
  boost::units::quantity<U>   getPositionShift() const { return t.getPositionShift(); }
  double                      getPowerLoss() const { return t.getPowerLoss(); }
  double                      getWavelengthScaleFactor() const
  {
    return t.getWavelengthScaleFactor();
  }
};

template<typename T>
using BeamTransformation_ptr = std::shared_ptr<BeamTransformation_Interface<T> >;

}  // namespace libGBP
