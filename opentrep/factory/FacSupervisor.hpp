#ifndef __OPENTREP_FAC_FACSUPERVISOR_HPP
#define __OPENTREP_FAC_FACSUPERVISOR_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <vector>

namespace OPENTREP {

  // Forward declarations
  class FacBomAbstract;
  class FacServiceAbstract;
  class FacXapianDB;
  class Logger;

  /**
   * Singleton class to register and clean all the factories.
   */
  class FacSupervisor {
  public:

    /**
     * Define the pool (list) of factories.
     */
    typedef std::vector<FacBomAbstract*> BomFactoryPool_T;
    typedef std::vector<FacServiceAbstract*> ServiceFactoryPool_T;

    /**
     * Provides the unique instance.
     * The singleton is instantiated when first used.
     * @return FacSupervisor&
     */
    static FacSupervisor& instance();

    /**
     * Register a newly instantiated concrete factory for the Bom layer.
     * When a concrete Factory is firstly instantiated,
     * this factory has to register itself to the FacSupervisor.
     *
     * @param FacAbstract& the concrete Factory to register.
     */
    void registerBomFactory (FacBomAbstract*);

    /**
     * Register a newly instantiated concrete factory for the Service layer.
     * When a concrete Factory is firstly instantiated,
     * this factory has to register itself to the FacSupervisor.
     * @param FacServiceAbstract& the concrete Factory to register.
     */
    void registerServiceFactory (FacServiceAbstract*);

    /**
     * Register a newly instantiated concrete factory for the
     * FacXapianDB object. In fact, as the FacXapianDB object
     * follows the singleton pattern, the concrete factory is the
     * FacXapianDB object itself.
     * When a concrete Factory is firstly instantiated, this
     * factory has to register itself to the FacSupervisor.
     * @param FacXapianDB* The concrete service/object to register.
     */
    void registerXapianDBFactory (FacXapianDB*);

    /**
     * Register a newly instantiated concrete factory for the
     * Logger object. In fact, as the Logger object
     * follows the singleton pattern, the concrete factory is the
     * Logger object itself.
     * When a concrete Factory is firstly instantiated, this
     * factory has to register itself to the FacSupervisor.
     * @param Logger* The concrete service/object to register.
     */
    void registerLoggerService (Logger*);

    /**
     * Clean all created object.
     * Call the clean method of all the instantiated  factories
     * for the Bom layer.
     */
    void cleanBomLayer();

    /**
     * Clean all Service created object.
     * Call the clean method of all the instantiated  factories
     * for the Service layer.
     */
    void cleanServiceLayer();

    /**
     * Delete the FacXapianDB object.
     */
    void cleanFacXapianDB();

    /**
     * Delete the Logger object.
     */
    void cleanLoggerService();

    /**
     * Clean the static instance.
     * The singleton is deleted.
     */
    static void cleanFactory ();

    /**
     * Destructor
     * The static instance is deleted (and reset to NULL)
     * by the static cleanFactory() method.
     */
    ~FacSupervisor();

  protected:
    /**
     * Default Constructor.
     * This constructor is protected 
     * to ensure the singleton pattern.
     */
    FacSupervisor ();
    FacSupervisor (const FacSupervisor&) {}


  private:
    /**
     * The unique instance.
     */
    static FacSupervisor* _instance;

    /**
     * FacXapianDB (singleton) instance.
     */
    FacXapianDB* _facXapianDB;
      
    /**
     * Logger (singleton) instance.
     */
    Logger* _logger;
      
    /**
     * List of instantiated factories for the Bom layer.
     */
    BomFactoryPool_T _bomPool;

    /**
     * List of instantiated factories for the Service layer.
     */
    ServiceFactoryPool_T _svcPool;
  };
}  
#endif // __OPENTREP_FAC_FACSUPERVISOR_HPP
