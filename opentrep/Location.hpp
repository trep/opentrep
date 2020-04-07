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
#include <opentrep/Language.hpp>
#include <opentrep/LocationKey.hpp>
#include <opentrep/NameMatrix.hpp>
#include <opentrep/LocationList.hpp>
#include <opentrep/CityDetailsList.hpp>

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
     * Get the IATA location type (e.g., A for airport).
     */
    const IATAType& getIataType() const {
      return _key.getIataType();
    }

    /**
     * Get the Geonames ID.
     */
    const GeonamesID_T& getGeonamesID() const {
      return _key.getGeonamesID();
    }

    /**
     * State whether that POR is referenced by Geonames.
     */
    const IsGeonames_T& isGeonames() const {
      return _key.isGeonames();
    }

    /**
     * Get the ICAO code.
     */
    const ICAOCode_T& getIcaoCode() const {
      return _icaoCode;
    }
    
    /** 
     * Get the FAA code. 
     */
    const FAACode_T& getFaaCode() const {
      return _faaCode;
    }
    
    /**
     * Get the list of UN/LOCODE five-letter codes.
     */
    const UNLOCodeList_T& getUNLOCodeList() const {
      return _unLOCodeList;
    }

    /**
     * Get the list of UIC codes.
     */
    const UICCodeList_T& getUICCodeList() const {
      return _uicCodeList;
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
     * Get the short list of alternate names (without language codes).
     */
    const AltNameShortListString_T& getAltNameShortListString() const {
      return _altNameShortListString;
    }

    /** 
     * Get the list of IATA codes of the travel-related POR.
     */
    const TvlPORListString_T& getTvlPORListString() const {
      return _tvlPORListString;
    }

    /**
     * Get the envelope ID.
     */
    const EnvelopeID_T& getEnvelopeID() const {
      return _envelopeID;
    }

    /**
     * Get the beginning date of the validity period
     */
    const Date_T& getDateFrom() const {
      return _dateFrom;
    }

    /**
     * Get the end date of the validity period
     */
    const Date_T& getDateEnd() const {
      return _dateEnd;
    }

    /**
     * Get the commentaries.
     */
    const Comment_T& getComment() const {
      return _comment;
    }

    /** 
     * Get the list of served cities.
     */
    const CityDetailsList_T& getCityList() const {
      return _cityList;
    }

    /**
     * Get the state code.
     */
    const StateCode_T& getStateCode() const {
      return _stateCode;
    }
    
    /**
     * Get the country code. 
     */
    const CountryCode_T& getCountryCode() const {
      return _countryCode;
    }
    
    /**
     * Get the alternative country code. 
     */
    const AltCountryCode_T& getAltCountryCode() const {
      return _altCountryCode;
    }
    
    /**
     * Get the country name.
     */
    const CountryName_T& getCountryName() const {
      return _countryName;
    }
    
    /**
     * Get the US DOT World Area Code (WAC).
     */
    const WAC_T& getWAC() const {
      return _wac;
    }
    
    /**
     * Get the US DOT World Area Code (WAC) name.
     */
    const WACName_T& getWACName() const {
      return _wacName;
    }
    
    /**
     * Get the currency code.
     */
    const CurrencyCode_T& getCurrencyCode() const {
      return _currencyCode;
    }
    
    /**
     * Get the continent code.
     */
    const ContinentCode_T& getContinentCode() const {
      return _continentCode;
    }
    
    /**
     * Get the continent name.
     */
    const ContinentName_T& getContinentName() const {
      return _continentName;
    }
    
    /**
     * Get the time-zone.
     */
    const TimeZone_T& getTimeZone() const {
      return _timeZone;
    }
    
    /**
     * Get the GMT offset (e.g., 1)
     */
    const GMTOffset_T& getGMTOffset() const {
      return _gmtOffset;
    }

    /**
     * Get the DST offset (e.g., 2)
     */
    const DSTOffset_T& getDSTOffset() const {
      return _dstOffset;
    }

    /**
     * Get the raw offset (e.g., 1)
     */
    const RawOffset_T& getRawOffset() const {
      return _rawOffset;
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
     * Get the feature class (e.g., S for spot).
     */
    const FeatureClass_T& getFeatureClass() const {
      return _featClass;
    }

    /**
     * Get the feature code (e.g., AIRP for airport).
     */
    const FeatureCode_T& getFeatureCode() const {
      return _featCode;
    }

    /**
     * Get the administrative code of rank 1 (e.g., IL for Illinois).
     */
    const Admin1Code_T& getAdmin1Code() const {
      return _admin1Code;
    }

    /**
     * Get the administrative UTF8 name of rank 1 (e.g., Illinois).
     */
    const Admin1UTFName_T& getAdmin1UtfName() const {
      return _admin1UtfName;
    }

    /**
     * Get the administrative ASCII name of rank 1 (e.g., Illinois).
     */
    const Admin1ASCIIName_T& getAdmin1AsciiName() const {
      return _admin1AsciiName;
    }

    /**
     * Get the administrative code of rank 2.
     */
    const Admin2Code_T& getAdmin2Code() const {
      return _admin2Code;
    }

    /**
     * Get the administrative UTF8 name of rank 2 (e.g., Alpes-Maritimes).
     */
    const Admin2UTFName_T& getAdmin2UtfName() const {
      return _admin2UtfName;
    }

    /**
     * Get the administrative ASCII name of rank 2 (e.g., Alpes-Maritimes).
     */
    const Admin2ASCIIName_T& getAdmin2AsciiName() const {
      return _admin2AsciiName;
    }

    /**
     * Get the administrative code of rank 3.
     */
    const Admin3Code_T& getAdmin3Code() const {
      return _admin3Code;
    }

    /**
     * Get the administrative code of rank 4.
     */
    const Admin4Code_T& getAdmin4Code() const {
      return _admin4Code;
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
     * Get the Geonames latitude.
     */
    const Latitude_T& getGeonameLatitude() const {
      return _geonameLatitude;
    }

    /**
     * Get the Geonames longitude.
     */
    const Longitude_T& getGeonameLongitude() const {
      return _geonameLongitude;
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
     * @param const LanguageCode_T& Language code.
     * @param NameList_T& Empty list of names, which will be filled by the
     *        method if a list exists for that language code.
     * @return bool Whether or not such a list exists for the given
     *         language.
     */
    bool getNameList (const LanguageCode_T& iLanguageCode,
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

    /**
     * Get the raw data string, as stored and retrieved by Xapian.
     */
    const RawDataString_T& getRawDataString() const {
      return _rawDataString;
    }


  public:
    // ///////////////////// Setters //////////////////////
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
     * Set the IATA location type (e.g., A for airport).
     */
    void setIataType (const IATAType& iIATAType) {
      _key.setIataType (iIATAType);
    }

    /**
     * Set the Geonames ID.
     */
    void setGeonamesID (const GeonamesID_T& iGeonamesID) {
      _key.setGeonamesID (iGeonamesID);
    }

    /**
     * Set the ICAO code.
     */
    void setIcaoCode (const std::string& iIcaoCode) {
      _icaoCode = ICAOCode_T (iIcaoCode);
    }
    
    /**
     * Set the FAA code. 
     */
    void setFaaCode (const std::string& iFaaCode) {
      _faaCode = FAACode_T (iFaaCode);
    }
    
    /**
     * Add a UN/LOCODE five-letter code to the underlying list.
     */
    void addUNLOCode (const UNLOCode_T& iUNLOCode) {
      _unLOCodeList.push_back (iUNLOCode);
    }

    /**
     * Add a UIC code to the underlying list.
     */
    void addUICCode (const UICCode_T& iUICCode) {
      _uicCodeList.push_back (iUICCode);
    }

    /** 
     * Set the common name (usually in American English, but not necessarily
     * in ASCII).
     */
    void setCommonName (const std::string& iName) {
      _commonName = CommonName_T (iName);
    }
    
    /** 
     * Set the ASCII name (not necessarily in English).
     */
    void setAsciiName (const std::string& iName) {
      _asciiName = ASCIIName_T (iName);
    }
    
    /** 
     * Set the short list of alternate names (without language codes).
     */
    void setAltNameShortListString (const std::string& iNameListString) {
      _altNameShortListString = AltNameShortListString_T (iNameListString);
    }

    /** 
     * Set the list of IATA codes of the travel-related POR.
     */
    void setTvlPORListString (const std::string& iPORListString) {
      _tvlPORListString = TvlPORListString_T (iPORListString);
    }

    /**
     * Set the envelope ID.
     */
    void setEnvelopeID (const EnvelopeID_T& iEnvelopeID) {
      _envelopeID = iEnvelopeID;
    }

    /**
     * Set the beginning date of the validity period
     */
    void setDateFrom (const Date_T& iDate) {
      _dateFrom = iDate;
    }

    /**
     * Set the end date of the validity period
     */
    void setDateEnd (const Date_T& iDate) {
      _dateEnd = iDate;
    }

    /**
     * Set the commentaries.
     */
    void setComment (const std::string& iComment) {
      _comment = Comment_T (iComment);
    }

    /** 
     * Set the list of served cities.
     */
    void setCityList (const CityDetailsList_T& iCityList) {
      _cityList = iCityList;
    }

    /**
     * Set the country code. 
     */
    void setCountryCode (const std::string& iCountryCode) {
      _countryCode = CountryCode_T (iCountryCode);
    }
    
    /**
     * Set the alternative country code. 
     */
    void setAltCountryCode (const std::string& iCountryCode) {
      _altCountryCode = AltCountryCode_T (iCountryCode);
    }
    
    /**
     * Set the country name.
     */
    void setCountryName (const std::string& iCountryName) {
      _countryName = CountryName_T (iCountryName);
    }
    
    /**
     * Set the state code. 
     */
    void setStateCode (const std::string& iStateCode) {
      _stateCode = StateCode_T (iStateCode);
    }
    
    /**
     * Set the US DOT World Area Code (WAC).
     */
    void setWAC (const WAC_T& iWAC) {
      _wac = iWAC;
    }
    
    /**
     * Set the US DOT World Area Code (WAC) name.
     */
    void setWACName (const std::string& iWACName) {
      _wacName = WACName_T (iWACName);
    }
    
    /**
     * Set the currency code.
     */
    void setCurrencyCode (const std::string& iCurrencyCode) {
      _currencyCode = CurrencyCode_T (iCurrencyCode);
    }
    
    /**
     * Set the continent code.
     */
    void setContinentCode (const std::string& iContinentCode) {
      _continentCode = ContinentCode_T (iContinentCode);
    }
    
    /**
     * Set the continent name. 
     */
    void setContinentName (const std::string& iContinentName) {
      _continentName = ContinentName_T (iContinentName);
    }
    
    /**
     * Set the time-zone. 
     */
    void setTimeZone (const std::string& iTimeZone) {
      _timeZone = TimeZone_T (iTimeZone);
    }
    
    /**
     * Set the GMT offset (e.g., 1)
     */
    void setGMTOffset (const GMTOffset_T& iOffset) {
      _gmtOffset = iOffset;
    }

    /**
     * Set the DST offset (e.g., 2)
     */
    void setDSTOffset (const DSTOffset_T& iOffset) {
      _dstOffset = iOffset;
    }

    /**
     * Set the raw offset (e.g., 1)
     */
    void setRawOffset (const RawOffset_T& iOffset) {
      _rawOffset = iOffset;
    }

    /**
     * Set the geographical latitude.
     */
    void setLatitude (const Latitude_T& iLatitude) {
      _latitude = iLatitude;
    }

    /**
     * Set the geographical longitude.
     */
    void setLongitude (const Longitude_T& iLongitude) {
      _longitude = iLongitude;
    }
    
    /**
     * Set the feature class (e.g., S for spot).
     */
    void setFeatureClass (const std::string& iFeatClass) {
      _featClass = FeatureClass_T (iFeatClass);
    }

    /**
     * Set the feature code (e.g., AIRP for airport).
     */
    void setFeatureCode (const std::string& iFeatCode) {
      _featCode = FeatureCode_T (iFeatCode);
    }

    /**
     * Set the administrative code of rank 1 (e.g., IL for Illinois).
     */
    void setAdmin1Code (const std::string& iAdminCode) {
      _admin1Code = Admin1Code_T (iAdminCode);
    }

    /**
     * Set the administrative UTF8 name of rank 1 (e.g., Illinois).
     */
    void setAdmin1UtfName (const std::string& iAdminName) {
      _admin1UtfName = Admin1UTFName_T (iAdminName);
    }

    /**
     * Set the administrative ASCII name of rank 1 (e.g., Illinois).
     */
    void setAdmin1AsciiName (const std::string& iAdminName) {
      _admin1AsciiName = Admin1ASCIIName_T (iAdminName);
    }

    /**
     * Set the administrative code of rank 2.
     */
    void setAdmin2Code (const std::string& iAdminCode) {
      _admin2Code = Admin2Code_T (iAdminCode);
    }

    /**
     * Set the administrative UTF8 name of rank 2 (e.g., Alpes-Maritimes).
     */
    void setAdmin2UtfName (const std::string& iAdminName) {
      _admin2UtfName = Admin2UTFName_T (iAdminName);
    }

    /**
     * Set the administrative ASCII name of rank 2 (e.g., Alpes-Maritimes).
     */
    void setAdmin2AsciiName (const std::string& iAdminName) {
      _admin2AsciiName = Admin2ASCIIName_T (iAdminName);
    }

    /**
     * Set the administrative code of rank 3.
     */
    void setAdmin3Code (const std::string& iAdminCode) {
      _admin3Code = Admin3Code_T (iAdminCode);
    }

    /**
     * Set the administrative code of rank 4.
     */
    void setAdmin4Code (const std::string& iAdminCode) {
      _admin4Code = Admin4Code_T (iAdminCode);
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
     * Set the Geonames latitude.
     */
    void setGeonameLatitude (const Latitude_T& iLatitude) {
      _geonameLatitude = iLatitude;
    }

    /**
     * Set the Geonames longitude.
     */
    void setGeonameLongitude (const Longitude_T& iLongitude) {
      _geonameLongitude = iLongitude;
    }
    
    /**
     * Add a name for that location.
     *
     * @param const LanguageCode_T& Language in which to add the name.
     * @param const std::string& Name to be added.
     */
    void addName (const LanguageCode_T& iLanguageCode,
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
    
    /**
     * Set the raw data string, as stored and retrieved by Xapian.
     */
    void setRawDataString (const std::string& iRawDataString) {
      _rawDataString = RawDataString_T (iRawDataString);
    }


  public:
    // ///////// Parsing support methods ////////
    /**
     * Calculate the date from the staging details.
     */
    Date_T calculateDate() const;

    /**
     * Add the details of the served city into the dedicated list.
     *
     * The staging _itCityXxx items are used to create a CityDetails object,
     * which is then added (copied) into the underlying _cityList member object.
     */
    void consolidateCityDetailsList();

    /**
     * Aggregate the temporary alternate names into the short list of alternate
     * names.
     */
    void consolidateAltNameShortListString();

    /**
     * Aggregate the temporary IATA codes into the list of travel-related POR.
     */
    void consolidateTvlPORListString();

    /**
     * Derive a list from a (Geonames) feature code. For instance, the 'AIRP'
     * feature code provides:
     * <ul>
     *   <li>airport</li>
     *   <li>airdrome</li>
     *   <li>aerodrome</li>
     *   <li>airfield</li>
     *   <li>airstrip</li>
     *   <li>airbase</li>
     * </ul>
     *
     * @param const FeatureCode_T& The feature code.
     * @return const FeatureNameList_T The list of feature names.
     */
    static FeatureNameList_T getFeatureList (const FeatureCode_T&);


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

    /** 
     * Display of the list of UN/LOCODE five-letter codes.
     *
     * For instance,
     * for [Atlantic City](http://www.geonames.org/4500546),
     * New Jersey (NJ), United States (US), it displays "USACX,USAIY"
     */
    std::string describeUNLOCodeList() const;

    /** 
     * Display of the list of UIC codes.
     *
     * For instance,
     * for [Gare de Grenoble](http://www.geonames.org/8288215),
     * Auvergne-Rhône-Alpes (ARA), France (FR), it displays "87747006"
     */
    std::string describeUICCodeList() const;

    /** 
     * Display of the served cities.
     *
     * For instance,
     * for [Bradley International Airport](http://www.geonames.org/5282636),
     * Connecticut (CT), United States (US), it displays "HFD,BDL,SFY"
     */
    std::string describeCityDetailsList() const;
    
  public:
    /**
     * Main Constructor. 
     */
    Location (const IATACode_T&, const IATAType&, const GeonamesID_T&,
              const ICAOCode_T&, const FAACode_T&,
              const CommonName_T&, const ASCIIName_T&,
              const EnvelopeID_T&,
              const Date_T& iDateFrom, const Date_T& iDateEnd,
              const Comment_T&,
              const CityDetailsList_T&,
              const StateCode_T&,
              const CountryCode_T&, const AltCountryCode_T&,
              const CountryName_T&, const WAC_T&, const WACName_T&,
              const CurrencyCode_T&,
              const ContinentName_T&,
              const Latitude_T&, const Longitude_T&,
              const FeatureClass_T&, const FeatureCode_T&,
              const Admin1Code_T&, const Admin1UTFName_T&,
              const Admin1ASCIIName_T&,
              const Admin2Code_T&, const Admin2UTFName_T&,
              const Admin2ASCIIName_T&,
              const Admin3Code_T&, const Admin4Code_T&,
              const Population_T&, const Elevation_T&, const GTopo30_T&,
              const TimeZone_T&,
              const GMTOffset_T&, const DSTOffset_T&, const RawOffset_T&,
              const Date_T& iModDate, const TvlPORListString_T&,
              const WikiLink_T&,
              const Latitude_T&, const Longitude_T&,
              const PageRank_T&,
              const std::string& iOriginalKeywords,
              const std::string& iCorrectedKeywords,
              const MatchingPercentage_T& iPercentage,
              const NbOfErrors_T& iEditDistance,
              const NbOfErrors_T& iAllowableEditDistance,
              const RawDataString_T&);

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
     * Primary key, composed of:
     * <ul>
     *   <li>IATA 3-letter code. See IATACode_T for more details.</li>
     *   <li>Travel type. See IATAType for more details.</li>
     *   <li>Geonames ID, when referenced by Geonames (0 otherwise).
     *       See GeonamesID_T for more details.</li>
     * </ul>
     */
    LocationKey _key;

    /**
     * ICAO code (e.g., KORD).
     *
     * See ICAOCode_T for more details.
     */
    ICAOCode_T _icaoCode;

    /**
     * FAA code (e.g., ORD).
     *
     * See FAACode_T for more details.
     */
    FAACode_T _faaCode;

    /**
     * List of UN/LOCODE five-letter codes (e.g., USACX and USAIY).
     *
     *
     * See UNLOCode_T for more details.
     */
    UNLOCodeList_T _unLOCodeList;

    /**
     * List of UIC codes (e.g., 87747006).
     *
     *
     * See UICCode_T for more details.
     */
    UICCodeList_T _uicCodeList;

    /** 
     * Common name (usually in American English, but not necessarily in ASCII).
     */
    CommonName_T _commonName;
    
    /** 
     * ASCII name (not necessarily in English).
     */
    ASCIIName_T _asciiName;
    
    /** 
     * Short list of alternate names, without the language codes (e.g.,
     * "Aéroport de Nice Côte d'Azur,Nice Airport,Flughafen Nizza"),
     * aggregated within a single string.
     */
    AltNameShortListString_T _altNameShortListString;

    /** 
     * List of the IATA codes of the travel-related POR, aggregated within
     * a single string (e.g., "EMB,JCC,SFO" for San Francisco, California, USA).
     */
    TvlPORListString_T _tvlPORListString;

    /**
     * Envelope ID.
     * <ul>
     *   <li>It is empty or null when valid/current.</li>
     *   <li>It has a value from 1 to positive infinite,
     *       when it used to exist, but no longer exists.</li>
     * </ul>
     */
    EnvelopeID_T _envelopeID;

    /**
     * Beginning date of the validity period
     */
    Date_T _dateFrom;

    /**
     * End date of the validity period
     */
    Date_T _dateEnd;

    /**
     * Commentaries.
     */
    Comment_T _comment;

    /** 
     * List of the served cities (e.g., HFD, BDL and SFY for
     * [Bradley International Airport](http://www.geonames.org/5282636),
     * Connecticut (CT), United States (US)).
     */
    CityDetailsList_T _cityList;

    /**
     * State code (e.g., IL).
     */
    StateCode_T _stateCode;

    /**
     * Country code (e.g., US).
     *
     * See CountryCode_T for more details.
     */
    CountryCode_T _countryCode;

    /**
     * Alternative country code (e.g., US).
     *
     * See AltCountryCode_T for more details.
     */
    AltCountryCode_T _altCountryCode;

    /**
     * Country name (e.g., Germany).
     */
    CountryName_T _countryName;

    /**
     * US DOT World Area Code (WAC) (e.g., 1 for Alaska, US, or 427 for France).
     *
     * See WAC_T for more details.
     */
    WAC_T _wac;

    /**
     * US DOT World Area Code (WAC) name (e.g., Alaska).
     */
    WACName_T _wacName;

    /**
     * Currency code (e.g., USD, EUR).
     *
     * See CurrencyCode_T for more details.
     */
    CurrencyCode_T _currencyCode;

    /**
     * Continent code (e.g., SA for South America).
     */
    ContinentCode_T _continentCode;

    /**
     * Continent name (e.g., North America).
     */
    ContinentName_T _continentName;

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
     *
     * See FeatureClass_T for more details.
     */
    FeatureClass_T _featClass;

    /**
     * Feature code (e.g., AIRP for airport).
     *
     * See FeatureCode_T for more details.
     */
    FeatureCode_T _featCode;

    /**
     * Administrative code of rank 1 (e.g., IL for Illinois).
     *
     * See Admin1Code_T for more details.
     */
    Admin1Code_T _admin1Code;

    /**
     * Administrative UTF8 name of rank 1 (e.g., Illinois).
     */
    Admin1UTFName_T _admin1UtfName;

    /**
     * Administrative ASCII name of rank 1 (e.g., Illinois).
     */
    Admin1ASCIIName_T _admin1AsciiName;

    /**
     * Administrative code of rank 2.
     *
     * See Admin2Code_T for more details.
     */
    Admin2Code_T _admin2Code;

    /**
     * Administrative UTF8 name of rank 2 (e.g., Alpes-Maritimes).
     */
    Admin2UTFName_T _admin2UtfName;

    /**
     * Administrative ASCII name of rank 2 (e.g., Alpes-Maritimes).
     */
    Admin2ASCIIName_T _admin2AsciiName;

    /**
     * Administrative code of rank 3.
     *
     * See Admin3Code_T for more details.
     */
    Admin3Code_T _admin3Code;

    /**
     * Administrative code of rank 4.
     *
     * See Admin4Code_T for more details.
     */
    Admin4Code_T _admin4Code;

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
     * Link on the Wikipedia entry
     */
    WikiLink_T _wikiLink;

    /**
     * Geonames latitude (e.g., 41.978603).
     */
    Latitude_T _geonameLatitude;

    /**
     * Geonames longitude (e.g., -87.904842).
     */
    Longitude_T _geonameLongitude;

    /**
     * PageRank/importance (e.g., ATL is 94.66% and BSL is 8.14%).
     *
     * See PageRank_T for more details.
     *
     * When the location is not referenced (e.g., by any schedule),
     * its PageRank/importance should be null (equal to zero).
     * But, as in some circonstances, it is required to have a
     * non-zero PageRank/importance, it is set to a pre-set value,
     * i.e., K_DEFAULT_PAGE_RANK (e.g., 0.1% == 0.001). Hence, if the
     * PageRank/importance value is equal to K_DEFAULT_PAGE_RANK,
     * it can be considered as to be equivalent to zero.
     */
    PageRank_T _pageRank;

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

    /**
     * Raw data string, before any parsing. It is used to be re-parsed once
     * retrieved by Xapian as document data.
     */
    RawDataString_T _rawDataString;


  public:
    // ///////// Parsing support temporary attributes ////////
    /**
     * Staging Date.
     */
    year_t _itYear;
    month_t _itMonth;
    day_t _itDay;

    /**
     * Staging alternate names.
     */
    Language::EN_Language _itLanguageCodeNum;
    LanguageCode_T _itLanguageCode;
    std::string _itLangCode2Char;
    std::string _itLangCodeExt;
    std::string _itLangCodeHist;
    AltNameShortList_T _itAltNameShortList;

    /**
     * Staging travel-related POR
     */
    IATACodeList_T _itTvlPORList;

    /**
     * Staging served cities
     */
    std::string _itCityIataCode;
    GeonamesID_T _itCityGeonamesID;
    std::string _itCityUtfName;
    std::string _itCityAsciiName;
    std::string _itCityCountryCode;
    std::string _itCityStateCode;
  };

}
#endif // __OPENTREP_LOCATION_HPP
