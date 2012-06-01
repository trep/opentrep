#ifndef __OPENTREP_SVC_OPENTREP_SERVICE_HPP
#define __OPENTREP_SVC_OPENTREP_SERVICE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
#include <string>
// OpenTREP
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/DBParams.hpp>
#include <opentrep/LocationList.hpp>
#include <opentrep/DistanceErrorRule.hpp>

namespace OPENTREP {

  // Forward declaration
  class OPENTREP_ServiceContext;


  /** 
   * @brief Interface for the OPENTREP Services.
   */
  class OPENTREP_Service {
  public:
    // /////////// Business Methods /////////////
    /** 
     * Build the Xapian database (index) on the BOM held in memory. 
     */
    NbOfDBEntries_T buildSearchIndex();

    /**
     * Match the given string, thanks to a full-text search on the
     * underlying Xapian index (named "database").
     *
     * @param const std::string& (Travel-related) query string (e.g.,
     *        "sna francicso rio de janero lso angles reykyavki nce iev mow").
     * @param LocationList_T& List of (geographical) locations, if any,
     *        matching the given query string.
     * @param WordList_T& List of non-matched words of the query string.
     * @return NbOfMatches_T Number of matches. 
     */
    NbOfMatches_T interpretTravelRequest (const std::string& iTravelQuery,
                                          LocationList_T&, WordList_T&);


    // ////////// Constructors and destructors //////////
    /**
     *  Constructor.
     *
     * @param std::ostream& Output log stream (for instance, std::cout).
     * @param const TravelDatabaseName_T& File-path of the Xapian
     *                                    index/database. 
     */
    OPENTREP_Service (std::ostream& ioLogStream, const TravelDatabaseName_T&);

    /**
     *  Constructor.
     *
     * @param std::ostream& Output log stream (for instance, std::cout).
     * @param const DBParams& The SQL database parameters.
     * @param const TravelDatabaseName_T& File-path of the Xapian
     *                                    index/database. 
     */
    OPENTREP_Service (std::ostream& ioLogStream, const DBParams&,
                      const TravelDatabaseName_T&);

    /** 
     * Destructor. 
     */
    ~OPENTREP_Service();


  private:
    // /////// Construction and Destruction helper methods ///////
    /**
     * Default constructor. 
     */
    OPENTREP_Service();
    /**
     * Default copy constructor. 
     */
    OPENTREP_Service (const OPENTREP_Service&);

    /**
     * Initialise.
     *
     * @param std::ostream& Output log stream (for instance, std::cout).
     * @param const TravelDatabaseName_T& File-path of the Xapian
     *                                    index/database. 
     */
    void init (std::ostream& ioLogStream, const TravelDatabaseName_T&);

    /**
     * Initialise.
     *
     * @param std::ostream& Output log stream (for instance, std::cout).
     * @param const DBParams& The SQL database parameters.
     * @param const TravelDatabaseName_T& File-path of the Xapian
     *                                    index/database. 
     */
    void init (std::ostream& ioLogStream, const DBParams&,
               const TravelDatabaseName_T&);

    /**
     *  Finalise. 
     */
    void finalise();


  private:
    // ///////// Service Context /////////
    /**
     * Opentrep context. 
     */
    OPENTREP_ServiceContext* _opentrepServiceContext;
  };
}
#endif // __OPENTREP_SVC_OPENTREP_SERVICE_HPP
