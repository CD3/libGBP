#ifndef GaussianBeam_hpp
#define GaussianBeam_hpp

/** @file GaussianBeam.hpp
  * @brief Class representing a Guassian beam.
  * @author C.D. Clark III
  * @date 06/28/16
  */

#include <iostream>
#include <complex>
#include "Units.hpp"
#include "Constants.hpp"

using std::complex;

class GaussianBeam
{

  protected:
    quantity<t::hertz> frequency; ///< frequency of the light
    quantity<t::nanometer> wavelength; ///< wavelength of light in the propagation medium
    quantity<t::centimeter> waistPosition; ///< position of the beam waist
    quantity<t::centimeter> waistDiameter; ///< diameter of the beam waist
    quantity<t::watt> power; ///< total power in the beam

  public:

    // frequency getters and setters
    template<typename T>
    void setFrequency(T v) { this->frequency = quantity<t::hertz>(v); } ///< performs unit conversion and sets frequency
    template<typename T>
    quantity<T> getFrequency() const { return quantity<T>(this->frequency); } ///< returns frequency in specified units
    inline quantity<t::hertz> getFrequency() const { return this->getFrequency<t::hertz>(); } ///< returns frequency in default units (t::hertz)

    // wavelength getters and setters
    template<typename T>
    void setWavelength(T v) { this->wavelength = quantity<t::nanometer>(v); } ///< performs unit conversion and sets wavelength
    template<typename T>
    quantity<T> getWavelength() const { return quantity<T>(this->wavelength); } ///< returns wavelength in specified units
    inline quantity<t::nanometer> getWavelength() const { return this->getWavelength<t::nanometer>(); } ///< returns wavelength in default units (t::nanometer)

    // waistPosition getters and setters
    template<typename T>
    void setWaistPosition(T v) { this->waistPosition = quantity<t::centimeter>(v); } ///< performs unit conversion and sets waistPosition
    template<typename T>
    quantity<T> getWaistPosition() const { return quantity<T>(this->waistPosition); } ///< returns waistPosition in specified units
    inline quantity<t::centimeter> getWaistPosition() const { return this->getWaistPosition<t::centimeter>(); } ///< returns waistPosition in default units (t::centimeter)

    // waistDiameter getters and setters
    template<typename T>
    void setWaistDiameter(T v) { this->waistDiameter = quantity<t::centimeter>(v); } ///< performs unit conversion and sets waistDiameter
    template<typename T>
    quantity<T> getWaistDiameter() const { return quantity<T>(this->waistDiameter); } ///< returns waistDiameter in specified units
    inline quantity<t::centimeter> getWaistDiameter() const { return this->getWaistDiameter<t::centimeter>(); } ///< returns waistDiameter in default units (t::centimeter)

    // power getters and setters
    template<typename T>
    void setPower(T v) { this->power = quantity<t::watt>(v); } ///< performs unit conversion and sets power
    template<typename T>
    quantity<T> getPower() const { return quantity<T>(this->power); } ///< returns power in specified units
    inline quantity<t::watt> getPower() const { return this->getPower<t::watt>(); } ///< returns power in default units (t::watt)


    // calculated parameters

    template<typename T>
    quantity<T> getFreeSpaceWavelength() const; ///< computes and returns free space wavelength in specified units
    inline quantity<t::nanometer> getFreeSpaceWavelength() const { return this->getFreeSpaceWavelength<t::nanometer>(); } ///< returns free space wavelength in default units (t::nanometer)

    template<typename T>
    quantity<T> getRayleighRange() const; ///< computes and returns rayleigh range in specified units
    inline quantity<t::centimeter> getRayleighRange() const { return this->getRayleighRange<t::centimeter>(); } ///< returns rayleigh range in default units (t::centimeter)

    template<typename T>
    quantity<T> getDivergence() const; ///< computes and returns divergence in specified units
    inline quantity<t::milliradian> getDivergence() const { return this->getDivergence<t::milliradian>(); } ///< returns divergence in default units ()

    template<typename T,typename U>
    quantity<T> getDiameter(U) const; ///< computes and returns diameter in specified units
    template<typename U>
    quantity<t::centimeter> getDiameter(U z) const { return this->getDiameter<t::centimeter>(z); } ///< returns diameter in default units (t::centimeter)

    template<typename T,typename U>
    quantity<T> getRadiusOfCurvature(U) const; ///< computes and returns radius of curvature in specified units
    template<typename U>
    quantity<t::centimeter> getRadiusOfCurvature(U z) const { return this->getRadiusOfCurvature<t::centimeter>(z); } ///< returns radiuus of curvature in default units (t::centimeter)

    template<typename T, typename U>
    quantity<T,complex<double> > getComplexBeamParameter(U) const; ///< computes and returns complex beam parameter in specified units
    template<typename U>
    quantity<t::centimeter, complex<double> > getComplexBeamParameter(U z) const { return this->getComplexBeamParameter<t::centimeter>(z); } ///< returns complex beam parameter in default units (t::centimeter, complex<double> )


};

template<typename T>
quantity<T> GaussianBeam::getFreeSpaceWavelength() const
{
  auto val = constants::SpeedOfLight / this->getFrequency<t::hertz>();

  return quantity<T>(val);
}

template<typename T>
quantity<T> GaussianBeam::getRayleighRange() const
{
  auto val = M_PI*pow<2>(this->getWaistDiameter<T>()/2.)/this->getWavelength<T>();

  return quantity<T>(val);
}

template<typename T>
quantity<T> GaussianBeam::getDivergence() const
{

  auto val = (4/M_PI)*(this->getWavelength<t::nanometer>()/this->getWaistDiameter<t::nanometer>())*t::radian();

  return quantity<T>(val);
}

template<typename T,typename U>
quantity<T> GaussianBeam::getDiameter(U z) const
{
  quantity<t::meter> dz = z - this->getWaistPosition<t::meter>();

  auto val = this->getWaistDiameter<t::meter>()*root<2>( 1 + pow<2>(dz/this->getRayleighRange<t::meter>()) );

  return quantity<T>(val);
}

template<typename T,typename U>
quantity<T> GaussianBeam::getRadiusOfCurvature(U z) const
{
  quantity<t::meter> dz = z - this->getWaistPosition<t::meter>();

  auto val = dz*( 1 + pow<2>(this->getRayleighRange<t::meter>()/dz) );

  return quantity<T>(val);
}

template<typename T,typename U>
quantity<T,complex<double> > GaussianBeam::getComplexBeamParameter(U z) const
{
  quantity<t::meter> dz = z - this->getWaistPosition<t::meter>();

  double real,imag;

  real = dz.value();
  imag = this->getRayleighRange<t::meter>().value();

  auto val = complex<double>(real,imag)*t::meter();

  return quantity<T,complex<double> >(val);
}

#endif // include protector
