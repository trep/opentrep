#ifndef __OPENTREP_FAC_FACXAPIANDB_HPP
#define __OPENTREP_FAC_FACXAPIANDB_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <vector>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>

// Forward declarations
namespace Xapian {
  class WritableDatabase;
}

namespace OPENTREP {

  /**
   * @brief Factory for a Xapian WritableDatabase object
   */
  class FacXapianDB {
  public:

    /**
     * Define the list (pool) of Xapian DB objects.
     */
    typedef std::vector<Xapian::WritableDatabase*> XapianDBPool_T;
    
    /**
     * Provide the unique instance.
     *
     * The singleton is instantiated when first used
     *
     * @return FacXapianDB&
     */
    static FacXapianDB& instance();

    /**
     * Destroy all the object instantiated by this factory.
     */
    void clean();
    
    /**
     * Destructor.
     *
     * The Destruction put the _instance to NULL in order to be clean
     * for the next FacXapianDB::instance()
     */
    ~FacXapianDB();

    /**
     * Create a new Xapian WritableDatabase object.
     *
     * This new object is added to the list of instantiated objects.
     *
     * @param const TravelDBFilePath_T& File-path of the Xapian database/index.
     * @param const int& Xapian action flag (usually Xapian::DB_CREATE)
     * @return Xapian::WritableDatabase& The newly created object.
     */
    Xapian::WritableDatabase* create (const TravelDBFilePath_T&,
                                      const int& iXapianActionFlag);

  private:
    /**
     * Default Constructor.
     *
     * This constructor is protected in order to ensure the singleton
     * pattern.
     */
    FacXapianDB() {}

  private:
    /**
     * The unique instance.
     */
    static FacXapianDB* _instance;

    /**
     * List of instantiated Xapian DB objects
     */
    XapianDBPool_T _pool;

  };

}
#endif // __OPENTREP_FAC_FACXAPIANDB_HPP
