# Microsoft Developer Studio Project File - Name="rapidsvn" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=rapidsvn - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "rapidsvn.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "rapidsvn.mak" CFG="rapidsvn - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "rapidsvn - Win32 Release" (basierend auf  "Win32 (x86) Application")
!MESSAGE "rapidsvn - Win32 Debug" (basierend auf  "Win32 (x86) Application")
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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(WX)/lib/msw" /I "$(WX)/include" /I "$(WX)/contrib/include" /I "$(SUBVERSION)/apr/include" /I "$(SUBVERSION)/apr-util/include" /I "$(SUBVERSION)/apr-util/xml/expat/lib" /I "$(SUBVERSION)/subversion/include" /D "__WXMSW__" /D "NDEBUG" /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "APR_DECLARE_STATIC" /D "__WIN95__" /D "__WIN32__" /D "STRICT" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "$(WX)/include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 src\svncpp\Release\svncpp.lib $(SUBVERSION)\Release\subversion\libsvn_client\libsvn_client-1.lib $(SUBVERSION)\Release\subversion\libsvn_delta\libsvn_delta-1.lib $(SUBVERSION)\Release\subversion\libsvn_diff\libsvn_diff-1.lib $(SUBVERSION)\Release\subversion\libsvn_fs\libsvn_fs-1.lib $(SUBVERSION)\Release\subversion\libsvn_ra\libsvn_ra-1.lib $(SUBVERSION)\Release\subversion\libsvn_ra_dav\libsvn_ra_dav-1.lib $(SUBVERSION)\Release\subversion\libsvn_ra_local\libsvn_ra_local-1.lib $(SUBVERSION)\Release\subversion\libsvn_ra_svn\libsvn_ra_svn-1.lib $(SUBVERSION)\Release\subversion\libsvn_repos\libsvn_repos-1.lib $(SUBVERSION)\Release\subversion\libsvn_subr\libsvn_subr-1.lib $(SUBVERSION)\Release\subversion\libsvn_wc\libsvn_wc-1.lib $(SUBVERSION)\apr-iconv\LibR\apriconv.lib $(SUBVERSION)\apr-util\LibR\aprutil.lib $(SUBVERSION)\apr-util\xml\expat\lib\LibR\xml.lib $(SUBVERSION)\apr\LibR\apr.lib $(SUBVERSION)\db4-win32\lib\libdb42.lib $(SUBVERSION)\neon\libneon.lib "$(WX)\lib\wxmsw.lib" wsock32.lib winspool.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib advapi32.lib kernel32.lib ole32.lib rpcrt4.lib shell32.lib comdlg32.lib gdi32.lib user32.lib comctl32.lib ws2_32.lib mswsock.lib shfolder.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libcmt"
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
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /I "$(WX)/lib/mswd" /I "$(WX)/include" /I "$(WX)/contrib/include" /I "$(SUBVERSION)/apr/include" /I "$(SUBVERSION)/apr-util/include" /I "$(SUBVERSION)/apr-util/xml/expat/lib" /I "$(SUBVERSION)/subversion/include" /D "_DEBUG" /D "__WINDOWS__" /D "__WXMSW__" /D DEBUG=1 /D "__WXDEBUG__" /D "__WIN95__" /D "__WIN32__" /D WINVER=0x0400 /D "STRICT" /D "WIN32" /D "_WINDOWS" /D "APR_DECLARE_STATIC" /D "APU_DECLARE_STATIC" /D "SVN_DEBUG" /FR /FD /GZ /c
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
# ADD LINK32 src\svncpp\Debug\svncpp.lib $(SUBVERSION)\Debug\subversion\libsvn_client\libsvn_client-1.lib $(SUBVERSION)\Debug\subversion\libsvn_delta\libsvn_delta-1.lib $(SUBVERSION)\Debug\subversion\libsvn_diff\libsvn_diff-1.lib $(SUBVERSION)\Debug\subversion\libsvn_fs\libsvn_fs-1.lib $(SUBVERSION)\Debug\subversion\libsvn_ra\libsvn_ra-1.lib $(SUBVERSION)\Debug\subversion\libsvn_ra_dav\libsvn_ra_dav-1.lib $(SUBVERSION)\Debug\subversion\libsvn_ra_local\libsvn_ra_local-1.lib $(SUBVERSION)\Debug\subversion\libsvn_ra_svn\libsvn_ra_svn-1.lib $(SUBVERSION)\Debug\subversion\libsvn_repos\libsvn_repos-1.lib $(SUBVERSION)\Debug\subversion\libsvn_subr\libsvn_subr-1.lib $(SUBVERSION)\Debug\subversion\libsvn_wc\libsvn_wc-1.lib $(SUBVERSION)\neon\libneond.lib $(SUBVERSION)\apr-iconv\LibD\apriconv.lib $(SUBVERSION)\apr-util\xml\expat\lib\LibD\xml.lib $(SUBVERSION)\apr\LibD\apr.lib $(SUBVERSION)\apr-util\LibD\aprutil.lib $(SUBVERSION)\db4-win32\lib\libdb42d.lib "$(WX)\lib\wxmswd.lib" wsock32.lib winspool.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib advapi32.lib kernel32.lib ole32.lib rpcrt4.lib shell32.lib comdlg32.lib gdi32.lib user32.lib comctl32.lib ws2_32.lib mswsock.lib shfolder.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"../../lib" /libpath:"../../contrib/lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "rapidsvn - Win32 Release"
# Name "rapidsvn - Win32 Debug"
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

SOURCE=.\src\bookmarks.cpp
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

SOURCE=.\src\rapidsvn.rc
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

SOURCE=.\src\tviewer.cpp
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

SOURCE=.\src\win32\verblist.cpp
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

SOURCE=.\src\bookmarks.hpp
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

SOURCE=.\src\icons.hpp
# End Source File
# Begin Source File

SOURCE=.\src\ids.hpp
# End Source File
# Begin Source File

SOURCE=.\src\import_action.hpp
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

SOURCE=.\src\svncpp\status.hpp
# End Source File
# Begin Source File

SOURCE=.\src\svn_notify.hpp
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

SOURCE=.\src\unique_array_string.hpp
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

SOURCE=.\src\verblist_win32.hpp
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

SOURCE=.\src\wx\msw\rightarr.cur
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

SOURCE=.\src\res\svn.ico
# End Source File
# Begin Source File

SOURCE=.\src\wx\msw\tip.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\tip.ico
# End Source File
# Begin Source File

SOURCE=.\src\res\bitmaps\versioned_folder.ico
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
# End Target
# End Project
