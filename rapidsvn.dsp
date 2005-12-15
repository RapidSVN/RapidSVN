# Microsoft Developer Studio Project File - Name="rapidsvn" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=rapidsvn - Win32 Unicode Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "rapidsvn.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "rapidsvn.mak" CFG="rapidsvn - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "rapidsvn - Win32 Release" (basierend auf  "Win32 (x86) Application")
!MESSAGE "rapidsvn - Win32 Debug" (basierend auf  "Win32 (x86) Application")
!MESSAGE "rapidsvn - Win32 Unicode Debug" (basierend auf  "Win32 (x86) Application")
!MESSAGE "rapidsvn - Win32 Unicode Release" (basierend auf  "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "rapidsvn - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(WX)/lib/msw" /I "include" /I "$(WX)/include" /I "$(WX)/contrib/include" /I "$(SUBVERSION)/apr/include" /I "$(SUBVERSION)/apr-util/include" /I "$(SUBVERSION)/apr-util/xml/expat/lib" /I "$(SUBVERSION)/subversion/include" /D "WIN32" /D "NDEBUG" /D WINVER=0x0400 /D "STRICT" /D "APR_DECLARE_STATIC" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "$(WX)/include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 svncpp.lib libsvn_client-1.lib libsvn_delta-1.lib libsvn_diff-1.lib libsvn_fs_base-1.lib libsvn_fs-1.lib libsvn_fs_fs-1.lib libsvn_ra-1.lib libsvn_ra_dav-1.lib libsvn_ra_local-1.lib libsvn_ra_svn-1.lib libsvn_repos-1.lib libsvn_subr-1.lib libsvn_wc-1.lib libneon.lib libapriconv.lib xml.lib libapr.lib libaprutil.lib libdb43.lib wxmsw.lib wsock32.lib winspool.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib advapi32.lib kernel32.lib ole32.lib rpcrt4.lib shell32.lib comdlg32.lib gdi32.lib user32.lib comctl32.lib ws2_32.lib mswsock.lib shfolder.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libcmt" /libpath:"src\svncpp\Release" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_client" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_delta" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_diff" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_fs_base" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_fs" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_fs_fs" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_ra" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_ra_dav" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_ra_local" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_ra_svn" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_repos" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_subr" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_wc" /libpath:"$(SUBVERSION)\neon" /libpath:"$(SUBVERSION)\apr-iconv\Release" /libpath:"$(SUBVERSION)\apr-util\xml\expat\lib\LibR" /libpath:"$(SUBVERSION)\apr\Release" /libpath:"$(SUBVERSION)\apr-util\Release" /libpath:"$(SUBVERSION)\db4-win32\lib" /libpath:"$(WX)\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "rapidsvn - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /I "$(WX)/lib/mswd" /I "include" /I "$(WX)/include" /I "$(WX)/contrib/include" /I "$(SUBVERSION)/apr/include" /I "$(SUBVERSION)/apr-util/include" /I "$(SUBVERSION)/apr-util/xml/expat/lib" /I "$(SUBVERSION)/subversion/include" /D "WIN32" /D "_DEBUG" /D "__WXDEBUG__" /D WINVER=0x0400 /D "STRICT" /D "APU_DECLARE_STATIC" /D "SVN_DEBUG" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "$(WX)/include/" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 svncpp.lib libsvn_client-1.lib libsvn_delta-1.lib libsvn_diff-1.lib libsvn_fs_base-1.lib libsvn_fs-1.lib libsvn_fs_fs-1.lib libsvn_ra-1.lib libsvn_ra_dav-1.lib libsvn_ra_local-1.lib libsvn_ra_svn-1.lib libsvn_repos-1.lib libsvn_subr-1.lib libsvn_wc-1.lib libneond.lib libapriconv.lib xml.lib libapr.lib libaprutil.lib libdb43d.lib wxmswd.lib wsock32.lib winspool.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib advapi32.lib kernel32.lib ole32.lib rpcrt4.lib shell32.lib comdlg32.lib gdi32.lib user32.lib comctl32.lib ws2_32.lib mswsock.lib shfolder.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmtd" /pdbtype:sept /libpath:"src\svncpp\Debug" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_client" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_delta" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_diff" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_fs_base" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_fs" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_fs_fs" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_ra" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_ra_dav" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_ra_local" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_ra_svn" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_repos" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_subr" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_wc" /libpath:"$(SUBVERSION)\neon" /libpath:"$(SUBVERSION)\apr-iconv\Debug" /libpath:"$(SUBVERSION)\apr-util\xml\expat\lib\LibD" /libpath:"$(SUBVERSION)\apr\Debug" /libpath:"$(SUBVERSION)\apr-util\Debug" /libpath:"$(SUBVERSION)\db4-win32\lib" /libpath:"$(WX)\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "rapidsvn - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "rapidsvn___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "rapidsvn___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Unicode_Debug"
# PROP Intermediate_Dir "Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /I "$(WX)/lib/mswd" /I "include" /I "$(WX)/include" /I "$(WX)/contrib/include" /I "$(SUBVERSION)/apr/include" /I "$(SUBVERSION)/apr-util/include" /I "$(SUBVERSION)/apr-util/xml/expat/lib" /I "$(SUBVERSION)/subversion/include" /D "_DEBUG" /D "__WINDOWS__" /D "__WXMSW__" /D DEBUG=1 /D "__WXDEBUG__" /D "__WIN95__" /D "__WIN32__" /D WINVER=0x0400 /D "STRICT" /D "WIN32" /D "_WINDOWS" /D "APR_DECLARE_STATIC" /D "APU_DECLARE_STATIC" /D "SVN_DEBUG" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /I "$(WX)/lib/mswud" /I "include" /I "$(WX)/include" /I "$(WX)/contrib/include" /I "$(SUBVERSION)/apr/include" /I "$(SUBVERSION)/apr-util/include" /I "$(SUBVERSION)/apr-util/xml/expat/lib" /I "$(SUBVERSION)/subversion/include" /D "WIN32" /D "_DEBUG" /D "__WXDEBUG__" /D WINVER=0x0400 /D "STRICT" /D "_UNICODE" /D "UNICODE" /D "APR_DECLARE_STATIC" /D "SVN_DEBUG" /FR /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i "$(WX)/include/" /d "_DEBUG"
# ADD RSC /l 0x409 /i "$(WX)/include/" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 svncpp.lib libsvn_client-1.lib libsvn_delta-1.lib libsvn_diff-1.lib libsvn_fs_base-1.lib libsvn_fs-1.lib libsvn_fs_fs-1.lib libsvn_ra-1.lib libsvn_ra_dav-1.lib libsvn_ra_local-1.lib libsvn_ra_svn-1.lib libsvn_repos-1.lib libsvn_subr-1.lib libsvn_wc-1.lib libneond.lib libapriconv.lib xml.lib libapr.lib libaprutil.lib libdb43d.lib wxmswd.lib wsock32.lib winspool.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib advapi32.lib kernel32.lib ole32.lib rpcrt4.lib shell32.lib comdlg32.lib gdi32.lib user32.lib comctl32.lib ws2_32.lib mswsock.lib shfolder.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"src\svncpp\Debug" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_client" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_delta" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_diff" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_fs_base" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_fs" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_fs_fs" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_ra" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_ra_dav" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_ra_local" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_ra_svn" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_repos" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_subr" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_wc" /libpath:"$(SUBVERSION)\neon" /libpath:"$(SUBVERSION)\apr-iconv\Debug" /libpath:"$(SUBVERSION)\apr-util\xml\expat\lib\LibD" /libpath:"$(SUBVERSION)\apr\Debug" /libpath:"$(SUBVERSION)\apr-util\Debug" /libpath:"$(SUBVERSION)\db4-win32\lib" /libpath:"$(WX)\lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 svncpp.lib libsvn_client-1.lib libsvn_delta-1.lib libsvn_diff-1.lib libsvn_fs_base-1.lib libsvn_fs-1.lib libsvn_fs_fs-1.lib libsvn_ra-1.lib libsvn_ra_dav-1.lib libsvn_ra_local-1.lib libsvn_ra_svn-1.lib libsvn_repos-1.lib libsvn_subr-1.lib libsvn_wc-1.lib libneond.lib libapriconv.lib xml.lib libapr.lib libaprutil.lib libdb43d.lib wxmswud.lib wsock32.lib winspool.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib advapi32.lib kernel32.lib ole32.lib rpcrt4.lib shell32.lib comdlg32.lib gdi32.lib user32.lib comctl32.lib ws2_32.lib mswsock.lib shfolder.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmtd" /pdbtype:sept /libpath:"src\svncpp\Unicode_Debug" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_client" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_delta" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_diff" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_fs_base" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_fs" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_fs_fs" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_ra" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_ra_dav" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_ra_local" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_ra_svn" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_repos" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_subr" /libpath:"$(SUBVERSION)\Debug\subversion\libsvn_wc" /libpath:"$(SUBVERSION)\neon" /libpath:"$(SUBVERSION)\apr-iconv\Debug" /libpath:"$(SUBVERSION)\apr-util\xml\expat\lib\LibD" /libpath:"$(SUBVERSION)\apr\Debug" /libpath:"$(SUBVERSION)\apr-util\Debug" /libpath:"$(SUBVERSION)\db4-win32\lib" /libpath:"$(WX)\lib"
# SUBTRACT LINK32 /pdb:none /incremental:no

!ELSEIF  "$(CFG)" == "rapidsvn - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "rapidsvn___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "rapidsvn___Win32_Unicode_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Unicode_Release"
# PROP Intermediate_Dir "Unicode_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "$(WX)/lib/msw" /I "include" /I "$(WX)/include" /I "$(WX)/contrib/include" /I "$(SUBVERSION)/apr/include" /I "$(SUBVERSION)/apr-util/include" /I "$(SUBVERSION)/apr-util/xml/expat/lib" /I "$(SUBVERSION)/subversion/include" /D "__WXMSW__" /D "NDEBUG" /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "APR_DECLARE_STATIC" /D "__WIN95__" /D "__WIN32__" /D "STRICT" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(WX)/lib/mswu" /I "include" /I "$(WX)/include" /I "$(WX)/contrib/include" /I "$(SUBVERSION)/apr/include" /I "$(SUBVERSION)/apr-util/include" /I "$(SUBVERSION)/apr-util/xml/expat/lib" /I "$(SUBVERSION)/subversion/include" /D "WIN32" /D "NDEBUG" /D WINVER=0x0400 /D "STRICT" /D "_UNICODE" /D "UNICODE" /D "APR_DECLARE_STATIC" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i "$(WX)/include" /d "NDEBUG"
# ADD RSC /l 0x409 /i "$(WX)/include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 svncpp.lib libsvn_client-1.lib libsvn_delta-1.lib libsvn_diff-1.lib libsvn_fs_base-1.lib libsvn_fs-1.lib libsvn_fs_fs-1.lib libsvn_ra-1.lib libsvn_ra_dav-1.lib libsvn_ra_local-1.lib libsvn_ra_svn-1.lib libsvn_repos-1.lib libsvn_subr-1.lib libsvn_wc-1.lib libneon.lib libapriconv.lib xml.lib libapr.lib libaprutil.lib libdb43.lib wxmsw.lib wsock32.lib winspool.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib advapi32.lib kernel32.lib ole32.lib rpcrt4.lib shell32.lib comdlg32.lib gdi32.lib user32.lib comctl32.lib ws2_32.lib mswsock.lib shfolder.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libcmt" /libpath:"src\svncpp\Release" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_client" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_delta" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_diff" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_fs_base" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_fs" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_fs_fs" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_ra" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_ra_dav" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_ra_local" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_ra_svn" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_repos" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_subr" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_wc" /libpath:"$(SUBVERSION)\neon" /libpath:"$(SUBVERSION)\apr-iconv\Release" /libpath:"$(SUBVERSION)\apr-util\xml\expat\lib\LibR" /libpath:"$(SUBVERSION)\apr\Release" /libpath:"$(SUBVERSION)\apr-util\Release" /libpath:"$(SUBVERSION)\db4-win32\lib" /libpath:"$(WX)\lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 svncpp.lib libsvn_client-1.lib libsvn_delta-1.lib libsvn_diff-1.lib libsvn_fs_base-1.lib libsvn_fs-1.lib libsvn_fs_fs-1.lib libsvn_ra-1.lib libsvn_ra_dav-1.lib libsvn_ra_local-1.lib libsvn_ra_svn-1.lib libsvn_repos-1.lib libsvn_subr-1.lib libsvn_wc-1.lib libneon.lib libapriconv.lib xml.lib libapr.lib libaprutil.lib libdb43.lib wxmswu.lib wsock32.lib winspool.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib advapi32.lib kernel32.lib ole32.lib rpcrt4.lib shell32.lib comdlg32.lib gdi32.lib user32.lib comctl32.lib ws2_32.lib mswsock.lib shfolder.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libcmt" /libpath:"src\svncpp\Unicode_Release" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_client" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_delta" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_diff" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_fs_base" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_fs" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_fs_fs" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_ra" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_ra_dav" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_ra_local" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_ra_svn" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_repos" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_subr" /libpath:"$(SUBVERSION)\Release\subversion\libsvn_wc" /libpath:"$(SUBVERSION)\neon" /libpath:"$(SUBVERSION)\apr-iconv\Release" /libpath:"$(SUBVERSION)\apr-util\xml\expat\lib\LibR" /libpath:"$(SUBVERSION)\apr\Release" /libpath:"$(SUBVERSION)\apr-util\Release" /libpath:"$(SUBVERSION)\db4-win32\lib" /libpath:"$(WX)\lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "rapidsvn - Win32 Release"
# Name "rapidsvn - Win32 Debug"
# Name "rapidsvn - Win32 Unicode Debug"
# Name "rapidsvn - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\about_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\action.cpp
# End Source File
# Begin Source File

SOURCE=.\src\add_action.cpp
# End Source File
# Begin Source File

SOURCE=.\src\auth_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cert_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\checkout_action.cpp
# End Source File
# Begin Source File

SOURCE=.\src\checkout_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cleanup_action.cpp
# End Source File
# Begin Source File

SOURCE=.\src\commit_action.cpp
# End Source File
# Begin Source File

SOURCE=.\src\commit_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\delete_action.cpp
# End Source File
# Begin Source File

SOURCE=.\src\delete_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\destination_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\diff_action.cpp
# End Source File
# Begin Source File

SOURCE=.\src\diff_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\exceptions.cpp
# End Source File
# Begin Source File

SOURCE=.\src\external_program_action.cpp
# End Source File
# Begin Source File

SOURCE=.\src\file_info.cpp
# End Source File
# Begin Source File

SOURCE=.\src\filelist_ctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\folder_browser.cpp
# End Source File
# Begin Source File

SOURCE=.\src\folder_item_data.cpp
# End Source File
# Begin Source File

SOURCE=.\src\get_action.cpp
# End Source File
# Begin Source File

SOURCE=.\src\hist_entries.cpp
# End Source File
# Begin Source File

SOURCE=.\src\hist_mgr.cpp
# End Source File
# Begin Source File

SOURCE=.\src\hist_val.cpp
# End Source File
# Begin Source File

SOURCE=.\src\import_action.cpp
# End Source File
# Begin Source File

SOURCE=.\src\import_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\listed_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\listener.cpp
# End Source File
# Begin Source File

SOURCE=.\src\lock_action.cpp
# End Source File
# Begin Source File

SOURCE=.\src\lock_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\log_action.cpp
# End Source File
# Begin Source File

SOURCE=.\src\log_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\merge_action.cpp
# End Source File
# Begin Source File

SOURCE=.\src\merge_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\mkdir_action.cpp
# End Source File
# Begin Source File

SOURCE=.\src\move_action.cpp
# End Source File
# Begin Source File

SOURCE=.\src\preferences.cpp
# End Source File
# Begin Source File

SOURCE=.\src\preferences_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\property_action.cpp
# End Source File
# Begin Source File

SOURCE=.\src\property_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\proportional_splitter.cpp
# End Source File
# Begin Source File

SOURCE=.\src\rapidsvn_app.cpp
# End Source File
# Begin Source File

SOURCE=.\src\rapidsvn_frame.cpp
# End Source File
# Begin Source File

SOURCE=.\src\report_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\resolve_action.cpp
# End Source File
# Begin Source File

SOURCE=.\src\revert_action.cpp
# End Source File
# Begin Source File

SOURCE=.\src\simple_worker.cpp
# End Source File
# Begin Source File

SOURCE=.\src\switch_action.cpp
# End Source File
# Begin Source File

SOURCE=.\src\trace_update.cpp
# End Source File
# Begin Source File

SOURCE=.\src\tracer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\unlock_action.cpp
# End Source File
# Begin Source File

SOURCE=.\src\unlock_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\update_action.cpp
# End Source File
# Begin Source File

SOURCE=.\src\update_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\utils.cpp
# End Source File
# Begin Source File

SOURCE=.\src\verblist.cpp
# End Source File
# Begin Source File

SOURCE=.\src\view_action.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\about_dlg.hpp
# End Source File
# Begin Source File

SOURCE=.\src\action.hpp
# End Source File
# Begin Source File

SOURCE=.\src\action_worker.hpp
# End Source File
# Begin Source File

SOURCE=.\src\add_action.hpp
# End Source File
# Begin Source File

SOURCE=.\src\auth_dlg.hpp
# End Source File
# Begin Source File

SOURCE=.\src\cert_dlg.hpp
# End Source File
# Begin Source File

SOURCE=.\src\checkout_action.hpp
# End Source File
# Begin Source File

SOURCE=.\src\checkout_data.hpp
# End Source File
# Begin Source File

SOURCE=.\src\checkout_dlg.hpp
# End Source File
# Begin Source File

SOURCE=.\src\cleanup_action.hpp
# End Source File
# Begin Source File

SOURCE=.\src\commit_action.hpp
# End Source File
# Begin Source File

SOURCE=.\src\commit_dlg.hpp
# End Source File
# Begin Source File

SOURCE=.\src\config.hpp
# End Source File
# Begin Source File

SOURCE=.\src\delete_action.hpp
# End Source File
# Begin Source File

SOURCE=.\src\delete_dlg.hpp
# End Source File
# Begin Source File

SOURCE=.\src\destination_dlg.hpp
# End Source File
# Begin Source File

SOURCE=.\src\diff_action.hpp
# End Source File
# Begin Source File

SOURCE=.\src\diff_data.hpp
# End Source File
# Begin Source File

SOURCE=.\src\diff_dlg.hpp
# End Source File
# Begin Source File

SOURCE=.\src\exceptions.hpp
# End Source File
# Begin Source File

SOURCE=.\src\external_program_action.hpp
# End Source File
# Begin Source File

SOURCE=.\src\file_info.hpp
# End Source File
# Begin Source File

SOURCE=.\src\filelist_ctrl.hpp
# End Source File
# Begin Source File

SOURCE=.\src\folder_browser.hpp
# End Source File
# Begin Source File

SOURCE=.\src\folder_item_data.hpp
# End Source File
# Begin Source File

SOURCE=.\src\get_action.hpp
# End Source File
# Begin Source File

SOURCE=.\src\get_data.hpp
# End Source File
# Begin Source File

SOURCE=.\src\hist_entries.hpp
# End Source File
# Begin Source File

SOURCE=.\src\hist_mgr.hpp
# End Source File
# Begin Source File

SOURCE=.\src\hist_val.hpp
# End Source File
# Begin Source File

SOURCE=.\src\icons.hpp
# End Source File
# Begin Source File

SOURCE=.\src\ids.hpp
# End Source File
# Begin Source File

SOURCE=.\src\import_action.hpp
# End Source File
# Begin Source File

SOURCE=.\src\import_data.hpp
# End Source File
# Begin Source File

SOURCE=.\src\import_dlg.hpp
# End Source File
# Begin Source File

SOURCE=.\src\listed_dlg.hpp
# End Source File
# Begin Source File

SOURCE=.\src\listener.hpp
# End Source File
# Begin Source File

SOURCE=.\src\lock_action.hpp
# End Source File
# Begin Source File

SOURCE=.\src\lock_dlg.hpp
# End Source File
# Begin Source File

SOURCE=.\src\log_action.hpp
# End Source File
# Begin Source File

SOURCE=.\src\log_dlg.hpp
# End Source File
# Begin Source File

SOURCE=.\src\merge_action.hpp
# End Source File
# Begin Source File

SOURCE=.\src\merge_dlg.hpp
# End Source File
# Begin Source File

SOURCE=.\src\mkdir_action.hpp
# End Source File
# Begin Source File

SOURCE=.\src\move_action.hpp
# End Source File
# Begin Source File

SOURCE=.\src\preferences.hpp
# End Source File
# Begin Source File

SOURCE=.\src\preferences_dlg.hpp
# End Source File
# Begin Source File

SOURCE=.\src\property_action.hpp
# End Source File
# Begin Source File

SOURCE=.\src\property_dlg.hpp
# End Source File
# Begin Source File

SOURCE=.\src\proportional_splitter.hpp
# End Source File
# Begin Source File

SOURCE=.\src\rapidsvn_app.hpp
# End Source File
# Begin Source File

SOURCE=.\src\rapidsvn_frame.hpp
# End Source File
# Begin Source File

SOURCE=.\src\report_dlg.hpp
# End Source File
# Begin Source File

SOURCE=.\src\resolve_action.hpp
# End Source File
# Begin Source File

SOURCE=.\src\resource.hpp
# End Source File
# Begin Source File

SOURCE=.\src\revert_action.hpp
# End Source File
# Begin Source File

SOURCE=.\src\simple_worker.hpp
# End Source File
# Begin Source File

SOURCE=.\src\switch_action.hpp
# End Source File
# Begin Source File

SOURCE=.\src\trace_update.hpp
# End Source File
# Begin Source File

SOURCE=.\src\tracer.hpp
# End Source File
# Begin Source File

SOURCE=.\src\unlock_action.hpp
# End Source File
# Begin Source File

SOURCE=.\src\unlock_dlg.hpp
# End Source File
# Begin Source File

SOURCE=.\src\update_action.hpp
# End Source File
# Begin Source File

SOURCE=.\src\update_data.hpp
# End Source File
# Begin Source File

SOURCE=.\src\update_dlg.hpp
# End Source File
# Begin Source File

SOURCE=.\src\utils.hpp
# End Source File
# Begin Source File

SOURCE=.\src\verblist.hpp
# End Source File
# Begin Source File

SOURCE=.\src\version.hpp
# End Source File
# Begin Source File

SOURCE=.\src\view_action.hpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\src\rapidsvn.rc
# End Source File
# End Group
# End Target
# End Project
