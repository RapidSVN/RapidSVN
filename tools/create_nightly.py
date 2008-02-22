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
from subprocess import Popen, PIPE

FILENAME="last-successful-revision.txt"

def run(cmd, args, silent=False):
  p=Popen("%s %s" % (cmd, args), stdout=PIPE)
  t=p.communicate()[0]

  if not silent:
    print t
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
  t=run("svn", "info .", 1)
  m=re.search("Revision: ([0-9]+)",t)
  return m.group(1)
  
def buildApplication():
  print "Rebuild rapidsvn"
  run("msdev", "rapidsvn.dsw /MAKE ALL /REBUILD")
  
  
def buildMessages():
  # First we have to check which translations we have
  l=glob.glob('src/locale/[a-z]*')
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
      print "Compiling message catalog %s into %s" % (po, mo)
      msgfmt.make(po,mo)
  

def buildInstaller():
  print "Clean installer"
  os.chdir("packages/win32")
  # Remove files
  x=glob.glob("tmp/*")
  for n in x: os.unlink(n)
  x=glob.glob("Output/*")
  for n in x: os.unlink(n)

  print "Build installer"
  run("cmd.exe", "/c FetchFiles.bat")
  innosetup=getEnviron("INNOSETUP")
  run("\"%s\iscc.exe\"" % innosetup, "rapidsvn.iss")
  
  #Get the name of the package and rename it
  n=glob.glob("Output/RapidSVN*exe")
  if not len(n):
    print "Hm, seems like we have a build error: aborting"
    sys.exit(1)
  old=n[0]
  e=os.path.splitext(old)
  print e
  pkg="%s-%s%s" % (e[0],currentRevision,e[1])
  os.rename(old, pkg)
  print "The new package is: %s" % (pkg)
  os.chdir("../..")
  return "packages/win32/%s" % (pkg)
  
def uploadInstaller(pkg):
  putty=getEnviron("PUTTY")
  url="rapidsvn_ftp@rapidsvn.org:/httpdocs/download/nightly/win32"
  run("\"%s/pscp.exe\"" % (putty),  "%s %s" % (pkg, url))

if __name__ == '__main__':
  # Check whether we are in the project dir
  if not os.path.exists("HACKING.txt"):
    print "Wrong directory to start this script!"
    sys.exit(1)
    
  print "Update working copy"
  run("svn", "-q up")

  # Now decide whether or not we have to create a nightly build
  lastSuccessfulRevision=readLastSuccessfulRevision()
  currentRevision=readCurrentRevision()

  if "" == lastSuccessfulRevision:
    print "No successful previous build detected"
  elif currentRevision <= lastSuccessfulRevision:
    print "No newer revision detected, aborting (last successful=%s, current=%s)" % (lastSuccessfulRevision, currentRevision)
    sys.exit(0)
    
  buildApplication()
  buildMessages()
  pkg=buildInstaller()
  uploadInstaller(pkg)
  
  #remember revision
  open(FILENAME, "w").write(currentRevision)

  print "Done"
