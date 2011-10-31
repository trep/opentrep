
# Xapian
#XAPIAN_DIR=/opt/xapian-1.0.11
XAPIAN_DIR=/usr
XAPIAN_CFLAGS=-I${XAPIAN_DIR}/include
XAPIAN_LIBS=-L${XAPIAN_DIR}/lib -lxapian

# Compilation and Building Flags
CXXFLAGS=${XAPIAN_CFLAGS}
LDFLAGS=${XAPIAN_LIBS}

# Tools
CC=g++ -g
RM=rm -f

# Project
PROG_IDX=simple_index
EXEC_IDX=${PROG_IDX}
SRC_IDX=${PROG_IDX}.cpp
PROG_SRH=simple_search
EXEC_SRH=${PROG_SRH}
SRC_SRH=${PROG_SRH}.cpp
PROG_STR_SRH=string_search
EXEC_STR_SRH=${PROG_STR_SRH}
SRC_STR_SRH=${PROG_STR_SRH}.cpp
PROG_STR_SRH=string_simple_search
EXEC_STR_SRH=${PROG_STR_SRH}
SRC_STR_SRH=${PROG_STR_SRH}.cpp

# Targets
all: ${EXEC_IDX} ${EXEC_SRH} ${EXEC_STR_SRH}

${EXEC_IDX}: ${SRC_IDX}
	${CC} ${CXXFLAGS} ${LDFLAGS} -o $@ $<
	@echo "Run ./${EXEC_IDX} <path to database> <document data> <document terms>"

${EXEC_SRH}: ${SRC_SRH}
	${CC} ${CXXFLAGS} ${LDFLAGS} -o $@ $<
	@echo "Run ./${EXEC_SRH} <path to database> <search terms>"

${EXEC_STR_SRH}: ${SRC_STR_SRH}
	${CC} ${CXXFLAGS} ${LDFLAGS} -o $@ $<
	@echo "Run ./${EXEC_STR_SRH} <path to database> <search terms>"

clean:
	@${RM} ${EXEC_IDX} ${EXEC_SRH}
