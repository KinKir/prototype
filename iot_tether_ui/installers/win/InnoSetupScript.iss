; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "InteractiveTetherHost"
#define MyAppVersion "1.0"
#define MyAppPublisher "BBC"
#define MyAppExeName "InteractiveTetherHost.exe"


[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{814F44C7-CAF0-47DA-8044-BBDB2CF103DE}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
OutputDir=C:\Users\IEUser\Desktop\New folder
OutputBaseFilename=setup
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: "C:\Users\IEUser\Documents\GitHub\microbug\iot_tether_ui\installers\win\PY2EXE\dist\InteractiveTetherHost.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\IEUser\Documents\GitHub\microbug\iot_tether_ui\installers\win\PY2EXE\dist\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

; NOTE: Don't use "Flags: ignoreversion" on any shared system files
Source: "C:\Users\IEUser\Documents\GitHub\microbug\iot_tether_ui\installers\win\usb_driver\*"; DestDir: "{app}\usb_driver"; Flags: ignoreversion createallsubdirs recursesubdirs; MinVersion: 0,6.0
Source: "C:\Users\IEUser\Documents\GitHub\microbug\iot_tether_ui\installers\win\usb_driver_xp\*"; DestDir: "{app}\usb_driver"; Flags: ignoreversion createallsubdirs recursesubdirs; OnlyBelowVersion: 0,6.0

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Flags: nowait postinstall skipifsilent; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"
Filename: "{app}\usb_driver\dpinst_x86.exe"; WorkingDir: "{app}\usb_driver"; Flags: waituntilterminated; Description: "Installing Drivers"; StatusMsg: "Installing Drivers"; Check: not IsWin64
Filename: "{app}\usb_driver\dpinst_x64.exe"; WorkingDir: "{app}\usb_driver"; Flags: waituntilterminated; Description: "Installing Drivers"; StatusMsg: "Installing Drivers"; Check: IsWin64

