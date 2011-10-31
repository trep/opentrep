## command sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

check_PROGRAMS = full_calculator parameter_parser schedule_parser \
	search_string_parser levenshtein

full_calculator_SOURCES = full_calculator.cpp
full_calculator_CXXFLAGS = $(BOOST_CFLAGS)
full_calculator_LDADD = $(BOOST_LIB)

parameter_parser_SOURCES = parameter_parser.cpp
parameter_parser_CXXFLAGS = $(BOOST_CFLAGS)
parameter_parser_LDADD = $(BOOST_LIB)

schedule_parser_SOURCES = schedule_parser.cpp
schedule_parser_CXXFLAGS = $(BOOST_CFLAGS)
schedule_parser_LDADD = $(BOOST_LIBS) $(BOOST_DATE_TIME_LIB)

search_string_parser_SOURCES = search_string_parser.cpp
search_string_parser_CXXFLAGS = $(BOOST_CFLAGS)
search_string_parser_LDADD = $(BOOST_LIBS) $(BOOST_DATE_TIME_LIB)

levenshtein_SOURCES = levenshtein.cpp
levenshtein_LDADD =

EXTRA_DIST = test_full_calculator.sh test_parameter_parser.sh \
		test_schedule_parser.sh
