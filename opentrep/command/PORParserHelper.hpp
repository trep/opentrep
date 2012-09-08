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

    /** Store the parsed Geonames ID. */
    struct storeGeonamesID : public ParserSemanticAction {
      /** Actor Constructor. */
      storeGeonamesID (Location&);
      /** Actor Function (functor). */
      void operator() (unsigned int,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed IATA code. */
    struct storeIataCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeIataCode (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<char>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed ICAO code. */
    struct storeIcaoCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeIcaoCode (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<char>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed POR type. */
    struct storePORType : public ParserSemanticAction {
      /** Actor Constructor. */
      storePORType (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<char>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };


    /** Store the parsed last modification date. */
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

    /** Store the parsed population. */
    struct storePopulation : public ParserSemanticAction {
      /** Actor Constructor. */
      storePopulation (Location&);
      /** Actor Function (functor). */
      void operator() (unsigned int,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed elevation. */
    struct storeElevation : public ParserSemanticAction {
      /** Actor Constructor. */
      storeElevation (Location&);
      /** Actor Function (functor). */
      void operator() (int,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed GTopo30. */
    struct storeGTopo30 : public ParserSemanticAction {
      /** Actor Constructor. */
      storeGTopo30 (Location&);
      /** Actor Function (functor). */
      void operator() (int,
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
    
    /** Store the parsed longitude value. */
    struct storeLongitude : public ParserSemanticAction {
      /** Actor Constructor. */
      storeLongitude (Location&);
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
