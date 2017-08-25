
FROM docker.io/centos:7
#FROM mkubenka/centos-systemd
MAINTAINER Denis Arnaud <des.arnaud_fedora@m4x.org>

#
ENV container docker
ENV HOME /root
ENV TREP_BASE_DIR /opt/trep
ENV TREP_DIR $TREP_BASE_DIR/opentrep
ENV TREP_BUILD_DIR $TREP_DIR/build
ENV DB_PASS dbpass
ENV INSTALL_BASEDIR=$TREP_BASE_DIR/deliveries
ENV TREP_VER=99.99.99
ENV XAPIANDB $TREP_BASE_DIR/xapiandb

# House keeping
RUN yum -y install epel-release
RUN yum -y install dnf
RUN dnf clean all
RUN dnf -y update
RUN dnf -y install initscripts
RUN dnf -y install htop less file rpmconf git-all bash-completion

# C++ development
RUN dnf -y install gcc-c++ cmake boost-devel xapian-core-devel soci-mysql-devel soci-sqlite3-devel \
    readline-devel sqlite-devel mariadb-devel libicu-devel protobuf-devel protobuf-compiler

# Python development
RUN dnf -y install python-devel python34-devel

# Documentation tools
RUN dnf -y install doxygen ghostscript "tex(latex)"

#
VOLUME ["/sys/fs/cgroup", "/run", "/tmp"]

# Set up and start Maria (MySQL) database
RUN dnf -y install mariadb-server
RUN mysql_install_db
RUN chown -R mysql.mysql /var/lib/mysql
WORKDIR /usr
RUN /usr/bin/mysqld_safe --datadir='/var/lib/mysql' --nowatch
#RUN mysqladmin -u root password '$DB_PASS'

# User environment
ADD bashrc $HOME/
RUN cat $HOME/bashrc >> $HOME/.bashrc && rm -f $HOME/bashrc

# Set up of the environment for OpenTREP
RUN mkdir -p $TREP_BASE_DIR
WORKDIR $TREP_BASE_DIR
RUN git clone https://github.com/trep/opentrep.git
WORKDIR $TREP_DIR
RUN mkdir -p build
WORKDIR $TREP_BUILD_DIR

# Launch the build
RUN cmake -DCMAKE_INSTALL_PREFIX=${INSTALL_BASEDIR}/opentrep-$TREP_VER \
   -DCMAKE_BUILD_TYPE:STRING=Debug -DINSTALL_DOC:BOOL=OFF \
   -DRUN_GCOV:BOOL=OFF -DLIB_SUFFIX=64 ..
RUN make && make install

# Launch the indexation
RUN ./opentrep/opentrep-indexer --xapiandb $XAPIANDB

# Check that the search works correctly
RUN ./opentrep/opentrep-searcher --xapiandb $XAPIANDB -q nce sfo

#
RUN echo "root:root"|chpasswd
#CMD ["/usr/sbin/init"]
CMD ["/bin/bash"]


