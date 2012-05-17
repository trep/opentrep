// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
#include <opentrep/basic/BasConst_General.hpp>
#include <opentrep/basic/BasConst_OPENTREP_Service.hpp>

namespace OPENTREP {

  /**
   * Default Xapian database name for the OPENTREP_Service.
   */
  const std::string DEFAULT_OPENTREP_SERVICE_DB_NAME = "traveldb";

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
