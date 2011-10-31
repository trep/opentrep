
# Xapian
XAPIAN_DIR=/opt/xapian-1.0.10
#XAPIAN_DIR=/usr
XAPIAN_CFLAGS=-I${XAPIAN_DIR}/include
XAPIAN_LIBS=-L${XAPIAN_DIR}/lib -lxapian

# Soci
MYSQL_BACKEND=soci_mysql
SOCI_DIR=/opt/soci-3.0.0
#SOCI_DIR=/projects/sim/local/Linux/soci-3.0.0
SOCI_INC=${SOCI_DIR}/include
SOCI_CFLAGS=-I${SOCI_INC}
SOCI_LIB=${SOCI_DIR}/lib
SOCI_LIBS=-L${SOCI_LIB} -l${MYSQL_BACKEND} -lsoci_core

# MySQL
MYSQL_INC=/usr/include/mysql
MYSQL_CFLAGS=-I${MYSQL_INC}
MYSQL_LIB=/usr/lib64/mysql
MYSQL_LIBS=-L${MYSQL_LIB} -ldl -lmysqlclient

# Boost
BOOST_INC=/usr/include
BOOST_CFLAGS=-I${BOOST_INC}
BOOST_LIB=/usr/lib
BOOST_LIBS=-L${BOOST_LIB}

# Compilation and Building Flags
CXXFLAGS=${XAPIAN_CFLAGS} ${SOCI_CFLAGS} ${MYSQL_CFLAGS} ${BOOST_CFLAGS}
LDFLAGS=${XAPIAN_LIBS} ${SOCI_LIBS} ${MYSQL_LIBS} ${BOOST_LIBS}

# Tools
CC=g++ -g
RM=rm -f

# Project
PROG_IDX=builddb
EXEC_IDX=${PROG_IDX}
SRC_IDX=${PROG_IDX}.cpp

# Targets
all: ${EXEC_IDX}

${EXEC_IDX}: ${SRC_IDX}
	${CC} ${CXXFLAGS} ${LDFLAGS} -o $@ $<
	@echo "Run ./${EXEC_IDX}"

clean:
	@${RM} ${EXEC_IDX}
