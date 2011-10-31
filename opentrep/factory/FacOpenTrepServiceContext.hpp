#ifndef __OPENTREP_FAC_FACOPENTREPSERVICECONTEXT_HPP
#define __OPENTREP_FAC_FACOPENTREPSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// OpenTrep
#include <opentrep/factory/FacServiceAbstract.hpp>

namespace OPENTREP {

  /** Forward declarations. */
  class OPENTREP_ServiceContext;

  /** Factory for Bucket. */
  class FacOpenTrepServiceContext : public FacServiceAbstract {
  public:

    /** Provide the unique instance.
        <br> The singleton is instantiated when first used
        @return FacOpentrepServiceContext& */
    static FacOpenTrepServiceContext& instance();

    /** Destructor.
        <br> The Destruction put the _instance to NULL
        in order to be clean for the next
        FacOpentrepServiceContext::instance() */
    ~FacOpenTrepServiceContext();

    /** Create a new OPENTREP_ServiceContext object.
        <br>This new object is added to the list of instantiated objects.
        @return OPENTREP_ServiceContext& The newly created object. */
    OPENTREP_ServiceContext& create (const std::string& iTravelDatabaseName);

    
  protected:
    /** Default Constructor.
        <br>This constructor is protected in order to ensure the singleton
        pattern.*/
    FacOpenTrepServiceContext () {}

  private:
    /** The unique instance.*/
    static FacOpenTrepServiceContext* _instance;
  };

}
#endif // __OPENTREP_FAC_FACOPENTREPSERVICECONTEXT_HPP
