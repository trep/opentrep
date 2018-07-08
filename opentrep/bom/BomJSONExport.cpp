// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <ostream>
// Boost ForEach
//#include <boost/foreach.hpp>
// OpenTREP
#include <opentrep/Location.hpp>
#include <opentrep/CityDetails.hpp>
#include <opentrep/bom/BomJSONExport.hpp>

namespace OPENTREP { 

  // ////////////////////////////////////////////////////////////////////
  void BomJSONExport::
  jsonExportLocationList (std::ostream& oStream,
                          const LocationList_T& iLocationList) {

    // Create empty Boost.Property_Tree objects
    bpt::ptree lPT;
    bpt::ptree lPTLocationList;

    for (LocationList_T::const_iterator itLocation = iLocationList.begin();
         itLocation != iLocationList.end(); ++itLocation) {
      const Location& lLocation = *itLocation;
      //
      bpt::ptree lPTLocation;
      jsonExportLocation (lPTLocation, lLocation);

      // List of extra matching locations (those with the same matching
      // weight/percentage)
      const LocationList_T& lExtraLocationList= lLocation.getExtraLocationList();
      if (lExtraLocationList.empty() == false) {
        //
        bpt::ptree lPTExtraLocationList;

        NbOfMatches_T idxExtra = 0;
        for (LocationList_T::const_iterator itLoc = lExtraLocationList.begin();
             itLoc != lExtraLocationList.end(); ++itLoc, ++idxExtra) {
          const Location& lExtraLocation = *itLoc;
          //
          bpt::ptree lPTExtraLocation;
          jsonExportLocation (lPTExtraLocation, lExtraLocation);

          // Put the current location tree in the location list
          lPTExtraLocationList.push_back (std::make_pair("", lPTExtraLocation));
        }

        // Add the extra location list to the Boost.Property_Tree representing
        // the current Location instance.
        lPTLocation.add_child ("extras", lPTExtraLocationList);
      }

      // List of alternate matching locations (those with a lower matching
      // weight/percentage)
      const LocationList_T& lAltLocationList =
        lLocation.getAlternateLocationList();
      if (lAltLocationList.empty() == false) {
        //
        bpt::ptree lPTAltLocationList;

        NbOfMatches_T idxAlter = 0;
        for (LocationList_T::const_iterator itLoc = lAltLocationList.begin();
             itLoc != lAltLocationList.end(); ++itLoc, ++idxAlter) {
          const Location& lAltLocation = *itLoc;
          //
          bpt::ptree lPTAltLocation;
          jsonExportLocation (lPTAltLocation, lAltLocation);

          // Put the current location tree in the location list
          lPTAltLocationList.push_back (std::make_pair ("", lPTAltLocation));
        }

        // Add the extra location list to the Boost.Property_Tree representing
        // the current Location instance.
        lPTLocation.add_child ("alternates", lPTAltLocationList);
      }

      // Add the current location property tree to the location list
      lPTLocationList.push_back (std::make_pair ("", lPTLocation));
    }

    // Add the location list to the root of the Boost.Property_Tree
    lPT.add_child ("locations", lPTLocationList);

    // Write the property tree into a JSON string
    write_json (oStream, lPT);
  }

  // ////////////////////////////////////////////////////////////////////
  void BomJSONExport::jsonExportLocation (bpt::ptree& ioPTLocation,
                                          const Location& iLocation) {
    // Fill all the fields of the JSON instance
    ioPTLocation.put ("iata_code", iLocation.getIataCode());
    ioPTLocation.put ("icao_code", iLocation.getIcaoCode());
    ioPTLocation.put ("geonames_id", iLocation.getGeonamesID());
    ioPTLocation.put ("faa_code", iLocation.getFaaCode());
    ioPTLocation.put ("state_code", iLocation.getStateCode());
    ioPTLocation.put ("country_code", iLocation.getCountryCode());
    ioPTLocation.put ("country_name", iLocation.getCountryName());
    ioPTLocation.put ("alt_country_code", iLocation.getAltCountryCode());
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
    ioPTLocation.put ("lat", iLocation.getLatitude());
    ioPTLocation.put ("lon", iLocation.getLongitude());
    ioPTLocation.put ("page_rank", iLocation.getPageRank());
    ioPTLocation.put ("wac", iLocation.getWAC());
    ioPTLocation.put ("wac_name", iLocation.getWACName());
    ioPTLocation.put ("wiki_link", iLocation.getWikiLink());
    ioPTLocation.put ("currency_code", iLocation.getCurrencyCode());
    ioPTLocation.put ("original_keywords", iLocation.getOriginalKeywords());
    ioPTLocation.put ("corrected_keywords", iLocation.getCorrectedKeywords());
    ioPTLocation.put ("matching_percentage", iLocation.getPercentage());
    ioPTLocation.put ("edit_distance", iLocation.getEditDistance());
    ioPTLocation.put ("allowable_distance",iLocation.getAllowableEditDistance());

    /**
     * List of UN/LOCODE codes
     */
    bpt::ptree ptUNLOCodeList;
    // Retrieve the list of UN/LOCODE codes
    const UNLOCodeList_T& lUNCodeList = iLocation.getUNLOCodeList();
    for (UNLOCodeList_T::const_iterator itUNLOCode = lUNCodeList.begin();
         itUNLOCode != lUNCodeList.end(); ++itUNLOCode) {
      // Retrieve the UN/LOCODE code
      const UNLOCode_T& lUNLOCode = *itUNLOCode;

      // Add the item into the UN/LOCODE list Boost.Property_Tree      
      ptUNLOCodeList.put ("unlocode_code", lUNLOCode);
    }

    // Add the UN/LOCODE Boost.Property_Tree to the main tree
    ioPTLocation.add_child ("unlocode_codes", ptUNLOCodeList);

    /**
     * List of served cities
     */
    bpt::ptree ptCityList;
    // Retrieve the list of served cities
    const CityDetailsList_T& lCityList = iLocation.getCityList();
    for (CityDetailsList_T::const_iterator itCity = lCityList.begin();
         itCity != lCityList.end(); ++itCity) {
      // Retrieve the details, ie, IATA code, Geonames ID and names
      const CityDetails& lCityDetails = *itCity;
      bpt::ptree ptCityDetails;
      ptCityDetails.put ("iata_code", lCityDetails.getIataCode());
      ptCityDetails.put ("geonames_id", lCityDetails.getGeonamesID());
      ptCityDetails.put ("name_utf", lCityDetails.getUtfName());
      ptCityDetails.put ("name_ascii", lCityDetails.getAsciiName());

      // Add the item into the city list Boost.Property_Tree
      ptCityList.push_back (std::make_pair ("city_details", ptCityDetails));
    }

    // Add the city detail list to the location Boost.Property_Tree
    ioPTLocation.add_child ("cities", ptCityList);

    /**
     * Alternate names
     */
    bpt::ptree ptLocationNameList;
    // Retrieve the place names in all the available languages
    const NameMatrix& lNameMatrixFull = iLocation.getNameMatrix();
    const NameMatrix_T& lNameMatrix = lNameMatrixFull.getNameMatrix();
    for (NameMatrix_T::const_iterator itNameList = lNameMatrix.begin();
         itNameList != lNameMatrix.end(); ++itNameList) {
      // Retrieve the language code and locale
      // const Language::EN_Language& lLanguage = itNameList->first;
      const Names& lNames = itNameList->second;

      // For a given language, retrieve the list of place names
      const NameList_T& lNameList = lNames.getNameList();
        
      for (NameList_T::const_iterator itName = lNameList.begin();
           itName != lNameList.end(); ++itName) {
        const std::string& lName = *itName;

        if (lName.empty() == false) {
          bpt::ptree ptLocationName;
          ptLocationName.put ("name", lName);
          ptLocationNameList.push_back (std::make_pair ("", ptLocationName));
        }
      }
    }

    // Add the name list to the Boost.Property_Tree representing the location
    ioPTLocation.add_child ("names", ptLocationNameList);
  }

}
