#ifndef __OPENTREP_CMD_XAPIANINDEXMANAGER_HPP
#define __OPENTREP_CMD_XAPIANINDEXMANAGER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/LocationList.hpp>

namespace OPENTREP {

  /**
   * @brief Command wrapping utilities for the management
   *        of the Xapian (database) index.
   */
  class XapianIndexManager {
    friend class OPENTREP_Service;
  private:
    /**
     * Give the number of documents indexed by the Xapian index
     * (named "database").
     *
     * @param const TravelDBFilePath_T& Filepath to the Xapian database.
     * @return NbOfDBEntries_T Number of entries in the database.
     */
    static NbOfDBEntries_T getSize (const TravelDBFilePath_T&);

    /**
     * Randomly draw a given number of documents from the Xapian index
     * (named "database").
     *
     * @param const TravelDBFilePath_T& Filepath to the Xapian database.
     * @param LocationList_T& List of Location structures randomly picked-up.
     * @return const NbOfMatches_T& Number of locations to randomly pick-up.
     */
    static NbOfMatches_T drawRandomLocations (const TravelDBFilePath_T&,
                                              const NbOfMatches_T& iNbOfDraws,
                                              LocationList_T&);

  private:
    /**
     * Constructors.
     */
    XapianIndexManager() {}
    XapianIndexManager (const XapianIndexManager&) {}
    /**
     * Destructor.
     */
    ~XapianIndexManager() {}
  };

}
#endif // __OPENTREP_CMD_XAPIANINDEXMANAGER_HPP
