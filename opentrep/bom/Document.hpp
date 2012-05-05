#ifndef __OPENTREP_BOM_DOCUMENT_HPP
#define __OPENTREP_BOM_DOCUMENT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <list>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/bom/BomAbstract.hpp>
// Xapian
#include <xapian.h>

namespace OPENTREP {

  // Forward declarations
  struct PlaceKey;


  // //////////////// Type definitions /////////////////
  /**
   * List of Xapian documents.
   */
  typedef std::list<Xapian::Document> XapianDocumentList_T;

  /**
   * Pair of a Xapian document and its associated matching percentage.
   */
  typedef std::pair<Xapian::percent, Xapian::Document> XapianDocumentPair_T;

  /**
   * List of Xapian documents.
   */
  typedef std::list<XapianDocumentPair_T> XapianAlternateDocumentList_T;
  

  // //////////////// Main Class /////////////////
  /**
   * Structure wrapping a Xapian document having matched part of a
   * given query string.
   *
   * It is a structure, as it is aimed to be temporary, the time
   * a Result object be created with the corresponding content.
   */
  struct Document : public BomAbstract {
  public:
    // ////////////////// Getters ////////////////
    /** Get the query string. */
    const TravelQuery_T& getTravelQuery() const {
     return _queryString;
    }

    /**
     * Get the corrected query string.
     * When empty, it means that no correction was necessary.
     */
    const TravelQuery_T& getCorrectedTravelQuery() const {
     return _correctedQueryString;
    }

    /**
     * Get the matching Xapian document.
     */
    const Xapian::Document& getXapianDocument() const {
     return _document;
    }

    /**
     * Get the matching percentage associated to the Xapian document.
     */
    const Xapian::percent& getXapianPercentage() const {
     return _percentage;
    }

    /**
     * Get the edit distance/error, with which the matching has been made.
     */
    const NbOfErrors_T& getEditDistance () const {
      return _editDistance;
    }
    
    /**
     * Get the maximal allowable edit distance/error, with which the
     * matching has been made.
     */
    const NbOfErrors_T& getAllowableEditDistance () const {
      return _allowableEditDistance;
    }
    
    /**
     * Get the extra list of matching Xapian documents (i.e., those
     * having matched with the same weight as the main one).
     */
    const XapianDocumentList_T& getExtraDocumentList() const {
     return _documentList;
    }

    /**
     * Get the alternate list of matching Xapian documents (i.e., those
     * having matched with a lower weight than the main one).
     */
    const XapianAlternateDocumentList_T& getAlternateDocumentList() const {
     return _alternateDocumentList;
    }


    // ////////////////// Setters ////////////////
    /**
     * Set the query string.
     */
    void setQueryString (const TravelQuery_T& iQueryString) {
      _queryString = iQueryString;
    }
    
    /**
     * Set the corrected query string.
     */
    void setCorrectedQueryString (const TravelQuery_T& iCorrectedQueryString) {
      _correctedQueryString = iCorrectedQueryString;
    }
    
    /**
     * Set the matching Xapian document.
     */
    void setXapianDocument (const Xapian::Document& iMatchingDocument) {
      _document = iMatchingDocument;
    }

    /**
     * Set the matching percentage associated to the Xapian document.
     */
    void setXapianPercentage (const Xapian::percent& iPercentage) {
      _percentage = iPercentage;
    }

    /**
     * Set the edit distance/error, with which the matching has been made.
     */
    void setEditDistance (const NbOfErrors_T& iEditDistance) {
      _editDistance = iEditDistance;
    }
    
    /**
     * Set the maxiaml allowable edit distance/error, with which the
     * matching has been made.
     */
    void setAllowableEditDistance (const NbOfErrors_T& iAllowableEditDistance) {
      _allowableEditDistance = iAllowableEditDistance;
    }
    
    /**
     * Add a matching Xapian document (having the same matching percentage).
     */
    void addExtraDocument (const Xapian::Document& iMatchingDocument) {
      _documentList.push_back (iMatchingDocument);
    }

    /**
     * Add a matching Xapian document (having a lower matching percentage).
     */
    void addAlternateDocument (const Xapian::percent& iMatchingPercentage,
                               const Xapian::Document& iMatchingDocument) {
      _alternateDocumentList.
        push_back (XapianDocumentPair_T (iMatchingPercentage,
                                         iMatchingDocument));
    }


  public:
    // /////////// Business methods /////////
    /**
     * Extract the primary key from the document data.
     *
     * The primary key is made of the first three words (IATA and ICAO codes,
     * as well as the Geonames ID) of the Xapian document data/content.
     *
     * @param Xapian::Document& The Xapian document.
     * @return PlaceKey& The primary key of the place/POR (point of reference).
     */
    static PlaceKey getPrimaryKey (const Xapian::Document&);

    /**
     * Retrieve the number of extra matches for the given query string,
     * and print a notification in the logs, so that the SQL and Xapian
     * databases can be altered in order to get more specific matches.
     *
     * @param const TravelQuery_T& Query string having yielded that matching
     *                             document.
     * @return NbOfMatches_T The number of matches (i.e., one chosen match
     *                       added to the extra matches).
     */
    NbOfMatches_T notifyIfExtraMatch() const;

    
  public:
    // /////////// Display support methods /////////
    /**
     * Dump the structure into an output stream.
     *
     * @param ostream& the output stream.
     */
    void toStream (std::ostream& ioOut) const;

    /**
     * Read a structure from an input stream.
     *
     * @param istream& the input stream.
     */
    void fromStream (std::istream& ioIn);

    /**
     * Get the serialised version of the structure.
     */
    std::string toString() const;
    
    /**
     * Get a shorter serialised version of the structure.
     */
    std::string toShortString() const;
    
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


  public:
    // //////////////// Constructors and Destructors /////////////
    /** Default constructor. */
    // Document();

    /** Default copy constructor. */
    // Document (const Document&);

    /**
     * Default destructor.
     */
    ~Document ();

    
  private:
    // ///////////////// Attributes //////////////////
    /**
     * Query string with which a Xapian full text search is done.
     */
    TravelQuery_T _queryString;

    /**
     * Query string with which a Xapian full text search is done.
     */
    TravelQuery_T _correctedQueryString;

    /**
     * Matching percentage, as returned by the Xapian full text search.
     *
     * Generally, that percentage is equal to, or close to, 100%.
     */
    Xapian::percent _percentage;

    /**
     * Matching document, as returned by the Xapian full text search.
     */
    Xapian::Document _document;

    /**
     * Edit distance/error, with which the matching has been made.
     */
    NbOfErrors_T _editDistance;
    
    /**
     * Maximum allowable edit distance/error, with which the matching
     * has been made.
     */
    NbOfErrors_T _allowableEditDistance;
    
    /**
     * List of Xapian documents having the same matching percentage.
     *
     * Hence, any of those other Xapian documents could have been
     * chosen, instead of the main one.
     */
    XapianDocumentList_T _documentList;

    /**
     * List of Xapian documents having the a lower matching percentage.
     *
     * Those alternate matches can be suggested (in the famous
     * "Did you mean Xxx?" question) to the end user.
     */
    XapianAlternateDocumentList_T _alternateDocumentList;
  };

}
#endif // __OPENTREP_BOM_DOCUMENT_HPP
