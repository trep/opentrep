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
#include <opentrep/DBType.hpp>
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
     * Get the number of documents within the Xapian database (index).
     *
     * @return NbOfDBEntries_T Number of documents within the Xapian database. 
     */
    NbOfDBEntries_T getIndexSize();

    /** 
     * Return a given number of locations, all randomly draw from the Xapian
     * database (index).
     *
     * @param LocationList_T& List of (geographical) locations randomly
     *        picked up from the Xapian database.
     * @return NbOfMatches_T Number of documents actually retrieved from
     *         the Xapian database/index.
     */
    NbOfMatches_T drawRandomLocations (const NbOfMatches_T& iNbOfDraws,
                                       LocationList_T&);

    /**
     * Build the Xapian database (index) from the file with the ORI-maintained
     * list of POR (points of reference).
     *
     * @return NbOfDBEntries_T Number of documents indexed by the Xapian
     *         database/index.
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


    /**
     * Get the file-paths of the Xapian database/index and of the ORI-maintained
     * POR (points of reference).
     *
     * @return std::pair<const PORFilePath_T,  File-path of the file of POR
     *                                         (points of reference).
     *                   std::pair<const TravelDBFilePath_T, File-path of the
     *                                                       Xapian database.
     *                             const SQLDBConnectionString_T> > Connection
     *                                                      string of the SQL DB.
     */
    typedef std::pair<const TravelDBFilePath_T,
                      const SQLDBConnectionString_T> DBFilePathPair_T;
    typedef std::pair<const PORFilePath_T, const DBFilePathPair_T> FilePathSet_T;
    FilePathSet_T getFilePaths() const;

  public:
    // ////////// Interaction with the SQL database //////////
    /**
     * Create the SQL database user ('trep' on MySQL) and database ('trep_trep'
     * on MySQL). That method has no effect on other database types (e.g.,
     * nosql, sqlite).
     */
    void createSQLDBUser();

    /**
     * Update the connection string.
     *
     * <ul>
     *  <li>Standard <tt>trep</tt> user:
     *      <tt>db=trep_trep user=trep password=trep</tt></li>
     *  <li>Administrator user:
     *      <tt>db=mysql user=root password=admin_user_passwd</tt></li>
     * </ul>
     *
     * @param const SQLDBConnectionString_T& Connection string.
     */
    void setSQLDBConnectString (const SQLDBConnectionString_T&);

    /**
     * Build the SQL database from the file with the ORI-maintained list
     * of POR (points of reference).
     *
     * The SQL database may be one of SQLite3, MySQL, PostgreSQL. That latter
     * is not yet supported (patches are welcome on
     * http://github.com/trep/opentrep/issues).
     */
    void buildSQLDB();

    /**
     * List all the POR (points of reference) corresponding
     * to the given IATA code.
     *
     * @param const IATACode_T& The given IATA code (key).
     * @param LocationList_T& List of (geographical) locations, if any,
     *                        matching the given key.
     * @return NbOfMatches_T Number of matches. 
     */
    NbOfMatches_T listByIataCode (const IATACode_T&, LocationList_T&);

    /**
     * List all the POR (points of reference) corresponding
     * to the given ICAO code.
     *
     * @param const ICAOCode_T& The given ICAO code (key).
     * @param LocationList_T& List of (geographical) locations, if any,
     *                        matching the given key.
     * @return NbOfMatches_T Number of matches. 
     */
    NbOfMatches_T listByIcaoCode (const ICAOCode_T&, LocationList_T&);

    /**
     * List all the POR (points of reference) corresponding
     * to the given FAA code.
     *
     * @param const FAACode_T& The given FAA code (key).
     * @param LocationList_T& List of (geographical) locations, if any,
     *                        matching the given key.
     * @return NbOfMatches_T Number of matches. 
     */
    NbOfMatches_T listByFaaCode (const FAACode_T&, LocationList_T&);

    /**
     * List all the POR (points of reference) corresponding
     * to the given Geoname ID.
     *
     * @param const GeonamesID_T& The given Geoname ID (key).
     * @param LocationList_T& List of (geographical) locations, if any,
     *                        matching the given key.
     * @return NbOfMatches_T Number of matches. 
     */
    NbOfMatches_T listByGeonameID (const GeonamesID_T&, LocationList_T&);


  public:
    // ////////// Constructors and destructors //////////
    /**
     *  Constructor.
     *
     * @param std::ostream& Output log stream (for instance, std::cout).
     * @param const TravelDBFilePath_T& File-path of the Xapian index/database.
     * @param const DBType& SQL database type (can be no database at all).
     * @param const SQLDBConnectionString_T& SQL DB connection string.
     */
    OPENTREP_Service (std::ostream& ioLogStream, const TravelDBFilePath_T&,
                      const DBType&, const SQLDBConnectionString_T&);

    /**
     *  Constructor.
     *
     * @param std::ostream& Output log stream (for instance, std::cout).
     * @param const PORFilePath_T& File-path of the file of POR
     *                             (points of reference).
     * @param const TravelDBFilePath_T& File-path of the Xapian index/database. 
     * @param const DBType& SQL database type (can be no database at all).
     * @param const SQLDBConnectionString_T& SQL DB connection string.
     */
    OPENTREP_Service (std::ostream& ioLogStream, const PORFilePath_T&,
                      const TravelDBFilePath_T&,
                      const DBType&, const SQLDBConnectionString_T&);

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
     * @param const TravelDBFilePath_T& File-path of the Xapian index/database. 
     * @param const DBType& SQL database type (can be no database at all).
     * @param const SQLDBConnectionString_T& SQL DB connection string.
     */
    void init (std::ostream& ioLogStream, const TravelDBFilePath_T&,
               const DBType&, const SQLDBConnectionString_T&);

    /**
     * Initialise.
     *
     * @param std::ostream& Output log stream (for instance, std::cout).
     * @param const PORFilePath_T& File-path of the file of POR
     *                             (points of reference).
     * @param const TravelDBFilePath_T& File-path of the Xapian index/database. 
     * @param const DBType& SQL database type (can be no database at all).
     * @param const SQLDBConnectionString_T& SQL DB connection string.
     */
    void init (std::ostream& ioLogStream, const PORFilePath_T&,
               const TravelDBFilePath_T&,
               const DBType&, const SQLDBConnectionString_T&);

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
