#ifndef __OPENTREP_BAS_UTILITIES_HPP
#define __OPENTREP_BAS_UTILITIES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <vector>
#include <map>
#include <locale>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/basic/BasConst_OPENTREP_Service.hpp>

namespace OPENTREP {

  // Forward declarations
  struct DBType;
  
  /**
   * Split a string into a list of tokens.
   */
  void tokeniseStringIntoWordList (const std::string& iPhrase, WordList_T&);

  /**
   * Create a string from a list of words.
   *
   * The string is made of the words up until a given index. The default
   * value of that index (0) means that the string will be made by all
   * the words of the list.
   *
   * If the flag is set to false, the string is made of the words from
   * the given index up until the end of the list.
   */
  std::string createStringFromWordList (const WordList_T&,
                                        const NbOfWords_T iSplitIdx = 0,
                                        const bool iFromBeginningFlag = true);

  /**
   * Map for character strings
   */
  typedef std::map<const std::string, std::string> StringMap_T;

  /**
   * Parse the connection string.
   *
   * Typically, the SQL connection string is like
   * 'db=trep_trep user=trep password=trep'.
   *
   * <ul>
   *   <li>First, split the connection string by the space character (' '),
   *       which leads to key-value pairs.</li>
   *   <li>Then, split each of those key-value pairs by the equal sign ('='),
   *       which leads to the key and the corresponding value.</li>
   * </ul>
   *
   * @param const SQLDBConnectionString_T& Connection string for MySQL/MariaDB
   * @return StringMap_T Connection details to the MySQL/MariaDB
   */
  StringMap_T parseMySQLConnectionString (const SQLDBConnectionString_T&);
  
  /**
   * Recompose the connection string.
   *
   * For instance, 'db=trep_trep0 user=trep password=trep'
   *
   * @param const StringMap_T& Connection details to the MySQL/MariaDB
   * @param const const DeploymentNumber_T& Deployment number
   * @return SQLDBConnectionString_T Connection string for MySQL/MariaDB
   */
  SQLDBConnectionString_T buildMySQLConnectionString (const StringMap_T&,
                                                      const DeploymentNumber_T&);

  /**
   *
   * @param const StringMap_T& Connection details to the MySQL/MariaDB
   * @param const const DeploymentNumber_T& Deployment number
   * @return std::string String displaying the connection string
   */
  std::string displayMySQLConnectionString (const StringMap_T&,
                                            const DeploymentNumber_T& iDN = DEFAULT_OPENTREP_DEPLOYMENT_NUMBER_SIZE);

  /**
   * Parse the connection string, and re-build it taking into account
   * the deployment number/version.
   *
   * @param const SQLDBConnectionString_T& Connection string for MySQL/MariaDB
   * @param const const DeploymentNumber_T& Deployment number
   * @return std::string String displaying the connection string
   */
  std::string parseAndDisplayConnectionString (const DBType&,
                                               const std::string& iSQLDBConnStr,
                                               const DeploymentNumber_T&);
  
  /**
   * Class implementing the separators for display of integers (rather than
   * relying on the locale).
   * See command/IndexBuilder::buildSearchIndex() for an example.
   */
  class NumSep : public std::numpunct<char> {
  protected:
    /**
     * Separator character for the thousands
     */
    virtual char do_thousands_sep() const {
      return ',';
    }
    
    /**
     * Group numbers together
     */
    virtual std::string do_grouping() const {
      return "\03";
    }
  };


}
#endif // __OPENTREP_BAS_UTILITIES_HPP
