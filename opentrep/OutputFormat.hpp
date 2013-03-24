#ifndef __OPENTREP_OUTPUTFORMAT_HPP
#define __OPENTREP_OUTPUTFORMAT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>

namespace OPENTREP {

  /**
   * @brief Enumeration of output formats.
   */
  struct OutputFormat {
  public:
    typedef enum {
      SHORT = 0,
      FULL,
      JSON,
      PROTOBUF,
      LAST_VALUE
    } EN_OutputFormat;

    /**
     * Get the label as a string (e.g., "Short", "Full", "JSON" or "PROTOBUF").
     */
    static const std::string& getLabel (const EN_OutputFormat&);

    /**
     * Get the format value from parsing a single char (e.g., 'S', 'F', 'J'
     * or 'P').
     */
    static EN_OutputFormat getFormat (const char);

    /**
     * Get the label as a single char (e.g., 'S', 'F', 'J' or 'P').
     */
    static char getFormatLabel (const EN_OutputFormat&);

    /**
     * Get the label as a string of a single char (e.g., "S", "F", "J" or "P").
     */
    static std::string getFormatLabelAsString (const EN_OutputFormat&);

    /**
     * List the labels.
     */
    static std::string describeLabels();

    /**
     * Get the enumerated value.
     */
    EN_OutputFormat getFormat() const;

    /**
     * Get the enumerated value as a short string (e.g., 'S', 'F', 'J' or 'P').
     */
    char getFormatAsChar() const;
    
    /**
     * Get the enumerated value as a short string (e.g., "S", "F", "J" or "P").
     */
    std::string getFormatAsString() const;
    
    /**
     * Give a description of the structure (e.g., "Short", "Full", "JSON"
     * or "PROTOBUF").
     */
    const std::string describe() const;

  public:
    /**
     * Comparison operator.
     */
    bool operator== (const EN_OutputFormat&) const;
    
  public:
    /**
     * Main constructor.
     */
    OutputFormat (const EN_OutputFormat&);
    /**
     * Alternative constructor.
     */
    OutputFormat (const char iFormat);
    /**
     * Alternative constructor.
     */
    OutputFormat (const std::string& iFormat);
    /**
     * Default copy constructor.
     */
    OutputFormat (const OutputFormat&);

  private:
    /**
     * Default constructor.
     */
    OutputFormat();


  private:
    /**
     * String version of the enumeration.
     */
    static const std::string _labels[LAST_VALUE];
    /**
     * Format version of the enumeration.
     */
    static const char _formatLabels[LAST_VALUE];

  private:
    // //////// Attributes /////////
    /**
     * Output format.
     */
    EN_OutputFormat _format;
  };

}
#endif // __OPENTREP_OUTPUTFORMAT_HPP
