// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// Boost
#include <boost/lexical_cast.hpp>
// OpenTrep
#include <opentrep/basic/float_utils.hpp>
#include <opentrep/bom/WordHolder.hpp>
#include <opentrep/bom/ScoreBoard.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  ScoreBoard::ScoreBoard (const TravelQuery_T& iQueryString)
    : _queryString (iQueryString) {
  }

  // //////////////////////////////////////////////////////////////////////
  ScoreBoard::ScoreBoard (const ScoreBoard& iScoreBoard)
    : _queryString (iScoreBoard._queryString),
      _scoreMap (iScoreBoard._scoreMap) {
  }

  // //////////////////////////////////////////////////////////////////////
  ScoreBoard::ScoreBoard (const TravelQuery_T& iQueryString,
                          const ScoreType& iType, const Score_T& iScore)
    : _queryString (iQueryString) {
    setScore (iType, iScore);
  }

  // //////////////////////////////////////////////////////////////////////
  ScoreBoard::~ScoreBoard() {
    _scoreMap.clear();
  }

  // //////////////////////////////////////////////////////////////////////
  Score_T ScoreBoard::getScore (const ScoreType& iScoreType) const {
    Score_T oScore = 0.0;

    // Check whether a score value already exists for that type
    const ScoreType::EN_ScoreType& lScoreTypeEnum = iScoreType.getType();
    ScoreMap_T::const_iterator itScore = _scoreMap.find (lScoreTypeEnum);
    if (itScore != _scoreMap.end()) {
      oScore = itScore->second;
    }

    return oScore;
  }

  // //////////////////////////////////////////////////////////////////////
  void ScoreBoard::setScore (const ScoreType& iScoreType,
                             const Score_T& iScore) {
    Score_T oScore = iScore;

    /**
     * For the full-text matching process, a trick is used to decrease
     * the overall percentage of word combinations, when compared to
     * the whole string. For instance, {"san francisco"}
     * will have a percentage of 99.999%, compared to {"san", "francisco"}
     * which will have a percentage of 99.998%.
     */
    if (iScoreType == ScoreType::XAPIAN_PCT) {
      const FloatingPoint<Percentage_T> lComparablePct (oScore);
      const FloatingPoint<Percentage_T> lFullMatchingPct (100.0);
      if (lComparablePct.AlmostEquals (lFullMatchingPct) == true) {
        oScore = 99.999;
      }
    }

    // Check whether a score value already exists for that type
    const ScoreType::EN_ScoreType& lScoreTypeEnum = iScoreType.getType();
    ScoreMap_T::iterator itScore = _scoreMap.find (lScoreTypeEnum);

    if (itScore != _scoreMap.end()) {
      // Just replace the score value
      Score_T& lScore = itScore->second;
      lScore = oScore;

    } else {
      // Insert the score value for that new type
      const bool insertSucceeded =
        _scoreMap.insert (ScoreMap_T::value_type (lScoreTypeEnum,
                                                  oScore)).second;

      // Sanity check
      if (insertSucceeded == false) {
        OPENTREP_LOG_ERROR ("The " << iScore << " score value can not be "
                            << "inserted in the dedicated list for the "
                            << iScoreType.describe() << " score type");
      }
      assert (insertSucceeded == true);
    }
  }

  // //////////////////////////////////////////////////////////////////////
  std::string ScoreBoard::describeKey() const {
    std::ostringstream oStr;
    oStr << _queryString;
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string ScoreBoard::describe() const {
    std::ostringstream oStr;
    oStr << describeKey() << " - ";

    unsigned short idx = 0;
    for (ScoreMap_T::const_iterator itScore = _scoreMap.begin();
         itScore != _scoreMap.end(); ++itScore, ++idx) {
      if (idx != 0) {
        oStr << ", ";
      }
      const ScoreType::EN_ScoreType& lScoreType = itScore->first;
      const Score_T& lScore = itScore->second;
      oStr << ScoreType::getTypeLabelAsString (lScoreType) << ": "
           << lScore << "%";
    }

    return oStr.str();
  }   

  // //////////////////////////////////////////////////////////////////////
  void ScoreBoard::toStream (std::ostream& ioOut) const {
    ioOut << describe();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void ScoreBoard::fromStream (std::istream& ioIn) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  Percentage_T ScoreBoard::calculateCombinedWeight() {
    Percentage_T oPercentage = 100.0;

    // Browse the registered scores
    for (ScoreMap_T::iterator itScore = _scoreMap.begin();
         itScore != _scoreMap.end(); ++itScore) {
      const ScoreType::EN_ScoreType& lScoreType = itScore->first;
      Score_T& lScore = itScore->second;

      /**
       * Take into account the score only when it is valid and does
       * correspond to an individual type (i.e., when it is not the
       * combined score).
       */
      const bool isIndividual = ScoreType::isIndividualScore (lScoreType);
      if (isIndividual == true) {
        oPercentage *= lScore / 100.0;
      }

      /**
      // DEBUG
      OPENTREP_LOG_DEBUG ("        [pct] '" << describeKey() << "' - "
                          << ScoreType::getTypeLabelAsString (lScoreType)
                          << ": " << lScore << "%, combined: "
                          << oPercentage << "%");
      */
    }

    // Register the combined score
    setCombinedWeight (oPercentage);

    //
    return oPercentage;
  }

}
