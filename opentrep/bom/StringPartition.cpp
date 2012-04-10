// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// OpenTrep
#include <opentrep/bom/StringPartition.hpp>
#include <opentrep/bom/Utilities.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  StringPartition::StringPartition (const std::string& iString) {
    init (iString);
  }

  // //////////////////////////////////////////////////////////////////////
  StringPartition::~StringPartition () {
  }

  // //////////////////////////////////////////////////////////////////////
  void StringPartition::push_back (const StringSet& iStringSet) {
    _partition.push_back (iStringSet);
  }
  
  // //////////////////////////////////////////////////////////////////////
  size_t StringPartition::size() const {
    return _partition.size();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void StringPartition::clear() {
    _partition.clear();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string StringPartition::describeShortKey() const {
    std::ostringstream oStr;
    oStr << "";
    return oStr.str();
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string StringPartition::describeKey() const {
    std::ostringstream oStr;
    oStr << "";
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string StringPartition::toShortString() const {
    std::ostringstream oStr;
    oStr << describeShortKey();

    //
    oStr << "{" << std::endl;

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
      oStr << lStringSet.toShortString();
    }

    //
    oStr << std::endl << "}" << std::endl;

    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string StringPartition::toString() const {
    std::ostringstream oStr;
    oStr << describeKey();
    
    //
    oStr << "{" << std::endl;

    short idx_sublist = 0;
    for (StringPartition_T::const_iterator itSet = _partition.begin();
         itSet != _partition.end(); ++itSet, ++idx_sublist) {
      //
      if (idx_sublist != 0) {
        oStr << "," << std::endl;
      }
      
      //
      const StringSet& lStringSet = *itSet;

      //
      oStr << lStringSet.toString();
    }

    //
    oStr << std::endl << "}" << std::endl;
    
    return oStr.str();
  }   

  // //////////////////////////////////////////////////////////////////////
  void StringPartition::toStream (std::ostream& ioOut) const {
    ioOut << toString();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void StringPartition::fromStream (std::istream& ioIn) {
  }

  // //////////////////////////////////////////////////////////////////////
  void StringPartition::init (const std::string& iString) {
    // 0. Initialisation
    // 0.1. Create the list with a single sub-list, itself containing only
    //      the given input string.
    StringSet oStringSet (iString);
      
    // 0.2. Initialisation of the tokenizer
    WordList_T lWordList;
    tokeniseStringIntoWordList (iString, lWordList);
    const short nbOfWords = lWordList.size();

    // 0.3. If the string contains no more than one word, the job is finished.
    if (nbOfWords <= 1) {
      _partition.push_back (oStringSet);
      return;
    }

    // 1. Iteration on all the words of the given string, from 1 to nbOfWords-1
    for (short idx_word = 1; idx_word != nbOfWords; ++idx_word) {
      // 1.1. Create a sub-string copy of the first idx_word
      const std::string& lLeftHandString = createStringFromWordList (lWordList,
                                                                     idx_word);

      // DEBUG
      // std::cout << "[" << iString << ", " << idx_word
      //          << "] Left-hand string: '" << lLeftHandString << "'"
      //          << std::endl;
        
      // 1.2. Create another sub-string with the remaining of the string
      const std::string& lRightHandString = createStringFromWordList (lWordList,
                                                                      idx_word,
                                                                      false);

      // DEBUG
      // std::cout << "[" << iString << ", " << idx_word
      //           << "] Right-hand string: '" << lRightHandString << "'"
      //           << std::endl;
        
      // 1.3. Recurse
      // 1.3.1. Call the Partition algorithm on the right-hand side string
      StringPartition lStringPartition (lRightHandString);

      // 1.3.2. Extract the sub-lists of strings, and add them to the current
      //        sub-lists.
      const StringPartition_T& lStringRHSPartition= lStringPartition._partition;
      for (StringPartition_T::const_iterator itSet= lStringRHSPartition.begin();
           itSet != lStringRHSPartition.end(); ++itSet) {
        const StringSet& lRHSStringSet = *itSet;
          
        // Create the sub-list which will be accommodate:
        //  - The left-hand-side sub-string.
        //  - The the sub-list having been generated by the recursion on the
        //    the right-hand-side sub-string.
        StringSet lNewStringSet (lLeftHandString);
        lNewStringSet.push_back (lRHSStringSet);

        //
        _partition.push_back (lNewStringSet);
      }
    }
    

    // 2.
    // 2.1. Add the sub-list with the full string (the one given as input) to
    // the back of the list.
    _partition.push_back (oStringSet);
  }

}
