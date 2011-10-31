# Process this file with autoconf to produce a configure script
#-------------------------------------------------------------------
AC_PREREQ(2.59)
AC_COPYRIGHT([Copyright (C) 2007-2009 Denis Arnaud <denis_arnaud@users.sourceforge.net>])
AC_INIT([OPENTREP],[99.99.99],[denis_arnaud@users.sourceforge.net],[opentrep])
AC_CONFIG_HEADER([opentrep/config.h])
AC_CONFIG_SRCDIR([opentrep/basic/BasConst.cpp])
AC_CONFIG_AUX_DIR([config])
AM_INIT_AUTOMAKE
AM_PATH_CPPUNIT(1.10)
AM_CONDITIONAL([HAVE_CPPUNIT], [test "x$CPPUNIT_LIBS" != x])

# RPM release number
RPM_RELEASE="1"
AC_SUBST(RPM_RELEASE)

# Shared library versioning
GENERIC_LIBRARY_VERSION="99:99:99"
#                        | | |
#                 +------+ | +---+
#                 |        |     |
#              current:revision:age
#                 |        |     |
#                 |        |     +- increment if interfaces have been added;
#                 |        |        set to zero if interfaces have been 
#                 |        |        removed or changed
#                 |        +- increment if source code has changed;
#                 |           set to zero if current is incremented
#                 +- increment if interfaces have been added, removed
#                    or changed
AC_SUBST(GENERIC_LIBRARY_VERSION)

# Upload command for SourceForge
UPLOAD_COMMAND="ncftpput -v upload.sourceforge.net /incoming"
AC_SUBST(UPLOAD_COMMAND)

# Check for host and disable building a shared library in Windows
AC_CANONICAL_HOST
case $host in
  *-*-msdos* | *-*-go32* | *-*-mingw32* | *-*-cygwin* | *-*-windows*) 
    AC_DISABLE_SHARED
    AC_ENABLE_STATIC
    ;;
  *)  
    AC_ENABLE_SHARED
    AC_DISABLE_STATIC
    ;;
esac

# Checks for programs
save_CXXFLAGS="$CXXFLAGS"
AC_PROG_CXX
AC_PROG_CXXCPP
CXXFLAGS="$save_CXXFLAGS"
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_LN_S
AC_PROG_MAKE_SET

# Set default language to C++
AC_LANG([C++])

# Check for exceptions switch
AC_ARG_ENABLE(exceptions,
              [AS_HELP_STRING([--enable-exceptions],
                              [enable exceptions handling mechanism])],
              [AC_DEFINE([OPENTREP_EXCEPTIONS], [1],
                         [Define if you want exceptions handling])])
test "x$enable_exceptions" != xno && enable_exceptions=yes
test "x$ax_cv_boost_date_time" != xno && enable_exceptions=yes

# Check for info pages
AC_ARG_ENABLE(info-doc, 
              [AS_HELP_STRING([--disable-info-doc],
                              [do not generate Info documentation])])
test "x$enable_info_doc" != xno && enable_info_doc=yes


# Check for html-doc switch
AC_ARG_ENABLE(html-doc, 
              [AS_HELP_STRING([--disable-html-doc],
                              [do not generate HTML documentation])])
test "x$enable_html_doc" != xno && enable_html_doc=yes


# Provide --with-docdir
AC_ARG_WITH(docdir,
            [AS_HELP_STRING([--with-docdir=DIR],
                            [set documentation directory to DIR])],
            [DOC_DIR="$withval"],
            [DOC_DIR=""])
if test "x$DOC_DIR" != x; then
  docdir="$DOC_DIR"
else
  docdir="${datadir}/doc/$PACKAGE-$VERSION"
fi
AC_SUBST(docdir)


# Default compilation flags
if test -z "${CXXFLAGS}"; then
   CXXFLAGS="-g -Wall"
fi


# -----------------------------------------------------------
# Python
# -----------------------------------------------------------
PGAC_CHECK_PYTHON_EMBED_SETUP
AC_SUBST(PYTHON_VERSION)
AC_SUBST(PYTHON_LIBS)
AC_SUBST(PYTHON_CFLAGS)
AC_SUBST(PYTHON_ADD_LIBS)

# -----------------------------------------------------------
# Python: http://www.python.org
# -----------------------------------------------------------
PGAC_CHECK_PYTHON_EMBED_SETUP
AC_SUBST(PYTHON_VERSION)
AC_SUBST(PYTHON_LIBS)
AC_SUBST(PYTHON_CFLAGS)
AC_SUBST(PYTHON_ADD_LIBS)

# -----------------------------------------------------------
# MPICH2: http://www.mcs.anl.gov/research/projects/mpich2
# Note: Boost.MPI depends on MPICH2, rather than on OpenMPI
# -----------------------------------------------------------
#AX_MPICH2
#AC_SUBST(MPICH2_VERSION)
#AC_SUBST(MPICH2_CFLAGS)
#AC_SUBST(MPICH2_LIBS)

# -----------------------------------------------------------
# OpenMPI: http://www.open-mpi.org
# -----------------------------------------------------------
#AX_OPENMPI
#AC_SUBST(OPENMPI_VERSION)
#AC_SUBST(OPENMPI_CFLAGS)
#AC_SUBST(OPENMPI_LIBS)

# ---------------------------------------------------------------
# Boost (STL Extensions): http://www.boost.org
# ---------------------------------------------------------------
AX_BOOST([1.33])
AC_SUBST(BOOST_VERSION)
AC_SUBST(BOOST_CFLAGS)
AC_SUBST(BOOST_LIBS)
AC_SUBST(BOOST_DATE_TIME_LIB)
AC_SUBST(BOOST_PROGRAM_OPTIONS_LIB)
AC_SUBST(BOOST_FILESYSTEM_LIB)
AC_SUBST(BOOST_SYSTEM_LIB)
AC_SUBST(BOOST_THREAD_LIB)
AC_SUBST(BOOST_PYTHON_LIB)
#AC_SUBST(BOOST_ASIO_LIB)
#AC_SUBST(BOOST_MPI_LIB)
#AC_SUBST(BOOST_MPI_PYTHON_LIB)

# --------------------------------------------------------------------
# Support for MySQL (C client API): http://www.mysql.org
# --------------------------------------------------------------------
AX_MYSQL
AC_SUBST(MYSQL_VERSION)
AC_SUBST(MYSQL_CFLAGS)
AC_SUBST(MYSQL_LIBS)

# --------------------------------------------------------
# SOCI (Database API: http://soci.sourceforge.net)
# --------------------------------------------------------
AX_SOCI([3.0])
AC_SUBST(SOCI_VERSION)
AC_SUBST(SOCI_CFLAGS)
AC_SUBST(SOCI_LIBS)

# -----------------------------------------------------------
# Xapian (Indexing & Search API: http://www.xapian.org
# -----------------------------------------------------------
XO_LIB_XAPIAN
AC_SUBST(XAPIAN_VERSION)
AC_SUBST(XAPIAN_CFLAGS)
AC_SUBST(XAPIAN_LIBS)

# --------------------------------------------------------------------
# Support for ICU (i18n C API): http://www.icu-project.org
# --------------------------------------------------------------------
AX_ICU
AC_SUBST(ICU_VERSION)
AC_SUBST(ICU_CFLAGS)
AC_SUBST(ICU_LIBS)
AC_SUBST(ICU_IO_LIB)


# -------------------------------------------------------------------
# Support for documentation
# -------------------------------------------------------------------
# Checks for documentation build tools
#AC_CHECK_PROG([texinfo_ok], [texinfo], [yes], [no])
#if test "x$enable_info_doc" != xno; then
#  test "x$texinfo_ok" != xyes && enable_info_doc=no
#  AC_PATH_PROG([PERL], [perl], [])
#fi
AM_CONDITIONAL([INFO_DOC], [test "x$enable_info_doc" = xyes])

# Checks for documentation build tools
AC_CHECK_PROG([doxygen_ok], [doxygen], [yes], [no])
AC_CHECK_PROG([latex_ok], [latex], [yes], [no])
AC_CHECK_PROG([dvips_ok], [dvips], [yes], [no])
AC_CHECK_PROG([gs_ok], [gs], [yes], [no])
if test "x$enable_html_doc" != xno; then
  test "x$doxygen_ok" != xyes && enable_html_doc=no
  test "x$latex_ok" != xyes && enable_html_doc=no
  test "x$dvips_ok" != xyes && enable_html_doc=no
  test "x$gs_ok" != xyes && enable_html_doc=no
  AC_PATH_PROG([PERL], [perl], [])
fi
AM_CONDITIONAL([HTML_DOC], [test "x$enable_html_doc" = xyes])


# Check for diff program used for tests
AC_CHECK_PROG([diff_ok], [diff], [yes], [no])
AC_CHECK_PROG([sed_ok], [sed], [yes], [no])
enable_tests=yes
test "x$diff_ok" != xyes && enable_tests=no
test "x$sed_ok" != xyes && enable_tests=no
AM_CONDITIONAL([RUN_TESTS], [test "x$enable_tests" = xyes])


# Checks for header files
AC_HEADER_STDC
AC_HEADER_TIME
AC_CHECK_HEADERS([cmath \
  deque \
  fstream \
  iomanip \
  iostream \
  sstream \
  limits \
  list \
  set \
  vector \
  map \
  queue \
  stdexcept \
  string])


# --------------------------------------------------------
# I18n (internationalisation) with GNU Gettext
# --------------------------------------------------------
AM_GNU_GETTEXT([external], [need-formatstring-macros])
AM_GNU_GETTEXT_VERSION([0.14])
# Force the usage of the i18n feature: on 64bit architecture,
# as gettext uses conversions from int to char*, there may be
# some issues. However, for regular translation (e.g., to German 
# or French), the PO files are correctly transformed into GMO.
USE_NLS=yes


# Define configured files
AC_CONFIG_FILES(
	Makefile
	opentrep-config
	opentrep.pc
	opentrep.spec
	opentrep.m4
	ternary_tree/Makefile
	opentrep/Makefile
	opentrep/basic/Makefile
	opentrep/bom/Makefile
	opentrep/factory/Makefile
	opentrep/dbadaptor/Makefile
	opentrep/command/Makefile
	opentrep/service/Makefile
	opentrep/config/Makefile
	opentrep/core/Makefile
	opentrep/batches/Makefile
	opentrep/python/Makefile
	man/Makefile
	info/Makefile
	doc/Makefile
	doc/images/Makefile
	doc/tutorial/Makefile
	doc/tutorial/src/Makefile
	doc/local/Makefile
	doc/local/index.doc
	doc/doxygen_html.cfg
	doc/sourceforge/howto_release_opentrep.html
	po/Makefile.in
	db/Makefile
	db/admin/Makefile
	gui/Makefile
	gui/icons/Makefile
	gui/psp/Makefile
	extracppunit/Makefile
	test/parsers/Makefile
	test/i18n/Makefile
	test/i18n/icu/Makefile
	test/i18n/utf8/Makefile
	test/python/Makefile
	test/iterator/Makefile
	test/Makefile
	win32/Makefile)
AC_OUTPUT

echo "
------------------------------------------------------------------------------
$PACKAGE-$VERSION library configuration:
------------------------------------------------------------------------------

Directories:
  - prefix ............ : ${prefix}
  - exec_prefix ....... : ${exec_prefix}
  - includedir ........ : ${includedir}
  - libdir ............ : ${libdir}
  - docdir ............ : ${docdir}
  - mandir ............ : ${mandir}
  - infodir ........... : ${infodir}

Switches:
  - exceptions ........ : ${enable_exceptions}
  - use-nls ........... : ${USE_NLS}
  - info-doc .......... : ${enable_info_doc}
  - html-doc .......... : ${enable_html_doc}
  - shared ............ : ${enable_shared}
  - static ............ : ${enable_static}

Documentation tools:
  - texinfo ........... : ${texinfo_ok}
  - doxygen ........... : ${doxygen_ok}
  - latex ............. : ${latex_ok}
  - dvips ............. : ${dvips_ok}
  - ghostscript ....... : ${gs_ok}

Testing tools:
  - diff .............. : ${diff_ok}
  - sed ............... : ${sed_ok}

Compiler/linker flags/libs/defs:
  - CXX ............... : ${CXX}
  - CXXFLAGS .......... : ${CXXFLAGS}
  - CPPFLAGS .......... : ${CPPFLAGS}
  - LDFLAGS ........... : ${LDFLAGS}
  - LIBS .............. : ${LIBS}

External libraries:
  - Python ............. :
    o PYTHON_version ... : ${PYTHON_VERSION}
    o PYTHON_CFLAGS .... : ${PYTHON_CFLAGS}
    o PYTHON_LIBS ...... : ${PYTHON_LIBS} 
    o PYTHON_ADD_LIBS .. : ${PYTHON_ADD_LIBS} 

  - Boost ............. :
    o BOOST_VERSION ... : ${BOOST_VERSION}
    o BOOST_CFLAGS .... : ${BOOST_CFLAGS}
    o BOOST_LIBS ...... : ${BOOST_LIBS}
    o BOOST_DT_LIB .... : ${BOOST_DATE_TIME_LIB}
    o BOOST_PO_LIB .... : ${BOOST_PROGRAM_OPTIONS_LIB}
    o BOOST_FS_LIB .... : ${BOOST_FILESYSTEM_LIB}
    o BOOST_SYS_LIB ... : ${BOOST_SYSTEM_LIB}
    o BOOST_THRD_LIB .. : ${BOOST_THREAD_LIB}
    o BOOST_PY_LIB .... : ${BOOST_PYTHON_LIB}
    o BOOST_ASIO_LIB .. : ${BOOST_ASIO_LIB}
    o BOOST_MPI_LIB ... : ${BOOST_MPI_LIB}
    o BOOST_MPIPY_LIB . : ${BOOST_MPI_PYTHON_LIB}

  - MySQL ............. :
    o MYSQL_version ... : ${MYSQL_VERSION}
    o MYSQL_CFLAGS .... : ${MYSQL_CFLAGS}
    o MYSQL_LIBS ...... : ${MYSQL_LIBS} 

  - Soci .............. :
    o SOCI_VERSION .... : ${SOCI_VERSION}
    o SOCI_CFLAGS ..... : ${SOCI_CFLAGS}
    o SOCI_LIBS ....... : ${SOCI_LIBS}

  - Xapian ............ :
    o XAPIAN_VERSION .. : ${XAPIAN_VERSION}
    o XAPIAN_CFLAGS ... : ${XAPIAN_CFLAGS}
    o XAPIAN_LIBS ..... : ${XAPIAN_LIBS}

  - ICU ............... :
    o ICU_version ..... : ${ICU_VERSION}
    o ICU_CFLAGS ...... : ${ICU_CFLAGS}
    o ICU_LIBS ........ : ${ICU_LIBS} 
    o ICU_IO_LIB ...... : ${ICU_IO_LIB}

  - CPPUNIT ........... :
    o CPPUNIT_VERSION . : ${CPPUNIT_VERSION}
    o CPPUNIT_CFLAGS .. : ${CPPUNIT_CFLAGS}
    o CPPUNIT_LIBS .... : ${CPPUNIT_LIBS}

------------------------------------------------------------------------------
Now type 'make && make install' to build and install $PACKAGE-$VERSION library
------------------------------------------------------------------------------
"
