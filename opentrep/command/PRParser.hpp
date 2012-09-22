#ifndef __OPENTREP_CMD_PRPARSER_HPP
#define __OPENTREP_CMD_PRPARSER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// OpenTREP
#include <opentrep/OPENTREP_Types.hpp>

namespace OPENTREP {
  
  /**
   * Class wrapping the parser entry point.
   */
  class PRParser {
  public:
    /**
     * Parses the CSV file describing the PR (PageRank), and generates
     * the PR BOM tree accordingly.
     *
     * @param const PRFilePath_T& The file-name of the CSV-formatted PR input file.
     */
    static void PRGeneration (const PRFilePath_T&);
  };
}
#endif // __OPENTREP_CMD_PRPARSER_HPP
