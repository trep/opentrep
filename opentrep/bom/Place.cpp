// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// OpenTrep
#include <opentrep/basic/BasConst_General.hpp>
#include <opentrep/basic/OTransliterator.hpp>
#include <opentrep/bom/WordCombinationHolder.hpp>
#include <opentrep/bom/Place.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  Place::Place() :
    _world (NULL), _placeHolder (NULL), _mainPlace (NULL),
    _location (IATACode_T (""), ICAOCode_T (""), 0,
               CommonName_T (""), ASCIIName_T (""),
               FAACode_T (""), CityCode_T (""), StateCode_T (""),
               CountryCode_T (""), AltCountryCode_T (""), RegionCode_T (""), 
               0.0, 0.0,
               FeatureClass_T (""), FeatureCode_T (""), IATAType_T (""),
               Admin1Code_T (""), Admin2Code_T (""),
               Admin3Code_T (""), Admin4Code_T (""),
               0, 0, 0, TimeZone_T (""), 0, 0, 0,
               Date_T (2000, 01, 01), false, false,
               WikiLink_T (""),  K_DEFAULT_PAGE_RANK, "", "", 0, 0, 0),
    _docID (0) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  Place::Place (const LocationKey& iKey) :
    _world (NULL), _placeHolder (NULL), _mainPlace (NULL),
    _location (iKey.getIataCode(), iKey.getIcaoCode(), iKey.getGeonamesID(),
               CommonName_T (""), ASCIIName_T (""),
               FAACode_T (""), CityCode_T (""), StateCode_T (""),
               CountryCode_T (""), AltCountryCode_T (""), RegionCode_T (""), 
               0.0, 0.0,
               FeatureClass_T (""), FeatureCode_T (""), IATAType_T (""),
               Admin1Code_T (""), Admin2Code_T (""),
               Admin3Code_T (""), Admin4Code_T (""),
               0, 0, 0, TimeZone_T (""), 0, 0, 0,
               Date_T (2000, 01, 01), false, false,
               WikiLink_T (""), K_DEFAULT_PAGE_RANK, "", "", 0, 0, 0),
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
    _termSet (iPlace._termSet), _spellingSet (iPlace._spellingSet),
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
    short idx = 0;
    for (Place::StringSet_T::const_iterator itString = _termSet.begin();
         itString != _termSet.end(); ++itString, ++idx) {
      if (idx != 0) {
        oStr << ", ";
      }
      const std::string& lString = *itString;
      oStr << lString;
    }

    // Xapian spelling dictionary
    oStr << "; [spelling] ";
    idx = 0;
    for (Place::StringSet_T::const_iterator itTerm = _spellingSet.begin();
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
    _termSet.clear();
    _spellingSet.clear();
    _stemmingSet.clear();
    _synonymSet.clear();
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

    // Add the IATA code
    const std::string& lIataCode = _location.getIataCode();
    if (lIataCode.empty() == false) {
      _termSet.insert (lIataCode);
      _spellingSet.insert (lIataCode);
    }

    // Add the ICAO code
    const std::string& lIcaoCode = _location.getIcaoCode();
    if (lIcaoCode.empty() == false) {
      _termSet.insert (lIcaoCode);
      _spellingSet.insert (lIcaoCode);
    }

    // Add the city IATA code
    const std::string& lCityCode = _location.getCityCode();
    if (lCityCode.empty() == false && lCityCode != lIataCode) {
      _termSet.insert (lCityCode);
      _spellingSet.insert (lCityCode);
    }

    // Add the state code
    const std::string& lStateCode = _location.getStateCode();
    if (lStateCode.empty() == false) {
      _termSet.insert (lStateCode);
      _spellingSet.insert (lStateCode);
    }

    // Add the country code
    const std::string& lCountryCode = _location.getCountryCode();
    _termSet.insert (lCountryCode);

    // Add the region code
    const std::string& lRegionCode = _location.getRegionCode();
    _termSet.insert (lRegionCode);

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
            _termSet.insert (lWordCombination);
            _spellingSet.insert (lWordCombination);

            // Add the normalised combination of words into the set of terms
            _termSet.insert (lNormalisedWordCombination);
            _spellingSet.insert (lNormalisedWordCombination);
          }
        }
      }
    }
  }

  // //////////////////////////////////////////////////////////////////////
  const Location& Place::completeLocation() {

    NameList_T lNameList;
    LanguageCode_T K_DEFAULT_LANGUAGE_CODE ("en_US");
    const bool hasFoundNameList = getNameList (K_DEFAULT_LANGUAGE_CODE, lNameList);

    if (hasFoundNameList == false) {
      //
      std::ostringstream errorStr;
      errorStr << "No list of names in (American) English (" << K_DEFAULT_LANGUAGE_CODE
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
