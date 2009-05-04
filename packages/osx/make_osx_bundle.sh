#!/bin/bash 
# Script to generate the RapidSVN.app bundle and
# a disk image ready for distribution
#

# Remove any old stuff. We wanna create a FRESH bundle
DISKIMAGE=RapidSVN-0.9.9
DISKIMAGE_FILE=$DISKIMAGE.dmg
test -e RapidSVN.app && rm -rf RapidSVN.app
rm *.dmg

# Create the bundle
BUNDLEDIR=RapidSVN.app/Contents
mkdir -p $BUNDLEDIR/MacOS
mkdir -p $BUNDLEDIR/Resources

cp ../../src/rapidsvn $BUNDLEDIR/MacOS/
strip $BUNDLEDIR/MacOS/rapidsvn
cp rapidsvn.icns $BUNDLEDIR/Resources/
echo -n 'APPL????' > $BUNDLEDIR/PkgInfo

mkdir -p $BUNDLEDIR/Resources/locale

pushd ../../src/locale
for DIR in ??; do
  mkdir -p ../../packages/osx/$BUNDLEDIR/Resources/locale/$DIR
  cp $DIR/rapidsvn.po ../../packages/osx/$BUNDLEDIR/Resources/locale/$DIR
  cp $DIR/rapidsvn.mo ../../packages/osx/$BUNDLEDIR/Resources/locale/$DIR
done
for DIR in ??_??; do
  mkdir -p ../../packages/osx/$BUNDLEDIR/Resources/locale/$DIR
  cp $DIR/rapidsvn.po ../../packages/osx/$BUNDLEDIR/Resources/locale/$DIR
  cp $DIR/rapidsvn.mo ../../packages/osx/$BUNDLEDIR/Resources/locale/$DIR
done
popd

cp Info.plist $BUNDLEDIR/Info.plist

# Now create the disk image from the bundle
hdiutil create -quiet -srcfolder RapidSVN.app $DISKIMAGE

test -e $DISKIMAGE_FILE && echo Success!!

