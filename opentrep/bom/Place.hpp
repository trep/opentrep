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
#include <opentrep/NameMatrix.hpp>
#include <opentrep/Location.hpp>
#include <opentrep/bom/BomAbstract.hpp>
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
     * Get the underlying Location structure, which is an extract of the
     * Place object.
     *
     * @return const Location& The underlying Location structure.
     */
    const Location& getLocation() const {
      return _location;
    }

    /**
     * Get the primary key (IATA and ICAO codes, Geonames ID) of the place.
     */
    const LocationKey& getKey() const {
      return _location.getKey();
    }
    
    /**
     * Get the IATA code.
     */
    const IATACode_T& getIataCode() const {
      return _location.getIataCode();
    }
    
    /**
     * Get the ICAO code.
     */
    const ICAOCode_T& getIcaoCode() const {
      return _location.getIcaoCode();
    }
    
    /**
     * Get the Geonames ID.
     */
    const GeonamesID_T& getGeonamesID() const {
      return _location.getGeonamesID();
    }

    /** 
     * Get the common name (usually in American English, but not necessarily
     * in ASCII).
     */
    const CommonName_T& getCommonName() const {
      return _location.getCommonName();
    }
    
    /** 
     * Get the ASCII name (not necessarily in English).
     */
    const ASCIIName_T& getAsciiName() const {
      return _location.getAsciiName();
    }
    
    /**
     * Get the FAA code.
     */
    const FAACode_T& getFaaCode() const {
      return _location.getFaaCode();
    }
    
    /**
     * Get the related/served IATA city code.
     */
    const CityCode_T& getCityCode() const {
      return _location.getCityCode();
    }

    /**
     * Get the state code.
     */
    const StateCode_T& getStateCode() const {
      return _location.getStateCode();
    }
    
    /**
     * Get the country code.
     */
    const CountryCode_T& getCountryCode() const {
      return _location.getCountryCode();
    }
    
    /**
     * Get the region code.
     */
    const RegionCode_T& getRegionCode() const {
      return _location.getRegionCode();
    }
    
    /**
     * Get the time-zone.
     */
    const TimeZone_T& getTimeZone() const {
      return _location.getTimeZone();
    }
    
    /**
     * Get the geographical latitude.
     */
    const Latitude_T& getLatitude() const {
      return _location.getLatitude();
    }

    /**
     * Get the geographical longitude.
     */
    const Longitude_T& getLongitude() const {
      return _location.getLongitude();
    }
    
    /**
     * Get the PageRank/importance. 
     */
    const PageRank_T& getPageRank() const {
      return _location.getPageRank();
    }
    
    /**
     * Get the Wikipedia link.
     */
    const WikiLink_T& getWikiLink() const {
      return _location.getWikiLink();
    }
    
    /**
     * Get the map of name lists.
     */
    const NameMatrix& getNameMatrix() const {
      return _location.getNameMatrix();
    }

    /**
     * Get the original keywords.
     */
    const std::string& getOriginalKeywords() const {
      return _originalKeywords;
    }
    
    /**
     * Get the corrected keywords.
     */
    const std::string& getCorrectedKeywords() const {
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
      const NameMatrix& lNameMatrix = _location.getNameMatrix();
      return lNameMatrix.getNameList (iLanguageCode, ioNameList);
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
    void setKey (const LocationKey& iKey) {
      _location.setKey (iKey);
    }
    
    /**
     * Set the IATA code.
     */
    void setIataCode (const std::string& iIataCode) {
      _location.setIataCode (iIataCode);
    }
    
    /**
     * Set the ICAO code.
     */
    void setIcaoCode (const std::string& iIcaoCode) {
      _location.setIcaoCode (iIcaoCode);
    }
    
    /**
     * Set the Geonames ID.
     */
    void setGeonamesID (const GeonamesID_T& iGeonamesID) {
      _location.setGeonamesID (iGeonamesID);
    }

    /** 
     * Set the common name (usually in American English, but not necessarily
     * in ASCII).
     */
    void setCommonName (const std::string& iName) {
      _location.setCommonName (iName);
    }
    
    /** 
     * Set the ASCII name (not necessarily in English).
     */
    void setAsciiName (const std::string& iName) {
      _location.setAsciiName (iName);
    }
    
    /**
     * Set the FAA code.
     */
    void setFaaCode (const std::string& iFaaCode) {
      _location.setFaaCode (iFaaCode);
    }
    
    /**
     * Set the related/served IATA city code.
     */
    void setCityCode (const std::string& iCityCode) {
      _location.setCityCode (iCityCode);
    }
    
    /**
     * Set the state code.
     */
    void setStateCode (const std::string& iStateCode) {
      _location.setStateCode (iStateCode);
    }
    
    /**
     * Set the country code.
     */
    void setCountryCode (const std::string& iCountryCode) {
      _location.setCountryCode (iCountryCode);
    }
    
    /**
     * Set the region code.
     */
    void setRegionCode (const std::string& iRegionCode) {
      _location.setRegionCode (iRegionCode);
    }
    
    /**
     * Set the time-zone.
     */
    void setTimeZone (const std::string& iTimeZone) {
      _location.setTimeZone (iTimeZone);
    }
    
    /**
     * Set the geographical latitude.
     */
    void setLatitude (const Latitude_T& iLatitude) {
      _location.setLatitude (iLatitude);
    }

    /**
     * Set the geographical longitude.
     */
    void setLongitude (const Longitude_T& iLongitude) {
      _location.setLongitude (iLongitude);
    }
    
    /**
     * Set the PageRank.
     */
    void setPageRank (const PageRank_T& iPageRank) {
      _location.setPageRank (iPageRank);
    }
    
    /**
     * Set the Wikipedia link.
     */
    void setWikiLink (const std::string& iWikiLink) {
      _location.setWikiLink (iWikiLink);
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
    void addName (const Language::EN_Language& iLanguageCode,
                  const std::string& iName) {
      _location.addName (iLanguageCode, iName);
    }

    /**
     * Reset the map of name lists.
     */
    void resetMatrix() {
      _location.resetMatrix();
    }

    /**
     * Reset the index/spelling (STL) sets.
     */
    void resetIndexSets();

    
  public:
    // /////////// Business methods /////////
    /**
     * Get the underlying Location structure, which is an extract of the
     * Place object. That (Location) structure is passed back to the caller
     * of the service.
     *
     * @return Location The Location structure just created and filled
     *                  with the parameters of the Place object.
     */
    const Location& completeLocation();

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
     * Get a string describing the whole key (IATA and ICAO codes, Geonames ID).
     */
    std::string describeKey() const {
      return "";
    }

    /**
     * Get a string describing the whole key (IATA and ICAO codes, Geonames ID).
     */
    std::string describeShortKey() const {
      return "";
    }
    
    /**
     * Get the serialised version of the Place object.
     */
    std::string toString() const;

    /**
     * Get a short display of the Business Object.
     */
    std::string toShortString() const;
    
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
     * Main constructor from a primary key.
     */
    Place (const LocationKey&);

    /**
     * Main constructor from a full Location structure.
     */
    Place (const Location&);

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
     * Location structure, containing a key (IATA and ICAO codes, Geonames ID)
     * and the full details of the POR (point of reference): name, geographical
     * coordinates, etc.
     */
    Location _location;

    
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
