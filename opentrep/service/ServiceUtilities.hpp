#ifndef __OPENTREP_SVC_SERVICEUTILITIES_HPP
#define __OPENTREP_SVC_SERVICEUTILITIES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>

namespace OPENTREP {

  /** Class wrapping some utility functions. */
  class ServiceUtilities {
  public:
    /** States whether the path given as parameter exists and is a directory.
        @param const std::string& Path to be tested for existence.
        @return bool Whether or not the path corresponds to a directory. */
    static bool isDirectory (const std::string& iPath);
  };

}
#endif // __OPENTREP_SVC_SERVICEUTILITIES_HPP
