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
#include <opentrep/IATAType.hpp>

namespace OPENTREP {

  /**
   * @brief Class modelling the primary key of a location/POR (point of
   *        reference).
   *
   * The primary key is composed of:
   * <ul>
   *   <li>IATA 3-letter code. See IATACode_T for more details.</li>
   *   <li>Travel type. See IATAType for more details.</li>
   *   <li>Geonames ID, when referenced by Geonames (0 otherwise).
   *       See GeonamesID_T for more details.</li>
   * </ul>
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
     * Get the IATA location type (e.g., A for airport).
     */
    const IATAType& getIataType() const {
      return _iataType;
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
     * Set the IATA location type (e.g., A for airport).
     */
    void setIataType (const IATAType& iIATAType) {
      _iataType = iIATAType;
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
    LocationKey (const IATACode_T&, const IATAType&, const GeonamesID_T&);

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
     * IATA three-letter code (e.g., ORD, CHI, SFO, LAX, LHR, LON, CDG, PAR).
     *
     * See IATACode_T for more details.
     */
    IATACode_T _iataCode;

    /**
     * IATA location type (e.g., A for airport).
     *
     * See IATAType for more details.
     */
    IATAType _iataType;

    /**
     * Geonames ID (e.g., 4887479).
     *
     * See GeonamesID_T for more details.
     */
    GeonamesID_T _geonamesID;

    /**
     * Whether or not that place is referenced by Geonames.
     */
    IsGeonames_T _isGeonames;
  };

}
#endif // __OPENTREP_LOCATIONKEY_HPP
