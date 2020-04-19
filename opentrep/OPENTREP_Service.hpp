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
     * Get the file-paths of the Xapian database/index and of the OPTD-maintained
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

    /**
     * Get the deployment number.
     *
     * @return DeploymentNumber_T& The current deployment number
     */
    const DeploymentNumber_T& getDeploymentNumber() const;

    /**
     * Check that the directory hosting the Xapian database/index exists
     * and is accessible.
     *
     * @return bool Whether the directory of the Xapian database/index exists
     *              and is accessible
     */
    bool checkXapianDBOnFileSystem (const TravelDBFilePath_T&) const;

  public:
    // ////////// Interaction with the SQL database //////////
    /**
     * Create the SQL database user ('trep' on MySQL) and database ('trep_trep'
     * on MySQL). That method has no effect on other database types (e.g.,
     * nosql, sqlite).
     *
     * @return bool Whether or not the creation of the SQL database user
     *              and DB was successful.
     */
    bool createSQLDBUser();

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
     * Create the SQL database tables and leave them empty.
     *
     * The SQL database may be one of SQLite3, MySQL, PostgreSQL. That latter
     * is not yet supported (patches are welcome on
     * http://github.com/trep/opentrep/issues).
     */
    void createSQLDBTables();

    /**
     * Create the SQL database indexes.
     */
    void createSQLDBIndexes();

    /**
     * Toggle the deployment number/version.
     *
     * @return OPENTREP::DeploymentNumber_T New value of the deployment number
     */
    OPENTREP::DeploymentNumber_T toggleDeploymentNumber();
    
    /**
     * Toggle the flag stating whether to index non-IATA-referenced POR
     *
     * @return OPENTREP::shouldIndexNonIATAPOR_T New value of the flag
     */
    OPENTREP::shouldIndexNonIATAPOR_T toggleShouldIncludeAllPORFlag();

    /**
     * Toggle the flag stating whether to add the POR in the Xapian index
     *
     * @return OPENTREP::shouldIndexPORInXapian_T New value of the flag
     */
    OPENTREP::shouldIndexPORInXapian_T toggleShouldIndexPORInXapianFlag();

    /**
     * Toggle the flag stating whether to add the POR in the SQL database
     *
     * @return OPENTREP::shouldAddPORInSQLDB_T New value of the flag
     */
    OPENTREP::shouldAddPORInSQLDB_T toggleShouldAddPORInSQLDBFlag();

    /**
     * From the file of OPTD-maintained POR (points of reference):
     * <ul>
     *   <li>Add the relevant POR into the SQL database,
     *       if the corresponding flag is set</li>
     *   <li>Add the relevant POR into the Xapian index,
     *       if the corresponding flag is set</li>
     * </ul>
     * The relevant POR are by default only the IATA-referenced ones.
     * If the corresponding flag is set, then all the POR are relevant.
     * All the above-mentioned flags are specified in the
     * underlying OPENTREP_ServiceContext object.
     *
     * @return NbOfDBEntries_T Number of documents of the file (stream).
     */
    NbOfDBEntries_T insertIntoDBAndXapian();    

    /**
     * Retrieve the number of POR (points of reference)
     * within the SQL database.
     *
     * @return NbOfDBEntries_T Number of POR in the SQL database. 
     */
    NbOfDBEntries_T getNbOfPORFromDB();

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
     * to the given UN/LOCODE code.
     *
     * @param const UNLOCode_T& The given UN/LOCODE code (key).
     * @param LocationList_T& List of (geographical) locations, if any,
     *                        matching the given key.
     * @return NbOfMatches_T Number of matches. 
     */
    NbOfMatches_T listByUNLOCode (const UNLOCode_T&, LocationList_T&);

    /**
     * List all the POR (points of reference) corresponding
     * to the given UIC code.
     *
     * @param const UICCode_T& The given UIC code (key).
     * @param LocationList_T& List of (geographical) locations, if any,
     *                        matching the given key.
     * @return NbOfMatches_T Number of matches. 
     */
    NbOfMatches_T listByUICCode (const UICCode_T&, LocationList_T&);

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
     *  Constructor for search-/query-related services.
     *
     * @param std::ostream& Output log stream (for instance, std::cout).
     * @param const TravelDBFilePath_T& File-path of the Xapian index/database.
     * @param const DBType& SQL database type (can be no database at all).
     * @param const SQLDBConnectionString_T& SQL DB connection string.
     * @param const DeploymentNumber_T& Deployment number/version.
     */
    OPENTREP_Service (std::ostream& ioLogStream, const TravelDBFilePath_T&,
                      const DBType&, const SQLDBConnectionString_T&,
                      const DeploymentNumber_T&);

    /**
     *  Constructor for indexing-related services.
     *
     * @param std::ostream& Output log stream (for instance, std::cout).
     * @param const PORFilePath_T& File-path of the file of POR
     *                             (points of reference).
     * @param const TravelDBFilePath_T& File-path of the Xapian index/database. 
     * @param const DBType& SQL database type (can be no database at all).
     * @param const SQLDBConnectionString_T& SQL DB connection string.
     * @param const DeploymentNumber_T& Deployment number/version.
     * @param const shouldIndexNonIATAPOR_T& Whether to include non-IATA POR.
     * @param const shouldIndexPORInXapian_T& Whether to index POR in Xapian.
     * @param const shouldAddPORInSQLDB_T& Whether to add POR in SQL database.
     */
    OPENTREP_Service (std::ostream& ioLogStream, const PORFilePath_T&,
                      const TravelDBFilePath_T&, const DBType&,
                      const SQLDBConnectionString_T&, const DeploymentNumber_T&,
                      const shouldIndexNonIATAPOR_T&,
                      const shouldIndexPORInXapian_T&,
                      const shouldAddPORInSQLDB_T&);

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
     * Initialise for query-/search-related services.
     *
     * @param std::ostream& Output log stream (for instance, std::cout).
     * @param const TravelDBFilePath_T& File-path of the Xapian index/database. 
     * @param const DBType& SQL database type (can be no database at all).
     * @param const SQLDBConnectionString_T& SQL DB connection string.
     * @param const DeploymentNumber_T& Deployment number/version.
     */
    void init (std::ostream& ioLogStream, const TravelDBFilePath_T&,
               const DBType&, const SQLDBConnectionString_T&,
               const DeploymentNumber_T&);

    /**
     * Initialise for indexing-related services.
     *
     * @param std::ostream& Output log stream (for instance, std::cout).
     * @param const PORFilePath_T& File-path of the file of POR
     *                             (points of reference).
     * @param const TravelDBFilePath_T& File-path of the Xapian index/database. 
     * @param const DBType& SQL database type (can be no database at all).
     * @param const SQLDBConnectionString_T& SQL DB connection string.
     * @param const DeploymentNumber_T& Deployment number/version.
     * @param const shouldIndexNonIATAPOR_T& Whether to include non-IATA POR.
     * @param const shouldIndexPORInXapian_T& Whether to index POR in Xapian.
     * @param const shouldAddPORInSQLDB_T& Whether to add POR in SQL database.
     */
    void init (std::ostream& ioLogStream, const PORFilePath_T&,
               const TravelDBFilePath_T&,
               const DBType&, const SQLDBConnectionString_T&,
               const DeploymentNumber_T&, const shouldIndexNonIATAPOR_T&,
               const shouldIndexPORInXapian_T&, const shouldAddPORInSQLDB_T&);

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
