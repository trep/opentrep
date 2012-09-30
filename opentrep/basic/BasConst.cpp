// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
#include <opentrep/basic/BasConst_OPENTREP_Service.hpp>
#include <opentrep/basic/BasConst_Unicode.hpp>
#include <opentrep/basic/BasConst_General.hpp>

namespace OPENTREP {

  // ////////////// OPENTREP_Service ///////////////
  /**
   * Default file-path for the PageRank value file for the OPENTREP_Service.
   */
  const std::string
  DEFAULT_OPENTREP_SERVICE_PR_FILEPATH ("ref_airport_pageranked.csv");
  
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
   * Black list, i.e., a list of words which should not be indexed
   * and/or searched for (e.g., "airport", "international").
   */
  const BlackList_T K_BLACK_LIST = ListGenerator::createBlackList();

  // //////////////////////////////////////////////////////////////////////
  BlackList_T ListGenerator::createBlackList() {
    BlackList_T oBL;
    oBL.push_back ("airport"); oBL.push_back ("airfield");
    oBL.push_back ("airbase"); oBL.push_back ("aerodrome");
    oBL.push_back ("aerodromo"); oBL.push_back ("aeroporto");
    oBL.push_back ("international"); oBL.push_back ("intl");
    oBL.push_back ("saint"); oBL.push_back ("sankt");
    oBL.push_back ("municipal"); oBL.push_back ("county");
    oBL.push_back ("local");
    oBL.push_back ("ferry");  oBL.push_back ("port");oBL.push_back ("harbour");
    return oBL;
  }

}
