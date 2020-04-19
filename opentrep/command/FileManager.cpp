// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// Boost
#include <boost/filesystem.hpp>
// OpenTrep
#include <opentrep/command/FileManager.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  bool FileManager::checkSQLiteDirectory (const std::string& iSQLDBConnStr) {
    bool oExistSQLDBDir = false;
    
    // Retrieve the full file-path of the SQLite3 directory
    boost::filesystem::path lSQLiteDBFullPath (iSQLDBConnStr.begin(),
                                               iSQLDBConnStr.end());

    // Retrieve the directory hosting the SQLite3 database
    boost::filesystem::path lSQLiteDBParentPath =
      lSQLiteDBFullPath.parent_path();

    // Check that the directory exists and is actually a directory
    oExistSQLDBDir = boost::filesystem::exists (lSQLiteDBParentPath)
      && boost::filesystem::is_directory (lSQLiteDBParentPath);

    return oExistSQLDBDir;
  }

  // //////////////////////////////////////////////////////////////////////
  bool FileManager::
  checkXapianDBOnFileSystem (const TravelDBFilePath_T& iTravelDBFilePath) {
    bool oExistXapianDBDir = false;

    // Convert into Boost structure
    boost::filesystem::path lTravelDBFilePath (iTravelDBFilePath.begin(),
                                               iTravelDBFilePath.end());

    // Check that the directory exists and is actually a directory
    oExistXapianDBDir = boost::filesystem::exists (lTravelDBFilePath)
      && boost::filesystem::is_directory (lTravelDBFilePath);

    return oExistXapianDBDir;
  }
  
  // //////////////////////////////////////////////////////////////////////
  void FileManager::
  recreateXapianDirectory (const std::string& iTravelDBFilePath) {
    // Remove any existing directory for Xapian
    boost::filesystem::path lTravelDBFilePath (iTravelDBFilePath.begin(),
                                               iTravelDBFilePath.end());
    // DEBUG
    OPENTREP_LOG_DEBUG ("The Xapian database ('" << iTravelDBFilePath
                        << "') will be cleared");
    boost::filesystem::remove_all (lTravelDBFilePath);

    // Re-create the directory for Xapian
    boost::filesystem::create_directories (lTravelDBFilePath);

    // Check whether the just created directory exists and is a directory
    if (!(boost::filesystem::exists (lTravelDBFilePath)
          && boost::filesystem::is_directory (lTravelDBFilePath))) {
      std::ostringstream oStr;
      oStr << "The directory for the Xapian database/index ('"
           << lTravelDBFilePath << "') cannot be created; check file-system "
           << "permissions and whether the file-system is writable";
      OPENTREP_LOG_ERROR (oStr.str());
      throw FileNotFoundException (oStr.str());
    }
  }
  
}

