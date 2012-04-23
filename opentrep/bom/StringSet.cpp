// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// OpenTREP
#include <opentrep/bom/StringSet.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  StringSet::StringSet() {
  }

  // //////////////////////////////////////////////////////////////////////
  StringSet::StringSet (const StringSet& iStringSet) {
    _set = iStringSet._set;
  }

  // //////////////////////////////////////////////////////////////////////
  StringSet::StringSet (const std::string& iString) {
    _set.push_back (iString);
  }

  // //////////////////////////////////////////////////////////////////////
  StringSet::~StringSet () {
  }

  // //////////////////////////////////////////////////////////////////////
  size_t StringSet::size() const {
    return _set.size();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void StringSet::clear() {
    _set.clear();
  }

  // //////////////////////////////////////////////////////////////////////
  void StringSet::push_back (const std::string& iString) {
    _set.push_back (iString);
  }
  
  // //////////////////////////////////////////////////////////////////////
  void StringSet::push_back (const StringSet& iStringSet) {
    const StringSet_T& lStringSet = iStringSet._set;
    for (StringSet_T::const_iterator itString = lStringSet.begin();
         itString != lStringSet.end(); ++itString) {
      const std::string& lString = *itString;
      push_back (lString);
    }
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string StringSet::describeShortKey() const {
    std::ostringstream oStr;
    oStr << "";
    return oStr.str();
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string StringSet::describeKey() const {
    std::ostringstream oStr;
    oStr << "";
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string StringSet::toShortString() const {
    std::ostringstream oStr;
    oStr << describeShortKey();

    //
    oStr << "  {";

    short idx_string = 0;
    for (StringSet_T::const_iterator itString = _set.begin();
         itString != _set.end(); ++itString, ++idx_string) {
      //
      if (idx_string != 0) {
        oStr << ", ";
      }

      //
      const std::string& lString = *itString;

      //
      oStr << "\"" << lString << "\"";
    }

    //
    oStr << "}";

    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string StringSet::toString() const {
    std::ostringstream oStr;
    oStr << describeKey();
    
    //
    oStr << "  {";

    short idx_string = 0;
    for (StringSet_T::const_iterator itString = _set.begin();
         itString != _set.end(); ++itString, ++idx_string) {
      //
      if (idx_string != 0) {
        oStr << ", ";
      }

      //
      const std::string& lString = *itString;

      //
      oStr << "\"" << lString << "\"";
    }

    //
    oStr << "}";
    
    return oStr.str();
  }   

  // //////////////////////////////////////////////////////////////////////
  void StringSet::toStream (std::ostream& ioOut) const {
    ioOut << toString();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void StringSet::fromStream (std::istream& ioIn) {
  }

}
