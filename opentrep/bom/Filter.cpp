// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// OpenTrep
#include <opentrep/basic/BasConst_General.hpp>
#include <opentrep/basic/Utilities.hpp>
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


  /**
   * Helper function to check whether the word should be kept.
   *
   * If the word has no more than <iMinWordLength> letters (e.g., 'de', 'san'),
   * it should be filtered out. Indeed, when 'san' is part of 'san francisco',
   * for instance, it should not be indexed/searched alone (in a search,
   * the resulting match score will be zero).
   */
  // //////////////////////////////////////////////////////////////////////
  bool hasGoodSize (const std::string& iWord, const NbOfLetters_T& iMinWordLength) {
    bool hasGoodSizeFlag = true;
    // 
    const size_t lWordLength = iWord.size();
    if (lWordLength < iMinWordLength) {
      hasGoodSizeFlag = false;
    }
    return hasGoodSizeFlag;
  }

  /**
   * Helper function to check whether the given word is black-listed.
   */
  // //////////////////////////////////////////////////////////////////////
  bool isBlackListed (const std::string& iWord) {
    // When the word is part of the "black list", it should obviously be
    // filtered out.
    BlackList_T::const_iterator itWord = K_BLACK_LIST.find (iWord);
    const bool isBlackListedFlag = (itWord != K_BLACK_LIST.end());

    // DEBUG
    // const std::string areEqualStr = (isBlackListedFlag)?"Yes":"No";
    // const std::string& lWord = *itWord;
    // OPENTREP_LOG_DEBUG ("Word: '" << iWord << "', black-list word: '"
    //                     << lWord << "', Equals: " << areEqualStr);

    return isBlackListedFlag;
  }

  /**
   * Helper function to trim the non-relevant right outer words.
   */
  // //////////////////////////////////////////////////////////////////////
  void rtrim (WordList_T& ioWordList, const NbOfLetters_T& iMinWordLength) {
    // If the list is empty, obviously nothing can be done at that stage.
    if (ioWordList.empty() == true) {
      return;
    }

    // Take the first right outer word
    WordList_T::reverse_iterator itWord = ioWordList.rbegin();
    assert (itWord != ioWordList.rend());
    const std::string& lWord = *itWord;

    // Check whether that word has the good size (>= iMinWordLength) and whether it is
    // black-listed.
    const bool hasGoodSizeFlag = hasGoodSize (lWord, iMinWordLength);
    const bool isBlackListedFlag = isBlackListed (lWord);
    if (hasGoodSizeFlag == false || isBlackListedFlag == true) {
      ioWordList.erase (--itWord.base());
      rtrim (ioWordList, iMinWordLength);
    }
  }

  /**
   * Helper function to trim the non-relevant left outer words.
   */
  // //////////////////////////////////////////////////////////////////////
  void ltrim (WordList_T& ioWordList, const NbOfLetters_T& iMinWordLength) {
    // If the list is empty, obviously nothing can be done at that stage.
    if (ioWordList.empty() == true) {
      return;
    }

    // Take the first left outer word
    WordList_T::iterator itWord = ioWordList.begin();
    assert (itWord != ioWordList.end());
    const std::string& lWord = *itWord;

    // Check whether that word has the good size (>= iMinWordLength) and whether it is
    // black-listed.
    const bool hasGoodSizeFlag = hasGoodSize (lWord, iMinWordLength);
    const bool isBlackListedFlag = isBlackListed (lWord);
    if (hasGoodSizeFlag == false || isBlackListedFlag == true) {
      ioWordList.erase (itWord);
      ltrim (ioWordList, iMinWordLength);
    }
  }

  /**
   * Helper function to trim the non-relevant left and right outer words.
   */
  // //////////////////////////////////////////////////////////////////////
  void trim (WordList_T& ioWordList, const NbOfLetters_T& iMinWordLength) {
    // Trim the non-relevant left outer words
    ltrim (ioWordList, iMinWordLength);

    // Trim the non-relevant right outer words
    rtrim (ioWordList, iMinWordLength);
  }

  // //////////////////////////////////////////////////////////////////////
  void Filter::trim (std::string& ioPhrase, const NbOfLetters_T& iMinWordLength) {
    // Create a list of words from the given phrase
    WordList_T lWordList;
    tokeniseStringIntoWordList (ioPhrase, lWordList);

    // Trim the non-relevant left and right outer words
    OPENTREP::trim (lWordList, iMinWordLength);

    // Re-create the phrase from the (potentially altered) list of words
    ioPhrase = createStringFromWordList (lWordList);
  }

  // //////////////////////////////////////////////////////////////////////
  bool Filter::shouldKeep (const std::string& iPhrase,
                           const std::string& iWord) {
    bool isToBeKept = true;

    // If both the phrase and the word are empty, the word should obviously
    // be filtered out.
    if (iPhrase.empty() == true && iWord.empty() == true) {
      isToBeKept = false;
      return isToBeKept;
    }

    // If the term to be added is equal to the whole phrase (e.g., 'san'),
    // it should be kept (not filtered out). Indeed, three-letter words
    // often correspond to IATA codes, and should obviously be kept for
    // indexation/searching.
    if (iPhrase == iWord) {
      return isToBeKept;
    }

    // Now, the word is part of the phrase, and not equal to it (and not empty).

    // If the word has no more than two letters (e.g., 'de'), it should be
    // filtered out. Indeed, when 'de' is part of 'charles de gaulle',
    // for instance, it should not be indexed/searched alone (in a search,
    // the resulting match score will be zero).
    isToBeKept = hasGoodSize (iWord, 3);
    if (isToBeKept == false) {
      return isToBeKept;
    }

    // Check whether the word is black-listed
    isToBeKept = !isBlackListed (iWord);

    //
    return isToBeKept;
  }

}
