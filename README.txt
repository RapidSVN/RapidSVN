RapidSVN

INTRODUCTION

RapidSVN is a cross-platform GUI front-end for the Subversion revision system 
(http://subversion.tigris.org/) written in C++ using the wxWindows GUI 
framework. It is distributed under the Apache Software License, Version 1.1.  
See LICENSE.txt for more details. 

Currently it compiles under Windows and it does so with Linux but there are a 
few runtime issues that are currently being worked out.  Eventually this 
should run on the Mac and several other flavors of *NIX.

BUILDING AND INSTALLING

* The RapidSVN source code is stored in a Subversion repository (of course!) 
  so in order to get it code you have to download a svn client from the 
  Subversion project page:
  http://subversion.tigris.org/servlets/ProjectDocumentList

* Download the source code for RapidSVN.  You can do this by running the
  following svn command:
  % svn co http://svn.collab.net/repos/svn/clients/rapidsvn/trunk rapidsvn  

* Download the wxWindows 2.3.2 libraries: http://www.wxwindows.org/

* Download the latest Subversion source tree.  Here are the directions: 
  http://subversion.tigris.org/project_source.html

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

* If you are running Windows then download the MS Platform SDK from 
  http://www.microsoft.com/msdownload/platformsdk/sdkupdate/ by
  choosing Core SDK.  Watch out though, because the download is
  enormous, around 400 MB.  It would be best if you just got this from
  an MSDN CD if possible.

  If you have the cvs client installed on your machine then you can download the
  apr libraries with the following commands:
  % cvs -d :pserver:anoncvs@cvs.apache.org:/home/cvspublic co apr
  % cvs -d :pserver:anoncvs@cvs.apache.org:/home/cvspublic co apr-util
  % cvs -d :pserver:anoncvs@cvs.apache.org:/home/cvspublic co apr-iconv

* Two environmental variables must be set as well: 
  SUBVERSION - SVN libraries root directory
  WX - wxWindows libraries directory

  On Windows you can add the following to autoexec.bat or through the 
  Environmental Variables editor.
  set WX=C:\Program Files\wx2
  set SUBVERSION=D:\dev\subversion

PROJECT CONTRIBUTORS

* Paul Marculescu - original author
* Brent R. Matzelle - developer and project leader
* Alexander Mueller - developer and project leader
