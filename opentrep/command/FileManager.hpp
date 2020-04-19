#ifndef __OPENTREP_CMD_FILEMANAGER_HPP
#define __OPENTREP_CMD_FILEMANAGER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>

namespace OPENTREP {

  /**
   * @brief Class wrapping utilities for file-system access.
   */
  class FileManager {
  public:
    /**
     * Check that the directory hosting the SQLite database exists
     * and is accessible.
     */
    static bool checkSQLiteDirectory (const std::string& iSQLDBConnStr);

    /**
     * Check that the directory hosting the Xapian database/index exists
     * and is accessible.
     */
    static bool checkXapianDBOnFileSystem (const TravelDBFilePath_T&);

    /**
     * Delete and re-create the directory hosting the Xapian index (aka database)
     */
    static void recreateXapianDirectory (const std::string& iTravelDBFilePath);
    
  private:
    /**
     * Default constructor.
     */
    FileManager() {}
    /**
     * Default copy constructor.
     */
    FileManager (const FileManager&) {}
    /**
     * Destructor.
     */
    ~FileManager() {}
  };
  
}
#endif // __OPENTREP_CMD_FILEMANAGER_HPP
