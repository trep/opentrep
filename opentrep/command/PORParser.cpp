// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <string>
// OpenTREP
#include <opentrep/service/Logger.hpp>
#include <opentrep/command/PORParserHelper.hpp>
#include <opentrep/command/PORParser.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  void PORParser::PORGeneration (const FilePath_T& iPORFilename) {

    // Initialise the POR file parser.
    PORFileParser lPorFileParser (iPORFilename);

    // Parse the CSV-formatted POR input file and generate the corresponding
    // POR structures.
    lPorFileParser.generateLocations();
  }

}
