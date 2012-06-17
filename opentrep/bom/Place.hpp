#ifndef __OPENTREP_BOM_PLACE_HPP
#define __OPENTREP_BOM_PLACE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
#include <string>
#include <set>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/Location.hpp>
#include <opentrep/bom/BomAbstract.hpp>
#include <opentrep/bom/PlaceKey.hpp>
#include <opentrep/bom/NameMatrix.hpp>
#include <opentrep/bom/PlaceList.hpp>

namespace OPENTREP {

  // Forward declarations
  class World;
  class PlaceHolder;
  
  /**
   * @brief Class modelling a place/POR (point of reference).
   */
  class Place : public BomAbstract {
    friend class FacWorld;
    friend class FacPlace;
    friend class FacPlaceHolder;
    friend class DbaPlace;
  public:
    // /////////// Type definitions //////////
    /**
     * (STL) Set of strings, to be added in the Xapian database (for
     * indexing, spelling, stemming, synonyms, etc).
     */
    typedef std::set<std::string> StringSet_T;


  public:
    // //////////////// Getters ///////////////
    /**
     * Get the primary key (IATA and ICAO codes, Geonames ID) of the place.
     */
    const PlaceKey& getKey() const {
      return _key;
    }
    
    /**
     * Get the IATA code.
     */
    const std::string& getIataCode() const {
      return _key.getIataCode();
    }
    
    /**
     * Get the ICAO code.
     */
    const std::string& getIcaoCode() const {
      return _key.getIcaoCode();
    }
    
    /**
     * Get the Geonames ID.
     */
    const GeonamesID_T& getGeonamesID() const {
      return _key.getGeonamesID();
    }

    /**
     * Get the FAA code.
     */
    const std::string& getFaaCode() const {
      return _faaCode;
    }
    
    /**
     * Get the related/served IATA city code.
     */
    std::string getCityCode() const {
      return _cityCode;
    }

    /**
     * Get the state code.
     */
    const std::string& getStateCode() const {
      return _stateCode;
    }
    
    /**
     * Get the country code.
     */
    const std::string& getCountryCode() const {
      return _countryCode;
    }
    
    /**
     * Get the region code.
     */
    const std::string& getRegionCode() const {
      return _regionCode;
    }
    
    /**
     * Get the time-zone group.
     */
    const std::string& getTimeZoneGroup() const {
      return _timeZoneGroup;
    }
    
    /**
     * Get the geographical latitude.
     */
    double getLatitude() const {
      return _latitude;
    }

    /**
     * Get the geographical longitude.
     */
    double getLongitude() const {
      return _longitude;
    }
    
    /**
     * Get the PageRank/importance. 
     */
    const double& getPageRank() const {
      return _pageRank;
    }
    
    /**
     * Get the Wikipedia link.
     */
    const std::string& getWikiLink() const {
      return _wikiLink;
    }
    
    /**
     * Get the original keywords.
     */
    std::string getOriginalKeywords() const {
      return _originalKeywords;
    }
    
    /**
     * Get the corrected keywords.
     */
    std::string getCorrectedKeywords() const {
      return _correctedKeywords;
    }
    
    /**
     * Get the Xapian document ID.
     */
    const XapianDocID_T& getDocID() const {
      return _docID;
    }

    /**
     * Get the matching percentage.
     */
    const MatchingPercentage_T& getPercentage() const {
      return _percentage;
    }

    /**
     * Get the allowed edit distance/error.
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
     * Get the map of name lists.
     */
    const NameMatrix& getNameMatrix() const {
      return _nameMatrix;
    }

    /**
     * Get, for a given language (code), the corresponding list of names.
     *
     * @param const Language::EN_Language& Language code.
     * @param NameList_T& Empty list of names, which will be filled by the
     *        method if a list exists for that language code.
     * @return bool Whether or not such a list exists for the given
     *         language.
     */
    bool getNameList (const Language::EN_Language& iLanguageCode,
                      NameList_T& ioNameList) const {
      return _nameMatrix.getNameList (iLanguageCode, ioNameList);
    }

    /**
     * Get the list of extra matching (similar) places.
     */
    const PlaceOrderedList_T& getExtraPlaceList() const {
      return _extraPlaceList;
    }

    /**
     * Get the list of alternate matching (less similar) places.
     */
    const PlaceOrderedList_T& getAlternatePlaceList() const {
      return _alternatePlaceList;
    }

    /**
     * Get the (STL) set of terms (for the Xapian index).
     */
    const StringSet_T& getTermSet() const {
      return _termSet;
    }

    /**
     * Get the (STL) set of spelling terms (for the Xapian spelling dictionary).
     */
    const StringSet_T& getSpellingSet() const {
      return _spellingSet;
    }

    /**
     * Get the (STL) set of stemming terms (for the Xapian stemming dictionary).
     */
    const StringSet_T& getStemmingSet() const {
      return _stemmingSet;
    }

    /**
     * Get the (STL) set of synonym terms (for the Xapian synonym dictionary).
     */
    const StringSet_T& getSynonymSet() const {
      return _synonymSet;
    }


  public:
    // ////////////////// Setters /////////////////
    /**
     * Set the primary key (IATA and ICAO codes, Geonames ID) of the place.
     */
    void setKey (const PlaceKey& iKey) {
      _key = iKey;
    }
    
    /**
     * Set the IATA code.
     */
    void setIataCode (const std::string& iIataCode) {
      _key.setIataCode (iIataCode);
    }
    
    /**
     * Set the ICAO code.
     */
    void setIcaoCode (const std::string& iIcaoCode) {
      _key.setIcaoCode (iIcaoCode);
    }
    
    /**
     * Get the Geonames ID.
     */
    void setGeonamesID (const GeonamesID_T& iGeonamesID) {
      _key.setGeonamesID (iGeonamesID);
    }

    /**
     * Set the FAA code.
     */
    void setFaaCode (const std::string& iFaaCode) {
      _faaCode = iFaaCode;
    }
    
    /**
     * Set the related/served IATA city code.
     */
    void setCityCode (const std::string& iCityCode) {
      _cityCode = iCityCode;
    }
    
    /**
     * Set the state code.
     */
    void setStateCode (const std::string& iStateCode) {
      _stateCode = iStateCode;
    }
    
    /**
     * Set the country code.
     */
    void setCountryCode (const std::string& iCountryCode) {
      _countryCode = iCountryCode;
    }
    
    /**
     * Set the region code.
     */
    void setRegionCode (const std::string& iRegionCode) {
      _regionCode = iRegionCode;
    }
    
    /**
     * Set the time-zone group.
     */
    void setTimeZoneGroup (const std::string& iTimeZoneGroup) {
      _timeZoneGroup = iTimeZoneGroup;
    }
    
    /**
     * Set the geographical latitude.
     */
    void setLatitude (const double& iLatitude) {
      _latitude = iLatitude;
    }

    /**
     * Set the geographical longitude.
     */
    void setLongitude (const double& iLongitude) {
      _longitude = iLongitude;
    }
    
    /**
     * Set the PageRank.
     */
    void setPageRank (const double& iPageRank) {
      _pageRank = iPageRank;
    }
    
    /**
     * Set the Wikipedia link.
     */
    void setWikiLink (const std::string& iWikiLink) {
      _wikiLink = iWikiLink;
    }
    
    /**
     * Set the original keywords.
     */
    void setOriginalKeywords (const std::string& iOriginalKeywords) {
      _originalKeywords = iOriginalKeywords;
    }
    
    /**
     * Set the corrected keywords.
     */
    void setCorrectedKeywords (const std::string& iCorrectedKeywords) {
      _correctedKeywords = iCorrectedKeywords;
    }
    
    /**
     * Set the Xapian document ID.
     */
    void setDocID (const XapianDocID_T& iDocID) {
      _docID = iDocID;
    }

    /**
     * Set the Xapian matching percentage.
     */
    void setPercentage (const MatchingPercentage_T& iPercentage) {
      _percentage = iPercentage;
    }

    /**
     * Set the allowed edit distance/error.
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

    
  public:
    // ////////// Setters in underlying names ////////
    /**
     * Add a name for the place.
     *
     * @param const Language::EN_Language& Language in which to add the name.
     * @param const std::string& Name to be added.
     */
    void addName (const Language::EN_Language& iLanguageCode, const std::string& iName) {
      _nameMatrix.addName (iLanguageCode, iName);
    }

    /**
     * Reset the map of name lists.
     */
    void resetMatrix() {
      _nameMatrix.reset();
    }

    /**
     * Reset the index/spelling (STL) sets.
     */
    void resetIndexSets();

    
  public:
    // /////////// Business methods /////////
    /**
     * Create a Location structure, which is a light copy of the Place
     * object. That (Location) structure is passed back to the caller
     * of the service.
     *
     * @return Location The Location structure just created and filled
     *                  with the parameters of the Place object.
     */
    Location createLocation() const;

    /**
     * Build the (STL) sets of (Xapian-related) terms, spelling,
     * synonyms, etc.
     */
    void buildIndexSets();


  public:
    // ///////// Display methods ////////
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
     * Get the serialised version of the Place object.
     */
    std::string toString() const;

    /**
     * Get a short display of the Business Object.
     */
    std::string toShortString() const;
    
    /**
     * Get a string describing the whole key (differentiating two
     * objects at any level).
     */
    std::string describeKey() const;

    /**
     * Get a string describing the short key (differentiating two
     * objects at the same level).
     */
    std::string describeShortKey() const;
    
    /**
     * Get a string describing the (STL) sets of terms for the Xapian database.
     */
    std::string describeSets() const;

    /**
     * Display the full Place context.
     */
    std::string display() const;

    /**
     * Display a short Place context.
     */
    std::string shortDisplay() const;

    
  private:
    /**
     * Main constructor.
     */
    Place (const PlaceKey&);

    /**
     * Default constructor.
     */
    Place();

    /**
     * Copy constructor.
     */
    Place (const Place&);
    
    /**
     * Destructor.
     */
    virtual ~Place();


  private:
    // ////////////////////// Parent objects ////////////////////
    /**
     * Parent: World object.
     */
    World* _world;
    
    /**
     * Parent PlaceHolder (not always defined,for instance if the
     * current Place object is an extra or alternate one).
     */
    PlaceHolder* _placeHolder;
    
    /**
     * Parent (main) Place (not always defined, for instance if the
     * current Place object is itself a main one).
     */
    Place* _mainPlace;

    
  private:
    // ///////////////// Attributes ///////////////////////
    /**
     * Primary key, made of the IATA and ICAO codes, as well as Geonames ID.
     */
    PlaceKey _key;

    /**
     * FAA code (e.g., ORD).
     */
    std::string _faaCode;

    /**
     * Related IATA city code (e.g., CHI).
     * \note The related city code is empty when the Place object is itself a city.
     */
    std::string _cityCode;

    /**
     * State code (e.g., IL).
     */
    std::string _stateCode;

    /**
     * Country code (e.g., US).
     */
    std::string _countryCode;

    /**
     * Region code (e.g., NAMER).
     */
    std::string _regionCode;

    /**
     * Time-zone group (e.g., America/Chicago).
     */
    std::string _timeZoneGroup;

    /**
     * Geographical latitude (e.g., 41.978603).
     */
    double _latitude;

    /**
     * Geographical longitude (e.g., -87.904842).
     */
    double _longitude;

    /**
     * PageRank/importance (e.g., ATL is 94.66% and BSL is 8.14%).
     */
    double _pageRank;

    /**
     * Link on the Wikipedia entry
     */
    std::string _wikiLink;

    /**
     * List of names, for each given language.
     */
    NameMatrix _nameMatrix;


  private:
    // ///////////// Full-text matching process support attributes //////////
    /**
     * Original keywords.
     */
    std::string _originalKeywords;
    
    /**
     * Original keywords.
     */
    std::string _correctedKeywords;
    
    /**
     * Xapian document ID.
     *
     * That ID is set only when inserting the Document structure into
     * the Xapian index (database).
     */
    XapianDocID_T _docID;

    /**
     * Matching percentage.
     */
    MatchingPercentage_T _percentage;

    /**
     * Allowed edit error/distance.
     */
    NbOfErrors_T _editDistance;

    /**
     * Maximum allowable edit distance/error, with which the matching
     * has been made.
     */
    NbOfErrors_T _allowableEditDistance;
    
    /**
     * List of extra matching (similar) places.
     */
    PlaceOrderedList_T _extraPlaceList;

    /**
     * List of alternate matching (less similar) places.
     */
    PlaceOrderedList_T _alternatePlaceList;

    /**
     * Set of unique terms (strings), which serve as indexing the
     * Xapian document corresponding to the current Place object.
     */
    StringSet_T _termSet;

    /**
     * Set of unique terms (strings), which serve as basis for right
     * spelling. They are added to the Xapian database.
     */
    StringSet_T _spellingSet;

    /**
     * Set of unique terms (strings), which serve as basis for
     * stemming. They are added to the Xapian database.
     */
    StringSet_T _stemmingSet;

    /**
     * Set of unique terms (strings), which serve as basis for
     * synonyms. They are added to the Xapian database.
     */
    StringSet_T _synonymSet;
  };

}
#endif // __OPENTREP_BOM_PLACE_HPP
