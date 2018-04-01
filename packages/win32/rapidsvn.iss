; Inno Setup script file
; Program: RapidSVN
;
; REMARKS: Requires ISPP or InnoSetup 5.4 and higher.
; Assumes that the %RAPIDSVN\..\dist directory contains all
; the binary files and translations. 

#define FullVersion "0.13.0.4"
#define Version "0.13"

[Setup]
AppName=RapidSVN-{#FullVersion}
AppVerName=RapidSVN-{#FullVersion}
AppPublisherURL=http://rapidsvn.tigris.org/
AppSupportURL=http://rapidsvn.tigris.org/
AppUpdatesURL=http://rapidsvn.tigris.org/
DefaultDirName={pf}\RapidSVN-{#Version}
DefaultGroupName=RapidSVN-{#Version}
LicenseFile=..\..\GPL.txt
InfoBeforeFile=..\..\README
OutputBaseFilename=RapidSVN-{#FullVersion}
MinVersion=5.1
VersionInfoVersion={#FullVersion}

[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl"
Name: "de"; MessagesFile: "compiler:Languages\German.isl"
Name: "fr"; MessagesFile: "compiler:Languages\French.isl"
Name: "it"; MessagesFile: "compiler:Languages\Italian.isl"
Name: "pt_BR"; MessagesFile: "compiler:Languages\BrazilianPortuguese.isl"
Name: "ru"; MessagesFile: "compiler:Languages\Russian.isl"
Name: "es"; MessagesFile: "compiler:Languages\Spanish.isl"
Name: "uk"; MessagesFile: "compiler:Languages\Ukrainian.isl"
Name: "zh_CN"; MessagesFile: "ChineseSimplified-5.5.9.isl"
Name: "ja"; MessagesFile: "compiler:Languages\Japanese.isl"

[Tasks]
Name: "desktopicon"; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"

[Components]
Name: main; Description: RapidSVN Application; Types: compact custom full; Flags: fixed
Name: svn; Description: Subversion Command Line; Types: custom full


[Files]
; RapidSVN application files
Source: "..\..\..\dist\rapidsvn.exe"; DestDir: "{app}\bin"; Flags: ignoreversion;
Source: "..\..\..\dist\locale\de\rapidsvn.po"; DestDir: "{app}\bin\locale\de"; Flags: ignoreversion; Components: main
Source: "..\..\..\dist\locale\de\rapidsvn.mo"; DestDir: "{app}\bin\locale\de"; Flags: ignoreversion; Components: main
Source: "..\..\..\dist\locale\es\rapidsvn.po"; DestDir: "{app}\bin\locale\es"; Flags: ignoreversion; Components: main
Source: "..\..\..\dist\locale\es\rapidsvn.mo"; DestDir: "{app}\bin\locale\es"; Flags: ignoreversion; Components: main
Source: "..\..\..\dist\locale\fr\rapidsvn.po"; DestDir: "{app}\bin\locale\fr"; Flags: ignoreversion; Components: main
Source: "..\..\..\dist\locale\fr\rapidsvn.mo"; DestDir: "{app}\bin\locale\fr"; Flags: ignoreversion; Components: main
Source: "..\..\..\dist\locale\hu\rapidsvn.po"; DestDir: "{app}\bin\locale\hu"; Flags: ignoreversion; Components: main
Source: "..\..\..\dist\locale\hu\rapidsvn.mo"; DestDir: "{app}\bin\locale\hu"; Flags: ignoreversion; Components: main
Source: "..\..\..\dist\locale\it_IT\rapidsvn.po"; DestDir: "{app}\bin\locale\it_IT"; Flags: ignoreversion; Components: main
Source: "..\..\..\dist\locale\it_IT\rapidsvn.mo"; DestDir: "{app}\bin\locale\it_IT"; Flags: ignoreversion; Components: main
Source: "..\..\..\dist\locale\ja\rapidsvn.po"; DestDir: "{app}\bin\locale\ja"; Flags: ignoreversion; Components: main
Source: "..\..\..\dist\locale\ja\rapidsvn.mo"; DestDir: "{app}\bin\locale\ja"; Flags: ignoreversion; Components: main
Source: "..\..\..\dist\locale\pt_BR\rapidsvn.po"; DestDir: "{app}\bin\locale\pt_BR"; Flags: ignoreversion; Components: main
Source: "..\..\..\dist\locale\pt_BR\rapidsvn.mo"; DestDir: "{app}\bin\locale\pt_BR"; Flags: ignoreversion; Components: main
Source: "..\..\..\dist\locale\ru\rapidsvn.po"; DestDir: "{app}\bin\locale\ru"; Flags: ignoreversion; Components: main
Source: "..\..\..\dist\locale\ru\rapidsvn.mo"; DestDir: "{app}\bin\locale\ru"; Flags: ignoreversion; Components: main
Source: "..\..\..\dist\locale\uk\rapidsvn.po"; DestDir: "{app}\bin\locale\uk"; Flags: ignoreversion; Components: main
Source: "..\..\..\dist\locale\uk\rapidsvn.mo"; DestDir: "{app}\bin\locale\uk"; Flags: ignoreversion; Components: main
Source: "..\..\..\dist\locale\zh_CN\rapidsvn.po"; DestDir: "{app}\bin\locale\zh_CN"; Flags: ignoreversion; Components: main
Source: "..\..\..\dist\locale\zh_CN\rapidsvn.mo"; DestDir: "{app}\bin\locale\zh_CN"; Flags: ignoreversion; Components: main
Source: "..\..\README"; DestDir: "{app}"; Flags: ignoreversion; Components: main
Source: "..\..\LICENSE.txt"; DestDir: "{app}"; Flags: ignoreversion; Components: main
Source: "..\..\GPL.txt"; DestDir: "{app}"; Flags: ignoreversion; Components: main
Source: "..\..\FDL.txt"; DestDir: "{app}"; Flags: ignoreversion; Components: main

; Subversion command line
Source: "..\..\..\dist\svn.exe"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: svn
Source: "..\..\..\dist\svnadmin.exe"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: svn

; Shared libraries
Source: "..\..\..\dist\*.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: main
Source: "..\..\..\dist\iconv\*.so"; DestDir: "{app}\bin\iconv"; Flags: ignoreversion; Components: main

; Visual Studio runtimes
Source: "vcredist_x86.exe"; DestDir: "{tmp}"; Flags: ignoreversion deleteafterinstall; Components: main

; Installation of Visual Studio runtimes
#define VCmsg "Installing Microsoft Visual C++ Redistributable...."
[Run]
Filename: "{tmp}\vcredist_x86.exe"; Parameters: "/Q"; StatusMsg: "{#VCmsg}"


[Icons]
Name: "{group}\RapidSVN"; Filename: "{app}\bin\rapidsvn.exe"; WorkingDir: "{app}\bin"; MinVersion: 0,4.0; Components: main
Name: "{userdesktop}\RapidSVN-0.13"; Filename: "{app}\bin\rapidsvn.exe"; WorkingDir: "{app}\bin"; Tasks: desktopicon; MinVersion: 0,4.0; Components: main

[Run]
Filename: "{app}\bin\rapidsvn.exe"; Description: "Launch RapidSVN"; WorkingDir: "{app}\bin"; Flags: nowait postinstall skipifsilent; MinVersion: 0,4.0; Components: main

