%define apache_version 2.0.43-0.1
Summary: A cross-platform GUI for the Subversion concurrent versioning system.
Name: rapidsvn
Version: @VERSION@
Release: @RELEASE@
Copyright: BSD
Group: Utilities/System
URL: http://rapidsvn.tigris.org
Source0: %{name}-%{version}-%{release}.tar.gz
Patch0: rapidsvn.include.patch
Patch1: apr.patch
Packager: David Summers <david@summersoft.fay.ar.us>
Requires: httpd-apr >= %{apache_version}
Requires: subversion
#Requires: /sbin/install-info
BuildPreReq: subversion-devel
BuildPreReq: wxGTK-devel >= 2.3.3
BuildPreReq: httpd >= %{apache_version}
BuildPreReq: httpd-devel >= %{apache_version}
BuildPreReq: httpd-apr-devel >= %{apache_version}
BuildPreReq: autoconf253 >= 2.53
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
* Wed Jul 31 2002 David Summers <david@summersoft.fay.ar.us> 0.1
- First version

%prep
%setup -q

AUTOCONF="autoconf"
AUTOHEADER="autoheader"

if [ -f /usr/bin/autoconf-2.53 ]; then
   AUTOCONF="autoconf-2.53"
   AUTOHEADER="autoheader-2.53"
fi

export AUTOCONF AUTOHEADER

${AUTOCONF}

%patch0 -p0

./configure \
	--with-wx-config=/usr/bin/wxgtk-2.3-config \
	--with-apr-config=/usr/bin/apr-config \
	--disable-no-exceptions

#%patch1 -p0

%build
cd src
make

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr/bin

cd src
cp rapidsvn $RPM_BUILD_ROOT/usr/bin

%clean
#rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
/usr/bin/rapidsvn
