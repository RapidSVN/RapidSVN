; Inno Setup script file
; Program: RapidSVN

[Setup]
AppName=RapidSVN
AppVerName=RapidSVN 0.4.0
AppPublisherURL=http://rapidsvn.tigris.org/
AppSupportURL=http://rapidsvn.tigris.org/
AppUpdatesURL=http://rapidsvn.tigris.org/
DefaultDirName={pf}\RapidSVN
DefaultGroupName=RapidSVN
LicenseFile=..\..\LICENSE.txt
InfoBeforeFile=..\..\README
OutputBaseFilename=RapidSVN-0.4.0

[Tasks]
Name: "desktopicon"; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"

[Components]
Name: main; Description: RapidSVN Application; Types: compact custom full; Flags: fixed

[Files]
Source: "libdb42.dll"; DestDir: "{app}\bin"; CopyMode: alwaysoverwrite; Components: main
Source: "msvcp60.dll"; DestDir: "{app}\bin"; CopyMode: onlyifdoesntexist; Components: main
Source: "libeay32.dll"; DestDir: "{app}\bin"; CopyMode: alwaysoverwrite; Components: main
Source: "ssleay32.dll"; DestDir: "{app}\bin"; CopyMode: alwaysoverwrite; Components: main
Source: "..\..\Release\rapidsvn.exe"; DestDir: "{app}\bin"; CopyMode: alwaysoverwrite; Components: main
Source: "..\..\src\locale\de\rapidsvn.mo"; DestDir: "{app}\bin\locale\de"; CopyMode: alwaysoverwrite; Components: main
Source: "..\..\TODO.txt"; DestDir: "{app}"; CopyMode: alwaysoverwrite; Components: main
Source: "..\..\README"; DestDir: "{app}"; CopyMode: alwaysoverwrite; Components: main
Source: "..\..\LICENSE.txt"; DestDir: "{app}"; CopyMode: alwaysoverwrite; Components: main

[Icons]
Name: "{group}\RapidSVN"; Filename: "{app}\bin\rapidsvn.exe"
Name: "{userdesktop}\RapidSVN"; Filename: "{app}\bin\rapidsvn.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\bin\rapidsvn.exe"; Description: "Launch RapidSVN"; Flags: nowait postinstall skipifsilent

