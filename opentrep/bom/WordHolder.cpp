// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// C
#include <cassert>
// Boost
#include <boost/tokenizer.hpp>
// OpenTREP
#include <opentrep/bom/WordHolder.hpp>

namespace OPENTREP {

  // Define the separators
  static const boost::char_separator<char> AllSepatorList (" .,;:|+-*/_=!@#$%`~^&(){}[]?'<>\"");
  static const boost::char_separator<char> DocSepatorList (" ,-%");
    
  // //////////////////////////////////////////////////////////////////////
  void baseTokeniseStringIntoWordList (const std::string& iPhrase,
                                       WordList_T& ioWordList,
                                       const boost::char_separator<char>& iSepatorList) {
    // Empty the word list
    ioWordList.clear();
    
    // Boost Tokeniser
    typedef boost::tokenizer<boost::char_separator<char> > Tokeniser_T;
    
    // Initialise the phrase to be tokenised
    Tokeniser_T lTokens (iPhrase, iSepatorList);
    for (Tokeniser_T::const_iterator tok_iter = lTokens.begin();
         tok_iter != lTokens.end(); ++tok_iter) {
      const std::string& lTerm = *tok_iter;
      ioWordList.push_back (lTerm);
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void WordHolder::tokeniseStringIntoWordList (const std::string& iPhrase,
                                               WordList_T& ioWordList) {
    OPENTREP::baseTokeniseStringIntoWordList (iPhrase, ioWordList,
                                              AllSepatorList);
  }

  // //////////////////////////////////////////////////////////////////////
  void WordHolder::tokeniseDocIntoWordList (const std::string& iPhrase,
                                            WordList_T& ioWordList) {
    OPENTREP::baseTokeniseStringIntoWordList (iPhrase, ioWordList,
                                              DocSepatorList);
  }

  // //////////////////////////////////////////////////////////////////////
  std::string WordHolder::
  createStringFromWordList (const WordList_T& iWordList) {
    std::ostringstream oStr;
    
    unsigned short idx = iWordList.size();
    for (WordList_T::const_iterator itWord = iWordList.begin();
         itWord != iWordList.end(); ++itWord, --idx) {
      const std::string& lWord = *itWord;
      oStr << lWord;
      if (idx > 1) {
        oStr << " ";
      }
    }
  
    return oStr.str();
  }

}
