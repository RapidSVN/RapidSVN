; Inno Setup script file
; Program: RapidSVN

[Setup]
AppName=RapidSVN
AppVerName=RapidSVN 0.7.1
AppPublisherURL=http://rapidsvn.tigris.org/
AppSupportURL=http://rapidsvn.tigris.org/
AppUpdatesURL=http://rapidsvn.tigris.org/
DefaultDirName={pf}\RapidSVN
DefaultGroupName=RapidSVN
LicenseFile=..\..\LICENSE.txt
InfoBeforeFile=..\..\README
OutputBaseFilename=RapidSVN-0.7.1

[Tasks]
Name: "desktopicon"; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"

[Components]
Name: main; Description: RapidSVN Application; Types: compact custom full; Flags: fixed

[Files]
Source: "libdb42.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: main
Source: "msvcp60.dll"; DestDir: "{app}\bin"; Flags: onlyifdoesntexist; Components: main
Source: "libeay32.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: main
Source: "ssleay32.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: main
Source: "..\..\Release\rapidsvn.exe"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: main
Source: "..\..\src\locale\de\rapidsvn.mo"; DestDir: "{app}\bin\locale\de"; Flags: ignoreversion; Components: main
Source: "..\..\TODO.txt"; DestDir: "{app}"; Flags: ignoreversion; Components: main
Source: "..\..\README"; DestDir: "{app}"; Flags: ignoreversion; Components: main
Source: "..\..\LICENSE.txt"; DestDir: "{app}"; Flags: ignoreversion; Components: main
Source: "libapr.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: main
Source: "libapriconv.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: main
Source: "libaprutil.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: main

[Icons]
Name: "{group}\RapidSVN"; Filename: "{app}\bin\rapidsvn.exe"; WorkingDir: "{app}\bin";
Name: "{userdesktop}\RapidSVN"; Filename: "{app}\bin\rapidsvn.exe"; WorkingDir: "{app}\bin"; Tasks: desktopicon

[Run]
Filename: "{app}\bin\rapidsvn.exe"; Description: "Launch RapidSVN"; WorkingDir: "{app}\bin"; Flags: nowait postinstall skipifsilent

