#ifndef GaussianBeam_hpp
#define GaussianBeam_hpp

/** @file GaussianBeam.hpp
  * @brief Class representing a Guassian beam.
  * @author C.D. Clark III
  * @date 06/28/16
  */

#include <iostream>
#include <complex>

#include <boost/property_tree/ptree.hpp>

#include "Units.hpp"
#include "Constants.hpp"
#include "OpticalElements/OpticalElementInterface.hpp"

using std::complex;
using boost::property_tree::ptree;

/** @class 
  * @brief A class implementing Gaussian Beam calculations.
  * @author C.D. Clark III
  *
  * NOTE: This class uses the same convension as "Lasers" by Minlonni and Eberly in which the beam radius/diameter is defined
  * by the 1/e^2 points. This differs from the ANSI standard.
  */
class GaussianBeam
{
  public:
    enum class SpotSize { E=1, E2=2 };

  protected:
    // Note on case: we are capitalizing member names here so that
    //               we can use macros to generate the accessor functions.

    quantity<t::hertz> Frequency; ///< frequency of the light
    quantity<t::nanometer> Wavelength; ///< wavelength of light in the propagation medium
    quantity<t::centimeter> WaistPosition; ///< position of the beam waist
    quantity<t::centimeter> WaistRadius; ///< radius (1/e^2) of the beam waist
    quantity<t::watt> Power; ///< total power in the beam
    quantity<t::centimeter> CurrentPosition; ///< the current position in the beam.

    SpotSize SpotSizeMode = SpotSize::E2;

  public:
    
    // this macro creates one setter and two getters for a member quantity.
    // units are handled automatically by the compiler magic (requires C++11 compiler).
#define SETTER_AND_GETTER(name)\
    typedef decltype(name) name##Type;\
    typedef name##Type::unit_type name##Unit;\
    template<typename T>\
    void set##name(T v) { this->name = name##Type(v); }\
    template<typename T>\
    quantity<T> get##name() const       { return quantity<T>(this->name); }\
    inline name##Type get##name() const { return this->get##name<name##Unit>(); }

    SETTER_AND_GETTER(Frequency);
    SETTER_AND_GETTER(Wavelength);
    SETTER_AND_GETTER(WaistPosition);
    SETTER_AND_GETTER(Power);
    SETTER_AND_GETTER(CurrentPosition);

#undef SETTER_AND_GETTER

    // the waist radius functions need to be handled the spot size mode
    typedef decltype(WaistRadius) WaistRadiusType;
    typedef WaistRadiusType::unit_type WaistRadiusUnit;
    template<typename T>
    void setWaistRadius(T v) { double scale = this->SpotSizeMode == SpotSize::E ? 2 : 1; this->WaistRadius = scale*WaistRadiusType(v);}
    template<typename T>
    quantity<T> getWaistRadius() const       { double scale = this->SpotSizeMode == SpotSize::E ? 1/sqrt(2.) : 1; return scale*quantity<T>(this->WaistRadius);}
    inline WaistRadiusType getWaistRadius() const { return this->getWaistRadius<WaistRadiusUnit>();}

    GaussianBeam::SpotSize setSpotSizeMode(SpotSize m) {this->SpotSizeMode = m;}
    GaussianBeam::SpotSize getSpotSizeMode() const {return this->SpotSizeMode;}

#undef SETTER_AND_GETTER

    // calculated parameters
#define CALCULATED_GETTER( name, U )\
    typedef U name##Unit;\
    typedef quantity<U> name##Type;\
    template<typename T>\
    quantity<T> get##name() const;\
    quantity<U> get##name() const {return this->get##name<U>(); }

    CALCULATED_GETTER( FreeSpaceWavelength, WavelengthUnit );
    CALCULATED_GETTER( RayleighRange, WaistRadiusUnit );
    CALCULATED_GETTER( Divergence, t::milliradian );
    CALCULATED_GETTER( WaistDiameter, WaistRadiusUnit );

#undef CALCULATED_GETTER

#define CALCULATED_GETTER( name, U )\
    typedef U name##Unit;\
    typedef quantity<U> name##Type;\
    template<typename T, typename V>\
    quantity<T> get##name(V z) const;\
    template<typename T>\
    quantity<T> get##name(   ) const {return this->get##name<T>( this->getCurrentPosition() );}\
    template<typename V>\
    quantity<U> get##name(V z) const {return this->get##name<U>(z); }\
    quantity<U> get##name(   ) const {return this->get##name<U>( ); }

    CALCULATED_GETTER( Radius, WaistRadiusUnit );
    CALCULATED_GETTER( Diameter, WaistRadiusUnit );
    CALCULATED_GETTER( RadiusOfCurvature, WaistPositionUnit );
    CALCULATED_GETTER( RelativeWaistPosition, WaistPositionUnit );
    CALCULATED_GETTER( Area, t::centimeter_squared );
    CALCULATED_GETTER( PeakIrradiance, t::watt_per_centimeter_squared );


#undef GETTER

#define U t::centimeter
    typedef U ComplexBeamParameterUnit;
    typedef quantity<U,complex<double> > ComplexBeamParameterType;
    template<typename T, typename V>
    quantity<T,complex<double> >   getComplexBeamParameter(V z) const;
    template<typename T>
    quantity<T,complex<double> >   getComplexBeamParameter(   ) const { return this->getComplexBeamParameter<T>(this->getCurrentPosition());}
    template<typename V>
    quantity<U,complex<double> >   getComplexBeamParameter(V z) const {return this->getComplexBeamParameter<U>(z); }
    quantity<U,complex<double> >   getComplexBeamParameter(   ) const {return this->getComplexBeamParameter<U>( ); }
#undef U


    // SPECIAL SETTERS
    // the theory of Gaussian beams is almost always formulated in terms of the beam radius,
    // but we almost always will want to work with the beam diameter. this setter allows
    // the user to set the beam diameter, even through we are storing the radius.
    template<typename T>
    void setWaistDiameter(T v) { this->setWaistRadius( WaistRadiusType(v)/2. ); }


    template<typename T, typename U>
    void transform( OpticalElementInterface<T>* elem, U z );
    template<typename T>
    void transform( OpticalElementInterface<T>* elem ) { this->transform( elem, this->getCurrentPosition() ); }
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
  auto val = M_PI*pow<2>(this->getWaistRadius<T>())/this->getWavelength<T>();

  return quantity<T>(val);
}

template<typename T>
quantity<T> GaussianBeam::getDivergence() const
{
  auto val = 2.*this->getWavelength<t::nanometer>()/( M_PI*this->getWaistRadius<t::nanometer>() )*t::radian();

  return quantity<T>(val);
}

template<typename T>
quantity<T> GaussianBeam::getWaistDiameter() const
{
  auto val = 2.*this->getWaistRadius<T>();

  return quantity<T>(val);
}

template<typename T,typename U>
quantity<T> GaussianBeam::getDiameter(U z) const
{
  auto val = 2.*this->getRadius<T>(z);

  return quantity<T>(val);
}

template<typename T,typename U>
quantity<T> GaussianBeam::getRadius(U z) const
{
  quantity<T> dz = this->getRelativeWaistPosition<T>(z);

  auto val = this->getWaistRadius<T>()*root<2>( 1 + pow<2>(dz/this->getRayleighRange<T>()) );

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

template<typename T,typename U>
quantity<T> GaussianBeam::getArea(U z) const
{
  auto val = M_PI*pow<2>(this->getRadius(z));

  return quantity<T>(val);
}

template<typename T,typename U>
quantity<T> GaussianBeam::getPeakIrradiance(U z) const
{
  // if SpotSizeMode == E, then peak irradiance is P/A
  // if SpotSizeMode == E2, then peak irradiance is 2*P/A
  // in other words, E = SpotSizeMode * P / A
  auto val = static_cast<double>(SpotSizeMode)*this->getPower()/this->getArea(z);

  return quantity<T>(val);
}



template<typename T, typename U>
void GaussianBeam::transform( OpticalElementInterface<T>* elem, U z )
{
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
  //
  // CAREFUL! Make sure to get the units right.
  //

  this->setWavelength( this->getWavelength()*elem->getWavelengthScaleFactor() );
  this->setPower(      this->getPower()     *(1.-elem->getPowerLoss()) );

  this->setWaistPosition( quantity<T>(z) - qf.real()*T() );
  this->setWaistRadius( sqrt(qf.imag()*this->getWavelength<T>().value()/M_PI)*T() );

}

#endif // include protector
