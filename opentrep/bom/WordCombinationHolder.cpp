// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
#include <set>
// OpenTrep
#include <opentrep/basic/Utilities.hpp>
#include <opentrep/bom/Filter.hpp>
#include <opentrep/bom/StringPartition.hpp>
#include <opentrep/bom/WordCombinationHolder.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  WordCombinationHolder::WordCombinationHolder (const std::string& iString) {
    init (iString);
  }

  // //////////////////////////////////////////////////////////////////////
  WordCombinationHolder::~WordCombinationHolder () {
  }

  // //////////////////////////////////////////////////////////////////////
  void WordCombinationHolder::push_back (const std::string& iString) {
    _list.push_back (iString);
  }
  
  // //////////////////////////////////////////////////////////////////////
  size_t WordCombinationHolder::size() const {
    return _list.size();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void WordCombinationHolder::clear() {
    _list.clear();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string WordCombinationHolder::describeKey() const {
    std::ostringstream oStr;
    oStr << "";
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string WordCombinationHolder::describe() const {
    std::ostringstream oStr;
    oStr << describeKey();

    //
    oStr << "{";

    short idx_sublist = 0;
    for (StringList_T::const_iterator itWordCombination = _list.begin();
         itWordCombination != _list.end(); ++itWordCombination, ++idx_sublist) {
      //
      if (idx_sublist != 0) {
        oStr << ", ";
      }
      
      //
      const StringSet& lStringSet = *itWordCombination;

      //
      oStr << lStringSet;
    }

    //
    oStr << " }";

    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  void WordCombinationHolder::toStream (std::ostream& ioOut) const {
    ioOut << describe();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void WordCombinationHolder::fromStream (std::istream& ioIn) {
  }

  // //////////////////////////////////////////////////////////////////////
  void WordCombinationHolder::init (const std::string& iPhrase) {
    // Set of unique strings
    typedef std::set<std::string> StringSet_T;
    StringSet_T lStringSet;

    // 1. Derive all the partitions of the initial (full) string
    const StringPartition lStringPartitionHolder (iPhrase);
    const StringPartition::StringPartition_T& lStringPartition =
      lStringPartitionHolder._partition;

    // 2.1. For every word combination, add it if not already in the
    //      list (STL set) of strings.
    for (StringPartition::StringPartition_T::const_iterator itSet =
           lStringPartition.begin(); itSet != lStringPartition.end(); ++itSet) {
      const StringSet& itStringList = *itSet;
      
      const StringList_T& lStringList = itStringList._set;
      for (StringList_T::const_iterator itWordCombination = lStringList.begin();
           itWordCombination != lStringList.end(); ++itWordCombination) {
        const std::string& lWordCombination = *itWordCombination;

        // Check whether the (remaining) word combination should be filtered out
        //const bool isToBeAdded= Filter::shouldKeep (iPhrase, lWordCombination);
        const bool isToBeAdded = true;
        if (isToBeAdded == true) {
          lStringSet.insert (lWordCombination);
        }
      }
    }    

    // 2.2. Convert the STL set into a STL list
    for (StringSet_T::const_iterator itWordCombination = lStringSet.begin();
         itWordCombination != lStringSet.end(); ++itWordCombination) {
      const std::string& lWordCombination = *itWordCombination;

      // Add that word combination in the list for indexation by Xapian.
      // Note that if that word combination is already present in the list,
      // it will not be added a second time (thanks to the STL list design).
      _list.push_back (lWordCombination);
    }

    // 3. Add the word combinations, made by removing all the possible groups
    //    of continuous words inbetween the two extreme words (from left- and
    //    right-hand sides).
    // 3.0. Initialisation of the list of words, made of all the words of the
    //      given string.
    WordList_T lWordList;
    tokeniseStringIntoWordList (iPhrase, lWordList);
    const short nbOfWords = lWordList.size();

    // 3.1. If the string contains no more than two words, the job is finished.
    if (nbOfWords <= 2) {
      return;
    }

    // 3.2. Iteration on the number of words to remove in the middle of the
    //      string, from 1 to (nbOfWords - 2)
    for (short mdl_string_len = 1; mdl_string_len != nbOfWords-1;
         ++mdl_string_len) {

      // 3.2. Iteration on all the middle words of the given string,
      //      from 1 to (nbOfWords - mdl_string_len)
      for (short idx_word=1; idx_word != nbOfWords-mdl_string_len; ++idx_word) {
        // 3.2.1. Copy the first idx_word word(s)
        const std::string& lLeftHandString =
          createStringFromWordList (lWordList, idx_word);

        // 3.2.2. Copy the last (nbOfWords - (idx_word + mdl_string_len)) words
        const std::string& lRightHandString =
          createStringFromWordList (lWordList,
                                    idx_word + mdl_string_len,
                                    false);

        // 3.2.3. Concatenate both sub-strings
        std::ostringstream lConcatenatedStr;
        lConcatenatedStr << lLeftHandString << " " << lRightHandString;
        const std::string& lConcatenatedString = lConcatenatedStr.str();

        // 3.2.4. Add the concatenated string into the list, if not filtered out
        // const bool isToBeAdded =
        //   Filter::shouldKeep (iPhrase, lConcatenatedString);
        const bool isToBeAdded = true;
        if (isToBeAdded == true) {
          _list.push_back (lConcatenatedString);
        }
      }
    }
  }

}
