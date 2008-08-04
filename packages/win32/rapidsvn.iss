; Inno Setup script file
; Program: RapidSVN
;
; REMARKS: run the batch FetchFiles.bat
;          before running this script

[Setup]
AppName=RapidSVN-0.9.7
AppVerName=RapidSVN-0.9.7
AppPublisherURL=http://rapidsvn.tigris.org/
AppSupportURL=http://rapidsvn.tigris.org/
AppUpdatesURL=http://rapidsvn.tigris.org/
DefaultDirName={pf}\RapidSVN-0.9.7
DefaultGroupName=RapidSVN-0.9.7
LicenseFile=..\..\GPL.txt
InfoBeforeFile=..\..\README
OutputBaseFilename=RapidSVN-0.9.7

[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl"
Name: "de"; MessagesFile: "compiler:Languages\German.isl"
Name: "fr"; MessagesFile: "compiler:Languages\French.isl"
Name: "it"; MessagesFile: "compiler:Languages\Italian.isl"
Name: "pt_BR"; MessagesFile: "compiler:Languages\BrazilianPortuguese.isl"
Name: "ru"; MessagesFile: "compiler:Languages\Russian.isl"
Name: "es"; MessagesFile: "SpanishStd-5-5.1.11.isl"
Name: "uk"; MessagesFile: "Ukrainian-6-5.1.11.isl"
Name: "zh_CN"; MessagesFile: "ChineseSimp-11-5.1.0.isl"
Name: "ja"; MessagesFile: "Japanese-5-5.1.11.isl"

[Tasks]
Name: "desktopicon"; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"

[Components]
; After we remove "rapidsvn_svn1_4" add "Flags: fixed" for rapidsvn
Name: rapidsvn; Description: RapidSVN Application; Types: compact custom full;
Name: rapidsvn_svn1_4; Description: RapidSVN Application (for Subversion 1.4.x); Types: compact custom full

[Files]
; RapidSVN application files
Source: "..\..\Release_rapidsvn\rapidsvn.exe"; DestDir: "{app}\bin"; Flags: ignoreversion; OnlyBelowVersion: 0,4.0; Components: rapidsvn
Source: "..\..\Unicode_Release_rapidsvn\rapidsvn.exe"; DestDir: "{app}\bin"; Flags: ignoreversion; MinVersion: 0,4.0; Components: rapidsvn
Source: "..\..\Release_rapidsvn_svn1_4\rapidsvn_svn1_4.exe"; DestDir: "{app}\bin"; Flags: ignoreversion; OnlyBelowVersion: 0,4.0; Components: rapidsvn_svn1_4
Source: "..\..\Unicode_Release_rapidsvn_svn1_4\rapidsvn_svn1_4.exe"; DestDir: "{app}\bin"; Flags: ignoreversion; MinVersion: 0,4.0; Components: rapidsvn_svn1_4
Source: "..\..\src\locale\de\rapidsvn.po"; DestDir: "{app}\bin\locale\de"; Flags: ignoreversion; Components: rapidsvn rapidsvn_svn1_4
Source: "..\..\src\locale\de\rapidsvn.mo"; DestDir: "{app}\bin\locale\de"; Flags: ignoreversion; Components: rapidsvn rapidsvn_svn1_4
Source: "..\..\src\locale\fr\rapidsvn.po"; DestDir: "{app}\bin\locale\fr"; Flags: ignoreversion; Components: rapidsvn rapidsvn_svn1_4
Source: "..\..\src\locale\fr\rapidsvn.mo"; DestDir: "{app}\bin\locale\fr"; Flags: ignoreversion; Components: rapidsvn rapidsvn_svn1_4
Source: "..\..\src\locale\it_IT\rapidsvn.po"; DestDir: "{app}\bin\locale\it_IT"; Flags: ignoreversion; Components: rapidsvn rapidsvn_svn1_4
Source: "..\..\src\locale\it_IT\rapidsvn.mo"; DestDir: "{app}\bin\locale\it_IT"; Flags: ignoreversion; Components: rapidsvn rapidsvn_svn1_4
Source: "..\..\src\locale\pt_BR\rapidsvn.po"; DestDir: "{app}\bin\locale\pt_BR"; Flags: ignoreversion; Components: rapidsvn rapidsvn_svn1_4
Source: "..\..\src\locale\pt_BR\rapidsvn.mo"; DestDir: "{app}\bin\locale\pt_BR"; Flags: ignoreversion; Components: rapidsvn rapidsvn_svn1_4
Source: "..\..\src\locale\ru\rapidsvn.po"; DestDir: "{app}\bin\locale\ru"; Flags: ignoreversion; Components: rapidsvn rapidsvn_svn1_4
Source: "..\..\src\locale\ru\rapidsvn.mo"; DestDir: "{app}\bin\locale\ru"; Flags: ignoreversion; Components: rapidsvn rapidsvn_svn1_4
Source: "..\..\src\locale\es\rapidsvn.po"; DestDir: "{app}\bin\locale\es"; Flags: ignoreversion; Components: rapidsvn rapidsvn_svn1_4
Source: "..\..\src\locale\es\rapidsvn.mo"; DestDir: "{app}\bin\locale\es"; Flags: ignoreversion; Components: rapidsvn rapidsvn_svn1_4
Source: "..\..\src\locale\uk\rapidsvn.po"; DestDir: "{app}\bin\locale\uk"; Flags: ignoreversion; Components: rapidsvn rapidsvn_svn1_4
Source: "..\..\src\locale\uk\rapidsvn.mo"; DestDir: "{app}\bin\locale\uk"; Flags: ignoreversion; Components: rapidsvn rapidsvn_svn1_4
Source: "..\..\src\locale\zh_CN\rapidsvn.po"; DestDir: "{app}\bin\locale\zh_CN"; Flags: ignoreversion; Components: rapidsvn rapidsvn_svn1_4
Source: "..\..\src\locale\zh_CN\rapidsvn.mo"; DestDir: "{app}\bin\locale\zh_CN"; Flags: ignoreversion; Components: rapidsvn rapidsvn_svn1_4
Source: "..\..\src\locale\ja\rapidsvn.po"; DestDir: "{app}\bin\locale\ja"; Flags: ignoreversion; Components: rapidsvn rapidsvn_svn1_4
Source: "..\..\src\locale\ja\rapidsvn.mo"; DestDir: "{app}\bin\locale\ja"; Flags: ignoreversion; Components: rapidsvn rapidsvn_svn1_4
Source: "..\..\README"; DestDir: "{app}"; Flags: ignoreversion; Components: rapidsvn rapidsvn_svn1_4
Source: "..\..\LICENSE.txt"; DestDir: "{app}"; Flags: ignoreversion; Components: rapidsvn rapidsvn_svn1_4
Source: "..\..\GPL.txt"; DestDir: "{app}"; Flags: ignoreversion; Components: rapidsvn rapidsvn_svn1_4
Source: "..\..\FDL.txt"; DestDir: "{app}"; Flags: ignoreversion; Components: rapidsvn rapidsvn_svn1_4

; Microsoft Runtime DLL
Source: "msvcp60.dll"; DestDir: "{app}\bin"; Flags: onlyifdoesntexist; Components: rapidsvn rapidsvn_svn1_4

; Berkeley DB
Source: "tmp\libdb44.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: rapidsvn rapidsvn_svn1_4

; APR
Source: "tmp\libapr-1.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: rapidsvn
Source: "tmp\libapriconv-1.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: rapidsvn
Source: "tmp\libaprutil-1.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: rapidsvn
; The iconv files for Subversion 1.4.x and Subversion 1.5.x are not compatible
; we can uncomment this after we remove RapidSVN for Subversion 1.4.x
; Source: "tmp\*.so"; DestDir: "{app}\bin\iconv"; Flags: ignoreversion; Components: rapidsvn rapidsvn_svn1_4

; APR for Subversion 1.4
Source: "tmp\libapr.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: rapidsvn_svn1_4
Source: "tmp\libapriconv.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: rapidsvn_svn1_4
Source: "tmp\libaprutil.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: rapidsvn_svn1_4

; LibIntl
Source: "tmp\intl3_svn.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: rapidsvn_svn1_4

[Icons]
Name: "{group}\RapidSVN"; Filename: "{app}\bin\rapidsvn.exe"; WorkingDir: "{app}\bin"; Components: rapidsvn
Name: "{userdesktop}\RapidSVN-0.9.7"; Filename: "{app}\bin\rapidsvn.exe"; WorkingDir: "{app}\bin"; Tasks: desktopicon; Components: rapidsvn
Name: "{group}\RapidSVN (for Subversion 1.4.x)"; Filename: "{app}\bin\rapidsvn_svn1_4.exe"; WorkingDir: "{app}\bin"; Components: rapidsvn_svn1_4
Name: "{userdesktop}\RapidSVN-0.9.7 (for Subversion 1.4.x)"; Filename: "{app}\bin\rapidsvn_svn1_4.exe"; WorkingDir: "{app}\bin"; Tasks: desktopicon; Components: rapidsvn_svn1_4

[Run]
Filename: "{app}\bin\rapidsvn.exe"; Description: "Launch RapidSVN"; WorkingDir: "{app}\bin"; Flags: nowait postinstall skipifsilent; Components: rapidsvn
Filename: "{app}\bin\rapidsvn_svn1_4.exe"; Description: "Launch RapidSVN (for Subversion 1.4.x)"; WorkingDir: "{app}\bin"; Flags: nowait postinstall skipifsilent; Components: rapidsvn_svn1_4

