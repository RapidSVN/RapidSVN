@echo off
rem Execute this command in the RAPIDSVN project directory
rem The following environment variable have to be set:
rem SUBVERSION - Subversion source directory
rem INNOSETUP  - Inno Setup program directory
rem LIBINTL    - Win32 Lib Intl directory
echo "Clean existing stuff"
msdev rapidsvn.dsw /MAKE ALL /CLEAN
echo "Build svncpp"
msdev src\svncpp\svncpp.dsp /MAKE ALL
echo "Build rapidsvn"
echo "Clean installer"
msdev rapidsvn.dsp /MAKE ALL
cd packages\win32
del /s /q tmp
del /s /q output\*.*
echo "Build installer"
call FetchFiles.bat
"%INNOSETUP%\iscc.exe" rapidsvn.iss
echo "Done"
