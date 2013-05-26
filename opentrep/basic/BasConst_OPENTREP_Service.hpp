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
  
  /**
   * Default name and location for the MySQL database (if existing; currently,
   * it does not).
   *
   * Usually, the default MySQL database details are:
   * <ul>
   *   <li>User: geo</li>
   *   <li>Passwd: geo</li>
   *   <li>DB name: geo_ori</li>
   *   <li>Server hostname: localhost</li>
   *   <li>Server port: 3306</li>
   * </ul>
   */
  extern const std::string DEFAULT_OPENTREP_MYSQL_DB_USER;
  extern const std::string DEFAULT_OPENTREP_MYSQL_DB_PASSWD;
  extern const std::string DEFAULT_OPENTREP_MYSQL_DB_DBNAME;
  extern const std::string DEFAULT_OPENTREP_MYSQL_DB_HOST;
  extern const std::string DEFAULT_OPENTREP_MYSQL_DB_PORT;

  /**
   * Default name and location for the SQLite3 database.
   *
   * Usually, the default file-path on Linux/Unix is
   * <tt><installation directory>/share/opentrep/data/ori_por_public.db</tt>
   * where <tt><installation directory></tt> may be:
   * <ul>
   *   <li><tt>/usr</tt> when installed as root (e.g., with the RPM)</li>
   *   <li><tt>$HOME/dev/deliveries/opentrep-<version></tt></li>
   * </ul>
   */
  extern const std::string DEFAULT_OPENTREP_SQLITE_DB_FILEPATH;
}
#endif // __OPENTREP_BAS_BASCONST_OPENTREP_SERVICE_HPP
