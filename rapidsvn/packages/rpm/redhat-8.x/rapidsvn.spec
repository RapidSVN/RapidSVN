Summary: A cross-platform GUI for the Subversion concurrent versioning system.
Name: rapidsvn
Version: @VERSION@
Release: @RELEASE@
Copyright: BSD
Group: Utilities/System
URL: http://rapidsvn.tigris.org
Source0: %{name}-%{version}-%{release}.tar.gz
Packager: David Summers <david@summersoft.fay.ar.us>
Requires: subversion
#Requires: /sbin/install-info
BuildPreReq: apr-devel
BuildPreReq: apr-util-devel
BuildPreReq: autoconf >= 2.53
BuildPreReq: docbook-style-xsl >= 1.58.1
BuildPreReq: doxygen
BuildPreReq: libtool >= 1.4.2
BuildPreReq: libxslt >= 1.0.27
BuildPreReq: neon-devel
BuildPreReq: subversion-devel >= 0.37.0
BuildPreReq: wxGTK-devel >= 2.4.2
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
* Tue Dec 30 2003 David Summers <david@summersoft.fay.ar.us> 0.5.0-7013
- Added build pre-requisites for apr-devel, apr-util-devel, and neon-devel.

* Tue Dec 30 2003 David Summers <david@summersoft.fay.ar.us> 0.4.0-6977
- Took out requirement for httpd and apr as that requirement is handled by
  the requirement for subversion.

* Sat Dec 27 2003 David Summers <david@summersoft.fay.ar.us> 0.4.0-6962
- Requires subversion-devel-0.34.0.
- Requires apache-2.0.48.
- Requires apr-0.9.5.

* Wed Jul 31 2002 David Summers <david@summersoft.fay.ar.us> 0.1
- First version

%prep
%setup -q

sh ./autogen.sh

# Fix up to include subversion include directory.
CPPFLAGS="-I/usr/include/subversion-1"
export CPPFLAGS
./configure \
	--prefix=/usr \
	--with-wx-config=/usr/bin/wxgtk-2.4-config \
	--with-apr-config=/usr/bin/apr-config \
	--with-apu-config=/usr/bin/apu-config \
	--with-svn-include=/usr/include \
	--with-svn-lib=/usr/lib \
	--with-docbook-xsl=/usr/share/sgml/docbook/xsl-stylesheets \
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
/usr/man/man1/*
