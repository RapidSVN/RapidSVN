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
del tmp\*.dll
del tmp\*.so

rem Copy Berkeley DB
copy %SUBVERSION%\db4-win32\bin\libdb44.dll tmp

rem Copy Subversion and APR DLLs
copy %SUBVERSION%\bin\*.dll tmp
copy %SUBVERSION%\iconv\*.so tmp

copy msvcp80.dll tmp
copy "..\..\build\vc2005\unicode release\rapidsvn.exe" tmp\rapidsvn_unicode.exe
copy ..\..\build\vc2005\release\rapidsvn.exe tmp\rapidsvn_ansi.exe
