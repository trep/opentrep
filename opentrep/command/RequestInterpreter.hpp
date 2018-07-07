#ifndef __OPENTREP_CMD_REQUESTINTERPRETER_HPP
#define __OPENTREP_CMD_REQUESTINTERPRETER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/LocationList.hpp>

namespace OPENTREP {

  // Forward declarations
  class OTransliterator;

  /**
   * @brief Command wrapping the travel request process.
   */
  class RequestInterpreter {
    friend class OPENTREP_Service;
  private:
    /**
     * Check whether all the words/items of the query string are either
     * IATA/ICAO codes or Geonames IDs. If all the words are codes/IDs,
     * the that method returns true. If any word/item is not a code or
     * an ID, that method returns false.
     * The list of words is simply the itemized version of the query string,
     * even though some of the words may not be codes or IDs.
     *
     * @param WordList_T& Itemized version of the query string.
     * @return bool Whether or not all the words are codes or IDs.
     */
    static bool areAllCodeOrGeoID (const TravelQuery_T&, WordList_T&);

    /**
     * Interpret the given string, thanks to various pieces of algorithm,
     * including a full-text search on the underlying Xapian index (named
     * "database"). A list of locations/places is returned.
     *
     * @param const TravelDBFilePath_T& Filepath to the Xapian database.
     * @param const DBType& SQL database type (can be no database at all).
     * @param const SQLDBConnectionString_T& SQL DB connection string.
     * @param const std::string& (Travel-related) query string (e.g.,
     *        "sna francicso rio de janero lso angles reykyavki nce iev mow").
     * @param LocationList_T& List of (geographical) locations, if any,
     *        matching the given query string.
     * @param WordList_T& List of non-matched words of the query string.
     * @param const OTransliterator& Unicode transliterator.
     * @return NbOfMatches_T Number of matches.
     */
    static NbOfMatches_T interpretTravelRequest (const TravelDBFilePath_T&,
                                                 const DBType&,
                                                 const SQLDBConnectionString_T&,
                                                 const TravelQuery_T&,
                                                 LocationList_T&, WordList_T&,
                                                 const OTransliterator&);

  private:
    /**
     * Constructors.
     */
    RequestInterpreter() {}
    RequestInterpreter (const RequestInterpreter&) {}
    /**
     * Destructor.
     */
    ~RequestInterpreter() {}
  };

}
#endif // __OPENTREP_CMD_REQUESTINTERPRETER_HPP
