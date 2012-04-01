#ifndef __OPENTREP_OPENTREP_EXCEPTIONS_HPP
#define __OPENTREP_OPENTREP_EXCEPTIONS_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <exception>

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
  
  /**
   * OpenTREP service not initialised.
   */
  class NonInitialisedServiceException : public RootException {
  public:
    /**
     * Constructor.
     */
    NonInitialisedServiceException (const std::string& iWhat)
      : RootException (iWhat) {}
  };

  /**
   * Multiple rows for a single Xapian document ID.
   */
  class MultipleRowsForASingleDocIDException : public RootException {
  public:
    /**
     * Constructor.
     */
    MultipleRowsForASingleDocIDException (const std::string& iWhat)
      : RootException (iWhat) {}
  };

  /**
   * Language code is not recognised.
   */
  class LanguageCodeNotDefinedInNameTableException : public RootException {
  public:
    /**
     * Constructor.
     */
    LanguageCodeNotDefinedInNameTableException (const std::string& iWhat)
      : RootException (iWhat) {}
  };

  /**
   * Memory allocation issue.
   */
  class MemoryAllocationException : public RootException {
  public:
    /**
     * Constructor.
     */
    MemoryAllocationException (const std::string& iWhat)
      : RootException (iWhat) {}
  };

  /**
   * The object cannot be retrieved.
   */
  class ObjectNotFoundException : public RootException {
  public:
    /**
     * Constructor.
     */
    ObjectNotFoundException (const std::string& iWhat)
      : RootException (iWhat) {}
  };

  /**
   * Parser.
   */
  class ParserException : public RootException { 
  public:
    /**
     * Constructor.
     */
    ParserException (const std::string& iWhat) : RootException (iWhat) {}
  };
  
  /**
   * Code conversion.
   */
  class CodeConversionException : public ParserException {  
  public:
    /**
     * Constructor.
     */
    CodeConversionException (const std::string& iWhat)
      : ParserException (iWhat) {}
  };
  
  /**
   * Code duplication.
   */
  class CodeDuplicationException : public ParserException { 
  public:
    /**
     * Constructor.
     */
    CodeDuplicationException (const std::string& iWhat)
      : ParserException (iWhat) {}
  };

  /**
   * Xapian root exception.
   */
  class XapianException : public RootException {
  public:
    /**
     * Constructor.
     */
    XapianException (const std::string& iWhat) : RootException (iWhat) {}
  };
  
  /**
   * The document can not be retrieved.
   */
  class DocumentNotFoundException : public XapianException {
  public:
    /**
     * Constructor.
     */
    DocumentNotFoundException (const std::string& iWhat)
      : XapianException (iWhat) {}
  };

  /**
   * Xapian database.
   */
  class XapianDatabaseFailureException : public XapianException {
  public:
    /**
     * Constructor.
     */
    XapianDatabaseFailureException (const std::string& iWhat)
      : XapianException (iWhat) {}
  };

  /** 
   * Xapian travel database empty.
   */
  class XapianTravelDatabaseEmptyException : public XapianException {
  public:
    /**
     * Constructor.
     */
    XapianTravelDatabaseEmptyException (const std::string& iWhat)
      : XapianException (iWhat) {}
  };

  /** 
   * The Xapian travel database file-path has not been set correctly.
   */
  class XapianTravelDatabaseWrongPathnameException : public XapianException {
  public:
    /**
     * Constructor.
     */
    XapianTravelDatabaseWrongPathnameException (const std::string& iWhat)
      : XapianException (iWhat) {}
  };

  /** 
   * Xapian travel database not synchronised with the MySQL database.
   */
  class XapianTravelDatabaseNotInSyncWithSQLDatabaseException : public XapianException {
  public:
    /**
     * Constructor.
     */
    XapianTravelDatabaseNotInSyncWithSQLDatabaseException (const std::string& iWhat)
      : XapianException (iWhat) {}
  };

  /**
   * MySQL Database root exception.
   */
  class SQLDatabaseException : public RootException {
  public:
    /**
     * Constructor.
     */
    SQLDatabaseException (const std::string& iWhat) : RootException (iWhat) {}
  };

  /**
   * The connection fails with the MySQL database.
   */
  class SQLDatabaseConnectionImpossibleException : public SQLDatabaseException {
  public:
    /**
     * Constructor.
     */
    SQLDatabaseConnectionImpossibleException (const std::string& iWhat)
      : SQLDatabaseException (iWhat) {}
  };

  /**
   * Index issue.
   */
  class BuildIndexException : public RootException {
  public:
    /**
     * Constructor.
     */
    BuildIndexException (const std::string& iWhat) : RootException (iWhat) {}
  };

  /**
   * Interpreter issue.
   */
  class InterpreterUseCaseException : public RootException {
  public:
    /**
     * Constructor.
     */
    InterpreterUseCaseException (const std::string& iWhat)
      : RootException (iWhat) {}
  };

  /**
   * Travel request interpreter issue.
   */
  class InterpreteTravelRequestException : public InterpreterUseCaseException {
  public:
    /**
     * Constructor.
     */
    InterpreteTravelRequestException (const std::string& iWhat)
      : InterpreterUseCaseException (iWhat) {}
  };

  /**
   * The travel request is empty.
   */
  class TravelRequestEmptyException : public InterpreterUseCaseException {
  public:
    /**
     * Constructor.
     */
    TravelRequestEmptyException (const std::string& iWhat)
      : InterpreterUseCaseException (iWhat) {}
  };

}
#endif // __OPENTREP_OPENTREP_EXCEPTIONS_HPP
