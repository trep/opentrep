#ifndef __OPENTREP_BOM_RESULTHOLDER_HPP
#define __OPENTREP_BOM_RESULTHOLDER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/bom/BomAbstract.hpp>
#include <opentrep/bom/ResultList.hpp>

// Forward declarations
namespace Xapian {
  class Database;
}

namespace OPENTREP {

  // Forward declarations
  class ResultCombination;

  /**
   * @brief Class wrapping functions on a list of Result objects.
   */
  class ResultHolder : public BomAbstract {
    friend class FacResultCombination;
    friend class FacResultHolder;
  public:
    // ////////////// Getters /////////////
    /**
     * Get the query string.
     */
    const TravelQuery_T& getQueryString () const {
      return _queryString;
    }

    /**
     * Retrieve the list of result objects.
     */
    const ResultList_T& getResultList() const {
      return _resultList;
    }


  public:
    // /////////// Business methods ///////////
    /**
     * Calculate the overall matching weight for all the full-text matches.
     */
    Percentage_T calculateMatchingWeights() const;


  public:
    // /////////// Display support methods /////////
    /**
     * Dump a Business Object into an output stream.
     *
     * @param ostream& the output stream.
     */
    void toStream (std::ostream&) const;

    /**
     * Read a Business Object from an input stream.
     *
     * @param istream& the input stream.
     */
    void fromStream (std::istream&);

    /**
     * Get the serialised version of the Business Object.
     */
    std::string toString() const;
    
    /**
     * Get a string describing the whole key (differentiating two objects
     * at any level).
     */
    std::string describeKey() const;

    /**
     * Get a string describing the short key (differentiating two objects
     * at the same level).
     */
    std::string describeShortKey() const;


  private:
    // ////////////// Constructors and Destructors /////////////
    /**
     * Main constructor.
     */
    ResultHolder (const TravelQuery_T&, const Xapian::Database&);
    /**
     * Default constructor.
     */
    ResultHolder ();
    /**
     * Default copy constructor.
     */
    ResultHolder (const ResultHolder&);
    /**
     * Destructor.
     */
    ~ResultHolder ();
    /**
     * Initialise (reset the list of documents).
     */
    void init ();

    
  private:
    // /////////////// Attributes ////////////////
    /**
     * Parent ResultCombination.
     */
    ResultCombination* _resultCombination;

    /**
     * Query string having generated the list of document.
     */
    const TravelQuery_T _queryString;

    /**
     * Xapian database.
     */
    const Xapian::Database& _database;
    
    /**
     * List of result objects.
     */
    ResultList_T _resultList;
  };

}
#endif // __OPENTREP_BOM_RESULTHOLDER_HPP
