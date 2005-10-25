#!/bin/sh

aclocal
autoconf
# on some platforms "libtoolize" is "glibtoolize"
# like on Mac OS/X. In this case, if "libtoolize" fails
# run "glibtoolize"
libtoolize --automake --force || glibtoolize --automake --force
aclocal
automake -a

