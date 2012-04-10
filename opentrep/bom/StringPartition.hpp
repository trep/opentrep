#ifndef __OPENTREP_BOM_STRINGPARTITION_HPP
#define __OPENTREP_BOM_STRINGPARTITION_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <list>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/bom/BomAbstract.hpp>
#include <opentrep/bom/StringSet.hpp>

namespace OPENTREP {

  /**
   * Class holding a string partition.
   *
   * A string partition contains all the ways to combine, serially,
   * all the words of a given string.
   *
   * For instance, "rio de janeiro" will give:
   * <ul>
   *   <li>{</li>
   *   <li><ul>
   *     <li>{"rio", "de", "janeiro"}</li>
   *     <li>{"rio", "de janeiro"}</li>
   *     <li>{"rio de", "janeiro"}</li>
   *     <li>{"rio de janeiro"}</li>
   *   </ul></li>
   *   <li>}</li>
   * </ul>
   */
  struct StringPartition : public BomAbstract {
    // //////////////// Type definitions //////////////////
    /**
     * Type gathering all the partitions of a string.
     */
    typedef std::list<StringSet> StringPartition_T;
  
  public:
    /**
     * Add an item (StringSet) into the list.
     */
    void push_back (const StringSet& iStringSet);
  
    /**
     * Return the size of the list.
     */
    size_t size() const;
  
    /**
     * Empty the list.
     */
    void clear();
  

  private:
    /**
     * Main partition algorithm.
     *
     * That method is called by the main constructor. It should not be called
     * directly.
     */
    void init (const std::string& iString);


  public:
    // /////////// Display support methods /////////
    /**
     * Dump the structure into an output stream.
     *
     * @param ostream& the output stream.
     */
    void toStream (std::ostream& ioOut) const;

    /**
     * Read a structure from an input stream.
     *
     * @param istream& the input stream.
     */
    void fromStream (std::istream& ioIn);

    /**
     * Get the serialised version of the structure.
     */
    std::string toString() const;
    
    /**
     * Get a shorter serialised version of the structure.
     */
    std::string toShortString() const;
    
    /**
     * Get a string describing the whole key (differentiating two objects
     * at any level).
     */
    std::string describeKey() const;

    /**
     * Get a string describing the short key (differentiating two objects
     * at the same level).
     */
    std::string describeShortKey() const;


  public:
    // //////////////// Constructors and Destructors /////////////
    /**
     * Constructor.
     */
    StringPartition (const std::string& iString);

    /**
     * Default destructor.
     */
    ~StringPartition();


  private:
    // //////////////// Attributes ///////////////
    /**
     * Partition, i.e., a list containing sub-lists of strings
     */
    StringPartition_T _partition;
  };

}
#endif // __OPENTREP_BOM_STRINGPARTITION_HPP
