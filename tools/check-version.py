#!/usr/bin/env python
#
# ====================================================================
# Copyright (c) 2002-2004 The RapidSvn Group.  All rights reserved.
#
# This software is licensed as described in the file LICENSE.txt,
# which you should have received as part of this distribution.
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
# Usage: check-version.py (while in tools directory)
#
#
# Version information is stored in the file "version.xml" in the
# top build-directory.
#
# The following files are being checked/patched:
#   version.xml           
#   configure.in
#   packages/win32/rapidsvn.iss
#   packages/debian
#   src/version.hpp
#

#import xmllib
from xml.dom.minidom import parse
import re

VERSION_FILE = "../version.xml"
CONFIGURE_IN_FILE = "../configure.in"
VERSION_HPP_FILE = "../src/version.hpp"
RAPIDSVN_ISS_FILE = "../packages/win32/rapidsvn.iss"
DEBIAN_FILES_FILE = "../packages/debian/files"
RAPIDSVN_RC_FILE = "../src/rapidsvn.rc"

class Version:
    def __init__(self, filename):
        self.major = ""
        self.minor = ""
        self.micro = ""
        self.milestone = ""

        self.readFromFile(filename)

    def readFromFile(self, filename):
        dom = parse(filename)
        version = dom.getElementsByTagName("version")[0]
        self.major = version.attributes["major"].value
        self.minor = version.attributes["minor"].value
        self.micro = version.attributes["micro"].value
        self.milestone = "%s.%s.%s" % (self.major, self.minor, self.micro)

def replace(fname, regex, newval, what):
    s = open(fname, "r").read()
    s_old = s
    r = re.compile(regex)
    m = r.search(s)
    if not m:
        print "%s: %s not found" % (fname, what)
    else:
        s = s[:m.start()] + newval + s[m.end():]
        if (s != s_old):
            open(fname, "w").write(s)
            print "%s: %s changed" % (fname, what)

class VersionChecker:
    def __init__(self):
        self.version = Version(VERSION_FILE)

    def checkConfigureIn(self):
        newval = "AM_INIT_AUTOMAKE(rapidsvn, %s)" % (self.version.milestone)
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
        replace(VERSION_HPP_FILE, 
                "_VER_MILESTONE[^\n]*", 
                "_VER_MILESTONE \"%s\"" % (self.version.milestone), 
                "RAPIDSVN_VER_MILESTONE")

    def checkRapidsvnIss(self):
        fname = RAPIDSVN_ISS_FILE
        replace(fname, "AppVerName=[^\n]*", 
                 "AppVerName=RapidSVN %s" % (self.version.milestone), 
                 "AppVerName")
        replace(fname, "OutputBaseFilename=[^\n]*", \
                "OutputBaseFilename=RapidSVN-%s" % (self.version.milestone),
                "OutputBaseFilename")
                
    def checkRapidsvnRc(self):
        fname = RAPIDSVN_RC_FILE
        replace(fname, "#define VERSION_MAJOR.*", 
                "#define VERSION_MAJOR %s" % (self.version.major),
                "VERSION_MAJOR")
        replace(fname, "#define VERSION_MINOR.*",
                "#define VERSION_MINOR %s" % (self.version.minor),
                "VERSION_MINOR")
        replace(fname, "#define VERSION_MICRO.*",
                "#define VERSION_MICRO %s" % (self.version.micro),
                "VERSION_MICRO")
        replace(fname, "#define VERSION_STRING.*",
                "#define VERSION_STRING \"%s\\0\"" % (self.version.milestone),
                "VERSION_STRING")

    def checkDebianFiles(self):
        fname = DEBIAN_FILES_FILE
        replace(fname, "rapidsvn_[^\n]*", \
                "rapidsvn_%s-1_i386.deb x11 optional" % (self.version.milestone),
                "Debian package name")

    def run(self):
        print "Version information:"
        print "  Major: ", self.version.major
        print "  Minor: ", self.version.minor
        print "  Micro: ", self.version.micro
        print "  Milestone: ", self.version.milestone

        self.checkConfigureIn()
        self.checkVersionHpp()
        self.checkRapidsvnIss()
        self.checkDebianFiles()
        self.checkRapidsvnRc()

if __name__ == "__main__":
    vc = VersionChecker()
    vc.run()
    
