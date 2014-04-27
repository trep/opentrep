#ifndef __OPENTREP_DBTYPE_HPP
#define __OPENTREP_DBTYPE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>

namespace OPENTREP {

  /**
   * @brief Enumeration of database types.
   */
  struct DBType {
  public:
    typedef enum {
      NODB = 0,
      SQLITE3,
      MYSQL,
      LAST_VALUE
    } EN_DBType;

    /**
     * Get the label as a string (e.g., "NoDB", "SQLite3", "MySQL/MariaDB").
     */
    static const std::string& getLabel (const EN_DBType&);

    /**
     * Get the type value from parsing a single char (e.g., 'N', 'S', 'M')
     */
    static EN_DBType getType (const char);

    /**
     * Get the label as a single char (e.g., 'N', 'S', 'M')
     */
    static char getTypeLabel (const EN_DBType&);

    /**
     * Get the label as a string of a single char (e.g., 'N', 'S', 'M')
     */
    static std::string getTypeLabelAsString (const EN_DBType&);

    /**
     * List the labels.
     */
    static std::string describeLabels();

    /**
     * Get the enumerated value.
     */
    EN_DBType getType() const;

    /**
     * Get the enumerated value as a short string (e.g., "N", "S", "M")
     */
    char getTypeAsChar() const;
    
    /**
     * Get the enumerated value as a short string (e.g., "N", "S", "M")
     */
    std::string getTypeAsString() const;
    
    /**
     * Give a description of the structure (e.g., "NoDB", "SQLite3",
     * "MySQL/MariaDB").
     */
    const std::string describe() const;

  public:
    /**
     * Comparison operators.
     */
    bool operator== (const EN_DBType&) const;
    bool operator== (const DBType&) const;
    
  public:
    /**
     * Main constructor.
     */
    DBType (const EN_DBType&);
    /**
     * Alternative constructor.
     */
    DBType (const char iType);
    /**
     * Alternative constructor.
     */
    DBType (const std::string& iType);
    /**
     * Default copy constructor.
     */
    DBType (const DBType&);

  private:
    /**
     * Default constructor.
     */
    DBType();


  private:
    /**
     * String version of the enumeration.
     */
    static const std::string _labels[LAST_VALUE];
    /**
     * Type version of the enumeration.
     */
    static const char _typeLabels[LAST_VALUE];

  private:
    // //////// Attributes /////////
    /**
     * Location type.
     */
    EN_DBType _type;
  };

}
#endif // __OPENTREP_DBTYPE_HPP
