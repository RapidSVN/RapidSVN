RapidSVN

This is a cross-platform GUI front-end for the Subversion source control system.  It is written in C++ using the wxWindows toolkit and is distributed under the GNU General Public License (GPL).  Originally this program was written by Paul Marculescu <paul@p16.pub.ro>.

Currently it compiles under Windows and it does so with Linux with a few issues that are currently being worked out. 

Building
* Download the source code for the program.
* Download the following libraries:
    wxWindows GUI toolkit - http://www.wxwindows.org/
    Apache Portable Runtime (APR) - http://apr.apache.org/
    Berkeley DB - http://www.sleepycat.com/
    Neon - http://www.webdav.org/neon/
    SVN libraries - http://subversion.tigris.org/project_source.html
* Download the latest version of subversion with all of the client libraries included.  This can now currently be done by downloading subversion 

Download all the files just as the directions of Subversion INSTALL file so you get this directory structure:

/subversion
   /apr
   /apr-iconv
   /apr-util
   /db4-win32
   /neon
/rapidsvn

* Two environmental variables must be set first: 
SUBVERSION - SVN libraries root directory
WX - wxWindows libraries directory
set WX=C:\Program Files\wx2
set SUBVERSION=D:\dev\subversion
