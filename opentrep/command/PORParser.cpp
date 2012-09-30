// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <string>
// OpenTREP
#include <opentrep/basic/BasFileMgr.hpp>
#include <opentrep/bom/PORParserHelper.hpp>
#include <opentrep/command/PORParser.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  void PORParser::PORGeneration (const PORFilePath_T& iPORFilename) {

    // Check that the file path given as input corresponds to an actual file
    const bool doesExistAndIsReadable =
      BasFileMgr::doesExistAndIsReadable (iPORFilename);
    if (doesExistAndIsReadable == false) {
      OPENTREP_LOG_ERROR ("The POR input file, '" << iPORFilename
                          << "', can not be retrieved on the file-system");
      throw FileNotFoundException ("The POR input file '" + iPORFilename
                                   + "' does not exist or can not be read");
    }

    // Initialise the POR file parser.
    PORFileParser lPorFileParser (iPORFilename);

    // Parse the CSV-formatted POR input file and generate the corresponding
    // POR structures.
    lPorFileParser.generateLocations();
  }

}
