#ifndef __OPENTREP_FAC_FACRESULTCOMBINATION_HPP
#define __OPENTREP_FAC_FACRESULTCOMBINATION_HPP

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

  // Forward declarations.
  class ResultCombination;
  class Result;

  /**
   * Factory for ResultCombination.
   */
  class FacResultCombination : public FacBomAbstract {
  public:

    /**
     * Provide the unique instance.
     *
     * The singleton is instantiated when first used
     *
     * @return FacResultCombination&
     */
    static FacResultCombination& instance();

    /**
     * Destructor.
     *
     * The Destruction put the _instance to NULL
     * in order to be clean for the next FacResultCombination::instance()
     */
    virtual ~FacResultCombination();

    /**
     * Create a new ResultCombination object.
     *
     * This new object is added to the list of instantiated objects.
     *
     * @return ResultCombination& The newly created object.
     */
    ResultCombination& create (const TravelQuery_T& iQueryString);

    /**
     * Initialise the link between a ResultCombination and a ResultHolder.
     *
     * @param ResultCombination&
     * @param ResultHolder&
     * @exception FacExceptionNullPointer
     * @exception FacException.
     */
    static void initLinkWithResultHolder (ResultCombination&, ResultHolder&);


  private:
    /**
     * Default Constructor.
     *
     * This constructor is private in order to ensure the singleton
     * pattern.
     */
    FacResultCombination ();
    FacResultCombination (const FacResultCombination&);

  private:
    /**
     * The unique instance.
     */
    static FacResultCombination* _instance;
  };
}
#endif // __OPENTREP_FAC_FACRESULTCOMBINATION_HPP
