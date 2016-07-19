%module libGBP
// NOTE: everything between %{ and %} gets copied verbatim into the *_wrap.cxx file
%{
#include "GaussianBeam.hpp"
#include "OpticalElements/ThinLens.hpp"

#define SET( name, unit ) \
void   set##name##DP( double v ){ $self->set##name( v*unit); } \

#define GET( name, unit ) \
double get##name##DP(          ){ return quantity_cast<double>( $self->get##name() ); }

%}


// NOTE: everything between %pythoncode %{ and %} will be copied verbatim into the *.py file
%pythoncode %{
import pint
ureg = pint.UnitRegistry()
Q_   = ureg.Quantity
%}


%include <std_string.i>
%include <std_complex.i>

%define pySET(class, name, unit)
%pythoncode %{
@ureg.wraps( None, (None,ureg('unit')), True )
def set##name (self,v):
  return self.set##name##DP(v)
class.set##name  = set##name 
%}
%enddef

%define pyGET(class, name, unit)
%pythoncode %{
@ureg.wraps( ureg('unit'), None, True )
def get##name (self):
  return self.get##name##DP()
class.get##name = get##name 
%}
%enddef






/*  ____                     _             ____                       */
/* / ___| __ _ _   _ ___ ___(_) __ _ _ __ | __ )  ___  __ _ _ __ ___  */
/*| |  _ / _` | | | / __/ __| |/ _` | '_ \|  _ \ / _ \/ _` | '_ ` _ \ */
/*| |_| | (_| | |_| \__ \__ \ | (_| | | | | |_) |  __/ (_| | | | | | |*/
/* \____|\__,_|\__,_|___/___/_|\__,_|_| |_|____/ \___|\__,_|_| |_| |_|*/
                                                                    

class GaussianBeam
{

public:

%extend {

SET(Frequency, hertz);
GET(Frequency, hertz);
SET(Wavelength, nanometer);
GET(Wavelength, nanometer);
SET(WaistPosition, centimeter);
GET(WaistPosition, centimeter);
SET(WaistDiameter, centimeter);
GET(WaistDiameter, centimeter);
SET(Power, watt);
GET(Power, watt);
SET(CurrentPosition, centimeter);
GET(CurrentPosition, centimeter);

GET(FreeSpaceWavelength, nanometer);
GET(RayleighRange, centimeter);

/*void transformDP(OpticalElementInterface<T>* elem, double z){ $self->transform(elem, z*centimeter); }*/


}

};

pySET(GaussianBeam, Frequency, hertz);
pyGET(GaussianBeam, Frequency, hertz);
pySET(GaussianBeam, Wavelength, nanometer);
pyGET(GaussianBeam, Wavelength, nanometer);
pySET(GaussianBeam, WaistPosition, centimeter);
pyGET(GaussianBeam, WaistPosition, centimeter);
pySET(GaussianBeam, WaistDiameter, centimeter);
pyGET(GaussianBeam, WaistDiameter, centimeter);
pySET(GaussianBeam, Power, watt);
pyGET(GaussianBeam, Power, watt);
pySET(GaussianBeam, CurrentPosition, centimeter);
pyGET(GaussianBeam, CurrentPosition, centimeter);

pyGET(GaussianBeam, FreeSpaceWavelength, nanometer);
pyGET(GaussianBeam, RayleighRange, centimeter);



/* _____ _     _       _                   */
/*|_   _| |__ (_)_ __ | |    ___ _ __  ___ */
/*  | | | '_ \| | '_ \| |   / _ \ '_ \/ __|*/
/*  | | | | | | | | | | |__|  __/ | | \__ \*/
/*  |_| |_| |_|_|_| |_|_____\___|_| |_|___/*/
                                         


template<typename T>
class ThinLens 
{

public:

%extend {

SET(FocalLength, T);
GET(FocalLength, T);

}

};

pySET(ThinLens, FocalLength, centimeter);
pyGET(ThinLens, FocalLength, centimeter);
