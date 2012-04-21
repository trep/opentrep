// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// C
#include <cassert>
// OpenTrep BOM
#include <opentrep/bom/Place.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  Place::Place () :
    _world (NULL), _placeHolder (NULL), _mainPlace (NULL),
    _iataCode (""), _icaoCode (""), _faaCode (""), _cityCode (""),
    _stateCode (""), _countryCode (""),
    _regionCode (""), _continentCode (""), _timeZoneGroup (""),
    _longitude (0.0), _latitude (0.0), _originalKeywords (""),
    _correctedKeywords (""), _docID (0),
    _percentage (0), _editDistance (0), _allowableEditDistance (0) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  Place::Place (const Place& iPlace) :
    _world (iPlace._world), _placeHolder (iPlace._placeHolder),
    _mainPlace (iPlace._mainPlace),
    _iataCode (iPlace._iataCode), _icaoCode (iPlace._icaoCode),
    _faaCode (iPlace._faaCode), _cityCode (iPlace._cityCode),
    _stateCode (iPlace._stateCode), _countryCode (iPlace._countryCode),
    _regionCode (iPlace._regionCode), _continentCode (iPlace._continentCode),
    _timeZoneGroup (iPlace._timeZoneGroup), _longitude (iPlace._longitude),
    _latitude (iPlace._latitude), _nameMatrix (iPlace._nameMatrix),
    _originalKeywords (iPlace._originalKeywords),
    _correctedKeywords (iPlace._correctedKeywords),
    _docID (iPlace._docID), _percentage (iPlace._percentage),
    _editDistance (iPlace._editDistance),
    _allowableEditDistance (iPlace._allowableEditDistance) {
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
    oStr << _iataCode;
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

    oStr << ", " << _icaoCode << ", " << _faaCode
         << ", " << _cityCode << ", " << _stateCode
         << ", " << _countryCode << ", " << _regionCode
         << ", " << _continentCode << ", " << _timeZoneGroup
         << ", " << _longitude << ", " << _latitude
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
         << ", " << _continentCode << ", " << _timeZoneGroup
         << ", " << _longitude << ", " << _latitude
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
  std::string Place::shortDisplay() const {
    /* When the city code is empty, it means that the place is a city and
       not an airport. The city code is thus the same as the place code
       itself. */
    std::ostringstream oStr;
    oStr << describeKey();

    const std::string& lCityCode = getCityCode();
    oStr << ", ICAO code = " << _icaoCode
         << ", FAA code = " << _faaCode
         << ", city code = " << lCityCode
         << ", state code = " << _stateCode
         << ", country code = " << _countryCode
         << ", region code = " << _regionCode
         << ", continent code = " << _continentCode
         << ", time zone group = " << _timeZoneGroup
         << ", longitude = " << _longitude
         << ", latitude = " << _latitude
         << ", original keywords = " << _originalKeywords
         << ", corrected keywords = " << _correctedKeywords
         << ", docID = " << _docID
         << ", percentage = " << _percentage << "%"
         << ", edit distance = " << _editDistance
         << ", allowable edit distance = " << _allowableEditDistance
         << std::endl;
    return oStr.str();
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string Place::display() const {
    std::ostringstream oStr;
    oStr << shortDisplay();
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
    Location oLocation (_iataCode, _icaoCode, _faaCode, lCityCode,
                        _stateCode, _countryCode,
                        _regionCode, _continentCode, _timeZoneGroup,
                        _longitude, _latitude, lNameList,
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
