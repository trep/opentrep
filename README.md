
# Overview
OpenTREP aims at providing a clean API, and the corresponding C++
implementation, for parsing travel-focused requests.
It powers the http://search.transport-intelligence.com Web site (as well as
its newer and older versions, http://www2.search-travel.org and
http://search-travel.org respectively).

OpenTREP uses Xapian (http://www.xapian.org) for the Information Retrieval part,
on freely available transport-/travel-related data (e.g., country names and
codes, city names and codes, airline names and codes, etc.), mainly to be found
in the [OpenTravelData project](http://github.com/opentraveldata/opentraveldata):
http://github.com/opentraveldata/opentraveldata/tree/master/opentraveldata

OpenTREP exposes a simple, clean and object-oriented, API. For instance,
the ``OPENTREP::interpretTravelRequest()`` method:
* takes, as input, a string containing the travel request;
* and yields, as output, the list of the recognized terms,
  as well as their corresponding types.

As an example, the travel request
'Washington DC Beijing Monday a/r +AA -UA 1 week 2 adults 1 dog' would yield
the following list:
 * Origin airport: Washington, DC, USA
 * Destination airport: Beijing, China
 * Date of travel: next Monday
 * Date of return: 1 week after next Monday
 * Preferred airline: American Airlines; non-preferred airline: United Airlines
 * Number of travelers: 2 adults and a dog

The output can then be used by other systems, for instance to book the
corresponding travel, or to visualize it on a map and calendar and to
share it with others.

Note that the current version of OpenTREP recognizes only geographical POR
(points of reference), whatever their number in the request. For instance,
the request ``lviv rivne jitomir kbp kharkiv dnk ods lwo`` would yield the
following list of POR: ``LWO``, ``RWN``, ``ZTR``, ``KBP``, ``HRK``, ``DNK``,
``ODS`` and ``LWO`` again.
See [that request in action on the search.transport-intelligence.com site](http://search.transport-intelligence.com/search/?q=lwo+rwn+ztr+kbp+hrk+dnk+ods+lwo).

The underlying data for the POR is the
[OpenTravelData ``optd_por_public_all.csv`` file](http://github.com/opentraveldata/opentraveldata/tree/master/opentraveldata/optd_por_public_all.csv).
A good complementary tool is
[GeoBase](http://opentraveldata.github.io/geobases/),
a Python-based software able to access any travel-related data source.

OpenTREP makes an extensive use of existing open-source libraries for
increased functionality, speed and accuracy. In particular the
[Boost](http://www.boost.org)(C++ Standard Extensions),
[Xapian](http://xapian.org) and [SOCI](http://soci.sourceforge.net) libraries
are used.

# Use Docker images
Docker images are ready to use on Docker Hub: https://hub.docker.com/r/opentrep/search-travel/
```bash
$ docker pull opentrep/search-travel:legacy
$ docker run --rm -it opentrep/search-travel:legacy bash
```
See https://github.com/trep/opentrep/tree/trunk/gui/legacy for more details.

# Installation
## RPM-based distributions (eg, Fedora/CentOS/RedHat)
Just use DNF (or Yum for the older distributions):
```bash
$ dnf -y install opentrep opentrep-doc
```

You can also get the RPM packages (which may work on Linux
distributions like Novel Suse and Mandriva) from the Fedora repository
(e.g., for Fedora 28,
http://fr2.rpmfind.net/linux/fedora/releases/28/Everything/)

## Installation from the sources
### Clone the Git repository
The GitHub repository may be cloned as following:
```bash
$ mkdir -p ~/dev/geo
$ cd ~/dev/geo
$ git clone https://github.com/trep/opentrep.git
$ cd opentrep
$ git checkout trunk
```

### Alternatively, download and extract the tar-ball
GitHub generates tar-balls on the fly for every tagged release:
https://github.com/trep/opentrep/releases. For instance:
```bash
$ wget https://github.com/trep/opentrep/archive/opentrep-0.07.0.tar.gz
```
Note that SourceForge also stores some
[older archived tar-balls](http://sourceforge.net/project/showfiles.php?group_id=252803).

### Installation of the dependencies
#### On Linux
The following packages may be needed (Fedora/RedHat/CentOS names on the left
hand side, Debian/Ubuntu names on the right hand side; names for other
Linux distributions may vary):
* ``cmake``
* ``gcc-c++``
* ``boost-devel`` / ``libboost-dev``
* ``xapian-core-devel`` / ``xapian-dev``
* ``python-devel`` / ``python-dev``
* ``libicu-devel`` / ``libicu-dev``
* ``soci-mysql-devel``, ``soci-sqlite3-devel`` / ``soci-mysql-dev``,
  ``soci-sqlite3-dev``
* ``sqlite3-devel`` / ``libsqlite3-dev``
* ``mariadb-devel`` / ``mariadb-dev``
* ``protobuf-devel``, ``protobuf-compiler`` / ``protobuf-dev``
* ``readline-devel`` / ``readline-dev``
* ``doxygen``
* ``tetex-latex`` (optional)
* ``rpm-build`` (optional)

For instance, on Fedora, the following installation command should
pull all the required dependencies:
```bash
$ dnf -y install git-all bash-completion gcc-c++ cmake boost-devel \
 xapian-core-devel soci-mysql-devel soci-sqlite3-devel readline-devel \
 sqlite-devel mariadb-devel libicu-devel protobuf-devel protobuf-compiler
$ # Python development (the package names may vary)
$ dnf -y install python-devel python-pip
$ # Documentation tools
$ dnf -y install doxygen ghostscript "tex(latex)"
```

### On MacOS
* With Homebrew:
```bash
$ brew install boost boost-python boost-python3 cmake libedit \
  xapian sqlite mysql icu4c protobuf protobuf-c doxygen
$ brew install homebrew/portable-ruby/portable-readline
```
 
#### SOCI
* On MacOS, as of mid-2018, SOCI 4.0 has still not been released,
  and ``soci-mysql`` is no longer available. Hence, SOCI must be built
  from the sources. The following shows how to do that:
```bash
$ mkdir -p ~/dev/infra/soci && cd ~/dev/infra/soci
$ git clone https://github.com/SOCI/soci.git
$ cd soci
$ mkdir build && cd build
$ cmake ..
$ make
$ sudo make install
```

### Building the library and test binary
To customize the following to your environment, you can alter
the installation directory:
```bash
export INSTALL_BASEDIR=$HOME/dev/deliveries
export TREP_VER=0.6.6
if [ -d /usr/lib64 ]; then LIBSUFFIX=64; fi
export LIBSUFFIX_4_CMAKE="-DLIB_SUFFIX=$LIBSUFFIX"
```

Then, as usual:
* To configure the project, type something like:
```bash
$ mkdir build && cd build
$ cmake -DCMAKE_INSTALL_PREFIX=${INSTALL_BASEDIR}/opentrep-$TREP_VER \
   -DCMAKE_BUILD_TYPE:STRING=Debug -DINSTALL_DOC:BOOL=ON \
   -DRUN_GCOV:BOOL=OFF ${LIBSUFFIX_4_CMAKE} ..
```

* For an *ad hoc* installation:
```bash
INSTALL_DIR=/var/www/webapps/opentrep/trep

cmake -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
   -DCMAKE_BUILD_TYPE:STRING=Debug -DINSTALL_DOC:BOOL=OFF \
   -DRUN_GCOV:BOOL=OFF ${LIBSUFFIX_4_CMAKE} ..
```

* On MacOS, a few software (e.g., ICU and Readline) are not in
the standard place. So, the ``cmake`` command becomes:
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
* To install the library (libopentrep*.so*) and the binary (opentrep), just type:
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
* To run the local binary version:
```bash
$ ./opentrep/opentrep-{dbmgr,indexer,searcher}
```
* To run the installed version (the first following line must be done once
  per session):
```bash
$ export TREP_LIB=$INSTALL_BASEDIR/opentrep-$TREP_VER/lib$LIBSUFFIX
$ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$TREP_LIB
$ $INSTALL_BASEDIR/opentrep-$TREP_VER/bin/opentrep-{dbmgr,indexer,searcher}
```

## Underlying (relational) database, SQLite or MySQL/MariaDB, if any
OpenTREP may use, if so configured, a relational database. For now,
two database products are supported, SQLite3 and MySQL/MariaDB.
The database accelerates the look up of POR by (IATA, ICAO, FAA) codes
and of Geonames ID. When OpenTREP is configured to run without database,
those codes and Geonames ID are full-text searched directly with Xapian.
Note that the database can be managed directly, i.e., without the
OpenTREP search interface on top of it, thanks to the opentrep-dbmgr utility,
which is detailed below.

# Indexing the POR data
## Filling the (relational) database, SQLite or MySQL/MariaDB, if any
Here, for clarity reason, we use the local version. It is easy (see above)
to derive the same commands with the installed version.

* The following command prompts a shell:
```bash
$ ./opentrep/opentrep-dbmgr -t sqlite
```

* Then, within the ``opentrep>`` shell, a typical sequence for SQLite would be:
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
./opentrep/opentrep-dbmgr -t mysql
```

* Then, within the ``opentrep>`` shell, a typical sequence for MySQL/MariaDB
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

## Xapian indexing with standard installation
By default, the Xapian indexer runs without filling any relational database,
as that step can be performed independantly by the opentrep-dbmgr, as seen above.

* Xapian indexing without any relational database:
```bash
$ ./opentrep/opentrep-indexer
```

* Xapian indexing and filling and indexing the SQLite database:
```bash
$ ./opentrep/opentrep-indexer -t sqlite
```

* Xapian indexing and filling and indexing the MySQL/MariaDB database:
```bash
$ ./opentrep/opentrep-indexer -t mysql
```

* There is an option to not even touch Xapian at all, for instance to check
  that the [OpenTravelData POR data file](http://github.com/opentraveldata/opentraveldata/tree/master/opentraveldata/optd_por_public_all.csv)
  is well-formed:
```bash
$ ./opentrep/opentrep-indexer -x 0
```

## Xapian indexing for an ad hoc deployed Web application
* Xapian indexing without any relational database:
```bash
$ ./opentrep/opentrep-indexer -d /var/www/webapps/opentrep/trep/traveldb
```

* (Optional) Filling and indexing the SQLite database:
```bash
$ ./opentrep/opentrep-dbmgr -t sqlite -s /var/www/webapps/opentrep/trep/sqlite_travel.db
    create_user
    fill_from_por_file
    quit
```

# Searching
* Searching without any relational database support:
```bash
$ ./opentrep/opentrep-searcher -q nce sfo
```

* Searching with SQLite (note that it should be quicker than without a database):
```bash
$ ./opentrep/opentrep-searcher -t sqlite -q nce sfo
```

* Searching with MySQL/MariaDB:
```bash
$ ./opentrep/opentrep-searcher -t mysql -q nce sfo
```

* Searching with SQLite (with Xapian and SQLite DB in a ``webapps`` directory):
```bash
$ ./opentrep/opentrep-searcher -d /var/www/webapps/opentrep/trep/traveldb -t sqlite -s /var/www/webapps/opentrep/trep/sqlite_travel.db -q nce sfo
```

# Deployment stages
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
(``opentrep-dbmgr``, ``opentrep-indexer``, ``opentrep-searcher``) is ``-m``.

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

# Index, or not, non-IATA POR
There is also a command-line option, namely ``-n``, to state whether or not
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
only used at indexing time (ie, by the ``opentrep-dbmgr`` and
``opentrep-indexer`` programs).

# Installing a Python virtual environment
## General
* Install ``pip`` and ``virtualenv`` for Python:
```bash
$ pip install --upgrade pip
$ pip install virtualenv
```

## Python 2
* Install a Python 2 virtual environment:
```bash
$ cd ~/dev
$ PYTHON_VERSION=$(python --version 2>&1 |cut -d' ' -f2,2|cut -d'.' -f1,2)
$ virtualenv -p python2 venv${PYTHON_VERSION}
$ ln -s venv${PYTHON_VERSION} venv2
$ source ~/dev/venv2/bin/activate
```

## Python 3
* Install a Python 3 virtual environment:
```bash
$ cd ~/dev
$ PYTHON_VERSION=$(python3 --version 2>&1 |cut -d' ' -f2,2|cut -d'.' -f1,2)
$ python3 -m venv venv${PYTHON_VERSION}
$ ln -s venv${PYTHON_VERSION} venv3
$ source ~/dev/venv3/bin/activate
```

# Checking that the Python module works
* With the standard installation:
```bash
PYTHONPATH=${INSTALL_BASEDIR}/opentrep-${TREP_VER}/lib${LIBSUFFIX}:${INSTALL_BASEDIR}/opentrep-${TREP_VER}/lib${LIBSUFFIX}/python${PYTHON_VERSION}/site-packages/pyopentrep \
 python -c "import pyopentrep; \
 openTrepLibrary = pyopentrep.OpenTrepSearcher(); \
 initOK = openTrepLibrary.init ('/tmp/opentrep/xapian_traveldb', 'sqlite', '/tmp/opentrep/sqlite_travel.db', 0, 'pyopentrep.log'); \
 print (openTrepLibrary.search ('S', 'los las'))"
```

* With an *ad hoc* installation:
```bash
PYTHONPATH=${INSTALL_DIR}/lib${LIBSUFFIX}:${INSTALL_BASEDIR}/lib${LIBSUFFIX}/python${PYTHON_VERSION}/site-packages/pyopentrep \
 python -c "import pyopentrep; \
 openTrepLibrary = pyopentrep.OpenTrepSearcher(); \
 initOK = openTrepLibrary.init ('/var/www/webapps/opentrep/trep/traveldb', 'mysql', 'db=trep_trep user=trep password=trep', 0, 'pyopentrep.log'); \
 print (openTrepLibrary.search ('S', 'los las'))"
```

# Running the Django-based application server
```bash
$ export TREP_LIB=${INSTALL_BASEDIR}/opentrep-${TREP_VER}/lib${LIBSUFFIX}
```

# Optional:
```bash
$ export TREP_TRAVELDB=/tmp/opentrep/traveldb
$ export TREP_LOG=django_trep.log
$ cd gui/django/opentrep
$ # The first time, the database must be initialised:
$ #./manage.py syncdb localhost:8000
$ # Add the Python library directories to PYTHONPATH:
$ export PYTHONPATH=${INSTALL_BASEDIR}/opentrep-${TREP_VER}/lib${LIBSUFFIX}:${INSTALL_BASEDIR}/opentrep-${TREP_VER}/lib${LIBSUFFIX}/python${PYTHON_VERSION}/site-packages/pyopentrep
$ # Start the Django Web application server
$ ./manage.py runserver localhost:8000
$ # In another Shell, check that everything went fine with, for instance:
$ midori http://localhost:8000/search?q=rio de janero reykyavik sna francicso
```

Denis Arnaud (October 2018)

