// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <string>
#include <sstream>
// OpenTREP
#include <opentrep/bom/Result.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  Result::Result (const TravelQuery_T& iQueryString,
                  const Xapian::Database& iDatabase)
    : _resultHolder (NULL), _queryString (iQueryString), _database (iDatabase),
      _matchingDocuments (iQueryString) {
    init();
  }
  
  // //////////////////////////////////////////////////////////////////////
  Result::~Result() {
  }

  // //////////////////////////////////////////////////////////////////////
  void Result::init() {
  }

  // //////////////////////////////////////////////////////////////////////
  std::string Result::describeShortKey() const {
    std::ostringstream oStr;
    oStr << _queryString;
    return oStr.str();
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string Result::describeKey() const {
    return describeShortKey();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string Result::toString() const {
    std::ostringstream oStr;
    if (hasFullTextMatched() == true) {
      oStr << _matchingDocuments.describe();
    } else {
      oStr << "No full-text match for '" << _queryString << "'";
    }
    return oStr.str();
  }   

  // //////////////////////////////////////////////////////////////////////
  void Result::toStream (std::ostream& ioOut) const {
    ioOut << toString();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void Result::fromStream (std::istream& ioIn) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  Percentage_T Result::calculateMatchingWeight() const {
    return _matchingDocuments.calculateMatchingWeight();
  }

}
