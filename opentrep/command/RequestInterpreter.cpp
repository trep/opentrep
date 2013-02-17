// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
#include <string>
#include <vector>
#include <exception>
// Boost
#include <boost/filesystem.hpp>
// OpenTrep
#include <opentrep/bom/Filter.hpp>
#include <opentrep/bom/WordHolder.hpp>
#include <opentrep/bom/Place.hpp>
#include <opentrep/bom/ResultCombination.hpp>
#include <opentrep/bom/ResultHolder.hpp>
#include <opentrep/bom/Result.hpp>
#include <opentrep/bom/PlaceHolder.hpp>
#include <opentrep/bom/QuerySlices.hpp>
#include <opentrep/bom/StringPartition.hpp>
#include <opentrep/factory/FacPlaceHolder.hpp>
#include <opentrep/factory/FacPlace.hpp>
#include <opentrep/factory/FacResultCombination.hpp>
#include <opentrep/factory/FacResultHolder.hpp>
#include <opentrep/factory/FacResult.hpp>
#include <opentrep/command/RequestInterpreter.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  /**
   * Helper function to add the given string to the list of unknown words,
   * only when that given string is made of a single word. Otherwise, the
   * given string is simply ignored.
   *
   * @param const TravelQuery_T& The given string
   * @param WordList_T& ioWordList The list to which the given single-word
   *        string should be added (if needed).
   * @param WordSet_T& A (STL) set of words, allowing to keep track of
   *        the already added words.
   */
  // //////////////////////////////////////////////////////////////////////
  void addUnmatchedWord (const TravelQuery_T& iQueryString,
                         WordList_T& ioWordList, WordSet_T& ioWordSet) {
    // Token-ise the given string
    WordList_T lQueryStringWordList;
    WordHolder::tokeniseStringIntoWordList (iQueryString,
                                            lQueryStringWordList);
    if (lQueryStringWordList.size() == 1) {
      // Add the unmatched/unknown word, only when that latter has not
      // already been stored, and when it is not black-listed.
      // const bool shouldBeKept = Filter::shouldKeep ("", iQueryString);
      const bool shouldBeKept = true;
              
      WordSet_T::const_iterator itWord = ioWordSet.find (iQueryString);
      if (shouldBeKept == true && itWord == ioWordSet.end()) {
        ioWordSet.insert (iQueryString);
        ioWordList.push_back (iQueryString);
      }
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void createPlaces (const ResultCombination& iResultCombination,
                     PlaceHolder& ioPlaceHolder) {
    
    // Retrieve the best matching ResultHolder object.
    const ResultHolder& lResultHolder =
      iResultCombination.getBestMatchingResultHolder();    

    // Browse the list of result objects
    const ResultList_T& lResultList = lResultHolder.getResultList();
    for (ResultList_T::const_iterator itResult = lResultList.begin();
         itResult != lResultList.end(); ++itResult) {
      // Retrieve the result object
      const Result* lResult_ptr = *itResult;
      assert (lResult_ptr != NULL);

      /**
       * When there has been no full-text match, the Result object exists,
       * but, by construction, it does not correspond to any Xapian document.
       */
      const bool hasFullTextMatched = lResult_ptr->hasFullTextMatched();
      if (hasFullTextMatched == false) {
        continue;
      }
      assert (hasFullTextMatched == true);

      // Retrieve the Xapian document data (string)
      const std::string& lDocDataStr = lResult_ptr->getBestDocData();
      const RawDataString_T& lDocData = RawDataString_T (lDocDataStr);

      // Parse the POR details and create the corresponding Location structure
      const Location& lLocation = Result::retrieveLocation (lDocData);

      // Instanciate an empty place object, which will be filled from the
      // rows retrieved from the database.
      Place& lPlace = FacPlace::instance().create (lLocation);
      
      // Insert the Place object within the PlaceHolder object
      FacPlaceHolder::initLinkWithPlace (ioPlaceHolder, lPlace);
      
      // Fill the place with the remaining of the Result details.
      lResult_ptr->fillPlace (lPlace);

      // DEBUG
      OPENTREP_LOG_DEBUG ("Retrieved Document: " << lPlace.toString());
    }
  }
  
  /**
   * For all the elements (StringSet) of the string partitions, derived
   * from the given travel query, perform a Xapian-based full-text match.
   * Each Xapian-based full-text match gives (potentially) a full set of
   * matches, some with the highest matching percentage and some with a
   * lower percentage.
   *
   * @param TravelQuery_T& The query string.
   * @param const Xapian::Database& The Xapian index/database.
   * @param ResultCombination& List of ResultHolder objects.
   * @param WordList_T& List of non-matched words of the query string.
   */
  // //////////////////////////////////////////////////////////////////////
  void searchString (const StringPartition& iStringPartition,
                     const Xapian::Database& iDatabase,
                     ResultCombination& ioResultCombination,
                     WordList_T& ioWordList) {

    // Catch any thrown Xapian::Error exceptions
    try {
      
      // Set of unknown words (just to eliminate the duplicates)
      WordSet_T lWordSet;

      // Browse the partitions
      for (StringPartition::StringPartition_T::const_iterator itSet =
             iStringPartition._partition.begin();
           itSet != iStringPartition._partition.end(); ++itSet) {
        const StringSet& lStringSet = *itSet;

        // DEBUG
        OPENTREP_LOG_DEBUG ("  ==========");
        OPENTREP_LOG_DEBUG ("  String set: " << lStringSet);

        // Create a ResultHolder object.
        ResultHolder& lResultHolder =
          FacResultHolder::instance().create (lStringSet.describe(), iDatabase);

        // Add the ResultHolder object to the dedicated list.
        FacResultCombination::initLinkWithResultHolder (ioResultCombination,
                                                        lResultHolder);

        // Browse through all the word combinations of the partition
        for (StringSet::StringSet_T::const_iterator itString =
               lStringSet._set.begin();
             itString != lStringSet._set.end(); ++itString) {
          //
          const std::string lQueryString (*itString);

          // DEBUG
          OPENTREP_LOG_DEBUG ("    --------");
          OPENTREP_LOG_DEBUG ("    Query string: '" << lQueryString << "'");

          // Create an empty Result object
          Result& lResult = FacResult::instance().create (lQueryString,
                                                          iDatabase);
      
          // Add the Result object to the dedicated list.
          FacResultHolder::initLinkWithResult (lResultHolder, lResult);

          // Perform the Xapian-based full-text match: the set of
          // matching documents is filled.
          const std::string& lMatchedString =
            lResult.fullTextMatch (iDatabase, lQueryString);

          // When a single-word string is unmatched/unknown by/from Xapian,
          // add it to the dedicated list (i.e., ioWordList).
          if (lMatchedString.empty() == true) {
            OPENTREP::addUnmatchedWord (lQueryString, ioWordList, lWordSet);
          }
        }

        // DEBUG
        OPENTREP_LOG_DEBUG (std::endl
                            << "========================================="
                            << std::endl << "Result holder: "
                            << lResultHolder.toString() << std::endl
                            << "========================================="
                            << std::endl << std::endl);
      }

      // DEBUG
      OPENTREP_LOG_DEBUG ("*********************");

    } catch (const Xapian::Error& error) {
      // Error
      OPENTREP_LOG_ERROR ("Exception: "  << error.get_msg());
      throw XapianException (error.get_msg());
    }
  }

  /**
   * Select the best matching string partition, based on the results of
   * several rules, that are all materialised by weighting percentages:
   * <ul>
   *   <li>Xapian-based full-text match. Score type: XAPIAN_PCT</li>
   *   <li>Schedule-derived PageRank for the point of reference (POR).
   *       Score type: PAGE_RANK</li>
   *   <li>Heuristic. Score type: Heuristic</li>
   * </ul>
   * \see ScoreType.hpp for the various types of score.
   *
   * The score for the combination (score type: COMBINATION) is simply the
   * product of all the scores/weighting percentages.
   *
   * @param ResultCombination& List of ResultHolder objects.
   */
  // //////////////////////////////////////////////////////////////////////
  void chooseBestMatchingResultHolder (ResultCombination& ioResultCombination) {

    // Calculate the weights for the full-text matches
    const bool doesBestMatchingResultHolderExist =
      ioResultCombination.chooseBestMatchingResultHolder();

    if (doesBestMatchingResultHolderExist == true) {
      const ResultHolder& lBestMatchingResultHolder =
        ioResultCombination.getBestMatchingResultHolder();

      // DEBUG
      const StringSet& lCorrectedStringSet =
        ioResultCombination.getCorrectedStringSet();
      OPENTREP_LOG_DEBUG ("The best matching string partition for '"
                          << ioResultCombination.describeShortKey() << "' is "
                          << lBestMatchingResultHolder.describeShortKey()
                          << ", and has got a weight of "
                          << ioResultCombination.getBestMatchingWeight()
                          << "%. The corrected string set is: "
                          << lCorrectedStringSet);

    } else {
      // DEBUG
      OPENTREP_LOG_DEBUG ("There is no match for '"
                          << ioResultCombination.describeShortKey() << "'");
    }
  }

  // //////////////////////////////////////////////////////////////////////
  NbOfMatches_T RequestInterpreter::
  interpretTravelRequest (const TravelDBFilePath_T& iTravelDBFilePath,
                          const TravelQuery_T& iTravelQuery,
                          LocationList_T& ioLocationList,
                          WordList_T& ioWordList) {
    NbOfMatches_T oNbOfMatches = 0;

    // Sanity check
    assert (iTravelQuery.empty() == false);

    // Create a PlaceHolder object, to collect the matching Place objects
    PlaceHolder& lPlaceHolder = FacPlaceHolder::instance().create();

    // Check whether the file-path to the Xapian database/index exists
    // and is a directory.
    boost::filesystem::path lTravelDBFilePath (iTravelDBFilePath.begin(),
                                               iTravelDBFilePath.end());
    if (!(boost::filesystem::exists (lTravelDBFilePath)
          && boost::filesystem::is_directory (lTravelDBFilePath))) {
      std::ostringstream oStr;
      oStr << "The file-path to the Xapian database/index ('"
           << iTravelDBFilePath << "') does not exist or is not a directory.";
      OPENTREP_LOG_ERROR (oStr.str());
      throw FileNotFoundException (oStr.str());
    }

    // Open the Xapian database
    Xapian::Database lXapianDatabase (iTravelDBFilePath);

    // DEBUG
    OPENTREP_LOG_DEBUG (std::endl
                        << "=========================================");
      
    /**
     * 0. Initialisation
     *
     * Create a ResultCombination BOM instance.
     */
    ResultCombination& lResultCombination =
      FacResultCombination::instance().create (iTravelQuery);

    // First, cut the travel query in slices and calculate all the partitions
    // for each of those query slices
    QuerySlices lQuerySlices (lXapianDatabase, iTravelQuery);

    // DEBUG
    OPENTREP_LOG_DEBUG ("+=+=+=+=+=+=+=+=+=+=+=+=+=+=+");
    OPENTREP_LOG_DEBUG ("Travel query: `" << iTravelQuery << "'");
    OPENTREP_LOG_DEBUG ("Query slices: `" << lQuerySlices << "'");

    // Browse the travel query slices
    const StringPartitionList_T& lStringPartitionList =
      lQuerySlices.getStringPartitionList();
    for (StringPartitionList_T::const_iterator itSlice =
           lStringPartitionList.begin();
         itSlice != lStringPartitionList.end(); ++itSlice) {
      StringPartition lStringPartition = *itSlice;
      const std::string& lTravelQuerySlice = lStringPartition.getInitialString();

      // DEBUG
      // DEBUG
      OPENTREP_LOG_DEBUG ("+++++++++++++++++++++");
      OPENTREP_LOG_DEBUG ("Travel query slice: `" << lTravelQuerySlice << "'");
      OPENTREP_LOG_DEBUG ("Partitions: " << lStringPartition);

      /**
       * 1.1. Perform all the full-text matches, and fill accordingly the
       *      list of Result instances.
       */
      OPENTREP::searchString (lTravelQuerySlice, lXapianDatabase,
                              lResultCombination, ioWordList);

      /**
       * 1.2. Calculate/set the PageRanks for all the matching documents
       */
      lResultCombination.calculatePageRanks();

      /**
       * 1.3. Calculate/set the heuristic weights for all the matching documents
       */
      lResultCombination.calculateHeuristicWeights();

      /**
       * 1.4. Calculate/set the combined weights for all the matching documents
       */
      lResultCombination.calculateCombinedWeights();

      /**
       * 2. Calculate the best matching scores / weighting percentages.
       */
      OPENTREP::chooseBestMatchingResultHolder (lResultCombination);

      /**
       * 3. Create the list of Place objects, for each of which a
       *    look-up is made in the SQL database (e.g., MySQL or Oracle)
       *    to retrieve complementary data.
       */
      createPlaces (lResultCombination, lPlaceHolder);
      
      // DEBUG
      OPENTREP_LOG_DEBUG (std::endl
                          << "========================================="
                          << std::endl << "Summary:" << std::endl
                          << lPlaceHolder.toShortString() << std::endl
                          << "========================================="
                          << std::endl);
    }

    /**
     * 4. Create the list of Location structures, which are light copies
     *    of the Place objects. Those (Location) structures are passed
     *    back to the caller of the service.
     */
    lPlaceHolder.createLocations (ioLocationList);
    oNbOfMatches = ioLocationList.size();
    
    return oNbOfMatches;
  }
  
}
