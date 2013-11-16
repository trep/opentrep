// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// Xapian
#include <xapian.h>
// OpenTrep
#include <opentrep/bom/StringSet.hpp>
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
  const Percentage_T& ResultCombination::getBestMatchingWeight() const {
    const ResultHolder& lResultHolder = getBestMatchingResultHolder();
    const Percentage_T& oPercentage = lResultHolder.getCombinedWeight();
    return oPercentage;
  }

  // //////////////////////////////////////////////////////////////////////
  StringSet ResultCombination::getCorrectedStringSet() const {
    const ResultHolder& lResultHolder = getBestMatchingResultHolder();
    const StringSet& oStringSet = lResultHolder.getCorrectedStringSet();
    return oStringSet;
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
  void ResultCombination::displayXapianPercentages() const {
    // Browse the ResultHolder objects
    for (ResultHolderList_T::const_iterator itResultHolder =
           _resultHolderList.begin();
         itResultHolder != _resultHolderList.end(); ++itResultHolder) {
      ResultHolder* lResultHolder_ptr = *itResultHolder;
      assert (lResultHolder_ptr != NULL);

      //
      lResultHolder_ptr->displayXapianPercentages();
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void ResultCombination::calculateEnvelopeWeights() const {
    // Browse the ResultHolder objects
    for (ResultHolderList_T::const_iterator itResultHolder =
           _resultHolderList.begin();
         itResultHolder != _resultHolderList.end(); ++itResultHolder) {
      ResultHolder* lResultHolder_ptr = *itResultHolder;
      assert (lResultHolder_ptr != NULL);

      //
      lResultHolder_ptr->calculateEnvelopeWeights();
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void ResultCombination::calculateCodeMatches() const {
    // Browse the ResultHolder objects
    for (ResultHolderList_T::const_iterator itResultHolder =
           _resultHolderList.begin();
         itResultHolder != _resultHolderList.end(); ++itResultHolder) {
      ResultHolder* lResultHolder_ptr = *itResultHolder;
      assert (lResultHolder_ptr != NULL);

      //
      lResultHolder_ptr->calculateCodeMatches();
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void ResultCombination::calculatePageRanks() const {
    // Browse the ResultHolder objects
    for (ResultHolderList_T::const_iterator itResultHolder =
           _resultHolderList.begin();
         itResultHolder != _resultHolderList.end(); ++itResultHolder) {
      ResultHolder* lResultHolder_ptr = *itResultHolder;
      assert (lResultHolder_ptr != NULL);

      //
      lResultHolder_ptr->calculatePageRanks();
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void ResultCombination::calculateHeuristicWeights() const {
    // Browse the ResultHolder objects
    for (ResultHolderList_T::const_iterator itResultHolder =
           _resultHolderList.begin();
         itResultHolder != _resultHolderList.end(); ++itResultHolder) {
      ResultHolder* lResultHolder_ptr = *itResultHolder;
      assert (lResultHolder_ptr != NULL);

      //
      lResultHolder_ptr->calculateHeuristicWeights();
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void ResultCombination::calculateCombinedWeights() const {
    // Browse the ResultHolder objects
    for (ResultHolderList_T::const_iterator itResultHolder =
           _resultHolderList.begin();
         itResultHolder != _resultHolderList.end(); ++itResultHolder) {
      ResultHolder* lResultHolder_ptr = *itResultHolder;
      assert (lResultHolder_ptr != NULL);

      //
      lResultHolder_ptr->calculateCombinedWeights();
    }
  }

  // //////////////////////////////////////////////////////////////////////
  bool ResultCombination::chooseBestMatchingResultHolder() {

    // Identify the ResultHolder object corresponding to the best
    // matching percentage
    Percentage_T lMaxMatchingPercentage = 0.0;
    for (ResultHolderList_T::const_iterator itResultHolder =
           _resultHolderList.begin();
         itResultHolder != _resultHolderList.end(); ++itResultHolder) {
      ResultHolder* lResultHolder_ptr = *itResultHolder;
      assert (lResultHolder_ptr != NULL);

      // Retrieve the weight from the full-text matching process
      const Percentage_T& lMatchingPercentage =
        lResultHolder_ptr->getCombinedWeight();

      // Override the maximum percentage, if needed
      if (lMatchingPercentage > lMaxMatchingPercentage) {
        lMaxMatchingPercentage = lMatchingPercentage;
        _bestMatchingResultHolder = lResultHolder_ptr;
      }
    }

    // DEBUG
    if (_bestMatchingResultHolder != NULL) {
      OPENTREP_LOG_DEBUG ("    [pct] The best match for the '" << describeKey()
                          << "' string has a weight of "
                          << lMaxMatchingPercentage
                          << "%. It is the following string partition: "
                          << _bestMatchingResultHolder->describeKey());

    } else {
      OPENTREP_LOG_DEBUG ("    [pct] There is no match for the '"
                          << describeKey() << "' string");
    }

    //
    const bool doesBestMatchingResultHolderExist =
      (_bestMatchingResultHolder != NULL);
    return doesBestMatchingResultHolderExist;
  }

}
