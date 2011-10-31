## opentrep/bom sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

#
noinst_LTLIBRARIES= libopentrepbom.la

libopentrepbom_la_SOURCES = $(bom_h_sources) $(bom_cc_sources) 
libopentrepbom_la_CXXFLAGS = $(GSL_CFLAGS)
libopentrepbom_la_LDFLAGS = $(GSL_LIBS)

#
#pkgincludedir = $(includedir)/@PACKAGE@/bom
#pkginclude_HEADERS = $(bom_h_sources)
