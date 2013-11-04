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
      COMBINATION = 0,
      XAPIAN_PCT,
      PAGE_RANK,
      PAX_NB,
      HEURISTIC,
      ENV_ID,
      CODE_FULL_MATCH,
      LAST_VALUE
    } EN_ScoreType;

    /**
     * Get the label as a string (e.g., "Xapian Percentage" or "Page Rank").
     */
    static const std::string& getLabel (const EN_ScoreType&);

    /**
     * Get the label as a string (e.g., "Xapian Percentage" or "Page Rank").
     */
    const std::string& getLongLabel() const;

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

    /**
     * State whether that score type is valid and is an individual one,
     * i.e., one that is neither COMBINATION nor LAST_VALUE.
     */
    bool isIndividualScore() const;

    /**
     * State whether that score type is valid and is an individual one,
     * i.e., one that is neither COMBINATION nor LAST_VALUE.
     */
    static bool isIndividualScore (const EN_ScoreType&);


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
