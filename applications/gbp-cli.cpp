
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <GBPCalc.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

using boost::property_tree::getSortedChildren;


// read configuration file (in various formats) and return a property tree
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
    boost::property_tree::read_json( filename, configTree);
  }

  if( format == "xml" )
  {
    boost::property_tree::read_xml( filename, configTree);
  }

  return configTree;
}

// utility function to convert data to strings
template<typename T>
std::string toString( T v )
{
  std::stringstream stream;
  stream << v;
  return stream.str();
}

// get a parameter from a beam based on a parameter name
std::string getBeamParam( GaussianBeam &beam, std::string name )
{
  
#define RETURN( pattern, param )\
  if(std::regex_match(name, std::regex(pattern, std::regex_constants::icase ) ) ) { return toString( beam.get##param() ); }

RETURN("^beam\\s*diameter$", Diameter);
RETURN("^diameter$", Diameter);
RETURN("^divergence$", Divergence );

return "UNKNOWN";
}

// structor for managing data to log
struct Log
{
  std::queue<std::string> lines;
  std::string line;

  std::vector<std::string> paramNames;

  template<typename T>
  void stage(T data)
  {
    if( line != "" )
      line += " ";

    line += toString(data);
  }

  void push()
  {
    lines.push( line );
    line = "";
  }

  void write( std::ostream& out )
  {
    while( !lines.empty() )
    {
      out << lines.front() << "\n";
      lines.pop();
    }
  }

  void read( GaussianBeam beam )
  {
    for( auto name : paramNames )
      this->stage( getBeamParam( beam, name ) );
  }


};


int main(int argc, char *argv[])
{

  std::string filename = "gbp.conf";


  GBPCalc<t::centimeter> calculator;
  ptree configTree;

  // read config file.
  configTree = readConfig( filename, "ini" );
  //boost::property_tree::write_json( std::cout, configTree );


  for( auto run : configTree.get_child("parametric_runs") )
  {
    std::string x_name = run.second.get<std::string>("parameter.name");
    std::queue<double> x_vals;
    boost::optional<double> min = run.second.get<double>("parameter.min");
    boost::optional<double> max = run.second.get<double>("parameter.max");
    boost::optional<size_t> n   = run.second.get<size_t>("parameter.n");

    if(min && max && n)
    {
      double dz = (max.value() - min.value())/(n.value()-1);

      for( size_t i = 0; i < n.value(); i++)
        x_vals.push( (min.value() + i*dz) );
    }
    auto valuesConfig = run.second.get_child_optional("values");
    if(valuesConfig)
      for( auto iter: getSortedChildren( valuesConfig.value(), keyIntComp, isInt ) )
        x_vals.push( iter->second.get<double>("") );


    std::map<std::string, boost::shared_ptr<Log>> logs;
    for( auto logConfig : getSortedChildren( run.second.get_child("logging"), keyIntComp, isInt) )
    {
      std::string name = logConfig->second.get<std::string>("tag","data");
      logs[ name ] = boost::shared_ptr<Log>( new Log() );
      calculator.sig_calculatedBeam.connect( decltype(calculator.sig_calculatedBeam)::slot_type( &Log::read, logs[ name ].get(), _1 ).track(logs[name]) );
      for( auto data : getSortedChildren( logConfig->second.get_child("data"), keyIntComp, isInt) )
        logs[ name ]->paramNames.push_back( data->second.get<std::string>("name") );
    }


    while( !x_vals.empty() )
    {
      configTree.put( x_name, x_vals.front() );

      calculator.configure( configTree );

      // log the parameter being changed
      for( auto log : logs )
        log.second->stage( x_vals.front() );

      calculator.calculate();

      // push new line onto log
      for( auto log : logs )
        log.second->push();


      x_vals.pop();
    }

    for( auto log : logs )
      log.second->write(std::cout);



  }

 
  // write output files

  return 0;
}
