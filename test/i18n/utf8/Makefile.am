## command sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

check_PROGRAMS = utf8 utf8string

utf8_SOURCES = utf8.cpp
utf8_CXXFLAGS =
utf8_LDFLAGS =

utf8string_SOURCES = utf8string.cpp
utf8string_CXXFLAGS =
utf8string_LDFLAGS = \
		$(BOOST_LIBS) $(SOCI_LIBS) $(CPPUNIT_LIBS) \
		$(top_builddir)/@PACKAGE@/lib@PACKAGE@.la

EXTRA_DIST =
