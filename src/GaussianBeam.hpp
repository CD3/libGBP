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
#include "OpticalElements/OpticalElementInterface.hpp"

using std::complex;

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
    quantity<t::radian> WaistPhase; ///< phase of the electric field at beam waist position.
    quantity<t::watt> Power; ///< total power in the beam
    quantity<t::centimeter> CurrentPosition; ///< the current position in the beam.
    quantity<t::dimensionless> BeamQualityFactor; ///< the M^2 factor for the beam.

    SpotSize SpotSizeMode = SpotSize::E2;

  public:

    // MACROS

    /**
     * this macro creates one setter and two getters for a member quantity.
     * units are handled automatically by the compiler magic (requires C++11 compiler).
     */
#define DEFAULT_ATTRIBUTE_SETTER_AND_GETTER(name)\
    typedef decltype(name) name##Type;\
    typedef name##Type::unit_type name##Unit;\
    template<typename T>\
    void set##name(T v) { this->name = name##Type(v); }\
    template<typename T>\
    quantity<T> get##name() const       { return quantity<T>(this->name); }\
    inline name##Type get##name() const { return this->get##name<name##Unit>(); }

    /**
     * setters and getters for SpotSizeMode (SSM) dependent attributes
     * the base setter and getter are only declared, not defined.
     */
#define SSM_DEPENDENT_ATTRIBUTE_SETTER_AND_GETTER(name, U)\
    typedef U name##Unit;\
    typedef quantity<U> name##Type;\
    template<typename T>\
    void set##name(T v, SpotSize mode);\
    template<typename T>\
    void set##name(T v){ this->set##name(v,this->SpotSizeMode);}\
    template<typename T>\
    quantity<T> get##name(SpotSize mode) const;\
    template<typename T>\
    quantity<T> get##name() const {return this->get##name<T>(this->SpotSizeMode);}\
    inline name##Type get##name(SpotSize mode) const { return this->get##name<name##Unit>(mode); }\
    inline name##Type get##name() const { return this->get##name<name##Unit>(); }

    /** defines base getter for a derived parameter (parameter that is calculated from member variables)
     */
#define DEFAULT_DERIVED_GETTER( name, U )\
    typedef U name##Unit;\
    typedef quantity<U> name##Type;\
    template<typename T>\
    quantity<T> get##name() const;\
    quantity<U> get##name() const {return this->get##name<U>(); }

#define SSM_DEPENDENT_DERIVED_GETTER( name, U )\
    typedef U name##Unit;\
    typedef quantity<U> name##Type;\
    template<typename T>\
    quantity<T> get##name(SpotSize mode) const;\
    template<typename T>\
    quantity<T> get##name() const { return this->get##name<T>(this->SpotSizeMode); }\
    quantity<U> get##name(SpotSize mode) const {return this->get##name<U>(); }\
    quantity<U> get##name() const {return this->get##name<U>(); }

    /**
     * getter for derived parameters that depend on z
     */
#define Z_DEPENDENT_DERIVED_GETTER( name, U )\
    typedef U name##Unit;\
    typedef quantity<U> name##Type;\
    template<typename T, typename V>\
    quantity<T> get##name(V z) const;\
    template<typename T>\
    quantity<T> get##name(   ) const {return this->get##name<T>( this->getCurrentPosition() );}\
    template<typename V>\
    quantity<U> get##name(V z) const {return this->get##name<U>(z); }\
    quantity<U> get##name(   ) const {return this->get##name<U>( ); }






    // DECLARATIONS

    DEFAULT_ATTRIBUTE_SETTER_AND_GETTER(Frequency);
    DEFAULT_ATTRIBUTE_SETTER_AND_GETTER(Wavelength);
    DEFAULT_ATTRIBUTE_SETTER_AND_GETTER(WaistPosition);
    DEFAULT_ATTRIBUTE_SETTER_AND_GETTER(WaistPhase);
    DEFAULT_ATTRIBUTE_SETTER_AND_GETTER(Power);
    DEFAULT_ATTRIBUTE_SETTER_AND_GETTER(CurrentPosition);

    GaussianBeam::SpotSize setSpotSizeMode(SpotSize m) {this->SpotSizeMode = m;}
    GaussianBeam::SpotSize getSpotSizeMode() const {return this->SpotSizeMode;}

    SSM_DEPENDENT_ATTRIBUTE_SETTER_AND_GETTER( WaistRadius,   decltype(WaistRadius)::unit_type );
    SSM_DEPENDENT_ATTRIBUTE_SETTER_AND_GETTER( WaistDiameter, WaistRadiusUnit );

    DEFAULT_DERIVED_GETTER( FreeSpaceWavelength, WavelengthUnit );
    DEFAULT_DERIVED_GETTER( RayleighRange, WaistRadiusUnit );

    SSM_DEPENDENT_DERIVED_GETTER( Divergence, t::milliradian );

    Z_DEPENDENT_DERIVED_GETTER( Radius, WaistRadiusUnit );
    Z_DEPENDENT_DERIVED_GETTER( Diameter, WaistRadiusUnit );
    Z_DEPENDENT_DERIVED_GETTER( RadiusOfCurvature, WaistPositionUnit );
    Z_DEPENDENT_DERIVED_GETTER( RelativeWaistPosition, WaistPositionUnit );
    Z_DEPENDENT_DERIVED_GETTER( Area, t::centimeter_squared );
    Z_DEPENDENT_DERIVED_GETTER( PeakIrradiance, t::watt_per_centimeter_squared );
    Z_DEPENDENT_DERIVED_GETTER( GouyPhase, t::radian );



    // SPECIAL CASES
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

#define U t::volt_per_meter
    typedef U ElectricFieldUnit;
    typedef quantity<U,complex<double> > ElectricFieldType;
    template<typename T, typename V, typename W>
    quantity<T,complex<double> >   getElectricField(V z, W r) const;
    template<typename T, typename W>
    quantity<T,complex<double> >   getElectricField(W r) const { return this->getElectricField<T>(this->getCurrentPosition(),r);}
    template<typename V, typename W>
    quantity<U,complex<double> >   getElectricField(V z, W r) const {return this->getElectricField<U>(z,r); }
    template<typename W>
    quantity<U,complex<double> >   getElectricField(W r) const {return this->getElectricField<U>(r); }
#undef U




    // OTHER METHODS

    template<typename T, typename U>
    void transform( OpticalElementInterface<T>* elem, U z );
    template<typename T>
    void transform( OpticalElementInterface<T>* elem ) { this->transform( elem, this->getCurrentPosition() ); }
};

// the waist radius functions need to be handled the spot size mode
template<typename T>
void GaussianBeam::setWaistRadius(T v, SpotSize mode)
{
  double scale = mode == SpotSize::E ? 2 : 1;
  this->WaistRadius = scale*WaistRadiusType(v);
}

template<typename T>
quantity<T> GaussianBeam::getWaistRadius(SpotSize mode) const
{
  double scale = mode == SpotSize::E ? 1/sqrt(2.) : 1;
  return scale*quantity<T>(this->WaistRadius);
}

// the theory of Gaussian beams is almost always formulated in terms of the beam radius,
// but we almost always will want to work with the beam diameter. this setter allows
// the user to set the beam diameter, even through we are storing the radius.
template<typename T>
void GaussianBeam::setWaistDiameter(T v, SpotSize mode)
{
  this->setWaistRadius( WaistRadiusType(v)/2., mode );
}

template<typename T>
quantity<T> GaussianBeam::getWaistDiameter(SpotSize mode) const
{
  return 2.*this->getWaistRadius<T>(mode);
}






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
quantity<T> GaussianBeam::getDivergence(SpotSize mode) const
{
  double scale = mode == SpotSize::E ? 1/sqrt(2.) : 1;
  //                  vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv this calculation gives the half-angle divergence
  auto val = scale*2.*this->getWavelength<t::nanometer>()/( M_PI*this->getWaistRadius<t::nanometer>() )*t::radian();

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

template<typename T,typename U>
quantity<T> GaussianBeam::getGouyPhase(U z) const
{
  auto val = atan( quantity<t::centimeter>(z) / this->getRayleighRange<t::centimeter>() );

  return quantity<T>(val);
}


template<typename T, typename V, typename W>
quantity<T,complex<double> >   GaussianBeam::getElectricField(V z, W r) const
{
  // @todo implement
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
  //                    vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv  this calculation will give us the 1/e^2 beam radius
  this->setWaistRadius( sqrt(qf.imag()*this->getWavelength<T>().value()/M_PI)*T(), SpotSize::E2 );

}

#endif // include protector
