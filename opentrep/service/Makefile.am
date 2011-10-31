## opentrep/command sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

#
noinst_LTLIBRARIES = libopentrepsvc.la

libopentrepsvc_la_SOURCES = $(svc_h_sources) $(svc_cc_sources) 
libopentrepsvc_la_CXXFLAGS = $(SOCI_CFLAGS) $(BOOST_CFLAGS)
libopentrepsvc_la_LDFLAGS =

#
#pkgincludedir = $(includedir)/@PACKAGE@/service
#pkginclude_HEADERS = $(svc_h_sources)
