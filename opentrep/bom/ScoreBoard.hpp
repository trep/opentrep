#ifndef __OPENTREP_BOM_SCOREBOARD_HPP
#define __OPENTREP_BOM_SCOREBOARD_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <list>
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
    typedef std::map<ScoreType::EN_ScoreType, Score_T> ScoreList_T;


  public:
    // ////////////////// Getters ////////////////
    /**
     * Get the list of scores.
     */
    const ScoreList_T& getScoreList() const {
     return _scoreList;
    }


  public:
    // //////////////////// Setters //////////////////


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
    /** Default constructor. */
    // ScoreBoard();

    /** Default copy constructor. */
    // ScoreBoard (const ScoreBoard&);

    /**
     * Default destructor.
     */
    ~ScoreBoard ();

    
  private:
    // ///////////////// Attributes //////////////////
    /**
     * List of scores.
     */
    ScoreList_T _scoreList;
  };

}
#endif // __OPENTREP_BOM_SCOREBOARD_HPP
