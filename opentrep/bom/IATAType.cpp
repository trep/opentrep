// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// OpenTrep Protobuf
#include <opentrep/Travel.pb.h>
// OpenTREP
#include <opentrep/IATAType.hpp>

namespace OPENTREP {
  
  // //////////////////////////////////////////////////////////////////////
  const std::string IATAType::_labels[LAST_VALUE] =
    { "CTY_AIRP", "CTY_HPT", "CTY_RSTN", "CTY_BSTN", "CTY_FERRY",
      "CITY", "AIRP", "HPT", "RSTN", "BSTN", "FERRY", "OFF" };

  // //////////////////////////////////////////////////////////////////////
  const char IATAType::_typeLabels[LAST_VALUE] =
    { 'a', 'h', 'r', 'b', 'p', 'C', 'A', 'H', 'R', 'B', 'P', 'O' };
  
  // //////////////////////////////////////////////////////////////////////
  IATAType::IATAType() : _type (LAST_VALUE) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  IATAType::IATAType (const IATAType& iIATAType)
    : _type (iIATAType._type) {
  }

  // //////////////////////////////////////////////////////////////////////
  IATAType::IATAType (const EN_IATAType& iIATAType)
    : _type (iIATAType) {
  }

  // //////////////////////////////////////////////////////////////////////
  IATAType::EN_IATAType IATAType::getType (const char iTypeChar) {
    EN_IATAType oType;
    switch (iTypeChar) {
    case 'a': oType = CTY_AIRP; break;
    case 'h': oType = CTY_HPT; break;
    case 'r': oType = CTY_RSTN; break;
    case 'b': oType = CTY_BSTN; break;
    case 'p': oType = CTY_FERRY; break;
    case 'C': oType = CITY; break;
    case 'A': oType = AIRP; break;
    case 'H': oType = HPT; break;
    case 'R': oType = RSTN; break;
    case 'B': oType = BSTN; break;
    case 'P': oType = FERRY; break;
    case 'O': oType = OFF; break;
    default: oType = LAST_VALUE; break;
    }

    if (oType == LAST_VALUE) {
      const std::string& lLabels = describeLabels();
      std::ostringstream oMessage;
      oMessage << "The place type '" << iTypeChar
               << "' is not known. Known place types: " << lLabels;
      throw CodeConversionException (oMessage.str());
    }

    return oType;
  }
  
  // //////////////////////////////////////////////////////////////////////
  IATAType::IATAType (const char iTypeChar)
    : _type (getType (iTypeChar)) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  IATAType::IATAType (const std::string& iTypeStr) : _type (LAST_VALUE) {
    if (iTypeStr == "CA") {
      _type = CTY_AIRP;
    } else if (iTypeStr == "CH") {
      _type = CTY_HPT;
    } else if (iTypeStr == "CR") {
      _type = CTY_RSTN;
    } else if (iTypeStr == "CB") {
      _type = CTY_BSTN;
    } else if (iTypeStr == "CP") {
      _type = CTY_FERRY;
    } else if (iTypeStr == "C") {
      _type = CITY;
    } else if (iTypeStr == "A") {
      _type = AIRP;
    } else if (iTypeStr == "H") {
      _type = HPT;
    } else if (iTypeStr == "R") {
      _type = RSTN;
    } else if (iTypeStr == "B") {
      _type = BSTN;
    } else if (iTypeStr == "P") {
      _type = FERRY;
    } else if (iTypeStr == "O") {
      _type = OFF;
    } else {
      _type = LAST_VALUE;
    }

    if (_type == LAST_VALUE) {
      const std::string& lLabels = describeLabels();
      std::ostringstream oMessage;
      oMessage << "The place type '" << iTypeStr
               << "' is not known. Known place types: " << lLabels;
      throw CodeConversionException (oMessage.str());
    }
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string& IATAType::getLabel (const EN_IATAType& iType) {
    return _labels[iType];
  }
  
  // //////////////////////////////////////////////////////////////////////
  char IATAType::getTypeLabel (const EN_IATAType& iType) {
    return _typeLabels[iType];
  }

  // //////////////////////////////////////////////////////////////////////
  std::string IATAType::getTypeLabelAsString (const EN_IATAType& iType) {
    std::ostringstream oStr;
    oStr << _typeLabels[iType];
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  treppb::PlaceType IATAType::getTypeLabelAsPB (const EN_IATAType& iType) {
    treppb::PlaceType oLocationType;
    switch (iType) {
    case CTY_AIRP: oLocationType.set_type (treppb::PlaceType::CTY_AIRP); break;
    case CTY_HPT: oLocationType.set_type (treppb::PlaceType::CTY_HPT); break;
    case CTY_RSTN: oLocationType.set_type (treppb::PlaceType::CTY_RSTN); break;
    case CTY_BSTN: oLocationType.set_type (treppb::PlaceType::CTY_BSTN); break;
    case CTY_FERRY: oLocationType.set_type (treppb::PlaceType::CTY_FERRY); break;
    case CITY: oLocationType.set_type (treppb::PlaceType::CITY); break;
    case AIRP: oLocationType.set_type (treppb::PlaceType::AIRP); break;
    case HPT: oLocationType.set_type (treppb::PlaceType::HPT); break;
    case RSTN: oLocationType.set_type (treppb::PlaceType::RSTN); break;
    case BSTN: oLocationType.set_type (treppb::PlaceType::BSTN); break;
    case FERRY: oLocationType.set_type (treppb::PlaceType::FERRY); break;
    case OFF: oLocationType.set_type (treppb::PlaceType::OFF); break;
    default: oLocationType.set_type (treppb::PlaceType::UNKNOWN); break;
    }
    return oLocationType;
  }

  // //////////////////////////////////////////////////////////////////////
  std::string IATAType::describeLabels() {
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
  IATAType::EN_IATAType IATAType::getType() const {
    return _type;
  }
  
  // //////////////////////////////////////////////////////////////////////
  char IATAType::getTypeAsChar() const {
    const char oTypeChar = _typeLabels[_type];
    return oTypeChar;
  }

  // //////////////////////////////////////////////////////////////////////
  std::string IATAType::getTypeAsString() const {
    std::ostringstream oStr;
    oStr << _typeLabels[_type];
    return oStr.str();
  }
  
  // //////////////////////////////////////////////////////////////////////
  treppb::PlaceType IATAType::getTypeAsPB() const {
    return getTypeLabelAsPB (_type);
  }
 
  // //////////////////////////////////////////////////////////////////////
  const std::string IATAType::describe() const {
    std::ostringstream ostr;
    ostr << _labels[_type];
    return ostr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  bool IATAType::operator== (const EN_IATAType& iType) const {
    return (_type == iType);
  }
  
  // //////////////////////////////////////////////////////////////////////
  bool IATAType::operator== (const IATAType& iIATAType) const {
    return (_type == iIATAType._type);
  }

}
