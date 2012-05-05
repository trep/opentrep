// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// C
#include <cassert>
// OpenTrep BOM
#include <opentrep/bom/WordCombinationHolder.hpp>
#include <opentrep/bom/Place.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  Place::Place () :
    _world (NULL), _placeHolder (NULL), _mainPlace (NULL),
    _key ("", "", 0),
    _faaCode (""), _cityCode (""),
    _stateCode (""), _countryCode (""),
    _regionCode (""), _timeZoneGroup (""),
    _latitude (0.0), _longitude (0.0), _originalKeywords (""),
    _correctedKeywords (""), _docID (0),
    _percentage (0), _editDistance (0), _allowableEditDistance (0) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  Place::Place (const Place& iPlace) :
    _world (iPlace._world), _placeHolder (iPlace._placeHolder),
    _mainPlace (iPlace._mainPlace), _key (iPlace._key),
    _faaCode (iPlace._faaCode), _cityCode (iPlace._cityCode),
    _stateCode (iPlace._stateCode), _countryCode (iPlace._countryCode),
    _regionCode (iPlace._regionCode),
    _timeZoneGroup (iPlace._timeZoneGroup),
    _latitude (iPlace._latitude), _longitude (iPlace._longitude),
    _nameMatrix (iPlace._nameMatrix),
    _originalKeywords (iPlace._originalKeywords),
    _correctedKeywords (iPlace._correctedKeywords),
    _docID (iPlace._docID), _percentage (iPlace._percentage),
    _editDistance (iPlace._editDistance),
    _allowableEditDistance (iPlace._allowableEditDistance),
    _termSet (iPlace._termSet), _spellingSet (iPlace._spellingSet),
    _stemmingSet (iPlace._stemmingSet), _synonymSet (iPlace._synonymSet) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  Place::~Place () {
  }

  // //////////////////////////////////////////////////////////////////////
  bool Place::getNameList (const Language::EN_Language& iLanguageCode,
                           NameList_T& ioNameList) const {
    bool oFoundNameList = false;
    
    NameMatrix_T::const_iterator itNameList = _nameMatrix.find (iLanguageCode);
    if (itNameList != _nameMatrix.end()) {
      const Names& lNameList = itNameList->second;
      ioNameList = lNameList.getNameList();
      oFoundNameList = true;
    }

    return oFoundNameList;
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string Place::describeShortKey() const {
    std::ostringstream oStr;
    oStr << _key.describe();
    return oStr.str();
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string Place::describeKey() const {
    return describeShortKey();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string Place::toString() const {
    std::ostringstream oStr;
    oStr << describeShortKey();

    oStr << ", " << _faaCode
         << ", " << _cityCode << ", " << _stateCode
         << ", " << _countryCode << ", " << _regionCode
         << ", " << _timeZoneGroup
         << ", " << _latitude << ", " << _longitude
         << ", " << _originalKeywords << ", " << _correctedKeywords
         << ", " << _docID << ", " << _percentage
         << ", " << _editDistance << ", " << _allowableEditDistance
         << ". ";
    
    for (NameMatrix_T::const_iterator itNameList = _nameMatrix.begin();
         itNameList != _nameMatrix.end(); ++itNameList) {
      const Names& lNameList = itNameList->second;
      oStr << lNameList.toString();
    }
    
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
    /* When the city code is empty, it means that the place is a city and
       not an airport. The city code is thus the same as the place code
       itself. */
    std::ostringstream oStr;
    oStr << describeShortKey();

    const std::string& lCityCode = getCityCode();
    oStr << ", " << lCityCode << ", " << _stateCode
         << ", " << _countryCode << ", " << _regionCode
         << ", " << _timeZoneGroup
         << ", " << _latitude << ", " << _longitude
         << ", " << _originalKeywords << ", " << _correctedKeywords
         << ", " << _docID << ", " << _percentage
         << ", " << _editDistance << ", " << _allowableEditDistance;

    NameMatrix_T::const_iterator itNameHolder = _nameMatrix.begin();
    if (itNameHolder != _nameMatrix.end()) {

      const Names& lNameHolder = itNameHolder->second;
      const std::string& lFirstName = lNameHolder.getFirstName();

      if (lFirstName.empty() == false) {
        oStr << ", " << lFirstName << ".";
      }
    }
    
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
    oStr << describeKey();

    const std::string& lCityCode = getCityCode();
    oStr << ", FAA code = " << _faaCode
         << ", city code = " << lCityCode
         << ", state code = " << _stateCode
         << ", country code = " << _countryCode
         << ", region code = " << _regionCode
         << ", time zone group = " << _timeZoneGroup
         << ", latitude = " << _latitude
         << ", longitude = " << _longitude
         << ", original keywords = " << _originalKeywords
         << ", corrected keywords = " << _correctedKeywords
         << ", docID = " << _docID
         << ", percentage = " << _percentage << "%"
         << ", edit distance = " << _editDistance
         << ", allowable edit distance = " << _allowableEditDistance;
    return oStr.str();
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string Place::display() const {
    std::ostringstream oStr;
    oStr << shortDisplay() << std::endl;
    for (NameMatrix_T::const_iterator itNameList = _nameMatrix.begin();
         itNameList != _nameMatrix.end(); ++itNameList) {
      const Names& lNameList = itNameList->second;
      oStr << lNameList;
    }
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  void Place::addName (const Language::EN_Language& iLanguageCode,
                       const std::string& iName) {

    // Check whether a name in that language has already been recorded
    NameMatrix_T::iterator itNameList = _nameMatrix.find (iLanguageCode);
    if (itNameList != _nameMatrix.end()) {
      // Just add the name for that language
      Names& lNameList = itNameList->second;

      lNameList.addName (iName);

    } else {
      // Create a new name list for the given language
      Names lNameList (iLanguageCode);
      lNameList.addName (iName);

      // Insert the name list with the dedicated list
      const bool insertSucceeded =
        _nameMatrix.insert (NameMatrix_T::value_type (iLanguageCode,
                                                      lNameList)).second;
      if (insertSucceeded == false) {
        OPENTREP_LOG_ERROR ("The " << iName << " name can not be inserted in "
                            << "the dedicated list for the "
                            << Language::getLongLabel (iLanguageCode)
                            << " language");
      }
      assert (insertSucceeded == true);
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void Place::resetMatrix() {
    _nameMatrix.clear();
  }

  // //////////////////////////////////////////////////////////////////////
  void Place::resetIndexSets() {
    _termSet.clear();
    _spellingSet.clear();
    _stemmingSet.clear();
    _synonymSet.clear();
  }

  // //////////////////////////////////////////////////////////////////////
  void Place::buildIndexSets() {

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
    const std::string& lIataCode = _key.getIataCode();
    if (lIataCode.empty() == false) {
      _termSet.insert (lIataCode);
      _spellingSet.insert (lIataCode);
    }

    // Add the ICAO code
    const std::string& lIcaoCode = _key.getIcaoCode();
    if (lIcaoCode.empty() == false) {
      _termSet.insert (lIcaoCode);
      _spellingSet.insert (lIcaoCode);
    }

    // Add the city IATA code
    if (_cityCode.empty() == false && _cityCode != lIataCode) {
      _termSet.insert (_cityCode);
      _spellingSet.insert (_cityCode);
    }

    // Add the state code
    if (_stateCode.empty() == false) {
      _termSet.insert (_stateCode);
      _spellingSet.insert (_stateCode);
    }

    // Add the country code
    _termSet.insert (_countryCode);

    // Add the region code
    _termSet.insert (_regionCode);
      
    // Retrieve the place names in all the available languages
    for (NameMatrix_T::const_iterator itNameList = _nameMatrix.begin();
         itNameList != _nameMatrix.end(); ++itNameList) {
      // Retrieve the language code and locale
      // const Language::EN_Language& lLanguage = itNameList->first;
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

            // Add that combination of words into the set of terms
            _termSet.insert (lWordCombination);
            _spellingSet.insert (lWordCombination);
          }
        }
      }
    }
  }

  // //////////////////////////////////////////////////////////////////////
  Location Place::createLocation() const {

    const std::string& lCityCode = getCityCode();

    NameList_T lNameList;
    const bool hasFoundNameList = getNameList (Language::en_US, lNameList);

    if (hasFoundNameList == false) {
      //
      std::ostringstream errorStr;
      errorStr << "No list of names in (American) English (en_US "
               << "locale) can be found for the following place: "
               << toShortString();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw LanguageCodeNotDefinedInNameTableException (errorStr.str());
    }
    assert (hasFoundNameList == true);

    // Copy the parameters from the Place object to the Location structure
    Location oLocation (_key.getIataCode(), _key.getIcaoCode(),
                        _key.getGeonamesID(), _faaCode, lCityCode,
                        _stateCode, _countryCode,
                        _regionCode, _timeZoneGroup,
                        _latitude, _longitude, lNameList,
                        _originalKeywords, _correctedKeywords,
                        _percentage, _editDistance, _allowableEditDistance);

    // Add extra matching locations, whenever they exist
    if (_extraPlaceList.empty() == false) {
      for (PlaceOrderedList_T::const_iterator itLoc = _extraPlaceList.begin();
           itLoc != _extraPlaceList.end(); ++itLoc) {
        const Place* lExtraPlace_ptr = *itLoc;
        assert (lExtraPlace_ptr != NULL);

        // Add the extra matching location
        const Location& lExtraLocation = lExtraPlace_ptr->createLocation();
        oLocation.addExtraLocation (lExtraLocation);
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
        const Location& lAlternateLocation =
          lAlternatePlace_ptr->createLocation();
        oLocation.addAlternateLocation (lAlternateLocation);
      }
    }
    
    return oLocation;
  }
}
