%define apache_version 2.0.48-0.1
Summary: A cross-platform GUI for the Subversion concurrent versioning system.
Name: rapidsvn
Version: @VERSION@
Release: @RELEASE@
Copyright: BSD
Group: Utilities/System
URL: http://rapidsvn.tigris.org
Source0: %{name}-%{version}-%{release}.tar.gz
Packager: David Summers <david@summersoft.fay.ar.us>
Requires: apache-libapr >= %{apache_version}
Requires: subversion
#Requires: /sbin/install-info
BuildPreReq: subversion-devel = 0.34.0
BuildPreReq: wxGTK-devel >= 2.3.3
BuildPreReq: apache-devel >= %{apache_version}
BuildPreReq: apache-libapr-devel >= %{apache_version}
BuildPreReq: autoconf >= 2.53
BuildPreReq: libtool >= 1.4.2
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}
Prefix: /usr
%description
Subversion does the same thing CVS does (Concurrent Versioning System) but has
major enhancements compared to CVS.

This is a GUI for Subversion.

*** Note: This is a relocatable package; it can be installed anywhere you like
with the "rpm -Uvh --prefix /your/favorite/path" command. This is useful
if you don't have root access on your machine but would like to use this
package.

%changelog
* Sat Dec 27 2003 David Summers <david@summersoft.fay.ar.us> 0.4.0-6962
- Requires subversion-devel-0.34.0.
- Requires apache-2.0.48.

* Wed Jul 31 2002 David Summers <david@summersoft.fay.ar.us> 0.1
- First version

%prep
%setup -q

sh ./autogen.sh

./configure \
	--prefix=/usr \
	--with-wx-config=/usr/bin/wxgtk-2.3-config \
	--with-apr-config=/usr/local/apache2/bin/apr-config \
	--with-svn-include=/usr/include \
	--with-svn-lib=/usr/lib \
	--disable-no-exceptions

%build
cd src
make

%install
rm -rf $RPM_BUILD_ROOT

make install DESTDIR=$RPM_BUILD_ROOT

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
/usr/bin/*
/usr/lib/*
