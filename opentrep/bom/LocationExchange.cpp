// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// STL
#include <iostream>
#include <fstream>
#include <string>
// OpenTrep Protobuf
#include <opentrep/Travel.pb.h>
// OpenTrep
#include <opentrep/bom/LocationExchange.hpp>

namespace OPENTREP {
  
  // //////////////////////////////////////////////////////////////////////
  std::string LocationExchange::exportLocation (const Location& iLocation) {
    std::ostringstream oStr;
    
    // Retrieve the primary key
    const LocationKey& lLocationKey = iLocation.getKey();

    // Retrieve the IATA code
    const IATACode_T& lIataCode = lLocationKey.getIataCode();

    // Retrieve the ICAO code
    //const ICAOCode_T& lIcaoCode = lLocationKey.getIcaoCode();

    // Retrieve the Geonames ID
    const GeonamesID_T& lGeonamesID = lLocationKey.getGeonamesID();

    // Protobuf structure
    treppb::Place ioPlace;
    
    // IATA code
    treppb::IATAPOR* lIataAirportPtr = ioPlace.mutable_iata_code();
    lIataAirportPtr->set_iata_code (lIataCode);
    // Location type
    ioPlace.set_loc_type (treppb::Place::A);
    // Geonames ID
    ioPlace.set_geoname_id (lGeonamesID);

    // Geographical coordinates
    treppb::Point* lPointPtr = ioPlace.mutable_coord();
    lPointPtr->set_latitude (50.345);
    lPointPtr->set_longitude (30.894722);

    // City code
    treppb::IATAPOR* lIataCityPtr = ioPlace.mutable_city_code();
    lIataCityPtr->set_iata_code ("IEV");

    // Beginning date of the validity period
    ioPlace.set_from_date ("1970-01-01");

    // Serialise the Protobuf
    ioPlace.SerializeToOstream (&oStr);

    //
    return oStr.str();
  }

}

