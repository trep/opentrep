#ifndef __OPENTREP_FAC_FACOPENTREPSERVICECONTEXT_HPP
#define __OPENTREP_FAC_FACOPENTREPSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// OpenTrep
#include <opentrep/factory/FacServiceAbstract.hpp>

namespace OPENTREP {

  // Forward declarations.
  class OPENTREP_ServiceContext;


  /**
   * @brief Factory for Bucket.
   */
  class FacOpenTrepServiceContext : public FacServiceAbstract {
  public:

    /**
     * Provide the unique instance.
     *
     * The singleton is instantiated when first used
     *
     * @return FacOpentrepServiceContext&
     */
    static FacOpenTrepServiceContext& instance();

    /**
     * Destructor.
     *
     * The Destruction put the _instance to NULL in order to be clean
     * for the next FacOpentrepServiceContext::instance()
     */
    ~FacOpenTrepServiceContext();

    /**
     * Create a new OPENTREP_ServiceContext object.
     *
     * This new object is added to the list of instantiated objects.
     *
     * @return OPENTREP_ServiceContext& The newly created object.
     */
    OPENTREP_ServiceContext& create (const TravelDatabaseName_T&);

    /**
     * Create a new OPENTREP_ServiceContext object.
     *
     * This new object is added to the list of instantiated objects.
     *
     * @param const PORFilePath_T& File-path of the POR file.
     * @param const TravelDatabaseName_T& File-path of the Xapian database.
     * @return OPENTREP_ServiceContext& The newly created object.
     */
    OPENTREP_ServiceContext& create (const PORFilePath_T&,
                                     const TravelDatabaseName_T&);


  protected:
    /**
     * Default Constructor.
     *
     * This constructor is protected in order to ensure the singleton
     * pattern.
     */
    FacOpenTrepServiceContext() {}

  private:
    /**
     * The unique instance.
     */
    static FacOpenTrepServiceContext* _instance;
  };

}
#endif // __OPENTREP_FAC_FACOPENTREPSERVICECONTEXT_HPP
