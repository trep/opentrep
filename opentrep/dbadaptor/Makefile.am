## opentrep/dbadaptor sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

#
noinst_LTLIBRARIES = libopentrepdba.la

libopentrepdba_la_SOURCES= $(dba_h_sources) $(dba_cc_sources) 
libopentrepdba_la_CXXFLAGS = $(SOCI_CFLAGS)
libopentrepdba_la_LIBADD = $(SOCI_LIBS)

#
#pkgincludedir = $(includedir)/@PACKAGE@/dba
#pkginclude_HEADERS = $(dba_h_sources)
