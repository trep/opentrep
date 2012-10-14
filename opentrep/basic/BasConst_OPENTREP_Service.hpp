#ifndef __OPENTREP_BAS_BASCONST_OPENTREP_SERVICE_HPP
#define __OPENTREP_BAS_BASCONST_OPENTREP_SERVICE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
#include <string>

namespace OPENTREP {

  /**
   * Default file-path for the ORI-maintained POR (points of reference) file.
   *
   * Usually, the default file-path on Linux/Unix is
   * <tt><installation directory>/share/opentrep/data/ori_por_public.csv</tt>
   * where <tt><installation directory></tt> may be:
   * <ul>
   *   <li><tt>/usr</tt> when installed as root (e.g., with the RPM)</li>
   *   <li><tt>$HOME/dev/deliveries/opentrep-<version></tt></li>
   * </ul>
   */
  extern const std::string DEFAULT_OPENTREP_POR_FILEPATH;
  
  /**
   * Default file-path for the Xapian database/index.
   *
   * Usually, the default file-path on Linux/Unix is
   * <tt>/tmp/opentrep/traveldb</tt>
   */
  extern const std::string DEFAULT_OPENTREP_XAPIAN_DB_FILEPATH;
  
}
#endif // __OPENTREP_BAS_BASCONST_OPENTREP_SERVICE_HPP
