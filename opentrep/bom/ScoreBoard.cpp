// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// C
#include <cassert>
#include <sstream>
// Boost
#include <boost/lexical_cast.hpp>
// OpenTrep
#include <opentrep/bom/PlaceKey.hpp>
#include <opentrep/bom/WordHolder.hpp>
#include <opentrep/bom/ScoreBoard.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  ScoreBoard::~ScoreBoard () {
  }

  // //////////////////////////////////////////////////////////////////////
  std::string ScoreBoard::describeKey() const {
    std::ostringstream oStr;
    oStr << "";
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string ScoreBoard::describe() const {
    std::ostringstream oStr;
    oStr << describeKey();

    unsigned short idx = 0;
    for (ScoreList_T::const_iterator itScore = _scoreList.begin();
         itScore != _scoreList.end(); ++itScore, ++idx) {
      if (idx != 0) {
        oStr << ", ";
      }
      const ScoreType::EN_ScoreType& lScoreType = itScore->first;
      const Score_T& lScore = itScore->second;
      oStr << "[" << lScoreType << "] " << lScore;
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
  
}
