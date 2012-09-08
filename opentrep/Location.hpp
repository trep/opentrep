#ifndef __OPENTREP_LOCATION_HPP
#define __OPENTREP_LOCATION_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
#include <string>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/OPENTREP_ParserHelperTypes.hpp>
#include <opentrep/OPENTREP_Abstract.hpp>
#include <opentrep/LocationKey.hpp>
#include <opentrep/NameMatrix.hpp>
#include <opentrep/LocationList.hpp>

namespace OPENTREP {

  /**
   * @brief Structure modelling a (geographical) location. 
   */
  struct Location : public OPENTREP_Abstract {
  public:
    // ///////// Getters ////////
    /**
     * Get the primary key (IATA and ICAO codes, Geonames ID) of that location.
     */
    const LocationKey& getKey() const {
      return _key;
    }
    
    /**
     * Get the IATA code.
     */
    const IATACode_T& getIataCode() const {
      return _key.getIataCode();
    }
    
    /**
     * Get the ICAO code.
     */
    const ICAOCode_T& getIcaoCode() const {
      return _key.getIcaoCode();
    }
    
    /**
     * Get the Geonames ID.
     */
    const GeonamesID_T& getGeonamesID() const {
      return _key.getGeonamesID();
    }

    /** 
     * Get the common name (usually in American English, but not necessarily
     * in ASCII).
     */
    const CommonName_T& getCommonName() const {
      return _commonName;
    }
    
    /** 
     * Get the ASCII name (not necessarily in English).
     */
    const ASCIIName_T& getAsciiName() const {
      return _asciiName;
    }
    
    /** 
     * Get the FAA code. 
     */
    const FAACode_T& getFaaCode() const {
      return _faaCode;
    }
    
    /**
     * Get the City code. 
     */
    const CityCode_T& getCityCode() const {
      return _cityCode;
    }
    
    /**
     * Get the State code. 
     */
    const StateCode_T& getStateCode() const {
      return _stateCode;
    }
    
    /**
     * Get the Country code. 
     */
    const CountryCode_T& getCountryCode() const {
      return _countryCode;
    }
    
    /**
     * Get the Region code. 
     */
    const RegionCode_T& getRegionCode() const {
      return _regionCode;
    }
    
    /**
     * Get the time-zone.
     */
    const TimeZone_T& getTimeZone() const {
      return _timeZone;
    }
    
    /**
     * Get the latitude.
     */
    const Latitude_T& getLatitude() const {
      return _latitude;
    }

    /**
     * Get the longitude.
     */
    const Longitude_T& getLongitude() const {
      return _longitude;
    }
    
    /**
     * Get the population (number of inhabitants).
     */
    const Population_T& getPopulation() const {
      return _population;
    }

    /**
     * Get the elevation.
     */
    const Elevation_T& getElevation() const {
      return _elevation;
    }

    /**
     * Get the GTopo30.
     */
    const GTopo30_T& getGTopo30() const {
      return _gTopo30;
    }

    /**
     * Get the PageRank/importance. 
     */
    const PageRank_T& getPageRank() const {
      return _pageRank;
    }
    
    /**
     * Get the modification date. 
     */
    const Date_T& getModificationDate() const {
      return _modificationDate;
    }
    
    /**
     * Get the Wikipedia link.
     */
    const WikiLink_T& getWikiLink() const {
      return _wikiLink;
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
    const NbOfErrors_T& getAllowableEditDistance () const {
      return _allowableEditDistance;
    }
    
    /**
     * Get the list of extra matching (similar) locations. 
     */
    const LocationList_T& getExtraLocationList() const {
      return _extraLocationList;
    }

    /**
     * Get the list of alternate matching (less similar) locations. 
     */
    const LocationList_T& getAlternateLocationList() const {
      return _alternateLocationList;
    }

    
  public:
    // ///////// Setters //////////
    /**
     * Set the primary key (IATA and ICAO codes, Geonames ID) of that location.
     */
    void setKey (const LocationKey& iKey) {
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
     * Get the common name (usually in American English, but not necessarily
     * in ASCII).
     */
    void setCommonName (const std::string& iName) {
      _commonName = CommonName_T (iName);
    }
    
    /** 
     * Get the ASCII name (not necessarily in English).
     */
    void setAsciiName (const std::string& iName) {
      _asciiName = ASCIIName_T (iName);
    }
    
    /**
     * Set the FAA code. 
     */
    void setFaaCode (const std::string& iFaaCode) {
      _faaCode = FAACode_T (iFaaCode);
    }
    
    /**
     * Set the City code. 
     */
    void setCityCode (const std::string& iCityCode) {
      _cityCode = CityCode_T (iCityCode);
    }
    
    /**
     * Set the State code. 
     */
    void setStateCode (const std::string& iStateCode) {
      _stateCode = StateCode_T (iStateCode);
    }
    
    /**
     * Set the Country code. 
     */
    void setCountryCode (const std::string& iCountryCode) {
      _countryCode = CountryCode_T (iCountryCode);
    }
    
    /**
     * Set the Region code. 
     */
    void setRegionCode (const std::string& iRegionCode) {
      _regionCode = RegionCode_T (iRegionCode);
    }
    
    /**
     * Set the time-zone. 
     */
    void setTimeZone (const std::string& iTimeZone) {
      _timeZone = TimeZone_T (iTimeZone);
    }
    
    /**
     * Set the Latitude.
     */
    void setLatitude (const Latitude_T& iLatitude) {
      _latitude = iLatitude;
    }

    /**
     * Set the Longitude.
     */
    void setLongitude (const Longitude_T& iLongitude) {
      _longitude = iLongitude;
    }
    
    /**
     * Set the population (number of inhabitants).
     */
    void setPopulation (const Population_T& iPopulation) {
      _population = iPopulation;
    }

    /**
     * Set the elevation.
     */
    void setElevation (const Elevation_T& iElevation) {
      _elevation = iElevation;
    }

    /**
     * Set the GTopo30.
     */
    void setGTopo30 (const GTopo30_T& iGTopo30) {
      _gTopo30 = iGTopo30;
    }

    /**
     * Set the PageRank.
     */
    void setPageRank (const PageRank_T& iPageRank) {
      _pageRank = iPageRank;
    }
    
    /**
     * Set the modification date. 
     */
    void setModificationDate (const Date_T& iModDate) {
      _modificationDate = iModDate;
    }

    /**
     * Set the Wikipedia link.
     */
    void setWikiLink (const std::string& iWikiLink) {
      _wikiLink = WikiLink_T (iWikiLink);
    }
    
    /**
     * Add a name for that location.
     *
     * @param const Language::EN_Language& Language in which to add the name.
     * @param const std::string& Name to be added.
     */
    void addName (const Language::EN_Language& iLanguageCode,
                  const std::string& iName) {
      _nameMatrix.addName (iLanguageCode, iName);
    }

    /**
     * Reset the map of name lists.
     */
    void resetMatrix() {
      _nameMatrix.reset();
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
    
    /**
     * Add an extra matching location. 
     */
    void addExtraLocation (const Location& iExtraLocation) {
      _extraLocationList.push_back (iExtraLocation);
    }
    
    /**
     * Add an alternate matching location.
     */
    void addAlternateLocation (const Location& iAlternateLocation) {
      _alternateLocationList.push_back (iAlternateLocation);
    }
    

  public:
    // ///////// Parsing support methods ////////
    /**
     * Calculate the date from the staging details.
     */
    Date_T calculateDate() const;


  public:
    // ///////// Display methods ////////
    /** 
     * Dump a structure into an output stream.
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
     * Display of the Location structure with its associated names.
     *
     * The alternate and extra matches are also fully displayed.
     */
    std::string toString() const;

    /**
     * Get a string describing the whole key (IATA and ICAO codes, Geonames ID).
     */
    std::string describeKey() const;

    /**
     * Get a string describing the whole key (IATA and ICAO codes, Geonames ID).
     */
    std::string describeShortKey() const;
    
    /** 
     * Basic display of the Location structure.
     *
     * The names are not displayed. No information is displayed about
     * alternate and extra matches.
     */
    std::string toBasicString() const;

    /** 
     * Short display of the Location structure.
     *
     * The names are not displayed. Basic information is displayed about
     * alternate and extra matches.
     */
    std::string toShortString() const;

    /** 
     * Display of the Location structure with its associated names.
     *
     * Contrary to the toString() method, the alternate and extra matches
     * are not displayed.
     */
    std::string toSingleLocationString() const;

    
  public:
    /**
     * Main Constructor. 
     */
    Location (const IATACode_T&, const ICAOCode_T&, const GeonamesID_T&,
              const CommonName_T&, const ASCIIName_T&,
              const FAACode_T&, const CityCode_T&, const StateCode_T&, 
              const CountryCode_T&, const RegionCode_T&, const TimeZone_T&,
              const Latitude_T&, const Longitude_T&,
              const PageRank_T&, const WikiLink_T&,
              const std::string& iOriginalKeywords,
              const std::string& iCorrectedKeywords,
              const MatchingPercentage_T& iPercentage,
              const NbOfErrors_T& iEditDistance,
              const NbOfErrors_T& iAllowableEditDistance);

    /**
     * Default Constructor. 
     */
    Location();

    /**
     * Default copy constructor. 
     */
    Location (const Location&);
    
    /**
     * Destructor. 
     */
    ~Location();

    
  private:
    // /////// Attributes /////////
    /**
     * Primary key, made of the IATA and ICAO codes, as well as Geonames ID.
     */
    LocationKey _key;

    /** 
     * Common name (usually in American English, but not necessarily in ASCII).
     */
    CommonName_T _commonName;
    
    /** 
     * ASCII name (not necessarily in English).
     */
    ASCIIName_T _asciiName;
    
    /**
     * FAA code (e.g., ORD).
     */
    FAACode_T _faaCode;

    /**
     * Related IATA city code (e.g., CHI).
     * \note The related city code is empty when the Location structure is itself a city.
     */
    CityCode_T _cityCode;

    /**
     * State code (e.g., IL).
     */
    StateCode_T _stateCode;

    /**
     * Country code (e.g., US).
     */
    CountryCode_T _countryCode;

    /**
     * Alternative country code (e.g., US).
     */
    AltCountryCode_T _altCountryCode;

    /**
     * Region code (e.g., NAMER).
     */
    RegionCode_T _regionCode;

    /**
     * Geographical latitude (e.g., 41.978603).
     */
    Latitude_T _latitude;

    /**
     * Geographical longitude (e.g., -87.904842).
     */
    Longitude_T _longitude;

    /**
     * Feature class (e.g., S for spot).
     */
    FeatureClass_T _featClass;

    /**
     * Feature code (e.g., AIRP for airport).
     */
    FeatureCode_T _featCode;

    /**
     * IATA location type (e.g., A for airport).
     */
    IATAType_T _iataType;

    /**
     * Administrative code of rank 1 (e.g., IL for Illinois).
     */
    AdminCode1_T _adminCode1;

    /**
     * Administrative code of rank 2.
     */
    AdminCode2_T _adminCode2;

    /**
     * Administrative code of rank 3.
     */
    AdminCode3_T _adminCode3;

    /**
     * Administrative code of rank 4.
     */
    AdminCode4_T _adminCode4;

    /**
     * Population.
     */
    Population_T _population;

    /**
     * Elevation.
     */
    Elevation_T _elevation;

    /**
     * Geographical topology 30.
     */
    GTopo30_T _gTopo30;

    /**
     * Time-zone (e.g., America/Chicago).
     */
    TimeZone_T _timeZone;

    /**
     * GMT offset (e.g., 1)
     */
    GMTOffset_T _gmtOffset;

    /**
     * DST offset (e.g., 2)
     */
    DSTOffset_T _dstOffset;

    /**
     * Raw offset (e.g., 1)
     */
    RawOffset_T _rawOffset;

    /**
     * Modification date
     */
    Date_T _modificationDate;

    /**
     * Whether or not that POR is referenced by Geonames
     */
    IsGeonames_T _isGeonames;

    /**
     * Whether or not that POR is an airport
     */
    IsAirport_T _isAirport;

    /**
     * Whether or not that POR is for commercial use
     */
    IsCommercial_T _isCommercial;

    /**
     * Link on the Wikipedia entry
     */
    WikiLink_T _wikiLink;

    /**
     * PageRank/importance (e.g., ATL is 94.66% and BSL is 8.14%).
     */
    PageRank_T _pageRank;

    /**
     * List of names, for each given language.
     */
    NameMatrix _nameMatrix;

    /**
     * Original keywords. 
     */
    std::string _originalKeywords;
    
    /**
     * Original keywords. 
     */
    std::string _correctedKeywords;
    
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
     * List of extra matching (similar) locations. 
     */
    LocationList_T _extraLocationList;

    /**
     * List of alternate matching (less similar) locations. 
     */
    LocationList_T _alternateLocationList;

  public:
    /**
     * Staging Date.
     */
    year_t _itYear;
    month_t _itMonth;
    day_t _itDay;
  };

}
#endif // __OPENTREP_LOCATION_HPP
