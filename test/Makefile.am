## test/opentrep sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

##
SUBDIRS = parsers

EXTRA_DIST = testIndexer.sh testSearcher.sh

##
check_PROGRAMS = IndexBuildingTestSuite
TESTS = $(check_PROGRAMS)
XFAIL_TESTS = #IndexBuildingTestSuite

#
IndexBuildingTestSuite_SOURCES = IndexBuildingTestSuite.hpp \
				IndexBuildingTestSuite.cpp
IndexBuildingTestSuite_CXXFLAGS= $(CPPUNIT_CFLAGS) $(BOOST_CFLAGS)
IndexBuildingTestSuite_LDADD =
IndexBuildingTestSuite_LDFLAGS = \
	$(BOOST_LIBS) $(SOCI_LIBS) $(XAPIAN_LIBS) $(CPPUNIT_LIBS) \
	$(top_builddir)/extracppunit/libextracc-extracppunit.la \
	$(top_builddir)/@PACKAGE@/core/lib@PACKAGE@.la
