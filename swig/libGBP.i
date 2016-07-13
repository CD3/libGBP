%module libGBP
// NOTE: everything between %{ and %} gets copied verbatim into the *_wrap.cxx file

%{
#include "GaussianBeam.hpp"
%}

%include <std_string.i>
%include <std_complex.i>




/*  ____                     _             ____                       */
/* / ___| __ _ _   _ ___ ___(_) __ _ _ __ | __ )  ___  __ _ _ __ ___  */
/*| |  _ / _` | | | / __/ __| |/ _` | '_ \|  _ \ / _ \/ _` | '_ ` _ \ */
/*| |_| | (_| | |_| \__ \__ \ | (_| | | | | |_) |  __/ (_| | | | | | |*/
/* \____|\__,_|\__,_|___/___/_|\__,_|_| |_|____/ \___|\__,_|_| |_| |_|*/
                                                                    


class GaussianBeam
{

public:

%extend {

#define GETSET( name, unit ) \
void   set##name##DP( double v ){ $self->set##name( v*unit); } \
double get##name##DP(          ){ return quantity_cast<double>( $self->get##name() ); }

#define GET( name, unit ) \
double get##name##DP(          ){ return quantity_cast<double>( $self->get##name() ); }

GETSET(Frequency, hertz);
GETSET(Wavelength, nanometer);
GETSET(WaistPosition, centimeter);
GETSET(WaistDiameter, centimeter);
GETSET(Power, watt);
GETSET(CurrentPosition, centimeter);

GET(FreeSpaceWavelength, nanometer);
GET(RayleighRange, centimeter);

#undef GETSET
#undef GET

}

};


%define GETSET(name, unit)
%pythoncode %{
@ureg.wraps( None, (None,ureg('unit')), True )
def set##name (self,v):
  return self.set##name##DP(v)
GaussianBeam.set##name  = set##name 

@ureg.wraps( ureg('unit'), None, True )
def get##name (self):
  return self.get##name##DP()
GaussianBeam.get##name = get##name 
%}
%enddef

%define GET(name, unit)
%pythoncode %{
@ureg.wraps( ureg('unit'), None, True )
def get##name (self):
  return self.get##name##DP()
GaussianBeam.get##name = get##name 
%}
%enddef


// NOTE: everything between %pythoncode %{ and %} will be copied verbatim into the *.py file

%pythoncode %{
import pint
ureg = pint.UnitRegistry()
Q_   = ureg.Quantity
%}

GETSET(Frequency, hertz);
GETSET(Wavelength, nanometer);
GETSET(WaistPosition, centimeter);
GETSET(WaistDiameter, centimeter);
GETSET(Power, watt);
GETSET(CurrentPosition, centimeter);

GET(FreeSpaceWavelength, nanometer);
GET(RayleighRange, centimeter);



/* _____ _     _       _                   */
/*|_   _| |__ (_)_ __ | |    ___ _ __  ___ */
/*  | | | '_ \| | '_ \| |   / _ \ '_ \/ __|*/
/*  | | | | | | | | | | |__|  __/ | | \__ \*/
/*  |_| |_| |_|_|_| |_|_____\___|_| |_|___/*/
                                         


