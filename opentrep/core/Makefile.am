## opentrep/core sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

## Source directory

MAINTAINERCLEANFILES = Makefile.in

SUBDIRS =


# Library
lib_LTLIBRARIES = lib@PACKAGE@.la

lib@PACKAGE@_la_SOURCES = $(service_h_sources) $(service_cc_sources)
lib@PACKAGE@_la_LIBADD = \
	$(top_builddir)/@PACKAGE@/basic/libopentrepbas.la \
	$(top_builddir)/@PACKAGE@/bom/libopentrepbom.la \
	$(top_builddir)/@PACKAGE@/factory/libopentrepfac.la \
	$(top_builddir)/@PACKAGE@/dbadaptor/libopentrepdba.la \
	$(top_builddir)/@PACKAGE@/command/libopentrepcmd.la \
	$(top_builddir)/@PACKAGE@/service/libopentrepsvc.la
lib@PACKAGE@_la_LDFLAGS = \
	$(BOOST_DATE_TIME_LIB) $(BOOST_PROGRAM_OPTIONS_LIB) \
	$(BOOST_FILESYSTEM_LIB) $(SOCI_LIBS) \
	-version-info $(GENERIC_LIBRARY_VERSION)

# Header files
pkginclude_HEADERS = $(service_h_sources)
nobase_nodist_pkginclude_HEADERS = $(top_builddir)/@PACKAGE@/config.h
