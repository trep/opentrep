#ifndef __OPENTREP_BOM_NAMEMATRIX_HPP
#define __OPENTREP_BOM_NAMEMATRIX_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
#include <string>
#include <list>
#include <map>
// OpenTrep
#include <opentrep/basic/StructAbstract.hpp>
#include <opentrep/bom/Names.hpp>

namespace OPENTREP {

  /**
   * Structure holding names for a BOM type (place, day, month, etc.),
   * for a list of given languages.
   */
  struct NameMatrix : public StructAbstract {
  public:
    // //// Constructors / Destructors ///////
    /**
     * Default constructor.
     */
    NameMatrix();
    /**
     * Copy constructor.
     */
    NameMatrix (const NameMatrix&);
    /**
     * Destructor.
     */
    ~NameMatrix();


  public:
    // /////////// Getters ///////////////
    /**
     * Get the map of name lists.
     */
    const NameMatrix_T& getNameMatrix() const {
      return _nameMatrix;
    }

    /**
     * Get, for a given language (code), the corresponding list of names.
     *
     * @param const Language::EN_Language& Language code.
     * @param NameList_T& Empty list of names, which will be filled by the
     *        method if a list exists for that language code.
     * @return bool Whether or not such a list exists for the given
     *         language.
     */
    bool getNameList (const Language::EN_Language&, NameList_T&) const;


  public:
    // /////////// Setters ///////////////
    /**
     * Add a name for the place.
     *
     * @param const Language::EN_Language& Language in which to add the name.
     * @param const std::string& Name to be added.
     */
    void addName (const Language::EN_Language&, const std::string& iName);

    /**
     * Reset the map of name lists.
     */
    void reset();


  public:
    // /////////// Display support methods /////////
    /**
     * Dump a Business Object into an output stream.
     *
     * @param ostream& the output stream.
     */
    void toStream (std::ostream&) const;

    /**
     * Read a Business Object from an input stream.
     *
     * @param istream& the input stream.
     */
    void fromStream (std::istream&);

    /**
     * Display the key of the structure.
     *
     * @return std::string Dump of the structure.
     */
    std::string describeKey() const;

    /**
     * Display of the structure.
     *
     * @return std::string Dump of the structure.
     */
    std::string describe() const;


  private:    
    // //////////// Attributes //////////////
    /**
     * Matrix of names, for the given place.
     */
    NameMatrix_T _nameMatrix;
  };

}
#endif // __OPENTREP_BOM_NAMEMATRIX_HPP
