; Inno Setup script file
; Program: RapidSVN

[Setup]
AppName=RapidSVN
AppVerName=RapidSVN Alpha 1
AppPublisherURL=http://rapidsvn.tigris.org/
AppSupportURL=http://rapidsvn.tigris.org/
AppUpdatesURL=http://rapidsvn.tigris.org/
DefaultDirName={pf}\RapidSVN
DefaultGroupName=RapidSVN
LicenseFile=..\..\LICENSE.txt
InfoBeforeFile=..\..\README.txt
InfoAfterFile=..\..\TODO.txt
OutputBaseFilename=RapidSVN-alpha

[Tasks]
Name: "desktopicon"; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"

[Components]
Name: main; Description: Application files; Types: full compact custom; Flags: fixed
Name: code; Description: Source Code; Types: full compact

[Files]
Source: "libdb40.dll"; DestDir: "{app}\bin"; CopyMode: alwaysoverwrite
Source: "..\..\Release\rapidsvn.exe"; DestDir: "{app}\bin"; CopyMode: alwaysoverwrite
Source: "..\..\TODO.txt"; DestDir: "{app}"; CopyMode: alwaysoverwrite
Source: "..\..\README.txt"; DestDir: "{app}"; CopyMode: alwaysoverwrite
Source: "..\..\LICENSE.txt"; DestDir: "{app}"; CopyMode: alwaysoverwrite

Source: "..\..\src\*.*"; DestDir: "{app}\src"; CopyMode: alwaysoverwrite; Components: code
Source: "..\..\src\res\*.*"; DestDir: "{app}\src\res"; CopyMode: alwaysoverwrite; Components: code
Source: "..\..\src\res\bitmaps\*.*"; DestDir: "{app}\src\res\bitmaps"; CopyMode: alwaysoverwrite; Components: code
Source: "..\..\rapidsvn.dsp"; DestDir: "{app}"; CopyMode: alwaysoverwrite; Components: code
Source: "..\..\rapidsvn.dsw"; DestDir: "{app}"; CopyMode: alwaysoverwrite; Components: code

[Icons]
Name: "{group}\RapidSVN"; Filename: "{app}\bin\rapidsvn.exe"
Name: "{group}\TODO"; Filename: "{app}\TODO.txt"
Name: "{group}\README"; Filename: "{app}\README.txt"
Name: "{group}\Uninstall RapidSVN"; Filename: "{uninstallexe}"
Name: "{userdesktop}\RapidSVN"; Filename: "{app}\bin\rapidsvn.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\bin\rapidsvn.exe"; Description: "Launch RapidSVN"; Flags: nowait postinstall skipifsilent

