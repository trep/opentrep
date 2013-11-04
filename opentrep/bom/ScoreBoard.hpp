#ifndef __OPENTREP_BOM_SCOREBOARD_HPP
#define __OPENTREP_BOM_SCOREBOARD_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/basic/ScoreType.hpp>
#include <opentrep/basic/StructAbstract.hpp>
// Xapian
#include <xapian.h>

namespace OPENTREP {

  /**
   * @brief Structure holding a board for all the types of
   *        score/matching having been performed.
   */
  struct ScoreBoard : public StructAbstract {
  public:
    // //////////////// Type definitions /////////////////
    /**
     * (STL) map of scores.
     */
    typedef std::map<ScoreType::EN_ScoreType, Score_T> ScoreMap_T;


  public:
    // ////////////////// Getters ////////////////
    /**
     * Get the query string.
     */
    const TravelQuery_T& getQueryString() const {
      return _queryString;
    }

    /**
     * Get the map of scores.
     */
    const ScoreMap_T& getScoreMap() const {
     return _scoreMap;
    }

    /**
     * Get the score for the given type. If no score value has
     * already been stored for that type, return 0.
     */
    Score_T getScore (const ScoreType&) const;

    /**
     * Get the combined weight, if existing (0 otherwise).
     */
    Score_T getCombinedWeight() const {
      return getScore (ScoreType::COMBINATION);
    }


  public:
    // //////////////////// Setters //////////////////
    /**
     * Set the query string.
     */
    void setQueryString (const TravelQuery_T& iQueryString) {
      _queryString = iQueryString;
    }

    /**
     * Set the score for the given type. If no score value has
     * already been stored for that type, create it.
     *
     * A trick is used to decrease the overall percentage of word combinations,
     * when compared to the whole string. For instance, {"san francisco"}
     * will have a percentage of 99.999%, compared to {"san", "francisco"}
     * which will have a percentage of 99.998%.
     *
     * A few rules are applied:
     * <ul>
     *   <li>For matching with IATA/ICAO codes:
     *     <ul>
     *       <li>When the query string fully matches with a IATA/ICAO code,
     *           the matching percentage is set to 100% (1.00).</li>
     *       <li>Otherwise, it is set to 99.999% (0.99999).</li>
     *     </ul></li>
     *   <li>The Xapian matching weight is registered as is.</li>
     *   <li>For envelope ID:
     *     <ul>
     *       <li>When null, the score is taken as the full matching,
     *           i.e., 100% (1.00). There is no need to take 99.999%,
     *           as the Xapian matching value above already plays that role.</li>
     *       <li>When not null, the score is taken as the minimal default,
     *           i.e., 0.10% (0.001).</li>
     *     </ul></li>
     * </ul>
     */
    void setScore (const ScoreType&, const Score_T&);

    /**
     * Set the combined weight.
     */
    void setCombinedWeight (const Score_T& iScore) {
      setScore (ScoreType::COMBINATION, iScore);
    }

    /**
     * Calculate the combination of the weights for all the score types,
     * resulting from the full-text matching process, PageRank, user input,
     * etc.
     */
    Percentage_T calculateCombinedWeight();


  public:
    // //////////////// Business methods ////////////////


  public:
    // /////////// Display support methods /////////
    /**
     * Dump the structure into an output stream.
     *
     * @param ostream& the output stream.
     */
    void toStream (std::ostream&) const;

    /**
     * Read a structure from an input stream.
     *
     * @param istream& the input stream.
     */
    void fromStream (std::istream&);

    /**
     * Display the key of the structure.
     *
     * @return std::string Dump of the structure.
     */
    std::string describeKey() const;

    /**
     * Display of the structure.
     *
     * @return std::string Dump of the structure.
     */
    std::string describe() const;


  public:
    // //////////////// Constructors and Destructors /////////////
    /**
     * Main constructor.
     */
    ScoreBoard (const TravelQuery_T&, const ScoreType&, const Score_T&);

    /**
     * Default constructor.
     */
    ScoreBoard (const TravelQuery_T&);

    /**
     * Copy constructor.
     */
    ScoreBoard (const ScoreBoard&);

    /**
     * Default destructor.
     */
    ~ScoreBoard();

    
  private:
    // ///////////////// Attributes //////////////////
    /**
     * Query string having generated the set of documents.
     */
    TravelQuery_T _queryString;

    /**
     * Map of scores.
     */
    ScoreMap_T _scoreMap;
  };

}
#endif // __OPENTREP_BOM_SCOREBOARD_HPP
