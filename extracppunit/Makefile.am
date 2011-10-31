## extracppunit sub-directory
include $(srcdir)/sources.mk

#
AM_CPPFLAGS = -I$(top_srcdir)

MAINTAINERCLEANFILES = Makefile.in

#
lib_LTLIBRARIES = libextracppunit.la

#
libextracppunit_la_SOURCES = \
	$(extracppunit_h_sources) $(extracppunit_cc_sources)
libextracppunit_la_CXXFLAGS = $(CPPUNIT_CFLAGS)
libextracppunit_la_LDFLAGS = $(CPPUNIT_LIBS) \
	-version-info $(GENERIC_LIBRARY_VERSION)


# Header files
#pkgincludedir = $(includedir)/extracppunit
#pkginclude_HEADERS = $(extracppunit_h_sources)
nobase_pkginclude_HEADERS = $(top_srcdir)/extracppunit/CppUnitCore.hpp
