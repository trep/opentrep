## command sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

# Libraries
lib_LTLIBRARIES = libpyboost.la

libpyboost_la_SOURCES = boost_python.cpp
libpyboost_la_CXXFLAGS = ${PYTHON_CFLAGS}
#libpyboost_la_LIBADD =
libpyboost_la_LDFLAGS = ${PYTHON_LIBS} ${PYTHON_ADD_LIBS} \
	${BOOST_PYTHON_LIB} -version-info $(GENERIC_LIBRARY_VERSION)

EXTRA_DIST = boost_python.py
