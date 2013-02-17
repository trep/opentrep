#
%global mydocs __tmp_docdir
#
Name:           opentrep
Version:        0.5.2
Release:        1%{?dist}

Summary:        C++ library providing a clean API for parsing travel-focused requests

Group:          System Environment/Libraries 
License:        LGPLv2+
URL:            http://%{name}.sourceforge.net
Source0:        http://downloads.sourceforge.net/%{name}/%{name}-%{version}.tar.bz2
BuildRoot:      %(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)

BuildRequires:  cmake, python-devel, xapian-core-devel
BuildRequires:  boost-devel, libicu-devel

%description
%{name} aims at providing a clean API, and the corresponding C++
implementation, for parsing travel-focused requests (e.g.,
"washington dc beijing monday a/r +aa -ua 1 week 2 adults 1 dog").

%{name} uses Xapian (http://www.xapian.org) for the Information Retrieval part,
on freely available travel-related data (e.g., country names and codes,
city names and codes, airline names and codes, etc.).

%{name} exposes a simple, clean and object-oriented, API. For instance,
the static Parse() method takes, as input, a string containing the travel
request, and yields, as output, the list of the recognised terms as well as
their corresponding types. As an example, the travel request
"washington dc beijing monday a/r +aa -ua 1 week 2 adults 1 dog" would give
the following list:
  * Origin airport: Washington, DC, USA
  * Destination airport: Beijing, China
  * Date of travel: next Monday
  * Date of return: 1 week after next Monday
  * Preferred airline: American Airlines; non-preferred airline: United Airlines
  * Number of travellers: 2 adults and a dog

The output can then be used by other systems, for instance to book the
corresponding travel or to visualise it on a map and calendar and to
share it with others.

%{name} makes an extensive use of existing open-source libraries for
increased functionality, speed and accuracy. In particular the
Boost (C++ Standard Extensions: http://www.boost.org) library is used.

%package        devel
Summary:        Header files, libraries and development helper tools for %{name}
Group:          Development/Libraries
Requires:       %{name}%{?_isa} = %{version}-%{release}
Requires:       pkgconfig

%description    devel
This package contains the header files, shared libraries and
development helper tools for %{name}. If you would like to develop
programs using %{name}, you will need to install %{name}-devel.

%package        doc
Summary:        HTML documentation for the %{name} library
Group:          Documentation
%if 0%{?fedora} || 0%{?rhel} > 5
BuildArch:      noarch
%endif
BuildRequires:  tex(latex)
BuildRequires:  doxygen, ghostscript

%description    doc
This package contains HTML pages, as well as a PDF reference manual,
for %{name}. All that documentation is generated thanks to Doxygen
(http://doxygen.org). The content is the same as what can be browsed
online (http://%{name}.org).


%prep
%setup -q


%build
%cmake .
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT

mkdir -p %{mydocs}
mv $RPM_BUILD_ROOT%{_docdir}/%{name}-%{version}/html %{mydocs}
rm -f %{mydocs}/html/installdox

%check
#ctest

%clean
rm -rf $RPM_BUILD_ROOT

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%doc AUTHORS ChangeLog COPYING NEWS README
%{_bindir}/%{name}-indexer
%{_bindir}/%{name}-searcher
%{_bindir}/py%{name}
%{_libdir}/lib%{name}.so.*
%{_libdir}/libpy%{name}.so.*
%{_mandir}/man1/py%{name}.1.*
%{_mandir}/man1/%{name}-indexer.1.*
%{_mandir}/man1/%{name}-searcher.1.*
%dir %{_datadir}/%{name}
%dir %{_datadir}/%{name}/data
%dir %{_datadir}/%{name}/data/por
%{_datadir}/%{name}/data/por/ori_por_public.csv

%files devel
%{_includedir}/%{name}
%{_bindir}/%{name}-config
%{_libdir}/lib%{name}.so
%{_libdir}/libpy%{name}.so
%{_libdir}/pkgconfig/%{name}.pc
%{_datadir}/aclocal/%{name}.m4
%{_datadir}/%{name}/CMake
%{_mandir}/man1/%{name}-config.1.*
%{_mandir}/man3/%{name}-library.3.*

%files doc
%doc %{mydocs}/html
%doc COPYING


%changelog
* Sat Feb 23 2013 Denis Arnaud <denis.arnaud_fedora@m4x.org> 0.5.2-1
- Upstream update

* Sat Feb 16 2013 Denis Arnaud <denis.arnaud_fedora@m4x.org> 0.5.1-1
- Upstream update

* Thu Oct 25 2012 Denis Arnaud <denis.arnaud_fedora@m4x.org> 0.5.0-2
- Took into account review request #866265 feedback

* Sun Oct 14 2012 Denis Arnaud <denis.arnaud_fedora@m4x.org> 0.5.0-1
- Upstream update

* Tue Nov 01 2011 Denis Arnaud <denis.arnaud_fedora@m4x.org> 0.4.0-1
- The build system is now based on CMake (instead of the GNU Autotools)

* Sun Mar 29 2009 Denis Arnaud <denis.arnaud_fedora@m4x.org> 0.3.0-1
- Now relies on the new SOCI RPM

* Sun Mar 22 2009 Denis Arnaud <denis.arnaud_fedora@m4x.org> 0.2.0-1
- RPM release for Fedora 10

