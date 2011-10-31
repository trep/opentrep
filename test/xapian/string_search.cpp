// C
#include <cassert>
// STL
#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <map>
// Boost
#include <boost/tokenizer.hpp>
// Xapian
#include <xapian.h>

// /////////////////////// Type definitions /////////////////////////////
/** List of simple words (STL strings). */
typedef std::list<std::string> WordList_T;

/** List of Xapian documents. */
typedef std::multimap<Xapian::percent, Xapian::Document> DocumentList_T;

/** List of results, organised by sets, for a given full string. */
typedef std::map<std::string, DocumentList_T> ResultList_T;

/** Memory allocation error. */
class MemoryError : public std::exception {
};


// ///////////////////////// Forward declarations ///////////////////////
std::string display (const Xapian::MSet& iMatchingSet);
std::string display (const DocumentList_T& iDocumentList);
std::string display (const ResultList_T& iResultList);


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

// //////////////////////////////////////////////////////////////////////
std::string createStringFromWordList (const WordList_T& iWordList) {
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

// //////////////////////////////////////////////////////////////////////
void createCorrectedWordList (const WordList_T& iOriginalWordList,
                              WordList_T& ioCorrectedWordList,
                              const Xapian::Database& iDatabase) {
  // Empty the target list
  ioCorrectedWordList.clear();
  
  // Catch any Xapian::Error exceptions thrown
  try {
      
    for (WordList_T::const_iterator itWord = iOriginalWordList.begin();
         itWord != iOriginalWordList.end(); ++itWord) {
      const std::string& lOriginalWord = *itWord;
      const std::string& lSuggestedWord =
        iDatabase.get_spelling_suggestion (lOriginalWord, 3);

      if (lSuggestedWord.empty() == true) {
        ioCorrectedWordList.push_back (lOriginalWord);

      } else {
        ioCorrectedWordList.push_back (lSuggestedWord);
      }

      // DEBUG
      /*
      std::cout << "Original word: `" << lOriginalWord
                << "' ==> corrected word: `" << lSuggestedWord << "'"
                << std::endl;
      */
    }

  } catch (const Xapian::Error& error) {
    std::cerr << "Exception: "  << error.get_msg() << std::endl;
  }
}

// ///////////////////////////////////////////////////////////////////
void searchString (Xapian::MSet& ioMatchingSet, const std::string& iSearchString,
                   Xapian::Database& ioDatabase, std::ostream& ioStream) {

  // Catch any Xapian::Error exceptions thrown
  try {
      
    /**
       Build another string, in addition to the original one. Overall,
       there are thus two strings:
       <br><ul>
       <li>One with the original words given by the user</li>
       <li>One with the orthographic-corrected words, wherever
       relevant (otherwise, the original word is taken)</li>
       </ul>
       <br>For instance, 'sna francisco' would give the following
       two strings:
       <br><ul>
       <li>'sna francicso' (original)</li>
       <li>'sna francisco' (corrected, where relevant, word by word)</li>
       </ul>
       <br>Note that, as 'sna' exists in the dictionary (Santa Ana, CA, USA),
       it is not replaced. We shall take care of the whole string in a
       further step below.
    */
    WordList_T lOriginalWordList;
    tokeniseStringIntoWordList (iSearchString, lOriginalWordList);

    /**
       We rebuild a clean query string from the word list. Indeed, the original
       string may have contained a few separators (e.g., '/', ';', etc.), which
       have been removed by the tokeniseStringIntoWordList() method. All those
       separators are thus replaced by spaces.
       For instance, the 'san francisco, ca, us' initial string would be
       replaced by 'san francisco ca us'.
     */
    const std::string lOriginalQueryString =
      createStringFromWordList (lOriginalWordList);
      
    WordList_T lCorrectedWordList;
    createCorrectedWordList (lOriginalWordList, lCorrectedWordList, ioDatabase);
      
    const std::string lCorrectedQueryString =
      createStringFromWordList (lCorrectedWordList);

    /**
       Try to find, if relevant, an orthographic suggestion for the whole
       phrase/string. With the above example, 'sna francisco' yields the
       suggestion 'san francisco'.
    */
    const std::string lFullWordCorrectedString =
      ioDatabase.get_spelling_suggestion (lOriginalQueryString, 3);

    // DEBUG
    /*
    ioStream << "Query string `" << lOriginalQueryString
              << "' ==> corrected query string: `" << lCorrectedQueryString
              << "' and correction for the full query string: `"
              << lFullWordCorrectedString << "'" << std::endl;
    */

    // Build the query object
    Xapian::QueryParser lQueryParser;
    lQueryParser.set_database (ioDatabase);
    /**
       As explained in http://www.xapian.org/docs/queryparser.html,
       Xapian::Query::OP_ADJ is better than Xapian::Query::OP_PHRASE,
       but only available from version 1.0.13 of Xapian.
    */
    // lQueryParser.set_default_op (Xapian::Query::OP_ADJ);
    lQueryParser.set_default_op (Xapian::Query::OP_PHRASE);

    // DEBUG
    /*
    ioStream << "Query parser `" << lQueryParser.get_description() << "'"
              << std::endl;
    */

    /**
       The Xapian::QueryParser::parse_query() method aggregates all the words
       with operators inbetween them (here, the "PHRASE" operator).
       With the above example ('sna francicso'), it yields
       "sna PHRASE 2 francicso".
    */
    Xapian::Query lQuery =
      lQueryParser.parse_query (lOriginalQueryString,
                                Xapian::QueryParser::FLAG_BOOLEAN
                                | Xapian::QueryParser::FLAG_PHRASE
                                | Xapian::QueryParser::FLAG_LOVEHATE
                                | Xapian::QueryParser::FLAG_SPELLING_CORRECTION);
    /**
       Strangely enough (is it?), the corrected query given by the Xapian
       QueryParser corresponds to the full original string, where words
       have been corrected one by one, but considered as a single block.
       With the above example, 'sna francicso' yields (wrongly)
       'sna francisco', instead of "sna PHRASE 2 francisco", as generated
       by the following code.
    */
    // Xapian::Query lCorrectedQuery =
    //         lQueryParser.get_corrected_query_string();
    Xapian::Query lCorrectedQuery = 
      lQueryParser.parse_query (lCorrectedQueryString,
                                Xapian::QueryParser::FLAG_BOOLEAN
                                | Xapian::QueryParser::FLAG_PHRASE
                                | Xapian::QueryParser::FLAG_LOVEHATE);

    /**
       As, with the above example, the full corrected string is
       'san francisco', it yields the query "san PHRASE 2 francisco",
       which is eventually right.
    */
    Xapian::Query lFullQueryCorrected = 
      lQueryParser.parse_query (lFullWordCorrectedString,
                                Xapian::QueryParser::FLAG_BOOLEAN
                                | Xapian::QueryParser::FLAG_PHRASE
                                | Xapian::QueryParser::FLAG_LOVEHATE);

    // DEBUG
    /*
    ioStream << "Query `" << lQuery.get_description()
              << "', corrected query `" << lCorrectedQuery.get_description()
              << "' and corrected for full query `"
              << lFullQueryCorrected.get_description() << "' " << std::endl;
    */
      
    // Start an enquire session
    Xapian::Enquire enquire (ioDatabase);

    // Give the query object to the enquire session
    enquire.set_query (lQuery);

    // Get the top 10 results of the query
    ioMatchingSet = enquire.get_mset (0, 10);

    // Display the results
    int nbMatches = ioMatchingSet.size();

    // DEBUG
    /*
    ioStream << nbMatches << " results found" << std::endl;
    */

    /**
       When no match is found, we search on the corrected phrase/string
       (where the words have been corrected one by one).
    */
    if (nbMatches == 0) {
      enquire.set_query (lCorrectedQuery);
      ioMatchingSet = enquire.get_mset (0, 10);

      // Display the results
      nbMatches = ioMatchingSet.size();

      // DEBUG
      /*
      ioStream << nbMatches << " results found on corrected string"
                << std::endl;
      */
    }
      
    /**
       If there is still no match, we search on the string corrected
       as a whole.
    */
    if (nbMatches == 0) {
      enquire.set_query (lFullQueryCorrected);
      ioMatchingSet = enquire.get_mset (0, 10);
        
      // Display the results
      nbMatches = ioMatchingSet.size();

      // DEBUG
      /*
      ioStream << nbMatches << " results found on corrected full string"
                << std::endl;
      */
    }

    const Xapian::Query& lActualQuery = enquire.get_query();

    // DEBUG
    /*
    ioStream << "Actual query `" << lActualQuery.get_description()
              << "'" << std::endl;
    */
      
  } catch (const Xapian::Error& error) {
    std::cerr << "Exception: "  << error.get_msg() << std::endl;
  }
}

// //////////////////////////////////////////////////////////////////////
void createDocumentListFromMSet (const Xapian::MSet& iMatchingSet,
                                 DocumentList_T& ioDocumentList) {
  // Empty the list of documents
  ioDocumentList.clear();

  for (Xapian::MSetIterator itDoc = iMatchingSet.begin();
       itDoc != iMatchingSet.end(); ++itDoc) {
    const Xapian::Document& lDocument = itDoc.get_document();

    ioDocumentList.insert (DocumentList_T::value_type (itDoc.get_percent(),
                                                       lDocument));
  }
}

// //////////////////////////////////////////////////////////////////////
void removeOneWord (std::string& ioQueryString) {
  assert (ioQueryString.empty() == false);
  
  WordList_T lWordList;
  tokeniseStringIntoWordList (ioQueryString, lWordList);
  assert (lWordList.empty() == false);

  // Remove the furthest right word
  lWordList.pop_back();
  
  const std::string& lReducedString = createStringFromWordList (lWordList);
  ioQueryString = lReducedString;
}

// //////////////////////////////////////////////////////////////////////
void subtractParsedToRemaining (std::string& ioAlreadyParsedQueryString,
                                std::string& ioRemainingQueryString,
                                std::ostream& ioStream) {
  /**
     Remove, from the lRemainingQueryString string, the part which
     has been already successfully parsed.
     <br>For instance, when 'sna francisco rio de janeiro' is the
     initial full clean query string, the searchString() method
     first reduce the query string to 'sna francisco', which
     successfully matches against SFO (San Francisco airport).
     <br>Then, the remaining part of the query string to be parsed is
     'rio de janeiro'. So, the already parsed part must be subtracted
     from the initial query string.
  */
  WordList_T lRemainingWordList;
  tokeniseStringIntoWordList (ioRemainingQueryString, lRemainingWordList);

  WordList_T lParsedWordList;
  tokeniseStringIntoWordList (ioAlreadyParsedQueryString, lParsedWordList);
  
  unsigned int idx = lParsedWordList.size();
  for ( ; idx != 0 && lRemainingWordList.empty() == false; --idx) {
    lRemainingWordList.pop_front();
  }
  
  // Build the remaining part of the string still to be parsed.
  // <br>Note that that part may be empty.
  ioRemainingQueryString = createStringFromWordList (lRemainingWordList);
}

// //////////////////////////////////////////////////////////////////////
void searchString (DocumentList_T& ioDocumentList, std::string& ioQueryString,
                   Xapian::Database& ioDatabase, std::ostream& ioStream) {

  // Catch any Xapian::Error exceptions thrown
  try {
      
    bool shouldStop = false;
    while (shouldStop == false) {
      // DEBUG
      /*
      ioStream << std::endl << "--------------------------------" << std::endl
               << "Current query string: `" << ioQueryString << "'" << std::endl;
      */
      
      // Retrieve the list of documents matching the query string
      Xapian::MSet lMatchingSet;
      searchString (lMatchingSet, ioQueryString, ioDatabase, ioStream);

      // DEBUG
      /*
      ioStream << "Matching set (" << lMatchingSet.size() << " elements): "
               << display (lMatchingSet);
      */
      
      // Create the corresponding list of documents
      createDocumentListFromMSet (lMatchingSet, ioDocumentList);

      // Stop if  a result is found.
      if (ioDocumentList.empty() == false) {
        shouldStop = true;
        break;
      }
      
      // Remove a word from the query string
      removeOneWord (ioQueryString);
      
      // Stop when the resulting string gets empty.
      if (ioQueryString.empty() == true) {
        shouldStop = true;
      }
    }

  } catch (const Xapian::Error& error) {
    std::cerr << "Exception: "  << error.get_msg() << std::endl;
  }
}

// //////////////////////////////////////////////////////////////////////
void searchString (const std::string& iFullCleanQueryString,
                   ResultList_T& ioResultList, Xapian::Database& ioDatabase,
                   std::ostream& ioStream) {

  // Catch any Xapian::Error exceptions thrown
  try {
      
    std::string lRemainingQueryString (iFullCleanQueryString);
    bool shouldStop = false;
    while (shouldStop == false) {
      // DEBUG
      /*
      ioStream << std::endl << "================================" << std::endl
               << "Current query string: `" << lRemainingQueryString << "'"
               << std::endl;
      */
      /**
         Search with the initial full string, then by removing a word if
         there was no result, then by removing another word if there was
         again no result, until either a result is found or the
         resulting string gets empty.
      */
      DocumentList_T lDocumentList;
      std::string lQueryString (lRemainingQueryString);
      searchString (lDocumentList, lQueryString, ioDatabase, ioStream);
      
      // Add the list of matching documents to the result list
      const bool hasInsertionBeenSuccessfull = 
        ioResultList.insert (ResultList_T::value_type (lQueryString,
                                                       lDocumentList)).second;
      if (hasInsertionBeenSuccessfull == false) {
        std::cerr << "Insertion of document list failed for: `"
                  << display (lDocumentList) << "´" << std::endl;
        throw MemoryError();
      }

      /**
         Remove, from the lRemainingQueryString string, the part which
         has been already successfully parsed.
         <br>For instance, when 'sna francisco rio de janeiro' is the
         initial full clean query string, the searchString() method
         first reduce the query string to 'sna francisco', which
         successfully matches against SFO (San Francisco airport).
         <br>Then, the remaining part of the query string to be parsed is
         'rio de janeiro'. So, the already parsed part, namely 'sna francisco',
         must be subtracted from the initial query string.
      */
      subtractParsedToRemaining (lQueryString, lRemainingQueryString, ioStream);

      // If there is nothing left to be parsed, we have then finished
      // to parse the initial string.
      if (lRemainingQueryString.empty() == true) {
        shouldStop = true;
        break;
      }
    }

  } catch (const Xapian::Error& error) {
    std::cerr << "Exception: "  << error.get_msg() << std::endl;
  }
}

// //////////////////////////////////////////////////////////////////////
void extractResults (const ResultList_T& iResultList,
                     DocumentList_T& ioDocumentList) {

  for (ResultList_T::const_reverse_iterator itResult = iResultList.rbegin();
       itResult != iResultList.rend(); ++itResult) {
    const std::string& lQueryString = itResult->first;
    const DocumentList_T& lDocumentList = itResult->second;

    // Retrieve the best matching document. As the document list (STL map)
    // is sorted by ascending order of the matching percentage, the best
    // matching one is located at the end (back) of the list (STL map).
    DocumentList_T::const_reverse_iterator itDocument = lDocumentList.rbegin();
    const Xapian::percent& lBestMatchingPercentage = itDocument->first;
    const Xapian::Document& lBestMatchingDocument = itDocument->second;

    ioDocumentList.insert (DocumentList_T::value_type (lBestMatchingPercentage,
                                                       lBestMatchingDocument));
  }
}

// //////////////////////////////////////////////////////////////////////
std::string display (const Xapian::MSet& iMatchingSet) {
  std::ostringstream oStr;
  
  for (Xapian::MSetIterator itDoc = iMatchingSet.begin();
       itDoc != iMatchingSet.end(); ++itDoc) {
    const Xapian::Document& lDocument = itDoc.get_document();
    const Xapian::docid& lDocID = lDocument.get_docid();
    
    oStr << "Document ID " << lDocID << "\t" << itDoc.get_percent()
         << "% [" << lDocument.get_data() << "]" << std::endl;
  }
  
  return oStr.str();
}

// //////////////////////////////////////////////////////////////////////
std::string display (const DocumentList_T& iDocumentList) {
  std::ostringstream oStr;
  
  for (DocumentList_T::const_iterator itDoc = iDocumentList.begin();
       itDoc != iDocumentList.end(); ++itDoc) {
    const Xapian::percent& lPercent = itDoc->first;
    const Xapian::Document& lDocument = itDoc->second;
    const Xapian::docid& lDocID = lDocument.get_docid();
    
    oStr << "Document ID " << lDocID << "\t" << lPercent
         << "% [" << lDocument.get_data() << "]" << std::endl;
  }
  
  return oStr.str();
}

// //////////////////////////////////////////////////////////////////////
std::string display (const ResultList_T& iResultList) {
  std::ostringstream oStr;
  
  for (ResultList_T::const_iterator itResult = iResultList.begin();
       itResult != iResultList.end(); ++itResult) {
    const std::string& lQueryString = itResult->first;
    const DocumentList_T& lDocumentList = itResult->second;
    
    oStr << "Result for query (sub-)string: `" << lQueryString << "´:"
         << std::endl << display (lDocumentList);
  }
  
  return oStr.str();
}

// //////////////////////////// M A I N //////////////////////////////
int main (int argc, char* argv[]) {

  // Simplest possible options parsing: we just require two or more
  // parameters.
  if (argc < 3) {
    std::cout << "Usage: " << argv[0]
              << " <path to database> <search terms>" << std::endl;
    return -1;
  }

  // Catch any Xapian::Error exceptions thrown
  try {
      
    // Make the database
    const std::string lXapianDatabaseFilepath (argv[1]);
    Xapian::Database lDatabase (lXapianDatabaseFilepath);

    /**
       Build a query string from the command line parameters.
       That way, any other front end producing a query string will
       be fine.
    */
    std::ostringstream lQueryStringStr;
    for (unsigned int idx = 2; idx != argc; ++idx) {
      if (idx != 2) {
        lQueryStringStr << " ";
      }
      const std::string lWord (argv[idx]);
      lQueryStringStr << lWord;
    }
    const std::string& lFullCleanQueryString = lQueryStringStr.str();

    //
    ResultList_T lResultList;
    searchString (lFullCleanQueryString, lResultList, lDatabase, std::cout);

    // DEBUG
    /*
    std::cout << std::endl << "Result list: "  << std::endl
              << display (lResultList);
    */
      
    // Extract the best matching results from each matching set (document list)
    DocumentList_T lDocumentList;
    extractResults (lResultList, lDocumentList);
    
    // DEBUG
    std::cout << std::endl
              << "_________________________________________" << std::endl
              << "=========================================" << std::endl
              << "-----------------------------------------" << std::endl
              << "Matching list: "  << std::endl
              << display (lDocumentList)
              << "_________________________________________" << std::endl
              << "=========================================" << std::endl
              << "-----------------------------------------" << std::endl
              << std::endl;

  } catch (const Xapian::Error& error) {
    std::cerr << "Exception: "  << error.get_msg() << std::endl;
  }

  return 0;
}
