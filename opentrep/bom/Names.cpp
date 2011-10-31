// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// C
#include <cassert>
// STL
#include <sstream>
// OpenTrep
#include <opentrep/bom/Names.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  Names::Names() : _languageCode (Language::en_US) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  Names::Names (const Language::EN_Language& iLanguageCode)
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
  std::string Names::describeShortKey() const {
    std::ostringstream oStr;
    oStr << "[" << Language::getLongLabel (_languageCode) << "]: ";
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string Names::describeKey() const {
    return describeShortKey();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string Names::toString() const {
    std::ostringstream oStr;
    oStr << describeShortKey();

    for (NameList_T::const_iterator itName = _nameList.begin();
         itName != _nameList.end(); ++itName) {
      const std::string& lName = *itName;
      oStr << lName << ", ";
    }
    oStr << std::endl;
    
    return oStr.str();
  }
    
  // //////////////////////////////////////////////////////////////////////
  void Names::toStream (std::ostream& ioOut) const {
    ioOut << toString();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void Names::fromStream (std::istream& ioIn) {
  }

  // //////////////////////////////////////////////////////////////////////
  void Names::addName (const std::string& iName) {
    _nameList.push_back (iName);
  }

  // //////////////////////////////////////////////////////////////////////
  void Names::resetList () {
    _nameList.clear();
  }
  
}
