# Microsoft Developer Studio Project File - Name="rapidsvn" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=rapidsvn - Win32 Debug DLL
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rapidsvn.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rapidsvn.mak" CFG="rapidsvn - Win32 Debug DLL"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "rapidsvn - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "rapidsvn - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "rapidsvn - Win32 Debug DLL" (based on "Win32 (x86) Application")
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
# ADD CPP /nologo /MDd /W3 /GX /O2 /I "$(WX)/include" /I "$(WX)/contrib/include" /I "$(SUBVERSION)/apr/include" /I "$(SUBVERSION)/apr-util/include" /I "$(SUBVERSION)/apr-util/xml/expat/lib" /I "$(SUBVERSION)/subversion/include" /D "NDEBUG" /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "APR_DECLARE_STATIC" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "$(WX)/include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 comctl32.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib rpcrt4.lib /nologo /subsystem:windows /machine:I386

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(SUBVERSION)/include" /I "$(WX)/include" /I "$(WX)/contrib/include" /I "$(APR)/include" /D "SVN_DEBUG" /D "_DEBUG" /D WINVER=0x400 /D "_MT" /D wxUSE_GUI=1 /D "__WXDEBUG__" /D WXDEBUG=1 /D "WIN32" /D "_WINDOWS" /D "APR_DECLARE_STATIC" /D "APU_DECLARE_STATIC" /Yu"include.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "$(WX)/include" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 $(SUBVERSION)\libsvn_client\Debug\libsvn_client.lib $(SUBVERSION)\libsvn_delta\Debug\libsvn_delta.lib $(SUBVERSION)\libsvn_subr\Debug\libsvn_subr.lib $(SUBVERSION)\libsvn_wc\Debug\libsvn_wc.lib $(SUBVERSION)\libsvn_ra\Debug\libsvn_ra.lib $(SUBVERSION)\libsvn_ra_local\Debug\libsvn_ra_local.lib $(SUBVERSION)\libsvn_ra_dav\Debug\libsvn_ra_dav.lib $(SUBVERSION)\libsvn_repos\Debug\libsvn_repos.lib $(SUBVERSION)\libsvn_fs\Debug\libsvn_fs.lib $(APR)\LibD\apr.lib D:\subversion\apr-util\LibD\aprutil.lib $(SVN)\neon\libneonD.lib $(SVN)\db4-win32\lib\libdb40d.lib $(WX)\lib\wxmswd.lib comctl32.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib rpcrt4.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "rapidsvn - Win32 Debug DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug DLL"
# PROP BASE Intermediate_Dir "Debug DLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugDLL"
# PROP Intermediate_Dir "DebugDLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /I "$(WX)/include" /I "$(WX)/contrib/include" /I "$(SUBVERSION)/apr/include" /I "$(SUBVERSION)/apr-util/include" /I "$(SUBVERSION)/apr-util/xml/expat/lib" /I "$(SUBVERSION)/subversion/include" /D "_DEBUG" /D "__WINDOWS__" /D "__WXMSW__" /D DEBUG=1 /D "__WXDEBUG__" /D "__WIN95__" /D "__WIN32__" /D WINVER=0x0400 /D "STRICT" /D "WIN32" /D "_WINDOWS" /D "APR_DECLARE_STATIC" /D "APU_DECLARE_STATIC" /D "SVN_DEBUG" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "$(WX)/include/" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 $(SUBVERSION)\db4-win32\lib\libdb40d.lib $(SUBVERSION)\subversion\libsvn_repos\Debug\libsvn_repos.lib $(SUBVERSION)\subversion\libsvn_fs\Debug\libsvn_fs.lib $(SUBVERSION)\subversion\libsvn_ra_dav\Debug\libsvn_ra_dav.lib $(SUBVERSION)\subversion\libsvn_ra_local\Debug\libsvn_ra_local.lib $(SUBVERSION)\subversion\libsvn_ra\Debug\libsvn_ra.lib advapi32.lib mswsock.lib ws2_32.lib $(SUBVERSION)\neon\libneonD.lib $(SUBVERSION)\apr-iconv\LibD\apriconv.lib $(SUBVERSION)\subversion\libsvn_wc\Debug\libsvn_wc.lib $(SUBVERSION)\subversion\libsvn_subr\Debug\libsvn_subr.lib $(SUBVERSION)\apr-util\xml\expat\lib\LibD\xml.lib $(SUBVERSION)\subversion\libsvn_client\Debug\libsvn_client.lib $(SUBVERSION)\subversion\libsvn_delta\Debug\libsvn_delta.lib $(SUBVERSION)\apr\LibD\apr.lib $(SUBVERSION)\apr-util\LibD\aprutil.lib rpcrt4.lib winmm.lib "$(WX)\lib\wxmswd.lib" comctl32.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcd.lib" /nodefaultlib:"libcid.lib" /pdbtype:sept /libpath:"../../lib" /libpath:"../../contrib/lib"

!ENDIF 

# Begin Target

# Name "rapidsvn - Win32 Release"
# Name "rapidsvn - Win32 Debug"
# Name "rapidsvn - Win32 Debug DLL"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\action_thread.cpp
# End Source File
# Begin Source File

SOURCE=.\src\add_action.cpp
# End Source File
# Begin Source File

SOURCE=.\src\auth_baton.cpp
# End Source File
# Begin Source File

SOURCE=.\src\checkout_action.cpp
# End Source File
# Begin Source File

SOURCE=.\src\checkout_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\commit_action.cpp
# End Source File
# Begin Source File

SOURCE=.\src\commit_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\copy_action.cpp
# End Source File
# Begin Source File

SOURCE=.\src\copy_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\delete_action.cpp
# End Source File
# Begin Source File

SOURCE=.\src\delete_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\exceptions.cpp
# End Source File
# Begin Source File

SOURCE=.\src\filelist_ctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\folder_browser.cpp
# End Source File
# Begin Source File

SOURCE=.\src\import_action.cpp
# End Source File
# Begin Source File

SOURCE=.\src\import_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\include.cpp
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

SOURCE=.\src\mkdir_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\notify.cpp
# End Source File
# Begin Source File

SOURCE=.\src\rapidsvn.cpp
# End Source File
# Begin Source File

SOURCE=.\src\rapidsvn.rc
# End Source File
# Begin Source File

SOURCE=.\src\rapidsvn_app.cpp
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

SOURCE=.\src\svn_file_info.cpp
# End Source File
# Begin Source File

SOURCE=.\src\svn_file_log.cpp
# End Source File
# Begin Source File

SOURCE=.\src\svn_file_status.cpp
# End Source File
# Begin Source File

SOURCE=.\src\trace_commit.cpp
# End Source File
# Begin Source File

SOURCE=.\src\trace_update.cpp
# End Source File
# Begin Source File

SOURCE=.\src\tracer.cpp
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
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\action_thread.h
# End Source File
# Begin Source File

SOURCE=.\src\add_action.h
# End Source File
# Begin Source File

SOURCE=.\src\auth_baton.h
# End Source File
# Begin Source File

SOURCE=.\src\bitmaps.h
# End Source File
# Begin Source File

SOURCE=.\src\checkout_action.h
# End Source File
# Begin Source File

SOURCE=.\src\checkout_dlg.h
# End Source File
# Begin Source File

SOURCE=.\src\commit_action.h
# End Source File
# Begin Source File

SOURCE=.\src\commit_dlg.h
# End Source File
# Begin Source File

SOURCE=.\src\copy_action.h
# End Source File
# Begin Source File

SOURCE=.\src\copy_dlg.h
# End Source File
# Begin Source File

SOURCE=.\src\delete_action.h
# End Source File
# Begin Source File

SOURCE=.\src\delete_dlg.h
# End Source File
# Begin Source File

SOURCE=.\src\exceptions.h
# End Source File
# Begin Source File

SOURCE=.\src\filelist_ctrl.h
# End Source File
# Begin Source File

SOURCE=.\src\folder_browser.h
# End Source File
# Begin Source File

SOURCE=.\src\icons.h
# End Source File
# Begin Source File

SOURCE=.\src\import_action.h
# End Source File
# Begin Source File

SOURCE=.\src\import_dlg.h
# End Source File
# Begin Source File

SOURCE=.\src\include.h
# End Source File
# Begin Source File

SOURCE=.\src\merge_action.h
# End Source File
# Begin Source File

SOURCE=.\src\merge_dlg.h
# End Source File
# Begin Source File

SOURCE=.\src\mkdir_action.h
# End Source File
# Begin Source File

SOURCE=.\src\mkdir_dlg.h
# End Source File
# Begin Source File

SOURCE=.\src\notify.h
# End Source File
# Begin Source File

SOURCE=.\src\rapidsvn.h
# End Source File
# Begin Source File

SOURCE=.\src\rapidsvn_app.h
# End Source File
# Begin Source File

SOURCE=.\src\report_dlg.h
# End Source File
# Begin Source File

SOURCE=.\src\resolve_action.h
# End Source File
# Begin Source File

SOURCE=.\src\resource.h
# End Source File
# Begin Source File

SOURCE=.\src\revert_action.h
# End Source File
# Begin Source File

SOURCE=.\src\svn_file_info.h
# End Source File
# Begin Source File

SOURCE=.\src\svn_file_log.h
# End Source File
# Begin Source File

SOURCE=.\src\svn_file_status.h
# End Source File
# Begin Source File

SOURCE=.\src\trace_commit.h
# End Source File
# Begin Source File

SOURCE=.\src\trace_update.h
# End Source File
# Begin Source File

SOURCE=.\src\tracer.h
# End Source File
# Begin Source File

SOURCE=.\src\update_action.h
# End Source File
# Begin Source File

SOURCE=.\src\update_dlg.h
# End Source File
# Begin Source File

SOURCE=.\src\utils.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmaps\absent_file.ico
# End Source File
# Begin Source File

SOURCE=.\src\res\bitmaps\absent_file.ico
# End Source File
# Begin Source File

SOURCE=.\res\bitmaps\added_file.ico
# End Source File
# Begin Source File

SOURCE=.\src\res\bitmaps\added_file.ico
# End Source File
# Begin Source File

SOURCE=.\src\wx\msw\blank.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\blank.cur
# End Source File
# Begin Source File

SOURCE=.\src\wx\msw\bullseye.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\bullseye.cur
# End Source File
# Begin Source File

SOURCE=.\src\wx\msw\cdrom.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\cdrom.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\colours.bmp
# End Source File
# Begin Source File

SOURCE=.\src\wx\msw\computer.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\computer.ico
# End Source File
# Begin Source File

SOURCE=.\res\bitmaps\conflicted_file.ico
# End Source File
# Begin Source File

SOURCE=.\src\res\bitmaps\conflicted_file.ico
# End Source File
# Begin Source File

SOURCE=.\res\bitmaps\deleted_file.ico
# End Source File
# Begin Source File

SOURCE=.\src\res\bitmaps\deleted_file.ico
# End Source File
# Begin Source File

SOURCE=.\src\wx\msw\drive.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\drive.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\error.ico
# End Source File
# Begin Source File

SOURCE=.\src\wx\msw\file1.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\file1.ico
# End Source File
# Begin Source File

SOURCE=.\src\wx\msw\floppy.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\floppy.ico
# End Source File
# Begin Source File

SOURCE=.\res\bitmaps\folder.ico
# End Source File
# Begin Source File

SOURCE=.\src\res\bitmaps\folder.ico
# End Source File
# Begin Source File

SOURCE=.\src\wx\msw\folder1.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\folder1.ico
# End Source File
# Begin Source File

SOURCE=.\src\wx\msw\folder2.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\folder2.ico
# End Source File
# Begin Source File

SOURCE=.\src\wx\msw\hand.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\hand.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\info.ico
# End Source File
# Begin Source File

SOURCE=.\src\wx\msw\magnif1.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\magnif1.cur
# End Source File
# Begin Source File

SOURCE=.\res\bitmaps\merged_file.ico
# End Source File
# Begin Source File

SOURCE=.\src\res\bitmaps\merged_file.ico
# End Source File
# Begin Source File

SOURCE=.\res\bitmaps\modified_file.ico
# End Source File
# Begin Source File

SOURCE=.\src\res\bitmaps\modified_file.ico
# End Source File
# Begin Source File

SOURCE=.\res\mondrian.ico
# End Source File
# Begin Source File

SOURCE=.\src\res\mondrian.ico
# End Source File
# Begin Source File

SOURCE=.\src\wx\msw\noentry.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\noentry.cur
# End Source File
# Begin Source File

SOURCE=.\res\bitmaps\nonsvn_file.ico
# End Source File
# Begin Source File

SOURCE=.\src\res\bitmaps\nonsvn_file.ico
# End Source File
# Begin Source File

SOURCE=.\res\bitmaps\normal_file.ico
# End Source File
# Begin Source File

SOURCE=.\src\res\bitmaps\normal_file.ico
# End Source File
# Begin Source File

SOURCE=.\src\wx\msw\pbrush.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\pbrush.cur
# End Source File
# Begin Source File

SOURCE=.\src\wx\msw\pencil.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\pencil.cur
# End Source File
# Begin Source File

SOURCE=.\src\wx\msw\pntleft.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\pntleft.cur
# End Source File
# Begin Source File

SOURCE=.\src\wx\msw\pntright.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\pntright.cur
# End Source File
# Begin Source File

SOURCE=.\src\wx\msw\query.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\query.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\question.ico
# End Source File
# Begin Source File

SOURCE=.\src\wx\msw\removble.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\removble.ico
# End Source File
# Begin Source File

SOURCE=.\res\bitmaps\replaced_file.ico
# End Source File
# Begin Source File

SOURCE=.\src\res\bitmaps\replaced_file.ico
# End Source File
# Begin Source File

SOURCE=.\src\wx\msw\roller.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\roller.cur
# End Source File
# Begin Source File

SOURCE=.\src\wx\msw\size.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\size.cur
# End Source File
# Begin Source File

SOURCE=.\src\wx\msw\tip.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\tip.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\warning.ico
# End Source File
# Begin Source File

SOURCE=.\src\wx\msw\watch1.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\watch1.cur
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\checkout.wxr
# End Source File
# End Target
# End Project
