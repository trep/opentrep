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

namespace OPENTREP {

  // ///////// Exceptions ///////////
  /**
   * @brief Root of the OpenTREP exceptions.
   *
   * All the OpenTREP exceptions inherit from that root, allowing to
   * catch them and to spot them easily when arising in code wrapping
   * the stdair library.
   */
  class RootException : public std::exception { 
  public:
    /**
     * Main Constructor.
     */
    RootException (const std::string& iWhat) : _what (iWhat) {}
    /**
     * Default constructor.
     */
    RootException() : _what ("No further details") {}
    
    /**
     * Destructor.
     */
    virtual ~RootException() throw() {}
    
    /**
     * Give the details of the exception.
     */
    const char* what() const throw() {
      return _what.c_str();
    }
    
  protected:
    /**
     * Details for the exception.
     */
    std::string _what;
  };
  
  /** OpenTREP service not initialised. */
  class NonInitialisedServiceException : public RootException {
  public:
    /** Constructor. */
    NonInitialisedServiceException (const std::string& iWhat)
      : RootException (iWhat) {}
  };

  /** Multiple rows for a single Xapian document ID. */
  class MultipleRowsForASingleDocIDException : public RootException {
  };

  /** Language code is not recognised. */
  class LanguageCodeNotDefinedInNameTableException : public RootException {
  };

  /** Memory allocation issue. */
  class MemoryAllocationException : public RootException {
  };

  /** The object cannot be retrieved. */
  class ObjectNotFoundException : public RootException {
  };

  /** Parser. */
  class ParserException : public RootException { 
  public:
    /** Constructor. */
    ParserException (const std::string& iWhat) : RootException (iWhat) {}
  };
  
  /** Code conversion. */
  class CodeConversionException : public ParserException {  
  public:
    /** Constructor. */
    CodeConversionException (const std::string& iWhat)
      : ParserException (iWhat) {}
  };
  
  /** Code duplication. */
  class CodeDuplicationException : public ParserException { 
  public:
    /** Constructor. */
    CodeDuplicationException (const std::string& iWhat)
      : ParserException(iWhat) {}
  };

  /** Xapian root exception. */
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

  /** Database root exception. */
  class SQLDatabaseException : public RootException {
  };

  class SQLDatabaseConnectionImpossibleException : public SQLDatabaseException {
  };

  /** Index issue. */
  class BuildIndexException : public RootException {
  };

  class InterpreteUseCaseException : public RootException {
  };

  class InterpreteTravelRequestException : public InterpreteUseCaseException {
  };

  class TravelRequestEmptyException : public InterpreteUseCaseException {
  };


  // /////////////// Log /////////////
  /** 
   * Level of logs. 
   */
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
