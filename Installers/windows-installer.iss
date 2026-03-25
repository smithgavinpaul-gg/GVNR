#define MyAppName "GVNRs Grunger"
#define MyAppVersion "1.0.0"
#define MyAppPublisher "GVNR Audio"
#define MyAppURL "https://github.com/gvnraudio/GVNR"

[Setup]
AppId={{B7F3A2E1-4D8C-4F9A-B6E2-1A3C5D7E9F0B}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
DefaultDirName={pf}\GVNR Audio\GVNRs Grunger
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes
OutputBaseFilename=GVNRs-Grunger-Installer-v{#MyAppVersion}
Compression=lzma2
SolidCompression=yes
WizardStyle=modern
PrivilegesRequired=admin
ArchitecturesAllowed=x64
ArchitecturesInstallMode=x64

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Types]
Name: "full"; Description: "Full installation (VST3 + Standalone)"
Name: "vst3only"; Description: "VST3 plugin only"
Name: "custom"; Description: "Custom installation"; Flags: iscustom

[Components]
Name: "vst3"; Description: "VST3 Plugin (for FL Studio, Ableton, etc.)"; Types: full vst3only custom; Flags: fixed
Name: "standalone"; Description: "Standalone Application"; Types: full custom

[Files]
Source: "..\build\TheGVNR_artefacts\Release\VST3\GVNRs Grunger.vst3\*"; DestDir: "{cf}\VST3\GVNRs Grunger.vst3"; Components: vst3; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\build\TheGVNR_artefacts\Release\Standalone\GVNRs Grunger.exe"; DestDir: "{app}"; Components: standalone; Flags: ignoreversion

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\GVNRs Grunger.exe"; Components: standalone
Name: "{group}\Uninstall {#MyAppName}"; Filename: "{uninstallexe}"

[Messages]
WelcomeLabel2=This will install {#MyAppName} v{#MyAppVersion} on your computer.%n%n{#MyAppName} is a hybrid distortion and multi-effect audio plugin featuring 30 distortion algorithms, 24 morphing filters, melodic feedback, multiband crossover, and deep modulation.%n%nThe VST3 plugin will be installed to the standard VST3 folder where FL Studio and other DAWs will find it automatically.

[Run]
Filename: "{app}\GVNRs Grunger.exe"; Description: "Launch {#MyAppName}"; Flags: nowait postinstall skipifsilent; Components: standalone
