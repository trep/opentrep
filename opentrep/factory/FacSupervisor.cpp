// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// OpenTREP
#include <opentrep/factory/FacBomAbstract.hpp>
#include <opentrep/factory/FacServiceAbstract.hpp>
#include <opentrep/factory/FacXapianDB.hpp>
#include <opentrep/factory/FacSupervisor.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  FacSupervisor* FacSupervisor::_instance = NULL;

  // //////////////////////////////////////////////////////////////////////
  FacSupervisor::FacSupervisor () :
    _facXapianDB (NULL), _logger (NULL) {
  }
    
  // //////////////////////////////////////////////////////////////////////
  FacSupervisor& FacSupervisor::instance() {
    if (_instance == NULL) {
      _instance = new FacSupervisor();
    }
    assert (_instance != NULL);
    return *_instance;
  }

  // //////////////////////////////////////////////////////////////////////
  void FacSupervisor::
  registerBomFactory (FacBomAbstract* ioFacBomAbstract_ptr) {
    _bomPool.push_back (ioFacBomAbstract_ptr);
  }

  // //////////////////////////////////////////////////////////////////////
  void FacSupervisor::
  registerServiceFactory (FacServiceAbstract* ioFacServiceAbstract_ptr) {
    _svcPool.push_back (ioFacServiceAbstract_ptr);
  }

  // //////////////////////////////////////////////////////////////////////
  void FacSupervisor::registerXapianDBFactory (FacXapianDB* ioFacXapianDB_ptr) {
    _facXapianDB = ioFacXapianDB_ptr;
  }

  // //////////////////////////////////////////////////////////////////////
  void FacSupervisor::registerLoggerService (Logger* ioLogger_ptr) {
    _logger = ioLogger_ptr;
  }

  // //////////////////////////////////////////////////////////////////////
  FacSupervisor::~FacSupervisor() {
    // For the underlying instance, if existing
    cleanFactory();

    // For the instance level
    cleanBomLayer();
    cleanServiceLayer();
    cleanFacXapianDB();
    cleanLoggerService();
  }

  // //////////////////////////////////////////////////////////////////////
  void FacSupervisor::cleanBomLayer() {
    for (BomFactoryPool_T::const_iterator itFactory = _bomPool.begin();
         itFactory != _bomPool.end(); itFactory++) {
      const FacBomAbstract* currentFactory_ptr = *itFactory;
      assert (currentFactory_ptr != NULL);

      delete currentFactory_ptr; currentFactory_ptr = NULL;
    }

    // Now that all the objects have been deleted, empty the underlying pool
    _bomPool.clear();
  }

  // //////////////////////////////////////////////////////////////////////
  void FacSupervisor::cleanServiceLayer() {
    for (ServiceFactoryPool_T::const_iterator itFactory = _svcPool.begin();
         itFactory != _svcPool.end(); itFactory++) {
      const FacServiceAbstract* currentFactory_ptr = *itFactory;
      assert (currentFactory_ptr != NULL);
      
      delete currentFactory_ptr; currentFactory_ptr = NULL;
    }
    
    // Now that all the objects have been deleted, empty the underlying pool
    _svcPool.clear();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void FacSupervisor::cleanFacXapianDB() {
    delete _facXapianDB; _facXapianDB = NULL;
  }
  
  // //////////////////////////////////////////////////////////////////////
  void FacSupervisor::cleanLoggerService() {
    delete _logger; _logger = NULL;
  }
  
  // //////////////////////////////////////////////////////////////////////
  void FacSupervisor::cleanFactory () {
	if (_instance != NULL) {
		_instance->cleanBomLayer();
		_instance->cleanServiceLayer();
        _instance->cleanFacXapianDB();
		_instance->cleanLoggerService();
	}
    delete _instance; _instance = NULL;
  }

}
