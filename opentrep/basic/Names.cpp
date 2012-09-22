// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// OpenTrep
#include <opentrep/Names.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  Names::Names() : _languageCode ("en_US") {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  Names::Names (const LanguageCode_T& iLanguageCode)
    : _languageCode (iLanguageCode) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  Names::Names (const Names& iName)
    : _languageCode (iName._languageCode),
      _nameList (iName._nameList) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  Names::~Names() {
  }

  // //////////////////////////////////////////////////////////////////////
  std::string Names::getFirstName() const {
    if (_nameList.empty() == true) {
      return "";
    }
    NameList_T::const_iterator itName = _nameList.begin();
    assert (itName != _nameList.end());
    const std::string& lName = *itName;
    return lName;
  }

  // //////////////////////////////////////////////////////////////////////
  std::string Names::describeKey() const {
    std::ostringstream oStr;
    //oStr << "[" << Language::getLongLabel (_languageCode) << "]: ";
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string Names::describe() const {
    std::ostringstream oStr;
    oStr << describeKey();

    // The language code is the same for all the items of the list. But it
    // is repeated for every item, so that it can be parsed easily.
    // const std::string& lLangCode = Language::getShortLabel (_languageCode);

    unsigned short idx = 0;
    for (NameList_T::const_iterator itName = _nameList.begin();
         itName != _nameList.end(); ++itName, ++idx) {
      if (idx != 0) {
        oStr << ",";
      }
      const std::string& lName = *itName;
      oStr << _languageCode << "," << lName;
    }

    return oStr.str();
  }
    
  // //////////////////////////////////////////////////////////////////////
  void Names::toStream (std::ostream& ioOut) const {
    ioOut << describe();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void Names::fromStream (std::istream& ioIn) {
  }

  // //////////////////////////////////////////////////////////////////////
  std::string Names::toString() const {
    std::ostringstream oStr;
    oStr << describe();
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  void Names::addName (const std::string& iName) {
    _nameList.push_back (iName);
  }

  // //////////////////////////////////////////////////////////////////////
  void Names::resetList() {
    _nameList.clear();
  }
  
}
