## command sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

# Library
lib_LTLIBRARIES = libpy@PACKAGE@.la

libpy@PACKAGE@_la_SOURCES = pyopentrep.cpp
libpy@PACKAGE@_la_CXXFLAGS = ${PYTHON_CFLAGS} ${BOOST_CFLAGS}
libpy@PACKAGE@_la_LIBADD = $(top_builddir)/@PACKAGE@/core/lib@PACKAGE@.la
libpy@PACKAGE@_la_LDFLAGS = ${PYTHON_LIBS} ${PYTHON_ADD_LIBS} \
	${BOOST_PYTHON_LIB} -version-info $(GENERIC_LIBRARY_VERSION)

EXTRA_DIST = pyopentrep.py
