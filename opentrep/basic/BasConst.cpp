// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
#include <opentrep/config/opentrep-paths.hpp>
#include <opentrep/basic/BasConst_OPENTREP_Service.hpp>
#include <opentrep/basic/BasConst_Unicode.hpp>
#include <opentrep/basic/BasConst_General.hpp>

namespace OPENTREP {

  // ////////////// OPENTREP_Service ///////////////
  /**
   * Default file-path for the OPTD-maintained POR (points of reference) file.
   */
  const std::string DEFAULT_OPENTREP_POR_FILEPATH (OPENTREP_POR_DATA_DIR
                                                   "/test_optd_por_public.csv");
  
  /**
   * Default file-path for the Xapian database/index.
   */
  const std::string
  DEFAULT_OPENTREP_XAPIAN_DB_FILEPATH ("/tmp/opentrep/xapian_traveldb");

  /**
   * Default boolean indicator for the SQLite3 database filling.
   */
  const bool DEFAULT_OPENTREP_SQLITE_DB_FILLING_INDICATOR (false);

  /**
   * Default type for the SQL database (e.g., "nodb", i.e., no SQL database).
   */
  const std::string DEFAULT_OPENTREP_SQL_DB_TYPE ("nodb");

  /**
   * Default connection string for the MySQL/MariaDB database.
   */
  const std::string
  DEFAULT_OPENTREP_MYSQL_CONN_STRING ("db=trep_trep user=trep password=trep");

  /**
   * Default name and location for the SQLite3 database.
   */
  const std::string
  DEFAULT_OPENTREP_SQLITE_DB_FILEPATH ("/tmp/opentrep/sqlite_travel.db");

  /**
   * Default name and location for the MySQL database (if existing).
   */
  const std::string DEFAULT_OPENTREP_MYSQL_DB_USER ("trep");
  const std::string DEFAULT_OPENTREP_MYSQL_DB_PASSWD ("trep");
  const std::string DEFAULT_OPENTREP_MYSQL_DB_DBNAME ("trep_trep");
  const std::string DEFAULT_OPENTREP_MYSQL_DB_HOST ("localhost");
  const std::string DEFAULT_OPENTREP_MYSQL_DB_PORT ("3306");

  /**
   * Number/version of the current deployment.
   *
   * By default, set to 0. If all the processes use the default deployment
   * number/version, then the deployment is direct in production,
   * there is no staging process.
   */
  const unsigned short DEFAULT_OPENTREP_DEPLOYMENT_NUMBER (0);

  /**
   * Size of the pieces of infrastructure for the deployment staging principle.
   *
   * By default, set to 2, one for production and one for staging.
   */
  const unsigned short DEFAULT_OPENTREP_DEPLOYMENT_NUMBER_SIZE (2);

  /**
   * Whether or not the non-IATA-referenced POR should be included
   * (and indexed).
   *
   * By default, and historically, only the POR, which are referenced
   * by IATA (ie, which have a specific IATA code), are indexed (and may
   * be searched for) in OpenTREP.
   */
  const bool DEFAULT_OPENTREP_INCLUDE_NONIATA_POR (false);

  /**
   * Whether or not to add the POR, or list of POR, in the Xapian index.
   *
   * By default, index the POR in Xapian.
   */
  const bool DEFAULT_OPENTREP_INDEX_IN_XAPIAN (true);

  /**
   * Whether or not to add the POR, or list of POR, in the SQL database.
   *
   * By default, do not insert the POR in the SQL database.
   */
  const bool DEFAULT_OPENTREP_ADD_IN_DB (false);

  /**
   * Default date-time with std::tm structure as type.
   */
  const std::tm DEFAULT_DATE_TM =
    boost::gregorian::to_tm (Date_T (1970, boost::gregorian::Jan, 01));

  /**
   * Default date-time with std::string as type.
   */
  const std::string DEFAULT_DATE_STR ("1970-01-01");
  
  // //////////////// Unicode (ICU) ////////////////
  /**
   * Default Unicode transliterator rule (e.g., "NFD; [:M:] Remove; NFC;")
   */
  const char* K_ICU_ACCENT_REMOVAL_RULE = "NFD; [:M:] Remove; NFC;";

  /**
   * Default Unicode transliterator rule for the removal of
   * quotes (e.g., "[\\u02B9] > \\u0027; [\\u002D] > \\u0020;")
   */
  const char* K_ICU_QUOTATION_REMOVAL_RULE =
    "[\\u02B9] > \\u0027; [\\u002D] > \\u0020; [\\u201C] > \\u0020; [\\u201D] > \\u0020; [\\u0027] > \\u0020;";

  /**
   * Default Unicode transliterator rule for the removal of
   * punctuation (e.g., "[:P:] Remove;")
   */
  const char* K_ICU_PUNCTUATION_REMOVAL_RULE = "[:P:] Remove;";

  /**
   * Default Unicode transliterator rule for the removal of
   * punctuation (e.g., "Any-Latin;")
   */
  const char* K_ICU_GENERIC_TRANSLITERATOR_RULE =
    "Any-Latin; NFD; [:M:] Remove; NFC; Lower;";


  // /////////////// General ////////////////
  /**
   * Default size of matching set for Xapian (e.g., 30)
   */
  const NbOfMatches_T K_DEFAULT_XAPIAN_MATCHING_SET_SIZE (30);

  /**
   * Default indexing weight for standard terms (e.g., 1)
   */
  const Weight_T K_DEFAULT_INDEXING_STD_WEIGHT (1);

  /**
   * Default indexing weight for some terms like IATA/ICAO/FAA codes (e.g., 2)
   */
  const Weight_T K_DEFAULT_INDEXING_EXTRA_WEIGHT (2);

  /**
   * Default modified matching percentage (e.g., 99.999% == 0.99999)
   */
  const Percentage_T K_DEFAULT_MODIFIED_MATCHING_PCT (100.0);

  /**
   * Default PageRank (e.g., 0.10% == 0.001)
   */
  const Percentage_T K_DEFAULT_PAGE_RANK (0.10);

  /**
   * Default envelope percentage (e.g., 0.10% == 0.001).
   * Usually equal to K_DEFAULT_PAGE_RANK.
   */
  const Percentage_T K_DEFAULT_ENVELOPE_PCT (K_DEFAULT_PAGE_RANK);

  /**
   * Default full match percentage for a IATA/ICAO code (e.g.,
   * 11,000,000% == 110,000 == 110 / 0.001).
   * Usually equal to 100 * 1.1 / (K_DEFAULT_PAGE_RANK / 100).
   */
  const Percentage_T K_DEFAULT_FULL_CODE_MATCH_PCT (11000 / K_DEFAULT_PAGE_RANK);

  /**
   * Default attenuation factor (e.g., 850.0). It weighs down a set
   * of strings when compared to the single string.
   * For instance, the {"paris", "texas"} combination is weighed down,
   * so that it is ranked less than "paris texas".
   */
  const Percentage_T K_DEFAULT_ATTENUATION_FCTR (850.0);

  /**
   * Default number of characters by allowable spelling error unit
   */
  const NbOfErrors_T K_DEFAULT_SIZE_FOR_SPELLING_ERROR_UNIT (4);

  /**
   * Maximum number of words a string may have (e.g., 14).
   * The issue typically arises with Bangkok (BKK):
   * http://www.geonames.org/1609350
   */
  const NbOfWords_T K_DEFAULT_MAXIMUM_NUMBER_OF_WORDS_IN_STRING (14);

  /**
   * Black list, i.e., a list of words which should not be indexed
   * and/or searched for (e.g., "airport", "international").
   */
  const BlackList_T K_BLACK_LIST = ListGenerator::createBlackList();

  // //////////////////////////////////////////////////////////////////////
  BlackList_T ListGenerator::createBlackList() {
    BlackList_T oBL;
    oBL.insert ("City"); oBL.insert ("city");
    oBL.insert ("Ville"); oBL.insert ("ville");
    oBL.insert ("Stadt"); oBL.insert ("stadt");
    oBL.insert ("Gorod"); oBL.insert ("gorod");
    oBL.insert ("Airport"); oBL.insert ("airport");
    oBL.insert ("Airfield"); oBL.insert ("airfield");
    oBL.insert ("Airbase"); oBL.insert ("airbase");
    oBL.insert ("Aeroport"); oBL.insert ("aeroport");
    oBL.insert ("Aéroport"); oBL.insert ("aéroport");
    oBL.insert ("Flughafen"); oBL.insert ("flughafen");
    oBL.insert ("Flughafe"); oBL.insert ("flughafe");
    oBL.insert ("Flughaveno"); oBL.insert ("flughaveno");
    oBL.insert ("Lufthavn"); oBL.insert ("lufthavn");
    oBL.insert ("Letise"); oBL.insert ("letise");
    oBL.insert ("Letisko"); oBL.insert ("letisko");
    oBL.insert ("Lotniczy"); oBL.insert ("lotniczy");
    oBL.insert ("Lentoasema"); oBL.insert ("lentoasema");
    oBL.insert ("Lapangan"); oBL.insert ("lapangan");
    oBL.insert ("Bandar"); oBL.insert ("bandar");
    oBL.insert ("Udara"); oBL.insert ("udara");
    oBL.insert ("Terbang"); oBL.insert ("terbang");
    oBL.insert ("Repülőtér"); oBL.insert ("repülőtér");
    oBL.insert ("Aerodrome"); oBL.insert ("aerodrome");
    oBL.insert ("Aerodromo"); oBL.insert ("aerodromo");
    oBL.insert ("Aeroporto"); oBL.insert ("aeroporto");
    oBL.insert ("Aeropuerto"); oBL.insert ("aeropuerto");
    oBL.insert ("Аэропорт"); oBL.insert ("аэропорт");
    oBL.insert ("Letiště"); oBL.insert ("letiště");
    oBL.insert ("Летище"); oBL.insert ("летище");
    oBL.insert ("Sân"); oBL.insert ("sân");
    oBL.insert ("Bay"); oBL.insert ("bay");
    oBL.insert ("Quốc"); oBL.insert ("quốc");
    oBL.insert ("Tế"); oBL.insert ("tế");
    oBL.insert ("International"); oBL.insert ("international");
    oBL.insert ("Internacional"); oBL.insert ("internacional");
    oBL.insert ("Internasional"); oBL.insert ("internasional");
    oBL.insert ("Internazionale"); oBL.insert ("internazionale");
    oBL.insert ("Internasjonale"); oBL.insert ("internasjonale");
    oBL.insert ("Международный"); oBL.insert ("международный");
    oBL.insert ("Международно"); oBL.insert ("международно");
    oBL.insert ("Kansainvälinen"); oBL.insert ("kansainvälinen");
    oBL.insert ("Nemzetközi"); oBL.insert ("nemzetközi");
    oBL.insert ("Intl"); oBL.insert ("intl");
    oBL.insert ("Saint"); oBL.insert ("saint");
    oBL.insert ("Sankt"); oBL.insert ("sankt");
    oBL.insert ("Municipal"); oBL.insert ("municipal");
    oBL.insert ("County"); oBL.insert ("county");
    oBL.insert ("Local"); oBL.insert ("local");
    oBL.insert ("Ferry"); oBL.insert ("ferry");
    oBL.insert ("Port"); oBL.insert ("port");
    oBL.insert ("Harbour"); oBL.insert ("harbour");
    oBL.insert ("Harbor"); oBL.insert ("harbor");
    return oBL;
  }

}
