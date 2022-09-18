#pragma once

/** @file Builder.hpp
 * @brief
 * @author C.D. Clark III
 * @date 07/24/16
 */

#include <functional>
#include <map>
#include <memory>
#include <regex>
#include <string>

#include "../utils/ptree.hpp"

namespace libGBP
{
template<typename CLASS>
CLASS* createInstance_imp(const std::true_type&)
{
  return nullptr;
}

template<typename CLASS>
CLASS* createInstance_imp(const std::false_type&)
{
  return new CLASS();
}

// return a new instance of DERIVED_CLASS as a BASE_CLASS pointer.
// if DERIVED_CLASS is abstract, nullptr is returned.
// it is necessary for this function to return a BASE_CLASS pointer so that it
// has the same signature for all DERIVED_CLASS types and can be stored in a
// container.
template<typename BASE_CLASS, typename DERIVED_CLASS>
BASE_CLASS* createInstance()
{
  return createInstance_imp<DERIVED_CLASS>(std::is_abstract<DERIVED_CLASS>());
}

template<typename BASE_CLASS>
class Builder
{
 protected:
  typedef std::function<BASE_CLASS*(void)> factoryFunc;
  std::map<std::string, std::string>
      namesMap;  ///< Mapping from regex's to type names.
  std::map<std::string, factoryFunc>
      creators;  ///< set of functions that create the instances from a
                 ///< typename.

  std::string getTypeName(std::string name);

 public:
  virtual void addNameMapping(std::string from, std::string to);
  virtual void addType(std::string typeName, factoryFunc);

  virtual BASE_CLASS* create(std::string typeName = "default");
  virtual BASE_CLASS* build(const ptree& configTree);

  virtual void configure(BASE_CLASS* elem, const ptree& configTree) = 0;
};

template<typename T>
std::string Builder<T>::getTypeName(std::string name)
{
  std::transform(name.begin(), name.end(), name.begin(), ::tolower);

  for(auto mapping : namesMap) {
    if(std::regex_match(name, std::regex(mapping.first)))
      return mapping.second;
  }

  return "UNKNOWN";
}

template<typename T>
T* Builder<T>::create(std::string typeName)
{
  auto c = creators.find(getTypeName(typeName));
  // this will return an instance of the class specified by typeName
  // if typeName cannot be found, an instance of the base class is returned.
  // if the base class is abstract, nullptr will be returned.
  return c != creators.end() ? (c->second)() : createInstance<T, T>();
}

template<typename T>
T* Builder<T>::build(const ptree& configTree)
{
  T* elem = create(configTree.get("type", "UNKNOWN"));
  configure(elem, configTree);

  return elem;
}

template<typename T>
void Builder<T>::addNameMapping(std::string from, std::string to)
{
  namesMap[from] = to;
}

template<typename T>
void Builder<T>::addType(std::string typeName, factoryFunc func)
{
  creators[typeName] = func;
}

}  // namespace libGBP
