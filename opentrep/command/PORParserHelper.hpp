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

    /** Unicode character. */
    //typedef char uchar_t;
    typedef boost::uint32_t uchar_t;


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
      void operator() (std::vector<uchar_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed ICAO code. */
    struct storeIcaoCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeIcaoCode (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<uchar_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the common name. */
    struct storeCommonName : public ParserSemanticAction {
      /** Actor Constructor. */
      storeCommonName (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<uchar_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the ASCII name. */
    struct storeAsciiName : public ParserSemanticAction {
      /** Actor Constructor. */
      storeAsciiName (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<uchar_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the short list of alternate names. */
    struct storeAltUtfName : public ParserSemanticAction {
      /** Actor Constructor. */
      storeAltUtfName (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<boost::uint32_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the short list of alternate names. */
    struct storeAltAsciiName : public ParserSemanticAction {
      /** Actor Constructor. */
      storeAltAsciiName (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<uchar_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the short list of alternate names. */
    struct storeAltShortNameList : public ParserSemanticAction {
      /** Actor Constructor. */
      storeAltShortNameList (Location&);
      /** Actor Function (functor). */
      void operator() (boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the feature class. */
    struct storeFeatureClass : public ParserSemanticAction {
      /** Actor Constructor. */
      storeFeatureClass (Location&);
      /** Actor Function (functor). */
      void operator() (uchar_t,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the feature code. */
    struct storeFeatureCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeFeatureCode (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<uchar_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the country code. */
    struct storeCountryCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeCountryCode (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<uchar_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the administrative 1 code. */
    struct storeAdm1Code : public ParserSemanticAction {
      /** Actor Constructor. */
      storeAdm1Code (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<uchar_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the administrative 2 code. */
    struct storeAdm2Code : public ParserSemanticAction {
      /** Actor Constructor. */
      storeAdm2Code (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<uchar_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the administrative 3 code. */
    struct storeAdm3Code : public ParserSemanticAction {
      /** Actor Constructor. */
      storeAdm3Code (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<uchar_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the administrative 4 code. */
    struct storeAdm4Code : public ParserSemanticAction {
      /** Actor Constructor. */
      storeAdm4Code (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<uchar_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the time zone. */
    struct storeTimeZone : public ParserSemanticAction {
      /** Actor Constructor. */
      storeTimeZone (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<uchar_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed GMT offset. */
    struct storeGMTOffset : public ParserSemanticAction {
      /** Actor Constructor. */
      storeGMTOffset (Location&);
      /** Actor Function (functor). */
      void operator() (float,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed DST offset. */
    struct storeDSTOffset : public ParserSemanticAction {
      /** Actor Constructor. */
      storeDSTOffset (Location&);
      /** Actor Function (functor). */
      void operator() (float,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed raw offset. */
    struct storeRawOffset : public ParserSemanticAction {
      /** Actor Constructor. */
      storeRawOffset (Location&);
      /** Actor Function (functor). */
      void operator() (float,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed airport flag. */
    struct storeIsAirport : public ParserSemanticAction {
      /** Actor Constructor. */
      storeIsAirport (Location&);
      /** Actor Function (functor). */
      void operator() (bool,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed commercial flag. */
    struct storeIsCommercial : public ParserSemanticAction {
      /** Actor Constructor. */
      storeIsCommercial (Location&);
      /** Actor Function (functor). */
      void operator() (bool,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed city IATA code. */
    struct storeCityCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeCityCode (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<uchar_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed state code. */
    struct storeStateCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeStateCode (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<uchar_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed region code. */
    struct storeRegionCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeRegionCode (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<uchar_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed POR type. */
    struct storePORType : public ParserSemanticAction {
      /** Actor Constructor. */
      storePORType (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<uchar_t>,
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
    
    /** Store the parsed Wiki link. */
    struct storeWikiLink : public ParserSemanticAction {
      /** Actor Constructor. */
      storeWikiLink (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<uchar_t>,
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
