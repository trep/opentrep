// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <vector>
#include <fstream>
// Boost
#include <boost/lexical_cast.hpp>
//#define BOOST_SPIRIT_DEBUG
#define BOOST_SPIRIT_UNICODE
// OpenTREP
#include <opentrep/basic/BasParserTypes.hpp>
#include <opentrep/bom/PRParserHelper.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  /** Namespaces. */
  namespace bsq = boost::spirit::qi;
  //namespace bsa = boost::spirit::ascii;
  namespace bsu = boost::spirit::unicode;
  namespace bsc = boost::spirit::classic;

  namespace PRParserHelper {
      
    // //////////////////////////////////////////////////////////////////
    //  Semantic actions
    // //////////////////////////////////////////////////////////////////

    ParserSemanticAction::ParserSemanticAction (Location& ioLocation)
      : _location (ioLocation) {
    }      
   
    // //////////////////////////////////////////////////////////////////
    storeIataCode::storeIataCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeIataCode::operator() (std::vector<uchar_t> iChar,
                                    bsq::unused_type, bsq::unused_type) const {

      const std::string lIataCodeStr (iChar.begin(), iChar.end());
      const OPENTREP::IATACode_T lIataCode (lIataCodeStr);
      _location.setIataCode (lIataCode);

      // Reset the values
      //_location.resetMatrix();
      //_location._itAltNameShortList.clear();

      // DEBUG
       //OPENTREP_LOG_DEBUG ( "IATA code: " << _location.getIataCode());
    }

    // //////////////////////////////////////////////////////////////////
    storePageRank::storePageRank (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storePageRank::operator() (double iPageRank,
                                    bsq::unused_type, bsq::unused_type) const {
      _location.setPageRank (iPageRank);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("PageRank: " << _location.getPageRank());
    }

    // //////////////////////////////////////////////////////////////////
    storePRType::storePRType (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    void storePRType::operator() (std::vector<uchar_t> iChar,
                                  bsq::unused_type, bsq::unused_type) const {
      const std::string lIATATypeStr (iChar.begin(), iChar.end());
      const IATAType_T lIATAType (lIATATypeStr);
      _location.setIATAType (lIATAType);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("IATA type: " << _location.getIATAType());
    }
    
    // //////////////////////////////////////////////////////////////////
    doEndPR::doEndPR (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void doEndPR::operator() (bsq::unused_type,
                              bsq::unused_type, bsq::unused_type) const {
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Do End. Location structure: " << _location);
    }  

    // ///////////////////////////////////////////////////////////////////
    //
    //  Utility Parsers
    //
    // ///////////////////////////////////////////////////////////////////
    
    /** 1-digit-integer parser */
    OPENTREP::uint1_p_t uint1_p;
    
    /** 2-digit-integer parser */
    OPENTREP::uint2_p_t uint2_p;

    /** Up-to-2-digit-integer parser */
    OPENTREP::int1_2_p_t int1_2_p;

    /** 4-digit-integer parser */
    OPENTREP::uint4_p_t uint4_p;
    
    /** Up-to-5-digit-integer parser */
    OPENTREP::int1_5_p_t int1_5_p;
    OPENTREP::int0_5_p_t int0_5_p;

    /** Up-to-4-digit-integer parser */
    OPENTREP::uint1_4_p_t uint1_4_p;

    /** Up-to-9-digit-integer parser */
    OPENTREP::uint1_9_p_t uint1_9_p;
    OPENTREP::uint0_9_p_t uint0_9_p;

    /** Time element parsers. */
    OPENTREP::hour_p_t hour_p;
    OPENTREP::minute_p_t minute_p;
    OPENTREP::second_p_t second_p;

    /** Date element parsers. */
    OPENTREP::year_p_t year_p;
    OPENTREP::month_p_t month_p;
    OPENTREP::day_p_t day_p;

    /** Boolean parser */
    OPENTREP::boolean_p_t boolean_p;

    /////////////////////////////////////////////////////////////////////////
    //
    //  (Boost Spirit) Grammar Definition
    //
    /////////////////////////////////////////////////////////////////////////
    /**

       -- PageRanked POR (points of reference):
       -- -------------------------------------
       --
       -- Samples:
       --
       -- LON-C^LON^1.0
       -- PAR-C^PAR^0.994632137197
       -- NYC-C^NYC^0.948221089373
       -- CHI-C^CHI^0.768305897463
       -- ATL-A^ATL^0.686723208248
       -- ATL-C^ATL^0.686723208248
       -- ORD-A^ORD^0.677280625337
       -- CDG-A^CDG^0.647060165878

       iata_code^location_type^page_rank

       iata_code       varchar(3)
       location_type   varchar(4)
       page_rank       decimal(15,12)
    */ 

    /**
     * Grammar for the PR-Rule parser.
     */
    template <typename Iterator>	
    struct LocationParser : public bsq::grammar<Iterator, bsu::blank_type> {

      LocationParser (Location& ioPRRule) : 
        LocationParser::base_type(start), _location(ioPRRule) {

        start = bsq::eps
          >> *(header | pr_rule);

        header = bsq::lit("iata_code") >> +(bsu::char_ - bsq::eoi - bsq::eol)
                                       >> (bsq::eoi | bsq::eol);

        pr_rule = pr_key
          >> '^' >> iata_code
          >> '^' >> page_rank
          >> pr_rule_end[doEndPR(_location)];
        // >> +( '^' >> segment )

        pr_rule_end = bsq::eps;

        pr_key = iata_code
          >> '-' >> pr_type
          ;

        iata_code =
          bsq::repeat(3)[bsu::char_('A', 'Z')][storeIataCode(_location)];

        page_rank = bsq::double_[storePageRank(_location)];

        pr_type =
          bsq::repeat(1,3)[bsu::char_("ABCGHOPRZ")][storePRType(_location)]
          ;

        //BOOST_SPIRIT_DEBUG_NODE (LocationParser);
        BOOST_SPIRIT_DEBUG_NODE (start);
        BOOST_SPIRIT_DEBUG_NODE (header);
        BOOST_SPIRIT_DEBUG_NODE (pr_rule);
        BOOST_SPIRIT_DEBUG_NODE (pr_rule_end);
        BOOST_SPIRIT_DEBUG_NODE (pr_key);
        BOOST_SPIRIT_DEBUG_NODE (iata_code);
        BOOST_SPIRIT_DEBUG_NODE (page_rank);
        BOOST_SPIRIT_DEBUG_NODE (pr_type);
      }

      // Instantiation of rules
      bsq::rule<Iterator, bsu::blank_type>
      start, header, pr_rule, pr_rule_end, pr_key, iata_code, page_rank, pr_type;

      // Parser Context
      Location& _location;
    };    
  }


  /////////////////////////////////////////////////////////////////////////
  //
  //  Entry class for the string parser
  //
  /////////////////////////////////////////////////////////////////////////
    
  // //////////////////////////////////////////////////////////////////////
  PRStringParser::PRStringParser (const std::string& iString)
    : _string (iString) {
    init();
  }

  // //////////////////////////////////////////////////////////////////////
  void PRStringParser::init() {
  }
    
  // //////////////////////////////////////////////////////////////////////
  PRStringParser::~PRStringParser() {
  }

  // //////////////////////////////////////////////////////////////////////
  const Location& PRStringParser::generateLocation() {
    // DEBUG
    // OPENTREP_LOG_DEBUG ("Parsing PR string: '" << _string << "'");

    // String to be parsed
    std::istringstream stringToBeParsed (_string);
    
    // Create an input iterator
    OPENTREP::base_iterator_t iStr (stringToBeParsed);

    // Convert input iterator to an iterator usable by spirit parser  
    OPENTREP::iterator_t fwd_start(boost::spirit::make_default_multi_pass(iStr));
    OPENTREP::iterator_t fwd_end;

    // Initialise the positional iterators
    OPENTREP::pos_iterator_t pos_start (fwd_start, fwd_end, _string);
    OPENTREP::pos_iterator_t pos_end;

    // Initialise the parser (grammar) with the helper/staging structure.
    PRParserHelper::LocationParser<OPENTREP::iterator_t> lPRParser (_location);
      
    // Launch the parsing of the file and, thanks to the doEndPR
    // call-back structure, the building of the whole BomRoot BOM
    bool hasParsingBeenSuccesful = false;
    try {

      hasParsingBeenSuccesful = bsq::phrase_parse (fwd_start, fwd_end,
                                                   lPRParser, bsu::blank);

    } catch (const bsq::expectation_failure<pos_iterator_t>& e) {
      const bsc::file_position_base<std::string>& pos = e.first.get_position();
      std::ostringstream oStr;
      oStr << "Parse error on PR string '" << _string
           << "', position " << pos.column << std::endl
           << "'" << e.first.get_currentline() << "'" << std::endl
           << std::setw(pos.column) << " " << "^- here";
      OPENTREP_LOG_ERROR (oStr.str());
      throw PRFileParsingException (oStr.str());
    }
      
    if (hasParsingBeenSuccesful == false) {
      OPENTREP_LOG_ERROR ("Parsing of PR input string: '" << _string
                          << "' failed");
      throw PRFileParsingException ("Parsing of PR input string: '"
                                     + _string + "' failed");
    }
    
    if  (fwd_start != fwd_end) {
      OPENTREP_LOG_ERROR ("Parsing of PR input string: '" << _string
                          << "' failed");
      throw PRFileParsingException ("Parsing of PR input file: '"
                                     + _string + "' failed");
    }
    
    //
    if (hasParsingBeenSuccesful == true && fwd_start == fwd_end) {
      // DEBUG
      /*
      OPENTREP_LOG_DEBUG ("Parsing of PR input string: '" << _string
                          << "' succeeded");
      */
    }

    return _location;
  }


  /////////////////////////////////////////////////////////////////////////
  //
  //  Entry class for the file parser
  //
  /////////////////////////////////////////////////////////////////////////
    
  // //////////////////////////////////////////////////////////////////////
  PRFileParser::PRFileParser (const PRFilePath_T& iFilename)
    : _filename (iFilename) {
    init();
  }

  // //////////////////////////////////////////////////////////////////////
  void PRFileParser::init() {
  }
    
  // //////////////////////////////////////////////////////////////////////
  void PRFileParser::generateLocations() {
      
    OPENTREP_LOG_DEBUG ("Parsing pr input file: " << _filename);

    // File to be parsed
    const char* lFilenameStr = _filename.c_str();
    std::ifstream fileToBeParsed (lFilenameStr, std::ios_base::in);

    // Check if the filename exist and can be open
    if (fileToBeParsed.is_open() == false) {
      OPENTREP_LOG_ERROR ("The pr file " << _filename << " can not be open."
                          << std::endl);

      throw FileNotFoundException ("The file " + _filename
                                   + " does not exist or can not be read");
    }
    
    // Create an input iterator
    OPENTREP::base_iterator_t iStr (fileToBeParsed);

    // Convert input iterator to an iterator usable by spirit parser  
    OPENTREP::iterator_t start (boost::spirit::make_default_multi_pass (iStr));
    OPENTREP::iterator_t end;

    // Initialise the parser (grammar) with the helper/staging structure.
    PRParserHelper::LocationParser<OPENTREP::iterator_t> lPRParser (_location);
      
    // Launch the parsing of the file and, thanks to the doEndPR
    // call-back structure, the building of the whole BomRoot BOM
    const bool hasParsingBeenSuccesful = 
       bsq::phrase_parse (start, end, lPRParser, bsu::blank);
      
    if (hasParsingBeenSuccesful == false) {
      OPENTREP_LOG_ERROR ("Parsing of pr input file: " << _filename
                          << " failed");
      throw PRFileParsingException ("Parsing of pr input file: "
                                     + _filename + " failed");
    }
    
    if  (start != end) {
      OPENTREP_LOG_ERROR ("Parsing of pr input file: " << _filename
                          << " failed");
      throw PRFileParsingException ("Parsing of pr input file: "
                                     + _filename + " failed");
    }
    
    if (hasParsingBeenSuccesful == true && start == end) {
      OPENTREP_LOG_DEBUG ("Parsing of pr input file: " << _filename
                          << " succeeded");
    } 
  }

}
