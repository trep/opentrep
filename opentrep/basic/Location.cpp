// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <sstream>
#include <string>
#include <list>
// OpenTrep
#include <opentrep/basic/BasConst_General.hpp>
#include <opentrep/IATAType.hpp>
#include <opentrep/Location.hpp>
#include <opentrep/CityDetails.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  Location::Location() :
    _key (IATACode_T (""), IATAType::LAST_VALUE, 0),
    _icaoCode (ICAOCode_T ("")), _faaCode (FAACode_T ("")),
    _unLOCodeList(), _uicCodeList(),
    _commonName (CommonName_T ("NotAvailable")),
    _asciiName (ASCIIName_T ("NotAvailable")),
    _altNameShortListString (AltNameShortListString_T ("NotAvailable")),
    _tvlPORListString (TvlPORListString_T ("")),
    _envelopeID (0),
    _dateFrom (1970, 01, 01), _dateEnd (2999, 12, 31), _comment (""),
    _stateCode (StateCode_T ("NA")), _countryCode (CountryCode_T ("NA")),
    _altCountryCode (AltCountryCode_T ("")), _countryName (CountryName_T ("NA")),
    _wac (0), _wacName (WACName_T ("")),
    _currencyCode ("ZZZ"),
    _continentCode (ContinentCode_T ("NA")),
    _continentName (ContinentName_T ("NotAvailable")),
    _latitude (0.0), _longitude (0.0),
    _featClass (FeatureClass_T ("Z")), _featCode (FeatureCode_T ("ZZZZ")),
    _admin1Code (Admin1Code_T ("")),
    _admin1UtfName (Admin1UTFName_T ("")),
    _admin1AsciiName (Admin1ASCIIName_T ("")),
    _admin2Code (Admin2Code_T ("")),
    _admin2UtfName (Admin2UTFName_T ("")),
    _admin2AsciiName (Admin2ASCIIName_T ("")),
    _admin3Code (Admin3Code_T ("Z")), _admin4Code (Admin4Code_T ("")),
    _population (0), _elevation (0), _gTopo30 (0),
    _timeZone (TimeZone_T ("NA")),
    _gmtOffset (0), _dstOffset (0), _rawOffset (0),
    _modificationDate (2000, 01, 01),
    _wikiLink (WikiLink_T ("http://en.wikipedia.org")),
    _geonameLatitude (0.0), _geonameLongitude (0.0),
    _pageRank (K_DEFAULT_PAGE_RANK),
    _originalKeywords ("NA"), _correctedKeywords ("NA"),
    _percentage (0), _editDistance (0), _allowableEditDistance (0),
    _rawDataString ("NA"),
    _itLanguageCodeNum (Language::LAST_VALUE),
    _itLanguageCode (LanguageCode_T ("")) {
  }

  // //////////////////////////////////////////////////////////////////////
  Location::Location (const Location& iLocation) :
    _key (iLocation._key),
    _icaoCode (iLocation._icaoCode), _faaCode (iLocation._faaCode),
    _unLOCodeList (iLocation._unLOCodeList),
    _uicCodeList (iLocation._uicCodeList),
    _commonName (iLocation._commonName), _asciiName (iLocation._asciiName),
    _altNameShortListString (iLocation._altNameShortListString),
    _tvlPORListString (iLocation._tvlPORListString),
    _envelopeID (iLocation.getEnvelopeID()),
    _dateFrom (iLocation.getDateFrom()), _dateEnd (iLocation.getDateEnd()),
    _comment (iLocation.getComment()),
    _cityList (iLocation._cityList),
    _stateCode (iLocation._stateCode), _countryCode (iLocation._countryCode),
    _altCountryCode (iLocation._altCountryCode),
    _countryName (iLocation._countryName),
    _wac (iLocation._wac), _wacName (iLocation._wacName),
    _currencyCode (iLocation._currencyCode),
    _continentCode (iLocation._continentCode),
    _continentName (iLocation._continentName),
    _latitude (iLocation._latitude), _longitude (iLocation._longitude),
    _featClass (iLocation._featClass), _featCode (iLocation._featCode),
    _admin1Code (iLocation._admin1Code),
    _admin1UtfName (iLocation._admin1UtfName),
    _admin1AsciiName (iLocation._admin1AsciiName),
    _admin2Code (iLocation._admin2Code),
    _admin2UtfName (iLocation._admin2UtfName),
    _admin2AsciiName (iLocation._admin2AsciiName),
    _admin3Code (iLocation._admin3Code), _admin4Code (iLocation._admin4Code),
    _population (iLocation._population), _elevation (iLocation._elevation),
    _gTopo30 (iLocation._gTopo30),
    _timeZone (iLocation._timeZone), _gmtOffset (iLocation._gmtOffset),
    _dstOffset (iLocation._dstOffset), _rawOffset (iLocation._rawOffset),
    _modificationDate (iLocation._modificationDate),
    _wikiLink (iLocation._wikiLink),
    _geonameLatitude (iLocation._geonameLatitude),
    _geonameLongitude (iLocation._geonameLongitude),
    _pageRank (iLocation._pageRank),
    _nameMatrix (iLocation._nameMatrix),
    _originalKeywords (iLocation._originalKeywords),
    _correctedKeywords (iLocation._correctedKeywords),
    _percentage (iLocation._percentage),
    _editDistance (iLocation._editDistance),
    _allowableEditDistance (iLocation._allowableEditDistance),
    _extraLocationList (iLocation._extraLocationList),
    _alternateLocationList (iLocation._alternateLocationList),
    _rawDataString (iLocation._rawDataString),
    _itLanguageCodeNum (iLocation._itLanguageCodeNum),
    _itLanguageCode (LanguageCode_T ("")) { 
  }
    
  // //////////////////////////////////////////////////////////////////////
  Location::Location (const IATACode_T& iIataCode, const IATAType& iIataType,
                      const GeonamesID_T& iGeonamesID,
                      const ICAOCode_T& iIcaoCode, const FAACode_T& iFaaCode,
                      const CommonName_T& iCommonName,
                      const ASCIIName_T& iASCIIName,
                      const EnvelopeID_T& iEnvelopeID,
                      const Date_T& iDateFrom, const Date_T& iDateEnd,
                      const Comment_T& iComment,
                      const CityDetailsList_T& iCityDetailsList,
                      const StateCode_T& iStateCode,
                      const CountryCode_T& iCountryCode,
                      const AltCountryCode_T& iAltCountryCode,
                      const CountryName_T& iCountryName,
                      const WAC_T& iWAC, const WACName_T& iWACName,
                      const CurrencyCode_T& iCurrencyCode,
                      const ContinentName_T& iContinentName,
                      const Latitude_T& iLatitude, const Longitude_T& iLongitude,
                      const FeatureClass_T& iFeatureClass,
                      const FeatureCode_T& iFeatureCode,
                      const Admin1Code_T& iAdmin1Code,
                      const Admin1UTFName_T& iAdmin1UtfName,
                      const Admin1ASCIIName_T& iAdmin1AsciiName,
                      const Admin2Code_T& iAdmin2Code,
                      const Admin2UTFName_T& iAdmin2UtfName,
                      const Admin2ASCIIName_T& iAdmin2AsciiName,
                      const Admin3Code_T& iAdmin3Code,
                      const Admin4Code_T& iAdmin4Code,
                      const Population_T& iPopulation,
                      const Elevation_T& iElevation, const GTopo30_T& iGTopo30,
                      const TimeZone_T& iTimeZone,
                      const GMTOffset_T& iGMTOffset,
                      const DSTOffset_T& iDSTOffset,
                      const RawOffset_T& iRawOffset,
                      const Date_T& iModDate,
                      const TvlPORListString_T& iTvlPORListString,
                      const WikiLink_T& iWikiLink, const PageRank_T& iPageRank,
                      const Latitude_T& iGeonameLatitude,
                      const Longitude_T& iGeonameLongitude,
                      const std::string& iOriginalKeywords,
                      const std::string& iCorrectedKeywords,
                      const MatchingPercentage_T& iPercentage,
                      const NbOfErrors_T& iEditDistance,
                      const NbOfErrors_T& iAllowableEditDistance,
                      const RawDataString_T& iRawDataString) :
    _key (iIataCode, iIataType, iGeonamesID),
    _icaoCode (iIcaoCode), _faaCode (iFaaCode),
    _commonName (iCommonName),_asciiName (iASCIIName),
    _altNameShortListString (AltNameShortListString_T ("NotAvailable")),
    _tvlPORListString (iTvlPORListString),
    _envelopeID (iEnvelopeID),
    _dateFrom (iDateFrom), _dateEnd (iDateEnd),
    _comment (iComment),
    _cityList (iCityDetailsList),
    _stateCode (iStateCode),
    _countryCode (iCountryCode), _altCountryCode (iAltCountryCode),
    _countryName (iCountryName),
    _wac (iWAC), _wacName (iWACName),
    _currencyCode (iCurrencyCode),
    _continentCode ("NA"), _continentName (iContinentName),
    _latitude (iLatitude), _longitude (iLongitude),
    _featClass (iFeatureClass), _featCode (iFeatureCode),
    _admin1Code (iAdmin1Code),
    _admin1UtfName (Admin1UTFName_T (iAdmin1UtfName)),
    _admin1AsciiName (Admin1ASCIIName_T (iAdmin1AsciiName)),
    _admin2Code (iAdmin2Code),
    _admin2UtfName (Admin2UTFName_T (iAdmin2UtfName)),
    _admin2AsciiName (Admin2ASCIIName_T (iAdmin2AsciiName)),
    _admin3Code (iAdmin3Code), _admin4Code (iAdmin4Code),
    _population (iPopulation), _elevation (iElevation), _gTopo30 (iGTopo30),
    _timeZone (iTimeZone),
    _gmtOffset (iGMTOffset), _dstOffset (iDSTOffset), _rawOffset (iRawOffset),
    _modificationDate (iModDate),
    _wikiLink (iWikiLink),
    _geonameLatitude (iGeonameLatitude), _geonameLongitude (iGeonameLongitude),
    _pageRank (iPageRank),
    _originalKeywords (iOriginalKeywords),
    _correctedKeywords (iCorrectedKeywords),
    _percentage (iPercentage), _editDistance (iEditDistance),
    _allowableEditDistance (iAllowableEditDistance),
    _rawDataString (iRawDataString),
    _itLanguageCodeNum (Language::LAST_VALUE),
    _itLanguageCode (LanguageCode_T ("")) {
  }

  // //////////////////////////////////////////////////////////////////////
  Location::~Location() {
  }

  // //////////////////////////////////////////////////////////////////////
  void Location::toStream (std::ostream& ioOut) const {
    ioOut << toString();
  }

  // //////////////////////////////////////////////////////////////////////
  void Location::fromStream (std::istream&) {
  }

  // //////////////////////////////////////////////////////////////////////
  std::string Location::describeShortKey() const {
    std::ostringstream oStr;
    oStr << _key.describe();
    return oStr.str();
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string Location::describeKey() const {
    return describeShortKey();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string Location::toBasicString() const {
    std::ostringstream oStr;
    oStr << describeShortKey();

    oStr << ", " << _pageRank << "%"
         << ", " << _commonName << ", " << _asciiName
         << ", " << _icaoCode << ", " << _faaCode
         << ", " << describeUNLOCodeList()
         << ", " << describeUICCodeList()
         << ", " << _envelopeID
         << ", " << _dateFrom << ", " << _dateEnd << ", " << _comment
         << ", " << describeCityDetailsList()
         << ", " << _stateCode
         << ", " << _countryCode << ", " << _altCountryCode
         << ", " << _countryName
         << ", " << _wac << ", " << _wacName
         << ", " << _currencyCode
         << ", " << _continentCode << ", " << _continentName
         << ", " << _latitude << ", " << _longitude
         << ", " << _featClass << ", " << _featCode
         << ", " << _admin1Code
         << ", " << _admin1UtfName << ", " << _admin1AsciiName
         << ", " << _admin2Code
         << ", " << _admin2UtfName << ", " << _admin2AsciiName
         << ", " << _admin3Code << ", " << _admin4Code
         << ", " << _population << ", " << _elevation << ", " << _gTopo30
         << ", " << _timeZone
         << ", " << _gmtOffset << ", " << _dstOffset << ", " << _rawOffset
         << ", " << _modificationDate
         << ", " << _tvlPORListString
         << ", " << _wikiLink
         << ", " << _geonameLatitude << ", " << _geonameLongitude
         << ", " << _originalKeywords << ", " << _correctedKeywords
         << ", " << _percentage << "%"
         << ", " << _editDistance << ", " << _allowableEditDistance;

    return oStr.str();
  }
    
  // //////////////////////////////////////////////////////////////////////
  std::string Location::toShortString() const {
    std::ostringstream oStr;
    oStr << toBasicString();

    if (_extraLocationList.empty() == false) {
      oStr << " with " << _extraLocationList.size() << " extra match(es)";
    }
      
    if (_alternateLocationList.empty() == false) {
      oStr << " with " << _alternateLocationList.size()
           << " alternate match(es)";
    }

    return oStr.str();
  }
    
  // //////////////////////////////////////////////////////////////////////
  std::string Location::toSingleLocationString() const {
    std::ostringstream oStr;
    oStr << toBasicString();
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string Location::toString() const {
    std::ostringstream oStr;
    oStr << toSingleLocationString();
    oStr << "; name matrix {" << _nameMatrix.describe() << "}";

    if (_extraLocationList.empty() == false) {
      oStr << "; Extra matches: {";
      unsigned short idx = 0;
      for (LocationList_T::const_iterator itLoc = _extraLocationList.begin();
           itLoc != _extraLocationList.end(); ++itLoc, ++idx) {
        if (idx != 0) {
          oStr << ". ";
        }
        const Location& lExtraLocation = *itLoc;
        oStr << lExtraLocation.toShortString();
      }
      oStr << "}";
    }

    if (_alternateLocationList.empty() == false) {
      oStr << "; Alternate matches: {";
      unsigned short idx = 0;
      for (LocationList_T::const_iterator itLoc =
             _alternateLocationList.begin();
           itLoc != _alternateLocationList.end(); ++itLoc, ++idx) {
        if (idx != 0) {
          oStr << ". ";
        }
        const Location& lAlternateLocation = *itLoc;
        oStr << lAlternateLocation.toShortString();
      }
      oStr << "}";
    }

    return oStr.str();
  }

  // ////////////////////////////////////////////////////////////////////
  Date_T Location::calculateDate() const {
    _itYear.check(); _itMonth.check(); _itDay.check();
    return Date_T (_itYear._value, _itMonth._value, _itDay._value);
  }

  // ////////////////////////////////////////////////////////////////////
  std::string Location::describeUNLOCodeList() const {
    std::ostringstream oStr;
    unsigned short idx = 0;
    for (UNLOCodeList_T::const_iterator itUNLOCode = _unLOCodeList.begin();
         itUNLOCode != _unLOCodeList.end(); ++itUNLOCode, ++idx) {
      if (idx != 0) {
        oStr << ",";
      }
      const UNLOCode_T& lUNLOCode = *itUNLOCode;
      oStr << lUNLOCode;
    }

    return oStr.str();
  }

  // ////////////////////////////////////////////////////////////////////
  std::string Location::describeUICCodeList() const {
    std::ostringstream oStr;
    unsigned short idx = 0;
    for (UICCodeList_T::const_iterator itUICCode = _uicCodeList.begin();
         itUICCode != _uicCodeList.end(); ++itUICCode, ++idx) {
      if (idx != 0) {
        oStr << ",";
      }
      const UICCode_T& lUICCode = *itUICCode;
      oStr << lUICCode;
    }

    return oStr.str();
  }

  // ////////////////////////////////////////////////////////////////////
  std::string Location::describeCityDetailsList() const {
    std::ostringstream oStr;
    unsigned short idx = 0;
    for (CityDetailsList_T::const_iterator itCity = _cityList.begin();
         itCity != _cityList.end(); ++itCity, ++idx) {
      if (idx != 0) {
        oStr << ",";
      }
      const CityDetails& lCityDetails = *itCity;
      oStr << lCityDetails;
    }

    return oStr.str();
  }

  // ////////////////////////////////////////////////////////////////////
  void Location::consolidateCityDetailsList() {
    const CityDetails lCityDetails (IATACode_T (_itCityIataCode),
                                    _itCityGeonamesID,
                                    CityUTFName_T (_itCityUtfName),
                                    CityASCIIName_T (_itCityAsciiName),
                                    CountryCode_T (_itCityCountryCode),
                                    StateCode_T (_itCityStateCode));
    _cityList.push_back (lCityDetails);
  }

  // ////////////////////////////////////////////////////////////////////
  void Location::consolidateAltNameShortListString() {
    std::ostringstream oStr;
    unsigned short idx = 0;
    for (AltNameShortList_T::const_iterator itName = _itAltNameShortList.begin();
         itName != _itAltNameShortList.end(); ++itName, ++idx) {
      if (idx != 0) {
        oStr << ",";
      }
      const AltNameShortListString_T& lAlternateLocation = *itName;
      oStr << lAlternateLocation;
    }

    // Store the short list of alternative names
    const std::string lAltNameShortListString (oStr.str());
    setAltNameShortListString (lAltNameShortListString);
  }

  // ////////////////////////////////////////////////////////////////////
  void Location::consolidateTvlPORListString() {
    std::ostringstream oStr;
    unsigned short idx = 0;
    for (IATACodeList_T::const_iterator itCode = _itTvlPORList.begin();
         itCode != _itTvlPORList.end(); ++itCode, ++idx) {
      if (idx != 0) {
        oStr << ",";
      }
      const IATACode_T& lTvlPORCode = *itCode;
      oStr << lTvlPORCode;
    }

    // Store the list of IATA codes of the travel-related POR
    const std::string lTvlPORCodeList (oStr.str());
    setTvlPORListString (lTvlPORCodeList);
  }

  // ////////////////////////////////////////////////////////////////////
  FeatureNameList_T Location::getFeatureList(const FeatureCode_T& iFeatureCode) {
    FeatureNameList_T oList;

    // Extract the first two/three characters of the feature code (e.g.,
    // 'MN' for 'MNCU', 'PPL' for 'PPLA'/'PPLC' or 'ADM' for 'ADM1'/'ADM2')
    const std::string l2CharFeatCode = iFeatureCode.substr (0, 2);
    const std::string l3CharFeatCode = iFeatureCode.substr (0, 3);
    
    if (iFeatureCode == "AIRP" || iFeatureCode == "AIRF"
        || iFeatureCode == "AIRQ") {
      oList.push_back (FeatureName_T ("airport"));
      oList.push_back (FeatureName_T ("apt"));
      oList.push_back (FeatureName_T ("airdrome"));
      oList.push_back (FeatureName_T ("aerodrome"));
      oList.push_back (FeatureName_T ("airfield"));
      oList.push_back (FeatureName_T ("air field"));
      oList.push_back (FeatureName_T ("field"));
      oList.push_back (FeatureName_T ("airstrip"));
      oList.push_back (FeatureName_T ("air strip"));
      oList.push_back (FeatureName_T ("strip"));

    } else if (iFeatureCode == "AIRB") {
      oList.push_back (FeatureName_T ("airport"));
      oList.push_back (FeatureName_T ("apt"));
      oList.push_back (FeatureName_T ("airbase"));
      oList.push_back (FeatureName_T ("air base"));
      oList.push_back (FeatureName_T ("ab"));
      oList.push_back (FeatureName_T ("base"));

    } else if (iFeatureCode == "AIRS") {
      oList.push_back (FeatureName_T ("airport"));
      oList.push_back (FeatureName_T ("apt"));
      oList.push_back (FeatureName_T ("spb"));
      oList.push_back (FeatureName_T ("sea plane base"));
      oList.push_back (FeatureName_T ("sea plane"));
      oList.push_back (FeatureName_T ("waterdrome"));
      oList.push_back (FeatureName_T ("aerodrome"));
      oList.push_back (FeatureName_T ("water aerodrome"));

    } else if (iFeatureCode == "AIRH") {
      oList.push_back (FeatureName_T ("airport"));
      oList.push_back (FeatureName_T ("apt"));
      oList.push_back (FeatureName_T ("heliport"));
      oList.push_back (FeatureName_T ("hpt"));

    } else if (iFeatureCode == "RSTN") {
      oList.push_back (FeatureName_T ("railway"));
      oList.push_back (FeatureName_T ("railroad"));
      oList.push_back (FeatureName_T ("train"));
      oList.push_back (FeatureName_T ("station"));
      oList.push_back (FeatureName_T ("railway station"));
      oList.push_back (FeatureName_T ("railroad station"));
      oList.push_back (FeatureName_T ("train station"));

    } else if (l3CharFeatCode == "BUS") {
      oList.push_back (FeatureName_T ("bus"));
      oList.push_back (FeatureName_T ("autobus"));
      oList.push_back (FeatureName_T ("station"));
      oList.push_back (FeatureName_T ("stn"));
      oList.push_back (FeatureName_T ("stop"));
      oList.push_back (FeatureName_T ("bus station"));
      oList.push_back (FeatureName_T ("bus stop"));
      oList.push_back (FeatureName_T ("autobus station"));
      oList.push_back (FeatureName_T ("autobus stop"));

    } else if (iFeatureCode == "MTRO") {
      oList.push_back (FeatureName_T ("metro"));
      oList.push_back (FeatureName_T ("station"));
      oList.push_back (FeatureName_T ("stn"));
      oList.push_back (FeatureName_T ("stop"));
      oList.push_back (FeatureName_T ("metro station"));
      oList.push_back (FeatureName_T ("metro stop"));

    } else if (iFeatureCode == "FY" || iFeatureCode == "PRT"
               || iFeatureCode == "PORT" || iFeatureCode == "HBR") {
      oList.push_back (FeatureName_T ("ferry"));
      oList.push_back (FeatureName_T ("maritime"));
      oList.push_back (FeatureName_T ("port"));
      oList.push_back (FeatureName_T ("prt"));
      oList.push_back (FeatureName_T ("harbor"));
      oList.push_back (FeatureName_T ("harbour"));
      oList.push_back (FeatureName_T ("hbr"));
      oList.push_back (FeatureName_T ("ferry port"));
      oList.push_back (FeatureName_T ("maritime port"));

    } else if (l3CharFeatCode == "PPL" || l3CharFeatCode == "ADM"
               || iFeatureCode == "LCTY") {
      oList.push_back (FeatureName_T ("city"));
      oList.push_back (FeatureName_T ("cty"));
      oList.push_back (FeatureName_T ("town"));
      oList.push_back (FeatureName_T ("downtown"));
      oList.push_back (FeatureName_T ("down town"));
      oList.push_back (FeatureName_T ("center"));
      oList.push_back (FeatureName_T ("centre"));

    } else if (l3CharFeatCode == "PCL" || iFeatureCode == "AREA") {
      oList.push_back (FeatureName_T ("city"));
      oList.push_back (FeatureName_T ("cty"));
      oList.push_back (FeatureName_T ("country"));
      oList.push_back (FeatureName_T ("town"));
      oList.push_back (FeatureName_T ("downtown"));
      oList.push_back (FeatureName_T ("down town"));
      oList.push_back (FeatureName_T ("center"));
      oList.push_back (FeatureName_T ("centre"));
      oList.push_back (FeatureName_T ("district"));
      oList.push_back (FeatureName_T ("prefecture"));

    } else if (iFeatureCode == "MT") {
      oList.push_back (FeatureName_T ("city"));
      oList.push_back (FeatureName_T ("cty"));
      oList.push_back (FeatureName_T ("mount"));
      oList.push_back (FeatureName_T ("mt"));

    } else if (iFeatureCode == "PLAT") {
      oList.push_back (FeatureName_T ("city"));
      oList.push_back (FeatureName_T ("cty"));
      oList.push_back (FeatureName_T ("plateau"));

    } else if (iFeatureCode == "OAS") {
      oList.push_back (FeatureName_T ("city"));
      oList.push_back (FeatureName_T ("cty"));
      oList.push_back (FeatureName_T ("oasis"));

    } else if (iFeatureCode == "PRK") {
      oList.push_back (FeatureName_T ("city"));
      oList.push_back (FeatureName_T ("cty"));
      oList.push_back (FeatureName_T ("park"));
      oList.push_back (FeatureName_T ("prk"));

    } else if (l3CharFeatCode == "RES") {
      oList.push_back (FeatureName_T ("city"));
      oList.push_back (FeatureName_T ("cty"));
      oList.push_back (FeatureName_T ("reserve"));
      oList.push_back (FeatureName_T ("rsv"));

    } else if (iFeatureCode == "CMP") {
      oList.push_back (FeatureName_T ("city"));
      oList.push_back (FeatureName_T ("cty"));
      oList.push_back (FeatureName_T ("camp"));
      oList.push_back (FeatureName_T ("cmp"));

    } else if (l2CharFeatCode == "MN") {
      oList.push_back (FeatureName_T ("city"));
      oList.push_back (FeatureName_T ("cty"));
      oList.push_back (FeatureName_T ("mine"));

    } else if (iFeatureCode == "HMSD") {
      oList.push_back (FeatureName_T ("city"));
      oList.push_back (FeatureName_T ("cty"));
      oList.push_back (FeatureName_T ("homestead"));

    } else if (l2CharFeatCode == "RK") {
      oList.push_back (FeatureName_T ("city"));
      oList.push_back (FeatureName_T ("cty"));
      oList.push_back (FeatureName_T ("rock"));

    } else if (iFeatureCode == "PT") {
      oList.push_back (FeatureName_T ("city"));
      oList.push_back (FeatureName_T ("cty"));
      oList.push_back (FeatureName_T ("point"));
      oList.push_back (FeatureName_T ("pt"));

    } else if (l3CharFeatCode == "RSV") {
      oList.push_back (FeatureName_T ("city"));
      oList.push_back (FeatureName_T ("cty"));
      oList.push_back (FeatureName_T ("reservoir"));
      oList.push_back (FeatureName_T ("rsv"));

    } else if (iFeatureCode == "LK") {
      oList.push_back (FeatureName_T ("city"));
      oList.push_back (FeatureName_T ("cty"));
      oList.push_back (FeatureName_T ("lake"));
      oList.push_back (FeatureName_T ("lk"));

    } else if (iFeatureCode == "BAY") {
      oList.push_back (FeatureName_T ("city"));
      oList.push_back (FeatureName_T ("cty"));
      oList.push_back (FeatureName_T ("bay"));

    } else if (iFeatureCode == "CHN") {
      oList.push_back (FeatureName_T ("city"));
      oList.push_back (FeatureName_T ("cty"));
      oList.push_back (FeatureName_T ("channel"));
      oList.push_back (FeatureName_T ("chn"));

    } else if (l3CharFeatCode == "ISL") {
      oList.push_back (FeatureName_T ("city"));
      oList.push_back (FeatureName_T ("cty"));
      oList.push_back (FeatureName_T ("island"));
      oList.push_back (FeatureName_T ("isl"));
      oList.push_back (FeatureName_T ("country"));

    } else if (iFeatureCode == "ATOL") {
      oList.push_back (FeatureName_T ("city"));
      oList.push_back (FeatureName_T ("cty"));
      oList.push_back (FeatureName_T ("island"));
      oList.push_back (FeatureName_T ("isl"));
      oList.push_back (FeatureName_T ("atoll"));
      oList.push_back (FeatureName_T ("atl"));
      oList.push_back (FeatureName_T ("country"));
    }

    //
    return oList;
  }

}
