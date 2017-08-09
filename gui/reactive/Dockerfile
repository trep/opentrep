
FROM docker.io/centos:7
MAINTAINER Denis Arnaud <des.arnaud_fedora@m4x.org>

# House keeping
RUN yum -y install epel-release
RUN yum -y install dnf
RUN dnf clean all
RUN dnf -y update

CMD ["/bin/bash"]

