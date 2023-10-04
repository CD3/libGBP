#pragma once

/** @file BeamBuilder.hpp
 * @brief A class that can configure a GaussianBeam based on various scenarios.
 * @author C.D. Clark III
 * @date 07/04/16
 *
 * The beam configurator class manages configuring a GaussianBeam instance for a
 * variety of situations so that the GaussianBeam class can remain simple. The
 * GaussianBeam class only allows the beam configuration to be specified by
 * calling setters. It does not support, for example reading configuration from a file.
 *
 * Basically, four parameters must be known to characterize a Gaussian beam,
 * and several sets of 4 parameters are commonly used. For example, if the
 * wavelength and divergence are known, then the beam waist diameter can be
 * calculated. If the diameter at a given position is also known, then the beam
 * waist position can also be determined.
 *
 * Rather than allow for all of the configuration posibilities in the
 * GaussianBeam class, they are handled here.
 */
#include <vector>

#include <boost/optional.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "../GaussianBeam.hpp"
#include "./Builder.hpp"

using std::vector;
namespace libGBP
{
struct BeamBuilder : public Builder<GaussianBeam> {
#define ADD_ATTRIBUTE(name, unit, n)                                         \
  vector<boost::units::quantity<unit> > name;                                \
  template<typename T>                                                       \
  BeamBuilder& set##name(T v, int i = -1)                                    \
  {                                                                          \
    if(i < 0 && this->name.size() < n)                                       \
      this->name.push_back(boost::units::quantity<unit>(v));                 \
    if(i < 0 && this->name.size() >= n)                                      \
      this->name[n - 1] = boost::units::quantity<unit>(v);                   \
    if(i >= 0 && i + 1 < n) this->name[i] = boost::units::quantity<unit>(v); \
    return *this;                                                            \
  }                                                                          \
  template<typename T>                                                       \
  boost::optional<boost::units::quantity<T> > get##name(int i = 0)           \
  {                                                                          \
    if(i < name.size()) {                                                    \
      return boost::units::quantity<T>(name[i]);                             \
    }                                                                        \
    return boost::none;                                                      \
  }                                                                          \
  bool has##name(int i = 0)                                                  \
  {                                                                          \
    if(i < name.size()) {                                                    \
      return true;                                                           \
    } else {                                                                 \
      return false;                                                          \
    }                                                                        \
  }

  ADD_ATTRIBUTE(Wavelength, units::t::nanometer, 1);
  ADD_ATTRIBUTE(FreeSpaceWavelength, units::t::nanometer, 1);
  ADD_ATTRIBUTE(Frequency, units::t::nanometer, 1);
  ADD_ATTRIBUTE(OneOverE2FullAngleDivergence, units::t::milliradian, 1);
  ADD_ATTRIBUTE(OneOverE2Diameter, units::t::centimeter, 2);
  ADD_ATTRIBUTE(Position, units::t::centimeter, 2);
  ADD_ATTRIBUTE(WaistPosition, units::t::centimeter, 1);
  ADD_ATTRIBUTE(OneOverE2WaistDiameter, units::t::centimeter, 1);
  ADD_ATTRIBUTE(Power, units::t::watt, 1);
  ADD_ATTRIBUTE(CurrentPosition, units::t::centimeter, 1);

  void configure(GaussianBeam* beam);
  void configure(GaussianBeam& beam) { this->configure(&beam); }

  void configure(GaussianBeam* beam,
                 const ptree&  configTree);  ///< required by the base class
  void configure(GaussianBeam& beam, const ptree& configTree)
  {
    this->configure(&beam, configTree);
  }
};

void BeamBuilder::configure(GaussianBeam* beam)
{
  if(this->hasWavelength())
    beam->setWavelength(this->getWavelength<units::t::nanometer>().value());

  if(this->hasPower()) beam->setPower(this->getPower<units::t::watt>().value());

  if(this->hasOneOverE2WaistDiameter())
    beam->setOneOverE2WaistDiameter(
        this->getOneOverE2WaistDiameter<units::t::centimeter>().value());

  if(this->hasWaistPosition())
    beam->setWaistPosition(this->getWaistPosition<units::t::centimeter>().value());

  if(!this->hasOneOverE2WaistDiameter()) {
    if(this->hasWavelength() &&
       this->hasOneOverE2FullAngleDivergence()) {  // if the divergence is known, then we can
                                                   // determine the beam waist diameter

      // \Theta   = \frac{2\lambda}{\pi \omega_0}
      // \omega_0 = \frac{2\lambda}{\pi \Theta  }
      boost::units::quantity<units::t::centimeter> waistRadius =
          2. * beam->getWavelength<units::t::centimeter>() /
          boost::units::quantity_cast<double>(M_PI *
                                              this->getOneOverE2FullAngleDivergence<units::t::radian>().value());
      beam->setOneOverE2WaistRadius(waistRadius);

      if(this->hasOneOverE2Diameter()) {  // if the diameter at some location is known,
                                          // then we can determine where the beam waist
                                          // is
        if(this->getOneOverE2Diameter<units::t::centimeter>() < 2. * waistRadius)
          throw std::runtime_error(
              "BEAM CONFIGURATION ERROR: configured beam diameter is smaller "
              "than the beam waist diameter based on wavelength and "
              "divergence.");

        boost::units::quantity<units::t::centimeter> pos = 0 * units::i::cm;
        if(this->hasPosition())
          pos = this->getPosition<units::t::centimeter>().value();

        boost::units::quantity<units::t::centimeter> waistPosition =
            pos -
            beam->getRayleighRange<units::t::centimeter>() *
                sqrt(boost::units::pow<2>(this->getOneOverE2Diameter<units::t::centimeter>().value() /
                                          beam->getOneOverE2WaistDiameter<units::t::centimeter>()) -
                     1.);
        beam->setWaistPosition(waistPosition);
      }
    }
  }

  if(!this->hasFrequency())
    beam->setFrequency(
        constants::SpeedOfLight /
        (this->hasFreeSpaceWavelength()
             ? this->getFreeSpaceWavelength<units::t::nanometer>().value()
             : this->getWavelength<units::t::nanometer>().value()));

  if(this->hasCurrentPosition())
    beam->setCurrentPosition(this->getCurrentPosition<units::t::centimeter>().value());
}

void BeamBuilder::configure(GaussianBeam* beam, const ptree& configTree)
{
  BeamBuilder  builder;
  const ptree* tree;
  tree = &configTree;
#define SET(name, key, val)                       \
  {                                               \
    auto param = tree->get_optional<double>(key); \
    if(param) {                                   \
      auto value = param.value();                 \
      builder.set##name(val);                     \
    }                                             \
  }

  SET(Wavelength, "wavelength", value * units::i::nm);
  SET(Power, "power", value * units::i::W);
  SET(OneOverE2FullAngleDivergence, "divergence", value * units::i::mrad);
  SET(WaistPosition, "waist.position", value * units::i::cm);
  SET(OneOverE2WaistDiameter, "waist.diameter", value * units::i::cm);
  SET(CurrentPosition, "current_position", value * units::i::cm);

  auto profiles = configTree.get_child_optional("profiles");
  if(profiles) {
    for(auto& iter : profiles.value()) {
      tree = &iter.second;
      SET(Position, "position", value * units::i::cm);
      SET(OneOverE2Diameter, "diameter", value * units::i::cm);
      SET(OneOverE2Diameter, "radius", 2. * value * units::i::cm);
    }
  }

  builder.configure(beam);

#undef SET
}

}  // namespace libGBP
