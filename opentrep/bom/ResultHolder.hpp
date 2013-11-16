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
  struct StringSet;

  /**
   * @brief Class wrapping functions on a list of Result objects.
   */
  class ResultHolder : public BomAbstract {
    friend class FacResultCombination;
    friend class FacResultHolder;
  public:
    // ////////////////////// Getters /////////////////////
    /**
     * Get the query string.
     */
    const TravelQuery_T& getQueryString() const {
      return _queryString;
    }

    /**
     * Retrieve the list of result objects.
     */
    const ResultList_T& getResultList() const {
      return _resultList;
    }

    /**
     * Get the combined weight, for all the rules (full-text, PageRank, etc)
     */
    const Percentage_T& getCombinedWeight() const {
      return _combinedWeight;
    }

    /**
     * Get the list of corrected strings
     */
    StringSet getCorrectedStringSet() const;


  public:
    // ////////////////////// Setters /////////////////////
    /**
     * Set the combined weight, for all the rules (full-text, PageRank, etc)
     */
    void setCombinedWeight (const Percentage_T& iPercentage) {
      _combinedWeight = iPercentage;
    }


  public:
    // /////////// Business methods ///////////
    /**
     * Display the Xapian matching percentages for all the matching documents.
     */
    void displayXapianPercentages() const;

    /**
     * Calculate/set the envelope weights for all the matching documents
     */
    void calculateEnvelopeWeights() const;

    /**
     * Calculate/set the IATA/ICAO code matching weights for all
     * the matching documents
     */
    void calculateCodeMatches() const;

    /**
     * Calculate/set the PageRanks for all the matching documents
     */
    void calculatePageRanks() const;

    /**
     * Calculate/set the heuristic weights for all the matching documents
     */
    void calculateHeuristicWeights() const;

    /**
     * Calculate/set the combined weights for all the matching documents
     */
    void calculateCombinedWeights();


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
    ResultHolder();
    /**
     * Default copy constructor.
     */
    ResultHolder (const ResultHolder&);
    /**
     * Destructor.
     */
    ~ResultHolder();
    /**
     * Initialise (reset the list of documents).
     */
    void init();

    
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

    /**
     * Combined weight, for all the rules (full-text, PageRank, etc)
     */
    Percentage_T _combinedWeight;
  };

}
#endif // __OPENTREP_BOM_RESULTHOLDER_HPP
