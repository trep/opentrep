// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// OpenTrep
#include <opentrep/basic/BasConst_General.hpp>
#include <opentrep/basic/OTransliterator.hpp>
#include <opentrep/basic/Utilities.hpp>
#include <opentrep/bom/WordCombinationHolder.hpp>
#include <opentrep/bom/Place.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  Place::Place() :
    _world (NULL), _placeHolder (NULL), _mainPlace (NULL),
    _location (IATACode_T (""), IATAType::LAST_VALUE, 0,
               ICAOCode_T (""), FAACode_T (""),
               CommonName_T (""), ASCIIName_T (""), 0,
               Date_T (1970, 01, 01), Date_T (2999, 12, 31), Comment_T (""),
               CityDetailsList_T(),
               StateCode_T (""),
               CountryCode_T (""), AltCountryCode_T (""), CountryName_T (""),
               0, WACName_T (""),
               CurrencyCode_T (""), ContinentName_T (""), 
               0.0, 0.0,
               FeatureClass_T (""), FeatureCode_T (""),
               Admin1Code_T (""), Admin1UTFName_T (""), Admin1ASCIIName_T (""),
               Admin2Code_T (""), Admin2UTFName_T (""), Admin2ASCIIName_T (""),
               Admin3Code_T (""), Admin4Code_T (""),
               0, 0, 0, TimeZone_T (""), 0, 0, 0,
               Date_T (2000, 01, 01), TvlPORListString_T (""),
               WikiLink_T (""), 0.0, 0.0, K_DEFAULT_PAGE_RANK, "", "", 0, 0, 0,
               RawDataString_T ("")),
    _docID (0) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  Place::Place (const LocationKey& iKey) :
    _world (NULL), _placeHolder (NULL), _mainPlace (NULL),
    _location (iKey.getIataCode(), iKey.getIataType(), iKey.getGeonamesID(),
               ICAOCode_T (""), FAACode_T (""),
               CommonName_T (""), ASCIIName_T (""), 0,
               Date_T (1970, 01, 01), Date_T (2999, 12, 31), Comment_T (""),
               CityDetailsList_T(),
               StateCode_T (""),
               CountryCode_T (""), AltCountryCode_T (""), CountryName_T (""),
               0, WACName_T (""),
               CurrencyCode_T (""), ContinentName_T (""), 
               0.0, 0.0,
               FeatureClass_T (""), FeatureCode_T (""),
               Admin1Code_T (""), Admin1UTFName_T (""), Admin1ASCIIName_T (""),
               Admin2Code_T (""), Admin2UTFName_T (""), Admin2ASCIIName_T (""),
               Admin3Code_T (""), Admin4Code_T (""),
               0, 0, 0, TimeZone_T (""), 0, 0, 0,
               Date_T (2000, 01, 01), TvlPORListString_T (""),
               WikiLink_T (""), 0.0, 0.0, K_DEFAULT_PAGE_RANK, "", "", 0, 0, 0,
               RawDataString_T ("")),
    _docID (0) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  Place::Place (const Location& iLocation) :
    _world (NULL), _placeHolder (NULL), _mainPlace (NULL),
    _location (iLocation), _docID (0) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  Place::Place (const Place& iPlace) :
    _world (iPlace._world), _placeHolder (iPlace._placeHolder),
    _mainPlace (iPlace._mainPlace),
    _location (iPlace._location),
    _docID (iPlace._docID),
    _termSetMap (iPlace._termSetMap), _spellingSet (iPlace._spellingSet),
    _stemmingSet (iPlace._stemmingSet), _synonymSet (iPlace._synonymSet) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  Place::~Place() {
  }

  // //////////////////////////////////////////////////////////////////////
  std::string Place::toString() const {
    std::ostringstream oStr;
    oStr << _location.toString();
    oStr << ", " << _docID
         << ". ";
    
    if (_extraPlaceList.empty() == false) {
      oStr << "; Extra matches: {";
      unsigned short idx = 0;
      for (PlaceOrderedList_T::const_iterator itLoc = _extraPlaceList.begin();
           itLoc != _extraPlaceList.end(); ++itLoc, ++idx) {
        if (idx != 0) {
          oStr << "; ";
        }
        const Place* lExtraPlace_ptr = *itLoc;
        assert (lExtraPlace_ptr != NULL);
        oStr << lExtraPlace_ptr->toShortString();
      }
      oStr << "}";
    }
    
    if (_alternatePlaceList.empty() == false) {
      oStr << "; Alternate matches: {";
      unsigned short idx = 0;
      for (PlaceOrderedList_T::const_iterator itLoc =
             _alternatePlaceList.begin();
           itLoc != _alternatePlaceList.end(); ++itLoc, ++idx) {
        if (idx != 0) {
          oStr << "; ";
        }
        const Place* lAlternatePlace_ptr = *itLoc;
        assert (lAlternatePlace_ptr != NULL);
        oStr << lAlternatePlace_ptr->toShortString();
      }
      oStr << "}";
    }
      
    return oStr.str();
  }   

  // //////////////////////////////////////////////////////////////////////
  std::string Place::toShortString() const {
    /**
     * When the city code is empty, it means that the place is a city and
     * not an airport. The city code is thus the same as the place code
     * itself.
     */
    std::ostringstream oStr;
    oStr << _location.toShortString();
    oStr << ", " << _docID;

    if (_extraPlaceList.empty() == false) {
      oStr << " " << _extraPlaceList.size() << " extra match(es)";
    }
    
    if (_alternatePlaceList.empty() == false) {
      oStr << " " << _alternatePlaceList.size() << " alternate match(es)";
    }
    
    return oStr.str();
  }   

  // //////////////////////////////////////////////////////////////////////
  void Place::toStream (std::ostream& ioOut) const {
    ioOut << toString();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void Place::fromStream (std::istream& ioIn) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string Place::describeSets() const {
    std::ostringstream oStr;

    // Xapian index for the current place/POR (point of reference)
    oStr << "[index] ";
    short idx_map = 0;
    for (TermSetMap_T::const_iterator itStringSet = _termSetMap.begin();
         itStringSet != _termSetMap.end(); ++itStringSet, ++idx_map) {
      if (idx_map != 0) {
        oStr << " - ";
      }
      // Retrieve the weight and display it
      const Weight_T& lWeight = itStringSet->first;
      oStr << "[" << lWeight << "] ";

      // Retrieve and browse the set of strings for that weight
      const StringSet_T& lStringSet = itStringSet->second;
      short idx_set = 0;
      for (StringSet_T::const_iterator itString = lStringSet.begin();
           itString != lStringSet.end(); ++itString, ++idx_set) {
        if (idx_set != 0) {
          oStr << ", ";
        }
        const std::string& lString = *itString;
        oStr << lString;
      }
    }

    // Xapian spelling dictionary
    oStr << "; [spelling] ";
    short idx = 0;
    for (StringSet_T::const_iterator itTerm = _spellingSet.begin();
         itTerm != _spellingSet.end(); ++itTerm, ++idx) {
      if (idx != 0) {
        oStr << ", ";
      }
      const std::string& lTerm = *itTerm;
      oStr <<  lTerm;
    }
    oStr << ";";

    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string Place::shortDisplay() const {
    /* When the city code is empty, it means that the place is a city and
       not an airport. The city code is thus the same as the place code
       itself. */
    std::ostringstream oStr;
    oStr << _location.toBasicString();
    oStr << ", " << _docID;
    return oStr.str();
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string Place::display() const {
    std::ostringstream oStr;
    oStr << shortDisplay() << std::endl;
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  void Place::resetIndexSets() {
    _termSetMap.clear();
    _spellingSet.clear();
    _stemmingSet.clear();
    _synonymSet.clear();
  }
  
  // //////////////////////////////////////////////////////////////////////
  Place::StringSet_T Place::getTermSet (const Weight_T& iWeight) const {
    // Initialise an empty string set
    StringSet_T oTermSet;

    // If existing, retrieve the string set for that weight.
    TermSetMap_T::const_iterator itTermSet = _termSetMap.find (iWeight);
    if (itTermSet != _termSetMap.end()) {
      oTermSet = itTermSet->second;
    }

    return oTermSet;
  }

  // //////////////////////////////////////////////////////////////////////
  bool Place::addTermSet (const Weight_T& iWeight, const StringSet_T& iTermSet) {
    bool hasInsertBeenSuccessful = true;

    // If existing, retrieve the string set for that weight.
    TermSetMap_T::iterator itTermSet = _termSetMap.find (iWeight);

    if (itTermSet == _termSetMap.end()) {
      // If there was no string set for that weight yet, insert it
      hasInsertBeenSuccessful =
        _termSetMap.insert (TermSetMap_T::value_type (iWeight, iTermSet)).second;

      // Sanity check
      assert (hasInsertBeenSuccessful == true);

    } else {
      // Otherwise, add the given string set, string by string,
      // to the existing one
      StringSet_T& lStringSet = itTermSet->second;
      for (StringSet_T::const_iterator itString = iTermSet.begin();
           itString != iTermSet.end(); ++itString) {
        const std::string& lString = *itString;
        lStringSet.insert (lString);
      }
    }

    return hasInsertBeenSuccessful;
  }

  // //////////////////////////////////////////////////////////////////////
  void Place::addNameToXapianSets (const Weight_T& iWeight,
                                   const std::string& iBaseName,
                                   const FeatureCode_T& iFeatureCode) {
    // Retrieve the string set for the given weight, if existing
    // (empty otherwise)
    StringSet_T lTermSet = getTermSet (iWeight);

    /**
     * Derive the list of feature names from the feature code. For instance,
     * the 'AIRP' feature code provides the ('airport', 'airdrome', 'aerodrome',
     * 'airfield', 'air field', 'airstrip', 'air strip', 'airbase', 'air base')
     * list.
     */
    const FeatureNameList_T& lFeatureNameList =
      Location::getFeatureList (iFeatureCode);
    for (FeatureNameList_T::const_iterator itFeatName = lFeatureNameList.begin();
         itFeatName != lFeatureNameList.end(); ++itFeatName) {
      const FeatureName_T& lFeatureName = *itFeatName;

      lTermSet.insert (iBaseName + " " + lFeatureName);
    }

    // Insert (or replace) the newly created (or just altered) string set
    addTermSet (iWeight, lTermSet);
  }

  // //////////////////////////////////////////////////////////////////////
  void Place::addNameToXapianSets (const Weight_T& iWeight,
                                   const LocationName_T& iLocationName,
                                   const FeatureCode_T& iFeatureCode,
                                   const CityNameList_T& iCityUtfNameList,
                                   const CityNameList_T& iCityAsciiNameList,
                                   const Admin1UTFName_T& iAdm1UtfName,
                                   const Admin1ASCIIName_T& iAdm1AsciiName,
                                   const Admin2UTFName_T& iAdm2UtfName,
                                   const Admin2ASCIIName_T& iAdm2AsciiName,
                                   const StateCode_T& iStateCode,
                                   const CountryCode_T& iCountryCode,
                                   const CountryName_T& iCountryName,
                                   const ContinentName_T& iContinentName,
                                   const OTransliterator& iTransliterator) {
    // Retrieve the string set for the given weight, if existing
    // (empty otherwise)
    StringSet_T lTermSet = getTermSet (iWeight);

    // Tokenise the name. Some of the names contain punctuation characters.
    // For instance, "Paris/FR/Gare" is transformed into "Paris FR Gare".
    WordList_T lWordList;
    tokeniseStringIntoWordList (iLocationName, lWordList);
    const std::string lTokenisedName = createStringFromWordList (lWordList);

    // Add the tokenised name to the Xapian index
    lTermSet.insert (lTokenisedName);

    // Add the (tokenised name, feature name) pair to the Xapian index
    addNameToXapianSets (iWeight, lTokenisedName, iFeatureCode);

    // Add the (tokenised name, UTF8 name) pair to the Xapian index
    for (CityNameList_T::const_iterator itCityUtfName = iCityUtfNameList.begin();
         itCityUtfName != iCityUtfNameList.end(); ++itCityUtfName) {
      const std::string& lCityUtfName = *itCityUtfName;
      if (lCityUtfName.empty() == false) {
        lTermSet.insert (lTokenisedName + " " + lCityUtfName);
      }
    }
    
    // Add the (tokenised name, ASCII city name) pair to the Xapian index
    for (CityNameList_T::const_iterator itCityAsciiName =
           iCityAsciiNameList.begin();
         itCityAsciiName != iCityAsciiNameList.end(); ++itCityAsciiName) {
      const std::string& lCityAsciiName = *itCityAsciiName;
      if (lCityAsciiName.empty() == false) {
        lTermSet.insert (lTokenisedName + " " + lCityAsciiName);
      }
    }

    // Add the (tokenised name, UTF8 admin level 1 name) pair to the Xapian index
    if (iAdm1UtfName.empty() == false) {
      lTermSet.insert (lTokenisedName + " " + iAdm1UtfName);
    }

    // Add the (tokenised name, ASCII adm level 1 name) pair to the Xapian index
    if (iAdm1AsciiName.empty() == false) {
      lTermSet.insert (lTokenisedName + " " + iAdm1AsciiName);
    }

    // Add the (tokenised name, UTF8 admin level 2 name) pair to the Xapian index
    if (iAdm2UtfName.empty() == false) {
      lTermSet.insert (lTokenisedName + " " + iAdm2UtfName);
    }

    // Add the (tokenised name, ASCII adm level 2 name) pair to the Xapian index
    if (iAdm2AsciiName.empty() == false) {
      lTermSet.insert (lTokenisedName + " " + iAdm2AsciiName);
    }

    // Add the (tokenised name, state code) pair to the Xapian index
    lTermSet.insert (lTokenisedName + " " + iStateCode);

    // Add the (tokenised name, country code) pair to the Xapian index
    lTermSet.insert (lTokenisedName + " " + iCountryCode);

    // Add the (tokenised name, country name) pair to the Xapian index
    lTermSet.insert (lTokenisedName + " " + iCountryName);

    // Add the (tokenised name, continent name) pair to the Xapian index
    lTermSet.insert (lTokenisedName + " " + iContinentName);

    // Add the tokenised name to the Xapian spelling dictionary
    _spellingSet.insert (lTokenisedName);

    // Normalise, according to the Unicode standard, the given name.
    // Note that it is important to normalise after the tokenisation process,
    // as the punctuation is eliminated (and not replaced by space)
    // by that latter.
    const std::string& lNormalisedCommonName =
      iTransliterator.normalise (lTokenisedName);

    // Add the tokenised and normalised name to the Xapian index
    lTermSet.insert (lNormalisedCommonName);

    // Add the (tokenised and normalised name, feature name) pair
    // to the Xapian index
    addNameToXapianSets (iWeight, lNormalisedCommonName, iFeatureCode);

    // Add the (tokenised and normalised name, UTF8 city name) pair
    // to the Xapian index
    for (CityNameList_T::const_iterator itCityUtfName = iCityUtfNameList.begin();
         itCityUtfName != iCityUtfNameList.end(); ++itCityUtfName) {
      const std::string& lCityUtfName = *itCityUtfName;
      if (lCityUtfName.empty() == false) {
        lTermSet.insert (lNormalisedCommonName + " " + lCityUtfName);
      }
    }
    
    // Add the (tokenised and normalised name, ASCII city name) pair
    // to the Xapian index
    for (CityNameList_T::const_iterator itCityAsciiName =
           iCityAsciiNameList.begin();
         itCityAsciiName != iCityAsciiNameList.end(); ++itCityAsciiName) {
      const std::string& lCityAsciiName = *itCityAsciiName;
      if (lCityAsciiName.empty() == false) {
        lTermSet.insert (lNormalisedCommonName + " " + lCityAsciiName);
      }
    }

    // Add the (tokenised and normalised name, UTF8 admin level 1 name) pair
    // to the Xapian index
    if (iAdm1UtfName.empty() == false) {
      lTermSet.insert (lNormalisedCommonName + " " + iAdm1UtfName);
    }

    // Add the (tokenised and normalised name, ASCII adm level 1 name) pair
    // to the Xapian index
    if (iAdm1AsciiName.empty() == false) {
      lTermSet.insert (lNormalisedCommonName + " " + iAdm1AsciiName);
    }

    // Add the (tokenised and normalised name, UTF8 admin level 2 name) pair
    // to the Xapian index
    if (iAdm2UtfName.empty() == false) {
      lTermSet.insert (lNormalisedCommonName + " " + iAdm2UtfName);
    }

    // Add the (tokenised and normalised name, ASCII adm level 2 name) pair
    // to the Xapian index
    if (iAdm2AsciiName.empty() == false) {
      lTermSet.insert (lNormalisedCommonName + " " + iAdm2AsciiName);
    }

    // Add the (tokenised and normalised name, state code) pair to the
    // Xapian index
    lTermSet.insert (lNormalisedCommonName + " " + iStateCode);

    // Add the (tokenised and normalised name, country code) pair to the
    // Xapian index
    lTermSet.insert (lNormalisedCommonName + " " + iCountryCode);

    // Add the (tokenised and normalised name, country name) pair to the
    // Xapian index
    lTermSet.insert (lNormalisedCommonName + " " + iCountryName);

    // Add the (tokenised and normalised name, continent name) pair to the
    // Xapian index
    lTermSet.insert (lNormalisedCommonName + " " + iContinentName);

    // Add the tokenised and normalised name to the Xapian spelling dictionary
    _spellingSet.insert (lNormalisedCommonName);

    // Insert (or replace) the newly created (or just altered) string set
    addTermSet (iWeight, lTermSet);
  }

  // //////////////////////////////////////////////////////////////////////
  void Place::buildIndexSets (const OTransliterator& iTransliterator) {

    /**
     * Add the place/POR details into Xapian:
     * <ul>
     *   <li>index related to the current place/POR</li>
     *   <li>spelling dictionary</li>
     *   <li>stemming dictionary</li>
     *   <li>synonym dictionary</li>
     * </ul>
     */

    // Retrieve the PageRank
    const PageRank_T& lPageRankDouble = _location.getPageRank();
    //const Weight_T lPageRank =
    //  (lPageRankDouble == K_DEFAULT_PAGE_RANK)?
    //  K_DEFAULT_INDEXING_EXTRA_WEIGHT:
    //  static_cast<const Weight_T> (lPageRankDouble);
    const Weight_T lPageRank = K_DEFAULT_INDEXING_EXTRA_WEIGHT
      + static_cast<const Weight_T> (lPageRankDouble / 5.0);

    // Retrieve the string set for the given weight, if existing
    // (empty otherwise)
    StringSet_T lWeightedTermSet = getTermSet (lPageRank);

    // Retrieve the string set for the given weight, if existing
    // (empty otherwise)
    StringSet_T lStdTermSet = getTermSet (K_DEFAULT_INDEXING_STD_WEIGHT);

    // Retrieve the feature code
    const FeatureCode_T& lFeatureCode = _location.getFeatureCode();

    // Add the IATA code
    const std::string& lIataCode = _location.getIataCode();
    if (lIataCode.empty() == false) {
      lWeightedTermSet.insert (lIataCode);
      _spellingSet.insert (lIataCode);

      // Add the (IATA code, feature name) to the Xapian index, where the
      // feature name is derived from the feature code.
      addNameToXapianSets (lPageRank, lIataCode, lFeatureCode);
    }

    // Add the ICAO code
    const std::string& lIcaoCode = _location.getIcaoCode();
    if (lIcaoCode.empty() == false) {
      lWeightedTermSet.insert (lIcaoCode);
      _spellingSet.insert (lIcaoCode);

      // Add the (ICAO code, feature name) to the Xapian index, where the
      // feature name is derived from the feature code.
      addNameToXapianSets (lPageRank, lIcaoCode, lFeatureCode);
    }

    // Add the FAA code
    const std::string& lFaaCode = _location.getFaaCode();
    if (lFaaCode.empty() == false) {
      lWeightedTermSet.insert (lFaaCode);
      _spellingSet.insert (lFaaCode);

      // Add the (FAA code, feature name) to the Xapian index, where the
      // feature name is derived from the feature code.
      addNameToXapianSets (lPageRank, lFaaCode, lFeatureCode);
    }

    // Add the UN/LOCODE codes
    const UNLOCodeList_T& lUNLOCodeList = _location.getUNLOCodeList();
    for (UNLOCodeList_T::const_iterator itUNLOCode = lUNLOCodeList.begin();
         itUNLOCode != lUNLOCodeList.end(); ++itUNLOCode) {
      const UNLOCode_T& lUNLOCode = *itUNLOCode;
      if (lUNLOCode.empty() == false) {
        lWeightedTermSet.insert (lUNLOCode);
        _spellingSet.insert (lUNLOCode);

        // Add the (UN/LOCODE code, feature name) to the Xapian index, where the
        // feature name is derived from the feature code.
        addNameToXapianSets (lPageRank, lUNLOCode, lFeatureCode);
      }
    }

    // Add the UIC codes
    const UICCodeList_T& lUICCodeList = _location.getUICCodeList();
    for (UICCodeList_T::const_iterator itUICCode = lUICCodeList.begin();
         itUICCode != lUICCodeList.end(); ++itUICCode) {
      const UICCode_T& lUICCode = *itUICCode;
      if (lUICCode != 0) {
        std::stringstream oStr;
        oStr << lUICCode;
        const std::string lUICCodeStr = oStr.str();
        lWeightedTermSet.insert (lUICCodeStr);
        _spellingSet.insert (lUICCodeStr);

        // Add the (UIC code, feature name) to the Xapian index, where the
        // feature name is derived from the feature code.
        addNameToXapianSets (lPageRank, lUICCodeStr, lFeatureCode);
      }
    }

    // Add the Geonames ID
    const GeonamesID_T& lGeonamesID = _location.getGeonamesID();
    if (lGeonamesID != 0) {
      std::stringstream oStr;
      oStr << lGeonamesID;
      const std::string lGeonamesIDStr = oStr.str();
      lStdTermSet.insert (lGeonamesIDStr);
      _spellingSet.insert (lGeonamesIDStr);
    }

    // Add the feature code
    if (lFeatureCode.empty() == false) {
      lWeightedTermSet.insert (lFeatureCode);
      _spellingSet.insert (lFeatureCode);
    }

    // Add the details of the served cities
    CityNameList_T lCityUtfNameList;
    CityNameList_T lCityAsciiNameList;
    const CityDetailsList_T& lCityList = _location.getCityList();
    for (CityDetailsList_T::const_iterator itCity = lCityList.begin();
         itCity != lCityList.end(); ++itCity) {
      const CityDetails& lCity = *itCity;
      
      // Add the city IATA code
      const std::string& lCityCode = lCity.getIataCode();
      if (lCityCode.empty() == false && lCityCode != lIataCode) {
        lWeightedTermSet.insert (lCityCode);
        _spellingSet.insert (lCityCode);
      }

      // Add the city UTF8 name
      const std::string& lCityUtfName = lCity.getUtfName();
      if (lCityUtfName.empty() == false) {
        lCityUtfNameList.push_back(lCityUtfName);
        lWeightedTermSet.insert (lCityUtfName);
        _spellingSet.insert (lCityUtfName);
      }

      // Add the city ASCII name
      const std::string& lCityAsciiName = lCity.getAsciiName();
      if (lCityAsciiName.empty() == false) {
        lCityAsciiNameList.push_back(lCityAsciiName);
        lWeightedTermSet.insert (lCityAsciiName);
        _spellingSet.insert (lCityAsciiName);
      }
    }

    // Add the state code
    const std::string& lStateCode = _location.getStateCode();
    if (lStateCode.empty() == false) {
      lWeightedTermSet.insert (lStateCode);
      _spellingSet.insert (lStateCode);
    }

    // Add the country code
    const std::string& lCountryCode = _location.getCountryCode();
    lWeightedTermSet.insert (lCountryCode);

    // Add the country name
    const std::string& lCountryName = _location.getCountryName();
    if (lCountryName.empty() == false) {
      lWeightedTermSet.insert (lCountryName);
      _spellingSet.insert (lCountryName);
    }

    // Add the administrative level 1 code
    const std::string& lAdm1Code = _location.getAdmin1Code();
    if (lAdm1Code.empty() == false) {
      lWeightedTermSet.insert (lAdm1Code);
    }

    // Add the administrative level 1 UTF8 name
    const std::string& lAdm1UtfName = _location.getAdmin1UtfName();
    if (lAdm1UtfName.empty() == false) {
      lWeightedTermSet.insert (lAdm1UtfName);
      _spellingSet.insert (lAdm1UtfName);
    }

    // Add the administrative level 1 ASCII name
    const std::string& lAdm1AsciiName = _location.getAdmin1AsciiName();
    if (lAdm1AsciiName.empty() == false) {
      lWeightedTermSet.insert (lAdm1AsciiName);
      _spellingSet.insert (lAdm1AsciiName);
    }

    // Add the administrative level 2 code
    const std::string& lAdm2Code = _location.getAdmin1Code();
    if (lAdm2Code.empty() == false) {
      lWeightedTermSet.insert (lAdm2Code);
    }

    // Add the administrative level 2 UTF8 name
    const std::string& lAdm2UtfName = _location.getAdmin2UtfName();
    if (lAdm2UtfName.empty() == false) {
      lWeightedTermSet.insert (lAdm2UtfName);
      _spellingSet.insert (lAdm2UtfName);
    }

    // Add the administrative level 2 ASCII name
    const std::string& lAdm2AsciiName = _location.getAdmin2AsciiName();
    if (lAdm2AsciiName.empty() == false) {
      lWeightedTermSet.insert (lAdm2AsciiName);
      _spellingSet.insert (lAdm2AsciiName);
    }

    // Add the continent name
    const std::string& lContinentName = _location.getContinentName();
    lWeightedTermSet.insert (lContinentName);

    // Add the common name (usually in American English, but not necessarily
    // in ASCII).
    const std::string& lCommonName = _location.getCommonName();
    if (lCommonName.empty() == false) {
      addNameToXapianSets (K_DEFAULT_INDEXING_STD_WEIGHT,
                           LocationName_T (lCommonName),
                           FeatureCode_T (lFeatureCode),
                           lCityUtfNameList, lCityAsciiNameList,
                           Admin1UTFName_T (lAdm1UtfName),
                           Admin1ASCIIName_T (lAdm1AsciiName),
                           Admin2UTFName_T (lAdm2UtfName),
                           Admin2ASCIIName_T (lAdm2AsciiName),
                           StateCode_T (lStateCode),
                           CountryCode_T (lCountryCode),
                           CountryName_T (lCountryName),
                           ContinentName_T (lContinentName), iTransliterator);
    }
    
    // Add the ASCII name (not necessarily in English).
    const std::string& lASCIIName = _location.getAsciiName();
    if (lASCIIName.empty() == false) {
      addNameToXapianSets (K_DEFAULT_INDEXING_STD_WEIGHT,
                           LocationName_T (lASCIIName),
                           FeatureCode_T (lFeatureCode),
                           lCityUtfNameList, lCityAsciiNameList,
                           Admin1UTFName_T (lAdm1UtfName),
                           Admin1ASCIIName_T (lAdm1AsciiName),
                           Admin2UTFName_T (lAdm2UtfName),
                           Admin2ASCIIName_T (lAdm2AsciiName),
                           StateCode_T (lStateCode),
                           CountryCode_T (lCountryCode),
                           CountryName_T (lCountryName),
                           ContinentName_T (lContinentName), iTransliterator);
    }

    // Retrieve the place names in all the available languages
    const NameMatrix& lNameMatrixFull = _location.getNameMatrix();
    const NameMatrix_T& lNameMatrix = lNameMatrixFull.getNameMatrix();
    for (NameMatrix_T::const_iterator itNameList = lNameMatrix.begin();
         itNameList != lNameMatrix.end(); ++itNameList) {
      // Retrieve the language code and locale
      // const LanguageCode_T& lLanguage = itNameList->first;
      const Names& lNames = itNameList->second;

      // For a given language, retrieve the list of place names
      const NameList_T& lNameList = lNames.getNameList();
        
      for (NameList_T::const_iterator itName = lNameList.begin();
           itName != lNameList.end(); ++itName) {
        const std::string& lName = *itName;

        // Add the alternate name, which can be made of several words
        // (e.g., 'san francisco').
        if (lName.empty() == false) {
          // Create a list made of all the word combinations of the
          // initial string
          WordCombinationHolder lWordCombinationHolder (lName);

          // Browse the list of unique strings (word combinations)
          const WordCombinationHolder::StringList_T& lStringList =
            lWordCombinationHolder._list;
          for (WordCombinationHolder::StringList_T::const_iterator itString =
                 lStringList.begin();
               itString != lStringList.end(); ++itString) {
            const std::string& lWordCombination = *itString;
            const std::string& lNormalisedWordCombination =
              iTransliterator.normalise (lWordCombination);

            // Add that combination of words into the set of terms
            lStdTermSet.insert (lWordCombination);
            _spellingSet.insert (lWordCombination);

            // Add the normalised combination of words into the set of terms
            lStdTermSet.insert (lNormalisedWordCombination);
            _spellingSet.insert (lNormalisedWordCombination);
          }
        }
      }
    }

    // Insert (or replace) the newly created (or just altered) string set
    addTermSet (lPageRank, lWeightedTermSet);

    // Insert (or replace) the newly created (or just altered) string set
    addTermSet (K_DEFAULT_INDEXING_STD_WEIGHT, lStdTermSet);
  }

  // //////////////////////////////////////////////////////////////////////
  const Location& Place::completeLocation() {

    NameList_T lNameList;
    LanguageCode_T K_DEFAULT_LANGUAGE_CODE ("en_US");
    const bool hasFoundNameList = getNameList (K_DEFAULT_LANGUAGE_CODE,
                                               lNameList);

    if (hasFoundNameList == false) {
      //
      std::ostringstream errorStr;
      errorStr << "No list of names in (American) English ("
               << K_DEFAULT_LANGUAGE_CODE
               << " locale) can be found for the following place: "
               << toShortString();
      OPENTREP_LOG_ERROR (errorStr.str());
      // throw LanguageCodeNotDefinedInNameTableException (errorStr.str());
    }
    // assert (hasFoundNameList == true);

    // Add extra matching locations, whenever they exist
    if (_extraPlaceList.empty() == false) {
      for (PlaceOrderedList_T::const_iterator itLoc = _extraPlaceList.begin();
           itLoc != _extraPlaceList.end(); ++itLoc) {
        const Place* lExtraPlace_ptr = *itLoc;
        assert (lExtraPlace_ptr != NULL);

        // Add the extra matching location
        const Location& lExtraLocation = lExtraPlace_ptr->getLocation();
        _location.addExtraLocation (lExtraLocation);
      }
    }
    
    // Add alternate matching locations, whenever they exist
    if (_alternatePlaceList.empty() == false) {
      for (PlaceOrderedList_T::const_iterator itLoc =
             _alternatePlaceList.begin();
           itLoc != _alternatePlaceList.end(); ++itLoc) {
        const Place* lAlternatePlace_ptr = *itLoc;
        assert (lAlternatePlace_ptr != NULL);

        // Add the alternate matching location
        const Location& lAlternateLocation = lAlternatePlace_ptr->getLocation();
        _location.addAlternateLocation (lAlternateLocation);
      }
    }
    
    return _location;
  }
}
