#!/bin/bash 
# Script to generate the RapidSVN.app bundle and
# a disk image ready for distribution
#

# Remove any old stuff. We wanna create a FRESH bundle
echo "Remove any old stuff"
DISKIMAGE=RapidSVN-0.12.1
DISKIMAGE_FILE=$DISKIMAGE.dmg
test -e RapidSVN.app && rm -rf RapidSVN.app
rm *.dmg

echo "Create the bundle"
BUNDLEDIR=RapidSVN.app/Contents
mkdir -p $BUNDLEDIR/MacOS
mkdir -p $BUNDLEDIR/Resources

cp ../../src/rapidsvn $BUNDLEDIR/MacOS/
strip $BUNDLEDIR/MacOS/rapidsvn
cp rapidsvn.icns $BUNDLEDIR/Resources/
echo -n 'APPL????' > $BUNDLEDIR/PkgInfo

mkdir -p $BUNDLEDIR/Resources/locale

pushd ../../src/locale
for DIR in ?? ??_?? ; do
  DSTDIR=../../packages/osx/$BUNDLEDIR/Resources/locale/$DIR
  SRC=$DIR/rapidsvn.po
  DST=$DSTDIR/rapidsvn.po
 
  mkdir -p $DSTDIR

  echo "Copy the message catalog $DST"
  cp $SRC $DST
  
  echo "Compile the message catalog $DST"
  python ../../tools/msgfmt.py $DST
done
popd

echo -n \
"<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist SYSTEM "file://localhost/System/Library/DTDs/PropertyList.dtd">
<plist version="0.9">
<dict>
	<key>CFBundleInfoDictionaryVersion</key>
	<string>6.0</string>
	<key>CFBundleIdentifier</key>
	<string>org.svn.rapidsvn</string>
	<key>CFBundleDevelopmentRegion</key>
	<string>English</string>
	<key>CFBundleExecutable</key>
	<string>rapidsvn</string>
	<key>CFBundleIconFile</key>
	<string>rapidsvn.icns</string>
	<key>CFBundleName</key>
	<string>RapidSVN</string>
	<key>CFBundlePackageType</key>
	<string>APPL</string>
	<key>CFBundleSignature</key>
	<string>????</string>
	<key>CFBundleVersion</key><string>0.12.1</string>
	<key>CFBundleShortVersionString</key><string>0.12.1</string>
	<key>CFBundleGetInfoString</key><string>RapidSVN version 0.12.1, (c) 2002-2012 RapidSVN</string>
	<key>CFBundleLongVersionString</key><string>0.12.1, (c) 2002-2012 RapidSVN</string>
	<key>NSHumanReadableCopyright</key>
	<string>Copyright 2002-2007 RapidSVN</string>
	<key>LSRequiresCarbon</key>
        <true/>
	<key>CSResourcesFileMapped</key>
	<true/>
</dict>
</plist>
" > $BUNDLEDIR/Info.plist

# Sometimes hdiutil will produce an error
# we dont have the cause for this yet.
# First try: we wait a while for things to settle
sleep 5

echo "Now create the disk image $DISKIMAGE from the bundle"
hdiutil create -srcfolder RapidSVN.app $DISKIMAGE || echo "hdiutil error"

test -e $DISKIMAGE_FILE && echo Success!!

