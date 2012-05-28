// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// Xapian
#include <xapian.h>
// OpenTrep
#include <opentrep/bom/ResultHolder.hpp>
#include <opentrep/bom/ResultCombination.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  ResultCombination::ResultCombination()
    : _travelQuery (""), _bestMatchingResultHolder (NULL) {
    assert (false);
  }
  
  // //////////////////////////////////////////////////////////////////////
  ResultCombination::ResultCombination (const ResultCombination&)
    : _travelQuery (""), _bestMatchingResultHolder (NULL) {
    assert (false);
  }
  
  // //////////////////////////////////////////////////////////////////////
  ResultCombination::ResultCombination (const TravelQuery_T& iQueryString)
    : _travelQuery (iQueryString), _bestMatchingResultHolder (NULL) {
    init();
  }
  
  // //////////////////////////////////////////////////////////////////////
  ResultCombination::~ResultCombination() {
    _bestMatchingResultHolder = NULL;
  }

  // //////////////////////////////////////////////////////////////////////
  void ResultCombination::init () {
    _resultHolderList.clear();
  }

  // //////////////////////////////////////////////////////////////////////
  const ResultHolder& ResultCombination::getBestMatchingResultHolder() const {
    if (_bestMatchingResultHolder == NULL) {
      std::ostringstream oStr;
      oStr << "There is no best matching ResultHolder object for the given "
           << "query string: '" << _travelQuery << "'";
      OPENTREP_LOG_ERROR (oStr.str());
      throw NoBestMatchingResultHolderException (oStr.str());
    }
    assert (_bestMatchingResultHolder != NULL);
    return *_bestMatchingResultHolder;
  }

  // //////////////////////////////////////////////////////////////////////
  std::string ResultCombination::describeShortKey() const {
    std::ostringstream oStr;
    oStr << _travelQuery;
    return oStr.str();
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string ResultCombination::describeKey() const {
    return describeShortKey();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string ResultCombination::toString() const {
    std::ostringstream oStr;
    oStr << describeShortKey() << std::endl;
    
    for (ResultHolderList_T::const_iterator itResultHolder =
           _resultHolderList.begin();
         itResultHolder != _resultHolderList.end(); ++itResultHolder) {
      const ResultHolder* lResultHolder_ptr = *itResultHolder;
      assert (lResultHolder_ptr != NULL);
      
      oStr << " ==> " << std::endl << lResultHolder_ptr->toString();
    }
    
    return oStr.str();
  }   

  // //////////////////////////////////////////////////////////////////////
  void ResultCombination::toStream (std::ostream& ioOut) const {
    ioOut << toString();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void ResultCombination::fromStream (std::istream& ioIn) {
  }

  // //////////////////////////////////////////////////////////////////////
  bool ResultCombination::chooseBestMatchingResultHolder() {

    // Identify the ResultHolder object corresponding to the best
    // matching percentage
    Percentage_T lMaxMatchingPercentage = 0.0;
    for (ResultHolderList_T::const_iterator itResultHolder =
           _resultHolderList.begin();
         itResultHolder != _resultHolderList.end(); ++itResultHolder) {
      const ResultHolder* lResultHolder_ptr = *itResultHolder;
      assert (lResultHolder_ptr != NULL);

      // Retrieve the weight from the full-text matching process
      const Percentage_T& lMatchingPercentage =
        lResultHolder_ptr->calculateMatchingWeights();

      // Override the maximum percentage, if needed
      if (lMatchingPercentage > lMaxMatchingPercentage) {
        lMaxMatchingPercentage = lMatchingPercentage;
        _bestMatchingResultHolder = lResultHolder_ptr;
      }
    }

    //
    const bool doesBestMatchingResultHolderExist =
      (_bestMatchingResultHolder != NULL);
    return doesBestMatchingResultHolderExist;
  }

}
