#ifndef __OPENTREP_BOM_RESULT_HPP
#define __OPENTREP_BOM_RESULT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// OpenTREP
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/bom/BomAbstract.hpp>
#include <opentrep/bom/Document.hpp>

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
     * Get the Matching Xapian document object, along with its
     * corresponding matching percentage.
     */
    const Document& getMatchingDocument() const {
      return _matchingDocument;
    }

    /**
     * Retrieve the percentage corresponding to the matching Xapian
     * document object.
     */
    const Percentage_T& getXapianPercentage() const {
      return _matchingDocument.getXapianPercentage();
    }
    
    /**
     * Retrieve the matching Xapian document object.
     */
    const Xapian::Document& getXapianDocument() const {
      return _matchingDocument.getXapianDocument();
    }

    
    // ////////////// Setters /////////////
    /**
     * Set the query string.
     */
    void setQueryString (const TravelQuery_T& iQueryString) {
      _queryString = iQueryString;
    }

    /**
     * Set the matching Xapian document object and its corresponding
     * matching percentage.
     */
    void setMatchingDocument (const Document& iMatchingDocument) {
      _matchingDocument = iMatchingDocument;
    }

    
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
    Result (const Xapian::Database&);

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
    ~Result ();

    /**
     * Initialise (reset the list of documents).
     */
    void init ();

    
  private:
    // /////////////// Attributes ////////////////
    /**
     * Parent ResultHolder.
     */
    ResultHolder* _resultHolder;
    
    /**
     * Query string having generated the list of document.
     */
    TravelQuery_T _queryString;
    
    /**
     * Xapian database.
     */
    const Xapian::Database& _database;
    
    /**
     * Matching Xapian document object, along with its corresponding
     * matching percentage.
     */
    Document _matchingDocument;
  };

}
#endif // __OPENTREP_BOM_RESULT_HPP
