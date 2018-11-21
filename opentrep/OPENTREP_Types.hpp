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
   * File-path for the OPTD-maintained list of POR (points of reference).
   */
  struct PORFilePath_T : public FilePath_T {
  public:
    explicit PORFilePath_T (const std::string& iValue) : FilePath_T (iValue) { }
  };

  /** 
   * Xapian database file-path/name, corresponding to the (potentially relative)
   * directory name (on the filesystem) where Xapian stores its index.
   */
  struct TravelDBFilePath_T : public std::string {
  public:
    explicit TravelDBFilePath_T (const std::string& iValue)
      : std::string (iValue) { }
  };

  /** 
   * SQL database type: MySQL ("mysql)), SQLite3 ("sqlite") or, the default,
   * no database at all ("nodb").
   */
  struct SQLDBConnectionString_T : public std::string {
  public:
    explicit SQLDBConnectionString_T (const std::string& iValue)
      : std::string (iValue) { }
  };

  /** 
   * SQLite database file-path, corresponding to the (potentially relative)
   * directory name (on the filesystem) where SQLite stores its database.
   */
  struct SQLiteDBFilePath_T : public std::string {
  public:
    explicit SQLiteDBFilePath_T (const std::string& iValue)
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
   * Number/version of the current deployment.
   *
   * The idea is to have at least two pieces of infrastructure (SQL
   * database, Xapian index) in parallel:
   * <ul>
   *   <li>one is used by the production;</li>
   *   <li>the other one used as a staging platform in order to test and
   * validate a new version.</li>
   *   <li>Once the new version has been validated, the two pieces
   *       of infrastructure can then be interverted, ie, the production
   *       becomes the new version, and the older version ends up in
   *       staging.</li>
   *   <li>It means that all programs have to choose which version they
   *       want to work on. That version may even be toggled in live.</li>
   *   <li>That method to deploy in production through a staging process
   *       is even more needed by the fact that indexing a new POR data file
   *       takes up to 30 minutes in the worst case. So, we cannot afford
   *       30-45 minutes of downtime everytime a new POR data file is
   *       released (potentially every day).</li>
   *   <li>With that staging process, it is even possible to fully automate
   *       the re-indexing after a new POR data file release:
   *       once the new release has been cleared by QA on staging,
   *       it becomes production.</li>
   * </ul>
   */
  typedef unsigned short DeploymentNumber_T;
  
  /**
   * Whether or not the non-IATA-referenced POR should be included
   * (and indexed).
   *
   * By default, and historically, only the POR, which are referenced
   * by IATA (ie, which have a specific IATA code), are indexed (and
   * may be searched for) in OpenTREP.
   *
   * POR are also referenced by other international organizations,
   * such as ICAO, UN/LOCODE or UIC, and may not be referenced by
   * IATA (in which case their IATA code is left empty).
   *
   * As of August 2018, there are around 110,000 POR in OpenTravelData (OPTD),
   * the reference data source for OpenTREP:
   * <ul>
   *  <li>Around 20,000 POR are referenced by IATA</li>
   *  <li>Around 90,000 POR are not referenced by IATA, but referenced
   *      by other international organizations (eg, ICAO, UN/LOCODE, UIC)</li>
   * </ul>
   *
   * Indexing 20,000 POR takes already a few minutes on standard hardware.
   * Indexing 110,000 POR would take 15 to 20 minutes.
   *
   * Once indexed, all those POR become searchable. That flag is therefore
   * only used at indexing time.
   */
  typedef bool shouldIndexNonIATAPOR_T;

  /**
   * Whether or not to add the POR, or list of POR, into the Xapian index.
   */
  typedef bool shouldIndexPORInXapian_T;

  /**
   * Whether or not to add the POR, or list of POR, into the SQL database.
   */
  typedef bool shouldAddPORInSQLDB_T;

  /**
   * IATA three-letter code (e.g., ORD for Chicago O'Hare, IL, USA).
   *
   * IATA is the [International Air Transport Association](http://en.wikipedia.org/wiki/International_Air_Transport_Association).
   * The corresponding alternate name code in Geonames is ``iata``.
   */
  struct IATACode_T : public std::string {
  public:
    explicit IATACode_T (const std::string& iValue) : std::string (iValue) { }
  };
  /**
   * List of IATA three-letter codes.
   */
  typedef std::list<IATACode_T> IATACodeList_T;

  /**
   * ICAO four-letter code (e.g., KORD).
   *
   * ICAO is the
   * [International Civil Aviation Organization](http://en.wikipedia.org/wiki/International_Civil_Aviation_Organization).
   * The corresponding alternate name code in Geonames is ``icao``.
   */
  struct ICAOCode_T : public std::string {
  public:
    explicit ICAOCode_T (const std::string& iValue) : std::string (iValue) { }
  };
  /**
   * List of ICAO four-letter codes.
   */
  typedef std::list<ICAOCode_T> ICAOCodeList_T;

  /**
   * Geonames ID.
   */
  typedef unsigned int GeonamesID_T;
  /**
   * List of Geonames IDs.
   */
  typedef std::list<GeonamesID_T> GeonamesIDList_T;

  /**
   * Envelope ID.
   */
  typedef unsigned int EnvelopeID_T;

  /**
   * FAA three-letter code (e.g., ORD).
   * FAA is the
   * [U.S. Federal Aviation Administration](http://en.wikipedia.org/wiki/Federal_Aviation_Administration).
   * The corresponding alternate name code in Geonames is ``faac``.
   */
  struct FAACode_T : public std::string {
  public:
    explicit FAACode_T (const std::string& iValue) : std::string (iValue) { }
  };

  /**
   * TC LID four-letter code (e.g., CTB6).
   *
   * TC stands for
   * [Transport Canada](http://en.wikipedia.org/wiki/Transport_Canada).
   * LID stands for
   * [Location Identifier](http://en.wikipedia.org/wiki/Location_identifier).
   * The corresponding alternate name code in Geonames is ``tcid``.
   */
  struct TCLIDCode_T : public std::string {
  public:
    explicit TCLIDCode_T (const std::string& iValue) : std::string (iValue) { }
  };

  /**
   * UN/LOCODE five-letter code (e.g., DEHAM).
   *
   * UN/LOCODE is the
   * [United Nations Code for Trade and Transport Locations](http://en.wikipedia.org/wiki/UN/LOCODE)
   * The corresponding alternate name code in Geonames is ``unlc``.
   */
  struct UNLOCode_T : public std::string {
  public:
    explicit UNLOCode_T (const std::string& iValue) : std::string (iValue) { }
  };  
  typedef std::list<UNLOCode_T> UNLOCodeList_T;

  /**
   * UIC code (e.g., 87747006).
   *
   * UIC is the
   * [International Union of Railways (Union Internationale des Chemins de fer in French)](http://en.wikipedia.org/wiki/International_Union_of_Railways)
   * The corresponding alternate name code in Geonames is ``uicn``.
   */
  typedef unsigned int UICCode_T;
  /**
   * List of UIC codes.
   */
  typedef std::list<UICCode_T> UICCodeList_T;

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
   * ASCII name (not necessarily in English, e.g., Aeroport de Nice Cote d'Azur).
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
   * List of the IATA codes of the travel-related POR, aggregated within
   * a single string (e.g., "EMB,JCC,SFO" for San Francisco, California, USA).
   */
  struct TvlPORListString_T : public std::string {
  public:
    explicit TvlPORListString_T (const std::string& iValue)
      : std::string (iValue) {
    }
  };

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
   * Comment (e.g., "Code claimed back by IATA in November 2012").
   */
  struct Comment_T : public std::string {
  public:
    explicit Comment_T (const std::string& iValue) : std::string (iValue) { }
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
   * List of the IATA codes of the served cities, aggregated within
   * a single string (e.g., "HFD,BDL,SFY" for
   * [Bradley International Airport](http://www.geonames.org/5282636),
   * Connecticut (CT), United States (US)).
   */
  struct CityListString_T : public std::string {
  public:
    explicit CityListString_T (const std::string& iValue)
      : std::string (iValue) {
    }
  };

  /**
   * State code (e.g., IL for Illinois, USA).
   */
  struct StateCode_T : public std::string {
  public:
    explicit StateCode_T (const std::string& iValue) : std::string (iValue) { }
  };

  /**
   * Country two-letter code (e.g., US for USA).
   *
   * The code follows the [ISO 639-1 standard](http://en.wikipedia.org/wiki/ISO_639-1).
   */
  struct CountryCode_T : public std::string {
  public:
    explicit CountryCode_T (const std::string& iValue) : std::string (iValue) { }
  };

  /**
   * Alternative country code (e.g., GB for UK).
   *
   * That alternative code is used when the place belongs to several countries.
   * For instance, [Jersey Airport](http://www.geonames.org/6296595) belongs
   * to both Jersey (JE) and UK (GB).
   */
  struct AltCountryCode_T : public std::string {
  public:
    explicit AltCountryCode_T (const std::string& iValue)
      : std::string (iValue) { }
  };

  /**
   * Country name (e.g., Germany, Switzerland, France).
   *
   * That is the (most used) English name, as reference in Wikipedia.
   */
  struct CountryName_T : public std::string {
  public:
    explicit CountryName_T (const std::string& iValue) : std::string (iValue) { }
  };

  /**
   * US DOT World Area Code (WAC) (e.g., 1 for Alaska, US, or 427 for France)
   *
   * Those [US DOT WAC codes](http://en.wikipedia.org/wiki/US_DOT_WAC) are
   * maintained by the [BTS](http://en.wikipedia.org/wiki/Bureau_of_Transportation_Statistics),
   * part of the [US DOT](http://en.wikipedia.org/wiki/United_States_Department_of_Transportation).
   */
  typedef unsigned int WAC_T;

  /**
   * US DOT World Area Code (WAC) name (e.g., Alaska)
   *
   * See above WAC_T for the details.
   */
  struct WACName_T : public std::string {
  public:
    explicit WACName_T (const std::string& iValue) : std::string (iValue) { }
  };

  /**
   * Currency code (e.g., USD, EUR)
   *
   * That currency code follows the [ISO 4217 standard](http://en.wikipedia.org/wiki/ISO_4217).
   */
  struct CurrencyCode_T : public std::string {
  public:
    explicit CurrencyCode_T (const std::string& iValue): std::string (iValue) { }
  };

  /**
   * Continent code (e.g., SA for South America).
   */
  struct ContinentCode_T : public std::string {
  public:
    explicit ContinentCode_T (const std::string& iValue) : std::string (iValue) {
    }
  };

  /**
   * Continent name (e.g., North America, Europe, Pacific, Asia, Australia).
   */
  struct ContinentName_T : public std::string {
  public:
    explicit ContinentName_T (const std::string& iValue) : std::string (iValue) {
    }
  };

  /**
   * Administrative code of rank 1 (e.g., IL for Illinois).
   *
   * It corresponds to Geonames ADM1 code and names.
   */
  struct Admin1Code_T : public std::string {
  public:
    explicit Admin1Code_T (const std::string& iValue) : std::string (iValue) { }
  };

  /**
   * Administrative UTF8 name of rank 1 (e.g., Illinois).
   *
   * It corresponds to Geonames ADM1 code and names.
   */
  struct Admin1UTFName_T : public std::string {
  public:
    explicit Admin1UTFName_T (const std::string& iValue)
      : std::string (iValue) { }
  };

  /**
   * Administrative ASCII name of rank 1 (e.g., Illinois).
   *
   * It corresponds to Geonames ADM1 code and names.
   */
  struct Admin1ASCIIName_T : public std::string {
  public:
    explicit Admin1ASCIIName_T (const std::string& iValue)
      : std::string (iValue) { }
  };

  /**
   * Administrative code of rank 2.
   *
   * It corresponds to Geonames ADM2 code and names.
   */
  struct Admin2Code_T : public std::string {
  public:
    explicit Admin2Code_T (const std::string& iValue) : std::string (iValue) { }
  };

  /**
   * Administrative UTF8 name of rank 2 (e.g., Alpes-Maritimes).
   *
   * It corresponds to Geonames ADM2 code and names.
   */
  struct Admin2UTFName_T : public std::string {
  public:
    explicit Admin2UTFName_T (const std::string& iValue)
      : std::string (iValue) { }
  };

  /**
   * Administrative ASCII name of rank 2 (e.g., Alpes-Maritimes).
   *
   * It corresponds to Geonames ADM2 code and names.
   */
  struct Admin2ASCIIName_T : public std::string {
  public:
    explicit Admin2ASCIIName_T (const std::string& iValue)
      : std::string (iValue) { }
  };

  /**
   * Administrative code of rank 3.
   *
   * It corresponds to Geonames ADM3 code.
   */
  struct Admin3Code_T : public std::string {
  public:
    explicit Admin3Code_T (const std::string& iValue) : std::string (iValue) { }
  };

  /**
   * Administrative code of rank 4.
   *
   * It corresponds to Geonames ADM4 code.
   */
  struct Admin4Code_T : public std::string {
  public:
    explicit Admin4Code_T (const std::string& iValue) : std::string (iValue) { }
  };

  /**
   * Feature class (e.g., S for spot).
   *
   * It corresponds to [Geonames feature codes and classes](http://www.geonames.org/export/codes.html).
   */
  struct FeatureClass_T : public std::string {
  public:
    explicit FeatureClass_T (const std::string& iValue) : std::string (iValue) {
    }
  };

  /**
   * Feature code (e.g., AIRP for airport).
   *
   * It corresponds to [Geonames feature codes and classes](http://www.geonames.org/export/codes.html).
   */
  struct FeatureCode_T : public std::string {
  public:
    explicit FeatureCode_T (const std::string& iValue) : std::string (iValue) { }
  };

  /**
   * Feature name (e.g., airport, airfield, airstrip, airbase, heliport).
   *
   * It corresponds to [Geonames feature codes and classes](http://www.geonames.org/export/codes.html).
   */
  struct FeatureName_T : public std::string {
  public:
    explicit FeatureName_T (const std::string& iValue) : std::string (iValue) { }
  };
  typedef std::list<FeatureName_T> FeatureNameList_T;

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
   *
   * It corresponds to the date when the place was last modified on Geonames.
   */
  typedef boost::gregorian::date Date_T;
    
  /**
   * Population (e.g., 400,000).
   *
   * It corresponds to the population size as curated by Geonames.
   */
  typedef unsigned int Population_T;

  /**
   * Geographical elevation (e.g., 300).
   *
   * It corresponds to the elevation as curated by Geonames.
   */
  typedef int Elevation_T;

  /**
   * Geographical topology 30.
   *
   * It corresponds to the topology value as curated by Geonames.
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
   * GMT offset (e.g., 1)
   */
  typedef float GMTOffset_T;

  /**
   * DST offset (e.g., 2)
   */
  typedef float DSTOffset_T;

  /**
   * Raw offset (e.g., 1)
   */
  typedef float RawOffset_T;

  /**
   * PageRank (e.g., 94.66%).
   *
   * That value is derived from the frequency of trips to/from the place.
   */
  typedef double PageRank_T;

  /**
   * Whether or not that place is referenced by Geonames
   */
  typedef bool IsGeonames_T;

  /**
   * Whether or not that place is an airport
   */
  typedef bool IsAirport_T;

  /**
   * Whether or not that place is for commercial use
   */
  typedef bool IsCommercial_T;

  /**
   * Xapian document ID.
   */
  typedef unsigned int XapianDocID_T;

  /**
   * Weight when indexing terms of a Xapian document.
   */
  typedef unsigned short Weight_T;

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
