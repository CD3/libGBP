%module libGBP
// everything between %{ and %} gets copied verbatim into the *_wrap.cxx file

// this is a generic exception handler that will work for multiple
// target languages. it basically just forwards any exceptions that are thrown
// to the swig exceptiosn library. specializations can be added before the
// default 'catch(...)' if more control is needed.
/*%exception {*/
  /*try {*/
    /*$action*/
  /*} catch (const std::invalid_argument& e) {*/
    /*SWIG_exception(SWIG_ValueError, e.what());*/
  /*} catch (const std::exception& e) {*/
    /*SWIG_exception(SWIG_RuntimeError, e.what());*/
  /*}*/
/*}*/


%{
#include "GaussianBeam.hpp"
%}

%include "GaussianBeam.hpp"

/*%template(MPECalc_2007) MPECalc<Z136_1_2007>;*/
/*%template(MPECalc_2014) MPECalc<Z136_1_2014>;*/

