; Inno Setup script file
; Program: RapidSVN

[Setup]
AppName=RapidSVN
AppVerName=RapidSVN Alpha 3
AppPublisherURL=http://rapidsvn.tigris.org/
AppSupportURL=http://rapidsvn.tigris.org/
AppUpdatesURL=http://rapidsvn.tigris.org/
DefaultDirName={pf}\RapidSVN
DefaultGroupName=RapidSVN
LicenseFile=..\..\LICENSE.txt
InfoBeforeFile=..\..\README
;REMOVE InfoAfterFile=..\..\TODO.txt
OutputBaseFilename=RapidSVN-0.1.3

[Tasks]
Name: "desktopicon"; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"

[Components]
Name: main; Description: RapidSVN Application; Types: compact custom full; Flags: fixed
;REMOVE Name: code; Description: Source Code; Types: full compact

[Files]
Source: "libdb40.dll"; DestDir: "{app}\bin"; CopyMode: alwaysoverwrite
Source: "..\..\Release\rapidsvn.exe"; DestDir: "{app}\bin"; CopyMode: alwaysoverwrite
Source: "..\..\src\locale\de\rapidsvn.mo"; DestDir: "{app}\bin\locale\de"; CopyMode: alwaysoverwrite
Source: "..\..\TODO.txt"; DestDir: "{app}"; CopyMode: alwaysoverwrite
Source: "..\..\README"; DestDir: "{app}"; CopyMode: alwaysoverwrite
Source: "..\..\LICENSE.txt"; DestDir: "{app}"; CopyMode: alwaysoverwrite

;REMOVE Source: "..\..\src\*.*"; DestDir: "{app}\src"; CopyMode: alwaysoverwrite; Components: code
;REMOVE Source: "..\..\src\res\*.*"; DestDir: "{app}\src\res"; CopyMode: alwaysoverwrite; Components: code
;REMOVE Source: "..\..\src\res\bitmaps\*.*"; DestDir: "{app}\src\res\bitmaps"; CopyMode: alwaysoverwrite; Components: code
;REMOVE Source: "..\..\rapidsvn.dsp"; DestDir: "{app}"; CopyMode: alwaysoverwrite; Components: code
;REMOVE Source: "..\..\rapidsvn.dsw"; DestDir: "{app}"; CopyMode: alwaysoverwrite; Components: code

[Icons]
Name: "{group}\RapidSVN"; Filename: "{app}\bin\rapidsvn.exe"
Name: "{userdesktop}\RapidSVN"; Filename: "{app}\bin\rapidsvn.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\bin\rapidsvn.exe"; Description: "Launch RapidSVN"; Flags: nowait postinstall skipifsilent

