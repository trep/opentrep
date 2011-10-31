// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// C
#include <cassert>
// OPENTREP
#include <opentrep/bom/ResultHolder.hpp>
#include <opentrep/bom/Result.hpp>
#include <opentrep/factory/FacSupervisor.hpp>
#include <opentrep/factory/FacResultHolder.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  FacResultHolder* FacResultHolder::_instance = NULL;

  // //////////////////////////////////////////////////////////////////////
  FacResultHolder::FacResultHolder () {
  }

  // //////////////////////////////////////////////////////////////////////
  FacResultHolder::FacResultHolder (const FacResultHolder&) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  FacResultHolder::~FacResultHolder () {
    _instance = NULL;
  }

  // //////////////////////////////////////////////////////////////////////
  FacResultHolder& FacResultHolder::instance () {

    if (_instance == NULL) {
      _instance = new FacResultHolder();
      assert (_instance != NULL);
      
      FacSupervisor::instance().registerBomFactory (_instance);
    }
    return *_instance;
  }

  // //////////////////////////////////////////////////////////////////////
  ResultHolder& FacResultHolder::create (const TravelQuery_T& iQueryString,
                                         const Xapian::Database& iDatabase) {
    ResultHolder* oResultHolder_ptr = NULL;

    oResultHolder_ptr = new ResultHolder (iQueryString, iDatabase);
    assert (oResultHolder_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (oResultHolder_ptr);

    return *oResultHolder_ptr;
  }

  // //////////////////////////////////////////////////////////////////////
  void FacResultHolder::initLinkWithResult (ResultHolder& ioResultHolder,
                                          Result& ioResult) {
      // Link the ResultHolder to the Result, and vice versa
      ioResult._resultHolder = &ioResultHolder;
      
      // Add the Result to the ResultHolder internal list (of Result objects)
      ioResultHolder._resultList.push_back (&ioResult);
    }

}
