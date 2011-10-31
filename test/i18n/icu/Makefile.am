## command sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

check_PROGRAMS = icufmt icuustring icucharsetdetector icuconv \
	icuutext icutranslit icutranslitru

icufmt_SOURCES = icufmt.cpp
icufmt_CXXFLAGS = $(ICU_CFLAGS)
icufmt_LDFLAGS = $(ICU_LIBS) $(ICU_IO_LIB)

icuustring_SOURCES = icuustring.cpp
icuustring_CXXFLAGS = $(ICU_CFLAGS)
icuustring_LDFLAGS = $(ICU_LIBS) $(ICU_IO_LIB)

icucharsetdetector_SOURCES = icucharsetdetector.cpp
icucharsetdetector_CXXFLAGS = $(ICU_CFLAGS)
icucharsetdetector_LDFLAGS = $(ICU_LIBS) $(ICU_IO_LIB)

icuconv_SOURCES = icuconv.cpp
icuconv_CXXFLAGS = $(ICU_CFLAGS)
icuconv_LDFLAGS = $(ICU_LIBS) $(ICU_IO_LIB)

icuutext_SOURCES = icuutext.cpp
icuutext_CXXFLAGS = $(ICU_CFLAGS)
icuutext_LDFLAGS = $(ICU_LIBS) $(ICU_IO_LIB)

icutranslit_SOURCES = icutranslit_unaccent.hpp icutranslit_unaccent.cpp \
	icutranslit_util.hpp icutranslit_util.cpp icutranslit.cpp
icutranslit_CXXFLAGS = $(ICU_CFLAGS)
icutranslit_LDFLAGS = $(ICU_LIBS) $(ICU_IO_LIB)

icutranslitru_SOURCES = icutranslitru.cpp \
	russian_latin_bgn.hpp ukrainian_latin_bgn.hpp
icutranslitru_CXXFLAGS = $(ICU_CFLAGS)
icutranslitru_LDFLAGS = $(ICU_LIBS) $(ICU_IO_LIB)

EXTRA_DIST =
