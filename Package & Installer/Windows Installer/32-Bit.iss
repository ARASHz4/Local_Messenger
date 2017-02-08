#define MyAppName "Local Messenger (32-Bit)"
#define MyAppVersion "0.6.5"
#define MyAppPublisher "ARASHz4"
#define MyAppURL "https://github.com/ARASHz4/Local_Messenger"
#define MyAppExeName "Local_Messenger.exe"

[Setup]
AppId={{08B5BF73-A9F0-40FC-B838-A4D21AB37913}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\Local Messenger
ArchitecturesAllowed=x86
MinVersion=6.0.6000
DefaultGroupName=Local Messenger
DisableProgramGroupPage=yes
DisableWelcomePage=no
OutputDir=K:\Qt\Local_Messenger\Binary\Windows\Windows Installer
OutputBaseFilename=32-Bit
LicenseFile=License Agreement.rtf
WizardImageFile=WizImage.bmp
WizardSmallImageFile=WizSmallImage.bmp
SetupIconFile=IconSetup.ico
UninstallDisplayIcon={app}\Local_Messenger.exe
UninstallDisplayName={#MyAppName}
Compression=lzma
SolidCompression=yes

[Code]
procedure InitializeWizard();
begin
  WizardForm.Left := WizardForm.Left -50;
  WizardForm.Height := WizardForm.Height + 40;
  WizardForm.Width := WizardForm.Width + 100;

  WizardForm.OuterNotebook.Height := WizardForm.OuterNotebook.Height +46;
  WizardForm.OuterNotebook.Width := WizardForm.OuterNotebook.Width +100;
  WizardForm.MainPanel.Width := WizardForm.MainPanel.Width+100;
  WizardForm.Bevel1.Width := WizardForm.Bevel1.Width +100;
  WizardForm.CancelButton.Top := WizardForm.CancelButton.Top +41;
  WizardForm.CancelButton.Left := WizardForm.CancelButton.Left +100;
  WizardForm.CancelButton.Height := WizardForm.CancelButton.Height +2;
  WizardForm.NextButton.Top := WizardForm.NextButton.Top +41;
  WizardForm.NextButton.Left := WizardForm.NextButton.Left +100;
  WizardForm.NextButton.Height := WizardForm.NextButton.Height +2;
  WizardForm.BackButton.Top := WizardForm.BackButton.Top +41;
  WizardForm.BackButton.Left := WizardForm.BackButton.Left +100;
  WizardForm.BackButton.Height := WizardForm.BackButton.Height +2;
  WizardForm.InnerPage.Width := WizardForm.InnerPage.Width + 100;
  WizardForm.Bevel.Top := WizardForm.Bevel.Top +46;
  WizardForm.Bevel.Width := WizardForm.Bevel.Width +100;
  WizardForm.WizardBitmapImage.Height := WizardForm.WizardBitmapImage.Height + 46;
  WizardForm.WizardBitmapImage2.Height := WizardForm.WizardBitmapImage2.Height + 46;
  WizardForm.WizardSmallBitmapImage.Left := WizardForm.WizardSmallBitmapImage.Left +100;

  WizardForm.InnerNotebook.Width :=  WizardForm.InnerNotebook.Width + 100;
  WizardForm.LicensePage.Width := WizardForm.LicensePage.Width + 100;
  WizardForm.LicenseMemo.Width := WizardForm.LicenseMemo.Width + 100;
  WizardForm.InnerPage.Height := WizardForm.InnerPage.Height + 40;
  WizardForm.InnerNotebook.Height :=  WizardForm.InnerNotebook.Height + 40;
  WizardForm.LicenseMemo.Height := WizardForm.LicenseMemo.Height + 40;
  WizardForm.LicenseNotAcceptedRadio.Top := WizardForm.LicenseNotAcceptedRadio.Top + 40;
  WizardForm.LicenseAcceptedRadio.Top := WizardForm.LicenseAcceptedRadio.Top + 40;
  WizardForm.ProgressGauge.Width := WizardForm.ProgressGauge.Width + 100;
  WizardForm.ReadyMemo.Width := WizardForm.ReadyMemo.Width +100;
  WizardForm.ReadyMemo.Height := WizardForm.ReadyMemo.Height +40;
  WizardForm.DirEdit.Width :=  WizardForm.DirEdit.Width +100;
  WizardForm.DiskSpaceLabel.Top := WizardForm.DiskSpaceLabel.Top+40;
  WizardForm.DirBrowseButton.Left := WizardForm.DirBrowseButton.Left +100;

  WizardForm.WelcomeLabel1.Width := WizardForm.WelcomeLabel1.Width +90;
  WizardForm.WelcomeLabel2.Width := WizardForm.WelcomeLabel1.Width +20;
  WizardForm.FinishedHeadingLabel.Width := WizardForm.FinishedHeadingLabel.Width +90;
  WizardForm.FinishedLabel.Width := WizardForm.FinishedLabel.Width +20; 
  WizardForm.LicenseLabel1.Width := WizardForm.LicenseLabel1.Width +100;

  WizardForm.PageDescriptionLabel.Width :=0;
  WizardForm.PageNameLabel.Top := WizardForm.PageNameLabel.Top +12;
end;
  
  procedure CurPageChanged(CurPageID: Integer);
begin
  if CurpageID = wpFinished then
  begin
    WizardForm.NextButton.Left := WizardForm.CancelButton.Left;
  end
end;

[LangOptions]
  WelcomeFontSize=9

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "persian"; MessagesFile: "compiler:Languages\Persian.islu"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}";

[Files]
Source: "K:\Qt\Local_Messenger\Binary\Windows\32-Bit\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

