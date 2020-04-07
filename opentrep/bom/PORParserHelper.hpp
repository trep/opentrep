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
#include <opentrep/CityDetails.hpp>

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

    /** Store the parsed FAA code. */
    struct storeFaaCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeFaaCode (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<uchar_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed UN/LOCODE code. */
    struct storeUNLOCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeUNLOCode (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<uchar_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed UIC code. */
    struct storeUICCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeUICCode (Location&);
      /** Actor Function (functor). */
      void operator() (unsigned int iUICCode,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
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

    /** Store the parsed envelope ID. */
    struct storeEnvelopeID : public ParserSemanticAction {
      /** Actor Constructor. */
      storeEnvelopeID (Location&);
      /** Actor Function (functor). */
      void operator() (unsigned int,
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

    /** Store the details of the served city. */
    struct storeCityDetailList : public ParserSemanticAction {
      /** Actor Constructor. */
      storeCityDetailList (Location&);
      /** Actor Function (functor). */
      void operator() (boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };    

    /** Store the short list of alternate names. */
    struct storeAltNameShort : public ParserSemanticAction {
      /** Actor Constructor. */
      storeAltNameShort (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<uchar_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the short list of alternate names. */
    struct storeAltNameShortListString : public ParserSemanticAction {
      /** Actor Constructor. */
      storeAltNameShortListString (Location&);
      /** Actor Function (functor). */
      void operator() (boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the list of IATA codes of the travel-related POR. */
    struct storeTvlPORCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeTvlPORCode (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<uchar_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the list of IATA codes of the travel-related POR. */
    struct storeTvlPORListString : public ParserSemanticAction {
      /** Actor Constructor. */
      storeTvlPORListString (Location&);
      /** Actor Function (functor). */
      void operator() (boost::spirit::qi::unused_type,
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
    
    /** Store the feature class. */
    struct storeFeatureClass : public ParserSemanticAction {
      /** Actor Constructor. */
      storeFeatureClass (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<uchar_t>,
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

    /** Store the parsed PageRank value. */
    struct storePageRank : public ParserSemanticAction {
      /** Actor Constructor. */
      storePageRank (Location&);
      /** Actor Function (functor). */
      void operator() (double,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const; 
    };
    
    /** Store the parsed date from. */
    struct storeDateFrom : public ParserSemanticAction {
      /** Actor Constructor. */
      storeDateFrom (Location&);
      /** Actor Function (functor). */
      void operator() (boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed date end. */
    struct storeDateEnd : public ParserSemanticAction {
      /** Actor Constructor. */
      storeDateEnd (Location&);
      /** Actor Function (functor). */
      void operator() (boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the comments. */
    struct storeComments : public ParserSemanticAction {
      /** Actor Constructor. */
      storeComments (Location&);
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

    /** Store the alternative country code. */
    struct storeAltCountryCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeAltCountryCode (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<uchar_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the country name. */
    struct storeCountryName : public ParserSemanticAction {
      /** Actor Constructor. */
      storeCountryName (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<uchar_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed continent name. */
    struct storeContinentName : public ParserSemanticAction {
      /** Actor Constructor. */
      storeContinentName (Location&);
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

    /** Store the administrative 1 UTF8 name. */
    struct storeAdm1UtfName : public ParserSemanticAction {
      /** Actor Constructor. */
      storeAdm1UtfName (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<uchar_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the administrative 1 ASCII name. */
    struct storeAdm1AsciiName : public ParserSemanticAction {
      /** Actor Constructor. */
      storeAdm1AsciiName (Location&);
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

    /** Store the administrative 1 ASCII name. */
    struct storeAdm2UtfName : public ParserSemanticAction {
      /** Actor Constructor. */
      storeAdm2UtfName (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<uchar_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the administrative 1 ASCII name. */
    struct storeAdm2AsciiName : public ParserSemanticAction {
      /** Actor Constructor. */
      storeAdm2AsciiName (Location&);
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

    /** Store the parsed last modification date. */
    struct storeModDate : public ParserSemanticAction {
      /** Actor Constructor. */
      storeModDate (Location&);
      /** Actor Function (functor). */
      void operator() (boost::spirit::qi::unused_type,
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

    /** Store the parsed city Geonames ID. */
    struct storeCityGeonamesID : public ParserSemanticAction {
      /** Actor Constructor. */
      storeCityGeonamesID (Location&);
      /** Actor Function (functor). */
      void operator() (unsigned int,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed city UTF8 name. */
    struct storeCityUtfName : public ParserSemanticAction {
      /** Actor Constructor. */
      storeCityUtfName (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<uchar_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed city ASCII name. */
    struct storeCityAsciiName : public ParserSemanticAction {
      /** Actor Constructor. */
      storeCityAsciiName (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<uchar_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the city country code. */
    struct storeCityCountryCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeCityCountryCode (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<uchar_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed city state code. */
    struct storeCityStateCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeCityStateCode (Location&);
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

    /** Store the parsed World Area Code (WAC). */
    struct storeWAC : public ParserSemanticAction {
      /** Actor Constructor. */
      storeWAC (Location&);
      /** Actor Function (functor). */
      void operator() (unsigned int,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed World Area Code (WAC). */
    struct storeWACName : public ParserSemanticAction {
      /** Actor Constructor. */
      storeWACName (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<uchar_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed currency code. */
    struct storeCurrencyCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeCurrencyCode (Location&);
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


    /** Store the parsed Wiki link. */
    struct storeWikiLink : public ParserSemanticAction {
      /** Actor Constructor. */
      storeWikiLink (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<uchar_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed Geonames latitude value. */
    struct storeGeonameLatitude : public ParserSemanticAction {
      /** Actor Constructor. */
      storeGeonameLatitude (Location&);
      /** Actor Function (functor). */
      void operator() (double,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const; 
    };
    
    /** Store the parsed Geonames longitude value. */
    struct storeGeonameLongitude : public ParserSemanticAction {
      /** Actor Constructor. */
      storeGeonameLongitude (Location&);
      /** Actor Function (functor). */
      void operator() (double,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const; 
    };
    
    /** Store the full language code of an alternate name. */
    struct storeAltLangCodeFull : public ParserSemanticAction {
      /** Actor Constructor. */
      storeAltLangCodeFull (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<boost::uint32_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /**
     * Store the first 2 characters of the language code of an alternate name
     */
    struct storeAltLangCode2Char : public ParserSemanticAction {
      /** Actor Constructor. */
      storeAltLangCode2Char (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<boost::uint32_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /**
     * Store the country-specific part of the language code of an alternate name
     */
    struct storeAltLangCodeExt : public ParserSemanticAction {
      /** Actor Constructor. */
      storeAltLangCodeExt (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<boost::uint32_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the historical part of the language code of an alternate name. */
    struct storeAltLangCodeHist : public ParserSemanticAction {
      /** Actor Constructor. */
      storeAltLangCodeHist (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<boost::uint32_t>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store an alternate name. */
    struct storeAltName : public ParserSemanticAction {
      /** Actor Constructor. */
      storeAltName (Location&);
      /** Actor Function (functor). */
      void operator() (std::vector<boost::uint32_t>,
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
  class PORStringParser {
  public:
    /**
     * Constructor.
     */
    PORStringParser (const std::string& iString);

    /**
     * Destructor.
     */
    ~PORStringParser();

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
     * POR Structure.
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
  class PORFileParser {
  public:
    /**
     * Constructor.
     */
    PORFileParser (const PORFilePath_T& iFilename);

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
    PORFilePath_T _filename;

    /**
     * POR Structure.
     */
    Location _location;
  };
    
}
#endif // __OPENTREP_CMD_PORPARSERHELPER_HPP
