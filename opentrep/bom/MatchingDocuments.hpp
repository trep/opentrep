#ifndef __OPENTREP_BOM_MATCHINGDOCUMENTS_HPP
#define __OPENTREP_BOM_MATCHINGDOCUMENTS_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <list>
#include <map>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/basic/StructAbstract.hpp>
#include <opentrep/bom/ScoreBoard.hpp>
// Xapian
#include <xapian.h>

namespace OPENTREP {

  // Forward declarations
  struct PlaceKey;


  // //////////////////// Type definitions /////////////////////
  /**
   * Pair of a Xapian document and its associated score board.
   */
  typedef std::pair<Xapian::Document, ScoreBoard> XapianDocumentPair_T;

  /**
   * (STL) List of Xapian documents and their associated score board.
   */
  typedef std::list<XapianDocumentPair_T> DocumentList_T;
  
  /**
   * (STL) Map of Xapian documents and their associated score board.
   */
  typedef std::map<Xapian::docid, XapianDocumentPair_T> DocumentMap_T;
  

  // //////////////////////// Main Class /////////////////////////
  /**
   * @brief Structure wrapping a set of Xapian documents having
   *        matched a given query string.
   *
   * It is a structure, as it is aimed to be temporary, the time
   * a Result BOM object be created with the corresponding content.
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
     * Get the edit distance/error, with which the matching has been made.
     */
    const NbOfErrors_T& getEditDistance() const {
      return _editDistance;
    }
    
    /**
     * Get the maximal allowable edit distance/error, with which the
     * matching has been made.
     */
    const NbOfErrors_T& getAllowableEditDistance() const {
      return _allowableEditDistance;
    }
    
    /**
     * Get the list of documents.
     */
    const DocumentList_T& getDocumentList() const {
     return _documentList;
    }

    /**
     * Get the map of documents.
     */
    const DocumentMap_T& getDocumentMap() const {
     return _documentMap;
    }

    /**
     * Get the Xapian ID of the best matching document.
     */
    const Xapian::docid& getBestDocID() const {
      return _bestDocID;
    }

    /**
     * Get the combined weight, for all the rules (full-text, PageRank, etc)
     */
    const Percentage_T& getBestCombinedWeight() const {
      return _bestCombinedWeight;
    }

    /**
     * Get the best matching Xapian document.
     */
    const Xapian::Document& getBestXapianDocument() const;


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
     * Set the latest score for the given type. If no score value has
     * already been stored for that type, create it.
    void setScore (const ScoreType& iScoreType, const Score_T& iScore) {
      _scoreBoard.setScore (iScoreType, iScore);
    }
     */

    /**
     * Add a Xapian document to the dedicated (STL) list and (STL) map.
     *
     * \note The score type is not specified, as it is corresponding,
     *       by construction, to the (Xapian-based) full-text matching.
     *       Indeed, when there is no (Xapian-based) full-text matching,
     *       by construction, no Xapian document has been found matching
     *       a given string.
     *
     * @param const Xapian::Document& The Xapian document to be added.
     * @param const Score_T& The matching percentage.
     */
    void addDocument (const Xapian::Document&, const Score_T&);

    /**
     * Set the Xapian ID of the best matching document.
     */
    void setBestDocID (const Xapian::docid& iDocID) {
      _bestDocID = iDocID;
    }

    /**
     * Set the best combined weight, for all the rules (full-text,
     * PageRank, etc)
     */
    void setBestCombinedWeight (const Percentage_T& iPercentage) {
      _bestCombinedWeight = iPercentage;
    }


  public:
    // /////////// Business methods /////////
    /**
     * Extract the primary key from the data of the given Xapian document.
     *
     * The primary key is made of the first three words (IATA and ICAO
     * codes, as well as the Geonames ID) of the Xapian document
     * data/content.
     *
     * @param Xapian::Document& The Xapian document.
     * @return PlaceKey& The primary key of the place/POR (point of
     *         reference).
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
     * That string reflects any spelling corrections, that Xapian may have
     * been suggested.
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
     * Edit distance/error, with which the matching has been made.
     */
    NbOfErrors_T _editDistance;
    
    /**
     * Maximum allowable edit distance/error, with which the matching
     * has been made.
     */
    NbOfErrors_T _allowableEditDistance;
    
    /**
     * Xapian ID of the best matching document.
     */
    Xapian::docid _bestDocID;

    /**
     * Best weight for all the rules (e.g., full-text matching,
     * PageRank, heuristic, popularity) for all the documents collected
     * so far.
     */
    Percentage_T _bestCombinedWeight;

    /**
     * (STL) List of Xapian documents and their associated score board.
     */
    DocumentList_T _documentList;
  
    /**
     * (STL) Map of Xapian documents and their associated score board.
     */
    DocumentMap_T _documentMap;
  };

}
#endif // __OPENTREP_BOM_MATCHINGDOCUMENTS_HPP
