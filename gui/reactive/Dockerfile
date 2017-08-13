
#FROM docker.io/centos:7
FROM mkubenka/centos-systemd
MAINTAINER Denis Arnaud <des.arnaud_fedora@m4x.org>

#
ENV HOME /root
ENV TREP_DIR /opt/trep
ENV DB_PASS dbpass

# House keeping
RUN yum -y install epel-release
RUN yum -y install dnf
RUN dnf clean all
RUN dnf -y update
RUN dnf -y install htop less rpmconf git-all bash-completion

# C++ development
RUN dnf -y install gcc-c++ cmake boost-devel xapian-core-devel soci-mysql-devel soci-sqlite3-devel \
    readline-devel sqlite-devel mariadb-devel libicu-devel protobuf-devel protobuf-compiler

# Python development
RUN dnf -y install python-devel python34-devel

# Documentation tools
RUN dnf -y install doxygen ghostscript "tex(latex)"

# Set up and start Maria (MySQL) database
RUN dnf -y install mariadb-server
RUN mysql_install_db
RUN mysqladmin' -u root password '$DB_PASS'


# User environment
ADD bashrc $HOME/
RUN cat $HOME/bashrc >> $HOME/.bashrc && rm -f $HOME/bashrc

# Set up of the environment for OpenTREP
RUN mkdir -p $TREP_DIR

WORKDIR $TREP_DIR

RUN git clone https://github.com/trep/opentrep.git

CMD ["/bin/bash"]

