// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// STL
#include <ostream>
#include <string>
// OpenTrep Protobuf
#include <opentrep/Travel.pb.h>
// OpenTrep
#include <opentrep/Location.hpp>
#include <opentrep/bom/LocationExchange.hpp>

namespace OPENTREP {
  
  // //////////////////////////////////////////////////////////////////////
  void LocationExchange::
  exportLocationList (std::ostream& oStream,
                      const LocationList_T& iLocationList) {
    // Protobuf structure
    treppb::PlaceList oPlaceList;
    
    // Browse the list of Location structures
    for (LocationList_T::const_iterator itLocation = iLocationList.begin();
         itLocation != iLocationList.end(); ++itLocation) {
      const Location& lLocation = *itLocation;

      // Create an instance of a Protobuf Place structure
      treppb::Place* lPlacePtr = oPlaceList.add_place();
      assert (lPlacePtr != NULL);
      
      // Fill the Protobuf Place structure with the content of
      // the Location structure
      exportLocation (*lPlacePtr, lLocation);
    }

    // Serialise the Protobuf
    oPlaceList.SerializeToOstream (&oStream);
  }

  // //////////////////////////////////////////////////////////////////////
  void LocationExchange::exportLocation (treppb::Place& ioPlace,
                                         const Location& iLocation) {
    // Retrieve the primary key
    const LocationKey& lLocationKey = iLocation.getKey();

    // Retrieve and set the travel-related IATA code (part of the primary key)
    const IATACode_T& lIataCode = lLocationKey.getIataCode();
    treppb::IATAPOR* lIataAirportPtr = ioPlace.mutable_tvl_code();
    assert (lIataAirportPtr != NULL);
    lIataAirportPtr->set_iata_code (lIataCode);

    // Retrieve and set the Geonames ID
    const GeonamesID_T& lGeonamesID = lLocationKey.getGeonamesID();
    ioPlace.set_geoname_id (lGeonamesID);

    // Retrieve and set the ICAO code
    const ICAOCode_T& lIcaoCode = lLocationKey.getIcaoCode();
    ioPlace.set_icao_code (lIcaoCode);

    // Retrieve and set the location type
    const IATAType& lLocationType = iLocation.getIATAType();
    const treppb::PlaceType& lPlaceType = lLocationType.getTypeAsPB();
    const treppb::PlaceType_LocationType& lPlaceTypeEnum = lPlaceType.type();
    treppb::PlaceType* lPlaceTypePtr = ioPlace.mutable_loc_type();
    assert (lPlaceTypePtr != NULL);
    lPlaceTypePtr->set_type (lPlaceTypeEnum);

    // Retrieve and set the geographical coordinates
    const Latitude_T& lLatitude = iLocation.getLatitude();
    const Longitude_T& lLongitude = iLocation.getLongitude();
    treppb::Point* lPointPtr = ioPlace.mutable_coord();
    lPointPtr->set_latitude (lLatitude);
    lPointPtr->set_longitude (lLongitude);

    // Retrieve and set the city IATA code
    const CityCode_T& lCityCode = iLocation.getCityCode();
    treppb::IATAPOR* lIataCityPtr = ioPlace.mutable_city_code();
    lIataCityPtr->set_iata_code (lCityCode);

    // Retrieve and set the PageRank value
    const PageRank_T& lPageRank = iLocation.getPageRank();
    ioPlace.set_page_rank (lPageRank);

    // Retrieve and set the matching percentage value
    const MatchingPercentage_T& lPercentage = iLocation.getPercentage();
    ioPlace.set_matching_percentage (lPercentage);

    // Beginning date of the validity period
    ioPlace.set_from_date ("1970-01-01");

    /*
    ioPTLocation.put ("faa_code", iLocation.getFaaCode());
    ioPTLocation.put ("city_name_utf", iLocation.getCityUtfName());
    ioPTLocation.put ("city_name_ascii", iLocation.getCityAsciiName());
    ioPTLocation.put ("state_code", iLocation.getStateCode());
    ioPTLocation.put ("country_code", iLocation.getCountryCode());
    ioPTLocation.put ("country_name", iLocation.getCountryName());
    ioPTLocation.put ("continent_name", iLocation.getContinentName());
    ioPTLocation.put ("adm1_code", iLocation.getAdmin1Code());
    ioPTLocation.put ("adm1_name_utf", iLocation.getAdmin1UtfName());
    ioPTLocation.put ("adm1_name_ascii", iLocation.getAdmin1AsciiName());
    ioPTLocation.put ("adm2_code", iLocation.getAdmin2Code());
    ioPTLocation.put ("adm2_name_utf", iLocation.getAdmin2UtfName());
    ioPTLocation.put ("adm2_name_ascii", iLocation.getAdmin2AsciiName());
    ioPTLocation.put ("adm3_code", iLocation.getAdmin3Code());
    ioPTLocation.put ("adm4_code", iLocation.getAdmin4Code());
    ioPTLocation.put ("tvl_por_list", iLocation.getTvlPORListString());
    ioPTLocation.put ("time_zone", iLocation.getTimeZone());
    ioPTLocation.put ("wiki_link", iLocation.getWikiLink());
    ioPTLocation.put ("original_keywords", iLocation.getOriginalKeywords());
    ioPTLocation.put ("corrected_keywords", iLocation.getCorrectedKeywords());
    ioPTLocation.put ("edit_distance", iLocation.getEditDistance());
    ioPTLocation.put ("allowable_distance", iLocation.getAllowableEditDistance());
     */
  }

}

