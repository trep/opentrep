#ifndef __OPENTREP_BOM_FILTER_HPP
#define __OPENTREP_BOM_FILTER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>

namespace OPENTREP {

  /**
   * @brief Class filtering out the words not suitable for indexing
   *        and/or searching, when part of greater strings. Hence, most of
   *        the methods take as parameter the "initial"/greater string.
   *
   *        For instance, words of length less than 3 (e.g., "de", "a", "san"),
   *        when part of greater strings (e.g., respectively, "rio de janeiro",
   *        "san francisco"), should not be indexed and searched for.
   */
  struct Filter {
  public:
    // /////////// Business support methods /////////
    /**
     * Trim all the non-relevant words from the given phrase.
     *
     * The following rules are applied to the right and left outer words,
     * iteratively until no more outer word can be stripped out:
     * <ul>
     *   <li>If the left or right outer word has no more than <iMinWordLength> letters
     *       (e.g., 'de', 'san'), it should be stripped out</li>
     *   <li>If the left or right outer word is part of the "black-list"
     *       (e.g., 'airport', 'intl', 'international'), it should be
     *       filtered out</li>
     * </ul>
     *
     * @param std::string& The phrase to be amended (e.g., 'de san francisco',
     *                     part of the 'aeroport de san francisco' global
     *                     phrase).
     * @param const NbOfLetters_T& The minimum length of the words (default is 4 letters).
     */
    static void trim (std::string& ioPhrase, const NbOfLetters_T& iMinWordLength = 4);

    /**
     * State whether or not to keep the given word, as opposed to filter out
     * a non-indexable/searchable word.
     *
     * The following rules are applied in sequence (if a rule applies, then
     * the method returns, and the other rules are not processed/checked):
     * <ul>
     *   <li>When the word is equal to the phrase (e.g., 'san'), it should be
     *       kept (not filtered out), as it is obviously here intentionally</li>
     *   <li>If the word has no more than 3 letters (e.g., 'de', 'san'),
     *       it should be filtered out</li>
     *   <li>If the word is part of the "black-list" (e.g., 'airport', 'intl',
     *       'international'), it should be filtered out</li>
     * </ul>
     *
     * @param const std::string& The initial phrase (e.g.,
     *                           'san francisco airport').
     * @param const std::string& The word on which a decision has to be made
     * @return bool Whether or not the word should be kept / filtered out
     */
    static bool shouldKeep (const std::string& iPhrase,
                            const std::string& iWord);


  private:
    // //////////////// Constructors and Destructors /////////////
    /**
     * Default constructor.
     */
    Filter();

    /**
     * Default copy constructor.
     */
    Filter (const Filter&);

    /**
     * Default destructor.
     */
    ~Filter();
  };

}
#endif // __OPENTREP_BOM_FILTER_HPP
