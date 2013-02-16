// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <ostream>
// Boost ForEach
//#include <boost/foreach.hpp>
// OpenTREP
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
      const LocationList_T& lExtraLocationList =
	lLocation.getExtraLocationList();
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
	  lPTAltLocationList.push_back (std::make_pair("", lPTAltLocation));
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
    ioPTLocation.put ("city_code", iLocation.getCityCode());
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
    ioPTLocation.put ("lat", iLocation.getLatitude());
    ioPTLocation.put ("lon", iLocation.getLongitude());
    ioPTLocation.put ("page_rank", iLocation.getPageRank());
    ioPTLocation.put ("wiki_link", iLocation.getWikiLink());
    ioPTLocation.put ("original_keywords", iLocation.getOriginalKeywords());
    ioPTLocation.put ("corrected_keywords", iLocation.getCorrectedKeywords());
    ioPTLocation.put ("matching_percentage", iLocation.getPercentage());
    ioPTLocation.put ("edit_distance", iLocation.getEditDistance());
    ioPTLocation.put ("allowable_distance", iLocation.getAllowableEditDistance());

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
