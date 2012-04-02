#ifndef __OPENTREP_BOM_PLACE_HPP
#define __OPENTREP_BOM_PLACE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
#include <string>
#include <map>
// OpenTrep Bom
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/Location.hpp>
#include <opentrep/bom/BomAbstract.hpp>
#include <opentrep/bom/Names.hpp>
#include <opentrep/bom/PlaceList.hpp>

namespace OPENTREP {

  // Forward declarations
  class World;
  class PlaceHolder;
  
  /** Class modelling a place. */
  class Place : public BomAbstract {
    friend class FacWorld;
    friend class FacPlace;
    friend class FacPlaceHolder;
    friend class DbaPlace;
  public:
    // ///////// Getters ////////
    /** Get the Place code. */
    const std::string& getPlaceCode() const {
      return _placeCode;
    }
    
    /** Get the City code. */
    std::string getCityCode() const {
      return _cityCode;
    }

    /** Get the City name. */
    const std::string& getCityName() const {
      return _cityName;
    }
    
    /** Get the State code. */
    const std::string& getStateCode() const {
      return _stateCode;
    }
    
    /** Get the Country code. */
    const std::string& getCountryCode() const {
      return _countryCode;
    }
    
    /** Get the Region code. */
    const std::string& getRegionCode() const {
      return _regionCode;
    }
    
    /** Get the Continent code. */
    const std::string& getContinentCode() const {
      return _continentCode;
    }
    
    /** Get the Time-zone group. */
    const std::string& getTimeZoneGroup() const {
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

    /** Get the original keywords. */
    std::string getOriginalKeywords() const {
      return _originalKeywords;
    }
    
    /** Get the corrected keywords. */
    std::string getCorrectedKeywords() const {
      return _correctedKeywords;
    }
    
    /** Get the Xapian document ID. */
    const XapianDocID_T& getDocID() const {
      return _docID;
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
    
    /** Get the map of name lists. */
    const NameMatrix_T& getNameMatrix () const {
      return _nameMatrix;
    }

    /** Get, for a given language (code), the corresponding list of names.
        @param const Language::EN_Language& Language code.
        @param NameList_T& Empty list of names, which will be filled by the
               method if a list exists for that language code.
        @return bool Whether or not such a list exists for the given
                language. */
    bool getNameList (const Language::EN_Language&, NameList_T&) const;

    /** Get the list of extra matching (similar) places. */
    const PlaceOrderedList_T& getExtraPlaceList() const {
      return _extraPlaceList;
    }

    /** Get the list of alternate matching (less similar) places. */
    const PlaceOrderedList_T& getAlternatePlaceList() const {
      return _alternatePlaceList;
    }

    
    // ///////// Setters ////////
    /** Set the Place code. */
    void setPlaceCode (const std::string& iPlaceCode) {
      _placeCode = iPlaceCode;
    }
    
    /** Set the City code. */
    void setCityCode (const std::string& iCityCode) {
      _cityCode = iCityCode;
    }
    
    /** Set the City name. */
    void setCityName (const std::string& iCityName) {
      _cityName = iCityName;
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

    /** Set the original keywords. */
    void setOriginalKeywords (const std::string& iOriginalKeywords) {
      _originalKeywords = iOriginalKeywords;
    }
    
    /** Set the corrected keywords. */
    void setCorrectedKeywords (const std::string& iCorrectedKeywords) {
      _correctedKeywords = iCorrectedKeywords;
    }
    
    /** Set the Xapian document ID. */
    void setDocID (const XapianDocID_T& iDocID) {
      _docID = iDocID;
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

    
  public:
    // ////////// Setters in underlying names ////////
    /** Add a name to the place. */
    void addName (const Language::EN_Language&, const std::string& iName);

    /** Reset the map of name lists. */
    void resetMatrix();

    
  public:
    // /////////// Business methods /////////
    /** Create a Location structure, which is a light copy
        of the Place object. That (Location) structure is passed
        back to the caller of the service. */
    Location createLocation() const;


  public:
    // ///////// Display methods ////////
    /** Dump a Business Object into an output stream.
        @param ostream& the output stream. */
    void toStream (std::ostream&) const;

    /** Read a Business Object from an input stream.
        @param istream& the input stream. */
    void fromStream (std::istream&);

    /** Get the serialised version of the Place object. */
    std::string toString() const;

    /** Get a short display of the Business Object. */
    std::string toShortString() const;
    
    /** Get a string describing the whole key (differentiating two objects
        at any level). */
    std::string describeKey() const;

    /** Get a string describing the short key (differentiating two objects
        at the same level). */
    std::string describeShortKey() const;
    
    /** Display the full Place context. */
    std::string display() const;

    /** Display a short Place context. */
    std::string shortDisplay() const;

    
  private:
    /** Constructor. */
    Place ();
    Place (const Place&);
    
    /** Destructor. */
    virtual ~Place();

  private:
    /** Parent World. */
    World* _world;
    
    /** Parent PlaceHolder (not always defined,for instance if the
        current Place object is an extra or alternate one). */
    PlaceHolder* _placeHolder;
    
    /** Parent (main) Place (not always defined,for instance if the
        current Place object is itself a main one). */
    Place* _mainPlace;
    
  private:
    // /////// Attributes /////////
    /** Place code. */
    std::string _placeCode;
    /** City code. */
    std::string _cityCode;
    /** City name. */
    std::string _cityName;
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
    /** List of names, for each given language. */
    NameMatrix_T _nameMatrix;

    /** Original keywords. */
    std::string _originalKeywords;
    
    /** Original keywords. */
    std::string _correctedKeywords;
    
    /** Xapian document ID. */
    XapianDocID_T _docID;

    /** Matching percentage. */
    MatchingPercentage_T _percentage;

    /** Allowed edit error/distance. */
    NbOfErrors_T _editDistance;

    /** Maximum allowable edit distance/error, with which the matching
        has been made. */
    NbOfErrors_T _allowableEditDistance;
    
    /** List of extra matching (similar) places. */
    PlaceOrderedList_T _extraPlaceList;

    /** List of alternate matching (less similar) places. */
    PlaceOrderedList_T _alternatePlaceList;
  };

}
#endif // __OPENTREP_BOM_PLACE_HPP
