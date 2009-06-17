; Inno Setup script file
; Program: RapidSVN
;
; REMARKS: run the batch FetchFiles.bat
;          before running this script

[Setup]
AppName=RapidSVN-0.9.9
AppVerName=RapidSVN-0.9.9
AppPublisherURL=http://rapidsvn.tigris.org/
AppSupportURL=http://rapidsvn.tigris.org/
AppUpdatesURL=http://rapidsvn.tigris.org/
DefaultDirName={pf}\RapidSVN-0.9.9
DefaultGroupName=RapidSVN-0.9.9
LicenseFile=..\..\GPL.txt
InfoBeforeFile=..\..\README
OutputBaseFilename=RapidSVN-0.9.9

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
Name: main; Description: RapidSVN Application; Types: compact custom full; Flags: fixed

[Files]
; RapidSVN application files
Source: "tmp/rapidsvn_ansi.exe"; DestDir: "{app}\bin"; DestName: "rapidsvn.exe"; Flags: ignoreversion; OnlyBelowVersion: 0,4.0; Components: main
Source: "tmp/rapidsvn_unicode.exe"; DestDir: "{app}\bin"; DestName: "rapidsvn.exe"; Flags: ignoreversion; MinVersion: 0,4.0; Components: main
Source: "..\..\src\locale\de\rapidsvn.po"; DestDir: "{app}\bin\locale\de"; Flags: ignoreversion; Components: main
Source: "..\..\src\locale\de\rapidsvn.mo"; DestDir: "{app}\bin\locale\de"; Flags: ignoreversion; Components: main
Source: "..\..\src\locale\fr\rapidsvn.po"; DestDir: "{app}\bin\locale\fr"; Flags: ignoreversion; Components: main
Source: "..\..\src\locale\fr\rapidsvn.mo"; DestDir: "{app}\bin\locale\fr"; Flags: ignoreversion; Components: main
Source: "..\..\src\locale\it_IT\rapidsvn.po"; DestDir: "{app}\bin\locale\it_IT"; Flags: ignoreversion; Components: main
Source: "..\..\src\locale\it_IT\rapidsvn.mo"; DestDir: "{app}\bin\locale\it_IT"; Flags: ignoreversion; Components: main
Source: "..\..\src\locale\pt_BR\rapidsvn.po"; DestDir: "{app}\bin\locale\pt_BR"; Flags: ignoreversion; Components: main
Source: "..\..\src\locale\pt_BR\rapidsvn.mo"; DestDir: "{app}\bin\locale\pt_BR"; Flags: ignoreversion; Components: main
Source: "..\..\src\locale\ru\rapidsvn.po"; DestDir: "{app}\bin\locale\ru"; Flags: ignoreversion; Components: main
Source: "..\..\src\locale\ru\rapidsvn.mo"; DestDir: "{app}\bin\locale\ru"; Flags: ignoreversion; Components: main
Source: "..\..\src\locale\es\rapidsvn.po"; DestDir: "{app}\bin\locale\es"; Flags: ignoreversion; Components: main
Source: "..\..\src\locale\es\rapidsvn.mo"; DestDir: "{app}\bin\locale\es"; Flags: ignoreversion; Components: main
Source: "..\..\src\locale\uk\rapidsvn.po"; DestDir: "{app}\bin\locale\uk"; Flags: ignoreversion; Components: main
Source: "..\..\src\locale\uk\rapidsvn.mo"; DestDir: "{app}\bin\locale\uk"; Flags: ignoreversion; Components: main
Source: "..\..\src\locale\zh_CN\rapidsvn.po"; DestDir: "{app}\bin\locale\zh_CN"; Flags: ignoreversion; Components: main
Source: "..\..\src\locale\zh_CN\rapidsvn.mo"; DestDir: "{app}\bin\locale\zh_CN"; Flags: ignoreversion; Components: main
Source: "..\..\src\locale\ja\rapidsvn.po"; DestDir: "{app}\bin\locale\ja"; Flags: ignoreversion; Components: main
Source: "..\..\src\locale\ja\rapidsvn.mo"; DestDir: "{app}\bin\locale\ja"; Flags: ignoreversion; Components: main
Source: "..\..\README"; DestDir: "{app}"; Flags: ignoreversion; Components: main
Source: "..\..\LICENSE.txt"; DestDir: "{app}"; Flags: ignoreversion; Components: main
Source: "..\..\GPL.txt"; DestDir: "{app}"; Flags: ignoreversion; Components: main
Source: "..\..\FDL.txt"; DestDir: "{app}"; Flags: ignoreversion; Components: main

; Shared libraries
Source: "tmp\*.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: main
Source: "tmp\*.so"; DestDir: "{app}\bin\iconv"; Flags: ignoreversion; Components: main

[Icons]
Name: "{group}\RapidSVN"; Filename: "{app}\bin\rapidsvn.exe"; WorkingDir: "{app}\bin";
Name: "{userdesktop}\RapidSVN-0.9.9"; Filename: "{app}\bin\rapidsvn.exe"; WorkingDir: "{app}\bin"; Tasks: desktopicon

[Run]
Filename: "{app}\bin\rapidsvn.exe"; Description: "Launch RapidSVN"; WorkingDir: "{app}\bin"; Flags: nowait postinstall skipifsilent

