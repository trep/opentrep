#ifndef __OPENTREP_BOM_QUERYSLICES_HPP
#define __OPENTREP_BOM_QUERYSLICES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <list>
// Xapian
#include <xapian.h>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/basic/StructAbstract.hpp>
#include <opentrep/bom/StringPartition.hpp>

namespace OPENTREP {

  // Forward declarations
  class OTransliterator;

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
   *   <li>[</li>
   *   <li><ul>
   *     <li>0. { {"san francisco"} }</li>
   *     <li>1. { {"nce"} }</li>
   *     <li>2. { {"rio de janeiro"} }</li>
   *   </ul></li>
   *   <li>]</li>
   * </ul>
   *
   * Indeed, "francisco nce" does not yield any match; hence, the query may
   * be split right after the "francisco" word, i.e., separating the
   * "san francisco" sub-query from the "nce rio de janeiro" one.
   * The same way, "nce rio" does not yield any match; hence, the remaining
   * query may be split into two sub-queries: "nce" and "rio de janeiro".
   */
  struct QuerySlices : public StructAbstract {
  public:
    /**
     * Get the underlying query string.
     *
     * Note that the query string has been normalised, and has therefore
     * potentially been altered.
     */
    const TravelQuery_T& getQueryString() const {
      return _queryString;
    }

    /**
     * Get the underlying list of string partitions.
     */
    const StringPartitionList_T& getStringPartitionList() const {
      return _slices;
    }

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
     *
     * @param const OTransliterator& Unicode transliterator
     */
    void init (const OTransliterator&);


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
     * Main constructor.
     *
     * @param const Xapian::Database& Xapian database (index)
     * @param const TravelQuery_T& The string for which the partitions are sought
     * @param const OTransliterator& Unicode transliterator
     */
    QuerySlices (const Xapian::Database&, const TravelQuery_T&,
                 const OTransliterator&);

    /**
     * Default destructor.
     */
    ~QuerySlices();


  public:
    // //////////////// Attributes ///////////////
    /**
     * Xapian database.
     */
    const Xapian::Database& _database;

    /**
     * Query string having generated the set of documents.
     */
    TravelQuery_T _queryString;

    /**
     * Partition, i.e., a list containing sub-lists of strings
     */
    StringPartitionList_T _slices;

    /**
     * Staging string holding the left part of the query
     */
    std::string _itLeftWords;
  };

}
#endif // __OPENTREP_BOM_QUERYSLICES_HPP
