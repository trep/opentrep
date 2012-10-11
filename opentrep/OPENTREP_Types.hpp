#ifndef __OPENTREP_OPENTREP_TYPES_HPP
#define __OPENTREP_OPENTREP_TYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <exception>
#include <string>
#include <list>
#include <map>
#include <set>
// Boost Date-Time
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
// Boost Array
#include <boost/array.hpp>
// OpenTrep
#include <opentrep/OPENTREP_exceptions.hpp>
#include <opentrep/OPENTREP_log.hpp>

namespace OPENTREP {

  // //////// Type definitions /////////
  /** 
   * File path.
   */
  struct FilePath_T : public std::string {
  public:
    explicit FilePath_T (const std::string& iValue) : std::string (iValue) {
    }
  };

  /** 
   * File-path for the ORI-maintained list of POR (points of reference).
   */
  struct PORFilePath_T : public FilePath_T {
  public:
    explicit PORFilePath_T (const std::string& iValue) : FilePath_T (iValue) { }
  };

  /** 
   * Xapian database name, corresponding to the (potentially relative)
   * directory name (on the filesystem) where Xapian stores its index.
   */
  struct TravelDatabaseName_T : public std::string {
  public:
    explicit TravelDatabaseName_T (const std::string& iValue)
      : std::string (iValue) { }
  };

  /**
   * Raw data string, before any parsing. It is used to be re-parsed once
   * retrieved by Xapian as document data.
   */
  struct RawDataString_T : public std::string {
  public:
    explicit RawDataString_T (const std::string& iValue)
      : std::string (iValue) { }
  };

  /**
   * IATA three-letter code (e.g., ORD for Chicago O'Hare, IL, USA).
   */
  struct IATACode_T : public std::string {
  public:
    explicit IATACode_T (const std::string& iValue) : std::string (iValue) { }
  };

  /**
   * ICAO four-letter code (e.g., KORD).
   */
  struct ICAOCode_T : public std::string {
  public:
    explicit ICAOCode_T (const std::string& iValue) : std::string (iValue) { }
  };

  /**
   * Geonames ID.
   */
  typedef int GeonamesID_T;

  /**
   * FAA three-letter code (e.g., ORD).
   */
  struct FAACode_T : public std::string {
  public:
    explicit FAACode_T (const std::string& iValue) : std::string (iValue) { }
  };

  /**
   * Location name (e.g., Nice Côte d'Azur).
   */
  struct LocationName_T : public std::string {
  public:
    explicit LocationName_T (const std::string& iValue) : std::string (iValue) {
    }
  };

  /** 
   * Common name (usually in American English, but not necessarily in ASCII,
   * e.g., Nice Côte d'Azur).
   */
  struct CommonName_T : public LocationName_T {
  public:
    explicit CommonName_T (const std::string& iValue) : LocationName_T (iValue) {
    }
  };
    
  /** 
   * ASCII name (not necessarily in English, e.g., aéroport de Nice Cote d'Azur).
   */
  struct ASCIIName_T : public LocationName_T {
  public:
    explicit ASCIIName_T (const std::string& iValue) : LocationName_T (iValue) {
    }
  };

  /** 
   * Short list of alternate names, without the language codes (e.g.,
   * "Aéroport de Nice Côte d'Azur,Nice Airport,Flughafen Nizza"),
   * aggregated within a single string.
   */
  struct AltNameShortListString_T : public LocationName_T {
  public:
    explicit AltNameShortListString_T (const std::string& iValue)
      : LocationName_T (iValue) {
    }
  };
  typedef std::list<AltNameShortListString_T> AltNameShortList_T;

  /**
   * Language code (e.g., en). Note that some language codes may contain
   * numerics; for instance, fr_1793.
   */
  struct LanguageCode_T : public std::string {
  public:
    explicit LanguageCode_T (const std::string& iValue) : std::string (iValue) {
    }
  };

  /**
   * IATA city code (e.g., CHI for Chicago, IL, USA).
   */
  struct CityCode_T : public IATACode_T {
  public:
    explicit CityCode_T (const std::string& iValue)
      : IATACode_T (std::string (iValue)) {
    }
  };

  /**
   * City name in UTF8 (e.g., Nice Côte d'Azur).
   */
  struct CityUTFName_T : public std::string {
  public:
    explicit CityUTFName_T (const std::string& iValue) : std::string (iValue) { }
  };

  /**
   * City name in ASCII (but not necessarily in English,
   * e.g., ville de Nice Cote d'Azur).
   */
  struct CityASCIIName_T : public std::string {
  public:
    explicit CityASCIIName_T (const std::string& iValue)
      : std::string (iValue) { }
  };

  /**
   * State code (e.g., IL for Illinois, USA).
   */
  struct StateCode_T : public std::string {
  public:
    explicit StateCode_T (const std::string& iValue) : std::string (iValue) { }
  };

  /**
   * Country code (e.g., US for USA).
   */
  struct CountryCode_T : public std::string {
  public:
    explicit CountryCode_T (const std::string& iValue) : std::string (iValue) { }
  };

  /**
   * Alternative country code (e.g., US for USA).
   */
  struct AltCountryCode_T : public std::string {
  public:
    explicit AltCountryCode_T (const std::string& iValue)
      : std::string (iValue) { }
  };

  /**
   * Country name (e.g., Germany).
   */
  struct CountryName_T : public std::string {
  public:
    explicit CountryName_T (const std::string& iValue) : std::string (iValue) { }
  };

  /**
   * Region code (e.g., NAMER for North America).
   */
  struct RegionCode_T : public std::string {
  public:
    explicit RegionCode_T (const std::string& iValue) : std::string (iValue) { }
  };

  /**
   * Administrative code of rank 1 (e.g., IL for Illinois).
   */
  struct Admin1Code_T : public std::string {
  public:
    explicit Admin1Code_T (const std::string& iValue) : std::string (iValue) { }
  };

  /**
   * Administrative UTF8 name of rank 1 (e.g., Illinois).
   */
  struct Admin1UTFName_T : public std::string {
  public:
    explicit Admin1UTFName_T (const std::string& iValue)
      : std::string (iValue) { }
  };

  /**
   * Administrative ASCII name of rank 1 (e.g., Illinois).
   */
  struct Admin1ASCIIName_T : public std::string {
  public:
    explicit Admin1ASCIIName_T (const std::string& iValue)
      : std::string (iValue) { }
  };

  /**
   * Administrative code of rank 2.
   */
  struct Admin2Code_T : public std::string {
  public:
    explicit Admin2Code_T (const std::string& iValue) : std::string (iValue) { }
  };

  /**
   * Administrative UTF8 name of rank 2 (e.g., Alpes-Maritimes).
   */
  struct Admin2UTFName_T : public std::string {
  public:
    explicit Admin2UTFName_T (const std::string& iValue)
      : std::string (iValue) { }
  };

  /**
   * Administrative ASCII name of rank 2 (e.g., Alpes-Maritimes).
   */
  struct Admin2ASCIIName_T : public std::string {
  public:
    explicit Admin2ASCIIName_T (const std::string& iValue)
      : std::string (iValue) { }
  };

  /**
   * Administrative code of rank 3.
   */
  struct Admin3Code_T : public std::string {
  public:
    explicit Admin3Code_T (const std::string& iValue) : std::string (iValue) { }
  };

  /**
   * Administrative code of rank 4.
   */
  struct Admin4Code_T : public std::string {
  public:
    explicit Admin4Code_T (const std::string& iValue) : std::string (iValue) { }
  };

  /**
   * Feature class (e.g., S for spot).
   */
  struct FeatureClass_T : public std::string {
  public:
    explicit FeatureClass_T (const std::string& iValue) : std::string (iValue) {
    }
  };

  /**
   * Feature code (e.g., AIRP for airport).
   */
  struct FeatureCode_T : public std::string {
  public:
    explicit FeatureCode_T (const std::string& iValue) : std::string (iValue) { }
  };

  /**
   * IATA location type (e.g., A for airport).
   */
  struct IATAType_T : public std::string {
  public:
    explicit IATAType_T (const std::string& iValue) : std::string (iValue) { }
  };

  /**
   * Geographical coordinate (e.g., -10.45 or 56.7).
   */
  typedef double GeoCoord_T;
  typedef GeoCoord_T Latitude_T;
  typedef GeoCoord_T Longitude_T;

  /**
   * Wikipedia link (e.g., http://en.wikipedia.org/wiki/Chicago).
   */
  struct WikiLink_T : public std::string {
  public:
    explicit WikiLink_T (const std::string& iValue) : std::string (iValue) { }
  };

  /**
   * Date (e.g., modification date).
   */
  typedef boost::gregorian::date Date_T;
    
  /**
   * Population (e.g., 400,000).
   */
  typedef unsigned int Population_T;

  /**
   * Geographical elevation (e.g., 300).
   */
  typedef int Elevation_T;

  /**
   * Geographical topology 30.
   */
  typedef int GTopo30_T;

  /**
   * Time-zone (e.g., America/Chicago).
   */
  struct TimeZone_T : public std::string {
  public:
    explicit TimeZone_T (const std::string& iValue) : std::string (iValue) { }
  };

  /**
   * PageRank (e.g., 94.66%).
   */
  typedef double PageRank_T;

  /**
   * Xapian document ID.
   */
  typedef int XapianDocID_T;

  /**
   * GMT offset (e.g., 1)
   */
  typedef short GMTOffset_T;

  /**
   * DST offset (e.g., 2)
   */
  typedef short DSTOffset_T;

  /**
   * Raw offset (e.g., 1)
   */
  typedef short RawOffset_T;

  /**
   * Whether or not that POR is referenced by Geonames
   */
  typedef bool IsGeonames_T;

  /**
   * Whether or not that POR is an airport
   */
  typedef bool IsAirport_T;

  /**
   * Whether or not that POR is for commercial use
   */
  typedef bool IsCommercial_T;


  /**
   * Travel search query.
   */
  typedef std::string TravelQuery_T;

  /**
   * Xapian percentage.
   */
  typedef double MatchingPercentage_T;

  /**
   * Matching percentage for a Xapian document.
   */
  typedef double Percentage_T;

  /**
   * Score (e.g., matching percentage) for a document/POR (point of reference).
   */
  typedef double Score_T;

  /**
   * Number of entries in the Xapian database.
   */
  typedef unsigned int NbOfDBEntries_T;
  
  /**
   * Word, which is the atomic element of a query string.
   */
  typedef std::string Word_T;

  /**
   * List of words.
   */
  typedef std::list<Word_T> WordList_T;

  /**
   * Set of words.
   */
  typedef std::set<std::string> WordSet_T;

  /**
   * List of (black-listed) words.
   */
  typedef std::set<std::string> BlackList_T;

  /**
   * Number of letters in a word or phrase.
   */
  typedef unsigned int NbOfLetters_T;
  
  /**
   * Number of words in a list.
   */
  typedef unsigned short NbOfWords_T;
  
  /**
   * Number of matching documents.
   */
  typedef unsigned short NbOfMatches_T;
  
  /**
   * Number of errors.
   */
  typedef unsigned short NbOfErrors_T;
  
  /**
   * Number of (distance) errors allowed for a given number of letters.
   */
  typedef std::map<NbOfLetters_T, NbOfErrors_T> DistanceErrorScale_T;

  /**
   * Number of (distance) errors allowed for a given number of letters.
   */
  typedef boost::array<NbOfLetters_T, 5> DistanceErrorScaleArray_T;
}
#endif // __OPENTREP_OPENTREP_TYPES_HPP
