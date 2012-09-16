#ifndef __OPENTREP_LOCATIONKEY_HPP
#define __OPENTREP_LOCATIONKEY_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
#include <string>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/OPENTREP_Abstract.hpp>

namespace OPENTREP {

  /**
   * @brief Class modelling the primary key of a location/POR (point of
   *        reference).
   */
  struct LocationKey : public OPENTREP_Abstract {
  public:
    // //////////////// Getters ///////////////
    /**
     * Get the IATA code.
     */
    const IATACode_T& getIataCode() const {
      return _iataCode;
    }
    
    /**
     * Get the ICAO code.
     */
    const ICAOCode_T& getIcaoCode() const {
      return _icaoCode;
    }
    
    /**
     * Get the Geonames ID.
     */
    const GeonamesID_T& getGeonamesID() const {
      return _geonamesID;
    }

    /**
     * State whether that POR is referenced by Geonames.
     */
    const IsGeonames_T& isGeonames() const {
      return _isGeonames;
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
     * Set the ICAO code.
     */
    void setIcaoCode (const std::string& iIcaoCode) {
      _icaoCode = ICAOCode_T (iIcaoCode);
    }
    
    /**
     * Set the Geonames ID.
     */
    void setGeonamesID (const GeonamesID_T& iGeonamesID) {
      _geonamesID = iGeonamesID;
    }    

    /**
     * Set the Geonames flag.
     */
    void setIsGeonames (const IsGeonames_T& isGeonames) {
      _isGeonames = isGeonames;
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
    LocationKey (const IATACode_T&, const ICAOCode_T&, const GeonamesID_T&);

    /**
     * Default copy constructor.
     */
    LocationKey (const LocationKey&);

    /**
     * Destructor.
     */
    ~LocationKey();

    /**
     * Comparison operator.
     */
    bool operator== (const LocationKey&) const;

    /**
     * Comparison operator.
     */
    bool operator!= (const LocationKey& iLocationKey) const {
      return !(iLocationKey == *this);
    }

  private:
    /**
     * Default constructor.
     */
    LocationKey();


  private:
    // //////////////////// Attributes ///////////////////////
    /**
     * IATA code (e.g., ORD, CHI, SFO, LAX, LHR, LON, CDG, PAR).
     */
    IATACode_T _iataCode;

    /**
     * ICAO code (e.g., KORD).
     */
    ICAOCode_T _icaoCode;

    /**
     * Geonames ID (e.g., 4887479).
     */
    GeonamesID_T _geonamesID;

    /**
     * Whether or not that POR is referenced by Geonames
     */
    IsGeonames_T _isGeonames;
  };

}
#endif // __OPENTREP_LOCATIONKEY_HPP
