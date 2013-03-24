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
#include <opentrep/bom/Travel.hpp>

namespace OPENTREP {
  
  // //////////////////////////////////////////////////////////////////////
  void fillLocation (Location& ioLocation) {
    // Protobuf structure
    treppb::Place ioPlace;
    
    // IATA code
    treppb::IATAPOR* lIataAirportPtr = ioPlace.mutable_iata_code();
    lIataAirportPtr->set_iata_code ("KBP");
    // Location type
    ioPlace.set_loc_type (treppb::Place::A);
    // Geonames ID
    ioPlace.set_geoname_id (6300952);

    // Geographical coordinates
    treppb::Point* lPointPtr = ioPlace.mutable_coord();
    lPointPtr->set_latitude (50.345);
    lPointPtr->set_longitude (30.894722);

    // City code
    treppb::IATAPOR* lIataCityPtr = ioPlace.mutable_iata_code();
    lIataCityPtr->set_iata_code ("IEV");

    // Beginning date of the validity period
    ioPlace.set_from_date ("1970-01-01");

    //
    LocationKey lLocationKey (IATACode_T ("KBP"), ICAOCode_T ("UKBB"),
                              GeonamesID_T (6300952));

    //
    ioLocation.setKey (lLocationKey);
  }

}

