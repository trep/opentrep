// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// Boost (Extended STL)
#include <boost/tokenizer.hpp>
// OpenTrep
#include <opentrep/basic/Utilities.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  void tokeniseStringIntoWordList (const std::string& iPhrase,
                                   WordList_T& ioWordList) {
    // Empty the word list
    ioWordList.clear();
  
    // Boost Tokeniser
    typedef boost::tokenizer<boost::char_separator<char> > Tokeniser_T;
  
    // Define the single-character separators.
    // Note that multi-byte Unicode characters (e.g., “, ”)
    // should not be inserted here
    const boost::char_separator<char>
      lSepatorList(" .,;:|+-*/_=!@#$%`~^&(){}[]?'<>\"");
  
    // Initialise the phrase to be tokenised
    Tokeniser_T lTokens (iPhrase, lSepatorList);
    for (Tokeniser_T::const_iterator tok_iter = lTokens.begin();
         tok_iter != lTokens.end(); ++tok_iter) {
      const std::string& lTerm = *tok_iter;
      ioWordList.push_back (lTerm);
    }
  }

  // //////////////////////////////////////////////////////////////////////
  std::string createStringFromWordList (const WordList_T& iWordList,
                                        const unsigned short iSplitIdx,
                                        const bool iFromBeginningFlag) {
    std::ostringstream oStr;

    // Browse the left-hand side of the string
    unsigned short idx = 0;
    WordList_T::const_iterator itWord = iWordList.begin();
    for ( ; itWord != iWordList.end(); ++itWord, ++idx) {

      if (iFromBeginningFlag == true) {
        // The beginning of the word list is needed

        // Check whether the sub-list has reached the expected split point,
        // if any
        if (iSplitIdx != 0 && idx >= iSplitIdx) {
          break;
        }
      
        //
        if (idx > 0) {
          oStr << " ";
        }
        //
        const std::string& lWord = *itWord;
        oStr << lWord;

      } else {
        // The end of the word list is needed
        
        // Check whether the sub-list has reached the expected split point,
        // if any
        if (iSplitIdx == 0 || idx >= iSplitIdx) {
          break;
        }
      }
    }

    // The beginning of the word list is needed
    if (iFromBeginningFlag == true) {
      return oStr.str();
    }

    // The end of the word list is needed
    assert (iFromBeginningFlag == false);

    // Browse the right-hand side of the string
    for ( ; itWord != iWordList.end(); ++itWord, ++idx) {
      // The end of the word list is needed

      //
      if (idx > iSplitIdx) {
        oStr << " ";
      }
      //
      const std::string& lWord = *itWord;
      oStr << lWord;
    }
  
    return oStr.str();
  }

}
