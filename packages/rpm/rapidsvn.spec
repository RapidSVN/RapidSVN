%define apache_version 2.0.40-0.8
%define neon_version 0.21.3
%define apache_dir /usr/local/apache2
Summary: A cross-platform GUI for the Subversion concurrent versioning system.
Name: rapidsvn
Version: @VERSION@
Release: @RELEASE@
Copyright: BSD
Group: Utilities/System
URL: http://rapidsvn.tigris.org
Source0: %{name}-%{version}-%{release}.tar.gz
Patch0: rapidsvn.include.patch
Vendor: Summersoft
Packager: David Summers <david@summersoft.fay.ar.us>
Requires: apache-libapr >= %{apache_version}
Requires: apache-libapr-utils >= %{apache_version}
Requires: subversion
#Requires: /sbin/install-info
BuildPreReq: wxGTK-devel
BuildPreReq: apache >= %{apache_version}
BuildPreReq: apache-devel >= %{apache_version}
BuildPreReq: apache-libapr-devel >= %{apache_version}
BuildPreReq: apache-libapr-utils-devel >= %{apache_version}
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

if [ -f /usr/bin/autoconf-2.53 ]; then
   AUTOCONF="autoconf-2.53"
   AUTOHEADER="autoheader-2.53"
   export AUTOCONF AUTOHEADER
fi

${AUTOCONF}

%patch0 -p0

./configure --with-apr-config=/usr/local/apache2/bin/apr-config

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
