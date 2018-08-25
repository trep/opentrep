// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// Xapian
#include <xapian.h>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/service/Logger.hpp>
#include <opentrep/factory/FacSupervisor.hpp>
#include <opentrep/factory/FacXapianDB.hpp>

namespace OPENTREP {

  FacXapianDB* FacXapianDB::_instance = NULL;

  // //////////////////////////////////////////////////////////////////////
  FacXapianDB::~FacXapianDB() {
    clean ();
    _instance = NULL;
  }

  // //////////////////////////////////////////////////////////////////////
  void FacXapianDB::clean() {
    for (XapianDBPool_T::iterator itXapianDB = _pool.begin();
         itXapianDB != _pool.end(); itXapianDB++) {
      Xapian::WritableDatabase* currentXapianDB_ptr = *itXapianDB;
      assert (currentXapianDB_ptr != NULL);
      
      delete currentXapianDB_ptr; currentXapianDB_ptr = NULL;
    }
    
    // Now that all the objects have been deleted, empty the underlying pool
    _pool.clear();
  }

  // //////////////////////////////////////////////////////////////////////
  FacXapianDB& FacXapianDB::instance() {
    if (_instance == NULL) {
      _instance = new FacXapianDB();
      FacSupervisor::instance().registerXapianDBFactory (_instance);
    }
    assert (_instance != NULL);
    return *_instance;
  }

  // //////////////////////////////////////////////////////////////////////
  Xapian::WritableDatabase* FacXapianDB::
  create (const TravelDBFilePath_T& iTravelIndexFilePath,
          const int& iXapianActionFlag) {
    Xapian::WritableDatabase* oXapianDatabase_ptr = NULL;

    oXapianDatabase_ptr = new Xapian::WritableDatabase (iTravelIndexFilePath,
                                                        iXapianActionFlag);
    if (oXapianDatabase_ptr == NULL) {
      std::ostringstream errorStr;
      errorStr << "Error when trying to create the Xapian database/index ('"
               << iTravelIndexFilePath << "')";
      OPENTREP_LOG_ERROR (errorStr.str());
      throw XapianDatabaseFailureException (errorStr.str());
    }
      

    // The new object is added to the Service pool
    _pool.push_back (oXapianDatabase_ptr);

    return oXapianDatabase_ptr;
  }

}
