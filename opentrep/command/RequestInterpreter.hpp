#ifndef __OPENTREP_CMD_REQUESTINTERPRETER_HPP
#define __OPENTREP_CMD_REQUESTINTERPRETER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/Location.hpp>

namespace OPENTREP {

  /**
   * @brief Command wrapping the travel request process.
   */
  class RequestInterpreter {
    friend class OPENTREP_Service;
  private:
    /**
     * Match the given string, thanks to a full-text search on the
     * underlying Xapian index (named "database").
     *
     * @param const TravelDatabaseName_T& Filepath to the Xapian database.
     * @param const std::string& (Travel-related) query string (e.g.,
     *        "sna francicso rio de janero lso angles reykyavki nce iev mow").
     * @param LocationList_T& List of (geographical) locations, if any,
     *        matching the given query string.
     * @param WordList_T& List of non-matched words of the query string.
     * @return NbOfMatches_T Number of matches.
     */
    static NbOfMatches_T interpretTravelRequest (const TravelDatabaseName_T&,
                                                 const TravelQuery_T&,
                                                 LocationList_T&, WordList_T&);

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
