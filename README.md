Open Travel Request Parser (TREP)
=================================

[![Build Status](https://travis-ci.com/trep/opentrep.svg?branch=master)](https://travis-ci.com/trep/opentrep)
[![Docker Repository on Quay](https://quay.io/repository/trep/opentrep/status "Docker Repository on Quay")](https://quay.io/repository/trep/opentrep)

# Table of content
- [Open Travel Request Parser (TREP)](#open-travel-request-parser--trep-)
- [Table of content](#table-of-content)
- [Overview](#overview)
- [Docker images](#docker-images)
  * [OpenTREP Docker images](#opentrep-docker-images)
  * [General purpose C++/Python Docker images](#general-purpose-c---python-docker-images)
    + [CentOS 7](#centos-7)
    + [Ubuntu 18.04 LTS Bionic Beaver](#ubuntu-1804-lts-bionic-beaver)
    + [Debian 9 Stretch](#debian-9-stretch)
    + [Common to all the above-mentioned Linux distributions](#common-to-all-the-above-mentioned-linux-distributions)
- [Native installation (without Docker)](#native-installation--without-docker-)
  * [RPM-based distributions (eg, Fedora/CentOS/RedHat)](#rpm-based-distributions--eg--fedora-centos-redhat-)
  * [Installation from the sources](#installation-from-the-sources)
    + [Clone the Git repository](#clone-the-git-repository)
    + [Alternatively, download and extract the tar-ball](#alternatively--download-and-extract-the-tar-ball)
    + [Installation of the dependencies](#installation-of-the-dependencies)
      - [On Linux](#on-linux)
        * [Fedora](#fedora)
        * [CentOS](#centos)
        * [Debian/Ubuntu](#debian-ubuntu)
      - [On MacOS](#on-macos)
    + [ICU](#icu)
    + [Boost](#boost)
      - [CentOS](#centos-1)
    + [SOCI](#soci)
      - [General Unix/Linux](#general-unix-linux)
      - [Debian](#debian)
      - [MacOS](#macos)
    + [Building the library and test binary](#building-the-library-and-test-binary)
  * [Underlying (relational) database, SQLite or MySQL/MariaDB, if any](#underlying--relational--database--sqlite-or-mysql-mariadb--if-any)
- [Indexing the POR data](#indexing-the-por-data)
  * [Filling the (relational) database, SQLite or MySQL/MariaDB, if any](#filling-the--relational--database--sqlite-or-mysql-mariadb--if-any)
  * [Xapian indexing with standard installation](#xapian-indexing-with-standard-installation)
  * [Xapian indexing for an ad hoc deployed Web application](#xapian-indexing-for-an-ad-hoc-deployed-web-application)
- [Searching](#searching)
- [Deployment stages](#deployment-stages)
- [Index, or not, non-IATA POR](#index--or-not--non-iata-por)
- [Installing a Python virtual environment](#installing-a-python-virtual-environment)
- [Checking that the Python module works](#checking-that-the-python-module-works)
- [Trouble-shooting Python issues on MacOS](#trouble-shooting-python-issues-on-macos)
  * [Interceptors not installed / late](#interceptors-not-installed---late)
- [OpenTREP as a Python extension](#opentrep-as-a-python-extension)
  * [References](#references)
  * [Build and package OpenTREP as a Python extension](#build-and-package-opentrep-as-a-python-extension)
    + [Install Python modules/dependencies](#install-python-modules-dependencies)
    + [Install OpenTrep Python extension with system-based `pip`](#install-opentrep-python-extension-with-system-based--pip-)
    + [Build the OpenTrep Python extension locally with system-based Scikit-build](#build-the-opentrep-python-extension-locally-with-system-based-scikit-build)
  * [Test the OpenTREP Python extension](#test-the-opentrep-python-extension)
  * [Use the OpenTREP Python extension](#use-the-opentrep-python-extension)
    + [Download the latest OpenTravelData (OPTD) POR data file](#download-the-latest-opentraveldata--optd--por-data-file)
    + [Xapian index initialization](#xapian-index-initialization)
    + [Search with the OpenTrep Python extension](#search-with-the-opentrep-python-extension)
    + [Search with the OpenTrepWrapper package](#search-with-the-opentrepwrapper-package)
- [(Optional) Running the Django-based application server (needs update)](#-optional--running-the-django-based-application-server--needs-update-)

<small><i><a href='http://ecotrust-canada.github.io/markdown-toc/'>Table of contents generated with markdown-toc</a></i></small>

# Overview
[OpenTREP](https://github.com/trep/opentrep)
aims at providing a clean API, and the corresponding C++
implementation, for parsing travel-/transport-focused requests.
It powers the https://transport-search.org Web site (as well
as its newer version, https://www2.transport-search.org).

OpenTREP uses Xapian (https://www.xapian.org) for the Information Retrieval
part, on freely available transport-/travel-related data (_e.g._, country
names and codes, city names and codes, airline names and codes, _etc_),
mainly to be found in the
[OpenTravelData (OPTD)
project](https://github.com/opentraveldata/opentraveldata):
http://github.com/opentraveldata/opentraveldata/tree/master/opentraveldata

OpenTREP exposes a simple, clean and object-oriented, API. For instance,
the
[`OPENTREP::interpretTravelRequest()`
method](https://github.com/trep/opentrep/blob/master/opentrep/OPENTREP_Service.hpp#L58):
* takes, as input, a character string containing the travel request;
* and yields, as output, the list of the recognized terms,
  as well as their corresponding types.

As an example, the travel request
`Washington DC Beijing Monday a/r +AA -UA 1 week 2 adults 1 dog` would yield
the following list:
 * Origin airport: Washington, DC, United States (US)
 * Destination airport: Beijing, China (CN)
 * Date of travel: next Monday
 * Date of return: 1 week after next Monday
 * Preferred airline: American Airlines (AA);
   non-preferred airline: United Airlines
 * Number of travelers: 2 adults and a dog

The output may then be used by other systems, for instance to book the
corresponding travel, or to visualize it on a map and calendar and to
share it with others.

Note that the current version of OpenTREP recognizes only geographical POR
(points of reference), whatever their number in the request. For instance,
the request `lviv rivne jitomir kbp kharkiv dnk ods lwo` yields the
following list of POR: `LWO`, `RWN`, `ZTR`, `KBP`, `HRK`, `DNK`,
`ODS` and `LWO` again.
See
[that request in action on the transport-search.org site](https://transport-search.org/search/?q=lwo+rwn+ztr+kbp+hrk+dnk+ods+lwo)
or [through the API](https://www2.transport-search.org/api/v1/geo?query=lwo+rwn+ztr+kbp+hrk+dnk+ods+lwo)
(enable [JSONView](https://github.com/gildas-lormeau/JSONView-for-Chrome)
or similar for a more confortable reading).

OpenTREP also deals with transport-related requests. For instance,
`cnshg deham nlrtm uslbg brssz cnshg` correspond to a world tour
of famous ports:
* Shanghai, China (CN)
* Hamburg, Germany (DE)
* Rotterdam, Netherlands (NL)
* Los Angeles/Long Beach, California (CA), United States (US)
* Santos, Brazil (BR)
* (Back to) Shanghai, China (CN)
See
[that request in action on the transport-search.org site](https://transport-search.org/search/?q=sha+ham+rtm+lgb+ssz+sha)
or [through the API](https://www2.transport-search.org/api/v1/geo?query=sha+ham+rtm+lgb+ssz+sha).

The underlying data for the POR is the
[OpenTravelData `optd_por_public_all.csv` file](http://github.com/opentraveldata/opentraveldata/tree/master/opentraveldata/optd_por_public_all.csv).
A good complementary tool is
[GeoBase](http://opentraveldata.github.io/geobases/),
a Python-based software able to access any travel-related data source.

OpenTREP makes an extensive use of existing open-source libraries for
increased functionality, speed and accuracy. In particular the
[Boost](http://www.boost.org) (C++ Standard Extensions),
[Xapian](http://xapian.org) and [SOCI](http://soci.sourceforge.net)
libraries are used.

# Docker images

## OpenTREP Docker images
Docker images provide ready-to-use environments, and are available on
[Docker Cloud](https://cloud.docker.com/u/opentrep/repository/docker/opentrep/search-travel)
and [Quay.io](https://quay.io/repository/trep/opentrep):
```bash
$ docker pull opentrep/search-travel:legacy # for Docker.io
$ docker pull quay.io/trep/opentrep # for Quay.io
$ docker run --rm -it opentrep/search-travel:legacy bash
```

See https://github.com/trep/opentrep/tree/master/gui/legacy for more details.

## General purpose C++/Python Docker images
General purpose Docker images for C++/Python development are also available from
[Docker Cloud](https://cloud.docker.com/u/cpppythondevelopment/repository/docker/cpppythondevelopment/base/general).
Those Docker images allow to develop on the major Linux distributions, _i.e._,
CentOS, Debian and Ubuntu.

### CentOS 8
```bash
$ docker pull cpppythondevelopment/base:centos8
$ docker run -t cpppythondevelopment/base:centos8 bash
[build@2..c ~]$ $ mkdir -p ~/dev/geo && cd ~/dev/geo
[build@2..c geo]$ git clone https://github.com/trep/opentrep.git
[build@2..c geo]$ cd opentrep && mkdir build && cd build
[build@2..c build (master)]$ cmake3 -DCMAKE_INSTALL_PREFIX=${HOME}/dev/deliveries/opentrep-99.99.99 -DCMAKE_BUILD_TYPE:STRING=Debug -DINSTALL_DOC:BOOL=ON -DRUN_GCOV:BOOL=OFF -DLIB_SUFFIX= ..
```

### CentOS 7
```bash
$ docker pull cpppythondevelopment/base:centos7
$ docker run -t cpppythondevelopment/base:centos7 bash
[build@2..c ~]$ $ mkdir -p ~/dev/geo && cd ~/dev/geo
[build@2..c geo]$ git clone https://github.com/trep/opentrep.git
[build@2..c geo]$ cd opentrep && mkdir build && cd build
[build@2..c build (master)]$ cmake3 -DCMAKE_INSTALL_PREFIX=${HOME}/dev/deliveries/opentrep-99.99.99 -DCMAKE_BUILD_TYPE:STRING=Debug -DINSTALL_DOC:BOOL=ON -DRUN_GCOV:BOOL=OFF -DLIB_SUFFIX= ..
```

### Ubuntu 18.04 LTS Focal Fossal
```bash
$ docker pull cpppythondevelopment/base:ubuntu2004
$ docker run -t cpppythondevelopment/base:ubuntu2004 bash
[build@2..c ~]$ $ mkdir -p ~/dev/geo && cd ~/dev/geo
[build@2..c geo]$ git clone https://github.com/trep/opentrep.git
[build@2..c geo]$ cd opentrep && mkdir build && cd build
[build@2..c build (master)]$ cmake -DCMAKE_INSTALL_PREFIX=${HOME}/dev/deliveries/opentrep-99.99.99 -DCMAKE_BUILD_TYPE:STRING=Debug -DINSTALL_DOC:BOOL=ON -DRUN_GCOV:BOOL=OFF -DLIB_SUFFIX= ..
```

### Ubuntu 18.04 LTS Bionic Beaver
```bash
$ docker pull cpppythondevelopment/base:ubuntu1804
$ docker run -t cpppythondevelopment/base:ubuntu1804 bash
[build@2..c ~]$ $ mkdir -p ~/dev/geo && cd ~/dev/geo
[build@2..c geo]$ git clone https://github.com/trep/opentrep.git
[build@2..c geo]$ cd opentrep && mkdir build && cd build
[build@2..c build (master)]$ cmake -DCMAKE_INSTALL_PREFIX=${HOME}/dev/deliveries/opentrep-99.99.99 -DCMAKE_BUILD_TYPE:STRING=Debug -DINSTALL_DOC:BOOL=ON -DRUN_GCOV:BOOL=OFF -DLIB_SUFFIX= ..
```

### Debian 10 Buster
```bash
$ docker pull cpppythondevelopment/base:debian10
$ docker run -t cpppythondevelopment/base:debian10 bash
[build@2..c ~]$ $ mkdir -p ~/dev/geo && cd ~/dev/geo
[build@2..c geo]$ git clone https://github.com/trep/opentrep.git
[build@2..c geo]$ cd opentrep && mkdir build && cd build
[build@2..c build (master)]$ cmake -DCMAKE_INSTALL_PREFIX=${HOME}/dev/deliveries/opentrep-99.99.99 -DCMAKE_BUILD_TYPE:STRING=Debug -DINSTALL_DOC:BOOL=ON -DRUN_GCOV:BOOL=OFF -DLIB_SUFFIX= ..
```

### Debian 9 Stretch
```bash
$ docker pull cpppythondevelopment/base:debian9
$ docker run -t cpppythondevelopment/base:debian9 bash
[build@2..c ~]$ $ mkdir -p ~/dev/geo && cd ~/dev/geo
[build@2..c geo]$ git clone https://github.com/trep/opentrep.git
[build@2..c geo]$ cd opentrep && mkdir build && cd build
[build@2..c build (master)]$ cmake -DCMAKE_INSTALL_PREFIX=${HOME}/dev/deliveries/opentrep-99.99.99 -DCMAKE_BUILD_TYPE:STRING=Debug -DINSTALL_DOC:BOOL=ON -DRUN_GCOV:BOOL=OFF -DLIB_SUFFIX= ..
```

### Common to all the above-mentioned Linux distributions
```bash
[build@2..c build (master)]$ make install
[build@2..c build (master)]$ ./opentrep/opentrep-indexer
[build@2..c build (master)]$ ./opentrep/opentrep-searcher -q "nice san francisco"
```

# Native installation (without Docker)

## RPM-based distributions (eg, Fedora/CentOS/RedHat)
Since OpenTREP has been approved as an official package of Fedora/CentOS/RedHat
(see the
[review request on Bugzilla for further details](https://bugzilla.redhat.com/show_bug.cgi?id=866265)),
just use [DNF](https://fedoraproject.org/wiki/DNF) (or
[Yum](https://fedoraproject.org/wiki/Yum) for the older distributions):
```bash
$ dnf -y install opentrep opentrep-doc
```

## Installation from the sources

### Configure the environment
* If not already specified, the following environment variables may be set:
```bash
export platform="$(uname | tr '[:upper:]' '[:lower:]')"
export architecture="$(uname -m|sed 's/x86_/amd/')"
```

### Clone the Git repository
The GitHub repository may be cloned as following:
```bash
$ mkdir -p ~/dev/geo && cd ~/dev/geo
$ git clone https://github.com/trep/opentrep.git
$ cd opentrep
$ git checkout master
```

### Alternatively, download and extract the tar-ball
GitHub generates
[tar-balls on the fly for every tagged release](https://github.com/trep/opentrep/releases).
For instance:
```bash
$ wget https://github.com/trep/opentrep/archive/opentrep-0.07.10.tar.gz
```

Note that SourceForge also stores some
[older archived tar-balls](https://sourceforge.net/projects/opentrep/files/opentrep/).

### Installation of the dependencies

#### On Linux
The following packages may be needed (Fedora/RedHat/CentOS names on the left
hand side, Debian/Ubuntu names on the right hand side; names for other
Linux distributions may vary):
* `cmake` (or `cmake3` on CentOS)
* `gcc-c++` / `gcc`, `g++`
* `boost-devel` (or `boost169-devel` on CentOS) / `libboost-all-dev`
* `xapian-core-devel` / `libxapian-dev`
* `python-devel` / `python`, `libpython-dev`
* `libicu-devel` / `libicu-dev`
* `soci-mysql-devel`, `soci-sqlite3-devel` / SOCI needs to be installed from
  the sources on Debian, Ubuntu and MacOS (see the dedicated section below)
* `sqlite3-devel` / `sqlite3`, `libsqlite3-dev`
* `mariadb-devel` / `libmariadb-dev`, `libmysql++-dev`
* `protobuf-devel`, `protobuf-compiler` / `libprotobuf-dev`, `protobuf-compiler`
* `readline-devel` / `libreadline-dev`, `libncurses5-dev`
* `ghostscript`
* `doxygen`
* (optional) `tetex-latex` / `texlive-latex-recommended`
* (optional) `rpm-build`

For instance, the following subsections show respective installation commands
for a few famous Linux distributions.

##### Fedora
* General C++-base stack:
```bash
$ dnf -y install git-all bash-completion gcc-c++ cmake boost-devel \
 xapian-core-devel soci-mysql-devel soci-sqlite3-devel readline-devel \
 sqlite-devel mariadb-devel libicu-devel protobuf-devel protobuf-compiler \
 autoconf automake libtool
```

* Python development (the package names may vary; Python 2 is no longer
  supported):
```bash
$ dnf -y install python-devel python-pip
```

* Related Python packages (the package names may vary):
```bash
$ dnf -y install python3-protobuf boost-python3-devel
```

* Documentation tools:
```bash
$ dnf -y install doxygen ghostscript "tex(latex)"
```

##### CentOS
* If DNF is not already installed (_e.g._, on CentOS 7):
```bash
$ yum -y install dnf
```

* General C++-based stack:
```bash
$ dnf -y install git-all bash-completion gcc-c++ cmake boost169-devel \
 xapian-core-devel soci-mysql-devel soci-sqlite3-devel readline-devel \
 sqlite-devel mariadb-devel libicu-devel protobuf-devel protobuf-compiler \
 autoconf automake libtool
```

* Related Python packages (the package names may vary):
```bash
$ dnf -y install python3-protobuf boost169-python3-devel
```

* Python development (the package names may vary):
```bash
$ dnf -y install python39 python39-devel
$ dnf -y install python3 python3-devel
```

* Documentation tools:
```bash
$ dnf -y install doxygen ghostscript "tex(latex)"
```

##### Debian/Ubuntu
* Various C++ and Python packages:
```bash
$ apt-get -y install locales && locale-gen "en_US.UTF-8"
$ apt-get -y install zlib1g-dev libbz2-dev lsb-release libgmp-dev \
   libgmp-dev gcc g++ clang cppcheck cmake libboost-all-dev libxapian-dev \
   graphviz libreadline-dev libncurses5-dev libczmq-dev libzmq3-dev \
   libssl-dev libffi-dev sqlite3 libsqlite3-dev libmariadb-dev \
   libmysql++-dev postgresql-server-dev-all libicu-dev \
   libprotobuf-dev protobuf-compiler
```

* Python development (the package names may vary)
```bash
$ apt-get -y install python3 libpython3-dev python3-pip
```

* Documentation tools
```bash
$ apt-get -y install doxygen ghostscript texlive-latex-recommended
```

#### On MacOS
* With [Homebrew](http://brew.sh):
```bash
$ brew install boost boost-python3 cmake libedit \
  xapian sqlite mysql mysql-client icu4c protobuf protobuf-c doxygen
$ brew install homebrew/portable-ruby/portable-readline
```

* Note that, as of January 2021, the Hombrew recipes for Python 3 are now
  specific up to the minor version, more specifically:
  + Python 3.8: `python@3.8` (Python 3.8.11 as of July 2021)
  + Python 3.9: `python@3.9` (Python 3.9.6 as of July 2021)

* Previously, the default Python 3 installation was Python 3.8 (now part of
  the `python@3.8` Homebrew package).
  As a reminder, on MacOS with Homebrew, a way to get the details is:
```bash
$ brew info python@3.8
python@3.8: stable 3.8.11 (bottled) [keg-only]
$ brew info python@3.9
python@3.9: stable 3.9.6 (bottled)
```

* Because of the multiple parallel installations of Python versions,
  the following setup may be necessary:
```bash
$ sudo mkdir -p /usr/local/Cellar/python
$ sudo chown -R $USER /usr/local/Cellar/python
$ ln -s /usr/local/Cellar/python\@3.8/3.8.11 /usr/local/Cellar/python/3.8.11
$ ln -s /usr/local/Cellar/python\@3.9/3.9.6 /usr/local/Cellar/python/3.9.6
```
  + Clean links on potential older versions:
```bash
$ ls -lFh /usr/local/Cellar/python/
total 0
lrwxr-xr-x 1 user staff 34B Jan 9 21:14 3.8.11@ -> /usr/local/Cellar/python@3.8/3.8.11
lrwxr-xr-x 1 user staff 34B Jan 9 21:14 3.9.6@ -> /usr/local/Cellar/python@3.9/3.9.6
$ sudo unlink /usr/local/Cellar/python/3.8.7
```
  + Check the following links, as installed by Homebrew on MacOS:
```bash
$ ls -lFh /usr/local/Frameworks/Python.framework/Versions/3.9
lrwxr-xr-x  1 darnaud  staff    75B Jan  9 04:01 /usr/local/Frameworks/Python.framework/Versions/3.9@ -> ../../../Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9
$ ls -lFh /usr/local/Frameworks/Python.framework/Versions/3.8
lrwxr-xr-x  1 root  staff    75B Jan  9 21:20 /usr/local/Frameworks/Python.framework/Versions/3.8@ -> /usr/local/Cellar/python@3.8/3.8.11/Frameworks/Python.framework/Versions/3.8
$ ls -lFh /usr/local/Frameworks/Python.framework/Versions/Current
lrwxr-xr-x  1 user  staff    79B Jan  9 04:01 /usr/local/Frameworks/Python.framework/Versions/Current@ -> ../../../Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/Current
```
  + If those links are not as expected (as of January 2021, they were correct),
    recreate them:
```bash
$ sudo chown -R $USER /usr/local/Frameworks/Python.framework
$ unlink /usr/local/Frameworks/Python.framework/Versions/3.9
$ ln -s /usr/local/Cellar/python\@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9 /usr/local/Frameworks/Python.framework/Versions/3.9
$ unlink /usr/local/Frameworks/Python.framework/Versions/3.8
$ ln -s /usr/local/Cellar/python\@3.8/3.8.11/Frameworks/Python.framework/Versions/3.8 /usr/local/Frameworks/Python.framework/Versions/3.8
$ unlink /usr/local/Frameworks/Python.framework/Versions/Current
$ ln -s /usr/local/Cellar/python\@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9 /usr/local/Frameworks/Python.framework/Versions/Current
```
  + Leading to:
```bash
$ ls -lFh /usr/local/Cellar/python/
total 0
lrwxr-xr-x  1 user  admin    34B Jan  9 21:14 3.8.11@ -> /usr/local/Cellar/python@3.8/3.8.11
lrwxr-xr-x  1 user  admin    34B Jan  9 21:14 3.9.6@ -> /usr/local/Cellar/python@3.9/3.9.6
$ ls -lFh /usr/local/Frameworks/Python.framework/Versions/
total 0
lrwxr-xr-x  1 root     staff    75B Jan  9 21:20 3.8@ -> /usr/local/Cellar/python@3.8/3.8.11/Frameworks/Python.framework/Versions/3.8
lrwxr-xr-x  1 user  staff    75B Jan  9 04:01 3.9@ -> ../../../Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9
lrwxr-xr-x  1 user  staff    79B Jan  9 04:01 Current@ -> ../../../Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/Current
```

* Up until recently (mid-2020), Boost.Python came with a dependency on
  the `libpython` Python library. As of July 2020, it seems to no longer
  be the case. As a matter of fact, as stated in the
  [What's new documentation of Python 3.8](https://docs.python.org/3/whatsnew/3.8.html#debug-build-uses-the-same-abi-as-release-build):
> On Unix, C extensions are no longer linked to libpython except on Android
> and Cygwin. It is now possible for a statically linked Python to load
> a C extension built using a shared library Python.
> (Contributed by Victor Stinner in [bpo-21536](https://bugs.python.org/issue21536).).

* The rationale is that most of the C extensions (including OpenTREP
  Python extension and Boost.Python) are meant to be loaded by the Python
  interpreter, which brings its own Python library (`libpython`).
  So, if the C extensions are linked against `libpython` themselves,
  it is highly probable that the versions of both those `libpython`
  libraries (the one integrated with the Python interpreter and
  the one linked with the C extension) would differ.
  
* The adoption of that change (no longer linking a C extension with
  `libpython`) is progressing slowly, and highly dependent on the
  C extensions and the platforms. For instance:
  + On Fedora Linux distributions, that change was implemented only
    from Boost 1.73 on Fedora 33 (to be released at the end of 2020).
  + On MacOS, that change seems to have been implemented as a patch to
    Boost 1.72 package; as it can be seen below, Boost 1.72+
	on MacOS no longer seems to be linked against `libpython`.
```bash
$ brew info boost-python3
boost-python3: stable 1.76.0 (bottled), HEAD
/usr/local/Cellar/boost-python3/1.76.0 (472 files, 18.2MB) *
$ ls -lFh /usr/local/Cellar/boost-python3/1.76.0/lib/libboost_python39.dylib
-r--r--r--  1 user  staff   410K May 26 21:13 /usr/local/Cellar/boost-python3/1.76.0/lib/libboost_python39.dylib
$ otool -L /usr/local/Cellar/boost-python3/1.76.0/lib/libboost_python39.dylib
/usr/local/Cellar/boost-python3/1.76.0/lib/libboost_python39.dylib:
	/usr/local/opt/boost-python3/lib/libboost_python39.dylib (compatibility version 0.0.0, current version 0.0.0)
	/usr/lib/libc++.1.dylib (compatibility version 1.0.0, current version 905.6.0)
	/usr/lib/libSystem.B.dylib (compatibility version 1.0.0, current version 1292.100.5)
```
  
* The OpenTREP Python extension itself has adopted that change with
  version 0.07.7 release at the end of May 2020.

* On some platforms, it may still happen that Boost.Python
  and the local Python are based on different versions. That may lead
  to some weird crashes not obvious to debug.

### ICU
* Install ICU with Homebrew
```bash
$ brew install icu4c
```

### CMake

* On some Linux distributions, _e.g._ on CentOS 8, the available version
  of CMake may lag well behind the
  [latest releases](https://github.com/Kitware/CMake/releases).
  It then may be needed to build and install CMake from the sources.

#### CentOS 8

* CMake 3.18 will be released on CentOS 8 only from CentOS 8.4 onwards,
  that is, most probably only from mid-2021. In the meantime, CMake may
  be installed, for instance following the details on
  [Linux-4-One - Install CMake on CentOS 8](https://linux4one.com/how-to-install-cmake-on-centos-8)

* Remove any installed CMake package:
```bash
$ sudo dnf -y remove cmake-data cmake cmake-filesystem
```

* Specify the CMake version to install (to be picked up on the
  [GitHub page for CMake releases](https://github.com/Kitware/CMake/releases)):
```bash
$ CMAKE_VERSION="3.19.3"
```

* Download the CMake source tar-ball and un-package it:
```bash
$ sudo mkdir -p /opt/cmake/archives && \
  sudo wget https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}.tar.gz \
  -O /opt/cmake/archives/cmake-${CMAKE_VERSION}.tar.gz && \
  pushd /opt/cmake && \
  sudo tar zxf /opt/cmake/archives/cmake-${CMAKE_VERSION}.tar.gz &&\
  pushd /opt/cmake/cmake-${CMAKE_VERSION} && \
  popd && popd && \
  sudo chown -R $USER /opt/cmake/cmake-${CMAKE_VERSION}
```

* Build and install CMake:
```bash
$ pushd /opt/cmake/cmake-${CMAKE_VERSION} && \
  ./bootstrap && \
  make && \
  sudo make install && \
  popd
```

* CMake is then installed in `/usr/local` (binaries in `/usr/local/bin`,
  manual pages in `/usr/local/man` and CMake files in
  `/usr/local/share/cmake-3.19/Modules`

* Check the just installed CMake distribution:
```bash
$ cmake --version
cmake version ${CMAKE_VERSION}
```

### Boost
Follow the instructions on
[Boost helper documentation on GitHub](https://github.com/cpp-projects-showcase/boost-helper)
to install Python and Boost on some platforms, including MacOS.

#### CentOS
* On CentOS, the version of Boost is often outdated, for instance
  [Boost 1.53 on CentOS 7](https://git.centos.org/rpms/boost/releases).
  It may have adversarial effects, for instance on CentOS 7, where
  [Python 3 libraries have been upgraded from Python 3.4 to
  Python 3.6](https://lists.fedoraproject.org/archives/list/epel-announce@lists.fedoraproject.org/thread/EGUMKAIMPK2UD5VSHXM53BH2MBDGDWMO/),
  but where Boost 1.53 Python libraries have not been upgraded accordingly,
  leading for the least to
  [linking errors](https://bugzilla.redhat.com/show_bug.cgi?id=1702242).

* Hopefully, [EPEL](https://fedoraproject.org/wiki/EPEL) provides
  more recent Boost releases, for instance
  [Boost 1.69 on RHEL/CentOS 7](https://src.fedoraproject.org/rpms/boost169/tree/epel7).

* Those additional specific Boost packages may be installed in parallel
  to the officiial Boost packages.
  CMake has however to be made aware of that additional Boost package and,
  equally importantly, of its location on the file-system.

* Example of additional CMake configuration variables for
  [Boost 1.69 on CentOS 7](https://src.fedoraproject.org/rpms/boost169/tree/epel7):
```bash
$ cmake3 [...] \
  -DBOOST_LIBRARYDIR=/usr/lib64/boost169 \
  -DBOOST_INCLUDEDIR=/usr/include/boost169 \
  -DBoost_ADDITIONAL_VERSIONS="1.69 1.69.0" \
  [...]
```
 
### SOCI
* On CentOS and Fedora, since the author of this package (OpenTREP)
  is also the official maintainer of the SOCI package, that latter
  is usually up-to-date

* On Debian, Ubuntu and MacOS however, as of mid-2020, SOCI 4.0
  has still not been released, and `soci-mysql` is no longer available.
  Hence, SOCI must be built from the sources. The following shows
  how to do that on MacOS (on Debian/Ubuntu, one can have a look at
  the part installing SOCI on the
  [C++/Python Docker files](https://github.com/cpp-projects-showcase/docker-images))

* Download and prepare SOCI:
```bash
$ sudo mkdir -p /opt/soci && sudo chown -R ${USER} /opt/soci
$ git clone https://github.com/SOCI/soci.git /opt/soci/socigit
```

#### General Unix/Linux
```bash
$ mkdir -p /opt/soci/socigit/build/head
$ pushd /opt/soci/socigit/build/head
$ cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -DSOCI_CXX11=ON -DSOCI_TESTS=OFF ../..
$ make
$ sudo make install
$ popd
```

#### Debian
```bash
$ wget https://github.com/trep/opentrep/raw/master/ci-scripts/soci-debian-cmake.patch -O /opt/soci/soci-debian-cmake.patch
$ pushd /opt/soci/socigit
$ patch -p1 < ../soci-debian-cmake.patch
$ popd
$ mkdir -p /opt/soci/socigit/build/head
$ pushd /opt/soci/socigit/build/head
$ cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -DSOCI_CXX11=ON -DSOCI_TESTS=OFF ../..
$ make
$ sudo make install
$ popd
```

#### MacOS
* Configure SOCI:
```bash
$ mkdir -p /opt/soci/socigit/build/head && cd /opt/soci/socigit/build/head
$ cmake -DCMAKE_INSTALL_PREFIX=/usr/local \
  -DCMAKE_BUILD_TYPE=Debug -DSOCI_CXX_C11=ON \
  -DSOCI_ASAN=ON -DCMAKE_VERBOSE_MAKEFILE=OFF \
  -DSOCI_TESTS=OFF -DSOCI_STATIC=OFF -DSOCI_DB2=OFF -DSOCI_EMPTY=ON \
  -DSOCI_FIREBIRD=OFF -DSOCI_MYSQL=ON -DSOCI_ODBC=OFF -DSOCI_ORACLE=OFF \
  -DSOCI_POSTGRESQL=ON -DSOCI_SQLITE3=ON ../..
$ make
$ sudo make install
```

### Building the library and test binary
To customize OpenTREP to your environment, you can alter
the installation directory:
```bash
export INSTALL_BASEDIR="${HOME}/dev/deliveries"
export TREP_VER="0.07.10"
if [ -d /usr/lib64 ]; then LIBSUFFIX="64"; else LIBSUFFIX=""; fi
export LIBSUFFIX_4_CMAKE="-DLIB_SUFFIX=$LIBSUFFIX"
```

Then, as usual:
* To configure the project, type something like:
```bash
$ rm -rf build && mkdir build && cd build
$ cmake -DCMAKE_INSTALL_PREFIX=${INSTALL_BASEDIR}/opentrep-${TREP_VER} \
   -DCMAKE_BUILD_TYPE:STRING=Debug -DENABLE_TEST:BOOL=ON \
   -DINSTALL_DOC:BOOL=ON -DRUN_GCOV:BOOL=OFF ${LIBSUFFIX_4_CMAKE} ..
```

* With a specific version of Boost, say
  [Boost 1.69](https://src.fedoraproject.org/rpms/boost169)
  installed in a parallel way (for instance, as an optional module):
```bash
$ cmake -DCMAKE_INSTALL_PREFIX=${INSTALL_BASEDIR}/opentrep-${TREP_VER} \
        -DCMAKE_BUILD_TYPE:STRING=Debug -DENABLE_TEST:BOOL=ON \
        -DBOOST_LIBRARYDIR=/usr/lib64/boost169 \
		-DBOOST_INCLUDEDIR=/usr/include/boost169 \
        -DBoost_ADDITIONAL_VERSIONS="1.69 1.69.0" \
        -DINSTALL_DOC:BOOL=ON -DRUN_GCOV:BOOL=OFF ${LIBSUFFIX_4_CMAKE} ..
```

* For an _ad hoc_ installation:
```bash
INSTALL_DIR=/var/www/webapps/opentrep/trep

cmake -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
   -DCMAKE_BUILD_TYPE:STRING=Debug -DINSTALL_DOC:BOOL=OFF \
   -DRUN_GCOV:BOOL=OFF ${LIBSUFFIX_4_CMAKE} ..
```

* On MacOS, a few software (_e.g._, ICU and Readline) are not in
the standard place. So, the `cmake` command becomes:
```bash
$ export CMAKE_CXX_FLAGS="-Wno-mismatched-new-delete"; \
  cmake -DCMAKE_INSTALL_PREFIX=${INSTALL_BASEDIR}/opentrep-$TREP_VER \
   -DREADLINE_ROOT=/usr/local/opt/portable-readline \
   -DREADLINE_INCLUDE_DIR=/usr/local/opt/portable-readline/include \
   -DREADLINE_LIBRARY=/usr/local/opt/libedit/lib/libedit.dylib \
   -DICU_ROOT=/usr/local/opt/icu4c \
   -DCMAKE_BUILD_TYPE:STRING=Debug -DINSTALL_DOC:BOOL=ON \
   -DRUN_GCOV:BOOL=OFF ${LIBSUFFIX_4_CMAKE} ..
```

* To build the project, type:
```bash
$ make
```
* To test the project, type:
```bash
$ make check
```
* To install the library (`libopentrep*.so*`) and the binary (`opentrep`),
  just type:
```bash
  make install
  cd ${INSTALL_BASEDIR}
  rm -f opentrep-stable && ln -s opentrep-${TREP_VER} opentrep-stable
  cd -
```
* To package the source files, type:
```bash
$ make dist
```
* To package the binary and the documentation:
```bash
$ make package
```

* Install the latest
  [OpenTravelData (OPTD) POR data file](https://github.com/opentraveldata/opentraveldata/tree/master/opentraveldata/optd_por_public_all.csv).
  Note that OpenTREP no longer ships with (full) OPTD data files;
  only [test files](https://github.com/trep/opentrep/raw/master/data/por/test_optd_por_public.csv)
  are shipped.
  The OPTD POR data file (`optd_por_public_all.csv`) has therefore
  to be downloaded aside, usually renamed as `optd_por_public.csv`,
  and the OpenTREP binaries have then to be referred to the file-path
  of that POR data file.
```bash
$ wget \
  https://github.com/opentraveldata/opentraveldata/raw/master/opentraveldata/optd_por_public_all.csv \
  -O ${INSTALL_BASEDIR}/share/opentrep/data/por/optd_por_public.csv
```

* To run the local binary version:
```bash
$ ./opentrep/opentrep-{dbmgr,indexer,searcher}
```
* To run the installed version (the first following line must be done once
  per session):
```bash
$ export TREP_LIB="${INSTALL_BASEDIR}/opentrep-${TREP_VER}/lib${LIBSUFFIX}"
$ export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${TREP_LIB}"
$ ${INSTALL_BASEDIR}/opentrep-${TREP_VER}/bin/opentrep-{dbmgr,indexer,searcher}
```

## Underlying (relational) database, SQLite or MySQL/MariaDB, if any
OpenTREP may use, if so configured, a relational database. For now,
two database products are supported, SQLite3 and MySQL/MariaDB.
The database accelerates the look up of POR by (IATA, ICAO, FAA) codes
and of Geonames ID. When OpenTREP is configured to run without database,
those codes and Geonames ID are full-text searched directly with Xapian.
Note that the database can be managed directly, _i.e._, without the
OpenTREP search interface on top of it, thanks to the `opentrep-dbmgr`
utility, which is detailed below.

# Use cases

## Indexing the POR data

### Filling the (relational) database, SQLite or MySQL/MariaDB, if any
Here, for clarity reason, we use the local version. It is easy (see above)
to derive the same commands with the installed version.

* The following command prompts a Shell:
```bash
$ ./opentrep/opentrep-dbmgr -t sqlite -p ${INSTALL_BASEDIR}/share/opentrep/data/por/optd_por_public.csv
```

* Then, within the `opentrep>` shell, a typical sequence for SQLite would be:
```bash
 info
 create_user
 fill_from_por_file
 list_nb
 list_by_iata nce
 list_by_icao lfmn
 list_by_faa afm
 list_by_geonameid 6299418
```

* The following command prompts a shell:
```bash
./opentrep/opentrep-dbmgr -t mysql -p ${INSTALL_BASEDIR}/share/opentrep/data/por/optd_por_public.csv
```

* Then, within the `opentrep>` shell, a typical sequence for MySQL/MariaDB
  would be:
```bash
 reset_connection_string db=mysql user=root password=<passwd>
 create_user
 reset_connection_string db=trep_trep user=trep password=trep
 fill_from_por_file
 list_nb
 list_by_iata nce
 list_by_icao lfmn
 list_by_faa afm
 list_by_geonameid 6299418
```

### Xapian indexing with standard installation
By default, the Xapian indexer runs without filling any relational database,
as that step can be performed independantly by `opentrep-dbmgr`,
as seen above.

* Xapian indexing without any relational database:
```bash
$ ./opentrep/opentrep-indexer -p ${INSTALL_BASEDIR}/share/opentrep/data/por/optd_por_public.csv
```

* Xapian indexing and filling and indexing the SQLite database:
```bash
$ ./opentrep/opentrep-indexer -t sqlite -p ${INSTALL_BASEDIR}/share/opentrep/data/por/optd_por_public.csv
```

* Xapian indexing and filling and indexing the MySQL/MariaDB database:
```bash
$ ./opentrep/opentrep-indexer -t mysql -p ${INSTALL_BASEDIR}/share/opentrep/data/por/optd_por_public.csv
```

* There is an option to not even touch Xapian at all, for instance to check
  that the
  [OpenTravelData (OPTD) POR data file](http://github.com/opentraveldata/opentraveldata/tree/master/opentraveldata/optd_por_public_all.csv)
  is well-formed:
```bash
$ ./opentrep/opentrep-indexer -x 0 -p ${INSTALL_BASEDIR}/share/opentrep/data/por/optd_por_public.csv
```

### Xapian indexing for an ad hoc deployed Web application
* Xapian indexing without any relational database:
```bash
$ ./opentrep/opentrep-indexer -d /var/www/webapps/opentrep/trep/traveldb -p ${INSTALL_BASEDIR}/share/opentrep/data/por/optd_por_public.csv
```

* (Optional) Filling and indexing the SQLite database:
```bash
$ ./opentrep/opentrep-dbmgr -t sqlite -s /var/www/webapps/opentrep/trep/sqlite_travel.db -p ${INSTALL_BASEDIR}/share/opentrep/data/por/optd_por_public.csv
    create_user
    fill_from_por_file
    quit
```

## Searching
* Searching without any relational database support:
```bash
$ ./opentrep/opentrep-searcher -q "nce sfo"
```

* Searching with SQLite (note that it should be quicker than without a database):
```bash
$ ./opentrep/opentrep-searcher -t sqlite -q "nce sfo"
```

* Searching with MySQL/MariaDB:
```bash
$ ./opentrep/opentrep-searcher -t mysql -q "nce sfo"
```

* Searching with SQLite (with Xapian and SQLite DB in a `webapps` directory):
```bash
$ ./opentrep/opentrep-searcher -d /var/www/webapps/opentrep/trep/traveldb -t sqlite -s /var/www/webapps/opentrep/trep/sqlite_travel.db -q "nce sfo"
```

## Deployment stages
The idea is to have at least two pieces of infrastructure (SQL database,
Xapian index) in parallel:
* one is used by the production;
* the other one used as a staging platform in order to test
  and validate a new version.

Once the new version has been validated, the two pieces of infrastructure
can then be interverted, ie, the production becomes the new version,
and the older version ends up in staging.

It means that all programs have to choose which version they want to work on.
That version may even be toggled in live.

That method to deploy in production through a staging process
is even more needed by the fact that indexing a new POR data file
takes up to 30 minutes in the worst case. So, we cannot afford 30-45 minutes
of downtime everytime a new POR data file is released (potentially every day).

With that staging process, it is even possible to fully automate
the re-indexing after a new POR data file release: once the new release
has been cleared by
[QA (Quality Assurance)](http://en.wikipedia.org/wiki/Quality_assurance)
on staging, it becomes production.

The corresponding command-line option for the various programs
(`opentrep-dbmgr`, `opentrep-indexer`, `opentrep-searcher`) is `-m`.

* For instance, to build the Xapian index/database for deployment number 0:
```bash
$ ./opentrep/opentrep-indexer -m 0 -d /var/www/webapps/opentrep/trep/traveldb
$ chown -R apache.apache /var/www/webapps/opentrep/trep
$ ls -laFh /var/www/webapps/opentrep/trep/
-rw-r--r-- 1 apache apache  16M Oct 14 2018 sqlite_travel.db0
drwxr-xr-x 2 apache apache 4.0K Oct 14 2018 traveldb0/
```

* And to build the Xapian index/database for deployment number 1:
```bash
$ ./opentrep/opentrep-indexer -m 1 -d /var/www/webapps/opentrep/trep/traveldb 
$ chown -R apache.apache /var/www/webapps/opentrep/trep
$ ls -laFh /var/www/webapps/opentrep/trep/
-rw-r--r-- 1 apache apache  16M Oct 14 2018 sqlite_travel.db1
drwxr-xr-x 2 apache apache 4.0K Oct 14 2018 traveldb1/
```

## Index, or not, non-IATA POR
There is also a command-line option, namely `-n`, to state whether or not
the non-IATA-referenced POR should be included/parsed and indexed.

By default, and historically, only the POR, which are referenced by
[IATA](http://en.wikipedia.org/wiki/IATA_airport_code)
(ie, which have a specific IATA code), are indexed (and may be searched for)
in OpenTREP.

POR are also referenced by other international organizations, such as
[ICAO](http://en.wikipedia.org/wiki/ICAO_airport_code)
or [UN/LOCODE](http://en.wikipedia.org/wiki/UN/LOCODE),
and may not be referenced by IATA (in which case their IATA code
is left empty).

As of October 2018, there are around 110,000 POR in
[OpenTravelData (OPTD)](http://github.com/opentraveldata/opentraveldata),
the reference data source for OpenTREP:
*  Around 20,000 POR are referenced by IATA
*  Around 90,000 POR are not referenced by IATA, but referenced
   by other international organizations (eg, ICAO, UN/LOCODE)

* Indexing 20,000 POR takes already a few minutes on standard hardware.
* Indexing 110,000 POR would take 15 to 20 minutes.

Once indexed, all those POR become searchable. That flag is therefore
only used at indexing time (_i.e._, by the `opentrep-dbmgr` and
`opentrep-indexer` programs).

## Installing a Python virtual environment
All the details are explained on a
[dedicated procedure](http://github.com/machine-learning-helpers/induction-python/tree/master/installation/virtual-env),
which works for the major Linux distributions and on MacOS.

The procedure first installs a specific version of Python (as of November 2021,
3.9.8) thanks to PyEnv, then install `pipenv` thanks to the `pip` utility
provided with that specific Python version.

## Checking that the Python module works
* With the standard installation:
```bash
PYTHON_VERSION=$(python --version 2>&1 | cut -d' ' -f2,2 | cut -d'.' -f1,2)
PYTHONPATH=${INSTALL_BASEDIR}/opentrep-${TREP_VER}/lib${LIBSUFFIX}:${INSTALL_BASEDIR}/opentrep-${TREP_VER}/lib${LIBSUFFIX}/python${PYTHON_VERSION}/site-packages/pyopentrep \
 python3 -c "import pyopentrep; \
 openTrepLibrary = pyopentrep.OpenTrepSearcher(); \
 initOK = openTrepLibrary.init (\"${INSTALL_BASEDIR}/opentrep-${TREP_VER}/share/opentrep/data/por/test_optd_por_public.csv\", '/tmp/opentrep/xapian_traveldb', 'sqlite', '/tmp/opentrep/sqlite_travel.db', 0, False, True, True, 'pyopentrep.log'); \
 print (openTrepLibrary.search ('S', 'los las'))"
```

* With an _ad hoc_ installation:
```bash
PYTHON_VERSION=$(python --version 2>&1 | cut -d' ' -f2,2 | cut -d'.' -f1,2)
PYTHONPATH=${INSTALL_DIR}/lib${LIBSUFFIX}:${INSTALL_BASEDIR}/lib${LIBSUFFIX}/python${PYTHON_VERSION}/site-packages/pyopentrep \
 python3 -c "import pyopentrep; \
 openTrepLibrary = pyopentrep.OpenTrepSearcher(); \
 initOK = openTrepLibrary.init (\"${INSTALL_BASEDIR}/opentrep-${TREP_VER}/share/opentrep/data/por/test_optd_por_public.csv\", '/var/www/webapps/opentrep/trep/traveldb', 'mysql', 'db=trep_trep user=trep password=trep', 0, False, True, True, 'pyopentrep.log'); \
 print (openTrepLibrary.search ('S', 'los las'))"
```

# Trouble-shooting Python issues on MacOS

## Interceptors not installed / late
* On some versions of MacOS (_e.g._, with 12.0 aka Monterey or 11.1 aka Big Sur),
  there may be some strange issue related to interceptors:
```bash
$ ./opentrep/python/pyopentrep -d /tmp/opentrep/xapian_traveldb "nce sfo"
==217==ERROR: Interceptors are not working. This may be because AddressSanitizer is loaded too late (e.g. via dlopen). Please launch the executable with:
DYLD_INSERT_LIBRARIES=/Library/Developer/CommandLineTools/usr/lib/clang/12.0.0/lib/darwin/libclang_rt.asan_osx_dynamic.dylib
"interceptors not installed" && 0Abort trap: 6
```

* A work around is to explicitly use the MacOS native Python interpreter.
  The whole command-line then becomes:
```bash
$ PYTHON_VERSION=$(python3 --version 2>&1 | cut -d' ' -f2,2 | cut -d'.' -f1,2)
$ PYTHONPATH=${INSTALL_BASEDIR}/opentrep-${TREP_VER}/lib${LIBSUFFIX}:${INSTALL_BASEDIR}/opentrep-${TREP_VER}/lib${LIBSUFFIX}/python${PYTHON_VERSION}/site-packages/pyopentrep \
  DYLD_INSERT_LIBRARIES=/Library/Developer/CommandLineTools/usr/lib/clang/12.0.0/lib/darwin/libclang_rt.asan_osx_dynamic.dylib \
  ASAN_OPTIONS=detect_container_overflow=0 \
  /usr/local/Cellar/python/3.9.1/Frameworks/Python.framework/Versions/3.9/Resources/Python.app/Contents/MacOS/Python \
  ./opentrep/python/pyopentrep.py -d /tmp/opentrep/xapian_traveldb "nce sfo"
OPTD-maintained list of POR (points of reference): '~/dev/deliveries/opentrep-${TREP_VER}/share/opentrep/data/por/test_optd_por_public.csv'
Xapian-based travel database/index: '/tmp/opentrep/xapian_traveldb0'
SQLite database: '/tmp/opentrep/sqlite_travel.db'
searchString: nce sfo
Compact format => recognised place (city/airport) codes:
NCE SFO
------------------
```

# OpenTREP as a Python extension

## References
* Scikit-build (packaging C/C++ Python extensions):
  + https://scikit-build.readthedocs.io/en/latest/
  + https://scikit-build.readthedocs.io/en/latest/usage.html#basic-usage-example
* PyJQ (not using Scikit-build): https://github.com/doloopwhile/pyjq

## Build and package OpenTREP as a Python extension
* OpenTREP depends on Boost libraries, including Boost Python C extensions,
  and on Protobuf Python extensions too. Those Boost and Protobuf C extensions
  generally come with the system (_e.g._, installed by Homebrew on MacOS,
  DNF on Fedora/CentOS/RedHat, APT on Debian/Ubuntu).

* Hence, installing a local virtual environment will not work,
  especially when the Python version of the virtual environment does not
  match exactly the Python version of the system-installed libraries.

* The simplest approach so far is to use the Python installed by the system.
  Still, this can be done with `pip`, with that latter potentially installed
  on the user account only with the `--user` option when upgrading `pip`
  itself (with `python -m pip install --user -U pip`).

* Note that Linux binary wheels cannot be pushed as is onto PyPi.
  [Manylinux](https://github.com/pypa/manylinux) should be used for that.
  Scikit-build maintains
  [some additions on top of Manylinux](https://github.com/scikit-build/manylinux)
  and the
  [corresponding Manylinux Docker images](https://hub.docker.com/r/scikitbuild/manylinux2010_x86_64).
  The way to run those have still to be documented below.
  In the meantime, the OpenTREP wheel has to be built from the sources, either
  (see below for the details):
  + By using the `pip` module of the system-based Python:
  + By cloning [this OpenTREP Git repository](https://github.com/trep/opentrep)

### Install Python modules/dependencies
* Install Python 3 modules if not already done so:
```bash
$ pyenv global system
$ python -m pip install --user -U pip
$ python -m pip install -U setuptools cmake build wheel ninja scikit-build
$ python -m pip install -U pytest tox twine sphinx keyrings.alt flake8 black
$ python -m pip install -U simplejson protobuf
$ python -m pip install -U pyjq pyyaml
```

### Install OpenTrep Python extension with system-based `pip`
* OpenTREP extension/module is installed here as a system-based module
  with system-based Python and `pip`

* Build from source and install with `pip`:
```bash
$ python -mpip install -U opentrep
Defaulting to user installation because normal site-packages is not writeable
Collecting opentrep
  Using cached opentrep-0.7.10.tar.gz (1.7 MB)
  Installing build dependencies ... done
  Getting requirements to build wheel ... done
    Preparing wheel metadata ... done
Building wheels for collected packages: opentrep
  Building wheel for opentrep (PEP 517) ... done
  Created wheel for opentrep: filename=opentrep-0.7.10-cp39-cp39-linux_x86_64.whl size=3060796 sha256=6362e3a86af016b251fe33b9f76db17322ec15a60575082f31f6b719ba2cf97f
  Stored in directory: ~/.cache/pip/wheels/82/b3/7c/f026b883cc204eefab1588f5e68661f78fec25395277bd221d
Successfully built opentrep
Installing collected packages: opentrep
Successfully installed opentrep-0.7.10
```

* Set the `LD_LIBRARY_PATH`/`DYLD_LIBRARY_PATH` and `PYTHONPATH`
   environment variables
  + On Linux:
```bash
$ INST_DIR=${HOME}/.local
TREPBINDIR=${INST_DIR}/bin
OPTDPOR=${INST_DIR}/share/opentrep/data/por/test_optd_por_public.csv
export LD_LIBRARY_PATH=${INST_DIR}/lib
export PYTHONPATH=${INST_DIR}/lib:${INST_DIR}/lib/python3.9/site-packages/pyopentrep
```
  + On MacOS:
```bash
$ export INST_DIR=/usr/local
export PYTHONPATH=${INST_DIR}/lib:${INST_DIR}/lib/python3.9/site-packages/pyopentrep
export DYLD_LIBRARY_PATH=${INST_DIR}/lib
```

* See how to use the newly installed extension
  in the dedicated sub-section below

### Build the OpenTrep Python extension locally with system-based Scikit-build
* Set `Pyenv` on the system-based Python, which should be Python 3:
```bash
$ pyenv global system
$ python --version
Python 3.9.8
```

* Clean potential former builds and launch a new build with Scikit-build:
```bash
$ rm -rf _skbuild/ dist/ .tox/ __pycache__/ .pytest_cache/ MANIFEST *.egg-info/
$ python setup.py --build-type=Debug build sdist bdist_wheel # the build takes a few minutes
$ ls -lFh _skbuild/*-x86_64-3.9/ dist/
dist/:
total 24408
-rw-r--r--  1 user staff 9.8M Jan 10 19:10 opentrep-0.7.10a0-cp39-cp39-macosx_12_0_x86_64.whl
-rw-r--r--  1 user staff 1.6M Jan 10 19:10 opentrep-0.7.10a0.tar.gz

_skbuild/*-x86_64-3.9/:
-rw-r--r--   1 user  staff     0B Jan 10 19:10 _skbuild_MANIFEST
drwxr-xr-x  24 user  staff   768B Jan 10 19:10 cmake-build/
drwxr-xr-x   6 user  staff   192B Jan 10 19:10 cmake-install/
drwxr-xr-x   3 user  staff    96B Jan 10 19:10 setuptools/
```

* Set the `LD_LIBRARY_PATH` and `PYTHONPATH` environment variables:
```bash
$ INST_DIR=${PWD}/_skbuild/macosx-12.0-x86_64-3.9/cmake-install
$ TREPBINDIR=${INST_DIR}/bin
$ OPTDPOR=${INST_DIR}/share/opentrep/data/por/test_optd_por_public.csv
$ export LD_LIBRARY_PATH=${INST_DIR}/lib; export PYTHONPATH=${INST_DIR}/lib:${INST_DIR}/lib/python3.9/site-packages/pyopentrep
```

* Manylinux (as of mid-2020, limited to Python2, so, not working):
```bash
$ docker pull scikitbuild/manylinux2010_x86_64:09d11d5f8
$ docker run --rm -e PLAT=manylinux2010_x86_64 -v `pwd`:/io scikitbuild/manylinux2010_x86_64:09d11d5f8 linux64 /io/travis/build-wheels.sh
```

* Upload to test PyPi (remember, no Linux binary wheel):
```bash
user@laptop$ PYPIURL="https://test.pypi.org"
user@laptop$ twine upload -u __token__ --repository-url ${PYPIURL}/legacy/ dist/*
Uploading distributions to https://test.pypi.org/legacy/
Uploading opentrep-0.7.10-cp39-cp39-macosx_12_0_x86_64.whl
100%|█████████████████████████████████████████████████████████████████████| 9.86M/9.86M [01:00<00:00, 172kB/s]
Uploading opentrep-0.7.10.tar.gz
100%|█████████████████████████████████████████████████████████████████████| 1.65M/1.65M [00:12<00:00, 139kB/s]

View at:
https://test.pypi.org/project/opentrep/0.7.10/
```

* Upload/release the Python packages onto the
  [PyPi repository](https://pypi.org):
```bash
user@laptop$ PYPIURL="https://upload.pypi.org"
user@laptop$ keyring set ${PYPIURL}/ __token__
Password for '__token__' in '${PYPIURL}/':
user@laptop$ twine upload -u __token__ --repository-url ${PYPIURL}/legacy/ dist/*
Uploading distributions to https://upload.pypi.org/legacy/
Uploading opentrep-0.7.10-cp39-cp39-macosx_12_0_x86_64.whl
100%|█████████████████████████████████████████████████████████████████████| 9.86M/9.86M [01:00<00:00, 172kB/s]
Uploading opentrep-0.7.10.tar.gz
100%|█████████████████████████████████████████████████████████████████████| 1.65M/1.65M [00:12<00:00, 139kB/s]

View at:
https://pypi.org/project/opentrep/0.7.10/
```

## Test the OpenTREP Python extension
* Set the `PYTHONPATH` environment variable to where the system Python
  is installed
  + On Linux:
```bash
$ export PYTHONPATH="${HOME}/.pyenv/versions/3.9.8/lib:${HOME}/.pyenv/versions/3.9.8/lib/python3.9/site-packages/pyopentrep"
  export LD_LIBRARY_PATH="${HOME}/.pyenv/versions/3.9.8/lib:${HOME}/.pyenv/versions/3.9.8/lib/python3.9/site-packages/pyopentrep:${LD_LIBRARY_PATH}"
```
  + On MacOS (Python binary is
    `/usr/local/Cellar/python\@3.9/3.9.8/Frameworks/Python.framework/Versions/3.9/Resources/Python.app/Contents/MacOS/Python`
	and Python libraries are installed simply in `/usr/local/lib`):
```bash
$ export PYTHONPATH="/usr/local/lib:/usr/local/lib/python3.9/site-packages/pyopentrep"
```

* Remove older version of and install the `opentrep` Python extension
  + On Linux:
```bash
$ python -mpip uninstall opentrep
$ python -mpip install -U opentrep # will take a few minutes, as OpenTREP is built again
```
  + On MacOS:
```bash
$ /usr/local/Cellar/python\@3.9/3.9.8/Frameworks/Python.framework/Versions/3.9/Resources/Python.app/Contents/MacOS/Python -mpip uninstall opentrep
$ /usr/local/Cellar/python\@3.9/3.9.8/Frameworks/Python.framework/Versions/3.9/Resources/Python.app/Contents/MacOS/Python -mpip install opentrep==0.7.10
```

* Launch a simple end-to-end test with `pytest`
  + On Linux:
```bash
$ python -mpytest test_trep_e2e_simple.py
```
  + On MacOS:
```bash
$ DYLD_INSERT_LIBRARIES=/Library/Developer/CommandLineTools/usr/lib/clang/13.0.0/lib/darwin/libclang_rt.asan_osx_dynamic.dylib ASAN_OPTIONS=detect_container_overflow=0 /usr/local/Cellar/python\@3.9/3.9.8/Frameworks/Python.framework/Versions/3.9/Resources/Python.app/Contents/MacOS/Python -mpytest test_trep_e2e_simple.py
```
  + It should give something like:
```bash
=============== test session starts ============
platform darwin -- Python 3.9.8, pytest-6.2.5, py-1.11.0, pluggy-1.0.0
rootdir: $HOME/dev/geo/opentrep
collected 1 item

test_trep_e2e_simple.py          [100%]

================== 1 passed in 1.38s ==============
```

## Use the OpenTREP Python extension

### Download the latest OpenTravelData (OPTD) POR data file
* If not already done, install a few more Python modules
  + On Linux:
```bash
$ python -mpip install -U opentrepwrapper opentraveldata
```
  + On MacOS:
```bash
$ DYLD_INSERT_LIBRARIES=/Library/Developer/CommandLineTools/usr/lib/clang/13.0.0/lib/darwin/libclang_rt.asan_osx_dynamic.dylib ASAN_OPTIONS=detect_container_overflow=0 /usr/local/Cellar/python\@3.9/3.9.8/Frameworks/Python.framework/Versions/3.9/Resources/Python.app/Contents/MacOS/Python -mpip install -U opentrepwrapper opentraveldata
```

* Download and use the latest POR data file
  + On Linux:
```bash
$ python
Python 3.9.8 (default, November 2021) 
```
  + On MacOS:
```bash
$ DYLD_INSERT_LIBRARIES=/Library/Developer/CommandLineTools/usr/lib/clang/13.0.0/lib/darwin/libclang_rt.asan_osx_dynamic.dylib ASAN_OPTIONS=detect_container_overflow=0 /usr/local/Cellar/python\@3.9/3.9.8/Frameworks/Python.framework/Versions/3.9/Resources/Python.app/Contents/MacOS/Python
```
  + Python interactive shell:
```python
>>> import opentraveldata
>>> myOPTD = opentraveldata.OpenTravelData()
>>> myOPTD.downloadFilesIfNeeded()
>>> myOPTD
OpenTravelData:
	Local IATA/ICAO POR file: /tmp/opentraveldata/optd_por_public_all.csv
	Local UN/LOCODE POR file: /tmp/opentraveldata/optd_por_unlc.csv
>>> myOPTD.fileSizes()
(44578066, 4966769)
```

### Xapian index initialization
* Initialize the Xapian index with the `-i` option of `pyopentrep.py`,
  so as to index the full OpenTravelData (OPTD) POR (points of reference)
  data file
  + On Linux:
```bash
$ python ~/.pyenv/versions/3.9.8/lib/python3.9/site-packages/pyopentrep/pyopentrep.py -p /tmp/opentraveldata/optd_por_public_all.csv -i
```
  + On MacOS:
```bash
$ DYLD_INSERT_LIBRARIES=/Library/Developer/CommandLineTools/usr/lib/clang/13.0.0/lib/darwin/libclang_rt.asan_osx_dynamic.dylib ASAN_OPTIONS=detect_container_overflow=0 /usr/local/Cellar/python\@3.9/3.9.8/Frameworks/Python.framework/Versions/3.9/Resources/Python.app/Contents/MacOS/Python $HOME/.pyenv/versions/3.9.8/lib/python3.9/site-packages/pyopentrep/pyopentrep.py -p /tmp/opentraveldata/optd_por_public_all.csv -i
```
```bash
OPTD-maintained list of POR (points of reference): '/tmp/opentraveldata/optd_por_public_all.csv'
Xapian-based travel database/index: '/tmp/opentrep/xapian_traveldb0'
SQLite database: '/tmp/opentrep/sqlite_travel.db'
Perform the indexation of the (Xapian-based) travel database.
That operation may take several minutes on some slow machines.
It takes less than 20 seconds on fast ones...
Number of actually parsed records: 1,000, out of 103,394 records in the POR data file so far
...
Number of actually parsed records: 20,000, out of 122,394 records in the POR data file so far
Done. Indexed 20348 POR (points of reference)
```

* The Xapian index may also be initialized with the C++ (non-Python)
  `opentrep-indexer` utility (that is the former way of initializing
  the Xapian index, when it was not available from the Python utility):
```bash
$ ${TREPBINDIR}/opentrep-indexer -t sqlite -a 1 -p ${OPTDPOR}
POR file-path is: ~/.pyenv/versions/3.9.8/share/opentrep/data/por/test_optd_por_public.csv
Deployment number: 0
Xapian index/database filepath is: /tmp/opentrep/xapian_traveldb0
SQL database type is: sqlite
SQL database connection string is: /tmp/opentrep/sqlite_travel.db0
Are non-IATA-referenced POR included? 0
Index the POR in Xapian? 1
Add and re-index the POR in the SQL-based database? 1
Log filename is: opentrep-indexer.log
Parsing and indexing the OpenTravelData POR data file (into Xapian and/or SQL databases) may take a few tens of minutes on some architectures (and a few minutes on fastest ones)...
9 entries have been processed
```

### Search with the OpenTrep Python extension
* Use a Python wrapper script around the OpenTrep Python extension to search
  for terms
  + On Linux:
```bash
$ python ~/.pyenv/versions/3.9.8/lib/python3.9/site-packages/pyopentrep/pyopentrep.py
```
  + On MacOS:
```bash
$ DYLD_INSERT_LIBRARIES=/Library/Developer/CommandLineTools/usr/lib/clang/13.0.0/lib/darwin/libclang_rt.asan_osx_dynamic.dylib ASAN_OPTIONS=detect_container_overflow=0 /usr/local/Cellar/python\@3.9/3.9.8/Frameworks/Python.framework/Versions/3.9/Resources/Python.app/Contents/MacOS/Python $HOME/.pyenv/versions/3.9.8/lib/python3.9/site-packages/pyopentrep/pyopentrep.py
```
```bash
OPTD-maintained list of POR (points of reference): '/tmp/opentrep/test_optd_por_public.csv'
Xapian-based travel database/index: '/tmp/opentrep/xapian_traveldb0'
SQLite database: '/tmp/opentrep/sqlite_travel.db'
searchString: sna francisco rio de janero los angeles reykyavki
Compact format => recognised place (city/airport) codes:
SFO RIO LAX REK
------------------
```

* When the full POR data file has been indexed
  + On Linux:
```bash
$ python ~/.pyenv/versions/3.9.8/lib/python3.9/site-packages/pyopentrep/pyopentrep.py -f F "cnsha deham deess"
```
  + On MacOS:
```bash
$ DYLD_INSERT_LIBRARIES=/Library/Developer/CommandLineTools/usr/lib/clang/13.0.0/lib/darwin/libclang_rt.asan_osx_dynamic.dylib ASAN_OPTIONS=detect_container_overflow=0 /usr/local/Cellar/python\@3.9/3.9.8/Frameworks/Python.framework/Versions/3.9/Resources/Python.app/Contents/MacOS/Python $HOME/.pyenv/versions/3.9.8/lib/python3.9/site-packages/pyopentrep/pyopentrep.py -f F "cnsha deham deess"
```
```bash
OPTD-maintained list of POR (points of reference): '/tmp/opentrep/test_optd_por_public.csv'
Xapian-based travel database/index: '/tmp/opentrep/xapian_traveldb0'
SQLite database: '/tmp/opentrep/sqlite_travel.db'
searchString: cnsha deham deess
Raw result from the OpenTrep library:
1. SHA-A-6301388, 34.7007%, Shanghai Hongqiao International Airport, Shanghai Hongqiao International Airport, ZSSS, , CNSHA, , 0, 1970-Jan-01, 2999-Dec-31, , SHA|1796236|Shanghai|Shanghai|CN|SH, SH, CN, , China, 713, China, CNY, NA, Asia, 31.1979, 121.336, S, AIRP, 23, Shanghai, Shanghai, , , , Z, , 0, 3, 3, Asia/Shanghai, 8, 8, 8, 2014-Aug-01, , https://en.wikipedia.org/wiki/Shanghai_Hongqiao_International_Airport, 31.1979, 121.336, cnsha, cnsha, 34.7007%, 0, 0
2. HAM-C-2911298, 12.8103%, Hamburg, Hamburg, , , DEHAM, , 0, 1970-Jan-01, 2999-Dec-31, , HAM|2911298|Hamburg|Hamburg|DE|HH, HH, DE, , Germany, 429, Germany, EUR, NA, Europe, 53.5753, 10.0153, P, PPLA, 04, Hamburg, Hamburg, 00, , , 02000, 02000000, 1739117, 0, 2, Europe/Berlin, 1, 2, 1, 2019-Nov-28, HAM,LBC,OBZ,XFW,ZMB, https://en.wikipedia.org/wiki/Hamburg, 53.5507, 9.99302, deham, deham, 12.8103%, 0, 0
3. ESS-C-2928810, 1.34094%, Essen, Essen, , , DEESS, , 0, 1970-Jan-01, 2999-Dec-31, , ESS|2928810|Essen|Essen|DE|NW, NW, DE, , Germany, 429, Germany, EUR, NA, Europe, 51.4566, 7.01228, P, PPLA3, 07, North Rhine-Westphalia, North Rhine-Westphalia, 051, Düsseldorf District, Duesseldorf District, 05113, 05113000, 593085, 0, 83, Europe/Berlin, 1, 2, 1, 2019-Oct-22, ESS,ESZ, https://en.wikipedia.org/wiki/Essen, 0, 0, deess, deess, 1.34094%, 0, 0
------------------
```

* Use the OpenTrep Python extension from a Python interactive shell
  + On Linux:
```bash
$ python
Python 3.9.8 (default, November 2021)
```
  + On MacOS:
```bash
$ DYLD_INSERT_LIBRARIES=/Library/Developer/CommandLineTools/usr/lib/clang/13.0.0/lib/darwin/libclang_rt.asan_osx_dynamic.dylib ASAN_OPTIONS=detect_container_overflow=0 /usr/local/Cellar/python\@3.9/3.9.8/Frameworks/Python.framework/Versions/3.9/Resources/Python.app/Contents/MacOS/Python
```
```python
>>> import pyopentrep
>>> openTrepLibrary = pyopentrep.OpenTrepSearcher()
>>> initOK = openTrepLibrary.init ('/tmp/opentraveldata/optd_por_public.csv', '/tmp/opentrep/xapian_traveldb', 'sqlite', '/tmp/opentrep/sqlite_travel.db', 0, False, True, True, 'pyopentrep.log')
>>> openTrepLibrary.search('S', 'nce sfo')
'NCE/0,SFO/0'
>>> openTrepLibrary.search('F', 'nce sfo')
"1. NCE-C-2990440, 8.16788%, Nice, Nice, , , FRNCE, , 0, 1970-Jan-01, 2999-Dec-31, , NCE|2990440|Nice|Nice|FR|PAC, PAC, FR, , France, 427, France, EUR, NA, Europe, 43.7031, 7.26608, P, PPLA2, 93, Provence-Alpes-Côte d'Azur, Provence-Alpes-Cote d'Azur, 06, Alpes-Maritimes, Alpes-Maritimes, 062, 06088, 338620, 25, 18, Europe/Paris, 1, 2, 1, 2019-Sep-05, NCE, https://en.wikipedia.org/wiki/Nice, 0, 0, NA, nce, 0%, 0, 0\n2. SFO-C-5391959, 32.496%, San Francisco, San Francisco, , , USSFO, , 0, 1970-Jan-01, 2999-Dec-31, , SFO|5391959|San Francisco|San Francisco|US|CA, CA, US, , United States, 91, California, USD, NA, North America, 37.7749, -122.419, P, PPLA2, CA, California, California, 075, City and County of San Francisco, City and County of San Francisco, Z, , 864816, 16, 28, America/Los_Angeles, -8, -7, -8, 2019-Sep-05, SFO, https://en.wikipedia.org/wiki/San_Francisco, 0, 0, NA, sfo, 0%, 0, 0\n"
>>> quit()
```

### Search with the OpenTrepWrapper package
* Use the OpenTREP wrapper on the full index
  + On Linux:
```bash
$ python
Python 3.9.8 (default, November 2021)
```
  + On MacOS:
```bash
$ DYLD_INSERT_LIBRARIES=/Library/Developer/CommandLineTools/usr/lib/clang/13.0.0/lib/darwin/libclang_rt.asan_osx_dynamic.dylib ASAN_OPTIONS=detect_container_overflow=0 /usr/local/Cellar/python\@3.9/3.9.8/Frameworks/Python.framework/Versions/3.9/Resources/Python.app/Contents/MacOS/Python
```
  + Python interactive shell:
```python
>>> from OpenTrepWrapper import main_trep, index_trep
>>> main_trep (searchString = 'nce sfo', outputFormat = 'S',  xapianDBPath = '/tmp/opentrep/xapian_traveldb',  logFilePath = '/tmp/opentrep/opeentrep-searcher.log',  verbose = False)
([(173.579, 'NCE'), (390.644, 'SFO')], '')
>>> main_trep (searchString = 'cnsha deham', outputFormat = 'S',  xapianDBPath = '/tmp/opentrep/xapian_traveldb',  logFilePath = '/tmp/opentrep/opeentrep-searcher.log',  verbose = False)
([(0.34700699999999995, 'SHA'), (0.128103, 'HAM')], '')
>>> quit()
```

# (Optional) Running the Django-based application server (needs update)
* Install Django as a Python module
  + On Linux:
```bash
$ python -mpip -U django
```
  + On MacOS:
```bash
$ DYLD_INSERT_LIBRARIES=/Library/Developer/CommandLineTools/usr/lib/clang/13.0.0/lib/darwin/libclang_rt.asan_osx_dynamic.dylib ASAN_OPTIONS=detect_container_overflow=0 /usr/local/Cellar/python\@3.9/3.9.8/Frameworks/Python.framework/Versions/3.9/Resources/Python.app/Contents/MacOS/Python -mpip -U django
```

* Go in the Django directory:
```bash
$ export TREP_TRAVELDB=/tmp/opentrep/traveldb
$ export TREP_LOG=django_trep.log
$ pushd gui/django/webapps/opentrep
```

* Start the Django Web application server
  + On Linux:
```bash
$ python manage.py runserver localhost:8000
Python 3.9.8 (default, November 2021)
```
  + On MacOS (as of end 2021, does not work):
```bash
$ DYLD_INSERT_LIBRARIES=/Library/Developer/CommandLineTools/usr/lib/clang/13.0.0/lib/darwin/libclang_rt.asan_osx_dynamic.dylib ASAN_OPTIONS=detect_container_overflow=0 /usr/local/Cellar/python\@3.9/3.9.8/Frameworks/Python.framework/Versions/3.9/Resources/Python.app/Contents/MacOS/Python manage.py runserver localhost:8000
```

* Query OpenTREP with a web browser:
```bash
$ open "http://localhost:8000/search?q=rio de janero reykyavik sna francicso"
```

* Go back to the top directory:
```bash
$ popd
```

