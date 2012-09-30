#ifndef __OPENTREP_CMD_PRPARSERHELPER_HPP
#define __OPENTREP_CMD_PRPARSERHELPER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// Boost
#include <boost/spirit/include/qi.hpp>
// Opentrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/Location.hpp>

namespace OPENTREP {

  namespace PRParserHelper {

    /** Unicode character. */
    //typedef char uchar_t;
    typedef boost::uint32_t uchar_t;


    // ///////////////////////////////////////////////////////////////////
    //  Semantic actions
    // ////////////////////////////////////////////////////
    ///////////////
    /** Generic Semantic Action (Actor / Functor) for the PR Parser. */
    struct ParserSemanticAction {
      /** Actor Constructor. */
      ParserSemanticAction (Location&);
      /** Actor Context. */
      Location& _location;
    };

    /** Store the parsed IATA code. */
    struct storeIataCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeIataCode (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<uchar_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed page_rank value. */
    struct storePageRank : public ParserSemanticAction {
      /** Actor Constructor. */
      storePageRank (Location&);
      /** Actor Function (functor). */
      void operator() (double,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const; 
    };

    /** Store the parsed PR type. */
    struct storePRType : public ParserSemanticAction {
      /** Actor Constructor. */
      storePRType (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<uchar_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Mark the end of the pr-rule parsing. */
    struct doEndPR : public ParserSemanticAction {
      /** Actor Constructor. */
      doEndPR (Location&);
      /** Actor Function (functor). */
      void operator() (boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

  }
  

  /////////////////////////////////////////////////////////////////////////
  //
  //  Entry class for the string parser
  //
  /////////////////////////////////////////////////////////////////////////
  /**
   * Class wrapping the initialisation and entry point of the parser.
   *
   * The seemingly redundancy is used to force the instantiation of
   * the actual parser, which is a templatised Boost Spirit grammar.
   * Hence, the actual parser is instantiated within that class object code.
   */
  class PRStringParser {
  public:
    /**
     * Constructor.
     */
    PRStringParser (const std::string& iString);

    /**
     * Destructor.
     */
    ~PRStringParser();

    /**
     * Parse the input string and generate the Location structures.
     */
    const Location& generateLocation();
      
  private:
    /**
     * Initialise.
     */
    void init();
      
  private:
    // Attributes
    /**
     * String to be parsed.
     */
    std::string _string;

    /**
     * PR Structure.
     */
    Location _location;
  };
    

  /////////////////////////////////////////////////////////////////////////
  //
  //  Entry class for the file parser
  //
  /////////////////////////////////////////////////////////////////////////
  /**
   * Class wrapping the initialisation and entry point of the parser.
   *
   * The seemingly redundancy is used to force the instantiation of
   * the actual parser, which is a templatised Boost Spirit grammar.
   * Hence, the actual parser is instantiated within that class object code.
   */
  class PRFileParser {
  public:
    /**
     * Constructor.
     */
    PRFileParser (const PRFilePath_T& iFilename);

    /**
     * Parse the input file and generate the Location structures.
     */
    void generateLocations();
      
  private:
    /**
     * Initialise.
     */
    void init();
      
  private:
    // Attributes
    /**
     * File-name of the CSV-formatted pr input file.
     */
    PRFilePath_T _filename;

    /**
     * PR Structure.
     */
    Location _location;
  };
    
}
#endif // __OPENTREP_CMD_PRPARSERHELPER_HPP
