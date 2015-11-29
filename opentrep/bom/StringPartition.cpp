// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
#include <set>
// OpenTrep
#include <opentrep/basic/BasConst_General.hpp>
#include <opentrep/basic/Utilities.hpp>
#include <opentrep/bom/Filter.hpp>
#include <opentrep/bom/StringPartition.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  StringPartition::StringPartition (const std::string& iString)
    : _initialString (iString) {
    init (iString);
  }

  // //////////////////////////////////////////////////////////////////////
  StringPartition::~StringPartition() {
  }

  // //////////////////////////////////////////////////////////////////////
  void StringPartition::push_back (const StringSet& iStringSet) {
    if (iStringSet.empty() == false) {
      _partition.push_back (iStringSet);
    }
  }
  
  // //////////////////////////////////////////////////////////////////////
  size_t StringPartition::size() const {
    return _partition.size();
  }
  
  // //////////////////////////////////////////////////////////////////////
  bool StringPartition::empty() const {
    return _partition.empty();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void StringPartition::clear() {
    _partition.clear();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string StringPartition::describeKey() const {
    std::ostringstream oStr;
    oStr << "";
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string StringPartition::describe() const {
    std::ostringstream oStr;
    oStr << describeKey();

    //
    oStr << "{";

    short idx_sublist = 0;
    for (StringPartition_T::const_iterator itSet = _partition.begin();
         itSet != _partition.end(); ++itSet, ++idx_sublist) {
      //
      if (idx_sublist != 0) {
        oStr << ", ";
      }
      
      //
      const StringSet& lStringSet = *itSet;

      //
      oStr << lStringSet;
    }

    //
    oStr << " }";

    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  void StringPartition::toStream (std::ostream& ioOut) const {
    ioOut << describe();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void StringPartition::fromStream (std::istream& ioIn) {
  }

  // //////////////////////////////////////////////////////////////////////
  void StringPartition::init (const std::string& iPhrase) {
    /**
     * 0. Initialisation
     * 0.1. Initialisation of the tokenizer
     */
    WordList_T lWordList;
    tokeniseStringIntoWordList (iPhrase, lWordList);
    NbOfWords_T nbOfWords = lWordList.size();

    /**
     * 0.2. Re-create the initial phrase:
     *   <ul>
     *     <li>Without any (potential) seperator</li>
     *     <li>Capping the number of words. Indeed, the number of string
     *         partitions increases exponentially with the number of words
     *         within the string. With the name of Bangkok (BKK), for instance,
     *         the number of string partitions would so big as to crash the
     *         indexing process, even on big servers.<br>
     *         See basic/BasCont.cpp for the value of
     *         K_DEFAULT_MAXIMUM_NUMBER_OF_WORDS_IN_STRING</li>
     *   </ul>
     */
    const std::string lPhrase =
      createStringFromWordList (lWordList,
                                K_DEFAULT_MAXIMUM_NUMBER_OF_WORDS_IN_STRING);
    
    /**
     * When the initial string has more words than the maximum allowed (see
     * above), then it is strip down.
     */
    if (nbOfWords > K_DEFAULT_MAXIMUM_NUMBER_OF_WORDS_IN_STRING) {
      // DEBUG
      OPENTREP_LOG_DEBUG ("The initial string ('" << iPhrase << "') has got "
                          << nbOfWords << " words, and will be strip down to "
                          << K_DEFAULT_MAXIMUM_NUMBER_OF_WORDS_IN_STRING
                          << " words, giving: ':" << lPhrase << "'");

      // Rebuild the list of words
      tokeniseStringIntoWordList (lPhrase, lWordList);

      // Recalculate the number of words
      nbOfWords = lWordList.size();

      // The number of wors must be, by construction, the maximum number of words
      assert (nbOfWords == K_DEFAULT_MAXIMUM_NUMBER_OF_WORDS_IN_STRING);
    }

    /**
     * 0.3. Create the list with a single sub-list, itself containing only
     *      the given input string.
     */
    StringSet oStringSet (lPhrase);
      
    /**
     * 0.4. If the string contains no more than one word, the job is finished.
     */
    if (nbOfWords <= 1) {
      _partition.push_back (oStringSet);
      return;
    }

    /**
     * 1. Iteration on all the words of the given string, from 1 to nbOfWords-1
     */
    for (NbOfWords_T idx_word = 1; idx_word != nbOfWords; ++idx_word) {
      // 1.1. Create a sub-string copy of the first idx_word
      const std::string& lLeftHandString = createStringFromWordList (lWordList,
                                                                     idx_word);

      // DEBUG
      /*
      OPENTREP_LOG_DEBUG ("[" << lPhrase << ", " << idx_word
                          << "] Left-hand string: '" << lLeftHandString << "'");
      */
        
      /**
       * 1.2. Create another sub-string with the remaining of the string
       */
      const std::string& lRightHandString = createStringFromWordList (lWordList,
                                                                      idx_word,
                                                                      false);

      // DEBUG
      // std::cout << "[" << lPhrase << ", " << idx_word
      //           << "] Right-hand string: '" << lRightHandString << "'"
      //           << std::endl;
        
      /**
       * 1.3. Recurse
       * 1.3.1. Call the Partition algorithm on the right-hand side string
       */
      StringPartition lStringPartition (lRightHandString);

      /**
       * 1.3.2. Extract the sub-lists of strings, and add them to the current
       *        sub-lists.
       */
      const StringPartition_T& lStringRHSPartition= lStringPartition._partition;
      for (StringPartition_T::const_iterator itSet= lStringRHSPartition.begin();
           itSet != lStringRHSPartition.end(); ++itSet) {
        const StringSet& lRHSStringSet = *itSet;
          
        /**
         * Create the sub-list which will accommodate:
         *  - The left-hand-side sub-string.
         *  - The sub-list having been generated by the recursion on the
         *    the right-hand-side sub-string.
         */
        StringSet lNewStringSet;

        // Add the string to the list
        lNewStringSet.push_back (lLeftHandString);

        //
        lNewStringSet.push_back (lRHSStringSet);

        //
        _partition.push_back (lNewStringSet);
      }
    }
    

    /**
     * 2.
     * 2.1. Add the sub-list with the full string (the one given as input) to
     * the back of the list.
     */
    _partition.push_back (oStringSet);
  }

  // //////////////////////////////////////////////////////////////////////
  StringSet StringPartition::calculateUniqueCombinations() const {
    StringSet oStringSet;

    // Set of unique strings
    WordSet_T lStringList;

    // Browse all the word combinations. Then, for every word combination,
    // add it if not already in the list (STD set) of strings.
    for (StringPartition_T::const_iterator itSet = _partition.begin();
         itSet != _partition.end(); ++itSet) {
      const StringSet& itStringSet = *itSet;
      
      const StringSet::StringSet_T& lStringSet = itStringSet._set;
      for (StringSet::StringSet_T::const_iterator itString = lStringSet.begin();
           itString != lStringSet.end(); ++itString) {
        const std::string& lWordCombination = *itString;

        // Check whether that word combination has already been stored once.
        WordSet_T::const_iterator itExistingString =
          lStringList.find (lWordCombination);
        if (itExistingString == lStringList.end()) {
          // If not, add it to the dedicated list (STD set).
          lStringList.insert (lWordCombination);
        }
      }
    }    

    // Convert the STD set into a StringSet structure
    for (WordSet_T::const_iterator itString = lStringList.begin();
         itString != lStringList.end(); ++itString) {
      const std::string& lWordCombination = *itString;
      oStringSet.push_back (lWordCombination);
    }

    //
    return oStringSet;
  }

}
