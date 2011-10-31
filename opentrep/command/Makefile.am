## opentrep/command sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

#
noinst_LTLIBRARIES = libopentrepcmd.la

libopentrepcmd_la_SOURCES= $(cmd_h_sources) $(cmd_cc_sources) 
libopentrepcmd_la_CXXFLAGS = $(SOCI_CFLAGS) $(XAPIAN_CFLAGS) \
	$(MYSQL_CFLAGS) $(BOOST_CFLAGS)
libopentrepcmd_la_LDFLAGS = $(SOCI_LIBS) $(XAPIAN_LIBS) $(MYSQL_LIBS)

#
#pkgincludedir = $(includedir)/@PACKAGE@/command
#pkginclude_HEADERS = $(cmd_h_sources)
