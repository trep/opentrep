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
#include <opentrep/basic/StructAbstract.hpp>
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
  struct StringPartition : public StructAbstract {
    // //////////////// Type definitions //////////////////
    /**
     * Type gathering all the partitions of a string.
     */
    typedef std::list<StringSet> StringPartition_T;
  
  public:
    /**
     * Add an item (StringSet) into the list.
     *
     * \note When the given string set is empty (zero-length),
     *       it is (obviously) not added to the list
     */
    void push_back (const StringSet& iStringSet);
  
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
     * Get the initial string, that is, the string having been given
     * to be partitioned.
     */
    const std::string& getInitialString() const {
      return _initialString;
    }

    /**
     * Return the list of all the unique word combinations, which can be made
     * from the partitions.
     *
     * For instance, with "rio de janeiro", the list of unique strings would be:
     * <ul>
     *   <li>"rio"</li>
     *   <li>"de"</li>
     *   <li>"janeiro"</li>
     *   <li>"rio de"</li>
     *   <li>"de janeiro"</li>
     *   <li>"rio de janeiro"</li>
     * </ul>
     *
     * @return StringSet The list of unique strings.
     */
    StringSet calculateUniqueCombinations() const;


  private:
    /**
     * Main partition algorithm.
     *
     * That method is called by the main constructor. It should not be called
     * directly.
     *
     * @param const std::string& The string for which the partitions are sought
     */
    void init (const std::string& iStringToBePartitioned);


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
     * Constructor.
     *
     * @param const std::string& The string for which the partitions are sought
     */
    StringPartition (const std::string& iStringToBePartitioned);

    /**
     * Default destructor.
     */
    ~StringPartition();


  public:
    // //////////////// Attributes ///////////////
    /**
     * String to be partitioned.
     */
    std::string _initialString;

    /**
     * Partition, i.e., a list containing sub-lists of strings
     */
    StringPartition_T _partition;
  };


  // //////////////// Type definitions //////////////////
  /**
   * Type gathering all the partitions of a string.
   */
  typedef std::list<StringPartition> StringPartitionList_T;
  
}
#endif // __OPENTREP_BOM_STRINGPARTITION_HPP
