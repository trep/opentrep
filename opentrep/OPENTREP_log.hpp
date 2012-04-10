#ifndef __OPENTREP_OPENTREP_LOG_HPP
#define __OPENTREP_OPENTREP_LOG_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////

namespace OPENTREP {

  // /////////////// Log /////////////
  /** 
   * Level of logs. 
   */
  namespace LOG {
    typedef enum {
      CRITICAL = 0,
      ERROR,
      NOTIFICATION,
      WARNING,
      DEBUG,
      VERBOSE,
      LAST_VALUE
    } EN_LogLevel;
  }
  
}
#endif // __OPENTREP_OPENTREP_LOG_HPP
