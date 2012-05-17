#ifndef __OPENTREP_BAS_SCORETYPE_HPP
#define __OPENTREP_BAS_SCORETYPE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// OpenTrep
#include <opentrep/basic/StructAbstract.hpp>

namespace OPENTREP {

  /**
   * @brief Enumeration of score types.
   *
   * Each type corresponds to a specific kind of matching:
   * <ul>
   *   <li>Xapian full-text matching</li>
   *   <li>Page-Rank from the schedule</li>
   *   <li>Number of passengers registered for that airport/POR (point of
   *       reference)</li>
   * </ul>
   */
  struct ScoreType : public StructAbstract {
  public:
    typedef enum { 
      XAPIAN_PCT = 0,
      PAGE_RANK,
      PAX_NB,
      LAST_VALUE
    } EN_ScoreType;

    /**
     * Get the label as a string (e.g., "Xapian Percentage" or "Page Rank").
     */
    static const std::string& getLabel (const EN_ScoreType&);

    /**
     * Get the label as a single char (e.g., 'X' or 'R').
     */
    static char getTypeLabel (const EN_ScoreType&);

    /**
     * Get the label as a single char (e.g., 'X' or 'R').
     */
    static std::string getTypeLabelAsString (const EN_ScoreType&);

    /**
     * List the labels.
     */
    static std::string describeLabels();

    /**
     * Get the enumerated value.
     */
    EN_ScoreType getType() const;

    /**
     * Get the enumerated value as a short string (e.g., 'X' or 'R').
     */
    std::string getTypeAsString() const;

    /**
     * Give a description of the structure (e.g., "Xapian Percentage"
     * or "Page Rank").
     */
    std::string describe() const;

  public:
    /**
     * Comparison operator.
     */
    bool operator== (const EN_ScoreType&) const;

  public:
    /**
     * Constructor.
     */
    ScoreType (const EN_ScoreType&);

    /**
     * Constructor.
     */
    ScoreType (const char iType);


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
     * Score type.
     */
    EN_ScoreType _type;
  };

}
#endif // __OPENTREP_BAS_SCORETYPE_HPP
