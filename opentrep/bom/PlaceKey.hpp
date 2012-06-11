#ifndef __OPENTREP_BOM_PLACEKEY_HPP
#define __OPENTREP_BOM_PLACEKEY_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
#include <string>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/bom/KeyAbstract.hpp>

namespace OPENTREP {

  /**
   * @brief Class modelling the primary key of a place/POR (point of reference).
   */
  struct PlaceKey : public KeyAbstract {
  public:
    // //////////////// Getters ///////////////
    /**
     * Get the IATA code.
     */
    const std::string& getIataCode() const {
      return _iataCode;
    }
    
    /**
     * Get the ICAO code.
     */
    const std::string& getIcaoCode() const {
      return _icaoCode;
    }
    
    /**
     * Get the Geonames ID.
     */
    const GeonamesID_T& getGeonamesID() const {
      return _geonamesID;
    }


  public:
    // ////////////////// Setters /////////////////
    /**
     * Set the IATA code.
     */
    void setIataCode (const std::string& iIataCode) {
      _iataCode = iIataCode;
    }
    
    /**
     * Set the ICAO code.
     */
    void setIcaoCode (const std::string& iIcaoCode) {
      _icaoCode = iIcaoCode;
    }
    
    /**
     * Get the Geonames ID.
     */
    void setGeonamesID (const GeonamesID_T& iGeonamesID) {
      _geonamesID = iGeonamesID;
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
    PlaceKey (const std::string& iIataCode, const std::string& iIcaoCode,
              const GeonamesID_T&);

    /**
     * Default copy constructor.
     */
    PlaceKey (const PlaceKey&);

    /**
     * Destructor.
     */
    ~PlaceKey();

    /**
     * Comparison operator.
     */
    bool operator== (const PlaceKey&) const;

    /**
     * Comparison operator.
     */
    bool operator!= (const PlaceKey& iPlaceKey) const {
      return !(iPlaceKey == *this);
    }

  private:
    /**
     * Default constructor.
     */
    PlaceKey();


  private:
    // //////////////////// Attributes ///////////////////////
    /**
     * IATA code (e.g., ORD, CHI, SFO, LAX, LHR, LON, CDG, PAR).
     */
    std::string _iataCode;

    /**
     * ICAO code (e.g., KORD).
     */
    std::string _icaoCode;

    /**
     * Geonames ID (e.g., 4887479).
     */
    GeonamesID_T _geonamesID;
  };

}
#endif // __OPENTREP_BOM_PLACEKEY_HPP
