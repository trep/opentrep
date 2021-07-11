#ifndef __OPENTREP_BAS_BASPARSERTYPES_HPP
#define __OPENTREP_BAS_BASPARSERTYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// Boost Spirit (Parsing)
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/support_multi_pass.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>
// OpenTREP
#include <opentrep/OPENTREP_ParserHelperTypes.hpp>

namespace OPENTREP {

  // ////////////////////////////////////////////////////////////////////
  //
  //  Definition of Basic Types
  //
  // ////////////////////////////////////////////////////////////////////
  // The types of iterator, scanner and rule are then derived from
  // the parsing unit.
  typedef std::istreambuf_iterator<char> base_iterator_t;
  typedef boost::spirit::multi_pass<base_iterator_t> iterator_t;  

  // Wrap the forward iterator with position iterator, to record the position
  typedef boost::spirit::classic::position_iterator2<iterator_t> pos_iterator_t;


  // ////////////////////////////////////////////////////////////////////
  //
  //  Parser related types
  //
  // ////////////////////////////////////////////////////////////////////
  /** 1-digit-integer parser */
  typedef boost::spirit::x3::uint_parser<int, 10, 1, 1> uint1_p_t;
    
  /** 2-digit-integer parser */
  typedef boost::spirit::x3::uint_parser<int, 10, 2, 2> uint2_p_t;
    
  /** Up-to-2-digit-integer parser */
  typedef boost::spirit::x3::uint_parser<int, 10, 1, 2> int1_2_p_t;

  /** 4-digit-integer parser */
  typedef boost::spirit::x3::uint_parser<int, 10, 4, 4> uint4_p_t;

  /** Up-to-5-digit-integer parser */
  typedef boost::spirit::x3::int_parser<int, 10, 1, 5> int1_5_p_t;
  typedef boost::spirit::x3::int_parser<int, 10, 0, 5> int0_5_p_t;

  /** Up-to-4-digit-integer parser */
  typedef boost::spirit::x3::uint_parser<int, 10, 1, 4> uint1_4_p_t;

  /** Up-to-9-digit-integer parser */
  typedef boost::spirit::x3::uint_parser<int, 10, 1, 9> uint1_9_p_t;
  typedef boost::spirit::x3::uint_parser<int, 10, 0, 9> uint0_9_p_t;

  /** Date & time element parsers */
  typedef boost::spirit::x3::uint_parser<hour_t, 10, 2, 2> hour_p_t;
  typedef boost::spirit::x3::uint_parser<minute_t, 10, 2, 2> minute_p_t;
  typedef boost::spirit::x3::uint_parser<second_t, 10, 2, 2> second_p_t;
  typedef boost::spirit::x3::uint_parser<year_t, 10, 4, 4> year_p_t;
  typedef boost::spirit::x3::uint_parser<month_t, 10, 2, 2> month_p_t;
  typedef boost::spirit::x3::uint_parser<day_t, 10, 2, 2> day_p_t;
  
}
#endif // __OPENTREP_BAS_BASPARSERTYPES_HPP
