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
// OpenTREP
#include <opentrep/config/opentrep-paths.hpp>

// //////////// Constants for the tests ///////////////
/**
 * File-path of the POR (points of reference) file.
 */
const std::string K_POR_FILEPATH (OPENTREP_POR_DATA_DIR
                                  "/test_world_schedule.csv");


// Type definitions
typedef char char_t;
//typedef char const* iterator_t;
typedef boost::spirit::classic::file_iterator<char_t> iterator_t;
typedef boost::spirit::classic::scanner<iterator_t> scanner_t;
typedef boost::spirit::classic::rule<scanner_t> rule_t;

/** LegCabin-Details. */
struct Cabin_T {
  // Attributes
  std::string _cabinCode;
  double _capacity;

  void display() const {
    std::cout << "        " << _cabinCode << " " << _capacity << ", ";
  }
};

/** List of Cabin-Detail strucutres. */
typedef std::vector<Cabin_T> CabinList_T;

/** Leg. */
struct Leg_T {
  // Attributes
  std::string _boardPoint;
  boost::posix_time::time_duration _boardTime;
  boost::gregorian::date_duration _boardDateOffSet;
  std::string _offPoint;
  boost::posix_time::time_duration _offTime;
  boost::gregorian::date_duration _offDateOffSet;
  boost::posix_time::time_duration _elapsed;
  CabinList_T _cabinList;

  /** Constructor. */
  Leg_T () : _boardDateOffSet (0), _offDateOffSet (0) {}
  
  void display() const {
    std::cout << "    " << _boardPoint << " / "
              << boost::posix_time::to_simple_string (_boardTime)
              << " -- " << _offPoint << " / "
              << boost::posix_time::to_simple_string (_offTime)
              << " --> " << boost::posix_time::to_simple_string (_elapsed)
              << std::endl;
    for (CabinList_T::const_iterator itCabin = _cabinList.begin();
	 itCabin != _cabinList.end(); itCabin++) {
      const Cabin_T& lCabin = *itCabin;
      lCabin.display();
    }
    std::cout << std::endl;
  }
};

/** List of Leg strucutres. */
typedef std::vector<Leg_T> LegList_T;

/** SegmentCabin-Details. */
struct SegmentCabin_T {
  // Attributes
  std::string _cabinCode;
  std::string _classes;

  void display() const {
    std::cout << "        " << _cabinCode << " " << _classes << ", ";
  }
};

/** List of SegmentCabin-Detail strucutres. */
typedef std::vector<SegmentCabin_T> SegmentCabinList_T;

/** Segment. */
struct Segment_T {
  // Attributes
  std::string _boardPoint;
  boost::posix_time::time_duration _boardTime;
  boost::gregorian::date_duration _boardDateOffSet;
  std::string _offPoint;
  boost::posix_time::time_duration _offTime;
  boost::gregorian::date_duration _offDateOffSet;
  boost::posix_time::time_duration _elapsed;
  SegmentCabinList_T _cabinList;
  
  /** Constructor. */
  Segment_T () : _boardDateOffSet (0), _offDateOffSet (0) {}
  
  void display() const {
    std::cout << "    " << _boardPoint << " / "
              << boost::posix_time::to_simple_string (_boardTime)
              << " -- " << _offPoint << " / "
              << boost::posix_time::to_simple_string (_offTime)
              << " --> " << boost::posix_time::to_simple_string (_elapsed)
              << std::endl;
    for (SegmentCabinList_T::const_iterator itCabin = _cabinList.begin();
	 itCabin != _cabinList.end(); itCabin++) {
      const SegmentCabin_T& lCabin = *itCabin;
      lCabin.display();
    }
    std::cout << std::endl;
  }
};

/** List of Segment strucutres. */
typedef std::vector<Segment_T> SegmentList_T;

/** Flight-Period. */
struct FlightPeriod_T {
  // Attributes
  std::string _airlineCode;
  unsigned int _flightNumber;
  boost::gregorian::date _dateRangeStart;
  boost::gregorian::date _dateRangeEnd;
  std::string _dow;
  LegList_T _legList;
  SegmentList_T _segmentList;
  
  /** Constructor. */
  FlightPeriod_T () : _legAlreadyDefined (false), _itSeconds (0) {}

  /** Set the date from the staging details. */
  boost::gregorian::date getDate() const {
    return boost::gregorian::date (_itYear, _itMonth, _itDay);
  }

  /** Set the time from the staging details. */
  boost::posix_time::time_duration getTime() const {
    return boost::posix_time::hours (_itHours)
      + boost::posix_time::minutes (_itMinutes)
      + boost::posix_time::seconds (_itSeconds);
  }
  
  void display() const {
    std::cout << _airlineCode << _flightNumber
              << ", " << boost::gregorian::to_simple_string (_dateRangeStart)
              << " - " << boost::gregorian::to_simple_string (_dateRangeEnd)
              << " - " << _dow
              << std::endl;
    
    for (LegList_T::const_iterator itLeg = _legList.begin();
	 itLeg != _legList.end(); itLeg++) {
      const Leg_T& lLeg = *itLeg;
      lLeg.display();
    }

    for (SegmentList_T::const_iterator itSegment = _segmentList.begin();
	 itSegment != _segmentList.end(); itSegment++) {
      const Segment_T& lSegment = *itSegment;
      lSegment.display();
    }
    
    //std::cout << "[Debug] - Staging Leg: ";
    //_itLeg.display();
    //std::cout << "[Debug] - Staging Cabin: ";
    //_itCabin.display();
    //std::cout << "[Debug] - Staging Segment: ";
    //_itSegment.display();
  }

  /** Add the given airport to the internal lists (if not already existing). */
  void addAirport (const std::string& iAirport) {
    std::set<std::string>::const_iterator itAirport =
      _airportList.find (iAirport);
    if (itAirport == _airportList.end()) {
      // Add the airport code to the airport set
      const bool insertSuccessful = _airportList.insert (iAirport).second;
      
      if (insertSuccessful == false) {
        // TODO: throw an exception
      }
      // Add the airport code to the airport vector
      _airportOrderedList.push_back (iAirport);
    }
  }
  
  /** Build the routing (segments). */
  void buildSegments () {
    // The list of airports encompasses all the airports on which
    // the flight takes off or lands. Moreover, that list is
    // time-ordered: the first airport is the initial departure of
    // the flight, and the last airport is the eventual point of
    // rest of the flight.
    // Be l the size of the ordered list of airports.
    // We want to generate all the segment combinations from the legs
    // and, hence, from all the possible (time-ordered) airport pairs.
    // Thus, we both iterator on i=0...l-1 and j=i+1...l
    assert (_airportOrderedList.size() >= 2);

    _segmentList.clear();
    for (std::vector<std::string>::const_iterator itAirport_i =
           _airportOrderedList.begin();
         itAirport_i != _airportOrderedList.end()-1; ++itAirport_i) {
      for (std::vector<std::string>::const_iterator itAirport_j =
             itAirport_i + 1;
           itAirport_j != _airportOrderedList.end(); ++itAirport_j) {
        Segment_T lSegment;
        lSegment._boardPoint = *itAirport_i;
        lSegment._offPoint = *itAirport_j;
        
        _segmentList.push_back (lSegment);
      }
    }

    // Clear the lists of airports, so that it is ready for the next flight
    _airportList.clear();
    _airportOrderedList.clear();
  }
  
  /** Add, to the Segment whose key corresponds to the
      given (board point, off point) pair, the specific segment cabin
      details (mainly, the list of the class codes).
      <br>Note that the Segment structure is retrieved from the internal
      list, already filled by a previous step (the buildSegments()
      method). */
  void addSegmentCabin (const Segment_T& iSegment,
                        const SegmentCabin_T& iCabin) {
    // Retrieve the Segment structure corresponding to the (board, off) point
    // pair.
    SegmentList_T::iterator itSegment = _segmentList.begin();
    for ( ; itSegment != _segmentList.end(); ++itSegment) {
      const Segment_T& lSegment = *itSegment;
      
      const std::string& lBoardPoint = iSegment._boardPoint;
      const std::string& lOffPoint = iSegment._offPoint;
      if (lSegment._boardPoint == lBoardPoint
          && lSegment._offPoint == lOffPoint) {
        break;
      }
    }
    
    // If the segment key (airport pair) given in the schedule input file
    // does not correspond to the leg (board, off) points, throw an exception
    // so that the user knows the schedule input file is corrupted.
    if (itSegment == _segmentList.end()) {
      std::cerr << "Within the schedule input file, there is a flight for which the airports of segments and those of the legs do not correspond.";
      throw std::exception();
    }
    
    // Add the Cabin structure to the Segment Cabin structure.
    assert (itSegment != _segmentList.end());
    Segment_T& lSegment = *itSegment;
    lSegment._cabinList.push_back (iCabin);
  }
      
  /** Add, to all the Segment, the general segment cabin details
      (mainly, the list of the class codes).
      <br>Note that the Segment structures are stored within the internal
      list, already filled by a previous step (the buildSegments()
      method). */
  void addSegmentCabin (const SegmentCabin_T& iCabin) {
    // Iterate on all the Segment (as they get the same cabin definitions)
    for (SegmentList_T::iterator itSegment = _segmentList.begin();
         itSegment != _segmentList.end(); ++itSegment) {
      Segment_T& lSegment = *itSegment;
      lSegment._cabinList.push_back (iCabin);
    }
  }
  
  /** Staging Leg (resp. Cabin) structure, gathering the result of the iteration
      on one leg (resp. cabin). */
  bool _legAlreadyDefined;
  Leg_T _itLeg;
  Cabin_T _itCabin;

  /** Staging Date. */
  unsigned int _itYear;
  unsigned int _itMonth;
  unsigned int _itDay;

  /** Staging Time. */
  long _itHours;
  long _itMinutes;
  long _itSeconds;
  int _dateOffSet;

  /** Staging Airport List (helper to derive the list of Segment
      structures). */
  std::set<std::string> _airportList;
  std::vector<std::string> _airportOrderedList;

  /** Staging Segment-related attributes. */
  bool _areSegmentDefinitionsSpecific;
  Segment_T _itSegment;
  SegmentCabin_T _itSegmentCabin;
};

///////////////////////////////////////////////////////////////////////////////
//
//  Semantic actions
//
///////////////////////////////////////////////////////////////////////////////
namespace {
  
  /** Store the parsed airline code. */
  struct store_airline_code {
    store_airline_code (FlightPeriod_T& ioFlightPeriod)
      : _flightPeriod (ioFlightPeriod) {}
    
    void operator() (iterator_t iStr, iterator_t iStrEnd) const { 
      std::string lAirlineCode (iStr, iStrEnd);
      _flightPeriod._airlineCode = lAirlineCode;
      // std::cout << "Airline code: " << lAirlineCode << std::endl;

      // As that's the beginning of a new flight, the list of legs must be reset
      _flightPeriod._legList.clear();
    }
    
    FlightPeriod_T& _flightPeriod;
  };
  
  /** Store the parsed flight number. */
  struct store_flight_number {
    store_flight_number (FlightPeriod_T& ioFlightPeriod)
      : _flightPeriod (ioFlightPeriod) {}

    void operator() (unsigned int iNumber) const { 
      _flightPeriod._flightNumber = iNumber;
      // std::cout << "Flight number: " << iNumber << std::endl;
    }
    
    FlightPeriod_T& _flightPeriod;
  };
  
  /** Store the start of the date range. */
  struct store_date_range_start {
    store_date_range_start (FlightPeriod_T& ioFlightPeriod)
      : _flightPeriod (ioFlightPeriod) {}
    
    void operator() (iterator_t iStr, iterator_t iStrEnd) const {
      _flightPeriod._dateRangeStart = _flightPeriod.getDate();
      // std::cout << "Date Range Start: "
      // << _flightPeriod._dateRangeStart << std::endl;
      
      // Reset the number of seconds
      _flightPeriod._itSeconds = 0;
    }
    
    FlightPeriod_T& _flightPeriod;
  };

  /** Store the end of the date range. */
  struct store_date_range_end {
    store_date_range_end (FlightPeriod_T& ioFlightPeriod)
      : _flightPeriod (ioFlightPeriod) {}
    
    void operator() (iterator_t iStr, iterator_t iStrEnd) const {
      _flightPeriod._dateRangeEnd = _flightPeriod.getDate();
      // std::cout << "Date Range End: "
      // << _flightPeriod._dateRangeEnd << std::endl;
      
      // Reset the number of seconds
      _flightPeriod._itSeconds = 0;
    }
    
    FlightPeriod_T& _flightPeriod;
  };

  /** Store the DOW (day of the Week). */
  struct store_dow {
    store_dow (FlightPeriod_T& ioFlightPeriod)
      : _flightPeriod (ioFlightPeriod) {}

    void operator() (iterator_t iStr, iterator_t iStrEnd) const {
      std::string lDow (iStr, iStrEnd);
      _flightPeriod._dow = lDow;
      // std::cout << "DOW: " << lDow << std::endl;
    }
    
    FlightPeriod_T& _flightPeriod;
  };

  /** Store the parsed board point. */
  struct store_board_point {
    store_board_point (FlightPeriod_T& ioFlightPeriod)
      : _flightPeriod (ioFlightPeriod) {}

    void operator() (iterator_t iStr, iterator_t iStrEnd) const {
      std::string lBoardPoint (iStr, iStrEnd);
      // std::cout << "Board point: " << lBoardPoint << std::endl;

      // If a leg has already been parsed, add it to the FlightPeriod
      if (_flightPeriod._legAlreadyDefined == true) {
        _flightPeriod._legList.push_back (_flightPeriod._itLeg);
      } else {
        _flightPeriod._legAlreadyDefined = true;
      }

      // Set the (new) board point
      _flightPeriod._itLeg._boardPoint = lBoardPoint;
      
      // As that's the beginning of a new leg, the list of cabins must be reset
      _flightPeriod._itLeg._cabinList.clear();

      // Add the airport code if it is not already stored in the airport lists
      _flightPeriod.addAirport (lBoardPoint);
    }
    
    FlightPeriod_T& _flightPeriod;
  };
  
  /** Store the parsed off point. */
  struct store_off_point {
    store_off_point (FlightPeriod_T& ioFlightPeriod)
      : _flightPeriod (ioFlightPeriod) {}

    void operator() (iterator_t iStr, iterator_t iStrEnd) const {
      std::string lOffPoint (iStr, iStrEnd);
      _flightPeriod._itLeg._offPoint = lOffPoint;
      // std::cout << "Off point: " << lOffPoint << std::endl;

      // Add the airport code if it is not already stored in the airport lists
      _flightPeriod.addAirport (lOffPoint);
    }
    
    FlightPeriod_T& _flightPeriod;
  };

  /** Store the board time. */
  struct store_board_time {
    store_board_time (FlightPeriod_T& ioFlightPeriod)
      : _flightPeriod (ioFlightPeriod) {}
    
    void operator() (iterator_t iStr, iterator_t iStrEnd) const {
      _flightPeriod._itLeg._boardTime = _flightPeriod.getTime();
      
      // Reset the number of seconds
      _flightPeriod._itSeconds = 0;

      // Reset the date off-set
      _flightPeriod._dateOffSet = 0;
    }
    
    FlightPeriod_T& _flightPeriod;
  };

  /** Store the off time. */
  struct store_off_time {
    store_off_time (FlightPeriod_T& ioFlightPeriod)
      : _flightPeriod (ioFlightPeriod) {}
    
    void operator() (iterator_t iStr, iterator_t iStrEnd) const {
      _flightPeriod._itLeg._offTime = _flightPeriod.getTime();
      
      // Reset the number of seconds
      _flightPeriod._itSeconds = 0;

      // As the board date off set is optional, it can be set only afterwards,
      // based on the staging date off-set value (_flightPeriod._dateOffSet).
      const boost::gregorian::date_duration lDateOffSet (_flightPeriod._dateOffSet);
      _flightPeriod._itLeg._boardDateOffSet = lDateOffSet;
    }
    
    FlightPeriod_T& _flightPeriod;
  };

  /** Store the elapsed time. */
  struct store_elapsed_time {
    store_elapsed_time (FlightPeriod_T& ioFlightPeriod)
      : _flightPeriod (ioFlightPeriod) {}
    
    void operator() (iterator_t iStr, iterator_t iStrEnd) const {
      _flightPeriod._itLeg._elapsed = _flightPeriod.getTime();
      
      // Reset the number of seconds
      _flightPeriod._itSeconds = 0;

      // As the board date off set is optional, it can be set only afterwards,
      // based on the staging date off-set value (_flightPeriod._dateOffSet).
      const boost::gregorian::date_duration lDateOffSet (_flightPeriod._dateOffSet);
      _flightPeriod._itLeg._offDateOffSet = lDateOffSet;
    }
    
    FlightPeriod_T& _flightPeriod;
  };

  /** Store the parsed cabin code. */
  struct store_cabin_code {
    store_cabin_code (FlightPeriod_T& ioFlightPeriod)
      : _flightPeriod (ioFlightPeriod) {}
    
    void operator() (char iChar) const { 
      _flightPeriod._itCabin._cabinCode = iChar; 
      // std::cout << "Cabin code: " << iChar << std::endl;
    }

    FlightPeriod_T& _flightPeriod;
  };
  
  /** Store the parsed capacity. */
  struct store_capacity {
    store_capacity (FlightPeriod_T& ioFlightPeriod)
      : _flightPeriod (ioFlightPeriod) {}
    
    void operator() (double iReal) const { 
      _flightPeriod._itCabin._capacity = iReal; 
      // std::cout << "Capacity: " << iReal << std::endl;

      // The capacity is the last (according to arrival order) detail
      // of the cabin. Hence, when a capacity is parsed, it means that
      // the full cabin details have already been parsed as well: the
      // cabin can thus be added to the leg.
      _flightPeriod._itLeg._cabinList.push_back (_flightPeriod._itCabin);
    }

    FlightPeriod_T& _flightPeriod;
  };
  
  /** Store whether or not all the segments are the same. */
  struct store_segment_specificity {
      store_segment_specificity (FlightPeriod_T& ioFlightPeriod)
        : _flightPeriod (ioFlightPeriod) {
      }

    void operator() (char iChar) const {
      if (iChar == '0') {
        _flightPeriod._areSegmentDefinitionsSpecific = false;
      } else {
        _flightPeriod._areSegmentDefinitionsSpecific = true;
      }

      // Do a few sanity checks: the two lists should get exactly the same
      // content (in terms of airport codes). The only difference is that one
      // is a STL set, and the other a STL vector.
      assert (_flightPeriod._airportList.size()
              == _flightPeriod._airportOrderedList.size());
      assert (_flightPeriod._airportList.size() >= 2);
      
      // Since all the legs have now been parsed, we get all the airports
      // and the segments may be built.
      _flightPeriod.buildSegments();
    }

    FlightPeriod_T& _flightPeriod;
  };
      
  /** Store the board point of the segment. */
  struct store_segment_board_point {
    store_segment_board_point (FlightPeriod_T& ioFlightPeriod)
      : _flightPeriod (ioFlightPeriod) {
    }

    void operator() (iterator_t iStr, iterator_t iStrEnd) const {
      std::string lBoardPoint (iStr, iStrEnd);
      _flightPeriod._itSegment._boardPoint = lBoardPoint;
      // std::cout << "Board point: " << lBoardPoint << std::endl;
    }

    FlightPeriod_T& _flightPeriod;
  };

  /** Store the off point of the segment. */
  struct store_segment_off_point {
    store_segment_off_point (FlightPeriod_T& ioFlightPeriod)
      : _flightPeriod (ioFlightPeriod) {
    }

    void operator() (iterator_t iStr, iterator_t iStrEnd) const {
      std::string lOffPoint (iStr, iStrEnd);
      _flightPeriod._itSegment._offPoint = lOffPoint;
      // std::cout << "Off point: " << lOffPoint << std::endl;
    }

    FlightPeriod_T& _flightPeriod;
  };

  /** Store the off point of the segment. */
  struct store_segment_cabin_code {
    store_segment_cabin_code (FlightPeriod_T& ioFlightPeriod)
      : _flightPeriod (ioFlightPeriod) {
    }
    
    void operator() (char iChar) const { 
      _flightPeriod._itSegmentCabin._cabinCode = iChar; 
      // std::cout << "Cabin code: " << iChar << std::endl;
    }

    FlightPeriod_T& _flightPeriod;
  };

  /** Store the classes of the segment-cabin. */
  struct store_classes {
    store_classes (FlightPeriod_T& ioFlightPeriod)
      : _flightPeriod (ioFlightPeriod) {
    }

    void operator() (iterator_t iStr, iterator_t iStrEnd) const {
      std::string lClasses (iStr, iStrEnd);
      _flightPeriod._itSegmentCabin._classes = lClasses;
      // std::cout << "Classes: " << lClasses << std::endl;

      // The list of classes is the last (according to the arrival order
      // within the schedule input file) detail of the segment cabin. Hence,
      // when a list of classes is parsed, it means that the full segment
      // cabin details have already been parsed as well: the segment cabin
      // can thus be added to the segment.
      if (_flightPeriod._areSegmentDefinitionsSpecific == true) {
        _flightPeriod.addSegmentCabin (_flightPeriod._itSegment,
                                       _flightPeriod._itSegmentCabin);
      } else {
        _flightPeriod.addSegmentCabin (_flightPeriod._itSegmentCabin);
      }
    }

    FlightPeriod_T& _flightPeriod;
  };
  
  /** Mark the end of the flight-period parsing. */
  struct do_end_flight {
    do_end_flight (FlightPeriod_T& ioFlightPeriod)
      : _flightPeriod (ioFlightPeriod) {}
    
    // void operator() (char iChar) const {
    void operator() (iterator_t iStr, iterator_t iStrEnd) const {
      // std::cout << "End of Flight-Period " << std::endl;

      assert (_flightPeriod._legAlreadyDefined == true);
      _flightPeriod._legList.push_back (_flightPeriod._itLeg);
      
      // The lists of legs and cabins must be reset
      _flightPeriod._legAlreadyDefined = false;
      _flightPeriod._itLeg._cabinList.clear();

      // Display the result
      _flightPeriod.display();
    }

    FlightPeriod_T& _flightPeriod;
  };
}

// /////////// Utilities /////////////
/** 1-digit-integer parser */
boost::spirit::classic::int_parser<unsigned int, 10, 1, 1> int1_p;
/** 2-digit-integer parser */
boost::spirit::classic::uint_parser<unsigned int, 10, 2, 2> uint2_p;
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
         AirlineCode; FlightNumber; DateRangeStart; DateRangeEnd; DOW;
         (list) BoardPoint; OffPoint; BoardTime; DateOffSet; OffTime;
                ElapsedTime;
         (list) CabinCode; Capacity;
         SegmentSpecificty (0 or 1);
           (list) (optional BoardPoint; OffPoint); CabinCode; Classes
           
       BA; 9; 2007-04-20; 2007-06-30; 0000011;
            LHR; BKK; 22:00; 15:15 / +1; 11:15; F; 5; J; 12; W; 20; Y; 300;
            BKK; SYD; 18:10 / +1; 06:05 / +2; 08:55; F; 5; J; 12; W; 20; Y; 300;
            0; F; FA; J; JCDI; W; WT; Y; YBHKMLSQ;
       BA; 9; 2007-04-20; 2007-06-30; 1111100;
            LHR; BKK; 22:00; 15:15 / +1; 11:15; F; 5; J; 12; W; 20; Y; 300;
            BKK; SYD; 18:10 / +1; 06:05 / +2; 08:55; F; 5; J; 12; W; 20; Y; 300;
            1; LHR; BKK; F; FA; J; JCDI; W; WT; Y; YBHKMLSQ;
               BKK; SYD; F; FA; J; JCDI; W; WT; Y; YBHKMLSQ;
               LHR; SYD; F; FA; J; JCDI; W; WT; Y; YBHKMLSQ;

         Grammar:
         DOW                 ::= int
         FlightKey           ::= AirlineCode ';' FlightNumber
                                 ';' DateRangeStart ';' DateRangeEnd ';' DOW
         LegKey              ::= BoardPoint ';' OffPoint
         LegDetails          ::= BoardTime ['/' BoardDateOffSet]
                                 ';' OffTime ['/' BoardDateOffSet]
                                 ';' Elapsed
         LegCabinDetails     ::= CabinCode ';' Capacity
         Leg                 ::= LegKey ';' LegDetails (';' CabinDetails)+
         SegmentKey          ::= BoardPoint ';' OffPoint
         SegmentCabinDetails ::= CabinCode ';' Classes
         FullSegmentCabinDetails::= (';' SegmentCabinDetails)+
         GeneralSegments     ::= '0' (';' SegmentCabinDetails)+
         SpecificSegments    ::= '1' (';' SegmentKey
                                      ';' FullSegmentCabinDetails)+
         Segment             ::= GeneralSegment | SpecificSegment
         FlightPeriod        ::= FlightKey (';' Leg)+
                                 (';' Segment)+ ';' EndOfFlight
         EndOfFlight         ::= ';'
    */

using namespace boost::spirit::classic;

/** Grammar for the Flight-Period parser. */
struct FlightPeriodParser : 
  public boost::spirit::classic::grammar<FlightPeriodParser> {

  FlightPeriodParser (FlightPeriod_T& ioFlightPeriod) 
  : _flightPeriod (ioFlightPeriod) {
  }

  template <typename ScannerT>
  struct definition {
    definition (FlightPeriodParser const& self) {

      flight_period_list = *( boost::spirit::classic::comment_p("//")
                              | boost::spirit::classic::comment_p("/*", "*/")
                              | flight_period )
        ;
      
      flight_period = flight_key
        >> +( ';' >> leg )
        >> +( ';' >> segment )
        >> flight_period_end[do_end_flight(self._flightPeriod)]
        ;

      flight_period_end =
        boost::spirit::classic::ch_p(';')
        ;
      
      flight_key = airline_code
        >> ';' >> flight_number
        >> ';' >> date[store_date_range_start(self._flightPeriod)]
        >> ';' >> date[store_date_range_end(self._flightPeriod)]
        >> ';' >> dow[store_dow(self._flightPeriod)]
        ;

      airline_code =
        lexeme_d[ (repeat_p(2,3)[chset_p("0-9A-Z")])[store_airline_code(self._flightPeriod)] ]
        ;
      
      flight_number =
        lexeme_d[ limit_d(0u, 9999u)[uint1_4_p][store_flight_number(self._flightPeriod)] ]
        ;

      date =
        lexeme_d[ limit_d(2000u,2099u)[uint4_p][assign_a(self._flightPeriod._itYear)]
                  >> '-' >> limit_d(1u,12u)[uint2_p][assign_a(self._flightPeriod._itMonth)]
                  >> '-' >> limit_d(1u,31u)[uint2_p][assign_a(self._flightPeriod._itDay)] ]
        ;

      dow =
        lexeme_d[ repeat_p(7)[chset_p("0-1")] ]
        ;
      
      leg = leg_key >> ';' >> leg_details >> +( ';' >> cabin_details )
        ;
	 
      leg_key =
        (repeat_p(3)[chset_p("0-9A-Z")])[store_board_point(self._flightPeriod)]
        >> ';'
        >> (repeat_p(3)[chset_p("0-9A-Z")])[store_off_point(self._flightPeriod)]
        ;
	 
      leg_details =
        time[store_board_time(self._flightPeriod)]
        >> !(date_offset)
        >> ';' 
        >> time[store_off_time(self._flightPeriod)]
        >> !(date_offset)
        >> ';'
        >> time[store_elapsed_time(self._flightPeriod)]
        ;

      time = lexeme_d[ limit_d(0u,23u)[uint2_p][assign_a(self._flightPeriod._itHours)]
                       >> ':' >> limit_d(0u,59u)[uint2_p][assign_a(self._flightPeriod._itMinutes)]
                       >> !(':' >> limit_d(0u,59u)[uint2_p][assign_a(self._flightPeriod._itSeconds)]) ]
        ;

      date_offset =
        boost::spirit::classic::ch_p('/')
        >> (int1_p)[boost::spirit::classic::assign_a(self._flightPeriod._dateOffSet)]
        ;          
        
      cabin_details = (chset_p("A-Z"))[store_cabin_code(self._flightPeriod)]
        >> ';' >> (boost::spirit::classic::ureal_p)[store_capacity(self._flightPeriod)]
        ;

      segment_key =
        (repeat_p(3)[chset_p("0-9A-Z")])[store_segment_board_point(self._flightPeriod)]
        >> ';'
        >> (repeat_p(3)[chset_p("0-9A-Z")])[store_segment_off_point(self._flightPeriod)]
        ;
	 
      segment =
        general_segments | specific_segments
        ;

      general_segments =
        boost::spirit::classic::ch_p('0')[store_segment_specificity(self._flightPeriod)]
        >> +(';' >> segment_cabin_details)
        ;

      specific_segments =
        boost::spirit::classic::ch_p('1')[store_segment_specificity(self._flightPeriod)]
        >> +(';' >> segment_key >> full_segment_cabin_details)
        ;
      
      full_segment_cabin_details =
        +(';' >> segment_cabin_details)
        ;
      
      segment_cabin_details =
        (chset_p("A-Z"))[store_segment_cabin_code(self._flightPeriod)]
        >> ';' >> (repeat_p(1,26)[chset_p("A-Z")])[store_classes(self._flightPeriod)]
        ;
      
      BOOST_SPIRIT_DEBUG_NODE (flight_period_list);
      BOOST_SPIRIT_DEBUG_NODE (flight_period);
      BOOST_SPIRIT_DEBUG_NODE (flight_period_end);
      BOOST_SPIRIT_DEBUG_NODE (flight_key);
      BOOST_SPIRIT_DEBUG_NODE (airline_code);
      BOOST_SPIRIT_DEBUG_NODE (flight_number);
      BOOST_SPIRIT_DEBUG_NODE (date);
      BOOST_SPIRIT_DEBUG_NODE (dow);
      BOOST_SPIRIT_DEBUG_NODE (leg);
      BOOST_SPIRIT_DEBUG_NODE (leg_key);
      BOOST_SPIRIT_DEBUG_NODE (leg_details);
      BOOST_SPIRIT_DEBUG_NODE (time);
      BOOST_SPIRIT_DEBUG_NODE (date_offset);
      BOOST_SPIRIT_DEBUG_NODE (cabin_details);
      BOOST_SPIRIT_DEBUG_NODE (segment);
      BOOST_SPIRIT_DEBUG_NODE (segment_key);
      BOOST_SPIRIT_DEBUG_NODE (general_segments);
      BOOST_SPIRIT_DEBUG_NODE (specific_segments);
      BOOST_SPIRIT_DEBUG_NODE (full_segment_cabin_details);
      BOOST_SPIRIT_DEBUG_NODE (segment_cabin_details);
    }
    
    boost::spirit::classic::rule<ScannerT> flight_period_list, flight_period,
      flight_period_end, flight_key, airline_code, flight_number,
      date, dow, leg, leg_key, leg_details, time, date_offset, cabin_details,
      segment, segment_key, general_segments, specific_segments,
      full_segment_cabin_details, segment_cabin_details;

    boost::spirit::classic::rule<ScannerT> const& start() const { return flight_period_list; }
  };

  FlightPeriod_T& _flightPeriod;
};

// /////////////// M A I N /////////////////
int main (int argc, char* argv[]) {
  try {
    
    // File to be parsed
    std::string lFilename (K_POR_FILEPATH);
    
    // Read the command-line parameters
    if (argc >= 1 && argv[1] != NULL) {
      std::istringstream istr (argv[1]);
      istr >> lFilename;
    }

    // Open the file
    iterator_t lFileIterator (lFilename);
    if (!lFileIterator) {
      std::cerr << "The file " << lFilename << " can not be open." << std::endl;
      return -1;
    }

    // Create an EOF iterator
    iterator_t lFileIteratorEnd = lFileIterator.make_end();
    
    // Instantiate the structure that will hold the result of the parsing.
    FlightPeriod_T lFlightPeriod;
    FlightPeriodParser lFlightPeriodParser (lFlightPeriod);
    boost::spirit::classic::parse_info<iterator_t> info =
      boost::spirit::classic::parse (lFileIterator, lFileIteratorEnd,
                            lFlightPeriodParser, 
                            boost::spirit::classic::space_p);

    // DEBUG
    std::cout << "Flight Period:" << std::endl;
    lFlightPeriod.display();

    std::cout << "-------------------------" << std::endl;
    if (info.full) {
      std::cout << "Parsing succeeded" << std::endl;
      
    } else {
      std::cout << "Parsing failed" << std::endl;
    }
    std::cout << "-------------------------" << std::endl;

  } catch (const std::exception& stde) {
    std::cerr << "Standard exception: " << stde.what() << std::endl;
    return -1;
    
  } catch (...) {
    return -1;
  }
  
  return 0;
}
