include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

## Source directory

MAINTAINERCLEANFILES = Makefile.in


# Binaries (batches)
bin_PROGRAMS = opentrep_indexer opentrep_searcher

opentrep_indexer_SOURCES = $(batches_idx_h_sources) $(batches_idx_cc_sources)
opentrep_indexer_CXXFLAGS = $(BOOST_CFLAGS)
#opentrep_indexer_LDADD =
opentrep_indexer_LDFLAGS = $(BOOST_PROGRAM_OPTIONS_LIB) $(SOCI_LIBS) \
	$(top_builddir)/@PACKAGE@/core/lib@PACKAGE@.la


opentrep_searcher_SOURCES = $(batches_srh_h_sources) $(batches_srh_cc_sources)
opentrep_searcher_CXXFLAGS = $(BOOST_CFLAGS)
#opentrep_searcher_LDADD =
opentrep_searcher_LDFLAGS = $(BOOST_PROGRAM_OPTIONS_LIB) $(SOCI_LIBS) \
	$(top_builddir)/@PACKAGE@/core/lib@PACKAGE@.la
