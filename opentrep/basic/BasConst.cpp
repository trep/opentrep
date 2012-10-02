// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
#include <opentrep/basic/BasConst_OPENTREP_Service.hpp>
#include <opentrep/basic/BasConst_Unicode.hpp>
#include <opentrep/basic/BasConst_General.hpp>

namespace OPENTREP {

  // ////////////// OPENTREP_Service ///////////////
  /**
   * Default file-path for the POR file for the OPENTREP_Service.
   */
  const std::string DEFAULT_OPENTREP_SERVICE_POR_FILEPATH ("por_default.csv");
  
  /**
   * Default Xapian database name for the OPENTREP_Service.
   */
  const std::string DEFAULT_OPENTREP_SERVICE_DB_NAME ("traveldb");


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
    "[\\u02B9] > \\u0027; [\\u002D] > \\u0020; [\\u0027] > \\u0020;";

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
   * Default PageRank (e.g., 0.1% == 0.001)
   */
  const Percentage_T K_DEFAULT_PAGE_RANK (0.1);

  /**
   * Default number of characters by allowable spelling error unit
   */
  const NbOfErrors_T K_DEFAULT_SIZE_FOR_SPELLING_ERROR_UNIT (4);

  /**
   * Black list, i.e., a list of words which should not be indexed
   * and/or searched for (e.g., "airport", "international").
   */
  const BlackList_T K_BLACK_LIST = ListGenerator::createBlackList();

  // //////////////////////////////////////////////////////////////////////
  BlackList_T ListGenerator::createBlackList() {
    BlackList_T oBL;
    oBL.insert ("Airport"); oBL.insert ("airport");
    oBL.insert ("Airfield"); oBL.insert ("airfield");
    oBL.insert ("Airbase"); oBL.insert ("airbase");
    oBL.insert ("Aeroport"); oBL.insert ("aeroport");
    oBL.insert ("Aéroport"); oBL.insert ("aéroport");
    oBL.insert ("Flughafen"); oBL.insert ("flughafen");
    oBL.insert ("Flughafe"); oBL.insert ("flughafe");
    oBL.insert ("Flughaveno"); oBL.insert ("flughaveno");
    oBL.insert ("Lufthavn"); oBL.insert ("lufthavn");
    oBL.insert ("Lentoasema"); oBL.insert ("lentoasema");
    oBL.insert ("Lapangan"); oBL.insert ("lapangan");
    oBL.insert ("Terbang"); oBL.insert ("terbang");
    oBL.insert ("Repülőtér"); oBL.insert ("repülőtér");
    oBL.insert ("Aerodrome"); oBL.insert ("aerodrome");
    oBL.insert ("Aerodromo"); oBL.insert ("aerodromo");
    oBL.insert ("Aeroporto"); oBL.insert ("aeroporto");
    oBL.insert ("Aeropuerto"); oBL.insert ("aeropuerto");
    oBL.insert ("Аэропорт"); oBL.insert ("аэропорт");
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
    oBL.insert ("Lotniczy"); oBL.insert ("lotniczy");
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
