#ifndef __OPENTREP_BOM_UTILITIES_HPP
#define __OPENTREP_BOM_UTILITIES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <vector>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>

namespace OPENTREP {
  
  /**
   * Split a string into a list of tokens.
   */
  void tokeniseStringIntoWordList (const std::string& iPhrase, WordList_T&);

  /**
   * Create a string from a list of words.
   *
   * The string is made of the words up until a given index. The default
   * value of that index (-1) means that the string will be made by all
   * the words of the list.
   *
   * If the flag is set to false, the string is made of the words from
   * the given index up until the end of the list.
   */
  std::string createStringFromWordList (const WordList_T&,
                                        const unsigned short iSplitIdx = 0,
                                        const bool iFromBeginningFlag = true);

}
#endif // __OPENTREP_BOM_UTILITIES_HPP
