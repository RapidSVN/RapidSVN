#!/usr/bin/env python
#
# ====================================================================
# Copyright (c) 2002-2018 The RapidSVN Group.  All rights reserved.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program (in the file GPL.txt).
# If not, see <http://www.gnu.org/licenses/>.
#
# This software consists of voluntary contributions made by many
# individuals.  For exact contribution history, see the revision
# history and logs, available at http://rapidsvn.tigris.org/.
# ====================================================================
#
# Purpose of this script:
#   Check if all version-info stored in several
#   places matches the current version and patch these files
#   if necessary
#
# Usage: check-version.py (while in the top build-directory)
#
#
# Version information is stored in the file "version.xml" in the
# top build-directory.
#

from xml.dom.minidom import parse
import re
from datetime import date

# The following files are being checked/patched:
VERSION_FILE = "version.xml"
CONFIGURE_IN_FILE = "configure.in"
VERSION_HPP_FILE = "librapidsvn/include/version.hpp"
RAPIDSVN_ISS_FILE = "packages/win32/rapidsvn.iss"
DEBIAN_FILES_FILE = "packages/debian/files"
MAKE_OSX_BUNDLE_FILE = "packages/osx/make_osx_bundle.sh"
SVNCPP_VERSION_HPP_FILE = "include/svncpp/version.hpp"

class Version:
    def __init__(self, filename):
        self.major = ""
        self.minor = ""
        self.micro = ""
        self.patch = ""
        self.str = ""
        self.milestone = ""
        self.number = 0x000000

        self.readFromFile(filename)
        self.year = date.today().year

    def readFromFile(self, filename):
        dom = parse(filename)
        version = dom.getElementsByTagName("version")[0]
        self.major = version.attributes["major"].value
        self.minor = version.attributes["minor"].value
        self.micro = version.attributes["micro"].value
        self.patch = version.attributes["patch"].value
        self.str = "%s.%s.%s" % (self.major, self.minor, self.micro)
        self.str_nomicro = "%s.%s" % (self.major, self.minor)
        self.str_nopatch=self.str
        if len(self.patch) > 0:
          self.str += "-" + self.patch;
        self.milestone = version.attributes["milestone"].value
        self.number = (int('0' + self.major, 10) << 24) + \
            (int(self.minor, 10) << 16) + \
            (int('0' + self.micro, 10) << 8) + \
            int('0' + self.patch, 10)

def replace(fname, regex, newval, what):
    s_old = open(fname, "r").read()
    rest = s_old
    r = re.compile(regex)
    m = r.search(rest)
    found = False
    s = ""
    while m:
        found = True
        s += rest[:m.start()] + newval
        rest = rest[m.end():]
        m = r.search(rest)

    s += rest

    if not found:
        print "%s: %s not found" % (fname, what)
    elif s == s_old:
        print "%s: %s already replaced" % (fname, what)
    else:
        open(fname, "w").write(s)
        print "%s: %s changed" % (fname, what)


class VersionChecker:
    def __init__(self):
        self.version = Version(VERSION_FILE)

    def checkConfigureIn(self):
        newval = "AM_INIT_AUTOMAKE(rapidsvn, %s)" % (self.version.str)
        what = "Version information"
        replace(CONFIGURE_IN_FILE,
                "AM_INIT_AUTOMAKE[^\n]*",
                newval, what)

    def checkVersionHpp(self):
        fname = VERSION_HPP_FILE
        replace(VERSION_HPP_FILE, \
                "_VER_MAJOR[^\n]*",
                "_VER_MAJOR %s" % (self.version.major),
                "RAPIDSVN_VER_MAJOR")
        replace(VERSION_HPP_FILE,
                "_VER_MINOR[^\n]*",
                "_VER_MINOR %s" % (self.version.minor),
                "RAPIDSVN_VER_MINOR")
        replace(VERSION_HPP_FILE,
                "_VER_MICRO[^\n]*",
                "_VER_MICRO %s" % (self.version.micro),
                "RAPIDSVN_VER_MICRO")
        if len(self.version.patch) > 0:
          patch = self.version.patch
        else:
          patch = 0
        replace(VERSION_HPP_FILE,
                "_VER_PATCH[^\n]*",
                "_VER_PATCH %s" % (patch),
                "RAPIDSVN_VER_PATCH")
        replace(VERSION_HPP_FILE,
                "_VER_STR[^\n]*",
                "_VER_STR \"%s\"" % (self.version.str),
                "RAPIDSVN_VER_STR")

        replace(VERSION_HPP_FILE,
                "_VER_MILESTONE[^\n]*",
                "_VER_MILESTONE \"%s\"" % (self.version.milestone),
                "RAPIDSVN_VER_MILESTONE")

    def checkRapidsvnIss(self):
        fname = RAPIDSVN_ISS_FILE
        replace(fname, "AppName=[^\n]*",
                 "AppName=RapidSVN-%s" % (self.version.str_nopatch),
                 "AppName")
        replace(fname, "AppVerName=[^\n]*",
                 "AppVerName=RapidSVN-%s" % (self.version.str),
                 "AppVerName")
        replace(fname, "OutputBaseFilename=[^\n]*", \
                "OutputBaseFilename=RapidSVN-%s" % (self.version.str),
                "OutputBaseFilename")
        replace(fname, "DefaultDirName=[^\n]*", \
                "DefaultDirName={pf}\RapidSVN-%s" % (self.version.str_nomicro),
                "DefaultDirName")
        replace(fname, "DefaultGroupName=[^\n]*", \
                "DefaultGroupName=RapidSVN-%s" % (self.version.str_nomicro),
                "DefaultGroupName")
        replace(fname, "userdesktop}\\\\RapidSVN[^\"]*\"", \
                "userdesktop}\RapidSVN-%s\"" % (self.version.str_nomicro),
                "[Icons]\\{userdesktop}")

    def checkDebianFiles(self):
        fname = DEBIAN_FILES_FILE
        replace(fname, "rapidsvn_[^\n]*", \
                "rapidsvn_%s-1_i386.deb x11 optional" % (self.version.str),
                "Debian package name")

    def checkMakeOsXBundle(self):
        fname = MAKE_OSX_BUNDLE_FILE
        replace(fname, "<key>CFBundleVersion.*", \
                "<key>CFBundleVersion</key><string>%s</string>" % \
                (self.version.str), "CFBundleVersion")
        replace(fname, "<key>CFBundleShortVersionString</key>.*", \
                "<key>CFBundleShortVersionString</key><string>%s</string>" % \
                (self.version.str), "CFBundleShortVersionString")
        replace(fname, "<key>CFBundleGetInfoString</key>.*", \
                "<key>CFBundleGetInfoString</key>" \
                "<string>RapidSVN version %s, " \
                "(c) 2002-%d RapidSVN</string>" % (self.version.str, \
                self.version.year), \
                "CFBundleGetInfoString")
        replace(fname, "<key>CFBundleLongVersionString</key>.*", \
                "<key>CFBundleLongVersionString</key>" \
                "<string>%s, (c) 2002-%d RapidSVN</string>" % \
                (self.version.str, self.version.year), \
                "CFBundleLongVersionString")
        replace(fname, "DISKIMAGE=.*", \
                "DISKIMAGE=RapidSVN-%s" % (self.version.str), \
                "DISKIMAGE")

    def checkSvncppVersionHpp(self):
        replace(SVNCPP_VERSION_HPP_FILE, 'SVNCPP_VERSION .*', \
            "SVNCPP_VERSION 0x%08x" % self.version.number, \
            'SVNCPP_VERSION')

    def run(self):
        print "Version information:"
        print "  Major: ", self.version.major
        print "  Minor: ", self.version.minor
        print "  Micro: ", self.version.micro
        print "  Patch: ", self.version.patch
        print "  Str:   ", self.version.str
        print "  Number: 0x%08x" % self.version.number
        print "  Milestone: ", self.version.milestone

        self.checkConfigureIn()
        self.checkVersionHpp()
        self.checkRapidsvnIss()
        self.checkDebianFiles()
        self.checkMakeOsXBundle()
        self.checkSvncppVersionHpp()

if __name__ == "__main__":
    vc = VersionChecker()
    vc.run()

