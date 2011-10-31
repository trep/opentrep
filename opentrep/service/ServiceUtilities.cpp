// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// Boost::Filesystem
#include <boost/filesystem.hpp>
// OpenTREP
#include <opentrep/service/Logger.hpp>
#include <opentrep/service/ServiceUtilities.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  bool ServiceUtilities::isDirectory (const std::string& iPath) {
    bool oIsDirectory = false;
    
    if (boost::filesystem::exists (iPath) == true) {

      if (boost::filesystem::is_directory (iPath) == true) {
        oIsDirectory = true;
      
      } else {
        OPENTREP_LOG_ERROR (iPath << " exists, but is not a directory");
      }

    } else {
      OPENTREP_LOG_ERROR (iPath << " does not exist");
    }

    return oIsDirectory;
  }

}
