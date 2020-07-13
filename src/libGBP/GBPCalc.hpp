#ifndef GBPCalc_hpp
#define GBPCalc_hpp

/** @file GBPCalc.hpp
 * @brief
 * @author C.D. Clark III
 * @date 07/27/16
 */

#include <boost/property_tree/ptree.hpp>
#include <boost/signals2.hpp>

#include "Builders/BeamBuilder.hpp"
#include "Builders/MediaStackBuilder.hpp"
#include "Builders/OpticalSystemBuilder.hpp"

/** @class GBPCalc
 * @brief Calculator class to peroform Gaussian Beam Propagation calculations.
 * @author C.D. Clark III
 */
template<typename LengthUnitType>
class GBPCalc
{
 protected:
  std::shared_ptr<OpticalSystem<LengthUnitType> > optics;
  std::shared_ptr<MediaStack<LengthUnitType> >    media;
  std::shared_ptr<GaussianBeam>                   beam;
  std::vector<quantity<LengthUnitType> >          evaluation_points;

 public:
  void configure(const ptree& configTree);
  void clear();

  template<typename V>
  GaussianBeam getBeam(V z);

  void calculate();

  boost::signals2::signal<void(const GaussianBeam&)>
      sig_calculatedBeam;  ///< signal that is emitted when a beam is calculated
                           ///< at a new z position
};

/** Returns a Gaussian beam that corresponds to a given position. The current
 * position of the beam is set to z. WARNING: the beam's power is set to the
 * power at the given z location. If the location is inside of an absorbing
 * material, then the beam's power will NOT be correct for any other location.
 */
template<typename T>
template<typename V>
GaussianBeam GBPCalc<T>::getBeam(V z)
{
  GaussianBeam beam = *(this->beam);

  double transmission =
      this->media->getTransmission(beam.getCurrentPosition(), z);
  beam.setPower(beam.getPower() * transmission);
  optics->transform(&beam, beam.getCurrentPosition(), z);
  beam.setCurrentPosition(z);

  return beam;
}

template<typename T>
void GBPCalc<T>::configure(const ptree& configTree)
{
  this->clear();

  OpticalSystemBuilder<T> OSb;
  MediaStackBuilder<T>    Mb;
  BeamBuilder             Bb;

  this->beam.reset(Bb.build(configTree.get_child("beam")));
  if (configTree.get_child_optional("media_stack"))
    this->media.reset(Mb.build(configTree.get_child("media_stack")));
  else
    this->media.reset(Mb.create());
  if (configTree.get_child_optional("optical_system"))
    this->optics.reset(OSb.build(configTree.get_child("optical_system")));
  else
    this->optics.reset(OSb.create());

  auto evalPointsConfig = configTree.get_child_optional("evaluation_points.z");
  if (evalPointsConfig) {
    auto min = evalPointsConfig.value().get_optional<double>("min");
    auto max = evalPointsConfig.value().get_optional<double>("max");
    auto n   = evalPointsConfig.value().get_optional<double>("n");

    if (min && max && n) {
      double dz = (max.value() - min.value()) / (n.value() - 1);

      for (size_t i = 0; i < n.value(); i++)
        evaluation_points.push_back((min.value() + i * dz) * T());
    }

    for (auto iter :
         getSortedChildren(evalPointsConfig.value(), keyIntComp, isInt)) {
      evaluation_points.push_back(iter->second.get<double>("") * T());
    }
  }
}

template<typename T>
void GBPCalc<T>::clear()
{
  beam.reset();
  optics.reset();
  media.reset();
  evaluation_points.clear();
}

template<typename T>
void GBPCalc<T>::calculate()
{
  for (size_t i = 0; i < evaluation_points.size(); i++) {
    auto z = evaluation_points[i];
    sig_calculatedBeam(this->getBeam(z));
  }
}

#endif  // include protector
