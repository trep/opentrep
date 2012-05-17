#ifndef __OPENTREP_BOM_WORDCOMBINATIONHOLDER_HPP
#define __OPENTREP_BOM_WORDCOMBINATIONHOLDER_HPP

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
   * Class holding a list of some specific word combinations within a string.
   *
   * The list contains all the ways to combine all the words of a given string,
   * while respecting the initial order.
   *
   * \note The 2- and 3-letter words (such as 'de' and 'san') are usually not
   *       to be indexed by Xapian. Idem with the 'airport' word.
   *
   * A way to derive that list is simply by:
   * <ol>
   *   <li>Calculating all the partitions of the initial (full) string</li>
   *   <li>Extract from those partitions all the unique word combinations</li>
   *   <li>Add all the word combinations, obtained from removing any group
   *       of words in the middle of the initial (full) string</li>
   * </ol>
   *
   * For instance, "san francisco international airport" will give:
   * <ul>
   *   <li>Derived from the partitions of the initial string:</li>
   *   <li><ol>
   *     <li>"san francisco international airport"</li>
   *     <li>"san francisco international"</li>
   *     <li>"san francisco"</li>
   *     <li>"francisco international airport"</li>
   *     <li>"international airport"</li>
   *   </ol></li>
   *   <li>Added word combinations:</li>
   *   <li><ol>
   *     <li>"san international airport"</li>
   *     <li>"san francisco airport"</li>
   *     <li>"san airport"</li>
   *   </ol></li>
   * </ul>
   */
  struct WordCombinationHolder : public StructAbstract {
    // //////////////// Type definitions //////////////////
    /**
     * List of strings.
     */
    typedef std::list<std::string> StringList_T;
  
  public:
    /**
     * Add an item (string) into the list.
     */
    void push_back (const std::string&);
  
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
     * Main algorithm.
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
     * Constructor.
     */
    WordCombinationHolder (const std::string&);

    /**
     * Default destructor.
     */
    ~WordCombinationHolder();


  public:
    // //////////////// Attributes ///////////////
    /**
     * List of word combinations (strings).
     */
    StringList_T _list;
  };

}
#endif // __OPENTREP_BOM_WORDCOMBINATIONHOLDER_HPP
