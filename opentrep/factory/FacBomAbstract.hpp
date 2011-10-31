#ifndef __OPENTREP_FAC_FACBOMABSTRACT_HPP
#define __OPENTREP_FAC_FACBOMABSTRACT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <vector>

namespace OPENTREP {

  // Forward declarations
  class BomAbstract;

  /** Base class for Factory layer. */
  class FacBomAbstract {
    friend class FacSupervisor;
  public:

    /** Define the list (pool) of Bom objects. */
    typedef std::vector<BomAbstract*> BomPool_T;

    /** Return the ID corresponding to the given object pointer. */
    static std::size_t getID (const BomAbstract*);

    /** Return the ID corresponding to the given object reference. */
    static std::size_t getID (const BomAbstract&);

    /** Return the ID, as a string, corresponding to the given object
        pointer. */
    static std::string getIDString (const BomAbstract*);

    /** Return the ID, as a string, corresponding to the given object
        reference. */
    static std::string getIDString (const BomAbstract&);

  protected:
    /** Default Constructor.
        <br>This constructor is protected to ensure the class is abstract. */
    FacBomAbstract() {}
    FacBomAbstract(const FacBomAbstract&) {}

    /** Destructor. */
    virtual ~FacBomAbstract();

  private:
    /** Destroyed all the object instantiated by this factory. */
    void clean();

  protected:
    /** List of instantiated Business Objects*/
    BomPool_T _pool;
  };
}
#endif // __OPENTREP_FAC_FACBOMABSTRACT_HPP
