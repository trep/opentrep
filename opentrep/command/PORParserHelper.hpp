#ifndef __OPENTREP_CMD_PORPARSERHELPER_HPP
#define __OPENTREP_CMD_PORPARSERHELPER_HPP

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

  namespace PorParserHelper {
    
    // ///////////////////////////////////////////////////////////////////
    //  Semantic actions
    // ////////////////////////////////////////////////////
    ///////////////
    /** Generic Semantic Action (Actor / Functor) for the Por Parser. */
    struct ParserSemanticAction {
      /** Actor Constructor. */
      ParserSemanticAction (Location&);
      /** Actor Context. */
      Location& _location;
    };

    /** Store the parsed por Id. */
    struct storeGeonamesId : public ParserSemanticAction {
      /** Actor Constructor. */
      storeGeonamesId (Location&);
      /** Actor Function (functor). */
      void operator() (unsigned int,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed origin. */
    struct storeIataCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeIataCode (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<char>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed customer trip type. */
    struct storePORType : public ParserSemanticAction {
      /** Actor Constructor. */
      storePORType (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<char>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };


    /** Store the parsed start of the date range. */
    struct storeModDate : public ParserSemanticAction {
      /** Actor Constructor. */
      storeModDate (Location&);
      /** Actor Function (functor). */
      void operator() (boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the cabin code. */
    struct storeCabinCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeCabinCode  (Location&);
      /** Actor Function (functor). */
      void operator() (char,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed advance purchase days. */
    struct storeAdvancePurchase : public ParserSemanticAction {
      /** Actor Constructor. */
      storeAdvancePurchase (Location&);
      /** Actor Function (functor). */
      void operator() (unsigned int,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed latitude value. */
    struct storeLatitude : public ParserSemanticAction {
      /** Actor Constructor. */
      storeLatitude (Location&);
      /** Actor Function (functor). */
      void operator() (double,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const; 
    };
    
    /** Mark the end of the por-rule parsing. */
    struct doEndPor : public ParserSemanticAction {
      /** Actor Constructor. */
      doEndPor (Location&);
      /** Actor Function (functor). */
      void operator() (boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };
  
  }
  
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
  class PORFileParser {
  public:
    /**
     * Constructor.
     */
    PORFileParser (const FilePath_T& iFilename);

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
     * File-name of the CSV-formatted por input file.
     */
    FilePath_T _filename;

    /**
     * POR Structure.
     */
    Location _location;
  };
    
}
#endif // __OPENTREP_CMD_PORPARSERHELPER_HPP
