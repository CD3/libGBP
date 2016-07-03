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
#include "OpticalElement.hpp"

using std::complex;

class GaussianBeam
{

  protected:
    quantity<t::hertz> frequency; ///< frequency of the light
    quantity<t::nanometer> wavelength; ///< wavelength of light in the propagation medium
    quantity<t::centimeter> waistPosition; ///< position of the beam waist
    quantity<t::centimeter> waistDiameter; ///< diameter of the beam waist
    quantity<t::watt> power; ///< total power in the beam
    quantity<t::centimeter> currentPosition; ///< the current position in the beam.

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

    // currentPosition getters and setters
    template<typename T>
    void setCurrentPosition(T v) { this->currentPosition = quantity<t::centimeter>(v); } ///< performs unit conversion and sets currentPosition
    template<typename T>
    quantity<T> getCurrentPosition() const { return quantity<T>(this->currentPosition); } ///< returns currentPosition in specified units
    inline quantity<t::centimeter> getCurrentPosition() const { return this->getCurrentPosition<t::centimeter>(); } ///< returns currentPosition in internal units (t::centimeter)


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
    quantity<T>              getDiameter(U   ) const; ///< computes and returns diameter in specified units
    template<typename U>
    quantity<t::centimeter>  getDiameter(U z ) const { return this->getDiameter<t::centimeter>(z); } ///< returns diameter in default units (t::centimeter)
    template<typename T>
    quantity<T>              getDiameter(    ) const { return this->getDiameter<T>(this->currentPosition);} ///< computes and returns diameter, in specified units, at the current position
    quantity<t::centimeter>  getDiameter(    ) const { return this->getDiameter<t::centimeter>(this->currentPosition); } ///< returns diameter in default units (t::centimeter) at the current position

    template<typename T,typename U>
    quantity<T>              getRadiusOfCurvature(U   ) const; ///< computes and returns radius of curvature in specified units
    template<typename U>
    quantity<t::centimeter>  getRadiusOfCurvature(U z ) const { return this->getRadiusOfCurvature<t::centimeter>(z); } ///< returns radiuus of curvature in default units (t::centimeter)
    template<typename T>
    quantity<T>              getRadiusOfCurvature(    ) const { return this->getRadiusOfCurvature<T>(this->currentPosition);} ///< computes and returns radius of curvature, in specified units, at the current position
    quantity<t::centimeter>  getRadiusOfCurvature(    ) const { return this->getRadiusOfCurvature<t::centimeter>(this->currentPosition); } ///< returns radius of curvature in default units (t::centimeter) at the current position

    template<typename T, typename U>
    quantity<T,complex<double> >               getComplexBeamParameter(U   ) const; ///< computes and returns complex beam parameter in specified units
    template<typename U>
    quantity<t::centimeter, complex<double> >  getComplexBeamParameter(U z ) const { return this->getComplexBeamParameter<t::centimeter>(z); } ///< returns complex beam parameter in default units (t::centimeter, complex<double> )
    template<typename T>
    quantity<T,complex<double> >               getComplexBeamParameter(    ) const { return this->getComplexBeamParameter<T>(this->currentPosition);} ///< computes and returns complex beam parameter, in specified units, at the current position
    quantity<t::centimeter, complex<double> >  getComplexBeamParameter(    ) const { return this->getComplexBeamParameter<t::centimeter>(this->currentPosition); } ///< returns complex beam parameter in default units (t::centimeter) at the current position

    template<typename T,typename U>
    quantity<T>              getRelativeWaistPosition(U   ) const; ///< computes and returns the relative waist position in specified units
    template<typename U>
    quantity<t::centimeter>  getRelativeWaistPosition(U z ) const { return this->getRelativeWaistPosition<t::centimeter>(z); } ///< returns relative waist position in default units (t::centimeter)
    template<typename T>
    quantity<T>              getRelativeWaistPosition(    ) const { return this->getRelativeWaistPosition<T>(this->currentPosition);} ///< computes and returns relative waist position, in specified units, at the current position
    quantity<t::centimeter>  getRelativeWaistPosition(    ) const { return this->getRelativeWaistPosition<t::centimeter>(this->currentPosition); } ///< returns relative waist position in default units (t::centimeter) at the current position


    template<typename T, typename U>
    void transform( OpticalElementInterface<T>* elem, U z );
    template<typename T>
    void transform( OpticalElementInterface<T>* elem ) { this->transform( elem, this->currentPosition ); }
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
  quantity<T> dz = this->getRelativeWaistPosition<T>(z);

  auto val = this->getWaistDiameter<T>()*root<2>( 1 + pow<2>(dz/this->getRayleighRange<T>()) );

  return quantity<T>(val);
}

template<typename T,typename U>
quantity<T> GaussianBeam::getRadiusOfCurvature(U z) const
{
  quantity<T> dz = this->getRelativeWaistPosition<T>(z);

  auto val = dz*( 1 + pow<2>(this->getRayleighRange<T>()/dz) );

  return quantity<T>(val);
}

template<typename T,typename U>
quantity<T,complex<double> > GaussianBeam::getComplexBeamParameter(U z) const
{
  quantity<T> dz = this->getRelativeWaistPosition<T>(z);

  double real,imag;

  real = dz.value();
  imag = this->getRayleighRange<T>().value();

  auto val = complex<double>(real,imag)*T();

  return quantity<T,complex<double> >(val);
}

template<typename T,typename U>
quantity<T> GaussianBeam::getRelativeWaistPosition(U z) const
{
  quantity<T> val = quantity<T>(z) - this->getWaistPosition<T>();

  return val;
}

template<typename T, typename U>
void GaussianBeam::transform( OpticalElementInterface<T>* elem, U z )
{
  this->setWavelength( this->getWavelength()*elem->getWavelengthScaleFactor() );
  this->setPower(      this->getPower()     *(1.-elem->getPowerLoss()) );

  complex<double> qi = this->getComplexBeamParameter<T>(z).value();
  auto RTM = elem->getRTMatrix();
  double A = RTM(0,0);
  double B = RTM(0,1);
  double C = RTM(1,0);
  double D = RTM(1,1);

  complex<double> qf = (A*qi + B)/(C*qi + D);

  // q = x + i y = z - i z_R
  //
  // z : distance to the beam waist
  // z_R : the rayleigh range
  //
  // The real part of q is the distance to the beam waist, so its actually \Delta z = z - z_0, where
  // z is the position that q_i is evaluated at.
  //
  // So, z_0 = z - Re{q}
  //
  // The imaginary part of q is the rayliegh range, z_R = \pi \omega_0^2 / \lambda
  //
  // So, \omega_0 = \sqrt{ z_R \lambda / \pi }
  // and d = 2 \omega_0
  //
  // CAREFUL! Make sure to get the units right.

  this->setWaistPosition( quantity<T>(z) - qf.real()*T() );
  this->setWaistDiameter( 2*sqrt(qf.imag()*this->getWavelength<T>().value()/M_PI)*T() );

}

#endif // include protector
