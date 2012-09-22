#ifndef __OPENTREP_NAMES_HPP
#define __OPENTREP_NAMES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
#include <string>
#include <list>
#include <map>
// OpenTrep
#include <opentrep/OPENTREP_Abstract.hpp>
#include <opentrep/OPENTREP_Types.hpp>

namespace OPENTREP {


  // ////////////////// Type definitions ///////////////////
  typedef std::list<std::string> NameList_T;

  /**
   * Structure holding names for a BOM type (place, day, month, etc.),
   * in a given language.
   */
  struct Names : public OPENTREP_Abstract {
  public:
    // //// Constructors / Destructors ///////
    /**
     * Main constructor.
     */
    Names (const LanguageCode_T&);
    /**
     * Copy constructor.
     */
    Names (const Names&);
    /**
     * Destructor.
     */
    ~Names();
  private:
    /**
     * Default constructor: should not be used.
     */
    Names();


  public:
    // /////////// Getters ///////////////
    /**
     * Get the language code.
     */
    LanguageCode_T getLanguageCode() const {
      return _languageCode;
    }

    /**
     * Get the name list.
     */
    const NameList_T& getNameList() const {
      return _nameList;
    }

    /**
     * Get the first name of the list.
     *
     * Note that it can be empty (when the list is itself empty).
     */
    std::string getFirstName() const;


  public:
    // /////////// Setters ///////////////
    /**
     * Set the language code.
     */
    void setLanguageCode (const LanguageCode_T& iLanguageCode) {
      _languageCode = iLanguageCode;
    }

    /**
     * Add a name to the dedicated list.
     */
    void addName (const std::string& iName);

    /**
     * Reset the list of names to null (i.e., to an empty list).
     */
    void resetList();


  public:
    // /////////// Display support methods /////////
    /**
     * Dump a Business Object into an output stream.
     *
     * @param ostream& the output stream.
     */
    void toStream (std::ostream& ioOut) const;

    /**
     * Read a Business Object from an input stream.
     *
     * @param istream& the input stream.
     */
    void fromStream (std::istream& ioIn);

    /**
     * Get the serialised version of the structure. 
     */
    std::string toString() const;

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
     * Language code (e.g., en_US, fr_FR, etc.).
     */
    LanguageCode_T _languageCode;

    /**
     * List of names, for the given place, and in the current language.
     */
    NameList_T _nameList;
  };

  // ////////////// Type definitions ////////////
  /**
   * Matrix of place names: for each of the language, the place gets a
   * corresponding list of names.
   *
   * For instance, MUC corresponds to Munich in English, München
   * in German, Munique in French, Мюнхен in Russian, etc.
   */
  typedef std::map<LanguageCode_T, Names> NameMatrix_T;

}
#endif // __OPENTREP_NAMES_HPP
