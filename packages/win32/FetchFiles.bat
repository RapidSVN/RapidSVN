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

rem Copy C++ Runtime
copy msvcp60.dll tmp

rem Copy RapidSvn
copy ..\..\Release\rapidsvn_ansi.exe tmp
copy ..\..\Unicode_Release\rapidsvn_unicode.exe tmp

rem pause
