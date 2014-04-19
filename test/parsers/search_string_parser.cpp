// STL
#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <vector>
// Boost (Extended STL)
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
// Boost Spirit (Parsing)
//#define BOOST_SPIRIT_DEBUG
#include <boost/spirit/home/classic/core.hpp>
#include <boost/spirit/home/classic/attribute.hpp>
#include <boost/spirit/home/classic/utility/functor_parser.hpp>
#include <boost/spirit/home/classic/utility/loops.hpp>
#include <boost/spirit/home/classic/utility/chset.hpp>
#include <boost/spirit/home/classic/utility/confix.hpp>
#include <boost/spirit/home/classic/iterator/file_iterator.hpp>
#include <boost/spirit/home/classic/actor/push_back_actor.hpp>
#include <boost/spirit/home/classic/actor/assign_actor.hpp>

// Type definitions
typedef char char_t;
typedef char const* iterator_t;
//typedef boost::spirit::classic::file_iterator<char_t> iterator_t;
typedef boost::spirit::classic::scanner<iterator_t> scanner_t;
typedef boost::spirit::classic::rule<scanner_t> rule_t;

/** Place. */
struct Place_T {
  // Attributes
  std::string _name;
  std::string _code;
  /** Constructor. */
  Place_T () : _name (""), _code ("") {}
  /* Display. */  
  void display() const {
    std::cout << "Place: " << _name << " (" << _code << ")" << std::endl;
  }
};

/** List of Place strucutres. */
typedef std::vector<Place_T> PlaceList_T;

/** Date. */
struct Date_T {
  // Attributes
  boost::gregorian::date _date;
  unsigned int _reldays;
  unsigned int _day;
  unsigned int _month;
  unsigned int _year;
  /** Constructor. */
  Date_T () : _reldays (14), _day(1), _month(1), _year(1970) {}
  /* Display. */  
  void display() const {
    std::cout << "Date: " << _date << " (" << _day << "/" << _month
              << "/" << _year << "), i.e. in " << _reldays << " days"
              << std::endl;
  }
  /** Set the date from the staging details. */
  boost::gregorian::date getDate() const {
    return boost::gregorian::date (_year, _month, _day);
  }
};

/** List of Date strucutres. */
typedef std::vector<Date_T> DateList_T;

/** Airline. */
struct Airline_T {
  // Attributes
  bool _isPreferred;
  std::string _name;
  std::string _code;
  /** Constructor. */
  Airline_T () : _isPreferred (true), _name(""), _code("") {}
  /* Display. */  
  void display() const {
    const std::string isPreferredStr = (_isPreferred)?"+":"-";
    std::cout << "Airline: " << isPreferredStr << _name << " (" << _code << ")"
              << std::endl;
  }
};

/** List of Airline strucutres. */
typedef std::vector<Airline_T> AirlineList_T;

/** Passenger. */
struct Passenger_T {
  // Attributes
  typedef enum { ADULT = 0, CHILD, PET, LAST_VALUE } PassengerType_T;
  static const std::string _labels[LAST_VALUE];
  PassengerType_T _type;
  unsigned short _number;
  /** Constructor. */
  Passenger_T () : _type(ADULT), _number(1) {}
  /* Display. */  
  void display() const {
    std::cout << "Passenger: " << _number << " (" << _labels[_type] << ")"
              << std::endl;
  }
};

/** Passenger type labels. */
const std::string Passenger_T::_labels[Passenger_T::LAST_VALUE] =
  { "Adult", "Child", "Pet" };

/** List of Passenger strucutres. */
typedef std::vector<Passenger_T> PassengerList_T;

/** Search string. */
struct SearchString_T {
  // Attributes
  PlaceList_T _placeList;
  DateList_T _dateList;
  AirlineList_T _airlineList;
  PassengerList_T _passengerList;
  
  /** Constructor. */
  SearchString_T () {}

  /* Display. */  
  void display() const {
    std::cout << std::endl;
    
    for (PlaceList_T::const_iterator itPlace = _placeList.begin();
	 itPlace != _placeList.end(); ++itPlace) {
      const Place_T& lPlace = *itPlace;
      lPlace.display();
    }

    for (DateList_T::const_iterator itDate = _dateList.begin();
	 itDate != _dateList.end(); ++itDate) {
      const Date_T& lDate = *itDate;
      lDate.display();
    }

    for (AirlineList_T::const_iterator itAirline = _airlineList.begin();
	 itAirline != _airlineList.end(); ++itAirline) {
      const Airline_T& lAirline = *itAirline;
      lAirline.display();
    }

    for (PassengerList_T::const_iterator itPassenger = _passengerList.begin();
	 itPassenger != _passengerList.end(); ++itPassenger) {
      const Passenger_T& lPassenger = *itPassenger;
      lPassenger.display();
    }

    std::cout << "-- Staging --" << std::endl;
    _tmpPlace.display();
  }

  // //// Staging ////
  Place_T _tmpPlace;
  Date_T _tmpDate;
  Airline_T _tmpAirline;
  Passenger_T _tmpPassenger;
};

///////////////////////////////////////////////////////////////////////////////
//
//  Semantic actions
//
///////////////////////////////////////////////////////////////////////////////
namespace {
  
  /** Store the parsed place element. */
  struct store_place_element {
    store_place_element (SearchString_T& ioSearchString)
      : _searchString (ioSearchString) {}

    void operator() (iterator_t iStr, iterator_t iStrEnd) const {
      std::string lPlace (iStr, iStrEnd);
      // std::cout << "Place: " << lPlace << std::endl;

      // Set the place
      _searchString._tmpPlace._name += " " + lPlace;

      // Add the parsed place to the list
      // _searchString._placeList.push_back (_searchString._tmpPlace);
    }
    
    SearchString_T& _searchString;
  };
  
  /** Store a parsed date. */
  struct store_date {
    store_date (SearchString_T& ioSearchString)
      : _searchString (ioSearchString) {}
    
    void operator() (iterator_t iStr, iterator_t iStrEnd) const {
      _searchString._tmpDate._date = _searchString._tmpDate.getDate();
      // std::cout << "Board date: "
      // << _searchString._date << std::endl;

      // Add the parsed date to the list
      _searchString._dateList.push_back (_searchString._tmpDate);
    }
    
    SearchString_T& _searchString;
  };

  /** Store the airline sign (+/-). */
  struct store_airline_sign {
    store_airline_sign (SearchString_T& ioSearchString)
      : _searchString (ioSearchString) {}

    void operator() (bool iAirlineSign) const { 
      _searchString._tmpAirline._isPreferred = !iAirlineSign;
      // std::cout << "Airline is preferred: " << iAirlineSign << std::endl;
    }
    
    SearchString_T& _searchString;
  };
  
  /** Store the parsed airline code. */
  struct store_airline_code {
    store_airline_code (SearchString_T& ioSearchString)
      : _searchString (ioSearchString) {}
    
    void operator() (iterator_t iStr, iterator_t iStrEnd) const { 
      std::string lAirlineCode (iStr, iStrEnd);
      _searchString._tmpAirline._code = lAirlineCode;
      // std::cout << "Airline code: " << lAirlineCode << std::endl;

      // Add the parsed airline to the list
      _searchString._airlineList.push_back (_searchString._tmpAirline);
    }
    
    SearchString_T& _searchString;
  };
  
  /** Store the parsed airline name. */
  struct store_airline_name {
    store_airline_name (SearchString_T& ioSearchString)
      : _searchString (ioSearchString) {}
    
    void operator() (iterator_t iStr, iterator_t iStrEnd) const { 
      std::string lAirlineName (iStr, iStrEnd);
      _searchString._tmpAirline._name = lAirlineName;
      // std::cout << "Airline: " << lAirlineName << std::endl;

      // Add the parsed airline to the list
      _searchString._airlineList.push_back (_searchString._tmpAirline);
    }
    
    SearchString_T& _searchString;
  };
  
  /** Store the parsed number of passengers. */
  struct store_passenger_number {
    store_passenger_number (SearchString_T& ioSearchString)
      : _searchString (ioSearchString) {}

    void operator() (unsigned int iNumber) const { 
      _searchString._tmpPassenger._number = iNumber;
      // std::cout << "Number of passengers: " << iNumber << std::endl;
    }
    
    SearchString_T& _searchString;
  };
  
  /** Store the parsed passenger type. */
  struct store_adult_passenger_type {
    store_adult_passenger_type (SearchString_T& ioSearchString)
      : _searchString (ioSearchString) {}
    
    void operator() (iterator_t iStr, iterator_t iStrEnd) const { 
      std::string lPassengerType (iStr, iStrEnd);
      _searchString._tmpPassenger._type = Passenger_T::ADULT;
      // std::cout << "Passenger type: " << lPassengerType << std::endl;

      // Add the parsed passenger to the list
      _searchString._passengerList.push_back (_searchString._tmpPassenger);
    }
    
    SearchString_T& _searchString;
  };
  
  /** Store the parsed passenger type. */
  struct store_child_passenger_type {
    store_child_passenger_type (SearchString_T& ioSearchString)
      : _searchString (ioSearchString) {}
    
    void operator() (iterator_t iStr, iterator_t iStrEnd) const { 
      std::string lPassengerType (iStr, iStrEnd);
      _searchString._tmpPassenger._type = Passenger_T::CHILD;
      // std::cout << "Passenger type: " << lPassengerType << std::endl;

      // Add the parsed passenger to the list
      _searchString._passengerList.push_back (_searchString._tmpPassenger);
    }
    
    SearchString_T& _searchString;
  };
  
  /** Store the parsed passenger type. */
  struct store_pet_passenger_type {
    store_pet_passenger_type (SearchString_T& ioSearchString)
      : _searchString (ioSearchString) {}
    
    void operator() (iterator_t iStr, iterator_t iStrEnd) const { 
      std::string lPassengerType (iStr, iStrEnd);
      _searchString._tmpPassenger._type = Passenger_T::PET;
      // std::cout << "Passenger type: " << lPassengerType << std::endl;

      // Add the parsed passenger to the list
      _searchString._passengerList.push_back (_searchString._tmpPassenger);
    }
    
    SearchString_T& _searchString;
  };
  
}

// /////////// Utilities /////////////
/** 1-digit-integer parser */
boost::spirit::classic::int_parser<unsigned int, 10, 1, 1> int1_p;
/** 1-digit-integer parser */
boost::spirit::classic::uint_parser<unsigned int, 10, 1, 1> uint1_p;
/** Up-to-2-digit-integer parser */
boost::spirit::classic::uint_parser<unsigned int, 10, 1, 2> uint1_2_p;
/** 2-digit-integer parser */
boost::spirit::classic::uint_parser<unsigned int, 10, 2, 2> uint2_p;
/** Up-to-4-digit-integer parser */
boost::spirit::classic::uint_parser<unsigned int, 10, 2, 4> uint2_4_p;
/** 4-digit-integer parser */
boost::spirit::classic::uint_parser<unsigned int, 10, 4, 4> uint4_p;
/** Up-to-4-digit-integer parser */
boost::spirit::classic::uint_parser<unsigned int, 10, 1, 4> uint1_4_p;

///////////////////////////////////////////////////////////////////////////////
//
//  Our calculator grammar (using subrules)
//
///////////////////////////////////////////////////////////////////////////////
    /**
       Sample
       guadeloupe rio de janeiro 07/22/2009 +aa -ua 2 adults 1 dog

       Grammar:
       search_string      ::= places
                              [dates]
                              *(preferred_airlines)
                              *(passengers)
       dates              ::= board_date [off_date]
       places             ::= [board_place] off_place
       board_place        ::= place_elements
       off_place          ::= place_elements
       place_elements     ::= country | city | airport
       country            ::= country_code | country_name
       city               ::= city_code | city_name
       airport            ::= airport_code | airport_name
       preferred_airlines ::= [+|-] airline_code | airline_name
       passengers         ::= adult_number adult_description
                              [child_number child_description]
                              [pet_number pet_description]
       adult_description  ::= 'adult' | 'adults' | 'pax' | 'passengers'
       child_description  ::= 'child' | 'children' | 'kid' | 'kids'
       pet_description    ::= 'dog' | 'dogs' | 'cat' | 'cats'
    */

using namespace boost::spirit::classic;

/** Grammar for the search string parser. */
struct SearchStringParser : 
  public boost::spirit::classic::grammar<SearchStringParser> {

  SearchStringParser (SearchString_T& ioSearchString) 
  : _searchString (ioSearchString) {
  }

  template <typename ScannerT>
  struct definition {
    definition (SearchStringParser const& self) {

      search_string = places
        >> !( dates )
        >> *( preferred_airlines )
        >> *( passengers )
        ;

      places =
        +( place_element )
        ;

      place_element =
        lexeme_d[ (repeat_p(1,20)[chset_p("a-z")])[store_place_element(self._searchString)] ]
        ;
      
      dates =
        date[store_date(self._searchString)]
        >> !date[store_date(self._searchString)]
        ;
      
      date =
        ( month | day )
        >> boost::spirit::classic::chset_p("/-")
        >> ( day | month )
        >> ! ( boost::spirit::classic::chset_p("/-")
               >> year )
        ;

      day =
        lexeme_d[ limit_d(1u,31u)[uint1_2_p][assign_a(self._searchString._tmpDate._day)] ]
        ;
        
      month =
        lexeme_d[ limit_d(1u,12u)[uint1_2_p][assign_a(self._searchString._tmpDate._month)] ]
        ;

      year =
        lexeme_d[ limit_d(2000u,2099u)[uint4_p][assign_a(self._searchString._tmpDate._year)] ]
        | lexeme_d[ limit_d(0u,99u)[uint2_p][assign_a(self._searchString._tmpDate._year)] ]
        ;

      preferred_airlines =
        !(boost::spirit::classic::sign_p)[store_airline_sign(self._searchString)]
        >> airline_code | airline_name
        ;

      airline_code =
        lexeme_d[ (repeat_p(2,3)[chset_p("0-9a-z")])[store_airline_code(self._searchString)] ]
        ;
      
      airline_name =
        lexeme_d[ (repeat_p(4,20)[chset_p("0-9a-z")])[store_airline_name(self._searchString)] ]
        ;

      passengers =
        passenger_number >> passenger_type
        ;
      
      passenger_number =
        lexeme_d[ limit_d(1u, 9u)[uint1_p][store_passenger_number(self._searchString)] ]
        ;

      passenger_type =
        passenger_adult_type[store_adult_passenger_type(self._searchString)]
        | passenger_child_type[store_child_passenger_type(self._searchString)]
        | passenger_pet_type[store_pet_passenger_type(self._searchString)]
        ;

      passenger_adult_type =
        lexeme_d[ as_lower_d [ str_p("adult") >> !ch_p('s') ] ]
        ;
      
      passenger_child_type =
        lexeme_d[ as_lower_d [ str_p("child") >> !str_p("ren") ] ]
        ;
      
      passenger_pet_type =
        lexeme_d[ as_lower_d [ str_p("dog") | str_p("cat") >> !ch_p('s') ] ]
        ;
      
      BOOST_SPIRIT_DEBUG_NODE (search_string);
      BOOST_SPIRIT_DEBUG_NODE (places);
      BOOST_SPIRIT_DEBUG_NODE (place_element);
      BOOST_SPIRIT_DEBUG_NODE (dates);
      BOOST_SPIRIT_DEBUG_NODE (date);
      BOOST_SPIRIT_DEBUG_NODE (day);
      BOOST_SPIRIT_DEBUG_NODE (month);
      BOOST_SPIRIT_DEBUG_NODE (year);
      BOOST_SPIRIT_DEBUG_NODE (preferred_airlines);
      BOOST_SPIRIT_DEBUG_NODE (airline_code);
      BOOST_SPIRIT_DEBUG_NODE (airline_name);
      BOOST_SPIRIT_DEBUG_NODE (passengers);
      BOOST_SPIRIT_DEBUG_NODE (passenger_number);
      BOOST_SPIRIT_DEBUG_NODE (passenger_type);
      BOOST_SPIRIT_DEBUG_NODE (passenger_adult_type);
      BOOST_SPIRIT_DEBUG_NODE (passenger_child_type);
      BOOST_SPIRIT_DEBUG_NODE (passenger_pet_type);
    }
    
    boost::spirit::classic::rule<ScannerT> search_string, places, place_element,
       dates, date, month, day, year,
       preferred_airlines, airline_code, airline_name,
       passengers, passenger_number, passenger_type, passenger_adult_type,
       passenger_child_type, passenger_pet_type;

    boost::spirit::classic::rule<ScannerT> const& start() const { return search_string; }
  };

  SearchString_T& _searchString;
};

// //////////////////////////////////////////////////////////
bool parseSearchString (std::ostream& oStream,
                        const std::string& iSearchString,
                        SearchString_T& ioSearchStringStruct) {
  bool oHasBeenParsingSuccessful = false;
  
  // Read the search string
  iterator_t lStringIterator = iSearchString.c_str();

  // Instantiate the structure that will hold the result of the parsing.
  SearchStringParser lSearchStringParser (ioSearchStringStruct);
  boost::spirit::classic::parse_info<iterator_t> info =
    boost::spirit::classic::parse (lStringIterator, lSearchStringParser, 
                                   boost::spirit::classic::space_p);
  
  oStream << "-------------------------" << std::endl;
  
  oHasBeenParsingSuccessful = info.full;
  if (oHasBeenParsingSuccessful == true) {
    oStream << "Parsing succeeded" << std::endl;
    
  } else {
    oStream << "Parsing failed" << std::endl;
  }
  oStream << "-------------------------" << std::endl;
  
  return oHasBeenParsingSuccessful;
}

// /////////////// M A I N /////////////////
int main (int argc, char* argv[]) {
  try {
    
    // String to be parsed
    const std::string lSearchString1 ("guadeloupe rio de janeiro 07/22/2009 +aa -ua 2 adults 3 children 1 dog");
    const std::string lSearchString2 ("nce rekjavik 07-22-2009 +aa -ua 2 ADULTS 3 CHILDREN 1 DOG");
    const std::string lSearchString3 ("sna francisco ods 07.22.2009 +aa");
    const std::string lSearchString4 ("nce ods 07/22 +aa");
    const std::string lSearchString5 ("nce ods 22/07 +aa");
    const std::string lSearchString6 ("sfo rio lax 17/07 24/07");
    
    // Parse the search string
    SearchString_T lSearchStringStruct1;
    bool hasBeenParsingSuccessful = parseSearchString (std::cout,
                                                       lSearchString1,
                                                       lSearchStringStruct1);
    if (hasBeenParsingSuccessful == false) {
      std::cerr << "Error when parsing " << lSearchString1 << std::endl;
      return -1;
    }

    // Parse the search string
    SearchString_T lSearchStringStruct2;
    hasBeenParsingSuccessful = parseSearchString (std::cout, lSearchString2,
                                                  lSearchStringStruct2);

    // Parse the search string
    SearchString_T lSearchStringStruct3;
    hasBeenParsingSuccessful = parseSearchString (std::cout, lSearchString3,
                                                  lSearchStringStruct3);

    // Parse the search string
    SearchString_T lSearchStringStruct4;
    hasBeenParsingSuccessful = parseSearchString (std::cout, lSearchString4,
                                                  lSearchStringStruct4);

    // Parse the search string
    SearchString_T lSearchStringStruct5;
    hasBeenParsingSuccessful = parseSearchString (std::cout, lSearchString5,
                                                  lSearchStringStruct5);

    // Parse the search string
    SearchString_T lSearchStringStruct6;
    hasBeenParsingSuccessful = parseSearchString (std::cout, lSearchString6,
                                                  lSearchStringStruct6);

    // DEBUG
    std::cout << "--------------------------------------------" << std::endl;
    std::cout << "Search string1: " << lSearchString1 << std::endl;
    lSearchStringStruct1.display();

    std::cout << "--------------------------------------------" << std::endl;
    std::cout << "Search string2: " << lSearchString2 << std::endl;
    lSearchStringStruct2.display();

    std::cout << "--------------------------------------------" << std::endl;
    std::cout << "Search string3: " << lSearchString3 << std::endl;
    lSearchStringStruct3.display();

    std::cout << "--------------------------------------------" << std::endl;
    std::cout << "Search string4: " << lSearchString4 << std::endl;
    lSearchStringStruct4.display();

    std::cout << "--------------------------------------------" << std::endl;
    std::cout << "Search string5: " << lSearchString5 << std::endl;
    lSearchStringStruct5.display();

    std::cout << "--------------------------------------------" << std::endl;
    std::cout << "Search string6: " << lSearchString6 << std::endl;
    lSearchStringStruct6.display();

  } catch (const std::exception& stde) {
    std::cerr << "Standard exception: " << stde.what() << std::endl;
    return -1;
    
  } catch (...) {
    return -1;
  }
  
  return 0;
}
