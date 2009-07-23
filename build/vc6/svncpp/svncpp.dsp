# Microsoft Developer Studio Project File - Name="svncpp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=svncpp - Win32 Unicode Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "svncpp.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "svncpp.mak" CFG="svncpp - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "svncpp - Win32 Release" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE "svncpp - Win32 Debug" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE "svncpp - Win32 Unicode Debug" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE "svncpp - Win32 Unicode Release" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "svncpp - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I "../../../include" /I "$(SUBVERSION)/include" /I "$(SUBVERSION)/include/apr" /I "$(SUBVERSION)/include/apr-iconv" /I "$(SUBVERSION)/include/apr-util" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../include" /I "$(SUBVERSION)/include" /I "$(SUBVERSION)/include/apr" /I "$(SUBVERSION)/include/apr-iconv" /I "$(SUBVERSION)/include/apr-util" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "svncpp - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../../include" /I "$(SUBVERSION)/include" /I "$(SUBVERSION)/include/apr" /I "$(SUBVERSION)/include/apr-iconv" /I "$(SUBVERSION)/include/apr-util" /D "WIN32" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../include" /I "$(SUBVERSION)/include" /I "$(SUBVERSION)/include/apr" /I "$(SUBVERSION)/include/apr-iconv" /I "$(SUBVERSION)/include/apr-util" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "SVN_DEBUG" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "svncpp - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "svncpp___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "svncpp___Win32_Unicode_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Unicode_Debug"
# PROP Intermediate_Dir "Unicode_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../include" /I "$(SUBVERSION)/include" /I "$(SUBVERSION)/include/apr" /I "$(SUBVERSION)/include/apr-iconv" /I "$(SUBVERSION)/include/apr-util" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "SVN_DEBUG" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../include" /I "$(SUBVERSION)/include" /I "$(SUBVERSION)/include/apr" /I "$(SUBVERSION)/include/apr-iconv" /I "$(SUBVERSION)/include/apr-util" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "SVN_DEBUG" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "svncpp - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "svncpp___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "svncpp___Win32_Unicode_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Unicode_Release"
# PROP Intermediate_Dir "Unicode_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../../../include" /I "$(SUBVERSION)/include" /I "$(SUBVERSION)/include/apr" /I "$(SUBVERSION)/include/apr-iconv" /I "$(SUBVERSION)/include/apr-util" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../include" /I "$(SUBVERSION)/include" /I "$(SUBVERSION)/include/apr" /I "$(SUBVERSION)/include/apr-iconv" /I "$(SUBVERSION)/include/apr-util" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "svncpp - Win32 Release"
# Name "svncpp - Win32 Debug"
# Name "svncpp - Win32 Unicode Debug"
# Name "svncpp - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\src\svncpp\apr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\svncpp\client.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\svncpp\client_annotate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\svncpp\client_cat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\svncpp\client_diff.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\svncpp\client_ls.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\svncpp\client_modify.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\svncpp\client_property.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\svncpp\client_status.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\svncpp\context.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\svncpp\datetime.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\svncpp\dirent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\svncpp\entry.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\svncpp\exception.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\svncpp\info.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\svncpp\log_entry.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\svncpp\path.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\svncpp\pool.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\svncpp\property.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\svncpp\revision.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\svncpp\status.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\svncpp\status_selection.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\svncpp\targets.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\svncpp\url.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\svncpp\wc.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\include\svncpp\annotate_line.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\include\svncpp\apr.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\include\svncpp\check.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\include\svncpp\client.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\include\svncpp\context.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\include\svncpp\context_listener.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\include\svncpp\datetime.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\include\svncpp\dirent.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\include\svncpp\entry.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\include\svncpp\exception.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\include\svncpp\info.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\include\svncpp\log_entry.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\include\svncpp\path.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\include\svncpp\pool.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\include\svncpp\property.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\include\svncpp\revision.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\include\svncpp\status.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\include\svncpp\status_selection.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\include\svncpp\targets.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\include\svncpp\url.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\include\svncpp\wc.hpp
# End Source File
# End Group
# Begin Group "Private Header Files"

# PROP Default_Filter "hpp"
# Begin Source File

SOURCE=..\..\..\src\svncpp\m_check.hpp
# End Source File
# End Group
# End Target
# End Project
