#ifndef Builder_hpp
#define Builder_hpp

/** @file Builder.hpp
  * @brief 
  * @author C.D. Clark III
  * @date 07/24/16
  */

#include <map>
#include <string>
#include <memory>
#include <regex>
#include <functional>
#include <boost/property_tree/ptree.hpp>

template<typename BASE_CLASS, typename DERIVED_CLASS>
std::shared_ptr<BASE_CLASS> createInstance() { return std::shared_ptr<BASE_CLASS>( new DERIVED_CLASS() ); }

template<typename BASE_CLASS>
class Builder
{
  protected:
    typedef std::function<std::shared_ptr<BASE_CLASS>(void)> factoryFunc;
    std::map<std::string, std::string > namesMap; ///< Mapping from regex's to type names.
    std::map<std::string, factoryFunc > creators; ///< set of functions that create the instances from a typename.

    std::string getTypeName( std::string name );

  public:
    virtual std::shared_ptr<BASE_CLASS> create( std::string typeName );
    virtual std::shared_ptr<BASE_CLASS> build( const ptree& configTree );
    virtual void addNameMapping( std::string from, std::string to );
    virtual void addType( std::string typeName, std::function<std::shared_ptr<BASE_CLASS>(void) > );

    virtual void configure( std::shared_ptr<BASE_CLASS> elem, const ptree& configTree ) = 0;

};



template<typename T>
std::string
Builder<T>::getTypeName( std::string name )
{
  std::transform(name.begin(), name.end(), name.begin(), ::tolower);

  for(auto mapping : namesMap)
  {
    if(std::regex_match(name, std::regex(mapping.first) ) )
      return mapping.second;
  }

  return "UNKNOWN";
}

template<typename T>
std::shared_ptr<T>
Builder<T>::create( std::string typeName )
{
  auto c = creators.find( getTypeName(typeName) );
  return c != creators.end() ? (c->second)() : nullptr;
}

template<typename T>
std::shared_ptr<T>
Builder<T>::build( const ptree& configTree )
{
  std::shared_ptr<T> elem = create( configTree.get("type", "UNKNOWN") );
  configure( elem, configTree );

  return elem;
}

template<typename T>
void
Builder<T>::addNameMapping( std::string from, std::string to )
{
  namesMap[from] = to;
}

template<typename T>
void
Builder<T>::addType( std::string typeName, factoryFunc func )
{
  creators[typeName] = func;
}


#endif // include protector
