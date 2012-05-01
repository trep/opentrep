// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// OpenTREP
#include <opentrep/basic/BasConst_General.hpp>
#include <opentrep/bom/Filter.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  Filter::Filter() {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  Filter::Filter (const Filter& iFilter) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  Filter::~Filter() {
  }

  // //////////////////////////////////////////////////////////////////////
  bool Filter::shouldKeep (const std::string& iPhrase,
                           const std::string& iWord) {
    bool isToBeKept  = true;

    // If the term to be added is equal to the whole phrase (e.g., 'san'),
    // it should be kept (not filtered out). Indeed, three-letter words
    // often correspond to IATA codes, and should obviously be kept for
    // indexation/searching.
    if (iPhrase == iWord) {
      return isToBeKept;
    }

    // Now, the word is part of the phrase, and not equal to it.

    // If the word has no more than 3 letters (e.g., 'de', 'san'),
    // it should be filtered out. Indeed, when 'san' is part of
    // 'san francisco', for instance, it should not be indexed/searched
    // alone (in a search, the resulting match score will be zero).
    const size_t lWordLength = iWord.size();
    if (lWordLength <= 3) {
      isToBeKept = false;
      return isToBeKept;
    }

    // When the word is part of the "black list", it should obviously be
    // filtered out.
    for (BlackList_T::const_iterator itWord = K_BLACK_LIST.begin();
         itWord != K_BLACK_LIST.end(); ++itWord) {
      const std::string& lWord = *itWord;

      // DEBUG
      // const std::string areEqualStr = (iWord == lWord)?"Yes":"No";
      // OPENTREP_LOG_DEBUG ("Phrase: '" << iPhrase << "', Word: '" << iWord
      //                     << "', black-list word: '" << lWord
      //                     << "', Equals: " << areEqualStr);
      
      if (iWord == lWord) {
        isToBeKept = false;
        break;
      }
    }    

    //
    return isToBeKept;
  }

}
