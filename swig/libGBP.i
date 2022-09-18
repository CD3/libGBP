%module libGBP
// NOTE: everything between %{ and %} gets copied verbatim into the *_wrap.cxx file
%{
#include <libGBP/GaussianBeam.hpp>
#include <libGBP/BeamTransformations/BeamTransformation_Interface.hpp>
#include <libGBP/BeamTransformations/ThinLens.hpp>
#include <libGBP/GBPCalc.hpp>
#include <vector>
#include <utility>
using namespace libGBP;

%}


// NOTE: everything between %pythoncode %{ and %} will be copied verbatim into the *.py file
%pythoncode %{
import pint
ureg = pint.UnitRegistry()
Q_   = ureg.Quantity
%}


%include <std_string.i>
%include <std_vector.i>
%include <std_pair.i>


%define cppPROPERTY(NAME, UNIT)
void   set##NAME##DP( double v ){ $self->set##NAME( v*UNIT); } \
double get##NAME##DP(          ){ return boost::units::quantity_cast<double>( $self->get##NAME() ); }
%enddef
%define cppROPROPERTY(NAME, UNIT)
double get##NAME##DP(          ){ return boost::units::quantity_cast<double>( $self->get##NAME() ); }
%enddef

%define pyPROPERTY(CLASS, NAME, UNIT)
%pythoncode %{
@ureg.wraps( None, (None,'UNIT'), True )
def set##NAME (self,v):
  return self.set##NAME##DP(v)
CLASS.set##NAME  = set##NAME 
@ureg.wraps( 'UNIT', None, True )
def get##NAME (self):
  return self.get##NAME##DP()
CLASS.get##NAME = get##NAME 
%}
%enddef
%define pyROPROPERTY(CLASS, NAME, UNIT)
%pythoncode %{
@ureg.wraps( 'UNIT', None, True )
def get##NAME (self):
  return self.get##NAME##DP()
CLASS.get##NAME = get##NAME 
%}
%enddef


// OPTICS
                                         


template<typename T>
class ThinLens
{

public:

%extend {

cppPROPERTY(FocalLength, T());

}

};

%template(ThinLens) ThinLens<t::centimeter>;
pyPROPERTY(ThinLens, FocalLength, centimeter);



// Beams


class GaussianBeam
{

public:
  inline void setUseDiffractionLimitedDivergence(bool val);
  inline bool getUseDiffractionLimitedDivergence();

%extend {

cppPROPERTY(Frequency, hertz);
cppPROPERTY(Wavelength, nanometer);
cppPROPERTY(WaistPosition, centimeter);
cppPROPERTY(OneOverEWaistDiameter, centimeter);
cppPROPERTY(OneOverEFullAngleDivergence, milliradian);
cppPROPERTY(OneOverE2WaistRadius, centimeter);
cppPROPERTY(OneOverE2HalfAngleDivergence, milliradian);
cppPROPERTY(Power, watt);
cppPROPERTY(CurrentPosition, centimeter);

cppROPROPERTY(BeamPropagationFactor, dimensionless);
cppROPROPERTY(FreeSpaceWavelength, nanometer);
cppROPROPERTY(RayleighRange, centimeter);
cppROPROPERTY(OneOverEFullAngleDiffractionLimitedDivergence, milliradian);

double getOneOverEDiameterDP( double v ){ return boost::units::quantity_cast<double>( $self->getOneOverEDiameter(v*i::cm) ); }

void transformDP(ThinLens<t::centimeter> elem, double z){ $self->transform(&elem, z*centimeter); }


}

};

pyPROPERTY(GaussianBeam, Frequency, hertz);
pyPROPERTY(GaussianBeam, Wavelength, nanometer);
pyPROPERTY(GaussianBeam, WaistPosition, centimeter);
pyPROPERTY(GaussianBeam, OneOverEWaistDiameter, centimeter);
pyPROPERTY(GaussianBeam, OneOverEFullAngleDivergence, milliradian);
pyPROPERTY(GaussianBeam, OneOverE2WaistRadius, centimeter);
pyPROPERTY(GaussianBeam, OneOverE2HalfAngleDivergence, milliradian);
pyPROPERTY(GaussianBeam, Power, watt);
pyPROPERTY(GaussianBeam, CurrentPosition, centimeter);

pyROPROPERTY(GaussianBeam, BeamPropagationFactor, dimensionless);
pyROPROPERTY(GaussianBeam, FreeSpaceWavelength, nanometer);
pyROPROPERTY(GaussianBeam, RayleighRange, centimeter);
pyROPROPERTY(GaussianBeam, OneOverEFullAngleDiffractionLimitedDivergence, milliradian);

%pythoncode %{
@ureg.wraps( 'cm', (None,'cm'), True )
def getOneOverEDiameter(self,v):
  return self.getOneOverEDiameterDP(v)
GaussianBeam.getOneOverEDiameter = getOneOverEDiameter
@ureg.wraps( 'cm', (None), True )
def getOneOverEDiameterAtCurrentPosition(self):
  return self.getOneOverEDiameterDP(self.getCurrentPosition().to("cm").magnitude)
GaussianBeam.getOneOverEDiameterAtCurrentPosition = getOneOverEDiameterAtCurrentPosition
%}
%pythoncode %{
@ureg.wraps( None, (None,None,'cm'), True )
def transform(self,e,v):
  return self.transformDP(e,v)
GaussianBeam.transform = transform
%}




template<typename LengthUnitType>
class GBPCalc
{
  public:

  void clear();

  %extend {

  void configure(std::string configString) {
    std::stringstream ss;
    ss << configString;

    boost::property_tree::ptree pt;
    boost::property_tree::read_json(ss,pt);
    self->configure(pt);

  }

  std::vector<GaussianBeam> calculateBeam()
  {
    std::vector<GaussianBeam> v;

    auto connection = self->sig_calculatedBeam.connect(
    [&v](const GaussianBeam& beam)
    {
      v.push_back( beam );
    }
    );
    self->calculate();
    connection.disconnect();

    return v;
  }


  }
};


%template(GBPCalc) GBPCalc<t::centimeter>;
%template(vector) std::vector<double>;
%template(beam_vector) std::vector<GaussianBeam>;

%pythoncode %{
@ureg.wraps( 'cm', None, True )
def calculateOneOverEDiameters(self):
  return self.calculateOneOverEDiametersDP()
GBPCalc.calculateOneOverEDiameters = calculateOneOverEDiameters
%}
