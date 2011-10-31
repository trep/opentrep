include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

##
# Source directory

DISTCLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

SUBDIRS =

EXTRA_DIST =


##
# Library
lib_LTLIBRARIES = libtst.la

libtst_la_SOURCES = $(tst_h_sources) $(tst_cc_sources)
#libtst_la_LIBADD =
libtst_la_LDFLAGS = -version-info $(GENERIC_LIBRARY_VERSION)

# Header files
nobase_pkginclude_HEADERS = $(ttree_h_sources)
#nobase_nodist_pkginclude_HEADERS = $(top_builddir)/@PACKAGE@/config.h


##
# Binaries (batches)
bin_PROGRAMS = simple_tst

simple_tst_SOURCES = simple_tst.cpp
#simple_tst_CXXFLAGS =
#simple_tst_LDADD =
#simple_tst_LDFLAGS = 

##
# Test binaries
#check_PROGRAMS = iterator_compile_test tst_concept_checks

#iterator_compile_test_SOURCES = iterator_compile_test.cpp
#iterator_compile_test_LDFLAGS = 

#tst_concept_checks_SOURCES = tst_concept_checks.cpp
#tst_concept_checks_LDFLAGS = 
