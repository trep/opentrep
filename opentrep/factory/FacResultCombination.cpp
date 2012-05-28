// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// C
#include <cassert>
// OpenTrep
#include <opentrep/bom/ResultCombination.hpp>
#include <opentrep/bom/ResultHolder.hpp>
#include <opentrep/factory/FacSupervisor.hpp>
#include <opentrep/factory/FacResultCombination.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  FacResultCombination* FacResultCombination::_instance = NULL;

  // //////////////////////////////////////////////////////////////////////
  FacResultCombination::FacResultCombination() {
  }

  // //////////////////////////////////////////////////////////////////////
  FacResultCombination::FacResultCombination (const FacResultCombination&) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  FacResultCombination::~FacResultCombination() {
    _instance = NULL;
  }

  // //////////////////////////////////////////////////////////////////////
  FacResultCombination& FacResultCombination::instance() {

    if (_instance == NULL) {
      _instance = new FacResultCombination();
      assert (_instance != NULL);
      
      FacSupervisor::instance().registerBomFactory (_instance);
    }
    return *_instance;
  }

  // //////////////////////////////////////////////////////////////////////
  ResultCombination& FacResultCombination::
  create (const TravelQuery_T& iQueryString) {
    ResultCombination* oResultCombination_ptr = NULL;

    oResultCombination_ptr = new ResultCombination (iQueryString);
    assert (oResultCombination_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (oResultCombination_ptr);

    return *oResultCombination_ptr;
  }

  // //////////////////////////////////////////////////////////////////////
  void FacResultCombination::
  initLinkWithResultHolder (ResultCombination& ioResultCombination,
                            ResultHolder& ioResultHolder) {
    // Link the ResultCombination to the ResultHolder, and vice versa
    ioResultHolder._resultCombination = &ioResultCombination;
      
    // Add the Result to the ResultCombination internal list (of
    // ResultHolder objects)
    ioResultCombination._resultHolderList.push_back (&ioResultHolder);
  }

}
