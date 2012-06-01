#ifndef __OPENTREP_BOM_WORDHOLDER_HPP
#define __OPENTREP_BOM_WORDHOLDER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// OpenTREP
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/bom/BomAbstract.hpp>

namespace OPENTREP {

  /**
   * @brief Class wrapping utility functions to transform back and forth
   * strings from and into a list of words.
   */
  class WordHolder : public BomAbstract {
    friend class FacWordHolder;
  public:

    // /////////////// Business Methods ////////////////
    /**
     * Tokenise a string into a list of words (STL strings).
     *
     * The Boost.Tokenizer library is used.
     */
    static void tokeniseStringIntoWordList (const TravelQuery_T&, WordList_T&);

    /**
     * Tokenise a Xapian document data into a list of words (STL strings).
     *
     * The Boost.Tokenizer library is used.
     */
    static void tokeniseDocIntoWordList (const TravelQuery_T&, WordList_T&);

    /**
     * Serialise a list of words (STL strings) into a single (STL) string.
     */
    static std::string createStringFromWordList (const WordList_T& iWordList);
    
  private:
    // ////////////// Constructors and Destructors /////////////
    /**
     * Default constructor.
     */
    WordHolder();
    /**
     * Default copy constructor.
     */
    WordHolder (const WordHolder&);
    /**
     * Destructor.
     */
    ~WordHolder();
    
  private:
    // /////////////// Attributes ////////////////
  };

}
#endif // __OPENTREP_BOM_WORDHOLDER_HPP
