##
# OpenTREP data directory
#
# For now, that data directories consist of the OPTD-maintained list of
# points of reference (POR). The main file comes from the OpenTravelData
# project (http://github.com/opentraveldata/opentraveldata/).
# The original file is directly accessible on GitHub:
# http://github.com/opentraveldata/opentraveldata/tree/master/opentraveldata/optd_por_public.csv
#

# Define the substitutes for the variables present in the script files
# using the OpenTrep-provided data files.
# Note that PACKAGE, PACKAGE_NAME and PACKAGE_VERSION
# are defined in the main CMakeLists.txt (of the top root directory).
set (OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set (top_srcdir       ${CMAKE_SOURCE_DIR}        )
set (srcdir           ${CMAKE_CURRENT_SOURCE_DIR})

set (por_data_DIR         por)
set (por_csv_data_DIR     por/csv)
set (por_pg_data_DIR      por/pg)
set (por_sqlite_data_DIR  por/sqlite)
set (por_mysql_data_DIR   por/mysql)

set (OPENTREP_DATA_DIR ${OPENTREP_SAMPLE_DIR}/${por_data_DIR})
set (OPENTREP_DATA_CSV_DIR ${OPENTREP_SAMPLE_DIR}/${por_csv_data_DIR})
set (OPENTREP_DATA_PG_DIR ${OPENTREP_SAMPLE_DIR}/${por_pg_data_DIR})
set (OPENTREP_DATA_SQLITE_DIR ${OPENTREP_SAMPLE_DIR}/${por_sqlite_data_DIR})
set (OPENTREP_DATA_MYSQL_DIR ${OPENTREP_SAMPLE_DIR}/${por_mysql_data_DIR})

##
# Installation of the documentation files (written in Markup Doc format)
install (DIRECTORY ${por_data_DIR} DESTINATION ${OPENTREP_SAMPLE_DIR}
  FILES_MATCHING PATTERN "*.md")

##
# Installation of the schemas (DDL) and data files (DML).
# CSV samples
install (DIRECTORY ${por_csv_data_DIR}
  DESTINATION ${OPENTREP_DATA_DIR}
  FILES_MATCHING PATTERN "*.*")

# PostgreSQL admin scripts
install (DIRECTORY ${por_pg_data_DIR}
  DESTINATION ${OPENTREP_DATA_DIR}
  FILES_MATCHING PATTERN "*.*")

# MySQL admin scripts
install (DIRECTORY ${por_mysql_data_DIR}
  DESTINATION ${OPENTREP_DATA_DIR}
  FILES_MATCHING PATTERN "*.*")

# SQLite admin scripts
install (DIRECTORY ${por_sqlite_data_DIR}
  DESTINATION ${OPENTREP_DATA_DIR}
  FILES_MATCHING PATTERN "*.*")

##
# Installing database scripts
module_script_add (${por_data_DIR}/pg/opentrep-create-db-user.sh)
