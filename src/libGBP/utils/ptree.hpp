#ifndef utils_ptree_hpp
#define utils_ptree_hpp

/** @file ptree.hpp
  * @brief A collection of ptree utility functions.
  * @author C.D. Clark III
  * @date 07/29/16
  */

#include <list>
#include <regex>
#include <boost/property_tree/ptree.hpp>

using boost::property_tree::ptree;


inline bool isInt( const std::string &str )
{
  if(std::regex_match(str, std::regex("^[0-9]+$") ) )
    return true;
  else
    return false;
}

inline bool intComp( const std::string &a, const std::string &b )
{
  return stoi(a) < stoi(b);
}


namespace boost
{
namespace property_tree
{
  // treat keys as integers and compare
  inline bool keyIntComp( ptree::const_iterator &a, ptree::const_iterator &b)
  {
    return intComp( a->first, b->first );
  }

  // returns all children that are "indexed" (have a key that can be converted to an int).
  template<typename Compare, typename Filter>
  std::list<ptree::const_iterator> getSortedChildren( const ptree& tree, Compare comp, Filter filt )
  {
    std::list< ptree::const_iterator > children;

    for( auto iter = tree.begin(); iter != tree.end(); iter++ )
    {
      if( filt(iter->first) )
        children.push_back( iter );
    }

    children.sort( comp );

    return children;
  }


}
}


#endif // include protector
