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
#include <boost/regex.hpp>
// SOCI
#include <soci/soci.h>
// OpenTrep
#include <opentrep/DBType.hpp>
#include <opentrep/basic/OTransliterator.hpp>
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
#include <opentrep/command/DBManager.hpp>
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
      const bool shouldBeKept = Filter::shouldKeep ("", iQueryString);
      //const bool shouldBeKept = true;
              
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
   *   <li>Heuristic. Score type: HEURISTIC</li>
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
  bool RequestInterpreter::areAllCodeOrGeoID (const TravelQuery_T& iQueryString,
                                              WordList_T& ioWordList) {
    bool areAllWordsCodes = true;

    // Token-ise the given string
    WordHolder::tokeniseStringIntoWordList (iQueryString, ioWordList);
    for (WordList_T::const_iterator itWord = ioWordList.begin();
         itWord != ioWordList.end(); ++itWord) {
      const std::string& lWord = *itWord;

      // IATA code: alpha{3}
      const boost::regex lIATACodeExp ("^[[:alpha:]]{3}$");
      const bool lMatchesWithIATACode = regex_match (lWord, lIATACodeExp);

      // ICAO code: (alpha|digit){4}
      const boost::regex lICAOCodeExp ("^([[:alpha:]]|[[:digit:]]){4}$");
      const bool lMatchesWithICAOCode = regex_match (lWord, lICAOCodeExp);

      // UN/LOCODE code: alpha{2}(alpha|digit){3}
      const boost::regex
        lUNLOCodeExp ("^[[:alpha:]]{2}([[:alpha:]]|[[:digit:]]){3}$");
      const bool lMatchesWithUNLOCode = regex_match (lWord, lUNLOCodeExp);

      // Geonames ID: digit{1,12}
      const boost::regex lGeoIDCodeExp ("^[[:digit:]]{1,12}$");
      const bool lMatchesWithGeoID = regex_match (lWord, lGeoIDCodeExp);

      // If the word is neither a IATA/ICAO code or a Geonames ID,
      // there is nothing more to be done at that stage. The query string
      // will have to be fully analysed.
      // Otherwise, we go on analysing the other words.
      if (lMatchesWithIATACode == false && lMatchesWithICAOCode == false
          && lMatchesWithUNLOCode == false && lMatchesWithGeoID == false) {
        areAllWordsCodes = false;
        break;
      }
    }

    return areAllWordsCodes;
  }

  /**
   * Retrieve the location matching the best, i.e., having the highest score
   * for some rules. In practice, that score is usually the PageRank value.
   * When two or morer Location structures have the same highest score,
   * the first one is taken from the given input list.
   */
  // //////////////////////////////////////////////////////////////////////
  Location getBestMatchingLocation (const LocationList_T& iLocationList) {
    Location oLocation;
    PageRank_T lMaxPageRank = 0.0;

    for (LocationList_T::const_iterator itLocation = iLocationList.begin();
         itLocation != iLocationList.end(); ++itLocation) {
      const Location& lLocation = *itLocation;
      
      // Get the PageRank value
      const PageRank_T& lPageRank = lLocation.getPageRank();
      if (lPageRank > lMaxPageRank) {
        lMaxPageRank = lPageRank;
        oLocation = lLocation;
      }
    }

    return oLocation;
  }
  
  /**
   * Return the list of locations/places corresponding
   * to the given IATA/ICAO/UNLOCODE codes or Geonames IDs.
   *
   * @param const DBType& SQL database type (can be no database at all).
   * @param const SQLDBConnectionString_T& SQL DB connection string.
   * @param const WordList_T& List of IATA/ICAO/UNLOCODE codes or Geonames ID
   *        (e.g., "sna 5391989 6299418 los chi cnshg lso rek lfmn iev mow").
   * @param LocationList_T& The matching (geographical) locations, if any,
   *                        are added to that list.
   * @param WordList_T& List of non-matched words of the query string.
   * @return NbOfMatches_T Number of matches.
   */
  // //////////////////////////////////////////////////////////////////////
  NbOfMatches_T getLocationList (const DBType& iSQLDBType,
                                 const SQLDBConnectionString_T& iSQLDBConnStr,
                                 const WordList_T& iCodeList,
                                 LocationList_T& ioLocationList,
                                 WordList_T& ioWordList) {
    NbOfMatches_T oNbOfMatches = 0;

    // Connect to the SQL database/file
    soci::session* lSociSession_ptr =
      DBManager::initSQLDBSession (iSQLDBType, iSQLDBConnStr);
    if (lSociSession_ptr == NULL) {
      std::ostringstream oStr;
      oStr << "The " << iSQLDBType.describe()
           << " database is not accessible. Connection string: "
           << iSQLDBConnStr << std::endl
           << "Hint: launch the 'opentrep-dbmgr' program and "
           << "see the 'tutorial' command.";
      OPENTREP_LOG_ERROR (oStr.str());
      throw SQLDatabaseImpossibleConnectionException (oStr.str());
    }
    assert (lSociSession_ptr != NULL);

    // Browse the list of words/items
    for (WordList_T::const_iterator itWord = iCodeList.begin();
         itWord != iCodeList.end(); ++itWord) {
      const std::string& lWord = *itWord;

      // Check for IATA code: alpha{3}
      const boost::regex lIATACodeExp ("^[[:alpha:]]{3}$");
      const bool lMatchesWithIATACode = regex_match (lWord, lIATACodeExp);
      if (lMatchesWithIATACode == true) {
        // Perform the select statement on the underlying SQL database
        const IATACode_T lIATACode (lWord);
        const bool lUniqueEntry = true;
        const NbOfDBEntries_T& lNbOfEntries =
          DBManager::getPORByIATACode (*lSociSession_ptr, lIATACode,
                                       ioLocationList, lUniqueEntry);
        oNbOfMatches += lNbOfEntries;
        continue;
      }

      // Check for ICAO code: (alpha|digit){4}
      const boost::regex lICAOCodeExp ("^([[:alpha:]]|[[:digit:]]){4}$");
      const bool lMatchesWithICAOCode = regex_match (lWord, lICAOCodeExp);
      if (lMatchesWithICAOCode == true) {
        // Perform the select statement on the underlying SQL database
        const ICAOCode_T lICAOCode (lWord);
        const NbOfDBEntries_T& lNbOfEntries =
          DBManager::getPORByICAOCode (*lSociSession_ptr, lICAOCode,
                                       ioLocationList);
        oNbOfMatches += lNbOfEntries;
        continue;
      }

      // Check for UN/LOCODE code: alpha{2}(alpha|digit){3}
      const boost::regex
        lUNLOCodeExp ("^[[:alpha:]]{2}([[:alpha:]]|[[:digit:]]){3}$");
      const bool lMatchesWithUNLOCode = regex_match (lWord, lUNLOCodeExp);
      if (lMatchesWithUNLOCode == true) {
        // Perform the select statement on the underlying SQL database
        const UNLOCode_T lUNLOCode (lWord);
        const bool lUniqueEntry = true;
        const NbOfDBEntries_T& lNbOfEntries =
          DBManager::getPORByUNLOCode (*lSociSession_ptr, lUNLOCode,
                                       ioLocationList, lUniqueEntry);
        oNbOfMatches += lNbOfEntries;
        continue;
      }      

      // Check for Geonames ID: digit{1,12}
      const boost::regex lGeoIDCodeExp ("^[[:digit:]]{1,12}$");
      const bool lMatchesWithGeoID = regex_match (lWord, lGeoIDCodeExp);
      if (lMatchesWithGeoID == true) {
        try {
          // Convert the character string into a number
          const GeonamesID_T lGeonamesID =
            boost::lexical_cast<GeonamesID_T> (lWord);
          
          // Perform the select statement on the underlying SQL database
          const NbOfDBEntries_T& lNbOfEntries =
            DBManager::getPORByGeonameID (*lSociSession_ptr, lGeonamesID,
                                          ioLocationList);
          oNbOfMatches += lNbOfEntries;

        } catch (boost::bad_lexical_cast& eCast) {
          OPENTREP_LOG_ERROR ("The Geoname ID ('" << lWord
                              << "') cannot be understood.");
        }
      }
    }

    return oNbOfMatches;
  }

  // //////////////////////////////////////////////////////////////////////
  NbOfMatches_T RequestInterpreter::
  interpretTravelRequest (const TravelDBFilePath_T& iTravelDBFilePath,
                          const DBType& iSQLDBType,
                          const SQLDBConnectionString_T& iSQLDBConnStr,
                          const TravelQuery_T& iTravelQuery,
                          LocationList_T& ioLocationList,
                          WordList_T& ioWordList,
                          const OTransliterator& iTransliterator) {
    NbOfMatches_T oNbOfMatches = 0;

    // Sanity check
    assert (iTravelQuery.empty() == false);

    // Check whether the file-path to the Xapian database/index exists
    // and is a directory.
    boost::filesystem::path lTravelDBFilePath (iTravelDBFilePath.begin(),
                                               iTravelDBFilePath.end());
    if (!(boost::filesystem::exists (lTravelDBFilePath)
          && boost::filesystem::is_directory (lTravelDBFilePath))) {
      std::ostringstream oStr;
      oStr << "The file-path to the Xapian database/index ('"
           << iTravelDBFilePath << "') does not exist or is not a directory. ";
      oStr << "That usually means that the OpenTREP indexer (opentrep-indexer) "
           << "has not been launched yet, or that it has operated "
           << "on a different Xapian database/index file-path.";
      OPENTREP_LOG_ERROR (oStr.str());
      throw FileNotFoundException (oStr.str());
    }

    // Open the Xapian database
    Xapian::Database lXapianDatabase (iTravelDBFilePath);

    // DEBUG
    OPENTREP_LOG_DEBUG (std::endl
                        << "=========================================");
      
    // First, cut the travel query in slices and calculate all the partitions
    // for each of those query slices
    QuerySlices lQuerySlices (lXapianDatabase, iTravelQuery, iTransliterator);

    // DEBUG
    OPENTREP_LOG_DEBUG ("+=+=+=+=+=+=+=+=+=+=+=+=+=+=+");
    OPENTREP_LOG_DEBUG ("Travel query: `" << iTravelQuery << "'");
    const TravelQuery_T& lNormalisedQueryString = lQuerySlices.getQueryString();
    if (!(iTravelQuery == lNormalisedQueryString)) {
      OPENTREP_LOG_DEBUG ("Normalised travel query: `" << lNormalisedQueryString
                          << "'");
    }
    OPENTREP_LOG_DEBUG ("Query slices: `" << lQuerySlices << "'");

    // Browse the travel query slices
    const StringPartitionList_T& lStringPartitionList =
      lQuerySlices.getStringPartitionList();
    for (StringPartitionList_T::const_iterator itSlice =
           lStringPartitionList.begin();
         itSlice != lStringPartitionList.end(); ++itSlice) {
      StringPartition lStringPartition = *itSlice;
      const std::string& lTravelQuerySlice = lStringPartition.getInitialString();

      /**
       * 0. Initialisation
       *
       * Create a ResultCombination BOM instance.
       */
      ResultCombination& lResultCombination =
        FacResultCombination::instance().create (lTravelQuerySlice);

      // DEBUG
      OPENTREP_LOG_DEBUG ("+++++++++++++++++++++");
      OPENTREP_LOG_DEBUG ("Travel query slice: `" << lTravelQuerySlice << "'");
      OPENTREP_LOG_DEBUG ("Partitions: " << lStringPartition);


      /**
       * 1.0. Check whether the travel query is made only
       *      of IATA/ICAO/UNLOCODE codes and Geonames ID.
       */
      WordList_T lCodeList;
      const bool areAllWordsCodes =
        areAllCodeOrGeoID (lTravelQuerySlice, lCodeList);

      NbOfMatches_T lNbOfMatches = 0;
      if (areAllWordsCodes == true && !(iSQLDBType == DBType::NODB)) {
        /**
         * All the words/items of the travel query are either
         * IATA/ICAO/UNLOCODE codes or Geonames ID. The corresponding details
         * will be retrieved directly from the underlying database,
         * if existing. The Xapian database/index is not used.
         */
        // DEBUG
        OPENTREP_LOG_DEBUG ("The travel query string (" << lTravelQuerySlice
                            << ") is made only of IATA/ICAO/UNLOCODE codes "
                            << "or Geonames ID. The " << iSQLDBType.describe()
                            << " SQL database (" << iSQLDBConnStr
                            << ") will be used. "
                            << "The Xapian database/index will not be used");

        lNbOfMatches = getLocationList (iSQLDBType, iSQLDBConnStr, lCodeList,
                                        ioLocationList, ioWordList);
      }

      if (lNbOfMatches == 0) {
        /**
         * <ul>
         *   <li>Some of the words/items of the travel query are neither
         *        IATA/ICAO codes nor Geonames ID;</li>
         *   <li>or there is no underlying SQL database;</li>
         *   <li>or the word/item is 3/4-character long but is not
         *       a IATA/ICAO code (e.g., lviv)</li>
         * </ul>
         * The Xapian database/index must therefore be used.
         */
        // DEBUG
        if (iSQLDBType == DBType::NODB) {
          OPENTREP_LOG_DEBUG ("No SQL database may be used. "
                              << "The Xapian database will be used instead");
        } else {
          OPENTREP_LOG_DEBUG ("The travel query string (" << lTravelQuerySlice
                              << ") has got items/words, which are neither "
                              << "IATA/ICAO codes nor Geonames ID. "
                              << "The Xapian database/index will be used");
        }

        /**
         * 1.1. Perform all the full-text matches, and fill accordingly the
         *      list of Result instances.
         */
        OPENTREP::searchString (lTravelQuerySlice, lXapianDatabase,
                                lResultCombination, ioWordList);

        /**
         * 1.2. Calculate/set all the weights for all the matching documents
         */
        lResultCombination.calculateAllWeights();

        /**
         * 2. Calculate the best matching scores / weighting percentages.
         */
        OPENTREP::chooseBestMatchingResultHolder (lResultCombination);

        /**
         * 3. Create the list of Place objects, for each of which a
         *    look-up is made in the SQL database (e.g., MySQL or Oracle)
         *    to retrieve complementary data.
         */
        // Create a PlaceHolder object, to collect the matching Place objects
        PlaceHolder& lPlaceHolder = FacPlaceHolder::instance().create();
        createPlaces (lResultCombination, lPlaceHolder);
      
        // DEBUG
        OPENTREP_LOG_DEBUG (std::endl
                            << "========================================="
                            << std::endl << "Summary:" << std::endl
                            << lPlaceHolder.toShortString() << std::endl
                            << "========================================="
                            << std::endl);

        /**
         * 4. Create a list of Location structures, which are light copies
         *    of the Place objects, and add them to the given list.
         */
        lPlaceHolder.createLocations (ioLocationList);
      }
    }

    oNbOfMatches = ioLocationList.size();
    return oNbOfMatches;
  }
  
}
