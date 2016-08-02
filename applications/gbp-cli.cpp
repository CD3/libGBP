
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <GBPCalc.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/program_options.hpp>

using boost::property_tree::getSortedChildren;
namespace po = boost::program_options;


void print_usage( std::string program_name, po::options_description &options_desc)
{
  std::cerr << "Usage: " << program_name << " [options] [<infile>] "<<std::endl;
  std::cerr << options_desc << std::endl;
  exit (8);
}


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
  if(std::regex_match(name, std::regex(pattern, std::regex_constants::icase ) ) ) { return toString( beam.get##param().value() ); }

RETURN("^beam\\s*diameter$", Diameter);
RETURN("^diameter$", Diameter);
RETURN("^divergence$", Divergence );
RETURN("^radius\\s*of\\s*curvature$", Divergence );
RETURN("^RoC$", Divergence );

return "UNKNOWN";
}

// structure for managing data to log
struct Log
{
  std::queue<std::string> lines;
  std::string line;

  std::vector<std::string> inputNames;
  std::vector<std::string> outputNames;

  std::string filename;

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

  void write( )
  {
    std::ofstream out( filename );
    write(out);
    out.close();
  }

  void readOutputs( GaussianBeam beam )
  {
    for( auto name : outputNames )
      this->stage( getBeamParam( beam, name ) );
  }

  void readInputs( ptree config )
  {
    for( auto name : inputNames )
      this->stage( config.get<std::string>( name, "UNKNOWN") );
  }

  void setFilename( std::string prefix, std::string name)
  {
    filename = prefix+"."+name+".log";
  }


};


int main(int argc, char *argv[])
{


  // option handler
  po::options_description options("Supported Options");
  options.add_options()
      ("help,h"                                                                           , "print help message")
      ("verbose,v"                , po::value<int>()->default_value(0)                    , "the verbose level (-v -> verbose level 1, -vv -> verbose level 2, etc)" )
      ("debug,d"                  , po::value<int>()->default_value(0)                    , "the debug level (-d -> debug level 1, -dd -> debug level 2, etc)" )
      ("config,m"                 , po::value<std::string>()->default_value("gbp.conf")   , "configuration file" )
      ;

  po::positional_options_description args;
  args.add("config", 1);


  po::variables_map vm;
  po::store(  po::command_line_parser(argc, argv).options(options).positional(args).run(), vm);
  po::notify(vm);

  // options (including arguments) have been parsed.

  if (argc == 1 || vm.count("help"))
  {
    print_usage( argv[0], options );
    return 1;
  }
  



  // application


  GBPCalc<t::centimeter> calculator;
  ptree configTree;

  // read config file.
  configTree = readConfig( vm["config"].as<std::string>(), "ini" );
  //boost::property_tree::write_json( std::cout, configTree );


  for( auto run : configTree.get_child("parametric_runs") )
  {
    ptree configTreeCopy = configTree;
    std::string x_name = run.second.get<std::string>("parameters.0.name");
    std::queue<double> x_vals;
    boost::optional<double> min = run.second.get<double>("parameters.0.min");
    boost::optional<double> max = run.second.get<double>("parameters.0.max");
    boost::optional<size_t> n   = run.second.get<size_t>("parameters.0.n");

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
    std::string logPrefix = run.second.get<std::string>("logging.prefix", "GBP");
    for( auto logConfig : getSortedChildren( run.second.get_child("logging.loggers"), keyIntComp, isInt) )
    {
      std::string name = logConfig->second.get<std::string>("tag","data");
      logs[ name ] = boost::shared_ptr<Log>( new Log() );
      logs[ name ]->setFilename( logPrefix, name );
      logs[ name ]->stage("#");
      calculator.sig_calculatedBeam.connect( decltype(calculator.sig_calculatedBeam)::slot_type( &Log::readOutputs, logs[ name ].get(), _1 ).track(logs[name]) );
      if( logConfig->second.get_child_optional("inputs") )
      {
        for( auto data : getSortedChildren( logConfig->second.get_child("inputs"), keyIntComp, isInt) )
        {
          std::string inputName = data->second.get<std::string>("name");
          logs[ name ]->inputNames.push_back( inputName );
          logs[ name ]->stage( inputName );
        }
      }
      if( logConfig->second.get_child_optional("outputs") )
      {
        for( auto data : getSortedChildren( logConfig->second.get_child("outputs"), keyIntComp, isInt) )
        {
          std::string outputName = data->second.get<std::string>("name");
          logs[ name ]->outputNames.push_back( data->second.get<std::string>("name") );
          logs[ name ]->stage( outputName );
        }
      }
      logs[ name ]->push();
    }


    while( !x_vals.empty() )
    {
      configTreeCopy.put( x_name, x_vals.front() );

      calculator.configure( configTreeCopy );

      // make sure inputs get logged first
      for( auto log : logs )
        log.second->readInputs( configTreeCopy );

      calculator.calculate();

      // push new line onto log
      for( auto log : logs )
        log.second->push();

      x_vals.pop();
    }

    for( auto log : logs )
      log.second->write();



  }

 

  return 0;
}
