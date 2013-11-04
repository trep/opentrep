// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// OpenTrep
#include <opentrep/OPENTREP_exceptions.hpp>
#include <opentrep/basic/ScoreType.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  const std::string ScoreType::_labels[LAST_VALUE] =
        { "Combination", "Xapian Percentage", "Page Rank", "Passenger Number",
          "Heuristic", "Envelope ID", "IATA/ICAO Code Full Match"};

  const char ScoreType::_typeLabels[LAST_VALUE] = { 'C', 'X', 'R', 'N', 'H',
                                                    'E', 'F' };


  // //////////////////////////////////////////////////////////////////////
  ScoreType::ScoreType (const EN_ScoreType& iScoreType)
    : _type (iScoreType) {
  }

  // //////////////////////////////////////////////////////////////////////
  ScoreType::ScoreType (const char iType) {
    switch (iType) {
    case 'C': _type = COMBINATION; break;
    case 'X': _type = XAPIAN_PCT; break;
    case 'R': _type = PAGE_RANK; break;
    case 'N': _type = PAX_NB; break;
    case 'H': _type = HEURISTIC; break;
    case 'E': _type = ENV_ID; break;
    case 'F': _type = CODE_FULL_MATCH; break;
    default: _type = LAST_VALUE; break;
    }

    if (_type == LAST_VALUE) {
      const std::string& lLabels = describeLabels();
      std::ostringstream oMessage;
      oMessage << "The score type '" << iType
               << "' is not known. Known score types: " << lLabels;
      throw CodeConversionException (oMessage.str());
    }
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string& ScoreType::getLongLabel() const {
    return _labels[_type];
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string& ScoreType::getLabel (const EN_ScoreType& iType) {
    return _labels[iType];
  }

  // //////////////////////////////////////////////////////////////////////
  char ScoreType::getTypeLabel (const EN_ScoreType& iType) {
    return _typeLabels[iType];
  }

  // //////////////////////////////////////////////////////////////////////
  std::string ScoreType::
  getTypeLabelAsString (const EN_ScoreType& iType) {
    std::ostringstream oStr;
    oStr << _typeLabels[iType];
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string ScoreType::describeLabels() {
    std::ostringstream ostr;
    for (unsigned short idx = 0; idx != LAST_VALUE; ++idx) {
      if (idx != 0) {
        ostr << ", ";
      }
      ostr << _labels[idx];
    }
    return ostr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  ScoreType::EN_ScoreType ScoreType::getType() const {
    return _type;
  }

  // //////////////////////////////////////////////////////////////////////
  std::string ScoreType::getTypeAsString() const {
    std::ostringstream oStr;
    oStr << _typeLabels[_type];
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string ScoreType::describe() const {
    std::ostringstream ostr;
    ostr << _labels[_type];
    return ostr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  bool ScoreType::isIndividualScore() const {
    bool oIsIndividual = true;
    if (_type == COMBINATION || _type == LAST_VALUE) {
      oIsIndividual = false;
    }
    return oIsIndividual;
  }

  // //////////////////////////////////////////////////////////////////////
  bool ScoreType::isIndividualScore (const EN_ScoreType& iTypeEnum) {
    ScoreType lType (iTypeEnum);
    return lType.isIndividualScore();
  }

  // //////////////////////////////////////////////////////////////////////
  bool ScoreType::operator== (const EN_ScoreType& iType) const {
    return (_type == iType);
  }

}
