#!/bin/bash 
# Script to generate the RapidSVN.app bundle and
# a disk image ready for distribution
#

# Remove any old stuff. We wanna create a FRESH bundle
DISKIMAGE=RapidSVN-0.9.6
DISKIMAGE_FILE=$DISKIMAGE.dmg
test -e RapidSVN.app && rm -rf RapidSVN.app
test -e $DISKIMAGE_FILE && rm -f $DISKIMAGE_FILE

# Create the bundle
BUNDLEDIR=RapidSVN.app/Contents
mkdir -p $BUNDLEDIR/MacOS
mkdir -p $BUNDLEDIR/Resources
cp ../../src/rapidsvn $BUNDLEDIR/MacOS/
strip $BUNDLEDIR/MacOS/rapidsvn
cp rapidsvn.icns $BUNDLEDIR/Resources/
echo -n 'APPL????' > $BUNDLEDIR/PkgInfo

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
	<key>CFBundleVersion</key><string>0.9.6</string>
	<key>CFBundleShortVersionString</key><string>0.9.6</string>
	<key>CFBundleGetInfoString</key><string>RapidSVN version 0.9.6, (c) 2002-2008 RapidSVN</string>
	<key>CFBundleLongVersionString</key><string>0.9.6, (c) 2002-2008 RapidSVN</string>
	<key>NSHumanReadableCopyright</key>
	<string>Copyright 2002-2007 RapidSVN</string>
	<key>LSRequiresCarbon</key>
        <true/>
	<key>CSResourcesFileMapped</key>
	<true/>
</dict>
</plist>
" > $BUNDLEDIR/Info.plist

# Now create the disk image from the bundle
hdiutil create -quiet -srcfolder RapidSVN.app $DISKIMAGE

test -e $DISKIMAGE_FILE && echo Success!!

