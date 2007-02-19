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

FILENAME="last-successful-revision.txt"

def run(cmd, args, silent=0):
  f=os.popen("%s %s" % (cmd, args), "r")
  t=f.read()
  f.close()
  if not silent:
    print t
  return t

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
  print "Clean existing stuff"
  run("msdev", "rapidsvn.dsw /MAKE ALL /CLEAN")

  print "Build svncpp"
  run("msdev", "src\svncpp\svncpp.dsp /MAKE ALL")

  print "Build rapidsvn"
  run("msdev", "rapidsvn.dsp /MAKE ALL")
  

def buildInstaller():
  print "Clean installer"
  os.chdir("packages/win32")
  # Remove files
  x=glob.glob("tmp/*")
  for n in x: os.unlink(n)
  x=glob.glob("Output/*")
  for n in x: os.unlink(n)

  print "Build installer"
  run("cmd", "/c FetchFiles.bat", 1)

  innosetup=os.environ["INNOSETUP"]
  run("\"%s\iscc.exe\"", "rapidsvn.iss")

  #Get the name of the package and rename it
  n=glob.glob("Output/RapidSVN*exe")
  if not len(n):
    print "Hm, seems like we have a build error: aborting"
    sys.exit(1)
  e=os.path.splitext(n)
  pkg="%s-%d%s" % (e[0],currentRevision,e[1])
  os.rename(n, pkg)
  print "The new package is: %s" % (pkg)
  os.chdir("../..")

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
    print "No newer revision detected, aborting (last successful=%d, current=%d)" % (lastSuccessfulRevision, currentRevision)
    sys.exit(0)
    
  buildApplication();
  buildInstaller();
  
  #remeber revision
  os.open("FILENAME", "w").write(currentRevision)

  #TODO: upload to server
  print "Done"
