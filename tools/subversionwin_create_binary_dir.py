import sys
import os
import re
from getopt import getopt

USAGE="""{0}: create a subversion binary distribution dir from the source tree
Usage:
  {0} --src=<source dir> --dst=<destination dir>
  <source dir>:       tree containing a compiled subversion distribution
  <destintation dir>: tree will contain all binaries, headers and libraries
                      needed to build rapidsvn from it
  The directories supplied have to be absolute and they have to end with the
  subversion version (e.g. -1.6.12).""".format(sys.argv[0])

rDIRNAME=re.compile(r".*-([0-9]+[.][0-9]+[.][0-9]+)$")

class Error(Exception): pass
class InvalidParameter(Exception): pass

def run(src, dst):
  if not(os.path.isabs(src)) or not(os.path.isabs(dst)):
    raise InvalidParameter("please use absolute paths")

  if src == dst:
    raise InvalidParameter("source and destination are identical")

  m_src=rDIRNAME.match(src)
  m_dst=rDIRNAME.match(dst)

  if not(m_src) or not(m_dst):
    raise InvalidParameter("source or destintation don't contain the version information")

  version_src=m_src.group(1)
  version_dst=m_dst.group(1)
  if version_src != version_dst:
    raise InvalidParameter("source or destination contain different versions")

  # First check the source distribution
  files_to_check = (
    # headers
    "subversion/include/svn_version.h",
    "apr/include/apr_version.h",
    "apr-iconv/include/api_version.h",
    "apr-util/include/apu_version.h",
    # libraries
    "Release/subversion/libsvn_client/libsvn_client-1.lib",
    "Debug/subversion/libsvn_client/libsvn_client-1.lib",
    "apr/Release/libapr-1.lib",
    "apr/Debug/libapr-1.lib",
    "apr-iconv/Release/libapriconv-1.lib",
    "apr-iconv/Debug/libapriconv-1.lib",
    "apr-iconv/Release/iconv/utf-8.so",
    "apr-iconv/Debug/iconv/utf-8.so",
    "apr-util/Release/libaprutil-1.lib",
    "apr-util/Debug/libaprutil-1.lib",
    "neon/libneon.lib",
    "neon/libneonD.lib",
    "db4-win32/lib/libdb44.lib",
    # binaries
    "Release/subversion/svn/svn.exe",
    "Release/subversion/svnadmin/svnadmin.exe",
    "Debug/subversion/svn/svn.exe",
    "Debug/subversion/svnadmin/svnadmin.exe",
    )
  print "Checking source distribution in {src}...".format(src=src),
  for file in files_to_check:
    if not os.path.isfile(os.path.join(src, file)):
      raise Error("can't find {file} in source".format(file=file))
  print "OK"
  print "Preparing destination in {dst}...".format(dst=dst),
  os.makedirs(dst)
  dirs_to_create = (
    "include",
    "Release", "Release/bin", "Release/lib", "Release/iconv",
    "Debug", "Debug/bin", "Debug/lib", "Debug/iconv",
  )
  for dir in dirs_to_create:
    os.mkdir(os.path.join(dst, dir))
  print "OK"
  print "Copying files...",
  files_to_copy = (
    # headers
    ["subversion/include/*.h", "include"],
    ["apr/include/*.h", "include"],
    ["apr-iconv/include/*.h", "include"],
    ["apr-util/include/*.h", "include"],
    # Release libraries
    ["Release/subversion/libsvn_client/*.lib", "Release/lib"],
    ["Release/subversion/libsvn_delta/*.lib", "Release/lib"],
    ["Release/subversion/libsvn_diff/*.lib", "Release/lib"],
    ["Release/subversion/libsvn_fs_base/*.lib", "Release/lib"],
    ["Release/subversion/libsvn_fs/*.lib", "Release/lib"],
    ["Release/subversion/libsvn_fs_fs/*.lib", "Release/lib"],
    ["Release/subversion/libsvn_fs_util/*.lib", "Release/lib"],
    ["Release/subversion/libsvn_ra/*.lib", "Release/lib"],
    ["Release/subversion/libsvn_ra_neon/*.lib", "Release/lib"],
    ["Release/subversion/libsvn_ra_local/*.lib", "Release/lib"],
    ["Release/subversion/libsvn_ra_svn/*.lib", "Release/lib"],
    ["Release/subversion/libsvn_subr/*.lib", "Release/lib"],
    ["Release/subversion/libsvn_wc/*.lib", "Release/lib"],
    ["apr/Release/*.lib", "Release/lib"],
    ["apr-iconv/Release/*.lib", "Release/lib"],
    ["apr-iconv/Release/iconv/*.so", "Release/iconv"],
    ["apr-util/Release/*.lib", "Release/lib"],
    ["db4-win32/lib/libdb44.lib", "Release/lib"],
    ["neon/libneon.lib", "Release/lib"],
    # Debug libraries
    ["Debug/subversion/libsvn_client/*.lib", "Debug/lib"],
    ["Debug/subversion/libsvn_delta/*.lib", "Debug/lib"],
    ["Debug/subversion/libsvn_diff/*.lib", "Debug/lib"],
    ["Debug/subversion/libsvn_fs_base/*.lib", "Debug/lib"],
    ["Debug/subversion/libsvn_fs/*.lib", "Debug/lib"],
    ["Debug/subversion/libsvn_fs_fs/*.lib", "Debug/lib"],
    ["Debug/subversion/libsvn_fs_util/*.lib", "Debug/lib"],
    ["Debug/subversion/libsvn_ra/*.lib", "Debug/lib"],
    ["Debug/subversion/libsvn_ra_neon/*.lib", "Debug/lib"],
    ["Debug/subversion/libsvn_ra_local/*.lib", "Debug/lib"],
    ["Debug/subversion/libsvn_ra_svn/*.lib", "Debug/lib"],
    ["Debug/subversion/libsvn_subr/*.lib", "Debug/lib"],
    ["Debug/subversion/libsvn_wc/*.lib", "Debug/lib"],
    ["apr/Debug/*.lib", "Debug/lib"],
    ["apr-iconv/Debug/*.lib", "Debug/lib"],
    ["apr-iconv/Debug/iconv/*.so", "Debug/iconv"],
    ["apr-util/Debug/*.lib", "Debug/lib"],
    ["db4-win32/lib/libdb44d.lib", "Debug/lib"],
    ["neon/libneond.lib", "Debug/lib"],
  )


if __name__=='__main__':
  try:
    opts, args=getopt(sys.argv[1:], [], ['src=', 'dst='])

    src=None
    dst=None
    for opt, value in opts:
      if opt == '--src':
        src=value
      elif opt == '--dst':
        dst=value

    # First check the validity of the parameters
    if (None==src) or (None==dst):
      raise Error("not enough parameters")

    run(src, dst)

    sys.exit(0)
  except InvalidParameter, e:
    print USAGE
    print "Error: {message}".format(message=str(e))
    sys.exit(1)
  except Error, e:
    print "Error: {message}\nAborted.".format(message=str(e))
    sys.exit(1)





