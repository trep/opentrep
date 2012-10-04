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
#include <opentrep/bom/QuerySlices.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  QuerySlices::QuerySlices (const std::string& iString) {
    init (iString);
  }

  // //////////////////////////////////////////////////////////////////////
  QuerySlices::~QuerySlices () {
  }

  // //////////////////////////////////////////////////////////////////////
  void QuerySlices::push_back (const StringPartition& iStringPartition) {
    if (iStringPartition.empty() == false) {
      _slices.push_back (iStringPartition);
    }
  }
  
  // //////////////////////////////////////////////////////////////////////
  size_t QuerySlices::size() const {
    return _slices.size();
  }
  
  // //////////////////////////////////////////////////////////////////////
  bool QuerySlices::empty() const {
    return _slices.empty();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void QuerySlices::clear() {
    _slices.clear();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string QuerySlices::describeKey() const {
    std::ostringstream oStr;
    oStr << "";
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string QuerySlices::describe() const {
    std::ostringstream oStr;
    oStr << describeKey();

    //
    oStr << "[ ";

    short idx_sublist = 0;
    for (QuerySlices_T::const_iterator itSlice = _slices.begin();
         itSlice != _slices.end(); ++itSlice, ++idx_sublist) {
      //
      if (idx_sublist != 0) {
        oStr << "; ";
      }
      
      //
      const StringPartition& lStringPartition = *itSlice;

      //
      oStr << lStringPartition;
    }

    //
    oStr << " ]";

    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  void QuerySlices::toStream (std::ostream& ioOut) const {
    ioOut << describe();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void QuerySlices::fromStream (std::istream& ioIn) {
  }

  /**
   * Helper function to query for a Xapian-based full text match
   */
  // //////////////////////////////////////////////////////////////////////
  bool doesMatch (const std::string& iWord1, const std::string& iWord2) {
    bool oDoesMatch = true;

    // TODO: implement that method based on Xapian matching (see Result.cpp)
    //

    return oDoesMatch;
  }

  // //////////////////////////////////////////////////////////////////////
  void QuerySlices::init (const std::string& iPhrase) {
    // 0. Initialisation
    // 0.1. Initialisation of the tokenizer
    WordList_T lWordList;
    tokeniseStringIntoWordList (iPhrase, lWordList);
    const unsigned short nbOfWords = lWordList.size();

    // When the query has a single word, stop here, as there is a single slice
    if (nbOfWords <= 1) {
      _slices.push_back (iPhrase);
      return;
    }

    // 0.2. Re-create the initial phrase, without any (potential) seperator
    const std::string lPhrase = createStringFromWordList (lWordList);

    // 1. Browse the words, two by two, and check whether their association
    //    matches with the Xapian index
    WordList_T::const_iterator itWord = lWordList.begin();
    WordList_T::const_iterator itNextWord = lWordList.begin();
    for (unsigned short idx = 1;
         itNextWord != lWordList.end(); ++itWord, ++itNextWord, ++idx) {
      const std::string& leftWord = *itWord;
      const std::string& rightWord = *itNextWord;
      // Check whether the juxtaposition of the two contiguous words matches
      const bool lDoesMatch = OPENTREP::doesMatch (leftWord, rightWord);

      if (lDoesMatch == true) {
        // When the two words give a match, add the left word to
        // the staging list
        _itLeftWords += leftWord;

      } else {
        // When the two words give no match, add the content of the staging list
        // to the list of slices. Then, empty the staging list.
        _slices.push_back (_itLeftWords);
        _itLeftWords = "";
      }
    }
    
    // 2.
    _slices.push_back (lPhrase);
  }

}
