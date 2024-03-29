// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// Boost (STL Extension)
// Boost Filesystem (http://www.boost.org/doc/libs/1_48_0/libs/filesystem/doc/index.htm)
#include <boost/version.hpp>
#if BOOST_VERSION >= 103500
#include <boost/filesystem.hpp>
#else // BOOST_VERSION >= 103500
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#endif // BOOST_VERSION >= 103500
// Opentrep
#include <opentrep/basic/BasFileMgr.hpp>

namespace boostfs = boost::filesystem;

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  bool BasFileMgr::doesExistAndIsReadable (const std::string& iFilepath) {
    bool oFine = false;
    
    boostfs::path lPath (iFilepath);

    if (boostfs::exists (lPath) == false) {
      return oFine;
    }

#if BOOST_VERSION >= 103500
    if (boostfs::is_regular_file (lPath) == true) {
      oFine = true;
    }
#endif // BOOST_VERSION >= 103500

    return oFine;
  }
  
}
