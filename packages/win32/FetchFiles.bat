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
del tmp\lib*.dll
del tmp\*.so

rem Copy Berkeley DB
copy %SUBVERSION%\db4-win32\bin\libdb44.dll tmp

rem Copy APR DLLs
copy %SUBVERSION%\apr\Release\libapr.dll tmp
copy %SUBVERSION%\apr-util\Release\libaprutil.dll tmp
copy %SUBVERSION%\apr-iconv\Release\libapriconv.dll tmp
copy %SUBVERSION%\apr-iconv\Release\iconv\*.so tmp

rem Copy Libintl DLL
copy %LIBINTL%\bin\intl3_svn.dll tmp

pause