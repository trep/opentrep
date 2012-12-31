##
# OpenTREP data directory
#
# For now, that date files consist of the ORI-maintained list of points of
# reference (POR). That file is a clone of the OpenTravelData project
# (https://github.com/opentraveldata/optd/).
# The original file is accessible directly on GitHub:
# http://github.com/opentraveldata/optd/blob/trunk/refdata/ORI/ori_por_public.csv
#

# Define the substitutes for the variables present in the script files
# using the OpenTrep-provided data files.
# Note that PACKAGE, PACKAGE_NAME and PACKAGE_VERSION
# are defined in the main CMakeLists.txt (of the top root directory).
set (OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set (top_srcdir       ${CMAKE_SOURCE_DIR}        )
set (srcdir           ${CMAKE_CURRENT_SOURCE_DIR})

set (por_data_DIR     por)
set (OPENTREP_DATA_ROOT_DIR ${CMAKE_INSTALL_PREFIX}/share/${PACKAGE}/data)
set (OPENTREP_DATA_DIR ${OPENTREP_DATA_ROOT_DIR}/${por_data_DIR})


##
# Installation of the data files.
install (DIRECTORY ${por_data_DIR} DESTINATION ${OPENTREP_DATA_ROOT_DIR}
  FILES_MATCHING PATTERN "*.csv")
