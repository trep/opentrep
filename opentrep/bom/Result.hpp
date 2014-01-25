#ifndef __OPENTREP_BOM_RESULT_HPP
#define __OPENTREP_BOM_RESULT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <list>
#include <map>
// Xapian
#include <xapian.h>
// OpenTREP
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/bom/BomAbstract.hpp>
#include <opentrep/bom/ScoreBoard.hpp>

namespace OPENTREP {

  // Forward declarations
  class ResultHolder;
  struct LocationKey;
  struct Location;
  class Place;


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
   * @brief Class wrapping a set of Xapian documents having matched a
   *        given query string.
   */
  class Result : public BomAbstract {
    friend class FacResultHolder;
    friend class FacResult;
  public:
    // ////////////////////// Getters /////////////////////
    /**
     * Get the query string.
     */
    const TravelQuery_T& getQueryString() const {
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
     * Get the Xapian document and associated score-board corresponding
     * to the given document ID.
     */
    const XapianDocumentPair_T& getDocumentPair (const Xapian::docid&) const;

    /**
     * Get the Xapian document corresponding to the given document ID.
     */
    const Xapian::Document& getDocument (const Xapian::docid&) const;

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
     * Get the details of the best matching document.
     */
    const RawDataString_T& getBestDocData() const {
      return _bestDocData;
    }

    /**
     * Get the best matching Xapian document.
     */
    const Xapian::Document& getBestXapianDocument() const {
      return getDocument (_bestDocID);
    }


  public:
    // ////////////////////// Setters /////////////////////
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
    
    /**
     * Set the details of the best matching document.
     */
    void setBestDocData (const std::string& iDocData) {
      _bestDocData = RawDataString_T (iDocData);
    }

    /**
     * Extract the best matching Xapian document.
     *
     * @param Xapian::MSet& The Xapian matching set. It can be empty.
     * @param Result& The holder for the Xapian documents
     *        to be stored.
     */
    void fillResult (const Xapian::MSet& iMatchingSet);

    /**
     * Fill the Place object with the details of the best matching
     * Xapian document.
     *
     * @param Place& The Place object.
     */
    void fillPlace (Place&) const;


  public:
    // /////////////////// Business support methods /////////////////
    /**
     * For all the elements (strings) of the travel query (string set),
     * perform a Xapian-based full-text match.
     * That Xapian-based full-text match gives (potentially) a full set of
     * matches, some with the highest matching percentage and some with a
     * lower percentage.
     *
     * @param const Xapian::Database& The Xapian index/database.
     * @param const TravelQuery_T& The query string.
     */
    std::string fullTextMatch (const Xapian::Database&, const TravelQuery_T&);

    /**
     * Parse the raw data, as stored by the given Xapian document, and
     * holding all the details of a POR (point of reference).
     *
     * @param const Xapian::Document& The Xapian document.
     * @return Location The Location structure holding all the details
     *                  of the place/POR (point of reference).
     */
    static Location retrieveLocation (const Xapian::Document&);

    /**
     * Parse the raw data, as stored by a typical Xapian document, and
     * holding all the details of a POR (point of reference).
     *
     * @param const RawDataString_T& The Xapian document data.
     * @return Location The Location structure holding all the details
     *                  of the place/POR (point of reference).
     */
    static Location retrieveLocation (const RawDataString_T&);

    /**
     * Extract the primary key from the data of the given Xapian document.
     *
     * The primary key is made of the IATA and ICAO codes, as well as of
     * the Geonames ID. The retrieveLocation() is used to parse the Xapian
     * document raw data.
     *
     * @param Xapian::Document& The Xapian document.
     * @return LocationKey& The primary key of the place/POR (point of
     *         reference).
     */
    static LocationKey getPrimaryKey (const Xapian::Document&);

    /**
     * Extract the Envelope ID from the data of the given Xapian document.
     *
     * The retrieveLocation() is used to parse the Xapian document raw data.
     *
     * @param Xapian::Document& The Xapian document.
     * @return Score_T& The Envelope ID of the place/POR (point of reference),
     *         converted into a floating point (from an integer value).
     */
    static Score_T getEnvelopeID (const Xapian::Document&);

    /**
     * Extract the PageRank from the data of the given Xapian document.
     *
     * The retrieveLocation() is used to parse the Xapian document raw data.
     *
     * @param Xapian::Document& The Xapian document.
     * @return PageRank_T& The PageRank of the place/POR (point of reference).
     */
    static PageRank_T getPageRank (const Xapian::Document&);

    /**
     * Display the Xapian matching percentages for all the matching documents.
     *
     * That is just a summarising step, for convenience/debug reason.
     * No calculation is done here.
     */
    void displayXapianPercentages() const;

    /**
     * Update the ScoreBoard structure, itself associated to the Xapian document
     * corresponding to the given document ID, with the given score.
     *
     * @param const Xapian::docid& Document ID for the Xapian document
     * @param const ScoreType& Score type
     * @param const Score_T& Score/weight value
     */
    void setScoreOnDocMap (const Xapian::docid&,
                           const ScoreType&, const Score_T&);

    /**
     * Calculate/set the envelope weights for all the matching documents.
     *
     * Some documents have a non-null envelope, meaning that they are no
     * longer valid: they were valid for a given period of time in the past,
     * but no longer are. For instance, the corresponding object (POR, airline,
     * etc) may have been closed, removed, replaced.
     */
    void calculateEnvelopeWeights();

    /**
     * Calculate/set the IATA/ICAO code matching weights for all
     * the matching documents
     */
    void calculateCodeMatches();

    /**
     * Calculate/set the PageRanks for all the matching documents
     */
    void calculatePageRanks();

    /**
     * Calculate/set the heuristic weights for all the matching documents
     */
    void calculateHeuristicWeights();

    /**
     * Calculate/set the combined weights for all the matching documents.
     * Store the best matching one in the _bestCombinedWeight attribute.
     */
    void calculateCombinedWeights();

  private:
    /**
     * For all the elements (strings) of the travel query (string set),
     * perform a Xapian-based full-text match.
     * That Xapian-based full-text match gives (potentially) a full set of
     * matches, some with the highest matching percentage and some with a
     * lower percentage.
     *
     * @param const Xapian::Database& The Xapian index/database.
     * @param TravelQuery_T& The query string.
     * @param Xapian::MSet& The resulting matching set of Xapian documents
     */
    std::string fullTextMatch (const Xapian::Database&, const TravelQuery_T&,
                               Xapian::MSet&);


  public:
    // /////////////////// Display support methods /////////////////
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
     * Xapian database.
     */
    const Xapian::Database& _database;

    /**
     * Query string having generated the set of documents.
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
     * Details of the best matching document.
     */
    RawDataString_T _bestDocData;

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
#endif // __OPENTREP_BOM_RESULT_HPP
