#ifndef __OPENTREP_CMD_PORPARSER_HPP
#define __OPENTREP_CMD_PORPARSER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// OpenTREP
#include <opentrep/OPENTREP_Types.hpp>

namespace OPENTREP {
  
  /**
   * Class wrapping the parser entry point.
   */
  class PORParser {
  public:
    /**
     * Parses the CSV file describing the POR (points of reference), and generates
     * the POR BOM tree accordingly.
     *
     * @param const PORFilePath_T& The file-name of the CSV-formatted POR input file.
     */
    static void PORGeneration (const PORFilePath_T&);
  };
}
#endif // __OPENTREP_CMD_PORPARSER_HPP
