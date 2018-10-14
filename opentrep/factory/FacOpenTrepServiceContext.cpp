// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/factory/FacSupervisor.hpp>
#include <opentrep/factory/FacOpenTrepServiceContext.hpp>
#include <opentrep/service/OPENTREP_ServiceContext.hpp>

namespace OPENTREP {

  FacOpenTrepServiceContext* FacOpenTrepServiceContext::_instance = NULL;

  // //////////////////////////////////////////////////////////////////////
  FacOpenTrepServiceContext::~FacOpenTrepServiceContext() {
    _instance = NULL;
  }

  // //////////////////////////////////////////////////////////////////////
  FacOpenTrepServiceContext& FacOpenTrepServiceContext::instance() {

    if (_instance == NULL) {
      _instance = new FacOpenTrepServiceContext();
      assert (_instance != NULL);
      
      FacSupervisor::instance().registerServiceFactory (_instance);
    }
    return *_instance;
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_ServiceContext& FacOpenTrepServiceContext::
  create (const TravelDBFilePath_T& iTravelDBFilePath, const DBType& iSQLDBType,
          const SQLDBConnectionString_T& iSQLDBConnStr,
          const DeploymentNumber_T& iDeploymentNumber) {
    OPENTREP_ServiceContext* aOPENTREP_ServiceContext_ptr = NULL;

    aOPENTREP_ServiceContext_ptr =
      new OPENTREP_ServiceContext (iTravelDBFilePath, iSQLDBType, iSQLDBConnStr,
                                   iDeploymentNumber);
    assert (aOPENTREP_ServiceContext_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aOPENTREP_ServiceContext_ptr);

    return *aOPENTREP_ServiceContext_ptr;
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_ServiceContext& FacOpenTrepServiceContext::
  create (const PORFilePath_T& iPORFilePath,
          const TravelDBFilePath_T& iTravelDBFilePath, const DBType& iSQLDBType,
          const SQLDBConnectionString_T& iSQLDBConnStr,
          const DeploymentNumber_T& iDeploymentNumber,
          const shouldIndexNonIATAPOR_T& iShouldIndexNonIATAPOR,
          const shouldIndexPORInXapian_T& iShouldIndexPORInXapian,
          const shouldAddPORInSQLDB_T& iShouldAddPORInSQLDB) {
    OPENTREP_ServiceContext* aOPENTREP_ServiceContext_ptr = NULL;

    aOPENTREP_ServiceContext_ptr =
      new OPENTREP_ServiceContext (iPORFilePath, iTravelDBFilePath, iSQLDBType,
                                   iSQLDBConnStr, iDeploymentNumber,
                                   iShouldIndexNonIATAPOR,
                                   iShouldIndexPORInXapian,
                                   iShouldAddPORInSQLDB);
    assert (aOPENTREP_ServiceContext_ptr != NULL);

    // The new object is added to the Service pool
    _pool.push_back (aOPENTREP_ServiceContext_ptr);

    return *aOPENTREP_ServiceContext_ptr;
  }

}
