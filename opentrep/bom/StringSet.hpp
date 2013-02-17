#ifndef __OPENTREP_BOM_STRINGSET_HPP
#define __OPENTREP_BOM_STRINGSET_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <list>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/basic/StructAbstract.hpp>

namespace OPENTREP {

  /**
   * @brief Class holding a set of strings, e.g., {"rio", "de", "janeiro"}
   */
  struct StringSet : public StructAbstract {
  public:
    // //////////////// Type definitions //////////////////
    /**
     * List of strings.
     */
    typedef std::list<std::string> StringSet_T;

  public:
    /**
     * Add an item (string) into the list.
     *
     * \note When the given string is empty (zero-length), it is (obviously)
     *       not added to the list
     */
    void push_back (const std::string&);
  
    /**
     * Add all the items (string) into the list.
     */
    void push_back (const StringSet&);
  
    /**
     * Return the size of the list.
     */
    size_t size() const;
  
    /**
     * Return whether or not the list is empty.
     */
    bool empty() const;
  
    /**
     * Empty the list.
     */
    void clear();
    
    /**
     * Get the first string of the underlying list/set.
     */
    std::string getFirstString() const;


  public:
    // /////////// Display support methods /////////
    /**
     * Dump the structure into an output stream.
     *
     * @param ostream& the output stream.
     */
    void toStream (std::ostream&) const;

    /**
     * Read a structure from an input stream.
     *
     * @param istream& the input stream.
     */
    void fromStream (std::istream&);

    /**
     * Get a string describing the whole key (differentiating two objects
     * at any level).
     */
    std::string describeKey() const;

    /**
     * Get the serialised version of the structure.
     */
    std::string describe() const;


  public:
    // //////////////// Constructors and Destructors /////////////
    /**
     * Default constructor.
     */
    StringSet();

    /**
     * Default copy constructor.
     */
    StringSet (const StringSet&);

    /**
     * Main constructor.
     */
    StringSet (const std::string&);

    /**
     * Default destructor.
     */
    ~StringSet();

  
  public:
    // //////////////// Attributes ///////////////
    /**
     * String set, i.e., a list of strings
     */
    StringSet_T _set;
  };

}
#endif // __OPENTREP_BOM_STRINGSET_HPP
