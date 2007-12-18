RapidSVN

INTRODUCTION

RapidSVN is a cross-platform GUI front-end for the Subversion revision system 
(http://subversion.tigris.org/) written in C++ using the wxWidgets GUI 
framework. It is distributed under the GNU General Public License.  
See LICENSE.txt for more details. 

Currently it compiles under Windows and it does so with Linux but there are a 
few runtime issues that are currently being worked out.  Eventually this 
should run on the Mac and several other flavors of *NIX.

WHY WXWIDGETS?

Many of you are probably curious why we would choose wxWidgets as the platform 
in which to build this project.  Since the beginning the goal of Subversion 
was to build a client that would improve source control for operating system 
platform.  By using the Apache Portable Runtime and a load of #ifdef's this 
goal has been effectively accomplished.  With RapidSVN we would like to 
continue this theme.  

In order to get people to transfer over to product to make it easier the 
program should conform to whatever platform they are using.  wxWidgets is the 
perfect platform for this.  It uses GDI on Windows so the Windows guys never
even think for a second that they are using a non-Windows tool and GTK for 
all the Unix guys.  There is even an effort to use QT as well.  This makes 
the best of both worlds without messing with a person's personal 
environment.  

For developers wxWidgets is great. It is easy to learn and quite powerful for 
any platform.  It made the choice easy.

BUILDING AND INSTALLING

* The RapidSVN source code is stored in a Subversion repository (of course!) 
  so in order to get it code you have to download a svn client from the 
  Subversion project page:
  http://subversion.tigris.org/servlets/ProjectDocumentList

* Download the source code for RapidSVN.  You can do this by running the
  following svn command:
  % svn co http://rapidsvn.tigris.org/svn/rapidsvn/trunk rapidsvn

* Download the wxWidgets 2.3.3 (or >= 2.4.0) libraries: http://www.wxWidgets.org/

  If you are building wxWidgets on linux, you will need to specify 
  the --disable-no-exceptions parameter to configure (see 
  http://www.wxwidgets.org/docs/faqgen.htm#exceptions). For development
  you will probably also want --enable-debug.

* Download the latest Subversion source tree.  Here are the directions: 
  http://subversion.tigris.org/project_packages.html

* Once you have Subversion the download all the files just as the directions in 
  the Subversion INSTALL file so you have the directory structure below. Note: 
  the db4-win32 directory name is for MS VC++.

  /subversion
     /apr (apache.org CVS)
     /apr-iconv (apache.org CVS)
     /apr-util (apache.org CVS)
     /db4-win32 (http://www.sleepycat.com/)
     /neon (from http://www.webdav.org/neon/)
  /rapidsvn

* If you are running Windows and you have dependency problems like a missing 
  shfolder.lib then your system very likely does not have the necessary files.
  In this case you can get them by downloading the MS Platform SDK from 
  http://www.microsoft.com/msdownload/platformsdk/sdkupdate/ by
  choosing Core SDK.  Watch out though, because the download is
  enormous, around 400 MB.  It would be best if you just got this from
  an MSDN CD if possible.

* If you have the cvs client installed on your machine then you can download the
  apr libraries with the following commands:
  % cvs -d :pserver:anoncvs@cvs.apache.org:/home/cvspublic co apr
  % cvs -d :pserver:anoncvs@cvs.apache.org:/home/cvspublic co apr-util
  % cvs -d :pserver:anoncvs@cvs.apache.org:/home/cvspublic co apr-iconv
  
  Otherwise, you can get the most recent versions here:
  http://apr.apache.org/download.cgi

* Two environmental variables must be set as well: 
  SUBVERSION - SVN libraries root directory
  WX - wxWidgets libraries directory

  On Windows you can add the following to autoexec.bat or through the 
  Environmental Variables editor.
  set WX=C:\Program Files\wx2
  set SUBVERSION=D:\dev\subversion


BUILDING ON LINUX/UNIX

* Download all of the Subversion code and build it. Note: it is much easier if
  you download the RPMs provided on the Subversion site:
  http://subversion.tigris.org/servlets/ProjectDocumentList

* Download the source code for RapidSVN.  You can do this by running the
  following svn command:
  % svn co http://rapidsvn.tigris.org/svn/rapidsvn/trunk rapidsvn

* Go to the rapidsvn directory and run:
  % ./autogen.sh
  % ./configure

* The configure script might complain about apps/paths it didnt find
  % ./configure --help 
  will return a list of options you might want to set-
  
* Then go to the src/ directory and run 'make'

* If it is built and you get an error like 'could not find something.so' then
  you probably need to add the Apache lib directory into /etc/ld.so.conf and
  then run 'ldconfig'. This is especially true for newer versions of APR.
  If you dont want to touch ld.so.conf you can add the path to the environment
  variable "LD_LIBRARY_PATH".
  Example: export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/apr/lib

* If you get strange segmentation faults after updating or after change
  some of the code: try to rebuild rapidsvn (make clean; make all).

