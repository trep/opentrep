// /////////////////////////////////////////////////////////////////////////
//
// String partition algorithm
//
// Author: Denis Arnaud
// Date:   April 2012
//
// /////////////////////////////////////////////////////////////////////////
// STL
#include <sstream>
#include <fstream>
#include <string>
#include <list>
// Boost (Extended STL)
#include <boost/tokenizer.hpp>
// Boost Unit Test Framework (UTF)
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE PartitionTestSuite
#include <boost/test/unit_test.hpp>

namespace boost_utf = boost::unit_test;

// (Boost) Unit Test XML Report
std::ofstream utfReportStream ("PartitionTestSuite_utfresults.xml");

/**
 * Configuration for the Boost Unit Test Framework (UTF)
 */
struct UnitTestConfig {
  /** Constructor. */
  UnitTestConfig() {
    boost_utf::unit_test_log.set_stream (utfReportStream);
    boost_utf::unit_test_log.set_format (boost_utf::XML);
    boost_utf::unit_test_log.set_threshold_level (boost_utf::log_test_units);
    //boost_utf::unit_test_log.set_threshold_level (boost_utf::log_successful_tests);
  }

  /** Destructor. */
  ~UnitTestConfig() {
  }
};


// /////////////////// Type definitions //////////////////
// List of words
typedef std::vector<std::string> WordList_T;
  
// //////////////////////////////////////////////////////////////////////
void tokeniseStringIntoWordList (const std::string& iPhrase,
                                 WordList_T& ioWordList) {
  // Empty the word list
  ioWordList.clear();
  
  // Boost Tokeniser
  typedef boost::tokenizer<boost::char_separator<char> > Tokeniser_T;
  
  // Define the separators
  const boost::char_separator<char> lSepatorList(" .,;:|+-*/_=!@#$%`~^&(){}[]?'<>\"");
  
  // Initialise the phrase to be tokenised
  Tokeniser_T lTokens (iPhrase, lSepatorList);
  for (Tokeniser_T::const_iterator tok_iter = lTokens.begin();
       tok_iter != lTokens.end(); ++tok_iter) {
    const std::string& lTerm = *tok_iter;
    ioWordList.push_back (lTerm);
  }
}

/**
 * Create a string from a list of words.
 *
 * The string is made of the words up until a given index. The default value
 * of that index (-1) means that the string will be made by all the words of
 * the list.
 *
 * If the flag is set to false, the string is made of the words from the given
 * index up until the end of the list.
 */
std::string createStringFromWordList (const WordList_T& iWordList,
                                      const unsigned short iSplitIdx = 0,
                                      const bool iFromBeginningFlag = true) {
  std::ostringstream oStr;

  // Browse the lef-hand side of the string
  unsigned short idx = 0;
  WordList_T::const_iterator itWord = iWordList.begin();
  for ( ; itWord != iWordList.end(); ++itWord, ++idx) {

    if (iFromBeginningFlag == true) {
      // The beginning of the word list is needed

      // Check whether the sub-list has reached the expected split point, if any
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

      // Check whether the sub-list has reached the expected split point, if any
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



/**
 * Class holding a set of strings, e.g., {"rio", "de", "janeiro"}
 */
struct StringSet {
  // //////////////// Type definitions //////////////////
  typedef std::list<std::string> StringSet_T;

  /**
   * Constructor.
   */
  StringSet (const std::string& iString) {
    _set.push_back (iString);
  }

  /**
   * Display
   */
  void display() const {
    //
    std::cout << "  {";

    short idx_string = 0;
    for (StringSet_T::const_iterator itString = _set.begin();
         itString != _set.end(); ++itString, ++idx_string) {
      //
      if (idx_string != 0) {
        std::cout << ", ";
      }

      //
      const std::string& lString = *itString;

      //
      std::cout << "\"" << lString << "\"";
    }

    //
    std::cout << "}";
  }

  /**
   * Add an item (string) into the list.
   */
  void push_back (const std::string& iString) {
    _set.push_back (iString);
  }
  
  /**
   * Add all the items (string) into the list.
   */
  void push_back (const StringSet& iStringSet) {
    const StringSet_T& lStringSet = iStringSet._set;
    for (StringSet_T::const_iterator itString = lStringSet.begin();
         itString != lStringSet.end(); ++itString) {
      const std::string& lString = *itString;
      push_back (lString);
    }
  }
  
  /**
   * Return the size of the list.
   */
  size_t size() const {
    return _set.size();
  }
  
  /**
   * Empty the list.
   */
  void clear() {
    _set.clear();
  }
  
  // //////////////// Attributes ///////////////
  /**
   * String set, i.e., a list of strings
   */
  StringSet_T _set;
};

/**
 * Class holding a string partition.
 *
 * A string partition contains all the ways to combine, serially,
 * all the words of a given string.
 *
 * For instance, "rio de janeiro" will give:
 * <ul>
 *   <li>{</li>
 *   <li><ul>
 *     <li>{"rio", "de", "janeiro"}</li>
 *     <li>{"rio", "de janeiro"}</li>
 *     <li>{"rio de", "janeiro"}</li>
 *     <li>{"rio de janeiro"}</li>
 *   </ul></li>
 *   <li>}</li>
 * </ul>
 */
struct StringPartition {
  // //////////////// Type definitions //////////////////
  typedef std::list<StringSet> StringPartition_T;
  
  /**
   * Display
   */
  void display() const {
    //
    std::cout << "{" << std::endl;

    short idx_sublist = 0;
    for (StringPartition_T::const_iterator itSet = _partition.begin();
         itSet != _partition.end(); ++itSet, ++idx_sublist) {
      //
      if (idx_sublist != 0) {
        std::cout << "," << std::endl;
      }
      
      //
      const StringSet& lStringSet = *itSet;

      //
      lStringSet.display();
    }

    //
    std::cout << std::endl << "}" << std::endl;
  }

  /**
   * Add an item (StringSet) into the list.
   */
  void push_back (const StringSet& iStringSet) {
    _partition.push_back (iStringSet);
  }
  
  /**
   * Return the size of the list.
   */
  size_t size() const {
    return _partition.size();
  }
  
  /**
   * Empty the list.
   */
  void clear() {
    _partition.clear();
  }
  
  /**
   * Main partition algorithm.
   */
  StringPartition_T& init (const std::string& iString) {
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
      return _partition;
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
      StringPartition lStringPartition;
      lStringPartition.init (lRightHandString);

      // 1.3.2. Extract the sub-lists of strings, and add them to the current
      //        sub-lists.
      const StringPartition_T& lStringRHSPartition =
        lStringPartition._partition;
      for (StringPartition_T::const_iterator itSet =
             lStringRHSPartition.begin();
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

    // 2.2. Return the partition, i.e., the list of sub-lists of strings
    return _partition;
  }

  // //////////////// Attributes ///////////////
  /**
   * Partition, i.e., a list containing sub-lists of strings
   */
  StringPartition_T _partition;
};


// /////////////// Main: Unit Test Suite //////////////

// Set the UTF configuration (re-direct the output to a specific file)
BOOST_GLOBAL_FIXTURE (UnitTestConfig);

// Start the test suite
BOOST_AUTO_TEST_SUITE (master_test_suite)

/**
 * Test the partitioning algorithm with a small string
 */
BOOST_AUTO_TEST_CASE (partition_small_string) {

  const std::string lLax1Str = "los angeles";
  const std::string lLax2Str = "lso angeles";
  const std::string lRio1Str = "rio de janeiro";
  const std::string lRio2Str = "rio de janero";
  const std::string lRek1Str = "reikjavik";
  const std::string lRek2Str = "rekyavik";
  const std::string lSfoRio1Str = "san francisco rio de janeiro";
  const std::string lSfoRio2Str = "san francicso rio de janero";
  const std::string lSfoRio3Str = "sna francicso rio de janero";

  //
  StringPartition lStringPartition;
  lStringPartition.init (lSfoRio3Str);
  lStringPartition.display();

  BOOST_CHECK_MESSAGE (lStringPartition.size() == 16,
                       "The partition, for '" << lRio2Str
                       << "', should contain a single list."
                       << " However, its size is " << lStringPartition.size()
                       << ".");
}

// End the test suite
BOOST_AUTO_TEST_SUITE_END()

