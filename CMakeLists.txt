#
cmake_minimum_required (VERSION 2.6)

##
# CMake macros for the project
include (config/project_config_embeddable.cmake)

##
# Project info
set_project_names (opentrep OpenTREP)
set_project_brief ("C++ Open Travel Request Parsing Library")
set_project_versions (0 5 3)

##
# Project options
# The parameters are:
#  * Whether or not to build and check the unit tests
#  * whether or not the documentation must be built and installed
set_project_options (on on off)


#####################################
##            Packaging            ##
#####################################
#
packaging_init (${PROJECT_NAME})
packaging_set_summary ("${PROJECT_NAME} That project aims at providing 
a clean API, and the corresponding C++ implementation, for parsing
travel-focused requests (e.g.,
"washington dc beijing monday a/r +aa -ua 1 week 2 adults 1 dog").

The C++ library uses Xapian (http://www.xapian.org) for the
Information Retrieval part, on freely available travel-related data
(e.g., country names and codes, city names and codes, airline names
and codes, etc.).

The C++ library exposes a simple, clean and object-oriented, API. For
instance, the static Parse() method takes, as input, a string
containing the travel request, and yields, as output, the list of the
recognised terms as well as their corresponding types. As an example,
the travel request "washington dc beijing monday a/r +aa -ua 1 week 2
adults 1 dog" would give the following list:
  * Origin airport: Washington, DC, USA
  * Destination airport: Beijing, China
  * Date of travel: next Monday
  * Date of return: 1 week after next Monday
  * Preferred airline: American Airlines; 
      non-preferred airline: United Airlines
  * Number of travellers: 2 adults and a dog

The output can then be used by other systems, for instance to book the
corresponding travel or to visualise it on a map and calendar and to
share it with others.

${PROJECT_NAME} makes an extensive use of existing open-source libraries for
increased functionality, speed and accuracy. In particular the
Boost (C++ Standard Extensions: http://www.boost.org) library is used")
packaging_set_contact ("Denis Arnaud <denis_arnaud - at - users dot sourceforge dot net>")
packaging_set_vendor ("Denis Arnaud")

#
packaging_set_other_options (TBZ2 "TBZ2;TGZ")


########################################
##            Dependencies            ##
########################################
#
get_external_libs (git "python 2.6" "boost 1.48" "icu 4.2" "xapian 1.0" doxygen)


##############################################
##           Build, Install, Export         ##
##############################################
##
# Initialise the building process
init_build ()

##
# Sub-modules
add_modules (${PROJECT_NAME})

##
# For other projects to use AirRAC, install a few helpers for standard
# build/packaging systems: CMake, GNU Autotools (M4), pkgconfig/pc, 
# airrac-config
install_dev_helper_files ()

##
# Basic documentation (i.e., AUTHORS, NEWS, README, INSTALL)
install_basic_documentation ()


##########################################
##            Documentation             ##
##########################################
#
handle_html_doc ()

##
# Subdirs
add_subdirectory (data)
add_subdirectory (man)


#####################################
##              Tests              ##
#####################################
##
# Initialise the unit test process
init_test ()

##
# Test suites
add_test_suite (${PROJECT_NAME})
add_test_suite (i18n)


#######################################
##          Overall Status           ##
#######################################
display_status ()


#######################################
##         CMake Cache Storage       ##
#######################################
store_in_cache ()
