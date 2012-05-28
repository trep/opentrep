#ifndef __OPENTREP_BOM_MATCHINGDOCUMENTS_HPP
#define __OPENTREP_BOM_MATCHINGDOCUMENTS_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <list>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/basic/StructAbstract.hpp>
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
  typedef std::pair<Percentage_T, Xapian::Document> XapianDocumentPair_T;

  /**
   * List of Xapian documents.
   */
  typedef std::list<XapianDocumentPair_T> XapianAlternateDocumentList_T;
  

  // //////////////// Main Class /////////////////
  /**
   * @brief Structure wrapping a Xapian document having matched part
   *        of a given query string.
   *
   * It is a structure, as it is aimed to be temporary, the time
   * a Result object be created with the corresponding content.
   */
  struct MatchingDocuments : public StructAbstract {
  public:
    // ////////////////// Getters ////////////////
    /**
     * Get the query string.
     */
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
     * State whether there has been a full-text match.
     */
    bool hasFullTextMatched() const {
      return _hasFullTextMatched;
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
    const Percentage_T& getXapianPercentage() const {
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


  public:
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
     * Set whether there has been a full-text match.
     */
    void setHasFullTextMatched (const bool iHasFullTextMatched) {
      _hasFullTextMatched = iHasFullTextMatched;
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
    void setXapianPercentage (const Percentage_T& iPercentage) {
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
    void addAlternateDocument (const Percentage_T& iMatchingPercentage,
                               const Xapian::Document& iMatchingDocument) {
      _alternateDocumentList.
        push_back (XapianDocumentPair_T (iMatchingPercentage,
                                         iMatchingDocument));
    }


  public:
    // /////////// Business methods /////////
    /**
     * Extract the primary key from the data of the given Xapian document.
     *
     * The primary key is made of the first three words (IATA and ICAO codes,
     * as well as the Geonames ID) of the Xapian document data/content.
     *
     * @param Xapian::Document& The Xapian document.
     * @return PlaceKey& The primary key of the place/POR (point of reference).
     */
    static PlaceKey getPrimaryKey (const Xapian::Document&);

    /**
     * Extract the PageRank from the data of the given Xapian document.
     *
     * The PageRank is the fourth word of the Xapian document data/content.
     *
     * @param Xapian::Document& The Xapian document.
     * @return Percentage_T& The PageRank of the place/POR (point of reference).
     */
    static Percentage_T getPageRank (const Xapian::Document&);

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

    /**
     * Get the best matching weight for the full-text match.
     */
    Percentage_T calculateMatchingWeight() const;


  public:
    // /////////// Display support methods /////////
    /**
     * Dump the structure into an output stream.
     *
     * @param ostream& the output stream.
     */
    void toStream (std::ostream&) const;

    /**
     * Read a structure from an input stream.
     *
     * @param istream& the input stream.
     */
    void fromStream (std::istream&);

    /**
     * Display the key of the structure.
     *
     * @return std::string Dump of the structure.
     */
    std::string describeKey() const;

    /**
     * Display of the structure.
     *
     * @return std::string Dump of the structure.
     */
    std::string describe() const;


  public:
    // //////////////// Constructors and Destructors /////////////
    /**
     * Main constructor.
     */
    MatchingDocuments (const TravelQuery_T&);

    /**
     * Default constructor; default implementation.
     */
    //MatchingDocuments();

    /**
     * Default copy constructor; default implementation.
     */
    //MatchingDocuments (const MatchingDocuments&);

    /**
     * Default destructor.
     */
    ~MatchingDocuments();


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
     * Whether or not there has been a full-text match.
     *
     * When there has been a full-text match, the set of documents contains
     * those matches. Otherwise, the set of documents is empty.
     */
    bool _hasFullTextMatched;

    /**
     * Matching percentage, as returned by the Xapian full text search.
     *
     * Generally, that percentage is equal to, or close to, 100%.
     */
    Percentage_T _percentage;

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
#endif // __OPENTREP_BOM_MATCHINGDOCUMENTS_HPP
