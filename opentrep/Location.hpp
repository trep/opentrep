#ifndef __OPENTREP_LOCATION_HPP
#define __OPENTREP_LOCATION_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
#include <string>
#include <list>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/OPENTREP_ParserHelperTypes.hpp>
#include <opentrep/OPENTREP_Abstract.hpp>
#include <opentrep/LocationList.hpp>

namespace OPENTREP {

  /**
   * @brief List of names for a given (geographical) location. 
   */
  typedef std::list<std::string> LocationNameList_T;

  
  /**
   * @brief Structure modelling a (geographical) location. 
   */
  struct Location : public OPENTREP_Abstract {
  public:
    // ///////// Getters ////////
    /** 
     * Get the IATA code. 
     */
    const std::string& getIataCode() const {
      return _iataCode;
    }
    
    /** 
     * Get the ICAO code. 
     */
    const std::string& getIcaoCode() const {
      return _icaoCode;
    }
    
    /**
     * Get the Geonames ID.
     */
    const GeonamesID_T& getGeonamesID() const {
      return _geonameID;
    }

    /** 
     * Get the FAA code. 
     */
    const std::string& getFaaCode() const {
      return _faaCode;
    }
    
    /**
     * Get the City code. 
     */
    const std::string& getCityCode() const {
      return _cityCode;
    }
    
    /**
     * Get the State code. 
     */
    const std::string& getStateCode() const {
      return _stateCode;
    }
    
    /**
     * Get the Country code. 
     */
    const std::string& getCountryCode() const {
      return _countryCode;
    }
    
    /**
     * Get the Region code. 
     */
    const std::string& getRegionCode() const {
      return _regionCode;
    }
    
    /**
     * Get the Time-zone group. 
     */
    const std::string& getTimeZoneGroup() const {
      return _timeZoneGroup;
    }
    
    /**
     * Get the Latitude. 
     */
    const double& getLatitude() const {
      return _latitude;
    }

    /**
     * Get the Longitude. 
     */
    const double& getLongitude() const {
      return _longitude;
    }
    
    /**
     * Get the PageRank/importance. 
     */
    const double& getPageRank() const {
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
    const std::string& getWikiLink() const {
      return _wikiLink;
    }

    /**
     * Get the list of (American) English names for that location. 
     */
    const LocationNameList_T& getNameList () const {
      return _nameList;
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
     * Set the IATA code. 
     */
    void setIataCode (const std::string& iIataCode) {
      _iataCode = iIataCode;
    }
    
    /**
     * Set the ICAO code. 
     */
    void setIcaoCode (const std::string& iIcaoCode) {
      _icaoCode = iIcaoCode;
    }
    
    /**
     * Get the Geonames ID.
     */
    void setGeonamesID (const GeonamesID_T& iGeonamesID) {
      _geonameID = iGeonamesID;
    }

    /**
     * Set the FAA code. 
     */
    void setFaaCode (const std::string& iFaaCode) {
      _faaCode = iFaaCode;
    }
    
    /**
     * Set the City code. 
     */
    void setCityCode (const std::string& iCityCode) {
      _cityCode = iCityCode;
    }
    
    /**
     * Set the State code. 
     */
    void setStateCode (const std::string& iStateCode) {
      _stateCode = iStateCode;
    }
    
    /**
     * Set the Country code. 
     */
    void setCountryCode (const std::string& iCountryCode) {
      _countryCode = iCountryCode;
    }
    
    /**
     * Set the Region code. 
     */
    void setRegionCode (const std::string& iRegionCode) {
      _regionCode = iRegionCode;
    }
    
    /**
     * Set the Time-zone group. 
     */
    void setTimeZoneGroup (const std::string& iTimeZoneGroup) {
      _timeZoneGroup = iTimeZoneGroup;
    }
    
    /**
     * Set the Latitude.
     */
    void setLatitude (const double& iLatitude) {
      _latitude = iLatitude;
    }

    /**
     * Set the Longitude.
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
     * Set the modification date. 
     */
    void setModificationDate (const Date_T& iModDate) {
      _modificationDate = iModDate;
    }

    /**
     * Set the Wikipedia link.
     */
    void setWikiLink (const std::string& iWikiLink) {
      _wikiLink = iWikiLink;
    }
    
    /**
     * Set the list of (American) English names for that location. 
     */
    void setNameList (const LocationNameList_T& iNameList) {
      _nameList = iNameList;
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
    Location (const std::string& iIataCode, const std::string& iIcaoCode,
              const GeonamesID_T& iGeonamesID, const std::string& iFaaCode,
              const std::string& iCityCode, const std::string& iStateCode,
              const std::string& iCountryCode, const std::string& iRegionCode,
              const std::string& iTimeZoneGroup, 
              const double iLatitude, const double iLongitude,
              const double iPageRank, const std::string& iWikiLink,
              const LocationNameList_T& iNameList,
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
    // \note The combination (IATA code, ICAO code, GeonamesID) forms a
    //       a primary key.
    /**
     * IATA code (e.g., ORD, CHI, SFO, LAX, LHR, LON, CDG, PAR).
     */
    std::string _iataCode;

    /**
     * ICAO code (e.g., KORD).
     */
    std::string _icaoCode;

    /**
     * Geonames ID (e.g., 4887479).
     */
    GeonamesID_T _geonameID;

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
     * Modification date
     */
    Date_T _modificationDate;

    /**
     * Link on the Wikipedia entry
     */
    std::string _wikiLink;

    /**
     * List of (American) English names. 
     */
    LocationNameList_T _nameList;

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
    /** Staging Date. */
    year_t _itYear;
    month_t _itMonth;
    day_t _itDay;
  };

}
#endif // __OPENTREP_LOCATION_HPP
