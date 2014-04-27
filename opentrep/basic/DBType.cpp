// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// OpenTREP
#include <opentrep/DBType.hpp>

namespace OPENTREP {
  
  // //////////////////////////////////////////////////////////////////////
  const std::string DBType::_labels[LAST_VALUE] =
    { "NoDB", "SQLite3", "MySQL/MariaDB" };

  // //////////////////////////////////////////////////////////////////////
  const char DBType::_typeLabels[LAST_VALUE] = { 'N', 'S', 'M' };
  
  // //////////////////////////////////////////////////////////////////////
  DBType::DBType() : _type (LAST_VALUE) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  DBType::DBType (const DBType& iDBType)
    : _type (iDBType._type) {
  }

  // //////////////////////////////////////////////////////////////////////
  DBType::DBType (const EN_DBType& iDBType)
    : _type (iDBType) {
  }

  // //////////////////////////////////////////////////////////////////////
  DBType::EN_DBType DBType::getType (const char iTypeChar) {
    EN_DBType oType;
    switch (iTypeChar) {
    case 'N': oType = NODB; break;
    case 'S': oType = SQLITE3; break;
    case 'M': oType = MYSQL; break;
    default: oType = LAST_VALUE; break;
    }

    if (oType == LAST_VALUE) {
      const std::string& lLabels = describeLabels();
      std::ostringstream oMessage;
      oMessage << "The database type '" << iTypeChar
               << "' is not known. Known database types: " << lLabels;
      throw CodeConversionException (oMessage.str());
    }

    return oType;
  }
  
  // //////////////////////////////////////////////////////////////////////
  DBType::DBType (const char iTypeChar)
    : _type (getType (iTypeChar)) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  DBType::DBType (const std::string& iTypeStr) : _type (LAST_VALUE) {
    if (iTypeStr == "sqlite3") {
      _type = SQLITE3;
    } else if (iTypeStr == "sqlite" || iTypeStr == "sqlite3") {
      _type = SQLITE3;
    } else if (iTypeStr == "mysql" || iTypeStr == "mariadb") {
      _type = MYSQL;
    } else if (iTypeStr == "nodb") {
      _type = NODB;
    } else {
      _type = LAST_VALUE;
    }

    if (_type == LAST_VALUE) {
      const std::string& lLabels = describeLabels();
      std::ostringstream oMessage;
      oMessage << "The database type '" << iTypeStr
               << "' is not known. Known database types: " << lLabels;
      throw CodeConversionException (oMessage.str());
    }
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string& DBType::getLabel (const EN_DBType& iType) {
    return _labels[iType];
  }
  
  // //////////////////////////////////////////////////////////////////////
  char DBType::getTypeLabel (const EN_DBType& iType) {
    return _typeLabels[iType];
  }

  // //////////////////////////////////////////////////////////////////////
  std::string DBType::getTypeLabelAsString (const EN_DBType& iType) {
    std::ostringstream oStr;
    oStr << _typeLabels[iType];
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string DBType::describeLabels() {
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
  DBType::EN_DBType DBType::getType() const {
    return _type;
  }
  
  // //////////////////////////////////////////////////////////////////////
  char DBType::getTypeAsChar() const {
    const char oTypeChar = _typeLabels[_type];
    return oTypeChar;
  }

  // //////////////////////////////////////////////////////////////////////
  std::string DBType::getTypeAsString() const {
    std::ostringstream oStr;
    oStr << _typeLabels[_type];
    return oStr.str();
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string DBType::describe() const {
    std::ostringstream ostr;
    ostr << _labels[_type];
    return ostr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  bool DBType::operator== (const EN_DBType& iType) const {
    return (_type == iType);
  }
  
  // //////////////////////////////////////////////////////////////////////
  bool DBType::operator== (const DBType& iDBType) const {
    return (_type == iDBType._type);
  }

}
