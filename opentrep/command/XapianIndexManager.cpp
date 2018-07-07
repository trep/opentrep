// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
#include <string>
#include <vector>
#include <exception>
// Boost
#include <boost/filesystem.hpp>
#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>
// Xapian
#include <xapian.h>
// OpenTrep
#include <opentrep/Location.hpp>
#include <opentrep/CityDetails.hpp>
#include <opentrep/bom/Result.hpp>
#include <opentrep/command/XapianIndexManager.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  boost::filesystem::path
  checkTravelDBFilePath (const TravelDBFilePath_T& iTravelDBFilePath) {
    boost::filesystem::path oTravelDBFilePath (iTravelDBFilePath.begin(),
                                               iTravelDBFilePath.end());
    if (!(boost::filesystem::exists (oTravelDBFilePath)
          && boost::filesystem::is_directory (oTravelDBFilePath))) {
      std::ostringstream oStr;
      oStr << "The file-path to the Xapian database/index ('"
           << iTravelDBFilePath << "') does not exist or is not a directory.";
      OPENTREP_LOG_ERROR (oStr.str());
      throw FileNotFoundException (oStr.str());
    }

    return oTravelDBFilePath;
  }

  // //////////////////////////////////////////////////////////////////////
  NbOfDBEntries_T XapianIndexManager::
  getSize (const TravelDBFilePath_T& iTravelDBFilePath) {
    NbOfDBEntries_T oNbOfDBEntries = 0;

    // Check whether the file-path to the Xapian database/index exists
    // and is a directory.
    checkTravelDBFilePath (iTravelDBFilePath);

    // Open the Xapian database
    Xapian::Database lXapianDatabase (iTravelDBFilePath);

    // Retrieve the actual number of documents indexed by the Xapian database
    const Xapian::doccount& lDocCount = lXapianDatabase.get_doccount();

    //
    oNbOfDBEntries = static_cast<const NbOfDBEntries_T> (lDocCount);

    return oNbOfDBEntries;
  }
  
  // //////////////////////////////////////////////////////////////////////
  NbOfMatches_T XapianIndexManager::
  drawRandomLocations (const TravelDBFilePath_T& iTravelDBFilePath,
                       const NbOfMatches_T& iNbOfDraws,
                       LocationList_T& ioLocationList) {
    NbOfMatches_T oNbOfMatches = 0;

    // Check whether the file-path to the Xapian database/index exists
    // and is a directory.
    checkTravelDBFilePath (iTravelDBFilePath);

    // Open the Xapian database
    Xapian::Database lXapianDatabase (iTravelDBFilePath);

    // Retrieve the number of documents indexed by the database
    const NbOfDBEntries_T& lTotalNbOfDocs = getSize (iTravelDBFilePath);

    // No need to go further when the Xapian database (index) is empty
    if (lTotalNbOfDocs == 0) {
      //
      OPENTREP_LOG_NOTIFICATION ("The Xapian database is empty");
      return oNbOfMatches;
    }

    // random_device is used as a source of entropy, since the generated
    // locations are expected not to be reproducible.
    boost::random::random_device lRandomDevice;
    boost::random::uniform_int_distribution<> uniformDistrib (1, lTotalNbOfDocs);
    
    // Randomly generate document IDs. If the corresponding documents
    // do not exist in the Xapian database, generate another one.
    for (NbOfMatches_T idx = 1; idx <= iNbOfDraws; ++idx) {
      unsigned int lRandomNbInt = uniformDistrib (lRandomDevice);
      Xapian::docid lDocID = static_cast<Xapian::docid> (lRandomNbInt);

      // Retrieve the document from the Xapian database/index
      Xapian::termcount lDocLength = lXapianDatabase.get_doclength (lDocID);

      unsigned short currentNbOfIterations = 0;
      while (lDocLength == 0 && currentNbOfIterations <= 100) {
        // DEBUG
        OPENTREP_LOG_DEBUG ("[" << idx << "] The " << lDocID
                            << " document ID does not exist in the Xapian "
                            << "database. Another ID will be generated.");

        // Re-draw another random document ID
        lRandomNbInt = uniformDistrib (lRandomDevice);
        lDocID = static_cast<Xapian::docid> (lRandomNbInt);

        // Retrieve the document from the Xapian database/index
        lDocLength = lXapianDatabase.get_doclength (lDocID);
      }

      // Bad luck: no document ID can be generated so that it corresponds to
      // an actual document in the Xapian database/index
      if (lDocLength == 0) {
        //
        OPENTREP_LOG_NOTIFICATION ("[" << idx << "] No document ID can be "
                                   << "generated so that it corresponds to "
                                   << "a document in the Xapian database.");

      } else {
        // Retrieve the actual document.
	const Xapian::Document lDoc = lXapianDatabase.get_document (lDocID);
        const std::string& lDocDataStr = lDoc.get_data();
        const RawDataString_T& lDocData = RawDataString_T (lDocDataStr);

        // Parse the POR details and create the corresponding Location structure
        const Location& lLocation = Result::retrieveLocation (lDocData);

        // Add the Location structure to the dedicated list
        ioLocationList.push_back (lLocation);
      }
    }

    // Consistency check
    oNbOfMatches = ioLocationList.size();
    if (oNbOfMatches != iNbOfDraws) {
        //
        OPENTREP_LOG_NOTIFICATION (iNbOfDraws << " random draws were expected, "
                                   << "but " << oNbOfMatches
                                   << " have been generated.");
    }

    //
    return oNbOfMatches;
  }

}
