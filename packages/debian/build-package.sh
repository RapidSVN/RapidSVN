#!/bin/sh
# ugly hack to get the package built
# run this from toplevel

if [ ! -e packages/debian ]
then
  echo "run from top dir"
  exit 1
fi
rm -rf debian
cp -R packages/debian .
dpkg-buildpackage -b -us -uc -rfakeroot
