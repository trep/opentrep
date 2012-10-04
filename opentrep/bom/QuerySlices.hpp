#ifndef __OPENTREP_BOM_QUERYSLICES_HPP
#define __OPENTREP_BOM_QUERYSLICES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <list>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/basic/StructAbstract.hpp>
#include <opentrep/bom/StringPartition.hpp>

namespace OPENTREP {

  /**
   * Class allowing to slice a query string into multiple slices.
   * Each of those slices will then give birth to the corresponding
   * string partitions.
   *
   * The initial query string is sliced in the interstices, which split apart
   * any two consecutive words yielding no full text match.
   *
   * For instance, "san francisco nce rio de janeiro" will give:
   * <ul>
   *   <li>{</li>
   *   <li><ul>
   *     <li>{"san francisco"}</li>
   *     <li>{"nce"}</li>
   *     <li>{"rio de janeiro"}</li>
   *   </ul></li>
   *   <li>}</li>
   * </ul>
   *
   * Indeed, "francisco nce" does not yield any match; hence, the query may
   * be split right after the "francisco" word, i.e., separating the
   * "san francisco" sub-query from the "nce rio de janeiro" one.
   * The same way, "nce rio" does not yield any match; hence, the remaining
   * query may be split into two sub-queries: "nce" and "rio de janeiro".
   */
  struct QuerySlices : public StructAbstract {
    // //////////////// Type definitions //////////////////
    /**
     * Type gathering all the partitions of a string.
     */
    typedef std::list<StringPartition> QuerySlices_T;
  
  public:
    /**
     * Add an item (StringPartition) into the list.
     *
     * \note When the given string set is empty (zero-length),
     *       it is (obviously) not added to the list
     */
    void push_back (const StringPartition& iStringPartition);
  
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


  private:
    /**
     * Main partition algorithm.
     *
     * That method is called by the main constructor. It should not be called
     * directly.
     * @param const std::string& The string for which the partitions are sought
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
     */
    QuerySlices (const std::string& iString);

    /**
     * Default destructor.
     */
    ~QuerySlices();


  public:
    // //////////////// Attributes ///////////////
    /**
     * Partition, i.e., a list containing sub-lists of strings
     */
    QuerySlices_T _slices;

    /**
     * Staging string holding the left part of the query
     */
    std::string _itLeftWords;
  };

}
#endif // __OPENTREP_BOM_QUERYSLICES_HPP
