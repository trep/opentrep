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
   * Default boolean indicator for the SQLite3 database filling.
   *
   * Usually, the SQLite3 database is not filled at the same time
   * as the Xapian database/index. Indeed, SQLite3 database filling takes
   * a lot of time on some configurations (e.g., CentOS 6.x VM).
   */
  extern const bool DEFAULT_OPENTREP_SQLITE_DB_FILLING_INDICATOR;

  /**
   * Default type for the SQL database (e.g., "nodb", i.e., no SQL database).
   */
  extern const std::string DEFAULT_OPENTREP_SQL_DB_TYPE;

  /**
   * Default connection string for the MySQL/MariaDB database.
   *
   * Usually, the default parameters are
   * <ul>
   *   <li>Database user name: <tt>trep</tt></li>
   *   <li>Database user password: <tt>trep</tt></li>
   *   <li>Database name: <tt>trep_trep</tt></li>
   *   <li>Server hostname: <tt>localhost</tt></li>
   *   <li>Server port: <tt>3306</tt></li>
   * </ul>
   */
  extern const std::string DEFAULT_OPENTREP_MYSQL_CONN_STRING;

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
}
#endif // __OPENTREP_BAS_BASCONST_OPENTREP_SERVICE_HPP
