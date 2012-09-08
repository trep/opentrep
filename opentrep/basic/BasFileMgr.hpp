#ifndef __OPENTREP_BAS_BASFILEMGR_HPP
#define __OPENTREP_BAS_BASFILEMGR_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>

namespace OPENTREP {

  /**
   * @brief Helper class for operations on files and on the file-system.
   */
  struct BasFileMgr {
  public:

    // //////////// Functional Support Methods ////////////
    static bool doesExistAndIsReadable (const std::string& iFilepath);
  };

}
#endif // __OPENTREP_BAS_BASFILEMGR_HPP
