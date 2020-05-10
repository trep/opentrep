// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <ostream>
#include <sstream>
// Boost (Extended STL)
#include <boost/tokenizer.hpp>
// OpenTrep
#include <opentrep/OPENTREP_exceptions.hpp>
#include <opentrep/DBType.hpp>
#include <opentrep/basic/Utilities.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  void tokeniseStringIntoWordList (const std::string& iPhrase,
                                   WordList_T& ioWordList) {
    // Empty the word list
    ioWordList.clear();
  
    // Boost Tokeniser
    typedef boost::tokenizer<boost::char_separator<char> > Tokeniser_T;
  
    // Define the single-character separators.
    // Note that multi-byte Unicode characters (e.g., “, ”)
    // should not be inserted here
    const boost::char_separator<char>
      lSepatorList(" .,;:|+-*/_=!@#$%`~^&(){}[]?'<>\"");
  
    // Initialise the phrase to be tokenised
    Tokeniser_T lTokens (iPhrase, lSepatorList);
    for (Tokeniser_T::const_iterator tok_iter = lTokens.begin();
         tok_iter != lTokens.end(); ++tok_iter) {
      const std::string& lTerm = *tok_iter;
      ioWordList.push_back (lTerm);
    }
  }

  // //////////////////////////////////////////////////////////////////////
  std::string createStringFromWordList (const WordList_T& iWordList,
                                        const NbOfWords_T iSplitIdx,
                                        const bool iFromBeginningFlag) {
    std::ostringstream oStr;

    // Browse the left-hand side of the string
    NbOfWords_T idx = 0;
    WordList_T::const_iterator itWord = iWordList.begin();
    for ( ; itWord != iWordList.end(); ++itWord, ++idx) {

      if (iFromBeginningFlag == true) {
        // The beginning of the word list is needed

        // Check whether the sub-list has reached the expected split point,
        // if any
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
        
        // Check whether the sub-list has reached the expected split point,
        // if any
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

  // //////////////////////////////////////////////////////////////////////
  StringMap_T
  parseMySQLConnectionString (const SQLDBConnectionString_T& iSQLDBConnStr) {
    StringMap_T oStrMap;
    
    std::stringstream lConnStream (iSQLDBConnStr);
    std::string kvStr;
    std::vector<std::string> kvList;
    unsigned short keyDBName = 0;
    unsigned short keyDBUser = 0;
    unsigned short keyDBPasswd = 0;
    unsigned short lastKey = 0;

    while (std::getline (lConnStream, kvStr, ' ')) {
      std::stringstream kvStream (kvStr);
      std::string keyStr;

      while (std::getline (kvStream, keyStr, '=')) {
        if (keyStr == "db") {
          ++lastKey;
          keyDBName = lastKey;
          continue;
          
        } else if (keyStr == "user") {
          ++lastKey;
          keyDBUser = lastKey;
          continue;
          
        } else if (keyStr == "password") {
          ++lastKey;
          keyDBPasswd = lastKey;
          continue;
          
        } else if (lastKey == keyDBName) {
          const bool isSuccess =
            oStrMap.insert (std::make_pair ("db", keyStr)).second;
	  if (isSuccess == false) {
	    std::ostringstream errStr;
            errStr << "Internal error while inserting the SQL database name ('"
                   << keyDBName << "') into the internal STL map";
            OPENTREP_LOG_ERROR (errStr.str());
	  }
          assert (isSuccess == true);
          continue;
          
        } else if (lastKey == keyDBUser) {
          const bool isSuccess =
            oStrMap.insert (std::make_pair ("user", keyStr)).second;
          if (isSuccess == false) {
            std::ostringstream errStr;
            errStr << "Internal error while inserting the SQL database user ('"
		   << keyDBUser << "') into the internal STL map";
	    OPENTREP_LOG_ERROR (errStr.str());
	  }
	  assert (isSuccess == true);
          continue;
          
        } else if (lastKey == keyDBPasswd) {
          const bool isSuccess =
            oStrMap.insert (std::make_pair ("password", keyStr)).second;
	  if (isSuccess == false) {
	    std::ostringstream errStr;
	    errStr << "Internal error while inserting the SQL database password "
		   << " into the internal STL map";
	    OPENTREP_LOG_ERROR (errStr.str());
	  }
          assert (isSuccess == true);
          continue;
        }
      }
    }
    
    /**
     * Check that the parsing went well
     */
    // SQL database name
    const StringMap_T::const_iterator itDBName = oStrMap.find ("db");
    if (itDBName == oStrMap.end()) {
      std::ostringstream errStr;
      errStr << "Error when parsing the SQL database connection string ('"
             << iSQLDBConnStr << "'), the 'db' value cannot be found";
      OPENTREP_LOG_ERROR (errStr.str());
      throw SQLDatabaseConnectionStringParsingException (errStr.str());
    }

    // SQL database user
    const StringMap_T::const_iterator itDBUser = oStrMap.find ("user");
    if (itDBUser == oStrMap.end()) {
      std::ostringstream errStr;
      errStr << "Error when parsing the SQL database connection string ('"
             << iSQLDBConnStr << "'), the 'user' value cannot be found";
      OPENTREP_LOG_ERROR (errStr.str());
      throw SQLDatabaseConnectionStringParsingException (errStr.str());
    }

    // SQL database password
    const StringMap_T::const_iterator itDBPasswd = oStrMap.find ("password");
    if (itDBPasswd == oStrMap.end()) {
      std::ostringstream errStr;
      errStr << "Error when parsing the SQL database connection string ('"
             << iSQLDBConnStr << "'), the 'password' value cannot be found";
      OPENTREP_LOG_ERROR (errStr.str());
      throw SQLDatabaseConnectionStringParsingException (errStr.str());
    }

    return oStrMap;
  }

  // //////////////////////////////////////////////////////////////////////
  SQLDBConnectionString_T
  buildMySQLConnectionString (const StringMap_T& iStringMap,
                              const DeploymentNumber_T& iDeploymentNumber) {
    std::ostringstream oStr;

    // SQL database name
    const StringMap_T::const_iterator itDBName = iStringMap.find ("db");
    assert (itDBName != iStringMap.end());
    const std::string& lDBName = itDBName->second;

    // SQL database user
    const StringMap_T::const_iterator itDBUser = iStringMap.find ("user");
    assert (itDBUser != iStringMap.end());
    const std::string& lDBUser = itDBUser->second;

    // SQL database password
    const StringMap_T::const_iterator itDBPasswd = iStringMap.find ("password");
    assert (itDBPasswd != iStringMap.end());
    const std::string& lDBPasswd = itDBPasswd->second;

    //
    oStr << "db=" << lDBName;
    if (lDBName != "mysql") {
      oStr << iDeploymentNumber;
    }

    //
    oStr << " user=" << lDBUser;

    //
    oStr << " password=" << lDBPasswd;
    
    return SQLDBConnectionString_T (oStr.str());
  }

  // //////////////////////////////////////////////////////////////////////
  std::string
  displayMySQLConnectionString (const StringMap_T& iStringMap,
                                const DeploymentNumber_T& iDeploymentNumber) {
    std::ostringstream oStr;

    for (StringMap_T::const_iterator itDBKV = iStringMap.begin();
         itDBKV != iStringMap.end(); ++itDBKV) {
      const std::string& lDBKey = itDBKV->first;
      const std::string& lDBValue = itDBKV->second;
      oStr << lDBKey << "=";
      oStr << lDBValue;
      if (lDBKey == "db" && lDBValue != "mysql"
          && iDeploymentNumber != DEFAULT_OPENTREP_DEPLOYMENT_NUMBER_SIZE) {
        oStr << iDeploymentNumber;
      }
      oStr << " ";
    }

    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string
  parseAndDisplayConnectionString (const DBType& iDBType,
                                   const std::string& iSQLDBConnStr,
                                   const DeploymentNumber_T& iDeploymentNumber) {
    std::ostringstream oStr;

    if (iDBType == DBType::NODB) {
      // Do nothing at this stage
      oStr << "";

    } else if (iDBType == DBType::SQLITE3) {
      oStr << iSQLDBConnStr << iDeploymentNumber;

    } else if (iDBType == DBType::MYSQL) {
      // Parse the connection string
      const SQLDBConnectionString_T lSQLDBConnStr (iSQLDBConnStr);
      const StringMap_T& lStrMap = parseMySQLConnectionString (lSQLDBConnStr);

      // Re-build the new connection string, taking into account the
      // deployment number/version
      const std::string& lNewSQLDBConnStr =
        displayMySQLConnectionString (lStrMap, iDeploymentNumber);
      oStr << lNewSQLDBConnStr;
    }

    //
    return oStr.str();
  }
}
