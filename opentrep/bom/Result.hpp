#ifndef __OPENTREP_BOM_RESULT_HPP
#define __OPENTREP_BOM_RESULT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// OpenTREP
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/bom/BomAbstract.hpp>
#include <opentrep/bom/MatchingDocuments.hpp>

namespace OPENTREP {

  // Forward declarations
  class ResultHolder;

  /**
   * @brief Class wrapping functions on a list of Xapian Document objects.
   */
  class Result : public BomAbstract {
    friend class FacResultHolder;
    friend class FacResult;
  public:
    // ////////////// Getters /////////////
    /**
     * Get the query string.
     */
    const TravelQuery_T& getQueryString () const {
      return _queryString;
    }

    /**
     * Get the set of Xapian document objects matching the given query
     * string, along with their corresponding matching percentages.
     */
    const MatchingDocuments& getMatchingDocument() const {
      return _matchingDocuments;
    }

    /**
     * State whether there has been a full-text match.
     */
    bool hasFullTextMatched() const {
      return _matchingDocuments.hasFullTextMatched();
    }

    /**
     * Get the combined weight, for all the rules (full-text, PageRank, etc)
     */
    const Percentage_T& getBestCombinedWeight() const {
      return _bestCombinedWeight;
    }


  public:
    // ////////////// Setters /////////////
    /**
     * Set the query string.
     */
    void setQueryString (const TravelQuery_T& iQueryString) {
      _queryString = iQueryString;
    }

    /**
     * Copy the set of Xapian document objects.
     */
    void setMatchingDocument (const MatchingDocuments& iMatchingDocument) {
      _matchingDocuments = iMatchingDocument;
    }

    /**
     * Set the best combined weight, for all the rules (full-text,
     * PageRank, etc)
     */
    void setBestCombinedWeight (const Percentage_T& iPercentage) {
      _bestCombinedWeight = iPercentage;
    }
    

  public:
    // /////////// Business support methods /////////
    /**
     * Calculate/set the PageRanks for all the matching documents
     */
    void calculatePageRanks();

    /**
     * Calculate/set the user input weights for all the matching documents
     */
    void calculateUserInputWeights();

    /**
     * Calculate/set the combined weights for all the matching documents.
     * Store the best matching one in the _bestCombinedWeight attribute.
     */
    void calculateCombinedWeights();


  public:
    // /////////// Display support methods /////////
    /**
     * Dump a Business Object into an output stream.
     *
     * @param ostream& the output stream.
     */
    void toStream (std::ostream& ioOut) const;

    /**
     * Read a Business Object from an input stream.
     *
     * @param istream& the input stream.
     */
    void fromStream (std::istream& ioIn);

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
    Result (const TravelQuery_T&, const Xapian::Database&);

    /**
     * Default constructor.
     */
    Result();

    /**
     * Default copy constructor.
     */
    Result (const Result&);

    /**
     * Destructor.
     */
    ~Result();

    /**
     * Initialise (reset the list of documents).
     */
    void init();

    
  private:
    // /////////////// Attributes ////////////////
    /**
     * Parent ResultHolder.
     */
    ResultHolder* _resultHolder;

    /**
     * Query string having generated the set of documents.
     */
    TravelQuery_T _queryString;

    /**
     * Xapian database.
     */
    const Xapian::Database& _database;

    /**
     * Set of Xapian document objects matching the given query string,
     * along with their corresponding matching percentages.
     */
    MatchingDocuments _matchingDocuments;

    /**
     * Combined weight for the best matching document, taking into account
     * all the rules (full-text, PageRank, etc)
     */
    Percentage_T _bestCombinedWeight;
  };

}
#endif // __OPENTREP_BOM_RESULT_HPP
