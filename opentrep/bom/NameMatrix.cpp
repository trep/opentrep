// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// C
#include <cassert>
// STL
#include <sstream>
// OpenTrep
#include <opentrep/bom/NameMatrix.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  NameMatrix::NameMatrix() {
  }

  // //////////////////////////////////////////////////////////////////////
  NameMatrix::NameMatrix (const NameMatrix& iNameMatrix)
    : _nameMatrix (iNameMatrix._nameMatrix) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  NameMatrix::~NameMatrix() {
  }

  // //////////////////////////////////////////////////////////////////////
  std::string NameMatrix::describeKey() const {
    std::ostringstream oStr;
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string NameMatrix::describe() const {
    std::ostringstream oStr;
    oStr << describeKey();

    for (NameMatrix_T::const_iterator itNameList = _nameMatrix.begin();
         itNameList != _nameMatrix.end(); ++itNameList) {
      const Names& lNameList = itNameList->second;
      oStr << lNameList.describe();
    }
    
    return oStr.str();
  }
    
  // //////////////////////////////////////////////////////////////////////
  void NameMatrix::toStream (std::ostream& ioOut) const {
    ioOut << describe();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void NameMatrix::fromStream (std::istream& ioIn) {
  }

  // //////////////////////////////////////////////////////////////////////
  bool NameMatrix::getNameList (const Language::EN_Language& iLanguageCode,
                                NameList_T& ioNameList) const {
    bool oFoundNameList = false;
    
    NameMatrix_T::const_iterator itNameList = _nameMatrix.find (iLanguageCode);
    if (itNameList != _nameMatrix.end()) {
      const Names& lNameList = itNameList->second;
      ioNameList = lNameList.getNameList();
      oFoundNameList = true;
    }

    return oFoundNameList;
  }
  
  // //////////////////////////////////////////////////////////////////////
  void NameMatrix::addName (const Language::EN_Language& iLanguageCode,
                            const std::string& iName) {

    // Check whether a name in that language has already been recorded
    NameMatrix_T::iterator itNameList = _nameMatrix.find (iLanguageCode);
    if (itNameList != _nameMatrix.end()) {
      // Just add the name for that language
      Names& lNameList = itNameList->second;

      lNameList.addName (iName);

    } else {
      // Create a new name list for the given language
      Names lNameList (iLanguageCode);
      lNameList.addName (iName);

      // Insert the name list with the dedicated list
      const bool insertSucceeded =
        _nameMatrix.insert (NameMatrix_T::value_type (iLanguageCode,
                                                      lNameList)).second;
      if (insertSucceeded == false) {
        OPENTREP_LOG_ERROR ("The " << iName << " name can not be inserted in "
                            << "the dedicated list for the "
                            << Language::getLongLabel (iLanguageCode)
                            << " language");
      }
      assert (insertSucceeded == true);
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void NameMatrix::reset() {
    _nameMatrix.clear();
  }

}
