#ifndef __OPENTREP_FAC_FACOPENTREPSERVICECONTEXT_HPP
#define __OPENTREP_FAC_FACOPENTREPSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// OpenTrep
#include <opentrep/DBType.hpp>
#include <opentrep/factory/FacServiceAbstract.hpp>

namespace OPENTREP {

  // Forward declarations
  class OPENTREP_ServiceContext;


  /**
   * @brief Factory for an OPENTREP_ServiceContext object
   */
  class FacOpenTrepServiceContext : public FacServiceAbstract {
  public:

    /**
     * Provide the unique instance.
     *
     * The singleton is instantiated when first used
     *
     * @return FacOpentrepServiceContext&
     */
    static FacOpenTrepServiceContext& instance();

    /**
     * Destructor.
     *
     * The Destruction put the _instance to NULL in order to be clean
     * for the next FacOpentrepServiceContext::instance()
     */
    ~FacOpenTrepServiceContext();

    /**
     * Create a new OPENTREP_ServiceContext object for search-related services.
     *
     * This new object is added to the list of instantiated objects.
     *
     * @param const TravelDBFilePath_T& File-path of the Xapian database.
     * @param const DBType& SQL database type (can be no database at all).
     * @param const SQLDBConnectionString_T& SQL DB connection string.
     * @param const DeploymentNumber_T& Deployment number.
     * @return OPENTREP_ServiceContext& The newly created object.
     */
    OPENTREP_ServiceContext& create (const TravelDBFilePath_T&,
                                     const DBType&,
                                     const SQLDBConnectionString_T&,
                                     const DeploymentNumber_T&);

    /**
     * Create a new OPENTREP_ServiceContext object for indexing-related services.
     *
     * This new object is added to the list of instantiated objects.
     *
     * @param const PORFilePath_T& File-path of the POR file.
     * @param const TravelDBFilePath_T& File-path of the Xapian database.
     * @param const DBType& SQL database type (can be no database at all).
     * @param const SQLDBConnectionString_T& SQL DB connection string.
     * @param const DeploymentNumber_T& Deployment number.
     * @param const shouldIndexNonIATAPOR_T& Whether to include non-IATA POR.
     * @param const DeploymentNumber_T& Deployment number/version.
     * @param const shouldIndexNonIATAPOR_T& Whether to include non-IATA POR.
     * @return OPENTREP_ServiceContext& The newly created object.
     */
    OPENTREP_ServiceContext& create (const PORFilePath_T&,
                                     const TravelDBFilePath_T&,
                                     const DBType&,
                                     const SQLDBConnectionString_T&,
                                     const DeploymentNumber_T&,
                                     const shouldIndexNonIATAPOR_T&,
                                     const shouldIndexPORInXapian_T&,
                                     const shouldAddPORInSQLDB_T&);


  protected:
    /**
     * Default Constructor.
     *
     * This constructor is protected in order to ensure the singleton
     * pattern.
     */
    FacOpenTrepServiceContext() {}

  private:
    /**
     * The unique instance.
     */
    static FacOpenTrepServiceContext* _instance;
  };

}
#endif // __OPENTREP_FAC_FACOPENTREPSERVICECONTEXT_HPP
