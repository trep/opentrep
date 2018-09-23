#ifndef __OPENTREP_SVC_LOGGER_HPP
#define __OPENTREP_SVC_LOGGER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
#include <string>
// Boost Date-Time
#include <boost/date_time.hpp>
// OpenTREP
#include <opentrep/OPENTREP_Types.hpp>

// /////////////// LOG MACROS /////////////////
#define OPENTREP_LOG_CORE(iLevel, iToBeLogged) \
  { std::ostringstream ostr; ostr << iToBeLogged; \
    OPENTREP::Logger::instance().log (iLevel, __LINE__, __FILE__, ostr.str()); }

#define OPENTREP_LOG_CRITICAL(iToBeLogged) \
  OPENTREP_LOG_CORE (OPENTREP::LOG::CRITICAL, iToBeLogged)

#define OPENTREP_LOG_ERROR(iToBeLogged) \
  OPENTREP_LOG_CORE (OPENTREP::LOG::ERROR, iToBeLogged)

#define OPENTREP_LOG_NOTIFICATION(iToBeLogged) \
  OPENTREP_LOG_CORE (OPENTREP::LOG::NOTIFICATION, iToBeLogged)

#define OPENTREP_LOG_WARNING(iToBeLogged) \
  OPENTREP_LOG_CORE (OPENTREP::LOG::WARNING, iToBeLogged)

#define OPENTREP_LOG_DEBUG(iToBeLogged) \
  OPENTREP_LOG_CORE (OPENTREP::LOG::DEBUG, iToBeLogged)

#define OPENTREP_LOG_VERBOSE(iToBeLogged) \
  OPENTREP_LOG_CORE (OPENTREP::LOG::VERBOSE, iToBeLogged)
// /////////// (END OF) LOG MACROS /////////////


namespace OPENTREP {

  /**
   * Class managing the stream for logs. 
   *
   * Note that the error logs are seen as standard output logs, 
   * but with a higher level of visibility.
   */
  class Logger {
    // Friend classes
    friend class FacSupervisor;
  public:
    
    /**
     * Main log entry.
     */
    template <typename T>
    void log (const LOG::EN_LogLevel iLevel, const int iLineNumber,
              const std::string& iFileName, const T& iToBeLogged) {
      if (iLevel <= _level) {
        assert (_logStream != NULL);
        
        // Get the current time in UTC Timezone
	boost::posix_time::ptime lTimeUTC =
          boost::posix_time::second_clock::universal_time();

        // Add some context and write down the log element
        *_logStream << "[" << lTimeUTC << "][" << iFileName << "#"
                    << iLineNumber << "]:" << iToBeLogged << std::endl;
      }
    }
    
    /**
     * Get the log level.
     */
    LOG::EN_LogLevel getLogLevel();
    
    /**
     * Get the log stream.
     */
    std::ostream& getLogStream();
    
    /**
     * Set the logger parameters (level and stream).
     */
    void setLogParameters (const LOG::EN_LogLevel iLogLevel, 
                           std::ostream& ioLogStream);
    
    /**
     * Returns a current Logger instance.
     */
    static Logger& instance();
    
  private:
    /**
     * Default constructors are private so that only the required 
     * constructor can be used.
     */
    Logger ();
    Logger (const Logger&);
    Logger (const LOG::EN_LogLevel iLevel, std::ostream& ioLogStream);

    /**
     * Destructor.
     */
    ~Logger ();
    
  private:
    /**
     * Log level.
     */
    LOG::EN_LogLevel _level;
    
    /**
     * Stream dedicated to the logs.
     */
    std::ostream* _logStream;
    
    /**
     * Singleton/Instance object.
     */
    static Logger* _instance;
  };
  
}
#endif // __OPENTREP_SVC_LOGGER_HPP
