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
// Boost Arry
#include <boost/array.hpp>

namespace OPENTREP {

  // ///////// Exceptions ///////////
  class RootException : public std::exception {
  };

  class NonInitialisedServiceException : public RootException {
  };

  class MultipleRowsForASingleDocIDException : public RootException {
  };

  class LanguageCodeNotDefinedInNameTableException : public RootException {
  };

  class MemoryAllocationException : public RootException {
  };

  class ObjectNotFoundException : public RootException {
  };

  class XapianException : public RootException {
  };
  
  class DocumentNotFoundException : public XapianException {
  };

  class XapianDatabaseFailureException : public XapianException {
  };

  class XapianTravelDatabaseEmptyException : public XapianException {
  };

  class XapianTravelDatabaseWrongPathnameException : public XapianException {
  };

  class XapianTravelDatabaseNotInSyncWithSQLDatabaseException : public XapianException {
  };

  class SQLDatabaseException : public RootException {
  };

  class SQLDatabaseConnectionImpossibleException : public SQLDatabaseException {
  };

  class BuildIndexException : public RootException {
  };

  class InterpreteUseCaseException : public RootException {
  };

  class InterpreteTravelRequestException : public InterpreteUseCaseException {
  };

  class TravelRequestEmptyException : public InterpreteUseCaseException {
  };


  // /////////////// Log /////////////
  /** Level of logs. */
  namespace LOG {
    typedef enum {
      CRITICAL = 0,
      ERROR,
      NOTIFICATION,
      WARNING,
      DEBUG,
      VERBOSE,
      LAST_VALUE
    } EN_LogLevel;
  }


  // //////// Type definitions /////////
  /** Xapian database name, corresponding to the (potentially relative)
      directory name (on the filesystem) where Xapian stores its index. */
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

