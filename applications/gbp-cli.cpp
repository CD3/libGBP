
#include <iostream>
#include <fstream>
#include <GBPCalc.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

class Sensor
{
  public:
};

void outputBeamDiameter( GaussianBeam beam )
{
  std::cout<< beam.getCurrentPosition() << " " << beam.getDiameter() << std::endl;
}

ptree readConfig(std::string filename, std::string format = "ini")
{
  ptree configTree;
  if( format == "ini" )
  {
    ptree stage;
    boost::property_tree::read_ini( filename, stage);

    for(auto iter : stage)
      configTree.put( iter.first, iter.second.get<std::string>("") );
  }

  if( format == "json" )
  {
  }

  return configTree;
}

int main(int argc, char *argv[])
{

  std::string filename = "gbp.conf";


  GBPCalc<t::centimeter> calculator;
  ptree configTree;

  // read config file.
  configTree = readConfig( filename, "ini" );
  boost::property_tree::write_json( std::cout, configTree );

  // configure the calculator
  calculator.configure( configTree );
  calculator.sig_calculatedBeam.connect( outputBeamDiameter );
  // run calculation
  calculator.calculate();

  
  // write output files

  return 0;
}
