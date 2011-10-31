## command sub-directory
include $(top_srcdir)/Makefile.common

SUBDIRS = icu utf8

MAINTAINERCLEANFILES = Makefile.in

check_PROGRAMS = boost_string loc2 stdlocru simple_io

boost_string_SOURCES = boost_string.cpp
boost_string_CXXFLAGS = $(BOOST_CFLAGS)
boost_string_LDFLAGS = $(BOOST_LIBS)
boost_string_LDADD =

loc2_SOURCES = loc2.cpp
loc2_CXXFLAGS = $(BOOST_CFLAGS)
loc2_LDFLAGS = $(BOOST_LIBS)

stdlocru_SOURCES = stdlocru.cpp
stdlocru_CXXFLAGS = $(BOOST_CFLAGS)
stdlocru_LDFLAGS = $(BOOST_LIBS)

simple_io_SOURCES = simple_io.cpp
simple_io_CXXFLAGS = $(BOOST_CFLAGS)
simple_io_LDFLAGS = $(BOOST_LIBS)

EXTRA_DIST =
