#ifndef __OPENTREP_FAC_FACRESULTHOLDER_HPP
#define __OPENTREP_FAC_FACRESULTHOLDER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// OpenTREP
#include <opentrep/factory/FacBomAbstract.hpp>
#include <opentrep/OPENTREP_Types.hpp>

// Forward declarations
namespace Xapian {
  class Database;
}

namespace OPENTREP {

  /** Forward declarations. */
  class ResultHolder;
  class Result;

  /** Factory for Result. */
  class FacResultHolder : public FacBomAbstract {
  public:

    /** Provide the unique instance.
        <br> The singleton is instantiated when first used
        @return FacResultHolder& */
    static FacResultHolder& instance();

    /** Destructor.
        <br> The Destruction put the _instance to NULL
        in order to be clean for the next FacResultHolder::instance() */
    virtual ~FacResultHolder();

    /** Create a new ResultHolder object.
        <br>This new object is added to the list of instantiated objects.
        @return ResultHolder& The newly created object. */
    ResultHolder& create (const TravelQuery_T& iQueryString,
                          const Xapian::Database& iDatabase);

    /** Initialise the link between a ResultHolder and a Result.
        @param ResultHolder&
        @param Result&
        @exception FacExceptionNullPointer
        @exception FacException.*/
    static void initLinkWithResult (ResultHolder&, Result&);

    
  private:
    /** Default Constructor.
        <br>This constructor is private in order to ensure the singleton
        pattern.*/
    FacResultHolder ();
    FacResultHolder (const FacResultHolder&);

  private:
    /** The unique instance.*/
    static FacResultHolder* _instance;

  };
}
#endif // __OPENTREP_FAC_FACRESULTHOLDER_HPP
