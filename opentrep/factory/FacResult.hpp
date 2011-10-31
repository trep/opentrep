#ifndef __OPENTREP_FAC_FACRESULT_HPP
#define __OPENTREP_FAC_FACRESULT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// OPENTREP
#include <opentrep/factory/FacBomAbstract.hpp>

namespace OPENTREP {

  /** Forward declarations. */
  class Result;

  /** Factory for Result. */
  class FacResult : public FacBomAbstract {
  public:

    /** Provide the unique instance.
        <br> The singleton is instantiated when first used
        @return FacResult& */
    static FacResult& instance();

    /** Destructor.
        <br> The Destruction put the _instance to NULL
        in order to be clean for the next FacResult::instance() */
    virtual ~FacResult();

    /** Create a new Result object.
        <br>This new object is added to the list of instantiated objects.
        @return Result& The newly created object. */
    Result& create (const Xapian::Database&);
    
    
  private:
    /** Default Constructor.
        <br>This constructor is private in order to ensure the singleton
        pattern.*/
    FacResult ();
    FacResult (const FacResult&);

  private:
    /** The unique instance.*/
    static FacResult* _instance;

  };
}
#endif // __OPENTREP_FAC_FACRESULT_HPP
