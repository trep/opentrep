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
   * Unicode failure.
   */
  class UnicodeException : public RootException {
  public:
    /**
     * Constructor.
     */
    UnicodeException (const std::string& iWhat) : RootException (iWhat) {}
  };

  /**
   * Unicode transliterator failure.
   */
  class UnicodeTransliteratorCreationException : public UnicodeException {
  public:
    /**
     * Constructor.
     */
    UnicodeTransliteratorCreationException (const std::string& iWhat)
      : UnicodeException (iWhat) {}
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
    MemoryAllocationException (const std::string& iWhat) : RootException (iWhat) {}
  };

  /**
   * The object cannot be retrieved.
   */
  class ObjectNotFoundException : public RootException {
  public:
    /**
     * Constructor.
     */
    ObjectNotFoundException (const std::string& iWhat) : RootException (iWhat) {}
  };

  /**
   * File.
   */
  class FileException : public RootException { 
  public:
    /**
     * Constructor.
     */
    FileException (const std::string& iWhat) : RootException (iWhat) {}
  };
  
  /**
   * File not found.
   */
  class FileNotFoundException : public FileException { 
  public:
    /**
     * Constructor.
     */
    FileNotFoundException (const std::string& iWhat) : FileException (iWhat) {}
  };
  
  /**
   * Unknown file extension.
   */
  class FileExtensionUnknownException : public FileException { 
  public:
    /**
     * Constructor.
     */
    FileExtensionUnknownException (const std::string& iWhat)
      : FileException (iWhat) {}
  };
  
  /**
   * Uncompressing file error.
   */
  class FileUncompressingErrorException : public FileException { 
  public:
    /**
     * Constructor.
     */
    FileUncompressingErrorException (const std::string& iWhat)
      : FileException (iWhat) {}
  };
  
  /**
   * Serialization / Deserialization.
   */
  class SerDeException : public RootException { 
  public:
    /**
     * Constructor.
     */
    SerDeException (const std::string& iWhat) : RootException (iWhat) {}
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
   * POR file parsing exception.
   */
  class PorFileParsingException : public ParserException {  
  public:
    /**
     * Constructor.
     */
    PorFileParsingException (const std::string& iWhat)
      : ParserException (iWhat) {}
  };
  
  /**
   * PR file parsing exception.
   */
  class PRFileParsingException : public ParserException {  
  public:
    /**
     * Constructor.
     */
    PRFileParsingException (const std::string& iWhat)
      : ParserException (iWhat) {}
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
   * The Xapian travel database/index file-path has not been set correctly.
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
   * There is best matching ResultHolder object.
   */
  class NoBestMatchingResultHolderException : public RootException {
  public:
    /**
     * Constructor.
     */
    NoBestMatchingResultHolderException (const std::string& iWhat)
      : RootException (iWhat) {}
  };

  /**
   * SQL Database root exception.
   */
  class SQLDatabaseException : public RootException {
  public:
    /**
     * Constructor.
     */
    SQLDatabaseException (const std::string& iWhat) : RootException (iWhat) {}
  };

  /**
   * The parsing of the connection string fails for the SQL database.
   */
  class SQLDatabaseConnectionStringParsingException
    : public SQLDatabaseException {
  public:
    /**
     * Constructor.
     */
    SQLDatabaseConnectionStringParsingException (const std::string& iWhat)
      : SQLDatabaseException (iWhat) {}
  };

  /**
   * The connection fails with the SQL database.
   */
  class SQLDatabaseImpossibleConnectionException : public SQLDatabaseException {
  public:
    /**
     * Constructor.
     */
    SQLDatabaseImpossibleConnectionException (const std::string& iWhat)
      : SQLDatabaseException (iWhat) {}
  };

  /**
   * The SQL database connection cannot be closed/released.
   */
  class SQLDatabaseConnectionReleaseException : public SQLDatabaseException {
  public:
    /**
     * Constructor.
     */
    SQLDatabaseConnectionReleaseException (const std::string& iWhat)
      : SQLDatabaseException (iWhat) {}
  };

  /**
   * The SQLite3 database file cannot be created.
   */
  class SQLDatabaseFileCannotBeCreatedException : public SQLDatabaseException {
  public:
    /**
     * Constructor.
     */
    SQLDatabaseFileCannotBeCreatedException (const std::string& iWhat)
      : SQLDatabaseException (iWhat) {}
  };

  /**
   * The given entry cannot be found in the database.
   */
  class SQLDatabaseElementNotFoundException : public SQLDatabaseException {
  public:
    /**
     * Constructor.
     */
    SQLDatabaseElementNotFoundException (const std::string& iWhat)
      : SQLDatabaseException (iWhat) {}
  };

  /**
   * SQL database user creation issue.
   */
  class SQLDatabaseUserCreationException : public SQLDatabaseException {
  public:
    /**
     * Constructor.
     */
    SQLDatabaseUserCreationException (const std::string& iWhat)
      : SQLDatabaseException (iWhat) {}
  };

  /**
   * SQL database table creation issue.
   */
  class SQLDatabaseTableCreationException : public SQLDatabaseException {
  public:
    /**
     * Constructor.
     */
    SQLDatabaseTableCreationException (const std::string& iWhat)
      : SQLDatabaseException (iWhat) {}
  };

  /**
   * SQL database index creation issue.
   */
  class SQLDatabaseIndexCreationException : public SQLDatabaseException {
  public:
    /**
     * Constructor.
     */
    SQLDatabaseIndexCreationException (const std::string& iWhat)
      : SQLDatabaseException (iWhat) {}
  };

  /**
   * Overall indexing issue.
   */
  class BuildIndexException : public RootException {
  public:
    /**
     * Constructor.
     */
    BuildIndexException (const std::string& iWhat) : RootException (iWhat) {}
  };

  /**
   * Overall interpreter issue.
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
