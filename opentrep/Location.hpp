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
#include <opentrep/LocationList.hpp>
#include <opentrep/OPENTREP_Abstract.hpp>

namespace OPENTREP {

  /** List of names for a given (geographical) location. */
  typedef std::list<std::string> LocationNameList_T;

  
  /** Structure modelling a (geographical) location. */
  struct Location : public OPENTREP_Abstract {
  public:
    // ///////// Getters ////////
    /** Get the Location code. */
    std::string getLocationCode() const {
      return _locationCode;
    }
    
    /** Get the City code. */
    std::string getCityCode() const {
      return _cityCode;
    }
    
    /** Get the State code. */
    std::string getStateCode() const {
      return _stateCode;
    }
    
    /** Get the Country code. */
    std::string getCountryCode() const {
      return _countryCode;
    }
    
    /** Get the Region code. */
    std::string getRegionCode() const {
      return _regionCode;
    }
    
    /** Get the Continent code. */
    std::string getContinentCode() const {
      return _continentCode;
    }
    
    /** Get the Time-zone group. */
    std::string getTimeZoneGroup() const {
      return _timeZoneGroup;
    }
    
    /** Get the Longitude. */
    double getLongitude() const {
      return _longitude;
    }
    
    /** Get the Latitude. */
    double getLatitude() const {
      return _latitude;
    }

    /** Get the list of (American) English names for that location. */
    const LocationNameList_T& getNameList () const {
      return _nameList;
    }
    
    /** Get the original keywords. */
    std::string getOriginalKeywords() const {
      return _originalKeywords;
    }
    
    /** Get the corrected keywords. */
    std::string getCorrectedKeywords() const {
      return _correctedKeywords;
    }
    
    /** Get the matching percentage. */
    const MatchingPercentage_T& getPercentage() const {
      return _percentage;
    }

    /** Get the allowed edit distance/error. */
    const NbOfErrors_T& getEditDistance() const {
      return _editDistance;
    }

    /** Get the maximal allowable edit distance/error, with which the
        matching has been made. */
    const NbOfErrors_T& getAllowableEditDistance () const {
      return _allowableEditDistance;
    }
    
    /** Get the list of extra matching (similar) locations. */
    const LocationList_T& getExtraLocationList() const {
      return _extraLocationList;
    }

    /** Get the list of alternate matching (less similar) locations. */
    const LocationList_T& getAlternateLocationList() const {
      return _alternateLocationList;
    }

    
    // ///////// Setters //////////
    /** Set the Location code. */
    void setLocationCode (const std::string& iLocationCode) {
      _locationCode = iLocationCode;
    }
    
    /** Set the City code. */
    void setCityCode (const std::string& iCityCode) {
      _cityCode = iCityCode;
    }
    
    /** Set the State code. */
    void setStateCode (const std::string& iStateCode) {
      _stateCode = iStateCode;
    }
    
    /** Set the Country code. */
    void setCountryCode (const std::string& iCountryCode) {
      _countryCode = iCountryCode;
    }
    
    /** Set the Region code. */
    void setRegionCode (const std::string& iRegionCode) {
      _regionCode = iRegionCode;
    }
    
    /** Set the Continent code. */
    void setContinentCode (const std::string& iContinentCode) {
      _continentCode = iContinentCode;
    }
    
    /** Set the Time-zone group. */
    void setTimeZoneGroup (const std::string& iTimeZoneGroup) {
      _timeZoneGroup = iTimeZoneGroup;
    }
    
    /** Set the Longitude. */
    void setLongitude (const double& iLongitude) {
      _longitude = iLongitude;
    }
    
    /** Set the Latitude. */
    void setLatitude (const double& iLatitude) {
      _latitude = iLatitude;
    }

    /** Set the list of (American) English names for that location. */
    void setNameList (const LocationNameList_T& iNameList) {
      _nameList = iNameList;
    }

    /** Set the original keywords. */
    void setOriginalKeywords (const std::string& iOriginalKeywords) {
      _originalKeywords = iOriginalKeywords;
    }
    
    /** Set the corrected keywords. */
    void setCorrectedKeywords (const std::string& iCorrectedKeywords) {
      _correctedKeywords = iCorrectedKeywords;
    }
    
    /** Set the Xapian matching percentage. */
    void setPercentage (const MatchingPercentage_T& iPercentage) {
      _percentage = iPercentage;
    }

    /** Set the allowed edit distance/error. */
    void setEditDistance (const NbOfErrors_T& iEditDistance) {
      _editDistance = iEditDistance;
    }

    /** Set the maxiaml allowable edit distance/error, with which the
        matching has been made. */
    void setAllowableEditDistance (const NbOfErrors_T& iAllowableEditDistance) {
      _allowableEditDistance = iAllowableEditDistance;
    }
    
    /** Add an extra matching location. */
    void addExtraLocation (const Location& iExtraLocation) {
      _extraLocationList.push_back (iExtraLocation);
    }
    
    /** Add an alternate matching location. */
    void addAlternateLocation (const Location& iAlternateLocation) {
      _alternateLocationList.push_back (iAlternateLocation);
    }
    

  public:
    // ///////// Display methods ////////
    /** Dump a structure into an output stream.
        @param ostream& the output stream. */
    void toStream (std::ostream& ioOut) const {
      ioOut << toString();
    }

    /** Read a structure from an input stream.
        @param istream& the input stream. */
    void fromStream (std::istream&) {
    }

    /** Get a short display of the Location structure. */
    std::string toShortString() const {
      std::ostringstream oStr;
      oStr << _locationCode << ", " << _cityCode << ", " << _stateCode
           << ", " << _countryCode << ", " << _regionCode
           << ", " << _continentCode << ", " << _timeZoneGroup
           << ", " << _longitude << ", " << _latitude
           << ", " << _originalKeywords << ", " << _correctedKeywords
           << ", " << _percentage
           << ", " << _editDistance << ", " << _allowableEditDistance;

      if (_extraLocationList.empty() == false) {
        oStr << " with " << _extraLocationList.size() << " extra match(es)";
      }
      
      if (_alternateLocationList.empty() == false) {
        oStr << " with " << _alternateLocationList.size()
             << " alternate match(es)";
      }
      
      return oStr.str();
    }
    
    /** Get the serialised version of the Location structure. */
    std::string toString() const {
      std::ostringstream oStr;
      oStr << toShortString();
      for (LocationNameList_T::const_iterator itName = _nameList.begin();
           itName != _nameList.end(); ++itName) {
        oStr << ", " << *itName;
      }

      if (_extraLocationList.empty() == false) {
        oStr << "; Extra matches: {";
        unsigned short idx = 0;
        for (LocationList_T::const_iterator itLoc = _extraLocationList.begin();
             itLoc != _extraLocationList.end(); ++itLoc, ++idx) {
          if (idx != 0) {
            oStr << ", ";
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
            oStr << ", ";
          }
          const Location& lAlternateLocation = *itLoc;
          oStr << lAlternateLocation.toShortString();
        }
        oStr << "}";
      }
      
      return oStr.str();
    }

    
  public:
    /** Main Constructor. */
    Location (const std::string& iPlaceCode, const std::string& iCityCode,
              const std::string& iStateCode, const std::string& iCountryCode,
              const std::string& iRegionCode, const std::string& iContinentCode,
              const std::string& iTimeZoneGroup, 
              const double iLongitude, const double iLatitude,
              const LocationNameList_T& iNameList,
              const std::string& iOriginalKeywords,
              const std::string& iCorrectedKeywords,
              const MatchingPercentage_T& iPercentage,
              const NbOfErrors_T& iEditDistance,
              const NbOfErrors_T& iAllowableEditDistance)
      : _locationCode (iPlaceCode), _cityCode (iCityCode),
        _stateCode (iStateCode), _countryCode (iCountryCode),
        _regionCode (iRegionCode), _continentCode (iContinentCode),
        _timeZoneGroup (iTimeZoneGroup), _longitude (iLongitude),
        _latitude (iLatitude), _nameList (iNameList),
        _originalKeywords (iOriginalKeywords),
        _correctedKeywords (iCorrectedKeywords),
        _percentage (iPercentage), _editDistance (iEditDistance),
        _allowableEditDistance (iAllowableEditDistance) {
    }

    /** Default Constructor. */
    // Location ();
    /** Default copy constructor. */
    // Location (const Location&);
    
    /** Destructor. */
    virtual ~Location() {}

    
  private:
    // /////// Attributes /////////
    /** Location code. */
    std::string _locationCode;
    /** City code. */
    std::string _cityCode;
    /** State code. */
    std::string _stateCode;
    /** Country code. */
    std::string _countryCode;
    /** Region code. */
    std::string _regionCode;
    /** Continent code. */
    std::string _continentCode;
    /** Time-zone group. */
    std::string _timeZoneGroup;
    /** Longitude. */
    double _longitude;
    /** Latitude. */
    double _latitude;
    /** List of (American) English names. */
    LocationNameList_T _nameList;

    /** Original keywords. */
    std::string _originalKeywords;
    
    /** Original keywords. */
    std::string _correctedKeywords;
    
    /** Matching percentage. */
    MatchingPercentage_T _percentage;

    /** Allowed edit error/distance. */
    NbOfErrors_T _editDistance;

    /** Maximum allowable edit distance/error, with which the matching
        has been made. */
    NbOfErrors_T _allowableEditDistance;
    
    /** List of extra matching (similar) locations. */
    LocationList_T _extraLocationList;

    /** List of alternate matching (less similar) locations. */
    LocationList_T _alternateLocationList;
  };

}
#endif // __OPENTREP_LOCATION_HPP
