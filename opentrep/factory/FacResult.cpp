// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// C
#include <cassert>
// OPENTREP
#include <opentrep/bom/Result.hpp>
#include <opentrep/factory/FacSupervisor.hpp>
#include <opentrep/factory/FacResult.hpp>

namespace OPENTREP {

  FacResult* FacResult::_instance = NULL;

  // //////////////////////////////////////////////////////////////////////
  FacResult::FacResult () {
  }

  // //////////////////////////////////////////////////////////////////////
  FacResult::FacResult (const FacResult&) {
  }

  // //////////////////////////////////////////////////////////////////////
  FacResult::~FacResult () {
    _instance = NULL;
  }

  // //////////////////////////////////////////////////////////////////////
  FacResult& FacResult::instance () {

    if (_instance == NULL) {
      _instance = new FacResult();
      assert (_instance != NULL);
      
      FacSupervisor::instance().registerBomFactory (_instance);
    }
    return *_instance;
  }

  // //////////////////////////////////////////////////////////////////////
  Result& FacResult::create (const Xapian::Database& iXapianDatabase) {
    Result* oResult_ptr = NULL;

    oResult_ptr = new Result (iXapianDatabase);
    assert (oResult_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (oResult_ptr);

    return *oResult_ptr;
  }

}
