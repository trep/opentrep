// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// OpenTREP
#include <opentrep/OutputFormat.hpp>

namespace OPENTREP {
  
  // //////////////////////////////////////////////////////////////////////
  const std::string OutputFormat::_labels[LAST_VALUE] =
    { "Short", "Full", "JSON", "PROTOBUF" };

  // //////////////////////////////////////////////////////////////////////
  const char OutputFormat::_formatLabels[LAST_VALUE] = { 'S', 'F', 'J', 'P' };

  
  // //////////////////////////////////////////////////////////////////////
  OutputFormat::OutputFormat() : _format (LAST_VALUE) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  OutputFormat::
  OutputFormat (const OutputFormat& iOutputFormat)
    : _format (iOutputFormat._format) {
  }

  // //////////////////////////////////////////////////////////////////////
  OutputFormat::
  OutputFormat (const EN_OutputFormat& iOutputFormat)
    : _format (iOutputFormat) {
  }

  // //////////////////////////////////////////////////////////////////////
  OutputFormat::EN_OutputFormat
  OutputFormat::getFormat (const char iFormatChar) {
    EN_OutputFormat oFormat;
    switch (iFormatChar) {
    case 'S': oFormat = SHORT; break;
    case 'F': oFormat = FULL; break;
    case 'J': oFormat = JSON; break;
    case 'P': oFormat = PROTOBUF; break;
    default: oFormat = LAST_VALUE; break;
    }

    if (oFormat == LAST_VALUE) {
      const std::string& lLabels = describeLabels();
      std::ostringstream oMessage;
      oMessage << "The output format '" << iFormatChar
               << "' is not known. Known output formats: " << lLabels;
      throw CodeConversionException (oMessage.str());
    }

    return oFormat;
  }
  
  // //////////////////////////////////////////////////////////////////////
  OutputFormat::OutputFormat (const char iFormatChar)
    : _format (getFormat (iFormatChar)) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  OutputFormat::
  OutputFormat (const std::string& iFormatStr) {
    // 
#ifndef NDEBUG
    const size_t lSize = iFormatStr.size();
    assert (lSize == 1);
#endif
    const char lFormatChar = iFormatStr[0];
    _format = getFormat (lFormatChar);
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string& OutputFormat::
  getLabel (const EN_OutputFormat& iFormat) {
    return _labels[iFormat];
  }
  
  // //////////////////////////////////////////////////////////////////////
  char OutputFormat::
  getFormatLabel (const EN_OutputFormat& iFormat) {
    return _formatLabels[iFormat];
  }

  // //////////////////////////////////////////////////////////////////////
  std::string OutputFormat::
  getFormatLabelAsString (const EN_OutputFormat& iFormat) {
    std::ostringstream oStr;
    oStr << _formatLabels[iFormat];
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string OutputFormat::describeLabels() {
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
  OutputFormat::EN_OutputFormat
  OutputFormat::getFormat() const {
    return _format;
  }
  
  // //////////////////////////////////////////////////////////////////////
  char OutputFormat::getFormatAsChar() const {
    const char oFormatChar = _formatLabels[_format];
    return oFormatChar;
  }

  // //////////////////////////////////////////////////////////////////////
  std::string OutputFormat::getFormatAsString() const {
    std::ostringstream oStr;
    oStr << _formatLabels[_format];
    return oStr.str();
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string OutputFormat::describe() const {
    std::ostringstream ostr;
    ostr << _labels[_format];
    return ostr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  bool OutputFormat::
  operator== (const EN_OutputFormat& iFormat) const {
    return (_format == iFormat);
  }
  
}
