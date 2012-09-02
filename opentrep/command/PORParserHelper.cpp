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

       -- Geonames-related part:
       -- ----------------------
       -- iata_code         : IATA code; varchar(3). See also:
       --                    http://www.iata.org/ps/publications/Pages/code-search.aspx
       -- icao_code         : ICAO code; varchar(4)
       -- geonameid         : Integer ID of record in geonames database
       -- name              : Name of geographical point
       --                     (UTF8) varchar(200)
       -- asciiname         : Name of geographical point in plain ascii characters
       --                     (ASCII) varchar(200)
       -- alternatenames    : Alternate names, comma separated
       --                     varchar(5000)
       -- latitude          : Latitude in decimal degrees (wgs84)
       -- longitude         : Longitude in decimal degrees (wgs84)
       -- feature class     : See http://www.geonames.org/export/codes.html
       --                     char(1)
       -- feature code      : See http://www.geonames.org/export/codes.html
       --                     varchar(10)
       -- country code      : ISO-3166 2-letter country code, 2 characters
       -- cc2               : Alternate country codes, comma separated, ISO-3166
       --                     2-letter country code, 60 characters
       -- admin1 code       : FIPS code (subject to change to ISO code), see exceptions
       --                     below. See file admin1Codes.txt for display names of
       --                     this code; varchar(20)
       -- admin2 code       : Code for the second administrative division, a county
       --                     in the US. See file admin2Codes.txt; varchar(80)
       -- admin3 code       : Code for third level administrative division
       --                     varchar(20)
       -- admin4 code       : Code for fourth level administrative division
       --                     varchar(20)
       -- population        : bigint (8 byte int) 
       -- elevation         : In meters, integer
       -- dem               : Digital elevation model, srtm3 or gtopo30, average
       --                     elevation of 3''x3'' (ca 90mx90m) or 30''x30''
       --                     (ca 900mx900m) area in meters, integer.
       --                     srtm processed by cgiar/ciat.
       -- timezone          : The time-zone ID (see file timeZone.txt)
       -- gmt offset        : GMT offset on 1st of January
       -- dst offset        : DST offset to GMT on 1st of July (of the current year)
       -- raw offset        : Raw Offset without DST
       -- modification date : Date of last modification in yyyy-MM-dd format
       --
       --
       -- ORI-related part:
       -- -----------------
       --
       -- is_geonames       : Whether that POR is known by Geonames; varchar(1)
       -- is_airport        : Whether that POR is an airport; varchar(1)
       -- is commercial     : Whether or not that POR hosts commercial activities
       --                     varchar(1)
       -- city_code         : The IATA code of the related city, when knwon; varchar(3)
       -- state_code        : The ISO code of the related state; varchar(3)
       -- region_code       : The code of the related region (see below); varchar(5)
       -- location type     : A/APT airport; B/BUS bus/coach station; C/CITY City;
       --                     G/GRD ground transport (this code is used for SK in
       --                     Sweden only); H/HELI Heliport;
       --                     O/OFF-PT off-line point, i.e. a city without an airport;
       --                     R/RAIL railway Station; S/ASSOC a location without its
       --                     own IATA code, but attached to an IATA location.
       --
       -- Regions:
       -- --------
       -- AFRIC / AF        : Africa (geonameId=6255146)
       -- ASIA  / AS        : Asia (geonameId=6255147)
       -- ATLAN             : Atlantic
       -- AUSTL             : Australia
       -- CAMER             : Central America
       -- CARIB             : Carribean
       -- EEURO             : Eastern Europe
       -- EURAS             : Euras
       -- EUROP / EU        : Europe (geonameId=6255148)
       -- IOCEA / OC        : Oceania (geonameId=6255151)
       -- MEAST             : Middle-East
       -- NAMER / NA        : North America (geonameId=6255149)
       -- NONE              : Non real POR
       -- PACIF             : Pacific
       -- SAMER / SA        : South America (geonameId=6255150)
       -- SEASI             : South East
       --       / AN        : Antarctica (geonameId=6255152)
       --
       -- Samples:
       -- CDG^LFPG^6269554^Paris - Charles-de-Gaulle^Paris - Charles-de-Gaulle^49.0127800^2.5500000^FR^AIRP^0^Europe/Paris^1.0^2.0^1.0^CDG,LFPG,Paris - Charles de Gaulle,París - Charles de Gaulle,Roissy Charles de Gaulle
       -- PAR^ZZZZ^2988507^Paris^Paris^48.8534100^2.3488000^FR^PPLC^2138551^Europe/Paris^1.0^2.0^1.0^Lungsod ng Paris,Lutece,Lutetia Parisorum,PAR,Pa-ri,Paarys,Paname,Pantruche,Paraeis,Paras,Pari,Paries,Pariggi,Parigi,Pariis,Pariisi,Parijs,Paris,Paris - Paris,Parisi,Pariz,Parize,Parizh,Parizo,Parizs,Parys,Paryz,Paryzh,Paryzius,Paryż,Paryžius,Paräis,París,París - Paris,Paríž,Parîs,Parīze,Paříž,Páras,Párizs,Ville-Lumiere,Ville-Lumière,ba li,barys,pali si,pari,paris,parys,paryzh,perisa,prys,pryz,pyaris,pyrs,Παρίσι,Париж,Париз,Парыж,Փարիզ,פריז,باريس,پارىژ,پاریس,پیرس,ܦܪܝܣ,पॅरिस,பாரிஸ்,ಪ್ಯಾರಿಸ್,ปารีส,პარიზი,ፓሪስ,パリ,巴黎,파리 시
       --
      
       iata_code          varchar(3)
       icao_code          varchar(4)
       is_geonames        varchar(1)
       geonameid          int(11)
       name               varchar(200)
       asciiname          varchar(200)
       alternatenames     varchar(4000)
       latitude           decimal(10,7)
       longitude          decimal(10,7)
       fclass             varchar(1)
       fcode              varchar(10)
       country_code       varchar(2)
       cc2                varchar(60)
       admin1             varchar(20)
       admin2             varchar(80)
       admin3             varchar(20)
       admin4             varchar(20)
       population         int(11)
       elevation          int(11)
       gtopo30            int(11)
       timezone           varchar(40)
       gmt_offset         decimal(3,1)
       dst_offset         decimal(3,1)
       raw_offset         decimal(3,1)
       moddate            date
       is_airport         varchar(1)
       is_commercial      varchar(1)
       city_code          varchar(3)
       state_code         varchar(3)
       region_code        varchar(5)
       location_type      varchar(4)
       wiki_link          varchar(200)
       lang_alt1          varchar(7)
       alt_name1          varchar(200)
       lang_alt2          varchar(7)
       alt_name2          varchar(200)
       lang_alt3          varchar(7)
       alt_name3          varchar(200)
       lang_alt4          varchar(7)
       alt_name4          varchar(200)
       lang_alt5          varchar(7)
       alt_name5          varchar(200)
       lang_alt6          varchar(7)
       alt_name6          varchar(200)
       lang_alt7          varchar(7)
       alt_name7          varchar(200)
       lang_alt8          varchar(7)
       alt_name8          varchar(200)
       lang_alt9          varchar(7)
       alt_name9          varchar(200)
       lang_alt10         varchar(7)
       alt_name10         varchar(200)
    */ 

    /**
     * Grammar for the Por-Rule parser.
     */
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
