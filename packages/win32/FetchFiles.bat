@echo off
rem ---------------------------------------------------
rem FetchFiles.bat
rem ---------------------------------------------------
rem This batch script fetches DLLs and other files
rem needed for creating the package
rem ---------------------------------------------------
rem These environment variables are needed:
rem   SUBVERSION
rem ---------------------------------------------------
rem TODO: check whether environment variables
rem       are set to existing paths (or at all)

mkdir tmp

rem Delete OLD Files
del /q tmp\*.*
del /q output\*.*

rem Copy Berkeley DB
copy %SUBVERSION%\db4-win32\bin\libdb44.dll tmp

rem Copy Subversion and APR DLLs
copy %SUBVERSION%\bin\*.dll tmp
copy %SUBVERSION%\iconv\*.so tmp

rem Copy Subversion command line clients
copy %SUBVERSION%\bin\svn.exe tmp
copy %SUBVERSION%\bin\svnadmin.exe tmp

rem Copy C++ Runtime
copy msvcp60.dll tmp

rem Copy RapidSvn
copy ..\..\build\vc6\rapidsvn\Release\rapidsvn_ansi.exe tmp
copy ..\..\build\vc6\rapidsvn\Unicode_Release\rapidsvn.exe tmp

rem pause
