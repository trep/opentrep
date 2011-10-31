// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// C
#include <assert.h>
// OPENTREP Common
#include <opentrep/factory/FacSupervisor.hpp>
#include <opentrep/factory/FacOpenTrepServiceContext.hpp>
#include <opentrep/service/OPENTREP_ServiceContext.hpp>

namespace OPENTREP {

  FacOpenTrepServiceContext* FacOpenTrepServiceContext::_instance = NULL;

  // //////////////////////////////////////////////////////////////////////
  FacOpenTrepServiceContext::~FacOpenTrepServiceContext () {
    _instance = NULL;
  }

  // //////////////////////////////////////////////////////////////////////
  FacOpenTrepServiceContext& FacOpenTrepServiceContext::instance () {

    if (_instance == NULL) {
      _instance = new FacOpenTrepServiceContext();
      assert (_instance != NULL);
      
      FacSupervisor::instance().registerServiceFactory (_instance);
    }
    return *_instance;
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_ServiceContext& FacOpenTrepServiceContext::
  create (const std::string& iTravelDatabaseName) {
    OPENTREP_ServiceContext* aOPENTREP_ServiceContext_ptr = NULL;

    aOPENTREP_ServiceContext_ptr =
      new OPENTREP_ServiceContext (iTravelDatabaseName);
    assert (aOPENTREP_ServiceContext_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aOPENTREP_ServiceContext_ptr);

    return *aOPENTREP_ServiceContext_ptr;
  }

}
