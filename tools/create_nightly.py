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
# Execute this command in the RapidSVN project directory
# The following environment variable have to be set:
# SUBVERSION - Subversion source directory
# INNOSETUP  - Inno Setup program directory
# LIBINTL    - Win32 Lib Intl directory
#
import os
import sys
import re
import glob
import msgfmt
import platform
from subprocess import Popen, PIPE
from getopt import getopt

FILENAME="last-successful-revision.txt"

def run(cmd, args=[], silent=False):
  cmd_args=[cmd]
  cmd_args.extend(args)
  #print cmd_args
  p=Popen(cmd_args, stdout=PIPE)
  t=p.communicate()[0]

  if not silent:
    print(t)
  return t

def getEnviron(key):
  try:
    return os.environ[key]
  except:
    raise Exception("Environment variable '%d' not set!" % (key))

def readLastSuccessfulRevision():
  try:
    return open(FILENAME, "r").read()
  except:
    return ""

def readCurrentRevision():
  t=run("svn", ["info",  "."], 1)
  m=re.search("Revision: ([0-9]+)",t)
  return m.group(1)

def readLatestChangeRevision():
  # list all files and revision numbers
  t=run("svn", ["status", "-v", "."], 1)
  # this will output something like
  # "    1234   3456    users src/rapidsvn.cpp"
  # we are interested in the second number, the changed revision
  # and the last part so we can skip "www"
  r=re.compile(r"^[ ]+[0-9]+[ ]+([0-9]+)[ ]+[^ ]+[ ]+(.*)$")
  latest_rev=0
  for line in t.split("\n"):
    m=r.match(line)
    if m:
      rev=int(m.group(1))
      dir=m.group(2)
      if dir.startswith("www"):
        continue # skip the www dir

      if rev > latest_rev:
        latest_rev = rev

  # we needed the int only for comparing, now we need a str
  return str(latest_rev)


def patchVersionHeader(revision):
  # first undo any previous patch
  VERSION_HEADER="librapidsvn/include/version.hpp"
  REVISION_DEFINE="#define RAPIDSVN_VER_REVISION "
  run("svn", ["revert", VERSION_HEADER], 1)

  # now read the header
  original=open(VERSION_HEADER, "r").read()
  m=re.search(REVISION_DEFINE + "[0-9]+", original)
  if not m:
    raise Exception("%s' nicht gefunden in '%s'" % (REVISION_DEFINE, VERSION_HEADER))

  # patch the revision
  patched=original[:m.start()] + REVISION_DEFINE + str(revision) + original[m.end():]
  open(VERSION_HEADER, "w").write(patched)


def buildApplicationVc2005():
  out=open('vcbuild.log', 'w')
  print("Rebuild rapidsvn (using Visual C++ 2005 vcbuild)")
  out.write(run('vcbuild', ['/useenv', '/rebuild', 'build\\vc2005\\rapidsvn.sln', 'Release|Win32'], silent=True))
  out.write(run('vcbuild', ['/useenv', '/rebuild', 'build\\vc2005\\rapidsvn.sln', 'Unicode Release|Win32'], silent=True))


def buildMessages():
  # First we have to check which translations we have
  l=glob.glob('librapidsvn/src/locale/[a-z]*')
  dirs=[]
  for f in l:
    if os.path.isdir(f):
      dirs.append(f)
  for f in dirs:
    po=f+"/rapidsvn.po"
    mo=f+"/rapidsvn.mo"
    if os.path.isfile(po):
      # Clear dictionary that was used by msgfmt in previous conversion
      msgfmt.MESSAGES = {}
      print("Compiling message catalog %s into %s" % (po, mo))
      msgfmt.make(po,mo)


def buildInstaller(compiler, suffix):
  print("Clean installer")
  os.chdir("packages/win32")
  # Remove files
  x=glob.glob("tmp/*")
  for n in x: os.unlink(n)
  x=glob.glob("Output/*")
  for n in x: os.unlink(n)

  print("Fetching files for installer")
  out=open('innosetup.log', 'w')
  if compiler=='vc2005':
    out.write(run('cmd.exe', ['/c', 'FetchFiles_vs2005.bat'], silent=True))
  innosetup="%s\iscc.exe" % getEnviron("INNOSETUP")
  print("Build installer (using %s)" %innosetup)
  out.write(run(innosetup, ['rapidsvn.iss'], silent=True))

  #Get the name of the package and rename it
  n=glob.glob("Output/RapidSVN*exe")
  if not len(n):
    print("Hm, seems like we have a build error: aborting")
    sys.exit(1)
  old=n[0]
  e=os.path.splitext(old)
  print(e)
  pkg="%s-%s%s%s" % (e[0],currentRevision,suffix,e[1])
  os.rename(old, pkg)
  print("The new package is: %s" % (pkg))
  os.chdir("../..")
  return "packages/win32/%s" % (pkg)

def makeApplication():
  print('Rebuild rapidsvn (using make)')
  if not os.path.isfile('config.status'):
    print('You have to run "configure" at least once!')
    sys.exit(1)
  run('./autogen.sh', silent=True)
  run('./config.status', silent=True)
  run('make', ['clean'], silent=True)
  run('make', ['all'], silent=True)

  n=glob.glob('src/rapidsvn')
  if not len(n):
    print("Hm, seems like we have a build error: aborting")
    sys.exit(1)


def buildMacDiskImage(suffix):
  print('Build Mac Disk Image')
  os.chdir('packages/osx')
  run('./make_osx_bundle.sh', silent=True)

  #Get the name of the package and rename it
  n=glob.glob("RapidSVN*dmg")
  if not len(n):
    print("Hm, seems like we have a build error: aborting")
    sys.exit(1)
  old=n[0]
  e=os.path.splitext(old)
  print(e)
  pkg="%s-%s%s%s" % (e[0],currentRevision,suffix,e[1])
  os.rename(old, pkg)
  print("The new package is: %s" % (pkg))
  os.chdir("../..")
  return "packages/osx/%s" % (pkg)


def uploadInstaller(pkg):
  dir=''
  scp='scp'
  if platform.system() == 'Windows':
    putty=getEnviron("PUTTY")
    scp="%s/pscp.exe" % (putty)
    dir='win32'
  elif platform.system() == 'Darwin':
    scp='scp'
    dir='osx'

  url="rapidsvn@rapidsvn.org:/srv/www/vhosts/rapidsvn.org/httpdocs/download/nightly/%s" % (dir)
  run(scp,  [pkg, url])


def usage():
  print("Usage: create_nightly.py [--compiler={vc2005}] [--suffix=<text>] [--force] [--skip-compile] [--skip-installer] [--skip-upload]")
  print()


if __name__ == '__main__':
  # Check if we need to chdir one up
  if os.path.exists("create_nightly.py"):
    os.chdir("..")
  # Check whether we are in the project dir
  if not os.path.exists("HACKING.txt"):
    print("Wrong directory to start this script!")
    sys.exit(1)

  # Parse the options
  compiler=None
  suffix=""
  skipCompile=False
  skipInstaller=False
  skipUpload=False
  force=False
  try:
    opts, args=getopt(sys.argv[1:], [], ['compiler=', 'suffix=', 'skip-compile', 'skip-installer', 'skip-upload', 'force'])

    if len(args) > 1:
      raise Exception()

    for opt, value in opts:
      if opt == '--compiler':
        if not value in ['vc2005']:
          raise Exception()
        else:
          compiler = value
      elif opt == '--suffix':
        suffix = value
      elif opt == '--skip-compile':
        skipCompile=True
      elif opt == '--skip-installer':
        skipInstaller=True
      elif opt == '--skip-upload':
        skipUpload=True
      elif opt == '--force':
        force=True

  except:
    usage()
    sys.exit(1)

  system=platform.system()
  if system=='Windows':
    print('Nightly build for Windows')
  elif system=='Darwin':
    print('Nightly build for Mac OS/X')
  else:
    print("Nighlty build not supported for %s" % system)
    sys.exit(1)

  print("Update working copy")
  run('svn', ['-q', 'up'])

  # Now decide whether or not we have to create a nightly build
  lastSuccessfulRevision=readLastSuccessfulRevision()
  currentRevision=readLatestChangeRevision()
  patchVersionHeader(currentRevision)

  if "" == lastSuccessfulRevision:
    print("No successful previous build detected")
  elif force:
    print("Forcing the build")
  elif currentRevision <= lastSuccessfulRevision:
    print("No newer revision detected, aborting (last successful=%s, current=%s)" % (lastSuccessfulRevision, currentRevision))
    sys.exit(0)

  buildMessages()

  pkg = ''
  if system == 'Darwin':
    if not skipCompile: makeApplication()
    if not skipInstaller: pkg=buildMacDiskImage(suffix)
  elif system == 'Windows':
    if not skipCompile:
      if compiler in [None, 'vc2005']:
        buildApplicationVc2005()
    if not skipInstaller: pkg=buildInstaller(compiler, suffix)

  if not skipUpload: uploadInstaller(pkg)

  #remember revision
  open(FILENAME, "w").write(currentRevision)

  print("Done")
