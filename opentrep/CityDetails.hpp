#ifndef __OPENTREP_CITYDETAILS_HPP
#define __OPENTREP_CITYDETAILS_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
#include <string>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/OPENTREP_Abstract.hpp>
#include <opentrep/IATAType.hpp>

namespace OPENTREP {

  /**
   * @brief Class modelling the elementary details of a city.
   *
   * The CityDetails structure contains:
   * <ul>
   *  <li>IATA three-letter code</li>
   *  <li>Geonames ID</li>
   *  <li>Most common used name, in UTF8</li>
   *  <li>Most common used name, transliterated in ASCII</li>
   *  <li>Country code (of the city)</li>
   *  <li>State (ISO-3166-2) code (of the city)</li>
   * </ul>
   */
  struct CityDetails : public OPENTREP_Abstract {
  public:
    // //////////////// Getters ///////////////
    /**
     * Get the IATA code.
     */
    const IATACode_T& getIataCode() const {
      return _iataCode;
    }
    
    /**
     * Get the Geonames ID.
     */
    const GeonamesID_T& getGeonamesID() const {
      return _geonamesID;
    }

    /**
     * Get the name in UTF8.
     */
    const CityUTFName_T& getUtfName() const {
      return _utfName;
    }
    
    /**
     * Get the name in ASCII (not necessarily in English).
     * That corresponds to the transliterated version of the Unicode one.
     */
    const CityASCIIName_T& getAsciiName() const {
      return _asciiName;
    }
    
    /**
     * Get the country code (of the city).
     */
    const CountryCode_T& getCountryCode() const {
      return _countryCode;
    }
    
    /**
     * Get the state (ISO-3166-2) code (of the city).
     */
    const StateCode_T& getStateCode() const {
      return _stateCode;
    }
    

  public:
    // ////////////////// Setters /////////////////
    /**
     * Set the IATA code.
     */
    void setIataCode (const std::string& iIataCode) {
      _iataCode = IATACode_T (iIataCode);
    }
    
    /**
     * Set the Geonames ID.
     */
    void setGeonamesID (const GeonamesID_T& iGeonamesID) {
      _geonamesID = iGeonamesID;
    }    

    /**
     * Set the name in UTF8.
     */
    void setUtfName (const CityUTFName_T& iUtfName) {
      _utfName = iUtfName;
    }
    
    /**
     * Set the name in ASCII (not necessarily in English).
     * That corresponds to the transliterated version of the Unicode one.
     */
    void setAsciiName (const CityASCIIName_T& iAsciiName) {
      _asciiName = iAsciiName;
    }
    
    /**
     * Set the country code. 
     */
    void setCountryCode (const std::string& iCountryCode) {
      _countryCode = CountryCode_T (iCountryCode);
    }
    
    /**
     * Set the state code. 
     */
    void setStateCode (const std::string& iStateCode) {
      _stateCode = StateCode_T (iStateCode);
    }
    

  public:
    // ////////////// Display methods //////////////
    /**
     * Dump the structure into an output stream.
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
     * Get the serialised version of the structure.
     */
    std::string toString() const;
    
    /**
     * Get a string describing the whole key (differentiating two objects
     * at any level).
     */
    std::string describe() const;


  public:
    // ////////////// Constructors and destructors //////////////
    /**
     * Main constructor.
     */
    CityDetails (const IATACode_T&, const GeonamesID_T&,
                 const CityUTFName_T&, const CityASCIIName_T&,
                 const CountryCode_T&, const StateCode_T&);

    /**
     * Default copy constructor.
     */
    CityDetails (const CityDetails&);

    /**
     * Destructor.
     */
    ~CityDetails();

    /**
     * Comparison operator.
     */
    bool operator== (const CityDetails&) const;

    /**
     * Comparison operator.
     */
    bool operator!= (const CityDetails& iCityDetails) const {
      return !(iCityDetails == *this);
    }

  private:
    /**
     * Default constructor.
     */
    CityDetails();


  private:
    // //////////////////// Attributes ///////////////////////
    /**
     * IATA three-letter code (e.g., ORD, CHI, SFO, LAX, LHR, LON, CDG, PAR).
     *
     * See IATACode_T for more details.
     */
    IATACode_T _iataCode;

    /**
     * Geonames ID (e.g., 4887479).
     *
     * \note The city Geonames ID is null when the related Location
     *       structure is itself a city.
     */
    GeonamesID_T _geonamesID;

    /**
     * Name in UTF8.
     */
    CityUTFName_T _utfName;
    
    /**
     * Name in ASCII (not necessarily in English).
     * That corresponds to the transliterated version of the Unicode one.
     */
    CityASCIIName_T _asciiName;

    /**
     * Country code (of the city).
     */
    CountryCode_T _countryCode;
    
    /**
     * State (ISO-3166-2) code (of the city).
     */
    StateCode_T _stateCode;
    
  };

}
#endif // __OPENTREP_CITYDETAILS_HPP
