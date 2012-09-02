// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <vector>
#include <fstream>
// Opentrep
//#define BOOST_SPIRIT_DEBUG
// OpenTREP
#include <opentrep/basic/BasParserTypes.hpp>
#include <opentrep/service/Logger.hpp>
#include <opentrep/command/PORParserHelper.hpp>

namespace OPENTREP {

  namespace PorParserHelper {
      
    // //////////////////////////////////////////////////////////////////
    //  Semantic actions
    // //////////////////////////////////////////////////////////////////

    ParserSemanticAction::ParserSemanticAction (Location& ioLocation)
      : _location (ioLocation) {
    }      
   
    // //////////////////////////////////////////////////////////////////
    storeGeonamesId::storeGeonamesId (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeGeonamesId::operator() (unsigned int iPorId,
                                 boost::spirit::qi::unused_type,
                                 boost::spirit::qi::unused_type) const {
      _location.setGeonamesID (iPorId);
      
      // DEBUG
      //OPENTREP_LOG_DEBUG ( "Por Id: " << _location.getPorID ());
    }
    
    // //////////////////////////////////////////////////////////////////
    storeIataCode ::storeIataCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeIataCode::operator() (std::vector<char> iChar,
                                    boost::spirit::qi::unused_type,
                                    boost::spirit::qi::unused_type) const {
      const std::string lIataCodeStr (iChar.begin(), iChar.end());
      const OPENTREP::IATACode_T lIataCode (lIataCodeStr);
      _location.setIataCode (lIataCode);
       // DEBUG
       //OPENTREP_LOG_DEBUG ( "IATA code: " << _location.getIataCode());
    }

    // //////////////////////////////////////////////////////////////////
    storePORType::storePORType (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storePORType::operator() (std::vector<char> iChar,
                                   boost::spirit::qi::unused_type,
                                   boost::spirit::qi::unused_type) const {
      const std::string lPORType (iChar.begin(), iChar.end());
      // DEBUG
      //OPENTREP_LOG_DEBUG ("POR type: " << _location.getPORType());
    }

    
    // //////////////////////////////////////////////////////////////////
    storeModDate::storeModDate (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeModDate::operator() (boost::spirit::qi::unused_type,
                                   boost::spirit::qi::unused_type,
                                   boost::spirit::qi::unused_type) const {
      const OPENTREP::Date_T& lModDate = _location.calculateDate();
      _location.setModificationDate (lModDate);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Modification date: " << _location.getModificationDate());
    }

    // //////////////////////////////////////////////////////////////////
    storeCabinCode::storeCabinCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeCabinCode::operator() (char iChar,
                                     boost::spirit::qi::unused_type,
                                     boost::spirit::qi::unused_type) const {
      std::ostringstream ostr;
      ostr << iChar;
      //const std::string cabinCodeStr = ostr.str();
     
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Cabin Code: " << _location.getCabinCode ());                 
    
    }

    // //////////////////////////////////////////////////////////////////
    storeAdvancePurchase::storeAdvancePurchase (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeAdvancePurchase::operator() (unsigned int iAdancePurchase,
                                           boost::spirit::qi::unused_type,
                                           boost::spirit::qi::unused_type) const {
      //const unsigned int lAdancePurchase = iAdancePurchase;
      // DEBUG
      //OPENTREP_LOG_DEBUG ( "Advance Purchase: " << _location.getAdvancePurchase ());
    }
    
    // //////////////////////////////////////////////////////////////////
    storeLatitude::storeLatitude (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeLatitude::operator() (double iLatitude,
                                    boost::spirit::qi::unused_type,
                                    boost::spirit::qi::unused_type) const {
      _location.setLatitude (iLatitude);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Latitude: " << _location.getLatitude());
    }

    // //////////////////////////////////////////////////////////////////
    doEndPor::doEndPor (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void doEndPor::operator() (boost::spirit::qi::unused_type,
                                boost::spirit::qi::unused_type,
                                boost::spirit::qi::unused_type) const {
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Do End");
      // Generation of the por rule object.
      // LocationGenerator::createAirportPair (_location);
      OPENTREP_LOG_DEBUG (_location.toString());
    }  

    // ///////////////////////////////////////////////////////////////////
    //
    //  Utility Parsers
    //
    // ///////////////////////////////////////////////////////////////////
    /** Namespaces. */
    namespace bsq = boost::spirit::qi;
    namespace bsa = boost::spirit::ascii;
    
    /** 1-digit-integer parser */
    OPENTREP::int1_p_t int1_p;
    
    /** 2-digit-integer parser */
    OPENTREP::uint2_p_t uint2_p;

    /** 4-digit-integer parser */
    OPENTREP::uint4_p_t uint4_p;
    
    /** Up-to-4-digit-integer parser */
    OPENTREP::uint1_4_p_t uint1_4_p;

    /** Time element parsers. */
    OPENTREP::hour_p_t hour_p;
    OPENTREP::minute_p_t minute_p;
    OPENTREP::second_p_t second_p;

    /** Date element parsers. */
    OPENTREP::year_p_t year_p;
    OPENTREP::month_p_t month_p;
    OPENTREP::day_p_t day_p;

    /////////////////////////////////////////////////////////////////////////
    //
    //  (Boost Spirit) Grammar Definition
    //
    /////////////////////////////////////////////////////////////////////////
    /**

       // POR: IataCode^XapianDocID^IcaoCode^IsGeonames^GeonameID^Latitude^Longitude^FeatClass^FeatCode^CountryCode^CC2^Admin1^Admin2^Admin3^Admin4^Population^Elevation^Gtopo30^TimeZone^GmtOffset^DstOffset^RawOffset^ModDate^IsAirport^IsCommercial^CityCode^StateCode^RegionCode^LocationType^WikiLink

     IataCode                (3-char IATA airport code)
     XapianDocID             (empty)
     IcaoCode                (4-char ICAO airport code)
     IsGeonames              (1-char boolean stating whether it is in Geonames)
     GeonameID               (7-digit ID)
     Latitude                (signed double for the geographical latitude)
     Longitude               (signed double for the geographical latitude)
     FeatClass               (1-char feature class)
     FeatCode                (1-char feature code)
     CountryCode             (2-char country code)
     CC2                     (2-char alternate country code)
     Admin1                  (5-alphanum code for rank-1 administrative center)
     Admin2                  (5-alphanum code for rank-2 administrative center)
     Admin3                  (5-alphanum code for rank-3 administrative center)
     Admin4                  (5-alphanum code for rank-4 administrative center)
     Population              (7-digit number of citizens)
     Elevation               (signed integer for the geographical elevation)
     Gtopo30                 (signed integer for the geographical topology)
     TimeZone                (Continent[/Country]/City time zone)
     GmtOffset               (signed 2-digit time off-set)
     DstOffset               (signed 2-digit time off-set)
     RawOffset               (signed 2-digit time off-set)
     ModDate                 (yyyy-mm-dd)
     IsAirport               (1-char boolean stating whether it is an airport)
     IsCommercial            (1-char boolean stating whether it is commercial)
     CityCode                (3-char city code)
     StateCode               (2-char state code)
     RegionCode              (5-char region code)
     LocationType            (1-char IATA location type)
     WikiLink                (URI)
     AltNameList             (List of alternate names)
     
    */ 

    /** Grammar for the Por-Rule parser. */
    template <typename Iterator>	
    struct LocationParser : 
      public boost::spirit::qi::grammar<Iterator, boost::spirit::ascii::space_type> {

      LocationParser (Location& ioporRule) : 
        LocationParser::base_type(start), _location(ioporRule) {

      start = *(header | por_rule);

      header = (bsq::lexeme[bsq::lit("iata_code")
                            >> +(bsa::char_ - bsq::eol) >> bsq::eol]);

      por_rule = por_key
        >> por_rule_end[doEndPor(_location)];
      // >> +( '^' >> segment )

      por_rule_end = bsa::char_('^');

      por_key = por_id
        >> '^' >> iata_code
        >> '^' >> tripType
        >> '^' >> modDate
        >> '^' >> cabinCode
        >> '^' >> advancePurchase
        >> '^' >> latitude;

      por_id = uint1_4_p[storeGeonamesId(_location)];

      iata_code = bsq::repeat(3)[bsa::char_("A-Z")][storeIataCode(_location)];
      
      tripType =
        bsq::repeat(2)[bsa::char_("A-Z")][storePORType(_location)];
      
      modDate = date[storeModDate(_location)];

      date = bsq::lexeme
        [year_p[boost::phoenix::ref(_location._itYear) = bsq::labels::_1]
        >> '-'
        >> month_p[boost::phoenix::ref(_location._itMonth) = bsq::labels::_1]
        >> '-'
        >> day_p[boost::phoenix::ref(_location._itDay) = bsq::labels::_1] ];

      cabinCode = bsa::char_("A-Z")[storeCabinCode(_location)];
            
      advancePurchase = uint1_4_p[storeAdvancePurchase(_location)];

      latitude = bsq::double_[storeLatitude(_location)];

      /*
      segment = bsq::repeat(2)[bsa::char_("A-Z")][storeAirlineCode(_location)]
        >> '^'
        >> bsq::repeat(1,bsq::inf)[bsa::char_("A-Z")][storeClass(_location)];
      */

      //BOOST_SPIRIT_DEBUG_NODE (LocationParser);
      BOOST_SPIRIT_DEBUG_NODE (start);
      BOOST_SPIRIT_DEBUG_NODE (header);
      BOOST_SPIRIT_DEBUG_NODE (por_rule);
      BOOST_SPIRIT_DEBUG_NODE (por_rule_end);
      BOOST_SPIRIT_DEBUG_NODE (por_key);
      BOOST_SPIRIT_DEBUG_NODE (por_id);
      BOOST_SPIRIT_DEBUG_NODE (iata_code);
      BOOST_SPIRIT_DEBUG_NODE (tripType);
      BOOST_SPIRIT_DEBUG_NODE (modDate);
      BOOST_SPIRIT_DEBUG_NODE (date);
      BOOST_SPIRIT_DEBUG_NODE (cabinCode);
      BOOST_SPIRIT_DEBUG_NODE (advancePurchase);
      BOOST_SPIRIT_DEBUG_NODE (latitude);
      }

      // Instantiation of rules
      boost::spirit::qi::rule<Iterator, boost::spirit::ascii::space_type>
      start, header, por_rule, por_rule_end, por_key, por_id, iata_code,
        destination, tripType, modDate, date, cabinCode, advancePurchase, latitude;
      
      // Parser Context
      Location& _location;
    };
    
  }


  /////////////////////////////////////////////////////////////////////////
  //
  //  Entry class for the file parser
  //
  /////////////////////////////////////////////////////////////////////////
    
  // //////////////////////////////////////////////////////////////////////
  PORFileParser::PORFileParser (const FilePath_T& iFilename) : _filename (iFilename) {
    init();
  }

  // //////////////////////////////////////////////////////////////////////
  void PORFileParser::init() {
  }
    
  // //////////////////////////////////////////////////////////////////////
  void PORFileParser::generateLocations() {
      
    OPENTREP_LOG_DEBUG ("Parsing por input file: " << _filename);

    // File to be parsed
    const std::string* lFileName = &_filename;
    const char *lChar = (*lFileName).c_str();
    std::ifstream fileToBeParsed(lChar, std::ios_base::in);

    // Check if the filename exist and can be open
    if (fileToBeParsed.is_open() == false) {
      OPENTREP_LOG_ERROR ("The por file " << _filename << " can not be open."
                          << std::endl);

      throw FileNotFoundException ("The file " + _filename
                                   + " does not exist or can not be read");
    }
    
    // Create an input iterator
    OPENTREP::base_iterator_t inputBegin (fileToBeParsed);

    // Convert input iterator to an iterator usable by spirit parser  
    OPENTREP::iterator_t start (boost::spirit::make_default_multi_pass (inputBegin));
    OPENTREP::iterator_t end;

    // Initialise the parser (grammar) with the helper/staging structure.
    PorParserHelper::LocationParser<OPENTREP::iterator_t> lPORParser (_location);
      
    // Launch the parsing of the file and, thanks to the doEndPor
    // call-back structure, the building of the whole BomRoot BOM
    const bool hasParsingBeenSuccesful = 
       boost::spirit::qi::phrase_parse (start, end, lPORParser,
                                        boost::spirit::ascii::space);
      
    if (hasParsingBeenSuccesful == false) {
      OPENTREP_LOG_ERROR ("Parsing of por input file: " << _filename << " failed");
      throw PorFileParsingException ("Parsing of por input file: "
                                     + _filename + " failed");
    }
    
    if  (start != end) {
      OPENTREP_LOG_ERROR ("Parsing of por input file: " << _filename
                        << " failed");
      throw PorFileParsingException ("Parsing of por input file: "
                                     + _filename + " failed");
    }
    
    if (hasParsingBeenSuccesful == true && start == end) {
      OPENTREP_LOG_DEBUG ("Parsing of por input file: " << _filename
      << " succeeded");
    }
 
  }
    
}
