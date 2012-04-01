#ifndef __OPENTREP_OPENTREP_TYPES_HPP
#define __OPENTREP_OPENTREP_TYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <exception>
#include <string>
#include <list>
#include <map>
// Boost Array
#include <boost/array.hpp>
// OpenTrep
#include <opentrep/OPENTREP_exceptions.hpp>
#include <opentrep/OPENTREP_log.hpp>

namespace OPENTREP {

  // //////// Type definitions /////////
  /** 
   * Xapian database name, corresponding to the (potentially relative)
   * directory name (on the filesystem) where Xapian stores its index.
   */
  typedef std::string TravelDatabaseName_T;

  /** Xapian document ID. */
  typedef int XapianDocID_T;

  /** Xapian percentage. */
  typedef unsigned int MatchingPercentage_T;

  /** Travel search query. */
  typedef std::string TravelQuery_T;

  /** Number of entries in the Xapian database. */
  typedef unsigned int NbOfDBEntries_T;
  
  /** Word, which is the atomic element of a query string. */
  typedef std::string Word_T;

  /** List of words. */
  typedef std::list<Word_T> WordList_T;

  /** Number of letters in a word or phrase. */
  typedef unsigned int NbOfLetters_T;
  
  /** Number of matching documents. */
  typedef unsigned short NbOfMatches_T;
  
  /** Number of errors. */
  typedef unsigned short NbOfErrors_T;
  
  /** Number of (distance) errors allowed for a given number of letters. */
  typedef std::map<NbOfLetters_T, NbOfErrors_T> DistanceErrorScale_T;

  /** Number of (distance) errors allowed for a given number of letters. */
  typedef boost::array<NbOfLetters_T, 5> DistanceErrorScaleArray_T;
}
#endif // __OPENTREP_OPENTREP_TYPES_HPP
