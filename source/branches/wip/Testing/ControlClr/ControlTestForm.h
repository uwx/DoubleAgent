#pragma once
#include "FormData.h"
#include "settings.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Configuration;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Diagnostics;
using namespace DoubleAgent::Control;

namespace DoubleAgent {
/////////////////////////////////////////////////////////////////////////////

/// <summary>
/// Summary for ControlTestForm
///
/// WARNING: If you change the name of this class, you will need to change the
///          'Resource File Name' property for the managed resource compiler tool
///          associated with all .resx files this class depends on.  Otherwise,
///          the designers will not be able to interact properly with localized
///          resources associated with this form.
/// </summary>
public ref class ControlTestForm : public System::Windows::Forms::Form
{
public:
	ControlTestForm (void);
protected:
	~ControlTestForm ();

//===========================================================================

public: DoubleAgent::AxControl::AxControl^  TestDaControl;

private: System::Windows::Forms::TabControl^  TabsMain;
private: System::Windows::Forms::TabPage^  PageMainCharacter;
private: System::Windows::Forms::TabPage^  PageMainControl;
private: System::Windows::Forms::TabControl^  TabsCharacter;
private: System::Windows::Forms::TabControl^  TabsControl;

private: System::Windows::Forms::ColorDialog^  ChooseColorDialog;
private: System::ComponentModel::IContainer^  components;

#pragma region Main Window Fields
private: System::Windows::Forms::CheckBox^  RaiseRequestErrorsButton;
private: System::Windows::Forms::CheckBox^  AutoConnectButton;
private: System::Windows::Forms::CheckBox^  ControlConnectedButton;
private: System::Windows::Forms::Button^  LoadButton;
private: System::Windows::Forms::Button^  UnloadButton;
private: System::Windows::Forms::Button^  SetCurrentButton;
private: System::Windows::Forms::Button^  CollectButton;
#pragma endregion

#pragma region Character Page Fields
private: DoubleAgent::CharacterData^ CharacterData;
private: System::Windows::Forms::BindingSource^  CharacterDataBinding;
private: System::Windows::Forms::BindingSource^  CharacterBindingSource;
private: System::Windows::Forms::TabPage^  PageCharacter;
private: System::Windows::Forms::NumericUpDown^  CharHeight;
private: System::Windows::Forms::NumericUpDown^  CharWidth;
private: System::Windows::Forms::NumericUpDown^  CharTop;
private: System::Windows::Forms::NumericUpDown^  CharLeft;
private: System::Windows::Forms::ComboBox^  CharMoveCause;
private: System::Windows::Forms::ComboBox^  CharVisibilityCause;
private: System::Windows::Forms::ComboBox^  CharListeningStatus;
private: System::Windows::Forms::ComboBox^  CharActiveState;
private: System::Windows::Forms::CheckBox^  CharHasOtherClients;
private: System::Windows::Forms::CheckBox^  CharHasIcon;
private: System::Windows::Forms::CheckBox^  CharAutoPopupMenu;
private: System::Windows::Forms::CheckBox^  CharIdleState;
private: System::Windows::Forms::CheckBox^  CharIdleEnabled;
private: System::Windows::Forms::CheckBox^  CharSoundEffectsEnabled;
private: System::Windows::Forms::CheckBox^  CharIconVisible;
private: System::Windows::Forms::CheckBox^  CharIconShown;
private: System::Windows::Forms::CheckBox^  CharVisible;
private: System::Windows::Forms::TextBox^  CharIconTip;
private: System::Windows::Forms::TextBox^  CharFilePath;
private: System::Windows::Forms::TextBox^  CharSRModeID;
private: System::Windows::Forms::TextBox^  CharTTSModeID;
private: System::Windows::Forms::TextBox^  CharIconIdentity;
private: System::Windows::Forms::TextBox^  CharFileName;
private: System::Windows::Forms::TextBox^  CharDescription;
private: System::Windows::Forms::TextBox^  CharVersion;
private: System::Windows::Forms::TextBox^  CharName;
private: System::Windows::Forms::TextBox^  CharUniqueID;
private: System::Windows::Forms::TextBox^  CharCharacterID;
private: System::Windows::Forms::TextBox^  CharLanguageID;
private: System::Windows::Forms::Button^  CharWidthDefault;
private: System::Windows::Forms::Button^  CharHeightDefault;
#pragma endregion

#pragma region Actions Page Fields
private: System::Windows::Forms::TabPage^  PageActions;
private: System::Windows::Forms::Button^  CharGestureAt;
private: System::Windows::Forms::Button^  CharMoveTo;
private: System::Windows::Forms::Button^  CharHide;
private: System::Windows::Forms::Button^  CharShow;
private: System::Windows::Forms::Button^  CharStopAll;
private: System::Windows::Forms::Button^  CharShowPopupMenu;
private: System::Windows::Forms::Button^  CharListen;
private: System::Windows::Forms::Button^  CharThink;
private: System::Windows::Forms::Button^  CharSpeak;
private: System::Windows::Forms::NumericUpDown^  CharMoveToTop;
private: System::Windows::Forms::NumericUpDown^  CharMoveToLeft;
private: System::Windows::Forms::CheckBox^  CharHideFast;
private: System::Windows::Forms::CheckBox^  CharShowFast;
private: System::Windows::Forms::NumericUpDown^  CharGestureAtX;
private: System::Windows::Forms::NumericUpDown^  CharGestureAtY;
private: System::Windows::Forms::TextBox^  CharThinkText;
private: System::Windows::Forms::TextBox^  CharSpeakText;
private: System::Windows::Forms::NumericUpDown^  CharShowMenuY;
private: System::Windows::Forms::NumericUpDown^  CharShowMenuX;
private: System::Windows::Forms::Button^  CharListenStop;
private: System::Windows::Forms::ListBox^  CharPlayAnimation;
private: System::Windows::Forms::Button^  CharPlay;
private: System::Windows::Forms::Button^  CharGenerateIcon;
#pragma endregion

#pragma region Balloon Page Fields
private: System::Windows::Forms::TabPage^  PageBalloon;
private: System::Windows::Forms::BindingSource^  BalloonBindingSource;
private: System::Windows::Forms::CheckBox^  CharBalloonAutoPace;
private: System::Windows::Forms::CheckBox^  CharBalloonAutoHide;
private: System::Windows::Forms::CheckBox^  CharBalloonSizeToText;
private: System::Windows::Forms::NumericUpDown^  CharBalloonNumberOfLines;
private: System::Windows::Forms::CheckBox^  CharBalloonVisible;
private: System::Windows::Forms::CheckBox^  CharBalloonEnabled;
private: System::Windows::Forms::CheckBox^  CharBalloonShowPartialLines;
private: System::Windows::Forms::TextBox^  CharBalloonBorderColorName;
private: System::Windows::Forms::TextBox^  CharBalloonTextColorName;
private: System::Windows::Forms::TextBox^  CharBalloonBackColorName;
private: System::Windows::Forms::CheckBox^  CharBalloonFontStrikeThru;
private: System::Windows::Forms::CheckBox^  CharBalloonFontUnderline;
private: System::Windows::Forms::CheckBox^  CharBalloonFontItalic;
private: System::Windows::Forms::CheckBox^  CharBalloonFontBold;
private: System::Windows::Forms::TextBox^  CharBalloonFontCharSet;
private: System::Windows::Forms::NumericUpDown^  CharBalloonFontSize;
private: System::Windows::Forms::TextBox^  CharBalloonFontName;
private: System::Windows::Forms::NumericUpDown^  CharBalloonCharsPerLine;
#pragma endregion

#pragma region Commands Page Fields
private: System::Windows::Forms::TabPage^  PageCommands;
private: System::Windows::Forms::BindingSource^  CommandsBindingSource;
private: System::Windows::Forms::BindingSource^  CommandBindingSource;
private: System::Windows::Forms::TextBox^  CharCommandsDefaultCommand;
private: System::Windows::Forms::NumericUpDown^  CharCommandsFontSize;
private: System::Windows::Forms::TextBox^  CharCommandsFontName;
private: System::Windows::Forms::TextBox^  CharCommandsVoiceGrammar;
private: System::Windows::Forms::TextBox^  CharCommandsVoiceCaption;
private: System::Windows::Forms::TextBox^  CharCommandsCaption;
private: System::Windows::Forms::CheckBox^  GlobalVoiceCommandsEnabled;
private: System::Windows::Forms::CheckBox^  CharCommandsVisible;
private: System::Windows::Forms::DataGridView^  CharCommands;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  CharCommandsNameCol;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  CharCommandsCaptionCol;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  CharCommandsVoiceCaptionCol;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  CharCommandsVoiceGrammarCol;
private: System::Windows::Forms::DataGridViewCheckBoxColumn^  CharCommandsEnabledCol;
private: System::Windows::Forms::DataGridViewCheckBoxColumn^  CharCommandsVisibleCol;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  CharCommandsConfidenceThresholdCol;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  CharCommandsConfidenceTextCol;
#pragma endregion

#pragma region Characters Page Fields
private: DoubleAgent::CharactersData^ CharactersData;
private: System::Windows::Forms::BindingSource^  CharactersDataBinding;
private: System::Windows::Forms::TabPage^  PageCharacters;
private: System::Windows::Forms::CheckBox^  CharactersBound;
private: System::Windows::Forms::DataGridView^  CharacterList;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  CharListID;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  CharListName;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  CharListFileName;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  CharListFilePath;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  CharListVersion;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  CharListUniqueID;
#pragma endregion

#pragma region Settings Page Fields
private: DoubleAgent::SettingsData^ SettingsData;
private: System::Windows::Forms::BindingSource^  SettingsDataBinding;
private: System::Windows::Forms::TabPage^  PageSettings;
private: System::Windows::Forms::CheckBox^  SettingsBound;
private: System::Windows::Forms::CheckBox^  BalloonEnabled;
private: System::Windows::Forms::TextBox^  BalloonFontName;
private: System::Windows::Forms::NumericUpDown^  BalloonFontSize;
private: System::Windows::Forms::CheckBox^  BalloonFontUnderline;
private: System::Windows::Forms::CheckBox^  BalloonFontItalic;
private: System::Windows::Forms::CheckBox^  BalloonFontBold;
private: System::Windows::Forms::CheckBox^  SoundEffectsEnabled;
private: System::Windows::Forms::CheckBox^  TTSEnabled;
private: System::Windows::Forms::TextBox^  TTSSpeed;
private: System::Windows::Forms::CheckBox^  SREnabled;
private: System::Windows::Forms::TextBox^  SRHotKey;
private: System::Windows::Forms::TextBox^  SRHotKeyTime;
private: System::Windows::Forms::CheckBox^  SRListeningTip;
private: System::Windows::Forms::CheckBox^  SRListeningPrompt;
#pragma endregion

#pragma region Property Sheet Page Fields
private: DoubleAgent::PropertySheetData^ PropertySheetData;
private: System::Windows::Forms::BindingSource^  PropertySheetDataBinding;
private: System::Windows::Forms::CheckBox^  PropertySheetBound;
private: System::Windows::Forms::TextBox^  PropSheetPage;
private: System::Windows::Forms::CheckBox^  PropSheetVisible;
private: System::Windows::Forms::TextBox^  PropSheetWidth;
private: System::Windows::Forms::TextBox^  PropSheetHeight;
private: System::Windows::Forms::NumericUpDown^  PropSheetLeft;
private: System::Windows::Forms::NumericUpDown^  PropSheetTop;
private: System::Windows::Forms::NumericUpDown^  ShowDefaultCharacterY;
private: System::Windows::Forms::NumericUpDown^  ShowDefaultCharacterX;
private: System::Windows::Forms::CheckBox^  ShowDefaultCharacterPos;
private: System::Windows::Forms::Button^  ShowDefaultCharacter;
#pragma endregion

#pragma region Character Files Page Fields
private: DoubleAgent::CharacterFilesData^ CharacterFilesData;
private: System::Windows::Forms::BindingSource^  CharacterFilesDataBinding;
private: System::Windows::Forms::TabPage^  PageCharacterFiles;
private: System::Windows::Forms::CheckBox^  FilesBound;
private: System::Windows::Forms::CheckBox^  ShowDaFiles;
private: System::Windows::Forms::CheckBox^  ShowMaFiles;
private: System::Windows::Forms::CheckBox^  ShowOfficeFiles;
private: System::Windows::Forms::CheckBox^  ShowSpeakingFiles;
private: System::Windows::Forms::CheckBox^  ShowSilentFiles;
private: System::Windows::Forms::CheckBox^  VerifyFileVersion;
private: System::Windows::Forms::TextBox^  DefaultFilePath;
private: System::Windows::Forms::TextBox^  DefaultSearchPath;
private: System::Windows::Forms::TextBox^  CurrentSearchPath;
private: System::Windows::Forms::ListBox^  FilesListBox;
#pragma endregion

#pragma region TTS Engines Page Fields
private: DoubleAgent::TTSEnginesData^ TTSEnginesData;
private: System::Windows::Forms::BindingSource^  TTSEnginesDataBinding;
private: System::Windows::Forms::TabPage^  PageTTSEngines;
private: System::Windows::Forms::CheckBox^  TTSEnginesBound;
private: System::Windows::Forms::DataGridView^  TTSEngineList;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  TTSModeID;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  TTSDisplayName;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  TTSManufacturer;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  TTSGender;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  TTSLanguageID;
#pragma endregion

#pragma region SR Engines Page Fields
private: DoubleAgent::SREnginesData^ SREnginesData;
private: System::Windows::Forms::BindingSource^  SREnginesDataBinding;
private: System::Windows::Forms::TabPage^  PageSREngines;
private: System::Windows::Forms::CheckBox^  SREnginesBound;
private: System::Windows::Forms::DataGridView^  SREngineList;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  SRModeID;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  SRDisplayName;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  SRManufacturer;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  SRLanguageID;
#pragma endregion

#pragma region Events Page Fields
private: System::Windows::Forms::TabPage^  PageMainEvents;
private: System::Windows::Forms::BindingSource^  EventsBindingSource;
private: System::Windows::Forms::DataGridView^  EventsGrid;
private: System::Windows::Forms::Button^  EventsGridClear;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  EventNameCol;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  EventCharacterIDCol;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  EventDataCol;
#pragma endregion

//===========================================================================

#pragma region Windows Form Designer generated code
	/// <summary>
	/// Required method for Designer support - do not modify
	/// the contents of this method with the code editor.
	/// </summary>
	void InitializeComponent(void)
	{
		this->components = (gcnew System::ComponentModel::Container());
		System::Windows::Forms::Label^  CharBalloonCharsPerLineLabel;
		System::Windows::Forms::Label^  CharIconTipLabel;
		System::Windows::Forms::Label^  CharMoveCauseLabel;
		System::Windows::Forms::Label^  CharVisibilityCauseLabel;
		System::Windows::Forms::Label^  CharFilePathLabel;
		System::Windows::Forms::Label^  CharHeightLabel;
		System::Windows::Forms::Label^  CharListeningStatusLabel;
		System::Windows::Forms::Label^  CharWidthLabel;
		System::Windows::Forms::Label^  CharSRModeIDLabel;
		System::Windows::Forms::Label^  CharTopLabel;
		System::Windows::Forms::Label^  CharActiveStateLabel;
		System::Windows::Forms::Label^  CharLeftLabel;
		System::Windows::Forms::Label^  CharIconIdentityLabel;
		System::Windows::Forms::Label^  CharTTSModeIDLabel;
		System::Windows::Forms::Label^  CharFileNameLabel;
		System::Windows::Forms::Label^  CharDescriptionLabel;
		System::Windows::Forms::Label^  CharLanguageIDLabel;
		System::Windows::Forms::Label^  CharVersionLabel;
		System::Windows::Forms::Label^  CharNameLabel;
		System::Windows::Forms::Label^  CharUniqueIDLabel;
		System::Windows::Forms::Label^  CharCharacterIDLabel;
		System::Windows::Forms::Label^  CharMoveToTopLabel;
		System::Windows::Forms::Label^  CharShowMenuYLabel;
		System::Windows::Forms::Label^  CharGestureAtYLabel;
		System::Windows::Forms::Label^  CharShowMenuXLabel;
		System::Windows::Forms::Label^  CharThinkTextLabel;
		System::Windows::Forms::Label^  CharGestureAtXLabel;
		System::Windows::Forms::Label^  CharSpeakTextLabel;
		System::Windows::Forms::Label^  CharMoveToLeftLabel;
		System::Windows::Forms::Label^  CharBalloonBorderColorLabel;
		System::Windows::Forms::Label^  CharBalloonTextColorLabel;
		System::Windows::Forms::Label^  CharBalloonBackColorLabel;
		System::Windows::Forms::Label^  CharBalloonFontCharSetLabel;
		System::Windows::Forms::Label^  CharBalloonFontSizeLabel;
		System::Windows::Forms::Label^  CharBalloonFontNameLabel;
		System::Windows::Forms::Label^  BalloonFontLabel;
		System::Windows::Forms::Label^  BalloonFontSizeLabel;
		System::Windows::Forms::Label^  TTSSpeedLabel;
		System::Windows::Forms::Label^  SRHotKeyLabel;
		System::Windows::Forms::Label^  SRHotKeyTimeLabel;
		System::Windows::Forms::TabPage^  PagePropertySheet;
		System::Windows::Forms::Label^  label21;
		System::Windows::Forms::Label^  label20;
		System::Windows::Forms::Label^  label19;
		System::Windows::Forms::Label^  label10;
		System::Windows::Forms::Label^  PropSheetPageLabel;
		System::Windows::Forms::Label^  PropSheetLeftLabel;
		System::Windows::Forms::Label^  PropSheetTopLabel;
		System::Windows::Forms::Label^  PropSheetWidthLabel;
		System::Windows::Forms::Label^  PropSheetHeightLabel;
		System::Windows::Forms::Label^  DefaultFileLabel;
		System::Windows::Forms::Label^  DefaultSearchLabel;
		System::Windows::Forms::Label^  CurrentSearchLabel;
		System::Windows::Forms::Label^  CharBalloonNumberOfLinesLabel;
		System::Windows::Forms::Label^  label1;
		System::Windows::Forms::Label^  label2;
		System::Windows::Forms::Label^  label3;
		System::Windows::Forms::Label^  label4;
		System::Windows::Forms::Label^  label5;
		System::Windows::Forms::Label^  label6;
		System::Windows::Forms::Label^  label7;
		System::Windows::Forms::Label^  label8;
		System::Windows::Forms::Label^  label9;
		System::Windows::Forms::Label^  label11;
		System::Windows::Forms::Label^  label12;
		System::Windows::Forms::Label^  label13;
		System::Windows::Forms::Label^  label14;
		System::Windows::Forms::Label^  label15;
		System::Windows::Forms::Label^  label16;
		System::Windows::Forms::Label^  label17;
		System::Windows::Forms::Label^  label18;
		System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(ControlTestForm::typeid));
		this->CharacterDataBinding = (gcnew System::Windows::Forms::BindingSource(this->components));
		this->SettingsDataBinding = (gcnew System::Windows::Forms::BindingSource(this->components));
		this->ShowDefaultCharacterY = (gcnew System::Windows::Forms::NumericUpDown());
		this->ShowDefaultCharacterX = (gcnew System::Windows::Forms::NumericUpDown());
		this->ShowDefaultCharacterPos = (gcnew System::Windows::Forms::CheckBox());
		this->ShowDefaultCharacter = (gcnew System::Windows::Forms::Button());
		this->PropertySheetBound = (gcnew System::Windows::Forms::CheckBox());
		this->PropertySheetDataBinding = (gcnew System::Windows::Forms::BindingSource(this->components));
		this->PropSheetVisible = (gcnew System::Windows::Forms::CheckBox());
		this->PropSheetPage = (gcnew System::Windows::Forms::TextBox());
		this->PropSheetLeft = (gcnew System::Windows::Forms::NumericUpDown());
		this->PropSheetTop = (gcnew System::Windows::Forms::NumericUpDown());
		this->PropSheetWidth = (gcnew System::Windows::Forms::TextBox());
		this->PropSheetHeight = (gcnew System::Windows::Forms::TextBox());
		this->CharacterFilesDataBinding = (gcnew System::Windows::Forms::BindingSource(this->components));
		this->TabsControl = (gcnew System::Windows::Forms::TabControl());
		this->PageCharacters = (gcnew System::Windows::Forms::TabPage());
		this->CharacterList = (gcnew System::Windows::Forms::DataGridView());
		this->CharListID = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->CharListName = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->CharListFileName = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->CharListFilePath = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->CharListVersion = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->CharListUniqueID = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->CharactersDataBinding = (gcnew System::Windows::Forms::BindingSource(this->components));
		this->CharactersBound = (gcnew System::Windows::Forms::CheckBox());
		this->PageSettings = (gcnew System::Windows::Forms::TabPage());
		this->SettingsBound = (gcnew System::Windows::Forms::CheckBox());
		this->BalloonEnabled = (gcnew System::Windows::Forms::CheckBox());
		this->BalloonFontName = (gcnew System::Windows::Forms::TextBox());
		this->BalloonFontSize = (gcnew System::Windows::Forms::NumericUpDown());
		this->BalloonFontBold = (gcnew System::Windows::Forms::CheckBox());
		this->BalloonFontItalic = (gcnew System::Windows::Forms::CheckBox());
		this->BalloonFontUnderline = (gcnew System::Windows::Forms::CheckBox());
		this->SoundEffectsEnabled = (gcnew System::Windows::Forms::CheckBox());
		this->TTSEnabled = (gcnew System::Windows::Forms::CheckBox());
		this->TTSSpeed = (gcnew System::Windows::Forms::TextBox());
		this->SREnabled = (gcnew System::Windows::Forms::CheckBox());
		this->SRHotKey = (gcnew System::Windows::Forms::TextBox());
		this->SRHotKeyTime = (gcnew System::Windows::Forms::TextBox());
		this->SRListeningPrompt = (gcnew System::Windows::Forms::CheckBox());
		this->SRListeningTip = (gcnew System::Windows::Forms::CheckBox());
		this->PageCharacterFiles = (gcnew System::Windows::Forms::TabPage());
		this->FilesBound = (gcnew System::Windows::Forms::CheckBox());
		this->ShowDaFiles = (gcnew System::Windows::Forms::CheckBox());
		this->ShowMaFiles = (gcnew System::Windows::Forms::CheckBox());
		this->ShowOfficeFiles = (gcnew System::Windows::Forms::CheckBox());
		this->ShowSpeakingFiles = (gcnew System::Windows::Forms::CheckBox());
		this->ShowSilentFiles = (gcnew System::Windows::Forms::CheckBox());
		this->VerifyFileVersion = (gcnew System::Windows::Forms::CheckBox());
		this->DefaultFilePath = (gcnew System::Windows::Forms::TextBox());
		this->DefaultSearchPath = (gcnew System::Windows::Forms::TextBox());
		this->CurrentSearchPath = (gcnew System::Windows::Forms::TextBox());
		this->FilesListBox = (gcnew System::Windows::Forms::ListBox());
		this->PageTTSEngines = (gcnew System::Windows::Forms::TabPage());
		this->TTSEnginesBound = (gcnew System::Windows::Forms::CheckBox());
		this->TTSEnginesDataBinding = (gcnew System::Windows::Forms::BindingSource(this->components));
		this->TTSEngineList = (gcnew System::Windows::Forms::DataGridView());
		this->TTSModeID = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->TTSDisplayName = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->TTSManufacturer = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->TTSGender = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->TTSLanguageID = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->PageSREngines = (gcnew System::Windows::Forms::TabPage());
		this->SREnginesBound = (gcnew System::Windows::Forms::CheckBox());
		this->SREnginesDataBinding = (gcnew System::Windows::Forms::BindingSource(this->components));
		this->SREngineList = (gcnew System::Windows::Forms::DataGridView());
		this->SRModeID = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->SRDisplayName = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->SRManufacturer = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->SRLanguageID = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->TestDaControl = (gcnew DoubleAgent::AxControl::AxControl());
		this->RaiseRequestErrorsButton = (gcnew System::Windows::Forms::CheckBox());
		this->AutoConnectButton = (gcnew System::Windows::Forms::CheckBox());
		this->ControlConnectedButton = (gcnew System::Windows::Forms::CheckBox());
		this->LoadButton = (gcnew System::Windows::Forms::Button());
		this->TabsMain = (gcnew System::Windows::Forms::TabControl());
		this->PageMainCharacter = (gcnew System::Windows::Forms::TabPage());
		this->TabsCharacter = (gcnew System::Windows::Forms::TabControl());
		this->PageCharacter = (gcnew System::Windows::Forms::TabPage());
		this->CharGenerateIcon = (gcnew System::Windows::Forms::Button());
		this->CharHeightDefault = (gcnew System::Windows::Forms::Button());
		this->CharWidthDefault = (gcnew System::Windows::Forms::Button());
		this->CharHeight = (gcnew System::Windows::Forms::NumericUpDown());
		this->CharacterBindingSource = (gcnew System::Windows::Forms::BindingSource(this->components));
		this->CharWidth = (gcnew System::Windows::Forms::NumericUpDown());
		this->CharTop = (gcnew System::Windows::Forms::NumericUpDown());
		this->CharLeft = (gcnew System::Windows::Forms::NumericUpDown());
		this->CharMoveCause = (gcnew System::Windows::Forms::ComboBox());
		this->CharVisibilityCause = (gcnew System::Windows::Forms::ComboBox());
		this->CharListeningStatus = (gcnew System::Windows::Forms::ComboBox());
		this->CharActiveState = (gcnew System::Windows::Forms::ComboBox());
		this->CharHasOtherClients = (gcnew System::Windows::Forms::CheckBox());
		this->CharHasIcon = (gcnew System::Windows::Forms::CheckBox());
		this->CharAutoPopupMenu = (gcnew System::Windows::Forms::CheckBox());
		this->CharIdleState = (gcnew System::Windows::Forms::CheckBox());
		this->CharIdleEnabled = (gcnew System::Windows::Forms::CheckBox());
		this->CharSoundEffectsEnabled = (gcnew System::Windows::Forms::CheckBox());
		this->CharIconVisible = (gcnew System::Windows::Forms::CheckBox());
		this->CharIconShown = (gcnew System::Windows::Forms::CheckBox());
		this->CharVisible = (gcnew System::Windows::Forms::CheckBox());
		this->CharIconTip = (gcnew System::Windows::Forms::TextBox());
		this->CharFilePath = (gcnew System::Windows::Forms::TextBox());
		this->CharSRModeID = (gcnew System::Windows::Forms::TextBox());
		this->CharTTSModeID = (gcnew System::Windows::Forms::TextBox());
		this->CharIconIdentity = (gcnew System::Windows::Forms::TextBox());
		this->CharFileName = (gcnew System::Windows::Forms::TextBox());
		this->CharDescription = (gcnew System::Windows::Forms::TextBox());
		this->CharLanguageID = (gcnew System::Windows::Forms::TextBox());
		this->CharVersion = (gcnew System::Windows::Forms::TextBox());
		this->CharName = (gcnew System::Windows::Forms::TextBox());
		this->CharUniqueID = (gcnew System::Windows::Forms::TextBox());
		this->CharCharacterID = (gcnew System::Windows::Forms::TextBox());
		this->PageActions = (gcnew System::Windows::Forms::TabPage());
		this->CharPlayAnimation = (gcnew System::Windows::Forms::ListBox());
		this->CharThinkText = (gcnew System::Windows::Forms::TextBox());
		this->CharSpeakText = (gcnew System::Windows::Forms::TextBox());
		this->CharMoveToTop = (gcnew System::Windows::Forms::NumericUpDown());
		this->CharShowMenuY = (gcnew System::Windows::Forms::NumericUpDown());
		this->CharShowMenuX = (gcnew System::Windows::Forms::NumericUpDown());
		this->CharGestureAtY = (gcnew System::Windows::Forms::NumericUpDown());
		this->CharGestureAtX = (gcnew System::Windows::Forms::NumericUpDown());
		this->CharMoveToLeft = (gcnew System::Windows::Forms::NumericUpDown());
		this->CharHideFast = (gcnew System::Windows::Forms::CheckBox());
		this->CharShowFast = (gcnew System::Windows::Forms::CheckBox());
		this->CharStopAll = (gcnew System::Windows::Forms::Button());
		this->CharPlay = (gcnew System::Windows::Forms::Button());
		this->CharShowPopupMenu = (gcnew System::Windows::Forms::Button());
		this->CharListenStop = (gcnew System::Windows::Forms::Button());
		this->CharListen = (gcnew System::Windows::Forms::Button());
		this->CharThink = (gcnew System::Windows::Forms::Button());
		this->CharSpeak = (gcnew System::Windows::Forms::Button());
		this->CharGestureAt = (gcnew System::Windows::Forms::Button());
		this->CharMoveTo = (gcnew System::Windows::Forms::Button());
		this->CharHide = (gcnew System::Windows::Forms::Button());
		this->CharShow = (gcnew System::Windows::Forms::Button());
		this->PageBalloon = (gcnew System::Windows::Forms::TabPage());
		this->CharBalloonBorderColorName = (gcnew System::Windows::Forms::TextBox());
		this->BalloonBindingSource = (gcnew System::Windows::Forms::BindingSource(this->components));
		this->CharBalloonTextColorName = (gcnew System::Windows::Forms::TextBox());
		this->CharBalloonBackColorName = (gcnew System::Windows::Forms::TextBox());
		this->CharBalloonFontStrikeThru = (gcnew System::Windows::Forms::CheckBox());
		this->CharBalloonFontUnderline = (gcnew System::Windows::Forms::CheckBox());
		this->CharBalloonFontItalic = (gcnew System::Windows::Forms::CheckBox());
		this->CharBalloonFontBold = (gcnew System::Windows::Forms::CheckBox());
		this->CharBalloonFontCharSet = (gcnew System::Windows::Forms::TextBox());
		this->CharBalloonFontSize = (gcnew System::Windows::Forms::NumericUpDown());
		this->CharBalloonFontName = (gcnew System::Windows::Forms::TextBox());
		this->CharBalloonAutoPace = (gcnew System::Windows::Forms::CheckBox());
		this->CharBalloonAutoHide = (gcnew System::Windows::Forms::CheckBox());
		this->CharBalloonShowPartialLines = (gcnew System::Windows::Forms::CheckBox());
		this->CharBalloonSizeToText = (gcnew System::Windows::Forms::CheckBox());
		this->CharBalloonCharsPerLine = (gcnew System::Windows::Forms::NumericUpDown());
		this->CharBalloonNumberOfLines = (gcnew System::Windows::Forms::NumericUpDown());
		this->CharBalloonVisible = (gcnew System::Windows::Forms::CheckBox());
		this->CharBalloonEnabled = (gcnew System::Windows::Forms::CheckBox());
		this->PageCommands = (gcnew System::Windows::Forms::TabPage());
		this->CharCommands = (gcnew System::Windows::Forms::DataGridView());
		this->CharCommandsNameCol = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->CharCommandsCaptionCol = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->CharCommandsVoiceCaptionCol = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->CharCommandsVoiceGrammarCol = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->CharCommandsEnabledCol = (gcnew System::Windows::Forms::DataGridViewCheckBoxColumn());
		this->CharCommandsVisibleCol = (gcnew System::Windows::Forms::DataGridViewCheckBoxColumn());
		this->CharCommandsConfidenceThresholdCol = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->CharCommandsConfidenceTextCol = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->CommandBindingSource = (gcnew System::Windows::Forms::BindingSource(this->components));
		this->GlobalVoiceCommandsEnabled = (gcnew System::Windows::Forms::CheckBox());
		this->CommandsBindingSource = (gcnew System::Windows::Forms::BindingSource(this->components));
		this->CharCommandsVisible = (gcnew System::Windows::Forms::CheckBox());
		this->CharCommandsDefaultCommand = (gcnew System::Windows::Forms::TextBox());
		this->CharCommandsFontSize = (gcnew System::Windows::Forms::NumericUpDown());
		this->CharCommandsFontName = (gcnew System::Windows::Forms::TextBox());
		this->CharCommandsVoiceGrammar = (gcnew System::Windows::Forms::TextBox());
		this->CharCommandsVoiceCaption = (gcnew System::Windows::Forms::TextBox());
		this->CharCommandsCaption = (gcnew System::Windows::Forms::TextBox());
		this->PageMainControl = (gcnew System::Windows::Forms::TabPage());
		this->PageMainEvents = (gcnew System::Windows::Forms::TabPage());
		this->EventsGridClear = (gcnew System::Windows::Forms::Button());
		this->EventsGrid = (gcnew System::Windows::Forms::DataGridView());
		this->EventNameCol = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->EventCharacterIDCol = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->EventDataCol = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->EventsBindingSource = (gcnew System::Windows::Forms::BindingSource(this->components));
		this->ChooseColorDialog = (gcnew System::Windows::Forms::ColorDialog());
		this->UnloadButton = (gcnew System::Windows::Forms::Button());
		this->SetCurrentButton = (gcnew System::Windows::Forms::Button());
		this->CollectButton = (gcnew System::Windows::Forms::Button());
		CharBalloonCharsPerLineLabel = (gcnew System::Windows::Forms::Label());
		CharIconTipLabel = (gcnew System::Windows::Forms::Label());
		CharMoveCauseLabel = (gcnew System::Windows::Forms::Label());
		CharVisibilityCauseLabel = (gcnew System::Windows::Forms::Label());
		CharFilePathLabel = (gcnew System::Windows::Forms::Label());
		CharHeightLabel = (gcnew System::Windows::Forms::Label());
		CharListeningStatusLabel = (gcnew System::Windows::Forms::Label());
		CharWidthLabel = (gcnew System::Windows::Forms::Label());
		CharSRModeIDLabel = (gcnew System::Windows::Forms::Label());
		CharTopLabel = (gcnew System::Windows::Forms::Label());
		CharActiveStateLabel = (gcnew System::Windows::Forms::Label());
		CharLeftLabel = (gcnew System::Windows::Forms::Label());
		CharIconIdentityLabel = (gcnew System::Windows::Forms::Label());
		CharTTSModeIDLabel = (gcnew System::Windows::Forms::Label());
		CharFileNameLabel = (gcnew System::Windows::Forms::Label());
		CharDescriptionLabel = (gcnew System::Windows::Forms::Label());
		CharLanguageIDLabel = (gcnew System::Windows::Forms::Label());
		CharVersionLabel = (gcnew System::Windows::Forms::Label());
		CharNameLabel = (gcnew System::Windows::Forms::Label());
		CharUniqueIDLabel = (gcnew System::Windows::Forms::Label());
		CharCharacterIDLabel = (gcnew System::Windows::Forms::Label());
		CharMoveToTopLabel = (gcnew System::Windows::Forms::Label());
		CharShowMenuYLabel = (gcnew System::Windows::Forms::Label());
		CharGestureAtYLabel = (gcnew System::Windows::Forms::Label());
		CharShowMenuXLabel = (gcnew System::Windows::Forms::Label());
		CharThinkTextLabel = (gcnew System::Windows::Forms::Label());
		CharGestureAtXLabel = (gcnew System::Windows::Forms::Label());
		CharSpeakTextLabel = (gcnew System::Windows::Forms::Label());
		CharMoveToLeftLabel = (gcnew System::Windows::Forms::Label());
		CharBalloonBorderColorLabel = (gcnew System::Windows::Forms::Label());
		CharBalloonTextColorLabel = (gcnew System::Windows::Forms::Label());
		CharBalloonBackColorLabel = (gcnew System::Windows::Forms::Label());
		CharBalloonFontCharSetLabel = (gcnew System::Windows::Forms::Label());
		CharBalloonFontSizeLabel = (gcnew System::Windows::Forms::Label());
		CharBalloonFontNameLabel = (gcnew System::Windows::Forms::Label());
		BalloonFontLabel = (gcnew System::Windows::Forms::Label());
		BalloonFontSizeLabel = (gcnew System::Windows::Forms::Label());
		TTSSpeedLabel = (gcnew System::Windows::Forms::Label());
		SRHotKeyLabel = (gcnew System::Windows::Forms::Label());
		SRHotKeyTimeLabel = (gcnew System::Windows::Forms::Label());
		PagePropertySheet = (gcnew System::Windows::Forms::TabPage());
		label21 = (gcnew System::Windows::Forms::Label());
		label20 = (gcnew System::Windows::Forms::Label());
		label19 = (gcnew System::Windows::Forms::Label());
		label10 = (gcnew System::Windows::Forms::Label());
		PropSheetPageLabel = (gcnew System::Windows::Forms::Label());
		PropSheetLeftLabel = (gcnew System::Windows::Forms::Label());
		PropSheetTopLabel = (gcnew System::Windows::Forms::Label());
		PropSheetWidthLabel = (gcnew System::Windows::Forms::Label());
		PropSheetHeightLabel = (gcnew System::Windows::Forms::Label());
		DefaultFileLabel = (gcnew System::Windows::Forms::Label());
		DefaultSearchLabel = (gcnew System::Windows::Forms::Label());
		CurrentSearchLabel = (gcnew System::Windows::Forms::Label());
		CharBalloonNumberOfLinesLabel = (gcnew System::Windows::Forms::Label());
		label1 = (gcnew System::Windows::Forms::Label());
		label2 = (gcnew System::Windows::Forms::Label());
		label3 = (gcnew System::Windows::Forms::Label());
		label4 = (gcnew System::Windows::Forms::Label());
		label5 = (gcnew System::Windows::Forms::Label());
		label6 = (gcnew System::Windows::Forms::Label());
		label7 = (gcnew System::Windows::Forms::Label());
		label8 = (gcnew System::Windows::Forms::Label());
		label9 = (gcnew System::Windows::Forms::Label());
		label11 = (gcnew System::Windows::Forms::Label());
		label12 = (gcnew System::Windows::Forms::Label());
		label13 = (gcnew System::Windows::Forms::Label());
		label14 = (gcnew System::Windows::Forms::Label());
		label15 = (gcnew System::Windows::Forms::Label());
		label16 = (gcnew System::Windows::Forms::Label());
		label17 = (gcnew System::Windows::Forms::Label());
		label18 = (gcnew System::Windows::Forms::Label());
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharacterDataBinding))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->SettingsDataBinding))->BeginInit();
		PagePropertySheet->SuspendLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ShowDefaultCharacterY))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ShowDefaultCharacterX))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PropertySheetDataBinding))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PropSheetLeft))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PropSheetTop))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharacterFilesDataBinding))->BeginInit();
		this->TabsControl->SuspendLayout();
		this->PageCharacters->SuspendLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharacterList))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharactersDataBinding))->BeginInit();
		this->PageSettings->SuspendLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->BalloonFontSize))->BeginInit();
		this->PageCharacterFiles->SuspendLayout();
		this->PageTTSEngines->SuspendLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TTSEnginesDataBinding))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TTSEngineList))->BeginInit();
		this->PageSREngines->SuspendLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->SREnginesDataBinding))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->SREngineList))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TestDaControl))->BeginInit();
		this->TabsMain->SuspendLayout();
		this->PageMainCharacter->SuspendLayout();
		this->TabsCharacter->SuspendLayout();
		this->PageCharacter->SuspendLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharHeight))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharacterBindingSource))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharWidth))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharTop))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharLeft))->BeginInit();
		this->PageActions->SuspendLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharMoveToTop))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharShowMenuY))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharShowMenuX))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharGestureAtY))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharGestureAtX))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharMoveToLeft))->BeginInit();
		this->PageBalloon->SuspendLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->BalloonBindingSource))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharBalloonFontSize))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharBalloonCharsPerLine))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharBalloonNumberOfLines))->BeginInit();
		this->PageCommands->SuspendLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharCommands))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CommandBindingSource))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CommandsBindingSource))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharCommandsFontSize))->BeginInit();
		this->PageMainControl->SuspendLayout();
		this->PageMainEvents->SuspendLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->EventsGrid))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->EventsBindingSource))->BeginInit();
		this->SuspendLayout();
		//
		// CharBalloonCharsPerLineLabel
		//
		CharBalloonCharsPerLineLabel->AutoSize = true;
		CharBalloonCharsPerLineLabel->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding,
			L"Bound", true)));
		CharBalloonCharsPerLineLabel->Location = System::Drawing::Point(420, 38);
		CharBalloonCharsPerLineLabel->Name = L"CharBalloonCharsPerLineLabel";
		CharBalloonCharsPerLineLabel->Size = System::Drawing::Size(94, 17);
		CharBalloonCharsPerLineLabel->TabIndex = 8;
		CharBalloonCharsPerLineLabel->Text = L"CharsPerLine";
		CharBalloonCharsPerLineLabel->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
		//
		// CharIconTipLabel
		//
		CharIconTipLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		CharIconTipLabel->AutoSize = true;
		CharIconTipLabel->Location = System::Drawing::Point(371, 306);
		CharIconTipLabel->Name = L"CharIconTipLabel";
		CharIconTipLabel->Size = System::Drawing::Size(54, 17);
		CharIconTipLabel->TabIndex = 34;
		CharIconTipLabel->Text = L"IconTip";
		//
		// CharMoveCauseLabel
		//
		CharMoveCauseLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		CharMoveCauseLabel->AutoSize = true;
		CharMoveCauseLabel->Location = System::Drawing::Point(297, 431);
		CharMoveCauseLabel->Name = L"CharMoveCauseLabel";
		CharMoveCauseLabel->Size = System::Drawing::Size(82, 17);
		CharMoveCauseLabel->TabIndex = 51;
		CharMoveCauseLabel->Text = L"MoveCause";
		//
		// CharVisibilityCauseLabel
		//
		CharVisibilityCauseLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		CharVisibilityCauseLabel->AutoSize = true;
		CharVisibilityCauseLabel->Location = System::Drawing::Point(297, 384);
		CharVisibilityCauseLabel->Name = L"CharVisibilityCauseLabel";
		CharVisibilityCauseLabel->Size = System::Drawing::Size(98, 17);
		CharVisibilityCauseLabel->TabIndex = 49;
		CharVisibilityCauseLabel->Text = L"VisibilityCause";
		//
		// CharFilePathLabel
		//
		CharFilePathLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		CharFilePathLabel->AutoSize = true;
		CharFilePathLabel->Location = System::Drawing::Point(255, 121);
		CharFilePathLabel->Name = L"CharFilePathLabel";
		CharFilePathLabel->Size = System::Drawing::Size(59, 17);
		CharFilePathLabel->TabIndex = 14;
		CharFilePathLabel->Text = L"FilePath";
		//
		// CharHeightLabel
		//
		CharHeightLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		CharHeightLabel->AutoSize = true;
		CharHeightLabel->Location = System::Drawing::Point(459, 348);
		CharHeightLabel->Name = L"CharHeightLabel";
		CharHeightLabel->Size = System::Drawing::Size(49, 17);
		CharHeightLabel->TabIndex = 41;
		CharHeightLabel->Text = L"Height";
		CharHeightLabel->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
		//
		// CharListeningStatusLabel
		//
		CharListeningStatusLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		CharListeningStatusLabel->AutoSize = true;
		CharListeningStatusLabel->Location = System::Drawing::Point(6, 431);
		CharListeningStatusLabel->Name = L"CharListeningStatusLabel";
		CharListeningStatusLabel->Size = System::Drawing::Size(105, 17);
		CharListeningStatusLabel->TabIndex = 47;
		CharListeningStatusLabel->Text = L"ListeningStatus";
		//
		// CharWidthLabel
		//
		CharWidthLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		CharWidthLabel->AutoSize = true;
		CharWidthLabel->Location = System::Drawing::Point(297, 348);
		CharWidthLabel->Name = L"CharWidthLabel";
		CharWidthLabel->Size = System::Drawing::Size(44, 17);
		CharWidthLabel->TabIndex = 40;
		CharWidthLabel->Text = L"Width";
		CharWidthLabel->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
		//
		// CharSRModeIDLabel
		//
		CharSRModeIDLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		CharSRModeIDLabel->AutoSize = true;
		CharSRModeIDLabel->Location = System::Drawing::Point(6, 177);
		CharSRModeIDLabel->Name = L"CharSRModeIDLabel";
		CharSRModeIDLabel->Size = System::Drawing::Size(75, 17);
		CharSRModeIDLabel->TabIndex = 18;
		CharSRModeIDLabel->Text = L"SRModeID";
		//
		// CharTopLabel
		//
		CharTopLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		CharTopLabel->AutoSize = true;
		CharTopLabel->Location = System::Drawing::Point(177, 348);
		CharTopLabel->Name = L"CharTopLabel";
		CharTopLabel->Size = System::Drawing::Size(33, 17);
		CharTopLabel->TabIndex = 38;
		CharTopLabel->Text = L"Top";
		CharTopLabel->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
		//
		// CharActiveStateLabel
		//
		CharActiveStateLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		CharActiveStateLabel->AutoSize = true;
		CharActiveStateLabel->Location = System::Drawing::Point(6, 384);
		CharActiveStateLabel->Name = L"CharActiveStateLabel";
		CharActiveStateLabel->Size = System::Drawing::Size(79, 17);
		CharActiveStateLabel->TabIndex = 44;
		CharActiveStateLabel->Text = L"ActiveState";
		//
		// CharLeftLabel
		//
		CharLeftLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		CharLeftLabel->AutoSize = true;
		CharLeftLabel->Location = System::Drawing::Point(57, 348);
		CharLeftLabel->Name = L"CharLeftLabel";
		CharLeftLabel->Size = System::Drawing::Size(32, 17);
		CharLeftLabel->TabIndex = 36;
		CharLeftLabel->Text = L"Left";
		CharLeftLabel->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
		//
		// CharIconIdentityLabel
		//
		CharIconIdentityLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		CharIconIdentityLabel->AutoSize = true;
		CharIconIdentityLabel->Location = System::Drawing::Point(6, 306);
		CharIconIdentityLabel->Name = L"CharIconIdentityLabel";
		CharIconIdentityLabel->Size = System::Drawing::Size(79, 17);
		CharIconIdentityLabel->TabIndex = 32;
		CharIconIdentityLabel->Text = L"IconIdentity";
		//
		// CharTTSModeIDLabel
		//
		CharTTSModeIDLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		CharTTSModeIDLabel->AutoSize = true;
		CharTTSModeIDLabel->Location = System::Drawing::Point(6, 149);
		CharTTSModeIDLabel->Name = L"CharTTSModeIDLabel";
		CharTTSModeIDLabel->Size = System::Drawing::Size(83, 17);
		CharTTSModeIDLabel->TabIndex = 16;
		CharTTSModeIDLabel->Text = L"TTSModeID";
		//
		// CharFileNameLabel
		//
		CharFileNameLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		CharFileNameLabel->AutoSize = true;
		CharFileNameLabel->Location = System::Drawing::Point(6, 121);
		CharFileNameLabel->Name = L"CharFileNameLabel";
		CharFileNameLabel->Size = System::Drawing::Size(67, 17);
		CharFileNameLabel->TabIndex = 12;
		CharFileNameLabel->Text = L"FileName";
		//
		// CharDescriptionLabel
		//
		CharDescriptionLabel->AutoSize = true;
		CharDescriptionLabel->Location = System::Drawing::Point(6, 75);
		CharDescriptionLabel->Name = L"CharDescriptionLabel";
		CharDescriptionLabel->Size = System::Drawing::Size(79, 17);
		CharDescriptionLabel->TabIndex = 10;
		CharDescriptionLabel->Text = L"Description";
		//
		// CharLanguageIDLabel
		//
		CharLanguageIDLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
		CharLanguageIDLabel->AutoSize = true;
		CharLanguageIDLabel->Location = System::Drawing::Point(339, 47);
		CharLanguageIDLabel->Name = L"CharLanguageIDLabel";
		CharLanguageIDLabel->Size = System::Drawing::Size(72, 17);
		CharLanguageIDLabel->TabIndex = 6;
		CharLanguageIDLabel->Text = L"Language";
		CharLanguageIDLabel->TextAlign = System::Drawing::ContentAlignment::TopRight;
		//
		// CharVersionLabel
		//
		CharVersionLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
		CharVersionLabel->AutoSize = true;
		CharVersionLabel->Location = System::Drawing::Point(492, 47);
		CharVersionLabel->Name = L"CharVersionLabel";
		CharVersionLabel->Size = System::Drawing::Size(56, 17);
		CharVersionLabel->TabIndex = 8;
		CharVersionLabel->Text = L"Version";
		CharVersionLabel->TextAlign = System::Drawing::ContentAlignment::TopRight;
		//
		// CharNameLabel
		//
		CharNameLabel->AutoSize = true;
		CharNameLabel->Location = System::Drawing::Point(6, 47);
		CharNameLabel->Name = L"CharNameLabel";
		CharNameLabel->Size = System::Drawing::Size(45, 17);
		CharNameLabel->TabIndex = 4;
		CharNameLabel->Text = L"Name";
		//
		// CharUniqueIDLabel
		//
		CharUniqueIDLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
		CharUniqueIDLabel->AutoSize = true;
		CharUniqueIDLabel->Location = System::Drawing::Point(216, 15);
		CharUniqueIDLabel->Name = L"CharUniqueIDLabel";
		CharUniqueIDLabel->Size = System::Drawing::Size(66, 17);
		CharUniqueIDLabel->TabIndex = 2;
		CharUniqueIDLabel->Text = L"UniqueID";
		CharUniqueIDLabel->TextAlign = System::Drawing::ContentAlignment::TopRight;
		//
		// CharCharacterIDLabel
		//
		CharCharacterIDLabel->AutoSize = true;
		CharCharacterIDLabel->Location = System::Drawing::Point(6, 15);
		CharCharacterIDLabel->Name = L"CharCharacterIDLabel";
		CharCharacterIDLabel->Size = System::Drawing::Size(83, 17);
		CharCharacterIDLabel->TabIndex = 0;
		CharCharacterIDLabel->Text = L"CharacterID";
		//
		// CharMoveToTopLabel
		//
		CharMoveToTopLabel->AutoSize = true;
		CharMoveToTopLabel->Location = System::Drawing::Point(257, 94);
		CharMoveToTopLabel->Name = L"CharMoveToTopLabel";
		CharMoveToTopLabel->Size = System::Drawing::Size(33, 17);
		CharMoveToTopLabel->TabIndex = 7;
		CharMoveToTopLabel->Text = L"Top";
		//
		// CharShowMenuYLabel
		//
		CharShowMenuYLabel->AutoSize = true;
		CharShowMenuYLabel->Location = System::Drawing::Point(257, 284);
		CharShowMenuYLabel->Name = L"CharShowMenuYLabel";
		CharShowMenuYLabel->Size = System::Drawing::Size(17, 17);
		CharShowMenuYLabel->TabIndex = 25;
		CharShowMenuYLabel->Text = L"Y";
		//
		// CharGestureAtYLabel
		//
		CharGestureAtYLabel->AutoSize = true;
		CharGestureAtYLabel->Location = System::Drawing::Point(257, 132);
		CharGestureAtYLabel->Name = L"CharGestureAtYLabel";
		CharGestureAtYLabel->Size = System::Drawing::Size(17, 17);
		CharGestureAtYLabel->TabIndex = 12;
		CharGestureAtYLabel->Text = L"Y";
		//
		// CharShowMenuXLabel
		//
		CharShowMenuXLabel->AutoSize = true;
		CharShowMenuXLabel->Location = System::Drawing::Point(149, 283);
		CharShowMenuXLabel->Name = L"CharShowMenuXLabel";
		CharShowMenuXLabel->Size = System::Drawing::Size(17, 17);
		CharShowMenuXLabel->TabIndex = 23;
		CharShowMenuXLabel->Text = L"X";
		//
		// CharThinkTextLabel
		//
		CharThinkTextLabel->AutoSize = true;
		CharThinkTextLabel->Location = System::Drawing::Point(149, 208);
		CharThinkTextLabel->Name = L"CharThinkTextLabel";
		CharThinkTextLabel->Size = System::Drawing::Size(35, 17);
		CharThinkTextLabel->TabIndex = 18;
		CharThinkTextLabel->Text = L"Text";
		//
		// CharGestureAtXLabel
		//
		CharGestureAtXLabel->AutoSize = true;
		CharGestureAtXLabel->Location = System::Drawing::Point(149, 131);
		CharGestureAtXLabel->Name = L"CharGestureAtXLabel";
		CharGestureAtXLabel->Size = System::Drawing::Size(17, 17);
		CharGestureAtXLabel->TabIndex = 10;
		CharGestureAtXLabel->Text = L"X";
		//
		// CharSpeakTextLabel
		//
		CharSpeakTextLabel->AutoSize = true;
		CharSpeakTextLabel->Location = System::Drawing::Point(149, 170);
		CharSpeakTextLabel->Name = L"CharSpeakTextLabel";
		CharSpeakTextLabel->Size = System::Drawing::Size(35, 17);
		CharSpeakTextLabel->TabIndex = 15;
		CharSpeakTextLabel->Text = L"Text";
		//
		// CharMoveToLeftLabel
		//
		CharMoveToLeftLabel->AutoSize = true;
		CharMoveToLeftLabel->Location = System::Drawing::Point(149, 94);
		CharMoveToLeftLabel->Name = L"CharMoveToLeftLabel";
		CharMoveToLeftLabel->Size = System::Drawing::Size(32, 17);
		CharMoveToLeftLabel->TabIndex = 5;
		CharMoveToLeftLabel->Text = L"Left";
		//
		// CharBalloonBorderColorLabel
		//
		CharBalloonBorderColorLabel->AutoSize = true;
		CharBalloonBorderColorLabel->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding,
			L"Bound", true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		CharBalloonBorderColorLabel->Location = System::Drawing::Point(430, 80);
		CharBalloonBorderColorLabel->Name = L"CharBalloonBorderColorLabel";
		CharBalloonBorderColorLabel->Size = System::Drawing::Size(84, 17);
		CharBalloonBorderColorLabel->TabIndex = 14;
		CharBalloonBorderColorLabel->Text = L"BorderColor";
		//
		// CharBalloonTextColorLabel
		//
		CharBalloonTextColorLabel->AutoSize = true;
		CharBalloonTextColorLabel->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		CharBalloonTextColorLabel->Location = System::Drawing::Point(227, 80);
		CharBalloonTextColorLabel->Name = L"CharBalloonTextColorLabel";
		CharBalloonTextColorLabel->Size = System::Drawing::Size(68, 17);
		CharBalloonTextColorLabel->TabIndex = 12;
		CharBalloonTextColorLabel->Text = L"TextColor";
		//
		// CharBalloonBackColorLabel
		//
		CharBalloonBackColorLabel->AutoSize = true;
		CharBalloonBackColorLabel->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		CharBalloonBackColorLabel->Location = System::Drawing::Point(6, 80);
		CharBalloonBackColorLabel->Name = L"CharBalloonBackColorLabel";
		CharBalloonBackColorLabel->Size = System::Drawing::Size(72, 17);
		CharBalloonBackColorLabel->TabIndex = 10;
		CharBalloonBackColorLabel->Text = L"BackColor";
		//
		// CharBalloonFontCharSetLabel
		//
		CharBalloonFontCharSetLabel->AutoSize = true;
		CharBalloonFontCharSetLabel->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding,
			L"Bound", true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		CharBalloonFontCharSetLabel->Location = System::Drawing::Point(427, 118);
		CharBalloonFontCharSetLabel->Name = L"CharBalloonFontCharSetLabel";
		CharBalloonFontCharSetLabel->Size = System::Drawing::Size(87, 17);
		CharBalloonFontCharSetLabel->TabIndex = 20;
		CharBalloonFontCharSetLabel->Text = L"FontCharSet";
		//
		// CharBalloonFontSizeLabel
		//
		CharBalloonFontSizeLabel->AutoSize = true;
		CharBalloonFontSizeLabel->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		CharBalloonFontSizeLabel->Location = System::Drawing::Point(227, 118);
		CharBalloonFontSizeLabel->Name = L"CharBalloonFontSizeLabel";
		CharBalloonFontSizeLabel->Size = System::Drawing::Size(63, 17);
		CharBalloonFontSizeLabel->TabIndex = 18;
		CharBalloonFontSizeLabel->Text = L"FontSize";
		//
		// CharBalloonFontNameLabel
		//
		CharBalloonFontNameLabel->AutoSize = true;
		CharBalloonFontNameLabel->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		CharBalloonFontNameLabel->Location = System::Drawing::Point(6, 118);
		CharBalloonFontNameLabel->Name = L"CharBalloonFontNameLabel";
		CharBalloonFontNameLabel->Size = System::Drawing::Size(73, 17);
		CharBalloonFontNameLabel->TabIndex = 16;
		CharBalloonFontNameLabel->Text = L"FontName";
		//
		// BalloonFontLabel
		//
		BalloonFontLabel->AutoSize = true;
		BalloonFontLabel->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->SettingsDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		BalloonFontLabel->Location = System::Drawing::Point(154, 13);
		BalloonFontLabel->Name = L"BalloonFontLabel";
		BalloonFontLabel->Size = System::Drawing::Size(36, 17);
		BalloonFontLabel->TabIndex = 2;
		BalloonFontLabel->Text = L"Font";
		BalloonFontLabel->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		//
		// BalloonFontSizeLabel
		//
		BalloonFontSizeLabel->AutoSize = true;
		BalloonFontSizeLabel->Location = System::Drawing::Point(155, 43);
		BalloonFontSizeLabel->Name = L"BalloonFontSizeLabel";
		BalloonFontSizeLabel->Size = System::Drawing::Size(35, 17);
		BalloonFontSizeLabel->TabIndex = 4;
		BalloonFontSizeLabel->Text = L"Size";
		//
		// TTSSpeedLabel
		//
		TTSSpeedLabel->AutoSize = true;
		TTSSpeedLabel->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->SettingsDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		TTSSpeedLabel->Location = System::Drawing::Point(154, 151);
		TTSSpeedLabel->Name = L"TTSSpeedLabel";
		TTSSpeedLabel->Size = System::Drawing::Size(78, 17);
		TTSSpeedLabel->TabIndex = 11;
		TTSSpeedLabel->Text = L"TTS speed";
		TTSSpeedLabel->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		//
		// SRHotKeyLabel
		//
		SRHotKeyLabel->AutoSize = true;
		SRHotKeyLabel->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->SettingsDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		SRHotKeyLabel->Location = System::Drawing::Point(154, 197);
		SRHotKeyLabel->Name = L"SRHotKeyLabel";
		SRHotKeyLabel->Size = System::Drawing::Size(56, 17);
		SRHotKeyLabel->TabIndex = 14;
		SRHotKeyLabel->Text = L"Hot key";
		SRHotKeyLabel->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		//
		// SRHotKeyTimeLabel
		//
		SRHotKeyTimeLabel->AutoSize = true;
		SRHotKeyTimeLabel->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->SettingsDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		SRHotKeyTimeLabel->Location = System::Drawing::Point(309, 197);
		SRHotKeyTimeLabel->Name = L"SRHotKeyTimeLabel";
		SRHotKeyTimeLabel->Size = System::Drawing::Size(39, 17);
		SRHotKeyTimeLabel->TabIndex = 16;
		SRHotKeyTimeLabel->Text = L"Time";
		SRHotKeyTimeLabel->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		//
		// PagePropertySheet
		//
		PagePropertySheet->Controls->Add(this->ShowDefaultCharacterY);
		PagePropertySheet->Controls->Add(label21);
		PagePropertySheet->Controls->Add(this->ShowDefaultCharacterX);
		PagePropertySheet->Controls->Add(label20);
		PagePropertySheet->Controls->Add(this->ShowDefaultCharacterPos);
		PagePropertySheet->Controls->Add(this->ShowDefaultCharacter);
		PagePropertySheet->Controls->Add(label19);
		PagePropertySheet->Controls->Add(label10);
		PagePropertySheet->Controls->Add(this->PropertySheetBound);
		PagePropertySheet->Controls->Add(this->PropSheetVisible);
		PagePropertySheet->Controls->Add(PropSheetPageLabel);
		PagePropertySheet->Controls->Add(this->PropSheetPage);
		PagePropertySheet->Controls->Add(PropSheetLeftLabel);
		PagePropertySheet->Controls->Add(this->PropSheetLeft);
		PagePropertySheet->Controls->Add(PropSheetTopLabel);
		PagePropertySheet->Controls->Add(this->PropSheetTop);
		PagePropertySheet->Controls->Add(PropSheetWidthLabel);
		PagePropertySheet->Controls->Add(this->PropSheetWidth);
		PagePropertySheet->Controls->Add(PropSheetHeightLabel);
		PagePropertySheet->Controls->Add(this->PropSheetHeight);
		PagePropertySheet->Location = System::Drawing::Point(4, 25);
		PagePropertySheet->Name = L"PagePropertySheet";
		PagePropertySheet->Padding = System::Windows::Forms::Padding(3);
		PagePropertySheet->Size = System::Drawing::Size(640, 481);
		PagePropertySheet->TabIndex = 1;
		PagePropertySheet->Text = L"Property Sheet";
		PagePropertySheet->UseVisualStyleBackColor = true;
		//
		// ShowDefaultCharacterY
		//
		this->ShowDefaultCharacterY->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 0});
		this->ShowDefaultCharacterY->Location = System::Drawing::Point(421, 156);
		this->ShowDefaultCharacterY->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {2000, 0, 0, 0});
		this->ShowDefaultCharacterY->Name = L"ShowDefaultCharacterY";
		this->ShowDefaultCharacterY->Size = System::Drawing::Size(58, 22);
		this->ShowDefaultCharacterY->TabIndex = 34;
		//
		// label21
		//
		label21->AutoSize = true;
		label21->Location = System::Drawing::Point(398, 158);
		label21->Name = L"label21";
		label21->Size = System::Drawing::Size(17, 17);
		label21->TabIndex = 33;
		label21->Text = L"Y";
		//
		// ShowDefaultCharacterX
		//
		this->ShowDefaultCharacterX->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 0});
		this->ShowDefaultCharacterX->Location = System::Drawing::Point(331, 156);
		this->ShowDefaultCharacterX->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {2000, 0, 0, 0});
		this->ShowDefaultCharacterX->Name = L"ShowDefaultCharacterX";
		this->ShowDefaultCharacterX->Size = System::Drawing::Size(61, 22);
		this->ShowDefaultCharacterX->TabIndex = 32;
		//
		// label20
		//
		label20->AutoSize = true;
		label20->Location = System::Drawing::Point(308, 158);
		label20->Name = L"label20";
		label20->Size = System::Drawing::Size(17, 17);
		label20->TabIndex = 31;
		label20->Text = L"X";
		//
		// ShowDefaultCharacterPos
		//
		this->ShowDefaultCharacterPos->AutoSize = true;
		this->ShowDefaultCharacterPos->Location = System::Drawing::Point(209, 157);
		this->ShowDefaultCharacterPos->Name = L"ShowDefaultCharacterPos";
		this->ShowDefaultCharacterPos->Size = System::Drawing::Size(80, 21);
		this->ShowDefaultCharacterPos->TabIndex = 30;
		this->ShowDefaultCharacterPos->Text = L"Position";
		this->ShowDefaultCharacterPos->UseVisualStyleBackColor = true;
		//
		// ShowDefaultCharacter
		//
		this->ShowDefaultCharacter->Location = System::Drawing::Point(6, 153);
		this->ShowDefaultCharacter->Name = L"ShowDefaultCharacter";
		this->ShowDefaultCharacter->Size = System::Drawing::Size(182, 27);
		this->ShowDefaultCharacter->TabIndex = 29;
		this->ShowDefaultCharacter->Text = L"ShowDefaultCharacter";
		this->ShowDefaultCharacter->UseVisualStyleBackColor = true;
		this->ShowDefaultCharacter->Click += gcnew System::EventHandler(this, &ControlTestForm::ShowDefaultCharacter_Click);
		//
		// label19
		//
		label19->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		label19->BackColor = System::Drawing::SystemColors::ActiveCaption;
		label19->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
		label19->Location = System::Drawing::Point(6, 135);
		label19->Name = L"label19";
		label19->Size = System::Drawing::Size(624, 3);
		label19->TabIndex = 28;
		//
		// label10
		//
		label10->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		label10->BackColor = System::Drawing::SystemColors::ActiveCaption;
		label10->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
		label10->Location = System::Drawing::Point(6, 40);
		label10->Name = L"label10";
		label10->Size = System::Drawing::Size(624, 3);
		label10->TabIndex = 28;
		//
		// PropertySheetBound
		//
		this->PropertySheetBound->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
		this->PropertySheetBound->Appearance = System::Windows::Forms::Appearance::Button;
		this->PropertySheetBound->AutoSize = true;
		this->PropertySheetBound->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->PropertySheetDataBinding,
			L"Bound", true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->PropertySheetBound->Location = System::Drawing::Point(575, 6);
		this->PropertySheetBound->Name = L"PropertySheetBound";
		this->PropertySheetBound->Size = System::Drawing::Size(59, 27);
		this->PropertySheetBound->TabIndex = 0;
		this->PropertySheetBound->Text = L"Bound";
		this->PropertySheetBound->UseVisualStyleBackColor = true;
		//
		// PropSheetVisible
		//
		this->PropSheetVisible->AutoSize = true;
		this->PropSheetVisible->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->PropertySheetDataBinding,
			L"Bound", true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->PropSheetVisible->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->PropertySheetDataBinding,
			L"Visible", true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->PropSheetVisible->Enabled = false;
		this->PropSheetVisible->Location = System::Drawing::Point(6, 11);
		this->PropSheetVisible->Name = L"PropSheetVisible";
		this->PropSheetVisible->Size = System::Drawing::Size(71, 21);
		this->PropSheetVisible->TabIndex = 1;
		this->PropSheetVisible->Text = L"Visible";
		this->PropSheetVisible->UseVisualStyleBackColor = true;
		//
		// PropSheetPageLabel
		//
		PropSheetPageLabel->AutoSize = true;
		PropSheetPageLabel->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->PropertySheetDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		PropSheetPageLabel->Location = System::Drawing::Point(116, 13);
		PropSheetPageLabel->Name = L"PropSheetPageLabel";
		PropSheetPageLabel->Size = System::Drawing::Size(41, 17);
		PropSheetPageLabel->TabIndex = 2;
		PropSheetPageLabel->Text = L"Page";
		PropSheetPageLabel->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
		//
		// PropSheetPage
		//
		this->PropSheetPage->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->PropertySheetDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->PropSheetPage->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", this->PropertySheetDataBinding, L"Page",
			true)));
		this->PropSheetPage->Location = System::Drawing::Point(163, 10);
		this->PropSheetPage->Name = L"PropSheetPage";
		this->PropSheetPage->Size = System::Drawing::Size(213, 22);
		this->PropSheetPage->TabIndex = 3;
		//
		// PropSheetLeftLabel
		//
		PropSheetLeftLabel->AutoSize = true;
		PropSheetLeftLabel->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->PropertySheetDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		PropSheetLeftLabel->Location = System::Drawing::Point(40, 81);
		PropSheetLeftLabel->Name = L"PropSheetLeftLabel";
		PropSheetLeftLabel->Size = System::Drawing::Size(32, 17);
		PropSheetLeftLabel->TabIndex = 4;
		PropSheetLeftLabel->Text = L"Left";
		PropSheetLeftLabel->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
		//
		// PropSheetLeft
		//
		this->PropSheetLeft->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->PropertySheetDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->PropSheetLeft->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Value", this->PropertySheetDataBinding, L"Left",
			true)));
		this->PropSheetLeft->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 0});
		this->PropSheetLeft->Location = System::Drawing::Point(78, 78);
		this->PropSheetLeft->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {2000, 0, 0, 0});
		this->PropSheetLeft->Name = L"PropSheetLeft";
		this->PropSheetLeft->Size = System::Drawing::Size(80, 22);
		this->PropSheetLeft->TabIndex = 5;
		//
		// PropSheetTopLabel
		//
		PropSheetTopLabel->AutoSize = true;
		PropSheetTopLabel->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->PropertySheetDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		PropSheetTopLabel->Location = System::Drawing::Point(124, 53);
		PropSheetTopLabel->Name = L"PropSheetTopLabel";
		PropSheetTopLabel->Size = System::Drawing::Size(33, 17);
		PropSheetTopLabel->TabIndex = 6;
		PropSheetTopLabel->Text = L"Top";
		PropSheetTopLabel->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
		//
		// PropSheetTop
		//
		this->PropSheetTop->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->PropertySheetDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->PropSheetTop->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Value", this->PropertySheetDataBinding, L"Top",
			true)));
		this->PropSheetTop->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 0});
		this->PropSheetTop->Location = System::Drawing::Point(163, 50);
		this->PropSheetTop->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {2000, 0, 0, 0});
		this->PropSheetTop->Name = L"PropSheetTop";
		this->PropSheetTop->Size = System::Drawing::Size(80, 22);
		this->PropSheetTop->TabIndex = 7;
		//
		// PropSheetWidthLabel
		//
		PropSheetWidthLabel->AutoSize = true;
		PropSheetWidthLabel->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->PropertySheetDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		PropSheetWidthLabel->Location = System::Drawing::Point(113, 109);
		PropSheetWidthLabel->Name = L"PropSheetWidthLabel";
		PropSheetWidthLabel->Size = System::Drawing::Size(44, 17);
		PropSheetWidthLabel->TabIndex = 8;
		PropSheetWidthLabel->Text = L"Width";
		PropSheetWidthLabel->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
		//
		// PropSheetWidth
		//
		this->PropSheetWidth->BackColor = System::Drawing::SystemColors::Window;
		this->PropSheetWidth->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->PropertySheetDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->PropSheetWidth->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", this->PropertySheetDataBinding, L"Width",
			true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->PropSheetWidth->ForeColor = System::Drawing::SystemColors::GrayText;
		this->PropSheetWidth->Location = System::Drawing::Point(163, 106);
		this->PropSheetWidth->Name = L"PropSheetWidth";
		this->PropSheetWidth->ReadOnly = true;
		this->PropSheetWidth->Size = System::Drawing::Size(80, 22);
		this->PropSheetWidth->TabIndex = 9;
		this->PropSheetWidth->TabStop = false;
		//
		// PropSheetHeightLabel
		//
		PropSheetHeightLabel->AutoSize = true;
		PropSheetHeightLabel->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->PropertySheetDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		PropSheetHeightLabel->Location = System::Drawing::Point(194, 81);
		PropSheetHeightLabel->Name = L"PropSheetHeightLabel";
		PropSheetHeightLabel->Size = System::Drawing::Size(49, 17);
		PropSheetHeightLabel->TabIndex = 10;
		PropSheetHeightLabel->Text = L"Height";
		PropSheetHeightLabel->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
		//
		// PropSheetHeight
		//
		this->PropSheetHeight->BackColor = System::Drawing::SystemColors::Window;
		this->PropSheetHeight->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->PropertySheetDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->PropSheetHeight->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", this->PropertySheetDataBinding, L"Height",
			true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->PropSheetHeight->ForeColor = System::Drawing::SystemColors::GrayText;
		this->PropSheetHeight->Location = System::Drawing::Point(249, 78);
		this->PropSheetHeight->Name = L"PropSheetHeight";
		this->PropSheetHeight->ReadOnly = true;
		this->PropSheetHeight->Size = System::Drawing::Size(80, 22);
		this->PropSheetHeight->TabIndex = 11;
		this->PropSheetHeight->TabStop = false;
		//
		// DefaultFileLabel
		//
		DefaultFileLabel->AutoSize = true;
		DefaultFileLabel->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterFilesDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		DefaultFileLabel->Location = System::Drawing::Point(3, 83);
		DefaultFileLabel->Name = L"DefaultFileLabel";
		DefaultFileLabel->Size = System::Drawing::Size(119, 17);
		DefaultFileLabel->TabIndex = 7;
		DefaultFileLabel->Text = L"Default Character";
		//
		// DefaultSearchLabel
		//
		DefaultSearchLabel->AutoSize = true;
		DefaultSearchLabel->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterFilesDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		DefaultSearchLabel->Location = System::Drawing::Point(3, 115);
		DefaultSearchLabel->Name = L"DefaultSearchLabel";
		DefaultSearchLabel->Size = System::Drawing::Size(102, 17);
		DefaultSearchLabel->TabIndex = 9;
		DefaultSearchLabel->Text = L"Default Search";
		//
		// CurrentSearchLabel
		//
		CurrentSearchLabel->AutoSize = true;
		CurrentSearchLabel->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterFilesDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		CurrentSearchLabel->Location = System::Drawing::Point(3, 145);
		CurrentSearchLabel->Name = L"CurrentSearchLabel";
		CurrentSearchLabel->Size = System::Drawing::Size(104, 17);
		CurrentSearchLabel->TabIndex = 11;
		CurrentSearchLabel->Text = L"Current Search";
		//
		// CharBalloonNumberOfLinesLabel
		//
		CharBalloonNumberOfLinesLabel->AutoSize = true;
		CharBalloonNumberOfLinesLabel->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding,
			L"Bound", true)));
		CharBalloonNumberOfLinesLabel->Location = System::Drawing::Point(407, 12);
		CharBalloonNumberOfLinesLabel->Name = L"CharBalloonNumberOfLinesLabel";
		CharBalloonNumberOfLinesLabel->Size = System::Drawing::Size(107, 17);
		CharBalloonNumberOfLinesLabel->TabIndex = 6;
		CharBalloonNumberOfLinesLabel->Text = L"NumberOfLines";
		CharBalloonNumberOfLinesLabel->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
		//
		// label1
		//
		label1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		label1->BackColor = System::Drawing::SystemColors::ActiveCaption;
		label1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
		label1->Location = System::Drawing::Point(6, 67);
		label1->Name = L"label1";
		label1->Size = System::Drawing::Size(624, 3);
		label1->TabIndex = 26;
		//
		// label2
		//
		label2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		label2->BackColor = System::Drawing::SystemColors::ActiveCaption;
		label2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
		label2->Location = System::Drawing::Point(6, 106);
		label2->Name = L"label2";
		label2->Size = System::Drawing::Size(624, 3);
		label2->TabIndex = 26;
		//
		// label3
		//
		label3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		label3->BackColor = System::Drawing::SystemColors::ActiveCaption;
		label3->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
		label3->Location = System::Drawing::Point(6, 201);
		label3->Name = L"label3";
		label3->Size = System::Drawing::Size(620, 3);
		label3->TabIndex = 20;
		//
		// label4
		//
		label4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		label4->BackColor = System::Drawing::SystemColors::ActiveCaption;
		label4->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
		label4->Location = System::Drawing::Point(6, 332);
		label4->Name = L"label4";
		label4->Size = System::Drawing::Size(620, 3);
		label4->TabIndex = 53;
		//
		// label5
		//
		label5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		label5->BackColor = System::Drawing::SystemColors::ActiveCaption;
		label5->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
		label5->Location = System::Drawing::Point(6, 264);
		label5->Name = L"label5";
		label5->Size = System::Drawing::Size(620, 3);
		label5->TabIndex = 27;
		//
		// label6
		//
		label6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		label6->BackColor = System::Drawing::SystemColors::ActiveCaption;
		label6->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
		label6->Location = System::Drawing::Point(6, 377);
		label6->Name = L"label6";
		label6->Size = System::Drawing::Size(620, 3);
		label6->TabIndex = 45;
		//
		// label7
		//
		label7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		label7->BackColor = System::Drawing::SystemColors::ActiveCaption;
		label7->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
		label7->Location = System::Drawing::Point(6, 98);
		label7->Name = L"label7";
		label7->Size = System::Drawing::Size(624, 3);
		label7->TabIndex = 27;
		//
		// label8
		//
		label8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		label8->BackColor = System::Drawing::SystemColors::ActiveCaption;
		label8->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
		label8->Location = System::Drawing::Point(6, 136);
		label8->Name = L"label8";
		label8->Size = System::Drawing::Size(624, 3);
		label8->TabIndex = 27;
		//
		// label9
		//
		label9->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		label9->BackColor = System::Drawing::SystemColors::ActiveCaption;
		label9->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
		label9->Location = System::Drawing::Point(6, 180);
		label9->Name = L"label9";
		label9->Size = System::Drawing::Size(624, 3);
		label9->TabIndex = 27;
		//
		// label11
		//
		label11->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		label11->BackColor = System::Drawing::SystemColors::ActiveCaption;
		label11->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
		label11->Location = System::Drawing::Point(6, 65);
		label11->Name = L"label11";
		label11->Size = System::Drawing::Size(624, 3);
		label11->TabIndex = 28;
		//
		// label12
		//
		label12->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		label12->BackColor = System::Drawing::SystemColors::ActiveCaption;
		label12->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
		label12->Location = System::Drawing::Point(6, 174);
		label12->Name = L"label12";
		label12->Size = System::Drawing::Size(624, 3);
		label12->TabIndex = 28;
		//
		// label13
		//
		label13->AutoSize = true;
		label13->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound", true)));
		label13->Location = System::Drawing::Point(6, 15);
		label13->Name = L"label13";
		label13->Size = System::Drawing::Size(56, 17);
		label13->TabIndex = 0;
		label13->Text = L"Caption";
		//
		// label14
		//
		label14->AutoSize = true;
		label14->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound", true)));
		label14->Location = System::Drawing::Point(340, 15);
		label14->Name = L"label14";
		label14->Size = System::Drawing::Size(91, 17);
		label14->TabIndex = 2;
		label14->Text = L"VoiceCaption";
		label14->TextAlign = System::Drawing::ContentAlignment::TopRight;
		//
		// label15
		//
		label15->AutoSize = true;
		label15->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound", true)));
		label15->Location = System::Drawing::Point(6, 43);
		label15->Name = L"label15";
		label15->Size = System::Drawing::Size(102, 17);
		label15->TabIndex = 4;
		label15->Text = L"VoiceGrammar";
		//
		// label16
		//
		label16->AutoSize = true;
		label16->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound", true)));
		label16->Location = System::Drawing::Point(6, 71);
		label16->Name = L"label16";
		label16->Size = System::Drawing::Size(73, 17);
		label16->TabIndex = 6;
		label16->Text = L"FontName";
		//
		// label17
		//
		label17->AutoSize = true;
		label17->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound", true)));
		label17->Location = System::Drawing::Point(368, 71);
		label17->Name = L"label17";
		label17->Size = System::Drawing::Size(63, 17);
		label17->TabIndex = 8;
		label17->Text = L"FontSize";
		label17->TextAlign = System::Drawing::ContentAlignment::TopRight;
		//
		// label18
		//
		label18->AutoSize = true;
		label18->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound", true)));
		label18->Location = System::Drawing::Point(8, 99);
		label18->Name = L"label18";
		label18->Size = System::Drawing::Size(116, 17);
		label18->TabIndex = 10;
		label18->Text = L"DefaultCommand";
		//
		// TabsControl
		//
		this->TabsControl->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
			| System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		this->TabsControl->Controls->Add(this->PageCharacters);
		this->TabsControl->Controls->Add(this->PageSettings);
		this->TabsControl->Controls->Add(PagePropertySheet);
		this->TabsControl->Controls->Add(this->PageCharacterFiles);
		this->TabsControl->Controls->Add(this->PageTTSEngines);
		this->TabsControl->Controls->Add(this->PageSREngines);
		this->TabsControl->Location = System::Drawing::Point(3, 6);
		this->TabsControl->Name = L"TabsControl";
		this->TabsControl->SelectedIndex = 0;
		this->TabsControl->Size = System::Drawing::Size(648, 510);
		this->TabsControl->TabIndex = 0;
		//
		// PageCharacters
		//
		this->PageCharacters->Controls->Add(this->CharacterList);
		this->PageCharacters->Controls->Add(this->CharactersBound);
		this->PageCharacters->Location = System::Drawing::Point(4, 25);
		this->PageCharacters->Name = L"PageCharacters";
		this->PageCharacters->Padding = System::Windows::Forms::Padding(3);
		this->PageCharacters->Size = System::Drawing::Size(640, 481);
		this->PageCharacters->TabIndex = 0;
		this->PageCharacters->Text = L"Characters";
		this->PageCharacters->UseVisualStyleBackColor = true;
		//
		// CharacterList
		//
		this->CharacterList->AllowUserToAddRows = false;
		this->CharacterList->AllowUserToDeleteRows = false;
		this->CharacterList->AllowUserToResizeRows = false;
		this->CharacterList->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
			| System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		this->CharacterList->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::AllCells;
		this->CharacterList->BackgroundColor = System::Drawing::SystemColors::Control;
		this->CharacterList->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
		this->CharacterList->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(6) {this->CharListID,
			this->CharListName, this->CharListFileName, this->CharListFilePath, this->CharListVersion, this->CharListUniqueID});
		this->CharacterList->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharactersDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharacterList->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"DataSource", this->CharactersDataBinding, L"CharactersList",
			true)));
		this->CharacterList->Location = System::Drawing::Point(6, 39);
		this->CharacterList->MultiSelect = false;
		this->CharacterList->Name = L"CharacterList";
		this->CharacterList->ReadOnly = true;
		this->CharacterList->RowHeadersVisible = false;
		this->CharacterList->RowTemplate->Height = 24;
		this->CharacterList->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
		this->CharacterList->Size = System::Drawing::Size(628, 436);
		this->CharacterList->StandardTab = true;
		this->CharacterList->TabIndex = 2;
		this->CharacterList->DataBindingComplete += gcnew System::Windows::Forms::DataGridViewBindingCompleteEventHandler(this, &ControlTestForm::CharacterList_DataBindingComplete);
		this->CharacterList->SelectionChanged += gcnew System::EventHandler(this, &ControlTestForm::CharacterList_SelectionChanged);
		//
		// CharListID
		//
		this->CharListID->DataPropertyName = L"CharacterID";
		this->CharListID->HeaderText = L"ID";
		this->CharListID->Name = L"CharListID";
		this->CharListID->ReadOnly = true;
		this->CharListID->Width = 46;
		//
		// CharListName
		//
		this->CharListName->DataPropertyName = L"Name";
		this->CharListName->HeaderText = L"Name";
		this->CharListName->Name = L"CharListName";
		this->CharListName->ReadOnly = true;
		this->CharListName->Width = 70;
		//
		// CharListFileName
		//
		this->CharListFileName->DataPropertyName = L"FileName";
		this->CharListFileName->HeaderText = L"File";
		this->CharListFileName->Name = L"CharListFileName";
		this->CharListFileName->ReadOnly = true;
		this->CharListFileName->Width = 55;
		//
		// CharListFilePath
		//
		this->CharListFilePath->DataPropertyName = L"FilePath";
		this->CharListFilePath->HeaderText = L"Path";
		this->CharListFilePath->Name = L"CharListFilePath";
		this->CharListFilePath->ReadOnly = true;
		this->CharListFilePath->Width = 62;
		//
		// CharListVersion
		//
		this->CharListVersion->DataPropertyName = L"Version";
		this->CharListVersion->HeaderText = L"Version";
		this->CharListVersion->Name = L"CharListVersion";
		this->CharListVersion->ReadOnly = true;
		this->CharListVersion->Width = 81;
		//
		// CharListUniqueID
		//
		this->CharListUniqueID->DataPropertyName = L"UniqueID";
		this->CharListUniqueID->HeaderText = L"UniqueID";
		this->CharListUniqueID->Name = L"CharListUniqueID";
		this->CharListUniqueID->ReadOnly = true;
		this->CharListUniqueID->Width = 91;
		//
		// CharactersBound
		//
		this->CharactersBound->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
		this->CharactersBound->Appearance = System::Windows::Forms::Appearance::Button;
		this->CharactersBound->AutoSize = true;
		this->CharactersBound->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->CharactersDataBinding,
			L"Bound", true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->CharactersBound->Location = System::Drawing::Point(575, 6);
		this->CharactersBound->Name = L"CharactersBound";
		this->CharactersBound->Size = System::Drawing::Size(59, 27);
		this->CharactersBound->TabIndex = 1;
		this->CharactersBound->Text = L"Bound";
		this->CharactersBound->UseVisualStyleBackColor = true;
		//
		// PageSettings
		//
		this->PageSettings->Controls->Add(label9);
		this->PageSettings->Controls->Add(label8);
		this->PageSettings->Controls->Add(label7);
		this->PageSettings->Controls->Add(this->SettingsBound);
		this->PageSettings->Controls->Add(this->BalloonEnabled);
		this->PageSettings->Controls->Add(BalloonFontLabel);
		this->PageSettings->Controls->Add(this->BalloonFontName);
		this->PageSettings->Controls->Add(BalloonFontSizeLabel);
		this->PageSettings->Controls->Add(this->BalloonFontSize);
		this->PageSettings->Controls->Add(this->BalloonFontBold);
		this->PageSettings->Controls->Add(this->BalloonFontItalic);
		this->PageSettings->Controls->Add(this->BalloonFontUnderline);
		this->PageSettings->Controls->Add(this->SoundEffectsEnabled);
		this->PageSettings->Controls->Add(this->TTSEnabled);
		this->PageSettings->Controls->Add(TTSSpeedLabel);
		this->PageSettings->Controls->Add(this->TTSSpeed);
		this->PageSettings->Controls->Add(this->SREnabled);
		this->PageSettings->Controls->Add(SRHotKeyLabel);
		this->PageSettings->Controls->Add(this->SRHotKey);
		this->PageSettings->Controls->Add(SRHotKeyTimeLabel);
		this->PageSettings->Controls->Add(this->SRHotKeyTime);
		this->PageSettings->Controls->Add(this->SRListeningPrompt);
		this->PageSettings->Controls->Add(this->SRListeningTip);
		this->PageSettings->Location = System::Drawing::Point(4, 25);
		this->PageSettings->Name = L"PageSettings";
		this->PageSettings->Padding = System::Windows::Forms::Padding(3);
		this->PageSettings->Size = System::Drawing::Size(640, 481);
		this->PageSettings->TabIndex = 0;
		this->PageSettings->Text = L"Settings";
		this->PageSettings->UseVisualStyleBackColor = true;
		//
		// SettingsBound
		//
		this->SettingsBound->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
		this->SettingsBound->Appearance = System::Windows::Forms::Appearance::Button;
		this->SettingsBound->AutoSize = true;
		this->SettingsBound->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->SettingsDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->SettingsBound->Location = System::Drawing::Point(575, 6);
		this->SettingsBound->Name = L"SettingsBound";
		this->SettingsBound->Size = System::Drawing::Size(59, 27);
		this->SettingsBound->TabIndex = 0;
		this->SettingsBound->Text = L"Bound";
		this->SettingsBound->UseVisualStyleBackColor = true;
		//
		// BalloonEnabled
		//
		this->BalloonEnabled->AutoCheck = false;
		this->BalloonEnabled->AutoSize = true;
		this->BalloonEnabled->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->SettingsDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->BalloonEnabled->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->SettingsDataBinding, L"BalloonEnabled",
			true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->BalloonEnabled->Enabled = false;
		this->BalloonEnabled->Location = System::Drawing::Point(6, 12);
		this->BalloonEnabled->Name = L"BalloonEnabled";
		this->BalloonEnabled->Size = System::Drawing::Size(132, 21);
		this->BalloonEnabled->TabIndex = 1;
		this->BalloonEnabled->Text = L"Balloon enabled";
		this->BalloonEnabled->UseVisualStyleBackColor = true;
		//
		// BalloonFontName
		//
		this->BalloonFontName->BackColor = System::Drawing::SystemColors::Window;
		this->BalloonFontName->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", this->SettingsDataBinding, L"BalloonFontName",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->BalloonFontName->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->SettingsDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->BalloonFontName->Enabled = false;
		this->BalloonFontName->ForeColor = System::Drawing::SystemColors::GrayText;
		this->BalloonFontName->Location = System::Drawing::Point(196, 10);
		this->BalloonFontName->Name = L"BalloonFontName";
		this->BalloonFontName->ReadOnly = true;
		this->BalloonFontName->Size = System::Drawing::Size(333, 22);
		this->BalloonFontName->TabIndex = 3;
		this->BalloonFontName->TabStop = false;
		//
		// BalloonFontSize
		//
		this->BalloonFontSize->BackColor = System::Drawing::SystemColors::Window;
		this->BalloonFontSize->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Value", this->SettingsDataBinding, L"BalloonFontSize",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->BalloonFontSize->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->SettingsDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->BalloonFontSize->Enabled = false;
		this->BalloonFontSize->ForeColor = System::Drawing::SystemColors::GrayText;
		this->BalloonFontSize->Location = System::Drawing::Point(196, 41);
		this->BalloonFontSize->Name = L"BalloonFontSize";
		this->BalloonFontSize->ReadOnly = true;
		this->BalloonFontSize->Size = System::Drawing::Size(87, 22);
		this->BalloonFontSize->TabIndex = 5;
		this->BalloonFontSize->TabStop = false;
		//
		// BalloonFontBold
		//
		this->BalloonFontBold->AutoCheck = false;
		this->BalloonFontBold->AutoSize = true;
		this->BalloonFontBold->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->SettingsDataBinding, L"BalloonFontBold",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->BalloonFontBold->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->SettingsDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->BalloonFontBold->Enabled = false;
		this->BalloonFontBold->ForeColor = System::Drawing::SystemColors::GrayText;
		this->BalloonFontBold->Location = System::Drawing::Point(158, 69);
		this->BalloonFontBold->Name = L"BalloonFontBold";
		this->BalloonFontBold->Size = System::Drawing::Size(58, 21);
		this->BalloonFontBold->TabIndex = 6;
		this->BalloonFontBold->TabStop = false;
		this->BalloonFontBold->Text = L"Bold";
		this->BalloonFontBold->UseVisualStyleBackColor = true;
		//
		// BalloonFontItalic
		//
		this->BalloonFontItalic->AutoCheck = false;
		this->BalloonFontItalic->AutoSize = true;
		this->BalloonFontItalic->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->SettingsDataBinding, L"BalloonFontItalic",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->BalloonFontItalic->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->SettingsDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->BalloonFontItalic->Enabled = false;
		this->BalloonFontItalic->ForeColor = System::Drawing::SystemColors::GrayText;
		this->BalloonFontItalic->Location = System::Drawing::Point(222, 69);
		this->BalloonFontItalic->Name = L"BalloonFontItalic";
		this->BalloonFontItalic->Size = System::Drawing::Size(58, 21);
		this->BalloonFontItalic->TabIndex = 7;
		this->BalloonFontItalic->TabStop = false;
		this->BalloonFontItalic->Text = L"Italic";
		this->BalloonFontItalic->UseVisualStyleBackColor = true;
		//
		// BalloonFontUnderline
		//
		this->BalloonFontUnderline->AutoCheck = false;
		this->BalloonFontUnderline->AutoSize = true;
		this->BalloonFontUnderline->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->SettingsDataBinding, L"BalloonFontUnderline",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->BalloonFontUnderline->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->SettingsDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->BalloonFontUnderline->Enabled = false;
		this->BalloonFontUnderline->ForeColor = System::Drawing::SystemColors::GrayText;
		this->BalloonFontUnderline->Location = System::Drawing::Point(286, 69);
		this->BalloonFontUnderline->Name = L"BalloonFontUnderline";
		this->BalloonFontUnderline->Size = System::Drawing::Size(91, 21);
		this->BalloonFontUnderline->TabIndex = 8;
		this->BalloonFontUnderline->TabStop = false;
		this->BalloonFontUnderline->Text = L"Underline";
		this->BalloonFontUnderline->UseVisualStyleBackColor = true;
		//
		// SoundEffectsEnabled
		//
		this->SoundEffectsEnabled->AutoCheck = false;
		this->SoundEffectsEnabled->AutoSize = true;
		this->SoundEffectsEnabled->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->SettingsDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->SoundEffectsEnabled->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->SettingsDataBinding, L"SoundEffectsEnabled",
			true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->SoundEffectsEnabled->Enabled = false;
		this->SoundEffectsEnabled->Location = System::Drawing::Point(6, 108);
		this->SoundEffectsEnabled->Name = L"SoundEffectsEnabled";
		this->SoundEffectsEnabled->Size = System::Drawing::Size(172, 21);
		this->SoundEffectsEnabled->TabIndex = 9;
		this->SoundEffectsEnabled->Text = L"Sound effects enabled";
		this->SoundEffectsEnabled->UseVisualStyleBackColor = true;
		//
		// TTSEnabled
		//
		this->TTSEnabled->AutoCheck = false;
		this->TTSEnabled->AutoSize = true;
		this->TTSEnabled->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->SettingsDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->TTSEnabled->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->SettingsDataBinding, L"TTSEnabled",
			true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->TTSEnabled->Enabled = false;
		this->TTSEnabled->Location = System::Drawing::Point(6, 150);
		this->TTSEnabled->Name = L"TTSEnabled";
		this->TTSEnabled->Size = System::Drawing::Size(112, 21);
		this->TTSEnabled->TabIndex = 10;
		this->TTSEnabled->Text = L"TTS enabled";
		this->TTSEnabled->UseVisualStyleBackColor = true;
		//
		// TTSSpeed
		//
		this->TTSSpeed->BackColor = System::Drawing::SystemColors::Window;
		this->TTSSpeed->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", this->SettingsDataBinding, L"TTSSpeed", true,
			System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->TTSSpeed->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->SettingsDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->TTSSpeed->Enabled = false;
		this->TTSSpeed->ForeColor = System::Drawing::SystemColors::GrayText;
		this->TTSSpeed->Location = System::Drawing::Point(244, 148);
		this->TTSSpeed->Name = L"TTSSpeed";
		this->TTSSpeed->ReadOnly = true;
		this->TTSSpeed->Size = System::Drawing::Size(87, 22);
		this->TTSSpeed->TabIndex = 12;
		this->TTSSpeed->TabStop = false;
		//
		// SREnabled
		//
		this->SREnabled->AutoCheck = false;
		this->SREnabled->AutoSize = true;
		this->SREnabled->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->SettingsDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->SREnabled->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->SettingsDataBinding, L"SREnabled",
			true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->SREnabled->Enabled = false;
		this->SREnabled->Location = System::Drawing::Point(6, 194);
		this->SREnabled->Name = L"SREnabled";
		this->SREnabled->Size = System::Drawing::Size(104, 21);
		this->SREnabled->TabIndex = 13;
		this->SREnabled->Text = L"SR enabled";
		this->SREnabled->UseVisualStyleBackColor = true;
		//
		// SRHotKey
		//
		this->SRHotKey->BackColor = System::Drawing::SystemColors::Window;
		this->SRHotKey->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->SettingsDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->SRHotKey->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", this->SettingsDataBinding, L"SRHotKey", true)));
		this->SRHotKey->Enabled = false;
		this->SRHotKey->ForeColor = System::Drawing::SystemColors::GrayText;
		this->SRHotKey->Location = System::Drawing::Point(216, 195);
		this->SRHotKey->Name = L"SRHotKey";
		this->SRHotKey->ReadOnly = true;
		this->SRHotKey->Size = System::Drawing::Size(87, 22);
		this->SRHotKey->TabIndex = 15;
		this->SRHotKey->TabStop = false;
		//
		// SRHotKeyTime
		//
		this->SRHotKeyTime->BackColor = System::Drawing::SystemColors::Window;
		this->SRHotKeyTime->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->SettingsDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->SRHotKeyTime->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", this->SettingsDataBinding, L"SRHotKeyTime",
			true)));
		this->SRHotKeyTime->Enabled = false;
		this->SRHotKeyTime->ForeColor = System::Drawing::SystemColors::GrayText;
		this->SRHotKeyTime->Location = System::Drawing::Point(354, 195);
		this->SRHotKeyTime->Name = L"SRHotKeyTime";
		this->SRHotKeyTime->ReadOnly = true;
		this->SRHotKeyTime->Size = System::Drawing::Size(87, 22);
		this->SRHotKeyTime->TabIndex = 17;
		this->SRHotKeyTime->TabStop = false;
		//
		// SRListeningPrompt
		//
		this->SRListeningPrompt->AutoCheck = false;
		this->SRListeningPrompt->AutoSize = true;
		this->SRListeningPrompt->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->SettingsDataBinding, L"SRListeningPrompt",
			true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->SRListeningPrompt->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->SettingsDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->SRListeningPrompt->Enabled = false;
		this->SRListeningPrompt->Location = System::Drawing::Point(157, 223);
		this->SRListeningPrompt->Name = L"SRListeningPrompt";
		this->SRListeningPrompt->Size = System::Drawing::Size(135, 21);
		this->SRListeningPrompt->TabIndex = 18;
		this->SRListeningPrompt->Text = L"Listening prompt";
		this->SRListeningPrompt->UseVisualStyleBackColor = true;
		//
		// SRListeningTip
		//
		this->SRListeningTip->AutoCheck = false;
		this->SRListeningTip->AutoSize = true;
		this->SRListeningTip->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->SettingsDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->SRListeningTip->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->SettingsDataBinding, L"SRListeningTip",
			true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->SRListeningTip->Enabled = false;
		this->SRListeningTip->Location = System::Drawing::Point(298, 223);
		this->SRListeningTip->Name = L"SRListeningTip";
		this->SRListeningTip->Size = System::Drawing::Size(106, 21);
		this->SRListeningTip->TabIndex = 19;
		this->SRListeningTip->Text = L"Listening tip";
		this->SRListeningTip->UseVisualStyleBackColor = true;
		//
		// PageCharacterFiles
		//
		this->PageCharacterFiles->Controls->Add(label12);
		this->PageCharacterFiles->Controls->Add(label11);
		this->PageCharacterFiles->Controls->Add(this->FilesBound);
		this->PageCharacterFiles->Controls->Add(this->ShowDaFiles);
		this->PageCharacterFiles->Controls->Add(this->ShowMaFiles);
		this->PageCharacterFiles->Controls->Add(this->ShowOfficeFiles);
		this->PageCharacterFiles->Controls->Add(this->ShowSpeakingFiles);
		this->PageCharacterFiles->Controls->Add(this->ShowSilentFiles);
		this->PageCharacterFiles->Controls->Add(this->VerifyFileVersion);
		this->PageCharacterFiles->Controls->Add(DefaultFileLabel);
		this->PageCharacterFiles->Controls->Add(this->DefaultFilePath);
		this->PageCharacterFiles->Controls->Add(DefaultSearchLabel);
		this->PageCharacterFiles->Controls->Add(this->DefaultSearchPath);
		this->PageCharacterFiles->Controls->Add(CurrentSearchLabel);
		this->PageCharacterFiles->Controls->Add(this->CurrentSearchPath);
		this->PageCharacterFiles->Controls->Add(this->FilesListBox);
		this->PageCharacterFiles->Location = System::Drawing::Point(4, 25);
		this->PageCharacterFiles->Name = L"PageCharacterFiles";
		this->PageCharacterFiles->Padding = System::Windows::Forms::Padding(3);
		this->PageCharacterFiles->Size = System::Drawing::Size(640, 481);
		this->PageCharacterFiles->TabIndex = 2;
		this->PageCharacterFiles->Text = L"Files";
		this->PageCharacterFiles->UseVisualStyleBackColor = true;
		//
		// FilesBound
		//
		this->FilesBound->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
		this->FilesBound->Appearance = System::Windows::Forms::Appearance::Button;
		this->FilesBound->AutoSize = true;
		this->FilesBound->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->CharacterFilesDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->FilesBound->Location = System::Drawing::Point(575, 6);
		this->FilesBound->Name = L"FilesBound";
		this->FilesBound->Size = System::Drawing::Size(59, 27);
		this->FilesBound->TabIndex = 0;
		this->FilesBound->Text = L"Bound";
		this->FilesBound->UseVisualStyleBackColor = true;
		//
		// ShowDaFiles
		//
		this->ShowDaFiles->AutoSize = true;
		this->ShowDaFiles->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterFilesDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->ShowDaFiles->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->CharacterFilesDataBinding, L"ShowDaFiles",
			true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->ShowDaFiles->Location = System::Drawing::Point(6, 10);
		this->ShowDaFiles->Name = L"ShowDaFiles";
		this->ShowDaFiles->Size = System::Drawing::Size(149, 21);
		this->ShowDaFiles->TabIndex = 1;
		this->ShowDaFiles->Text = L"Double Agent Files";
		this->ShowDaFiles->UseVisualStyleBackColor = true;
		//
		// ShowMaFiles
		//
		this->ShowMaFiles->AutoSize = true;
		this->ShowMaFiles->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterFilesDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->ShowMaFiles->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->CharacterFilesDataBinding, L"ShowMaFiles",
			true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->ShowMaFiles->Location = System::Drawing::Point(161, 10);
		this->ShowMaFiles->Name = L"ShowMaFiles";
		this->ShowMaFiles->Size = System::Drawing::Size(124, 21);
		this->ShowMaFiles->TabIndex = 2;
		this->ShowMaFiles->Text = L"MS Agent Files";
		this->ShowMaFiles->UseVisualStyleBackColor = true;
		//
		// ShowOfficeFiles
		//
		this->ShowOfficeFiles->AutoSize = true;
		this->ShowOfficeFiles->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterFilesDataBinding,
			L"Bound", true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->ShowOfficeFiles->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->CharacterFilesDataBinding,
			L"ShowOfficeFiles", true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->ShowOfficeFiles->Location = System::Drawing::Point(291, 10);
		this->ShowOfficeFiles->Name = L"ShowOfficeFiles";
		this->ShowOfficeFiles->Size = System::Drawing::Size(124, 21);
		this->ShowOfficeFiles->TabIndex = 3;
		this->ShowOfficeFiles->Text = L"MS Office Files";
		this->ShowOfficeFiles->UseVisualStyleBackColor = true;
		//
		// ShowSpeakingFiles
		//
		this->ShowSpeakingFiles->AutoSize = true;
		this->ShowSpeakingFiles->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterFilesDataBinding,
			L"Bound", true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->ShowSpeakingFiles->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->CharacterFilesDataBinding,
			L"ShowSpeakingFiles", true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->ShowSpeakingFiles->Location = System::Drawing::Point(6, 37);
		this->ShowSpeakingFiles->Name = L"ShowSpeakingFiles";
		this->ShowSpeakingFiles->Size = System::Drawing::Size(162, 21);
		this->ShowSpeakingFiles->TabIndex = 4;
		this->ShowSpeakingFiles->Text = L"Speaking Characters";
		this->ShowSpeakingFiles->UseVisualStyleBackColor = true;
		//
		// ShowSilentFiles
		//
		this->ShowSilentFiles->AutoSize = true;
		this->ShowSilentFiles->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterFilesDataBinding,
			L"Bound", true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->ShowSilentFiles->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->CharacterFilesDataBinding,
			L"ShowSilentFiles", true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->ShowSilentFiles->Location = System::Drawing::Point(174, 37);
		this->ShowSilentFiles->Name = L"ShowSilentFiles";
		this->ShowSilentFiles->Size = System::Drawing::Size(191, 21);
		this->ShowSilentFiles->TabIndex = 5;
		this->ShowSilentFiles->Text = L"Non-speaking Characters";
		this->ShowSilentFiles->UseVisualStyleBackColor = true;
		//
		// VerifyFileVersion
		//
		this->VerifyFileVersion->AutoSize = true;
		this->VerifyFileVersion->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterFilesDataBinding,
			L"Bound", true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->VerifyFileVersion->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->CharacterFilesDataBinding,
			L"VerifyFileVersion", true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->VerifyFileVersion->Location = System::Drawing::Point(371, 37);
		this->VerifyFileVersion->Name = L"VerifyFileVersion";
		this->VerifyFileVersion->Size = System::Drawing::Size(118, 21);
		this->VerifyFileVersion->TabIndex = 6;
		this->VerifyFileVersion->Text = L"Verify Version";
		this->VerifyFileVersion->UseVisualStyleBackColor = true;
		//
		// DefaultFilePath
		//
		this->DefaultFilePath->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		this->DefaultFilePath->BackColor = System::Drawing::SystemColors::Window;
		this->DefaultFilePath->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterFilesDataBinding,
			L"Bound", true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->DefaultFilePath->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", this->CharacterFilesDataBinding, L"DefaultFilePath",
			true)));
		this->DefaultFilePath->ForeColor = System::Drawing::SystemColors::GrayText;
		this->DefaultFilePath->Location = System::Drawing::Point(143, 80);
		this->DefaultFilePath->Name = L"DefaultFilePath";
		this->DefaultFilePath->ReadOnly = true;
		this->DefaultFilePath->Size = System::Drawing::Size(489, 22);
		this->DefaultFilePath->TabIndex = 8;
		this->DefaultFilePath->TabStop = false;
		//
		// DefaultSearchPath
		//
		this->DefaultSearchPath->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		this->DefaultSearchPath->BackColor = System::Drawing::SystemColors::Window;
		this->DefaultSearchPath->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterFilesDataBinding,
			L"Bound", true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->DefaultSearchPath->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", this->CharacterFilesDataBinding, L"DefaultSearchPath",
			true)));
		this->DefaultSearchPath->ForeColor = System::Drawing::SystemColors::GrayText;
		this->DefaultSearchPath->Location = System::Drawing::Point(142, 112);
		this->DefaultSearchPath->Name = L"DefaultSearchPath";
		this->DefaultSearchPath->ReadOnly = true;
		this->DefaultSearchPath->Size = System::Drawing::Size(490, 22);
		this->DefaultSearchPath->TabIndex = 10;
		this->DefaultSearchPath->TabStop = false;
		//
		// CurrentSearchPath
		//
		this->CurrentSearchPath->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		this->CurrentSearchPath->BackColor = System::Drawing::SystemColors::Window;
		this->CurrentSearchPath->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterFilesDataBinding,
			L"Bound", true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CurrentSearchPath->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", this->CharacterFilesDataBinding, L"CurrentSearchPath",
			true)));
		this->CurrentSearchPath->ForeColor = System::Drawing::SystemColors::GrayText;
		this->CurrentSearchPath->Location = System::Drawing::Point(143, 142);
		this->CurrentSearchPath->Name = L"CurrentSearchPath";
		this->CurrentSearchPath->ReadOnly = true;
		this->CurrentSearchPath->Size = System::Drawing::Size(489, 22);
		this->CurrentSearchPath->TabIndex = 12;
		this->CurrentSearchPath->TabStop = false;
		//
		// FilesListBox
		//
		this->FilesListBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
			| System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		this->FilesListBox->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterFilesDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->FilesListBox->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"DataSource", this->CharacterFilesDataBinding,
			L"FilePaths", true)));
		this->FilesListBox->FormattingEnabled = true;
		this->FilesListBox->IntegralHeight = false;
		this->FilesListBox->ItemHeight = 16;
		this->FilesListBox->Location = System::Drawing::Point(6, 186);
		this->FilesListBox->Name = L"FilesListBox";
		this->FilesListBox->Size = System::Drawing::Size(626, 289);
		this->FilesListBox->TabIndex = 13;
		//
		// PageTTSEngines
		//
		this->PageTTSEngines->Controls->Add(this->TTSEnginesBound);
		this->PageTTSEngines->Controls->Add(this->TTSEngineList);
		this->PageTTSEngines->Location = System::Drawing::Point(4, 25);
		this->PageTTSEngines->Name = L"PageTTSEngines";
		this->PageTTSEngines->Padding = System::Windows::Forms::Padding(3);
		this->PageTTSEngines->Size = System::Drawing::Size(640, 481);
		this->PageTTSEngines->TabIndex = 3;
		this->PageTTSEngines->Text = L"TTS Engines";
		this->PageTTSEngines->UseVisualStyleBackColor = true;
		//
		// TTSEnginesBound
		//
		this->TTSEnginesBound->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
		this->TTSEnginesBound->Appearance = System::Windows::Forms::Appearance::Button;
		this->TTSEnginesBound->AutoSize = true;
		this->TTSEnginesBound->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->TTSEnginesDataBinding,
			L"Bound", true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->TTSEnginesBound->Location = System::Drawing::Point(575, 6);
		this->TTSEnginesBound->Name = L"TTSEnginesBound";
		this->TTSEnginesBound->Size = System::Drawing::Size(59, 27);
		this->TTSEnginesBound->TabIndex = 0;
		this->TTSEnginesBound->Text = L"Bound";
		this->TTSEnginesBound->UseVisualStyleBackColor = true;
		//
		// TTSEngineList
		//
		this->TTSEngineList->AllowUserToAddRows = false;
		this->TTSEngineList->AllowUserToDeleteRows = false;
		this->TTSEngineList->AllowUserToResizeRows = false;
		this->TTSEngineList->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
			| System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		this->TTSEngineList->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::AllCells;
		this->TTSEngineList->BackgroundColor = System::Drawing::SystemColors::Control;
		this->TTSEngineList->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
		this->TTSEngineList->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(5) {this->TTSModeID,
			this->TTSDisplayName, this->TTSManufacturer, this->TTSGender, this->TTSLanguageID});
		this->TTSEngineList->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->TTSEnginesDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->TTSEngineList->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"DataSource", this->TTSEnginesDataBinding, L"TTSEnginesList",
			true)));
		this->TTSEngineList->Location = System::Drawing::Point(6, 39);
		this->TTSEngineList->MultiSelect = false;
		this->TTSEngineList->Name = L"TTSEngineList";
		this->TTSEngineList->ReadOnly = true;
		this->TTSEngineList->RowHeadersVisible = false;
		this->TTSEngineList->RowTemplate->Height = 24;
		this->TTSEngineList->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
		this->TTSEngineList->Size = System::Drawing::Size(628, 436);
		this->TTSEngineList->StandardTab = true;
		this->TTSEngineList->TabIndex = 1;
		//
		// TTSModeID
		//
		this->TTSModeID->DataPropertyName = L"TTSModeID";
		this->TTSModeID->HeaderText = L"TTSModeID";
		this->TTSModeID->Name = L"TTSModeID";
		this->TTSModeID->ReadOnly = true;
		this->TTSModeID->Width = 108;
		//
		// TTSDisplayName
		//
		this->TTSDisplayName->DataPropertyName = L"DisplayName";
		this->TTSDisplayName->HeaderText = L"Name";
		this->TTSDisplayName->Name = L"TTSDisplayName";
		this->TTSDisplayName->ReadOnly = true;
		this->TTSDisplayName->Width = 70;
		//
		// TTSManufacturer
		//
		this->TTSManufacturer->DataPropertyName = L"Manufacturer";
		this->TTSManufacturer->HeaderText = L"Manufacturer";
		this->TTSManufacturer->Name = L"TTSManufacturer";
		this->TTSManufacturer->ReadOnly = true;
		this->TTSManufacturer->Width = 117;
		//
		// TTSGender
		//
		this->TTSGender->DataPropertyName = L"Gender";
		this->TTSGender->HeaderText = L"Gender";
		this->TTSGender->Name = L"TTSGender";
		this->TTSGender->ReadOnly = true;
		this->TTSGender->Width = 81;
		//
		// TTSLanguageID
		//
		this->TTSLanguageID->DataPropertyName = L"LanguageID";
		this->TTSLanguageID->HeaderText = L"Language Id";
		this->TTSLanguageID->Name = L"TTSLanguageID";
		this->TTSLanguageID->ReadOnly = true;
		this->TTSLanguageID->Width = 112;
		//
		// PageSREngines
		//
		this->PageSREngines->Controls->Add(this->SREnginesBound);
		this->PageSREngines->Controls->Add(this->SREngineList);
		this->PageSREngines->Location = System::Drawing::Point(4, 25);
		this->PageSREngines->Name = L"PageSREngines";
		this->PageSREngines->Padding = System::Windows::Forms::Padding(3);
		this->PageSREngines->Size = System::Drawing::Size(640, 481);
		this->PageSREngines->TabIndex = 4;
		this->PageSREngines->Text = L"SR Engines";
		this->PageSREngines->UseVisualStyleBackColor = true;
		//
		// SREnginesBound
		//
		this->SREnginesBound->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
		this->SREnginesBound->Appearance = System::Windows::Forms::Appearance::Button;
		this->SREnginesBound->AutoSize = true;
		this->SREnginesBound->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->SREnginesDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->SREnginesBound->Location = System::Drawing::Point(575, 6);
		this->SREnginesBound->Name = L"SREnginesBound";
		this->SREnginesBound->Size = System::Drawing::Size(59, 27);
		this->SREnginesBound->TabIndex = 0;
		this->SREnginesBound->Text = L"Bound";
		this->SREnginesBound->UseVisualStyleBackColor = true;
		//
		// SREngineList
		//
		this->SREngineList->AllowUserToAddRows = false;
		this->SREngineList->AllowUserToDeleteRows = false;
		this->SREngineList->AllowUserToResizeRows = false;
		this->SREngineList->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
			| System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		this->SREngineList->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::AllCells;
		this->SREngineList->BackgroundColor = System::Drawing::SystemColors::Control;
		this->SREngineList->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
		this->SREngineList->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(4) {this->SRModeID,
			this->SRDisplayName, this->SRManufacturer, this->SRLanguageID});
		this->SREngineList->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->SREnginesDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->SREngineList->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"DataSource", this->SREnginesDataBinding, L"SREnginesList",
			true)));
		this->SREngineList->Location = System::Drawing::Point(6, 39);
		this->SREngineList->MultiSelect = false;
		this->SREngineList->Name = L"SREngineList";
		this->SREngineList->ReadOnly = true;
		this->SREngineList->RowHeadersVisible = false;
		this->SREngineList->RowTemplate->Height = 24;
		this->SREngineList->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
		this->SREngineList->Size = System::Drawing::Size(628, 436);
		this->SREngineList->StandardTab = true;
		this->SREngineList->TabIndex = 1;
		//
		// SRModeID
		//
		this->SRModeID->DataPropertyName = L"SRModeID";
		this->SRModeID->HeaderText = L"SRModeID";
		this->SRModeID->Name = L"SRModeID";
		this->SRModeID->ReadOnly = true;
		//
		// SRDisplayName
		//
		this->SRDisplayName->DataPropertyName = L"DisplayName";
		this->SRDisplayName->HeaderText = L"Name";
		this->SRDisplayName->Name = L"SRDisplayName";
		this->SRDisplayName->ReadOnly = true;
		this->SRDisplayName->Width = 70;
		//
		// SRManufacturer
		//
		this->SRManufacturer->DataPropertyName = L"Manufacturer";
		this->SRManufacturer->HeaderText = L"Manufacturer";
		this->SRManufacturer->Name = L"SRManufacturer";
		this->SRManufacturer->ReadOnly = true;
		this->SRManufacturer->Width = 117;
		//
		// SRLanguageID
		//
		this->SRLanguageID->DataPropertyName = L"LanguageID";
		this->SRLanguageID->HeaderText = L"Language Id";
		this->SRLanguageID->Name = L"SRLanguageID";
		this->SRLanguageID->ReadOnly = true;
		this->SRLanguageID->Width = 112;
		//
		// TestDaControl
		//
		this->TestDaControl->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"RaiseRequestErrors", ::settings::Default, L"RaiseRequestErrors",
			true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->TestDaControl->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"AutoConnect", ::settings::Default, L"AutoConnect",
			true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->TestDaControl->Enabled = true;
		this->TestDaControl->Location = System::Drawing::Point(12, 12);
		this->TestDaControl->Name = L"TestDaControl";
		this->TestDaControl->OcxState = (cli::safe_cast<System::Windows::Forms::AxHost::State^  >(resources->GetObject(L"TestDaControl.OcxState")));
		this->TestDaControl->Size = System::Drawing::Size(200, 200);
		this->TestDaControl->TabIndex = 0;
		this->TestDaControl->TabStop = false;
		this->TestDaControl->AgentShow += gcnew DoubleAgent::AxControl::ShowEventHandler(this, &ControlTestForm::TestDaControl_AgentShow);
		this->TestDaControl->AgentBalloonHide += gcnew DoubleAgent::AxControl::BalloonHideEventHandler(this, &ControlTestForm::TestDaControl_BalloonHide);
		this->TestDaControl->AgentActiveClientChange += gcnew DoubleAgent::AxControl::ActiveClientChangeEventHandler(this, &ControlTestForm::TestDaControl_ActiveClientChange);
		this->TestDaControl->AgentMove += gcnew DoubleAgent::AxControl::MoveEventHandler(this, &ControlTestForm::TestDaControl_MoveEvent);
		this->TestDaControl->AgentPropertyChange += gcnew System::EventHandler(this, &ControlTestForm::TestDaControl_AgentPropertyChange);
		this->TestDaControl->AgentIdleComplete += gcnew DoubleAgent::AxControl::IdleCompleteEventHandler(this, &ControlTestForm::TestDaControl_IdleComplete);
		this->TestDaControl->AgentDeactivateInput += gcnew DoubleAgent::AxControl::DeactivateInputEventHandler(this, &ControlTestForm::TestDaControl_PopupDeactivateInput);
		this->TestDaControl->AgentDblClick += gcnew DoubleAgent::AxControl::DblClickEventHandler(this, &ControlTestForm::TestDaControl_PopupDblClick);
		this->TestDaControl->AgentDefaultCharacterChange += gcnew DoubleAgent::AxControl::DefaultCharacterChangeEventHandler(this, &ControlTestForm::TestDaControl_DefaultCharacterChange);
		this->TestDaControl->AgentDragStart += gcnew DoubleAgent::AxControl::DragStartEventHandler(this, &ControlTestForm::TestDaControl_PopupDragStart);
		this->TestDaControl->AgentBookmark += gcnew DoubleAgent::AxControl::BookmarkEventHandler(this, &ControlTestForm::TestDaControl_Bookmark);
		this->TestDaControl->AgentActivateInput += gcnew DoubleAgent::AxControl::ActivateInputEventHandler(this, &ControlTestForm::TestDaControl_PopupActivateInput);
		this->TestDaControl->AgentHide += gcnew DoubleAgent::AxControl::HideEventHandler(this, &ControlTestForm::TestDaControl_HideEvent);
		this->TestDaControl->AgentClick += gcnew DoubleAgent::AxControl::ClickEventHandler(this, &ControlTestForm::TestDaControl_PopupClick);
		this->TestDaControl->AgentCommand += gcnew DoubleAgent::AxControl::CommandEventHandler(this, &ControlTestForm::TestDaControl_Command);
		this->TestDaControl->AgentBalloonShow += gcnew DoubleAgent::AxControl::BalloonShowEventHandler(this, &ControlTestForm::TestDaControl_BalloonShow);
		this->TestDaControl->AgentRequestStart += gcnew DoubleAgent::AxControl::RequestStartEventHandler(this, &ControlTestForm::TestDaControl_RequestStart);
		this->TestDaControl->AgentIdleStart += gcnew DoubleAgent::AxControl::IdleStartEventHandler(this, &ControlTestForm::TestDaControl_IdleStart);
		this->TestDaControl->AgentListenStart += gcnew DoubleAgent::AxControl::ListenStartEventHandler(this, &ControlTestForm::TestDaControl_ListenStart);
		this->TestDaControl->AgentDragComplete += gcnew DoubleAgent::AxControl::DragCompleteEventHandler(this, &ControlTestForm::TestDaControl_PopupDragComplete);
		this->TestDaControl->AgentSize += gcnew DoubleAgent::AxControl::SizeEventHandler(this, &ControlTestForm::TestDaControl_SizeEvent);
		this->TestDaControl->AgentListenComplete += gcnew DoubleAgent::AxControl::ListenCompleteEventHandler(this, &ControlTestForm::TestDaControl_ListenComplete);
		this->TestDaControl->AgentRequestComplete += gcnew DoubleAgent::AxControl::RequestCompleteEventHandler(this, &ControlTestForm::TestDaControl_RequestComplete);
		//
		// RaiseRequestErrorsButton
		//
		this->RaiseRequestErrorsButton->AutoSize = true;
		this->RaiseRequestErrorsButton->Checked = true;
		this->RaiseRequestErrorsButton->CheckState = System::Windows::Forms::CheckState::Checked;
		this->RaiseRequestErrorsButton->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", ::settings::Default, L"RaiseRequestErrors",
			true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->RaiseRequestErrorsButton->Location = System::Drawing::Point(12, 230);
		this->RaiseRequestErrorsButton->Name = L"RaiseRequestErrorsButton";
		this->RaiseRequestErrorsButton->Size = System::Drawing::Size(125, 21);
		this->RaiseRequestErrorsButton->TabIndex = 1;
		this->RaiseRequestErrorsButton->Text = L"Request errors";
		this->RaiseRequestErrorsButton->UseVisualStyleBackColor = true;
		//
		// AutoConnectButton
		//
		this->AutoConnectButton->AutoSize = true;
		this->AutoConnectButton->Checked = true;
		this->AutoConnectButton->CheckState = System::Windows::Forms::CheckState::Checked;
		this->AutoConnectButton->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", ::settings::Default, L"AutoConnect",
			true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->AutoConnectButton->Location = System::Drawing::Point(12, 257);
		this->AutoConnectButton->Name = L"AutoConnectButton";
		this->AutoConnectButton->Size = System::Drawing::Size(114, 21);
		this->AutoConnectButton->TabIndex = 2;
		this->AutoConnectButton->Text = L"Auto-connect";
		this->AutoConnectButton->UseVisualStyleBackColor = true;
		//
		// ControlConnectedButton
		//
		this->ControlConnectedButton->AutoSize = true;
		this->ControlConnectedButton->Location = System::Drawing::Point(12, 284);
		this->ControlConnectedButton->Name = L"ControlConnectedButton";
		this->ControlConnectedButton->Size = System::Drawing::Size(98, 21);
		this->ControlConnectedButton->TabIndex = 3;
		this->ControlConnectedButton->Text = L"Connected";
		this->ControlConnectedButton->UseVisualStyleBackColor = true;
		//
		// LoadButton
		//
		this->LoadButton->Location = System::Drawing::Point(12, 336);
		this->LoadButton->Name = L"LoadButton";
		this->LoadButton->Size = System::Drawing::Size(108, 32);
		this->LoadButton->TabIndex = 4;
		this->LoadButton->Text = L"Load...";
		this->LoadButton->UseVisualStyleBackColor = true;
		this->LoadButton->Click += gcnew System::EventHandler(this, &ControlTestForm::LoadButton_Click);
		//
		// TabsMain
		//
		this->TabsMain->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
			| System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		this->TabsMain->Controls->Add(this->PageMainCharacter);
		this->TabsMain->Controls->Add(this->PageMainControl);
		this->TabsMain->Controls->Add(this->PageMainEvents);
		this->TabsMain->ItemSize = System::Drawing::Size(120, 21);
		this->TabsMain->Location = System::Drawing::Point(218, 12);
		this->TabsMain->Name = L"TabsMain";
		this->TabsMain->SelectedIndex = 0;
		this->TabsMain->Size = System::Drawing::Size(664, 551);
		this->TabsMain->TabIndex = 7;
		//
		// PageMainCharacter
		//
		this->PageMainCharacter->Controls->Add(this->TabsCharacter);
		this->PageMainCharacter->Location = System::Drawing::Point(4, 25);
		this->PageMainCharacter->Name = L"PageMainCharacter";
		this->PageMainCharacter->Padding = System::Windows::Forms::Padding(3);
		this->PageMainCharacter->Size = System::Drawing::Size(656, 522);
		this->PageMainCharacter->TabIndex = 0;
		this->PageMainCharacter->Text = L"Character";
		this->PageMainCharacter->UseVisualStyleBackColor = true;
		//
		// TabsCharacter
		//
		this->TabsCharacter->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
			| System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		this->TabsCharacter->Controls->Add(this->PageCharacter);
		this->TabsCharacter->Controls->Add(this->PageActions);
		this->TabsCharacter->Controls->Add(this->PageBalloon);
		this->TabsCharacter->Controls->Add(this->PageCommands);
		this->TabsCharacter->Location = System::Drawing::Point(6, 6);
		this->TabsCharacter->Name = L"TabsCharacter";
		this->TabsCharacter->SelectedIndex = 0;
		this->TabsCharacter->Size = System::Drawing::Size(644, 510);
		this->TabsCharacter->TabIndex = 0;
		//
		// PageCharacter
		//
		this->PageCharacter->Controls->Add(this->CharGenerateIcon);
		this->PageCharacter->Controls->Add(label5);
		this->PageCharacter->Controls->Add(label6);
		this->PageCharacter->Controls->Add(label4);
		this->PageCharacter->Controls->Add(label3);
		this->PageCharacter->Controls->Add(this->CharHeightDefault);
		this->PageCharacter->Controls->Add(this->CharWidthDefault);
		this->PageCharacter->Controls->Add(this->CharHeight);
		this->PageCharacter->Controls->Add(this->CharWidth);
		this->PageCharacter->Controls->Add(this->CharTop);
		this->PageCharacter->Controls->Add(this->CharLeft);
		this->PageCharacter->Controls->Add(this->CharMoveCause);
		this->PageCharacter->Controls->Add(this->CharVisibilityCause);
		this->PageCharacter->Controls->Add(this->CharListeningStatus);
		this->PageCharacter->Controls->Add(this->CharActiveState);
		this->PageCharacter->Controls->Add(this->CharHasOtherClients);
		this->PageCharacter->Controls->Add(this->CharHasIcon);
		this->PageCharacter->Controls->Add(this->CharAutoPopupMenu);
		this->PageCharacter->Controls->Add(this->CharIdleState);
		this->PageCharacter->Controls->Add(this->CharIdleEnabled);
		this->PageCharacter->Controls->Add(this->CharSoundEffectsEnabled);
		this->PageCharacter->Controls->Add(this->CharIconVisible);
		this->PageCharacter->Controls->Add(this->CharIconShown);
		this->PageCharacter->Controls->Add(this->CharVisible);
		this->PageCharacter->Controls->Add(this->CharIconTip);
		this->PageCharacter->Controls->Add(this->CharFilePath);
		this->PageCharacter->Controls->Add(this->CharSRModeID);
		this->PageCharacter->Controls->Add(this->CharTTSModeID);
		this->PageCharacter->Controls->Add(this->CharIconIdentity);
		this->PageCharacter->Controls->Add(this->CharFileName);
		this->PageCharacter->Controls->Add(CharIconTipLabel);
		this->PageCharacter->Controls->Add(CharMoveCauseLabel);
		this->PageCharacter->Controls->Add(this->CharDescription);
		this->PageCharacter->Controls->Add(CharVisibilityCauseLabel);
		this->PageCharacter->Controls->Add(CharFilePathLabel);
		this->PageCharacter->Controls->Add(CharHeightLabel);
		this->PageCharacter->Controls->Add(CharListeningStatusLabel);
		this->PageCharacter->Controls->Add(CharWidthLabel);
		this->PageCharacter->Controls->Add(CharSRModeIDLabel);
		this->PageCharacter->Controls->Add(CharTopLabel);
		this->PageCharacter->Controls->Add(CharActiveStateLabel);
		this->PageCharacter->Controls->Add(CharLeftLabel);
		this->PageCharacter->Controls->Add(CharIconIdentityLabel);
		this->PageCharacter->Controls->Add(CharTTSModeIDLabel);
		this->PageCharacter->Controls->Add(CharFileNameLabel);
		this->PageCharacter->Controls->Add(CharDescriptionLabel);
		this->PageCharacter->Controls->Add(this->CharLanguageID);
		this->PageCharacter->Controls->Add(this->CharVersion);
		this->PageCharacter->Controls->Add(this->CharName);
		this->PageCharacter->Controls->Add(CharLanguageIDLabel);
		this->PageCharacter->Controls->Add(CharVersionLabel);
		this->PageCharacter->Controls->Add(CharNameLabel);
		this->PageCharacter->Controls->Add(this->CharUniqueID);
		this->PageCharacter->Controls->Add(this->CharCharacterID);
		this->PageCharacter->Controls->Add(CharUniqueIDLabel);
		this->PageCharacter->Controls->Add(CharCharacterIDLabel);
		this->PageCharacter->Location = System::Drawing::Point(4, 25);
		this->PageCharacter->Name = L"PageCharacter";
		this->PageCharacter->Padding = System::Windows::Forms::Padding(3);
		this->PageCharacter->Size = System::Drawing::Size(636, 481);
		this->PageCharacter->TabIndex = 0;
		this->PageCharacter->Text = L"Character";
		this->PageCharacter->UseVisualStyleBackColor = true;
		//
		// CharGenerateIcon
		//
		this->CharGenerateIcon->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		this->CharGenerateIcon->Location = System::Drawing::Point(303, 271);
		this->CharGenerateIcon->Name = L"CharGenerateIcon";
		this->CharGenerateIcon->Size = System::Drawing::Size(142, 26);
		this->CharGenerateIcon->TabIndex = 31;
		this->CharGenerateIcon->Text = L"GenerateIcon";
		this->CharGenerateIcon->UseVisualStyleBackColor = true;
		this->CharGenerateIcon->Click += gcnew System::EventHandler(this, &ControlTestForm::CharGenerateIcon_Click);
		//
		// CharHeightDefault
		//
		this->CharHeightDefault->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		this->CharHeightDefault->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"IsNotDefaultHeight",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharHeightDefault->Enabled = false;
		this->CharHeightDefault->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"CharHeightDefault.Image")));
		this->CharHeightDefault->Location = System::Drawing::Point(595, 345);
		this->CharHeightDefault->Name = L"CharHeightDefault";
		this->CharHeightDefault->Padding = System::Windows::Forms::Padding(3);
		this->CharHeightDefault->Size = System::Drawing::Size(22, 22);
		this->CharHeightDefault->TabIndex = 44;
		this->CharHeightDefault->UseMnemonic = false;
		this->CharHeightDefault->UseVisualStyleBackColor = true;
		this->CharHeightDefault->Click += gcnew System::EventHandler(this, &ControlTestForm::CharHeightDefault_Click);
		//
		// CharWidthDefault
		//
		this->CharWidthDefault->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		this->CharWidthDefault->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"IsNotDefaultWidth",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharWidthDefault->Enabled = false;
		this->CharWidthDefault->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"CharWidthDefault.Image")));
		this->CharWidthDefault->Location = System::Drawing::Point(431, 346);
		this->CharWidthDefault->Name = L"CharWidthDefault";
		this->CharWidthDefault->Padding = System::Windows::Forms::Padding(3);
		this->CharWidthDefault->Size = System::Drawing::Size(22, 22);
		this->CharWidthDefault->TabIndex = 42;
		this->CharWidthDefault->UseMnemonic = false;
		this->CharWidthDefault->UseVisualStyleBackColor = true;
		this->CharWidthDefault->Click += gcnew System::EventHandler(this, &ControlTestForm::CharWidthDefault_Click);
		//
		// CharHeight
		//
		this->CharHeight->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		this->CharHeight->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Value", this->CharacterBindingSource, L"Height",
			true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->CharHeight->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharHeight->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 0});
		this->CharHeight->Location = System::Drawing::Point(514, 346);
		this->CharHeight->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {500, 0, 0, 0});
		this->CharHeight->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {20, 0, 0, 0});
		this->CharHeight->Name = L"CharHeight";
		this->CharHeight->Size = System::Drawing::Size(75, 22);
		this->CharHeight->TabIndex = 43;
		this->CharHeight->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {20, 0, 0, 0});
		//
		// CharacterBindingSource
		//
		this->CharacterBindingSource->DataSource = DoubleAgent::Control::Character::typeid;
		//
		// CharWidth
		//
		this->CharWidth->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		this->CharWidth->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Value", this->CharacterBindingSource, L"Width", true,
			System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->CharWidth->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharWidth->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 0});
		this->CharWidth->Location = System::Drawing::Point(347, 346);
		this->CharWidth->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {500, 0, 0, 0});
		this->CharWidth->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {20, 0, 0, 0});
		this->CharWidth->Name = L"CharWidth";
		this->CharWidth->Size = System::Drawing::Size(75, 22);
		this->CharWidth->TabIndex = 41;
		this->CharWidth->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {20, 0, 0, 0});
		//
		// CharTop
		//
		this->CharTop->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		this->CharTop->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Value", this->CharacterBindingSource, L"Top", true,
			System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->CharTop->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharTop->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 0});
		this->CharTop->Location = System::Drawing::Point(216, 346);
		this->CharTop->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {2000, 0, 0, 0});
		this->CharTop->Name = L"CharTop";
		this->CharTop->Size = System::Drawing::Size(75, 22);
		this->CharTop->TabIndex = 39;
		//
		// CharLeft
		//
		this->CharLeft->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		this->CharLeft->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Value", this->CharacterBindingSource, L"Left", true,
			System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->CharLeft->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharLeft->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 0});
		this->CharLeft->Location = System::Drawing::Point(96, 346);
		this->CharLeft->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {2000, 0, 0, 0});
		this->CharLeft->Name = L"CharLeft";
		this->CharLeft->Size = System::Drawing::Size(75, 22);
		this->CharLeft->TabIndex = 37;
		//
		// CharMoveCause
		//
		this->CharMoveCause->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		this->CharMoveCause->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharMoveCause->DataSource = this->CharacterBindingSource;
		this->CharMoveCause->DisplayMember = L"MoveCause";
		this->CharMoveCause->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
		this->CharMoveCause->FormattingEnabled = true;
		this->CharMoveCause->Location = System::Drawing::Point(297, 451);
		this->CharMoveCause->Name = L"CharMoveCause";
		this->CharMoveCause->Size = System::Drawing::Size(330, 24);
		this->CharMoveCause->TabIndex = 52;
		this->CharMoveCause->TabStop = false;
		//
		// CharVisibilityCause
		//
		this->CharVisibilityCause->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		this->CharVisibilityCause->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharVisibilityCause->DataSource = this->CharacterBindingSource;
		this->CharVisibilityCause->DisplayMember = L"VisibilityCause";
		this->CharVisibilityCause->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
		this->CharVisibilityCause->FormattingEnabled = true;
		this->CharVisibilityCause->Location = System::Drawing::Point(300, 404);
		this->CharVisibilityCause->Name = L"CharVisibilityCause";
		this->CharVisibilityCause->Size = System::Drawing::Size(327, 24);
		this->CharVisibilityCause->TabIndex = 50;
		this->CharVisibilityCause->TabStop = false;
		//
		// CharListeningStatus
		//
		this->CharListeningStatus->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		this->CharListeningStatus->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharListeningStatus->DataSource = this->CharacterBindingSource;
		this->CharListeningStatus->DisplayMember = L"ListeningStatus";
		this->CharListeningStatus->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
		this->CharListeningStatus->FormattingEnabled = true;
		this->CharListeningStatus->Location = System::Drawing::Point(6, 451);
		this->CharListeningStatus->Name = L"CharListeningStatus";
		this->CharListeningStatus->Size = System::Drawing::Size(285, 24);
		this->CharListeningStatus->TabIndex = 48;
		this->CharListeningStatus->TabStop = false;
		//
		// CharActiveState
		//
		this->CharActiveState->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		this->CharActiveState->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharActiveState->DataSource = this->CharacterBindingSource;
		this->CharActiveState->DisplayMember = L"ActiveState";
		this->CharActiveState->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
		this->CharActiveState->FormattingEnabled = true;
		this->CharActiveState->Location = System::Drawing::Point(6, 404);
		this->CharActiveState->Name = L"CharActiveState";
		this->CharActiveState->Size = System::Drawing::Size(285, 24);
		this->CharActiveState->TabIndex = 46;
		this->CharActiveState->TabStop = false;
		//
		// CharHasOtherClients
		//
		this->CharHasOtherClients->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		this->CharHasOtherClients->AutoCheck = false;
		this->CharHasOtherClients->AutoSize = true;
		this->CharHasOtherClients->CausesValidation = false;
		this->CharHasOtherClients->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->CharacterBindingSource,
			L"HasOtherClients", true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharHasOtherClients->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharHasOtherClients->ForeColor = System::Drawing::SystemColors::GrayText;
		this->CharHasOtherClients->Location = System::Drawing::Point(7, 238);
		this->CharHasOtherClients->Name = L"CharHasOtherClients";
		this->CharHasOtherClients->Size = System::Drawing::Size(133, 21);
		this->CharHasOtherClients->TabIndex = 25;
		this->CharHasOtherClients->TabStop = false;
		this->CharHasOtherClients->Text = L"HasOtherClients";
		this->CharHasOtherClients->UseVisualStyleBackColor = true;
		//
		// CharHasIcon
		//
		this->CharHasIcon->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		this->CharHasIcon->AutoCheck = false;
		this->CharHasIcon->AutoSize = true;
		this->CharHasIcon->CausesValidation = false;
		this->CharHasIcon->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->CharacterBindingSource, L"HasIcon",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharHasIcon->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharHasIcon->ForeColor = System::Drawing::SystemColors::GrayText;
		this->CharHasIcon->Location = System::Drawing::Point(9, 276);
		this->CharHasIcon->Name = L"CharHasIcon";
		this->CharHasIcon->Size = System::Drawing::Size(81, 21);
		this->CharHasIcon->TabIndex = 28;
		this->CharHasIcon->TabStop = false;
		this->CharHasIcon->Text = L"HasIcon";
		this->CharHasIcon->UseVisualStyleBackColor = true;
		//
		// CharAutoPopupMenu
		//
		this->CharAutoPopupMenu->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		this->CharAutoPopupMenu->AutoSize = true;
		this->CharAutoPopupMenu->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->CharacterBindingSource, L"AutoPopupMenu",
			true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->CharAutoPopupMenu->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharAutoPopupMenu->Location = System::Drawing::Point(365, 211);
		this->CharAutoPopupMenu->Name = L"CharAutoPopupMenu";
		this->CharAutoPopupMenu->Size = System::Drawing::Size(135, 21);
		this->CharAutoPopupMenu->TabIndex = 24;
		this->CharAutoPopupMenu->Text = L"AutoPopupMenu";
		this->CharAutoPopupMenu->UseVisualStyleBackColor = true;
		//
		// CharIdleState
		//
		this->CharIdleState->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		this->CharIdleState->AutoCheck = false;
		this->CharIdleState->AutoSize = true;
		this->CharIdleState->CausesValidation = false;
		this->CharIdleState->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->CharacterBindingSource, L"IdleState",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharIdleState->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharIdleState->ForeColor = System::Drawing::SystemColors::GrayText;
		this->CharIdleState->Location = System::Drawing::Point(146, 238);
		this->CharIdleState->Name = L"CharIdleState";
		this->CharIdleState->Size = System::Drawing::Size(85, 21);
		this->CharIdleState->TabIndex = 26;
		this->CharIdleState->TabStop = false;
		this->CharIdleState->Text = L"IdleState";
		this->CharIdleState->UseVisualStyleBackColor = true;
		//
		// CharIdleEnabled
		//
		this->CharIdleEnabled->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		this->CharIdleEnabled->AutoSize = true;
		this->CharIdleEnabled->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->CharacterBindingSource, L"IdleEnabled",
			true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->CharIdleEnabled->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharIdleEnabled->Location = System::Drawing::Point(255, 211);
		this->CharIdleEnabled->Name = L"CharIdleEnabled";
		this->CharIdleEnabled->Size = System::Drawing::Size(104, 21);
		this->CharIdleEnabled->TabIndex = 23;
		this->CharIdleEnabled->Text = L"IdleEnabled";
		this->CharIdleEnabled->UseVisualStyleBackColor = true;
		//
		// CharSoundEffectsEnabled
		//
		this->CharSoundEffectsEnabled->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		this->CharSoundEffectsEnabled->AutoSize = true;
		this->CharSoundEffectsEnabled->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->CharacterBindingSource,
			L"SoundEffectsEnabled", true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->CharSoundEffectsEnabled->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding,
			L"Bound", true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharSoundEffectsEnabled->Location = System::Drawing::Point(83, 211);
		this->CharSoundEffectsEnabled->Name = L"CharSoundEffectsEnabled";
		this->CharSoundEffectsEnabled->Size = System::Drawing::Size(166, 21);
		this->CharSoundEffectsEnabled->TabIndex = 22;
		this->CharSoundEffectsEnabled->Text = L"SoundEffectsEnabled";
		this->CharSoundEffectsEnabled->UseVisualStyleBackColor = true;
		//
		// CharIconVisible
		//
		this->CharIconVisible->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		this->CharIconVisible->AutoCheck = false;
		this->CharIconVisible->AutoSize = true;
		this->CharIconVisible->CausesValidation = false;
		this->CharIconVisible->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->CharacterBindingSource, L"IconVisible",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharIconVisible->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharIconVisible->ForeColor = System::Drawing::SystemColors::GrayText;
		this->CharIconVisible->Location = System::Drawing::Point(200, 276);
		this->CharIconVisible->Name = L"CharIconVisible";
		this->CharIconVisible->Size = System::Drawing::Size(97, 21);
		this->CharIconVisible->TabIndex = 30;
		this->CharIconVisible->TabStop = false;
		this->CharIconVisible->Text = L"IconVisible";
		this->CharIconVisible->UseVisualStyleBackColor = true;
		//
		// CharIconShown
		//
		this->CharIconShown->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		this->CharIconShown->AutoSize = true;
		this->CharIconShown->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->CharacterBindingSource, L"IconShown",
			true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->CharIconShown->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharIconShown->Location = System::Drawing::Point(96, 276);
		this->CharIconShown->Name = L"CharIconShown";
		this->CharIconShown->Size = System::Drawing::Size(98, 21);
		this->CharIconShown->TabIndex = 29;
		this->CharIconShown->Text = L"IconShown";
		this->CharIconShown->UseVisualStyleBackColor = true;
		//
		// CharVisible
		//
		this->CharVisible->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		this->CharVisible->AutoCheck = false;
		this->CharVisible->AutoSize = true;
		this->CharVisible->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->CharacterBindingSource, L"Visible",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharVisible->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharVisible->ForeColor = System::Drawing::SystemColors::GrayText;
		this->CharVisible->Location = System::Drawing::Point(6, 211);
		this->CharVisible->Name = L"CharVisible";
		this->CharVisible->Size = System::Drawing::Size(71, 21);
		this->CharVisible->TabIndex = 21;
		this->CharVisible->TabStop = false;
		this->CharVisible->Text = L"Visible";
		this->CharVisible->UseVisualStyleBackColor = true;
		//
		// CharIconTip
		//
		this->CharIconTip->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		this->CharIconTip->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", this->CharacterBindingSource, L"IconTip",
			true)));
		this->CharIconTip->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharIconTip->Location = System::Drawing::Point(431, 303);
		this->CharIconTip->Name = L"CharIconTip";
		this->CharIconTip->Size = System::Drawing::Size(196, 22);
		this->CharIconTip->TabIndex = 35;
		//
		// CharFilePath
		//
		this->CharFilePath->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		this->CharFilePath->BackColor = System::Drawing::SystemColors::Window;
		this->CharFilePath->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", this->CharacterBindingSource, L"FilePath",
			true)));
		this->CharFilePath->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharFilePath->ForeColor = System::Drawing::SystemColors::GrayText;
		this->CharFilePath->Location = System::Drawing::Point(320, 118);
		this->CharFilePath->Name = L"CharFilePath";
		this->CharFilePath->ReadOnly = true;
		this->CharFilePath->Size = System::Drawing::Size(307, 22);
		this->CharFilePath->TabIndex = 15;
		this->CharFilePath->TabStop = false;
		//
		// CharSRModeID
		//
		this->CharSRModeID->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		this->CharSRModeID->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", this->CharacterBindingSource, L"SRModeID",
			true)));
		this->CharSRModeID->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharSRModeID->Location = System::Drawing::Point(95, 174);
		this->CharSRModeID->Name = L"CharSRModeID";
		this->CharSRModeID->Size = System::Drawing::Size(532, 22);
		this->CharSRModeID->TabIndex = 19;
		//
		// CharTTSModeID
		//
		this->CharTTSModeID->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		this->CharTTSModeID->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", this->CharacterBindingSource, L"TTSModeID",
			true)));
		this->CharTTSModeID->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharTTSModeID->Location = System::Drawing::Point(95, 146);
		this->CharTTSModeID->Name = L"CharTTSModeID";
		this->CharTTSModeID->Size = System::Drawing::Size(532, 22);
		this->CharTTSModeID->TabIndex = 17;
		//
		// CharIconIdentity
		//
		this->CharIconIdentity->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		this->CharIconIdentity->BackColor = System::Drawing::SystemColors::Window;
		this->CharIconIdentity->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", this->CharacterBindingSource, L"IconIdentity",
			true)));
		this->CharIconIdentity->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharIconIdentity->ForeColor = System::Drawing::SystemColors::GrayText;
		this->CharIconIdentity->Location = System::Drawing::Point(96, 303);
		this->CharIconIdentity->Name = L"CharIconIdentity";
		this->CharIconIdentity->ReadOnly = true;
		this->CharIconIdentity->Size = System::Drawing::Size(269, 22);
		this->CharIconIdentity->TabIndex = 33;
		this->CharIconIdentity->TabStop = false;
		//
		// CharFileName
		//
		this->CharFileName->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		this->CharFileName->BackColor = System::Drawing::SystemColors::Window;
		this->CharFileName->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", this->CharacterBindingSource, L"FileName",
			true)));
		this->CharFileName->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharFileName->ForeColor = System::Drawing::SystemColors::GrayText;
		this->CharFileName->Location = System::Drawing::Point(95, 118);
		this->CharFileName->Name = L"CharFileName";
		this->CharFileName->ReadOnly = true;
		this->CharFileName->Size = System::Drawing::Size(154, 22);
		this->CharFileName->TabIndex = 13;
		this->CharFileName->TabStop = false;
		//
		// CharDescription
		//
		this->CharDescription->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
			| System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		this->CharDescription->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", this->CharacterBindingSource, L"Description",
			true)));
		this->CharDescription->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharDescription->Location = System::Drawing::Point(95, 72);
		this->CharDescription->Multiline = true;
		this->CharDescription->Name = L"CharDescription";
		this->CharDescription->Size = System::Drawing::Size(532, 40);
		this->CharDescription->TabIndex = 11;
		//
		// CharLanguageID
		//
		this->CharLanguageID->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
		this->CharLanguageID->BackColor = System::Drawing::SystemColors::Window;
		this->CharLanguageID->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", this->CharacterBindingSource, L"LanguageID",
			true, System::Windows::Forms::DataSourceUpdateMode::OnValidation, nullptr, L"X4")));
		this->CharLanguageID->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharLanguageID->ForeColor = System::Drawing::SystemColors::GrayText;
		this->CharLanguageID->Location = System::Drawing::Point(416, 44);
		this->CharLanguageID->Name = L"CharLanguageID";
		this->CharLanguageID->ReadOnly = true;
		this->CharLanguageID->Size = System::Drawing::Size(70, 22);
		this->CharLanguageID->TabIndex = 7;
		this->CharLanguageID->TabStop = false;
		//
		// CharVersion
		//
		this->CharVersion->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
		this->CharVersion->BackColor = System::Drawing::SystemColors::Window;
		this->CharVersion->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", this->CharacterBindingSource, L"Version",
			true)));
		this->CharVersion->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharVersion->ForeColor = System::Drawing::SystemColors::GrayText;
		this->CharVersion->Location = System::Drawing::Point(554, 44);
		this->CharVersion->Name = L"CharVersion";
		this->CharVersion->ReadOnly = true;
		this->CharVersion->Size = System::Drawing::Size(73, 22);
		this->CharVersion->TabIndex = 9;
		this->CharVersion->TabStop = false;
		//
		// CharName
		//
		this->CharName->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		this->CharName->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", this->CharacterBindingSource, L"Name", true)));
		this->CharName->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharName->Location = System::Drawing::Point(95, 44);
		this->CharName->Name = L"CharName";
		this->CharName->Size = System::Drawing::Size(238, 22);
		this->CharName->TabIndex = 5;
		//
		// CharUniqueID
		//
		this->CharUniqueID->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
		this->CharUniqueID->BackColor = System::Drawing::SystemColors::Window;
		this->CharUniqueID->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", this->CharacterBindingSource, L"UniqueID",
			true)));
		this->CharUniqueID->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharUniqueID->ForeColor = System::Drawing::SystemColors::GrayText;
		this->CharUniqueID->Location = System::Drawing::Point(285, 12);
		this->CharUniqueID->Name = L"CharUniqueID";
		this->CharUniqueID->ReadOnly = true;
		this->CharUniqueID->Size = System::Drawing::Size(342, 22);
		this->CharUniqueID->TabIndex = 3;
		this->CharUniqueID->TabStop = false;
		//
		// CharCharacterID
		//
		this->CharCharacterID->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		this->CharCharacterID->BackColor = System::Drawing::SystemColors::Window;
		this->CharCharacterID->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", this->CharacterBindingSource, L"CharacterID",
			true)));
		this->CharCharacterID->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharCharacterID->ForeColor = System::Drawing::SystemColors::GrayText;
		this->CharCharacterID->Location = System::Drawing::Point(95, 12);
		this->CharCharacterID->Name = L"CharCharacterID";
		this->CharCharacterID->ReadOnly = true;
		this->CharCharacterID->Size = System::Drawing::Size(115, 22);
		this->CharCharacterID->TabIndex = 1;
		this->CharCharacterID->TabStop = false;
		//
		// PageActions
		//
		this->PageActions->Controls->Add(this->CharPlayAnimation);
		this->PageActions->Controls->Add(this->CharThinkText);
		this->PageActions->Controls->Add(this->CharSpeakText);
		this->PageActions->Controls->Add(this->CharMoveToTop);
		this->PageActions->Controls->Add(CharMoveToTopLabel);
		this->PageActions->Controls->Add(this->CharShowMenuY);
		this->PageActions->Controls->Add(this->CharShowMenuX);
		this->PageActions->Controls->Add(this->CharGestureAtY);
		this->PageActions->Controls->Add(CharShowMenuYLabel);
		this->PageActions->Controls->Add(this->CharGestureAtX);
		this->PageActions->Controls->Add(CharGestureAtYLabel);
		this->PageActions->Controls->Add(this->CharMoveToLeft);
		this->PageActions->Controls->Add(CharShowMenuXLabel);
		this->PageActions->Controls->Add(CharThinkTextLabel);
		this->PageActions->Controls->Add(CharGestureAtXLabel);
		this->PageActions->Controls->Add(CharSpeakTextLabel);
		this->PageActions->Controls->Add(CharMoveToLeftLabel);
		this->PageActions->Controls->Add(this->CharHideFast);
		this->PageActions->Controls->Add(this->CharShowFast);
		this->PageActions->Controls->Add(this->CharStopAll);
		this->PageActions->Controls->Add(this->CharPlay);
		this->PageActions->Controls->Add(this->CharShowPopupMenu);
		this->PageActions->Controls->Add(this->CharListenStop);
		this->PageActions->Controls->Add(this->CharListen);
		this->PageActions->Controls->Add(this->CharThink);
		this->PageActions->Controls->Add(this->CharSpeak);
		this->PageActions->Controls->Add(this->CharGestureAt);
		this->PageActions->Controls->Add(this->CharMoveTo);
		this->PageActions->Controls->Add(this->CharHide);
		this->PageActions->Controls->Add(this->CharShow);
		this->PageActions->Location = System::Drawing::Point(4, 25);
		this->PageActions->Name = L"PageActions";
		this->PageActions->Padding = System::Windows::Forms::Padding(3);
		this->PageActions->Size = System::Drawing::Size(636, 481);
		this->PageActions->TabIndex = 4;
		this->PageActions->Text = L"Actions";
		this->PageActions->UseVisualStyleBackColor = true;
		//
		// CharPlayAnimation
		//
		this->CharPlayAnimation->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
			| System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		this->CharPlayAnimation->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharPlayAnimation->FormattingEnabled = true;
		this->CharPlayAnimation->IntegralHeight = false;
		this->CharPlayAnimation->ItemHeight = 16;
		this->CharPlayAnimation->Location = System::Drawing::Point(152, 314);
		this->CharPlayAnimation->Name = L"CharPlayAnimation";
		this->CharPlayAnimation->Size = System::Drawing::Size(478, 161);
		this->CharPlayAnimation->TabIndex = 28;
		this->CharPlayAnimation->DoubleClick += gcnew System::EventHandler(this, &ControlTestForm::CharPlayAnimation_DoubleClick);
		//
		// CharThinkText
		//
		this->CharThinkText->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		this->CharThinkText->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharThinkText->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", ::settings::Default, L"ThinkText", true,
			System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->CharThinkText->Location = System::Drawing::Point(187, 205);
		this->CharThinkText->Name = L"CharThinkText";
		this->CharThinkText->Size = System::Drawing::Size(439, 22);
		this->CharThinkText->TabIndex = 19;
		this->CharThinkText->Text = L"Think something";
		//
		// CharSpeakText
		//
		this->CharSpeakText->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		this->CharSpeakText->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharSpeakText->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", ::settings::Default, L"SpeakText", true,
			System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->CharSpeakText->Location = System::Drawing::Point(187, 167);
		this->CharSpeakText->Name = L"CharSpeakText";
		this->CharSpeakText->Size = System::Drawing::Size(439, 22);
		this->CharSpeakText->TabIndex = 16;
		this->CharSpeakText->Text = L"Say something";
		//
		// CharMoveToTop
		//
		this->CharMoveToTop->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharMoveToTop->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Value", this->CharacterBindingSource, L"Top",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharMoveToTop->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 0});
		this->CharMoveToTop->Location = System::Drawing::Point(296, 92);
		this->CharMoveToTop->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {2000, 0, 0, 0});
		this->CharMoveToTop->Name = L"CharMoveToTop";
		this->CharMoveToTop->Size = System::Drawing::Size(64, 22);
		this->CharMoveToTop->TabIndex = 8;
		//
		// CharShowMenuY
		//
		this->CharShowMenuY->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharShowMenuY->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 0});
		this->CharShowMenuY->Location = System::Drawing::Point(296, 282);
		this->CharShowMenuY->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {2000, 0, 0, 0});
		this->CharShowMenuY->Name = L"CharShowMenuY";
		this->CharShowMenuY->Size = System::Drawing::Size(64, 22);
		this->CharShowMenuY->TabIndex = 26;
		//
		// CharShowMenuX
		//
		this->CharShowMenuX->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharShowMenuX->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 0});
		this->CharShowMenuX->Location = System::Drawing::Point(187, 281);
		this->CharShowMenuX->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {2000, 0, 0, 0});
		this->CharShowMenuX->Name = L"CharShowMenuX";
		this->CharShowMenuX->Size = System::Drawing::Size(64, 22);
		this->CharShowMenuX->TabIndex = 24;
		//
		// CharGestureAtY
		//
		this->CharGestureAtY->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharGestureAtY->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 0});
		this->CharGestureAtY->Location = System::Drawing::Point(296, 130);
		this->CharGestureAtY->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {2000, 0, 0, 0});
		this->CharGestureAtY->Name = L"CharGestureAtY";
		this->CharGestureAtY->Size = System::Drawing::Size(64, 22);
		this->CharGestureAtY->TabIndex = 13;
		//
		// CharGestureAtX
		//
		this->CharGestureAtX->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharGestureAtX->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 0});
		this->CharGestureAtX->Location = System::Drawing::Point(187, 129);
		this->CharGestureAtX->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {2000, 0, 0, 0});
		this->CharGestureAtX->Name = L"CharGestureAtX";
		this->CharGestureAtX->Size = System::Drawing::Size(64, 22);
		this->CharGestureAtX->TabIndex = 11;
		//
		// CharMoveToLeft
		//
		this->CharMoveToLeft->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharMoveToLeft->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Value", this->CharacterBindingSource, L"Left",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharMoveToLeft->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 0});
		this->CharMoveToLeft->Location = System::Drawing::Point(187, 92);
		this->CharMoveToLeft->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {2000, 0, 0, 0});
		this->CharMoveToLeft->Name = L"CharMoveToLeft";
		this->CharMoveToLeft->Size = System::Drawing::Size(64, 22);
		this->CharMoveToLeft->TabIndex = 6;
		//
		// CharHideFast
		//
		this->CharHideFast->AutoSize = true;
		this->CharHideFast->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharHideFast->Location = System::Drawing::Point(152, 55);
		this->CharHideFast->Name = L"CharHideFast";
		this->CharHideFast->Size = System::Drawing::Size(57, 21);
		this->CharHideFast->TabIndex = 3;
		this->CharHideFast->Text = L"Fast";
		this->CharHideFast->UseVisualStyleBackColor = true;
		//
		// CharShowFast
		//
		this->CharShowFast->AutoSize = true;
		this->CharShowFast->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharShowFast->Location = System::Drawing::Point(152, 17);
		this->CharShowFast->Name = L"CharShowFast";
		this->CharShowFast->Size = System::Drawing::Size(57, 21);
		this->CharShowFast->TabIndex = 1;
		this->CharShowFast->Text = L"Fast";
		this->CharShowFast->UseVisualStyleBackColor = true;
		//
		// CharStopAll
		//
		this->CharStopAll->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		this->CharStopAll->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharStopAll->Location = System::Drawing::Point(6, 443);
		this->CharStopAll->Name = L"CharStopAll";
		this->CharStopAll->Size = System::Drawing::Size(120, 32);
		this->CharStopAll->TabIndex = 29;
		this->CharStopAll->Text = L"StopAll";
		this->CharStopAll->UseVisualStyleBackColor = true;
		this->CharStopAll->Click += gcnew System::EventHandler(this, &ControlTestForm::CharStopAll_Click);
		//
		// CharPlay
		//
		this->CharPlay->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharPlay->Location = System::Drawing::Point(6, 314);
		this->CharPlay->Name = L"CharPlay";
		this->CharPlay->Size = System::Drawing::Size(120, 32);
		this->CharPlay->TabIndex = 27;
		this->CharPlay->Text = L"Play";
		this->CharPlay->UseVisualStyleBackColor = true;
		this->CharPlay->Click += gcnew System::EventHandler(this, &ControlTestForm::CharPlay_Click);
		//
		// CharShowPopupMenu
		//
		this->CharShowPopupMenu->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharShowPopupMenu->Location = System::Drawing::Point(6, 276);
		this->CharShowPopupMenu->Name = L"CharShowPopupMenu";
		this->CharShowPopupMenu->Size = System::Drawing::Size(120, 32);
		this->CharShowPopupMenu->TabIndex = 22;
		this->CharShowPopupMenu->Text = L"ShowMenu";
		this->CharShowPopupMenu->UseVisualStyleBackColor = true;
		this->CharShowPopupMenu->Click += gcnew System::EventHandler(this, &ControlTestForm::CharShowPopupMenu_Click);
		//
		// CharListenStop
		//
		this->CharListenStop->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"IsListening",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharListenStop->Enabled = false;
		this->CharListenStop->Location = System::Drawing::Point(152, 238);
		this->CharListenStop->Name = L"CharListenStop";
		this->CharListenStop->Size = System::Drawing::Size(120, 32);
		this->CharListenStop->TabIndex = 21;
		this->CharListenStop->Text = L"Stop Listening";
		this->CharListenStop->UseVisualStyleBackColor = true;
		this->CharListenStop->Click += gcnew System::EventHandler(this, &ControlTestForm::CharListenStop_Click);
		//
		// CharListen
		//
		this->CharListen->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharListen->Location = System::Drawing::Point(6, 238);
		this->CharListen->Name = L"CharListen";
		this->CharListen->Size = System::Drawing::Size(120, 32);
		this->CharListen->TabIndex = 20;
		this->CharListen->Text = L"Listen";
		this->CharListen->UseVisualStyleBackColor = true;
		this->CharListen->Click += gcnew System::EventHandler(this, &ControlTestForm::CharListen_Click);
		//
		// CharThink
		//
		this->CharThink->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharThink->Location = System::Drawing::Point(6, 200);
		this->CharThink->Name = L"CharThink";
		this->CharThink->Size = System::Drawing::Size(120, 32);
		this->CharThink->TabIndex = 17;
		this->CharThink->Text = L"Think";
		this->CharThink->UseVisualStyleBackColor = true;
		this->CharThink->Click += gcnew System::EventHandler(this, &ControlTestForm::CharThink_Click);
		//
		// CharSpeak
		//
		this->CharSpeak->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharSpeak->Location = System::Drawing::Point(6, 162);
		this->CharSpeak->Name = L"CharSpeak";
		this->CharSpeak->Size = System::Drawing::Size(120, 32);
		this->CharSpeak->TabIndex = 14;
		this->CharSpeak->Text = L"Speak";
		this->CharSpeak->UseVisualStyleBackColor = true;
		this->CharSpeak->Click += gcnew System::EventHandler(this, &ControlTestForm::CharSpeak_Click);
		//
		// CharGestureAt
		//
		this->CharGestureAt->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharGestureAt->Location = System::Drawing::Point(6, 124);
		this->CharGestureAt->Name = L"CharGestureAt";
		this->CharGestureAt->Size = System::Drawing::Size(120, 32);
		this->CharGestureAt->TabIndex = 9;
		this->CharGestureAt->Text = L"GestureAt";
		this->CharGestureAt->UseVisualStyleBackColor = true;
		this->CharGestureAt->Click += gcnew System::EventHandler(this, &ControlTestForm::CharGestureAt_Click);
		//
		// CharMoveTo
		//
		this->CharMoveTo->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharMoveTo->Location = System::Drawing::Point(6, 86);
		this->CharMoveTo->Name = L"CharMoveTo";
		this->CharMoveTo->Size = System::Drawing::Size(120, 32);
		this->CharMoveTo->TabIndex = 4;
		this->CharMoveTo->Text = L"MoveTo";
		this->CharMoveTo->UseVisualStyleBackColor = true;
		this->CharMoveTo->Click += gcnew System::EventHandler(this, &ControlTestForm::CharMoveTo_Click);
		//
		// CharHide
		//
		this->CharHide->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharHide->Location = System::Drawing::Point(6, 48);
		this->CharHide->Name = L"CharHide";
		this->CharHide->Size = System::Drawing::Size(120, 32);
		this->CharHide->TabIndex = 2;
		this->CharHide->Text = L"Hide";
		this->CharHide->UseVisualStyleBackColor = true;
		this->CharHide->Click += gcnew System::EventHandler(this, &ControlTestForm::CharHide_Click);
		//
		// CharShow
		//
		this->CharShow->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharShow->Location = System::Drawing::Point(6, 10);
		this->CharShow->Name = L"CharShow";
		this->CharShow->Size = System::Drawing::Size(120, 32);
		this->CharShow->TabIndex = 0;
		this->CharShow->Text = L"Show";
		this->CharShow->UseVisualStyleBackColor = true;
		this->CharShow->Click += gcnew System::EventHandler(this, &ControlTestForm::CharShow_Click);
		//
		// PageBalloon
		//
		this->PageBalloon->Controls->Add(label2);
		this->PageBalloon->Controls->Add(label1);
		this->PageBalloon->Controls->Add(this->CharBalloonBorderColorName);
		this->PageBalloon->Controls->Add(this->CharBalloonTextColorName);
		this->PageBalloon->Controls->Add(this->CharBalloonBackColorName);
		this->PageBalloon->Controls->Add(CharBalloonBorderColorLabel);
		this->PageBalloon->Controls->Add(CharBalloonTextColorLabel);
		this->PageBalloon->Controls->Add(CharBalloonBackColorLabel);
		this->PageBalloon->Controls->Add(this->CharBalloonFontStrikeThru);
		this->PageBalloon->Controls->Add(this->CharBalloonFontUnderline);
		this->PageBalloon->Controls->Add(this->CharBalloonFontItalic);
		this->PageBalloon->Controls->Add(this->CharBalloonFontBold);
		this->PageBalloon->Controls->Add(this->CharBalloonFontCharSet);
		this->PageBalloon->Controls->Add(CharBalloonFontCharSetLabel);
		this->PageBalloon->Controls->Add(this->CharBalloonFontSize);
		this->PageBalloon->Controls->Add(CharBalloonFontSizeLabel);
		this->PageBalloon->Controls->Add(this->CharBalloonFontName);
		this->PageBalloon->Controls->Add(CharBalloonFontNameLabel);
		this->PageBalloon->Controls->Add(this->CharBalloonAutoPace);
		this->PageBalloon->Controls->Add(this->CharBalloonAutoHide);
		this->PageBalloon->Controls->Add(this->CharBalloonShowPartialLines);
		this->PageBalloon->Controls->Add(this->CharBalloonSizeToText);
		this->PageBalloon->Controls->Add(this->CharBalloonCharsPerLine);
		this->PageBalloon->Controls->Add(this->CharBalloonNumberOfLines);
		this->PageBalloon->Controls->Add(CharBalloonCharsPerLineLabel);
		this->PageBalloon->Controls->Add(CharBalloonNumberOfLinesLabel);
		this->PageBalloon->Controls->Add(this->CharBalloonVisible);
		this->PageBalloon->Controls->Add(this->CharBalloonEnabled);
		this->PageBalloon->Location = System::Drawing::Point(4, 25);
		this->PageBalloon->Name = L"PageBalloon";
		this->PageBalloon->Padding = System::Windows::Forms::Padding(3);
		this->PageBalloon->Size = System::Drawing::Size(636, 481);
		this->PageBalloon->TabIndex = 1;
		this->PageBalloon->Text = L"Balloon";
		this->PageBalloon->UseVisualStyleBackColor = true;
		//
		// CharBalloonBorderColorName
		//
		this->CharBalloonBorderColorName->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", this->BalloonBindingSource,
			L"BorderColor", true, System::Windows::Forms::DataSourceUpdateMode::Never, nullptr, L"X8")));
		this->CharBalloonBorderColorName->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding,
			L"Bound", true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharBalloonBorderColorName->Location = System::Drawing::Point(520, 77);
		this->CharBalloonBorderColorName->Name = L"CharBalloonBorderColorName";
		this->CharBalloonBorderColorName->Size = System::Drawing::Size(110, 22);
		this->CharBalloonBorderColorName->TabIndex = 15;
		//
		// BalloonBindingSource
		//
		this->BalloonBindingSource->DataSource = DoubleAgent::Control::Balloon::typeid;
		//
		// CharBalloonTextColorName
		//
		this->CharBalloonTextColorName->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", this->BalloonBindingSource,
			L"TextColor", true, System::Windows::Forms::DataSourceUpdateMode::Never, nullptr, L"X8")));
		this->CharBalloonTextColorName->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding,
			L"Bound", true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharBalloonTextColorName->Location = System::Drawing::Point(301, 77);
		this->CharBalloonTextColorName->Name = L"CharBalloonTextColorName";
		this->CharBalloonTextColorName->Size = System::Drawing::Size(110, 22);
		this->CharBalloonTextColorName->TabIndex = 13;
		//
		// CharBalloonBackColorName
		//
		this->CharBalloonBackColorName->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", this->BalloonBindingSource,
			L"BackColor", true, System::Windows::Forms::DataSourceUpdateMode::Never, nullptr, L"X8")));
		this->CharBalloonBackColorName->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding,
			L"Bound", true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharBalloonBackColorName->Location = System::Drawing::Point(103, 77);
		this->CharBalloonBackColorName->Name = L"CharBalloonBackColorName";
		this->CharBalloonBackColorName->Size = System::Drawing::Size(110, 22);
		this->CharBalloonBackColorName->TabIndex = 11;
		//
		// CharBalloonFontStrikeThru
		//
		this->CharBalloonFontStrikeThru->AutoSize = true;
		this->CharBalloonFontStrikeThru->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->BalloonBindingSource,
			L"FontStrikethru", true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->CharBalloonFontStrikeThru->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding,
			L"Bound", true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharBalloonFontStrikeThru->Location = System::Drawing::Point(320, 146);
		this->CharBalloonFontStrikeThru->Name = L"CharBalloonFontStrikeThru";
		this->CharBalloonFontStrikeThru->Size = System::Drawing::Size(124, 21);
		this->CharBalloonFontStrikeThru->TabIndex = 25;
		this->CharBalloonFontStrikeThru->Text = L"FontStrikeThru";
		this->CharBalloonFontStrikeThru->UseVisualStyleBackColor = true;
		//
		// CharBalloonFontUnderline
		//
		this->CharBalloonFontUnderline->AutoSize = true;
		this->CharBalloonFontUnderline->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->BalloonBindingSource,
			L"FontUnderline", true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->CharBalloonFontUnderline->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding,
			L"Bound", true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharBalloonFontUnderline->Location = System::Drawing::Point(195, 146);
		this->CharBalloonFontUnderline->Name = L"CharBalloonFontUnderline";
		this->CharBalloonFontUnderline->Size = System::Drawing::Size(119, 21);
		this->CharBalloonFontUnderline->TabIndex = 24;
		this->CharBalloonFontUnderline->Text = L"FontUnderline";
		this->CharBalloonFontUnderline->UseVisualStyleBackColor = true;
		//
		// CharBalloonFontItalic
		//
		this->CharBalloonFontItalic->AutoSize = true;
		this->CharBalloonFontItalic->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->BalloonBindingSource,
			L"FontItalic", true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->CharBalloonFontItalic->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding,
			L"Bound", true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharBalloonFontItalic->Location = System::Drawing::Point(103, 146);
		this->CharBalloonFontItalic->Name = L"CharBalloonFontItalic";
		this->CharBalloonFontItalic->Size = System::Drawing::Size(86, 21);
		this->CharBalloonFontItalic->TabIndex = 23;
		this->CharBalloonFontItalic->Text = L"FontItalic";
		this->CharBalloonFontItalic->UseVisualStyleBackColor = true;
		//
		// CharBalloonFontBold
		//
		this->CharBalloonFontBold->AutoSize = true;
		this->CharBalloonFontBold->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->BalloonBindingSource, L"FontBold",
			true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->CharBalloonFontBold->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharBalloonFontBold->Location = System::Drawing::Point(9, 146);
		this->CharBalloonFontBold->Name = L"CharBalloonFontBold";
		this->CharBalloonFontBold->Size = System::Drawing::Size(86, 21);
		this->CharBalloonFontBold->TabIndex = 22;
		this->CharBalloonFontBold->Text = L"FontBold";
		this->CharBalloonFontBold->UseVisualStyleBackColor = true;
		//
		// CharBalloonFontCharSet
		//
		this->CharBalloonFontCharSet->BackColor = System::Drawing::SystemColors::Window;
		this->CharBalloonFontCharSet->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", this->BalloonBindingSource, L"FontCharSet",
			true, System::Windows::Forms::DataSourceUpdateMode::Never, nullptr, L"N0")));
		this->CharBalloonFontCharSet->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding,
			L"Bound", true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharBalloonFontCharSet->ForeColor = System::Drawing::SystemColors::GrayText;
		this->CharBalloonFontCharSet->Location = System::Drawing::Point(520, 115);
		this->CharBalloonFontCharSet->Name = L"CharBalloonFontCharSet";
		this->CharBalloonFontCharSet->ReadOnly = true;
		this->CharBalloonFontCharSet->Size = System::Drawing::Size(110, 22);
		this->CharBalloonFontCharSet->TabIndex = 21;
		this->CharBalloonFontCharSet->TabStop = false;
		//
		// CharBalloonFontSize
		//
		this->CharBalloonFontSize->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharBalloonFontSize->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Value", this->BalloonBindingSource, L"FontSize",
			true)));
		this->CharBalloonFontSize->ForeColor = System::Drawing::SystemColors::WindowText;
		this->CharBalloonFontSize->Location = System::Drawing::Point(301, 115);
		this->CharBalloonFontSize->Name = L"CharBalloonFontSize";
		this->CharBalloonFontSize->Size = System::Drawing::Size(110, 22);
		this->CharBalloonFontSize->TabIndex = 19;
		//
		// CharBalloonFontName
		//
		this->CharBalloonFontName->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharBalloonFontName->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", this->BalloonBindingSource, L"FontName",
			true)));
		this->CharBalloonFontName->ForeColor = System::Drawing::SystemColors::GrayText;
		this->CharBalloonFontName->Location = System::Drawing::Point(103, 115);
		this->CharBalloonFontName->Name = L"CharBalloonFontName";
		this->CharBalloonFontName->Size = System::Drawing::Size(110, 22);
		this->CharBalloonFontName->TabIndex = 17;
		//
		// CharBalloonAutoPace
		//
		this->CharBalloonAutoPace->AutoSize = true;
		this->CharBalloonAutoPace->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->BalloonBindingSource, L"AutoPace",
			true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->CharBalloonAutoPace->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharBalloonAutoPace->Location = System::Drawing::Point(103, 12);
		this->CharBalloonAutoPace->Name = L"CharBalloonAutoPace";
		this->CharBalloonAutoPace->Size = System::Drawing::Size(91, 21);
		this->CharBalloonAutoPace->TabIndex = 2;
		this->CharBalloonAutoPace->Text = L"AutoPace";
		this->CharBalloonAutoPace->UseVisualStyleBackColor = true;
		//
		// CharBalloonAutoHide
		//
		this->CharBalloonAutoHide->AutoSize = true;
		this->CharBalloonAutoHide->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->BalloonBindingSource, L"AutoHide",
			true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->CharBalloonAutoHide->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharBalloonAutoHide->Location = System::Drawing::Point(210, 12);
		this->CharBalloonAutoHide->Name = L"CharBalloonAutoHide";
		this->CharBalloonAutoHide->Size = System::Drawing::Size(88, 21);
		this->CharBalloonAutoHide->TabIndex = 4;
		this->CharBalloonAutoHide->Text = L"AutoHide";
		this->CharBalloonAutoHide->UseVisualStyleBackColor = true;
		//
		// CharBalloonShowPartialLines
		//
		this->CharBalloonShowPartialLines->AutoSize = true;
		this->CharBalloonShowPartialLines->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->BalloonBindingSource,
			L"ShowPartialLines", true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->CharBalloonShowPartialLines->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding,
			L"Bound", true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharBalloonShowPartialLines->Location = System::Drawing::Point(210, 39);
		this->CharBalloonShowPartialLines->Name = L"CharBalloonShowPartialLines";
		this->CharBalloonShowPartialLines->Size = System::Drawing::Size(138, 21);
		this->CharBalloonShowPartialLines->TabIndex = 5;
		this->CharBalloonShowPartialLines->Text = L"ShowPartialLines";
		this->CharBalloonShowPartialLines->UseVisualStyleBackColor = true;
		//
		// CharBalloonSizeToText
		//
		this->CharBalloonSizeToText->AutoSize = true;
		this->CharBalloonSizeToText->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->BalloonBindingSource,
			L"SizeToText", true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->CharBalloonSizeToText->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding,
			L"Bound", true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharBalloonSizeToText->Location = System::Drawing::Point(103, 39);
		this->CharBalloonSizeToText->Name = L"CharBalloonSizeToText";
		this->CharBalloonSizeToText->Size = System::Drawing::Size(101, 21);
		this->CharBalloonSizeToText->TabIndex = 3;
		this->CharBalloonSizeToText->Text = L"SizeToText";
		this->CharBalloonSizeToText->UseVisualStyleBackColor = true;
		//
		// CharBalloonCharsPerLine
		//
		this->CharBalloonCharsPerLine->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding,
			L"Bound", true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharBalloonCharsPerLine->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Value", this->BalloonBindingSource,
			L"CharsPerLine", true)));
		this->CharBalloonCharsPerLine->Location = System::Drawing::Point(520, 38);
		this->CharBalloonCharsPerLine->Name = L"CharBalloonCharsPerLine";
		this->CharBalloonCharsPerLine->Size = System::Drawing::Size(110, 22);
		this->CharBalloonCharsPerLine->TabIndex = 9;
		//
		// CharBalloonNumberOfLines
		//
		this->CharBalloonNumberOfLines->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding,
			L"Bound", true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharBalloonNumberOfLines->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Value", this->BalloonBindingSource,
			L"NumberOfLines", true)));
		this->CharBalloonNumberOfLines->Location = System::Drawing::Point(520, 10);
		this->CharBalloonNumberOfLines->Name = L"CharBalloonNumberOfLines";
		this->CharBalloonNumberOfLines->Size = System::Drawing::Size(110, 22);
		this->CharBalloonNumberOfLines->TabIndex = 7;
		//
		// CharBalloonVisible
		//
		this->CharBalloonVisible->AutoSize = true;
		this->CharBalloonVisible->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->BalloonBindingSource, L"Visible",
			true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->CharBalloonVisible->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharBalloonVisible->Location = System::Drawing::Point(6, 39);
		this->CharBalloonVisible->Name = L"CharBalloonVisible";
		this->CharBalloonVisible->Size = System::Drawing::Size(71, 21);
		this->CharBalloonVisible->TabIndex = 1;
		this->CharBalloonVisible->Text = L"Visible";
		this->CharBalloonVisible->UseVisualStyleBackColor = true;
		//
		// CharBalloonEnabled
		//
		this->CharBalloonEnabled->AutoSize = true;
		this->CharBalloonEnabled->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->BalloonBindingSource, L"Enabled",
			true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->CharBalloonEnabled->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true, System::Windows::Forms::DataSourceUpdateMode::Never)));
		this->CharBalloonEnabled->Location = System::Drawing::Point(6, 12);
		this->CharBalloonEnabled->Name = L"CharBalloonEnabled";
		this->CharBalloonEnabled->Size = System::Drawing::Size(82, 21);
		this->CharBalloonEnabled->TabIndex = 0;
		this->CharBalloonEnabled->Text = L"Enabled";
		this->CharBalloonEnabled->UseVisualStyleBackColor = true;
		//
		// PageCommands
		//
		this->PageCommands->Controls->Add(this->CharCommands);
		this->PageCommands->Controls->Add(this->GlobalVoiceCommandsEnabled);
		this->PageCommands->Controls->Add(this->CharCommandsVisible);
		this->PageCommands->Controls->Add(this->CharCommandsDefaultCommand);
		this->PageCommands->Controls->Add(label18);
		this->PageCommands->Controls->Add(this->CharCommandsFontSize);
		this->PageCommands->Controls->Add(label17);
		this->PageCommands->Controls->Add(this->CharCommandsFontName);
		this->PageCommands->Controls->Add(label16);
		this->PageCommands->Controls->Add(this->CharCommandsVoiceGrammar);
		this->PageCommands->Controls->Add(label15);
		this->PageCommands->Controls->Add(this->CharCommandsVoiceCaption);
		this->PageCommands->Controls->Add(label14);
		this->PageCommands->Controls->Add(this->CharCommandsCaption);
		this->PageCommands->Controls->Add(label13);
		this->PageCommands->Location = System::Drawing::Point(4, 25);
		this->PageCommands->Name = L"PageCommands";
		this->PageCommands->Padding = System::Windows::Forms::Padding(3);
		this->PageCommands->Size = System::Drawing::Size(636, 481);
		this->PageCommands->TabIndex = 2;
		this->PageCommands->Text = L"Commands";
		this->PageCommands->UseVisualStyleBackColor = true;
		//
		// CharCommands
		//
		this->CharCommands->AllowUserToResizeRows = false;
		this->CharCommands->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
			| System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		this->CharCommands->AutoGenerateColumns = false;
		this->CharCommands->BackgroundColor = System::Drawing::SystemColors::Control;
		this->CharCommands->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::DisableResizing;
		this->CharCommands->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(8) {this->CharCommandsNameCol,
			this->CharCommandsCaptionCol, this->CharCommandsVoiceCaptionCol, this->CharCommandsVoiceGrammarCol, this->CharCommandsEnabledCol,
			this->CharCommandsVisibleCol, this->CharCommandsConfidenceThresholdCol, this->CharCommandsConfidenceTextCol});
		this->CharCommands->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true)));
		this->CharCommands->DataSource = this->CommandBindingSource;
		this->CharCommands->Location = System::Drawing::Point(9, 162);
		this->CharCommands->MultiSelect = false;
		this->CharCommands->Name = L"CharCommands";
		this->CharCommands->ReadOnly = true;
		this->CharCommands->RowHeadersWidthSizeMode = System::Windows::Forms::DataGridViewRowHeadersWidthSizeMode::DisableResizing;
		this->CharCommands->RowTemplate->Height = 24;
		this->CharCommands->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
		this->CharCommands->Size = System::Drawing::Size(619, 313);
		this->CharCommands->TabIndex = 14;
		//
		// CharCommandsNameCol
		//
		this->CharCommandsNameCol->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::AllCells;
		this->CharCommandsNameCol->DataPropertyName = L"Name";
		this->CharCommandsNameCol->HeaderText = L"Name";
		this->CharCommandsNameCol->Name = L"CharCommandsNameCol";
		this->CharCommandsNameCol->ReadOnly = true;
		this->CharCommandsNameCol->Width = 70;
		//
		// CharCommandsCaptionCol
		//
		this->CharCommandsCaptionCol->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::AllCells;
		this->CharCommandsCaptionCol->DataPropertyName = L"Caption";
		this->CharCommandsCaptionCol->HeaderText = L"Caption";
		this->CharCommandsCaptionCol->Name = L"CharCommandsCaptionCol";
		this->CharCommandsCaptionCol->ReadOnly = true;
		this->CharCommandsCaptionCol->Width = 81;
		//
		// CharCommandsVoiceCaptionCol
		//
		this->CharCommandsVoiceCaptionCol->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::AllCells;
		this->CharCommandsVoiceCaptionCol->DataPropertyName = L"VoiceCaption";
		this->CharCommandsVoiceCaptionCol->HeaderText = L"VoiceCaption";
		this->CharCommandsVoiceCaptionCol->Name = L"CharCommandsVoiceCaptionCol";
		this->CharCommandsVoiceCaptionCol->ReadOnly = true;
		this->CharCommandsVoiceCaptionCol->Width = 116;
		//
		// CharCommandsVoiceGrammarCol
		//
		this->CharCommandsVoiceGrammarCol->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::AllCells;
		this->CharCommandsVoiceGrammarCol->DataPropertyName = L"VoiceGrammar";
		this->CharCommandsVoiceGrammarCol->HeaderText = L"VoiceGrammar";
		this->CharCommandsVoiceGrammarCol->Name = L"CharCommandsVoiceGrammarCol";
		this->CharCommandsVoiceGrammarCol->ReadOnly = true;
		this->CharCommandsVoiceGrammarCol->Width = 127;
		//
		// CharCommandsEnabledCol
		//
		this->CharCommandsEnabledCol->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::AllCells;
		this->CharCommandsEnabledCol->DataPropertyName = L"Enabled";
		this->CharCommandsEnabledCol->HeaderText = L"Enabled";
		this->CharCommandsEnabledCol->Name = L"CharCommandsEnabledCol";
		this->CharCommandsEnabledCol->ReadOnly = true;
		this->CharCommandsEnabledCol->Width = 66;
		//
		// CharCommandsVisibleCol
		//
		this->CharCommandsVisibleCol->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::AllCells;
		this->CharCommandsVisibleCol->DataPropertyName = L"Visible";
		this->CharCommandsVisibleCol->HeaderText = L"Visible";
		this->CharCommandsVisibleCol->Name = L"CharCommandsVisibleCol";
		this->CharCommandsVisibleCol->ReadOnly = true;
		this->CharCommandsVisibleCol->Width = 55;
		//
		// CharCommandsConfidenceThresholdCol
		//
		this->CharCommandsConfidenceThresholdCol->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::AllCells;
		this->CharCommandsConfidenceThresholdCol->DataPropertyName = L"ConfidenceThreshold";
		this->CharCommandsConfidenceThresholdCol->HeaderText = L"ConfidenceThreshold";
		this->CharCommandsConfidenceThresholdCol->Name = L"CharCommandsConfidenceThresholdCol";
		this->CharCommandsConfidenceThresholdCol->ReadOnly = true;
		this->CharCommandsConfidenceThresholdCol->Width = 168;
		//
		// CharCommandsConfidenceTextCol
		//
		this->CharCommandsConfidenceTextCol->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::AllCells;
		this->CharCommandsConfidenceTextCol->DataPropertyName = L"ConfidenceText";
		this->CharCommandsConfidenceTextCol->HeaderText = L"ConfidenceText";
		this->CharCommandsConfidenceTextCol->Name = L"CharCommandsConfidenceTextCol";
		this->CharCommandsConfidenceTextCol->ReadOnly = true;
		this->CharCommandsConfidenceTextCol->Width = 131;
		//
		// CommandBindingSource
		//
		this->CommandBindingSource->DataSource = DoubleAgent::Control::Command::typeid;
		//
		// GlobalVoiceCommandsEnabled
		//
		this->GlobalVoiceCommandsEnabled->AutoSize = true;
		this->GlobalVoiceCommandsEnabled->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->CommandsBindingSource,
			L"GlobalVoiceCommandsEnabled", true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->GlobalVoiceCommandsEnabled->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding,
			L"Bound", true)));
		this->GlobalVoiceCommandsEnabled->Location = System::Drawing::Point(130, 132);
		this->GlobalVoiceCommandsEnabled->Name = L"GlobalVoiceCommandsEnabled";
		this->GlobalVoiceCommandsEnabled->Size = System::Drawing::Size(228, 21);
		this->GlobalVoiceCommandsEnabled->TabIndex = 13;
		this->GlobalVoiceCommandsEnabled->Text = L"GlobalVoiceCommandsEnabled";
		this->GlobalVoiceCommandsEnabled->UseVisualStyleBackColor = true;
		//
		// CommandsBindingSource
		//
		this->CommandsBindingSource->DataSource = DoubleAgent::Control::Commands::typeid;
		//
		// CharCommandsVisible
		//
		this->CharCommandsVisible->AutoSize = true;
		this->CharCommandsVisible->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Checked", this->CommandsBindingSource,
			L"Visible", true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->CharCommandsVisible->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true)));
		this->CharCommandsVisible->Location = System::Drawing::Point(11, 132);
		this->CharCommandsVisible->Name = L"CharCommandsVisible";
		this->CharCommandsVisible->Size = System::Drawing::Size(71, 21);
		this->CharCommandsVisible->TabIndex = 12;
		this->CharCommandsVisible->Text = L"Visible";
		this->CharCommandsVisible->UseVisualStyleBackColor = true;
		//
		// CharCommandsDefaultCommand
		//
		this->CharCommandsDefaultCommand->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding,
			L"Bound", true)));
		this->CharCommandsDefaultCommand->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", this->CommandsBindingSource,
			L"DefaultCommand", true)));
		this->CharCommandsDefaultCommand->Location = System::Drawing::Point(130, 96);
		this->CharCommandsDefaultCommand->Name = L"CharCommandsDefaultCommand";
		this->CharCommandsDefaultCommand->Size = System::Drawing::Size(192, 22);
		this->CharCommandsDefaultCommand->TabIndex = 11;
		//
		// CharCommandsFontSize
		//
		this->CharCommandsFontSize->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding,
			L"Bound", true)));
		this->CharCommandsFontSize->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Value", this->CommandsBindingSource, L"FontSize",
			true)));
		this->CharCommandsFontSize->Location = System::Drawing::Point(437, 69);
		this->CharCommandsFontSize->Name = L"CharCommandsFontSize";
		this->CharCommandsFontSize->Size = System::Drawing::Size(118, 22);
		this->CharCommandsFontSize->TabIndex = 9;
		//
		// CharCommandsFontName
		//
		this->CharCommandsFontName->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding,
			L"Bound", true)));
		this->CharCommandsFontName->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", this->CommandsBindingSource, L"FontName",
			true)));
		this->CharCommandsFontName->Location = System::Drawing::Point(130, 68);
		this->CharCommandsFontName->Name = L"CharCommandsFontName";
		this->CharCommandsFontName->Size = System::Drawing::Size(192, 22);
		this->CharCommandsFontName->TabIndex = 7;
		//
		// CharCommandsVoiceGrammar
		//
		this->CharCommandsVoiceGrammar->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		this->CharCommandsVoiceGrammar->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding,
			L"Bound", true)));
		this->CharCommandsVoiceGrammar->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", this->CommandsBindingSource,
			L"VoiceGrammar", true)));
		this->CharCommandsVoiceGrammar->Location = System::Drawing::Point(130, 40);
		this->CharCommandsVoiceGrammar->Name = L"CharCommandsVoiceGrammar";
		this->CharCommandsVoiceGrammar->Size = System::Drawing::Size(499, 22);
		this->CharCommandsVoiceGrammar->TabIndex = 5;
		//
		// CharCommandsVoiceCaption
		//
		this->CharCommandsVoiceCaption->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding,
			L"Bound", true)));
		this->CharCommandsVoiceCaption->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", this->CommandsBindingSource,
			L"VoiceCaption", true)));
		this->CharCommandsVoiceCaption->Location = System::Drawing::Point(437, 12);
		this->CharCommandsVoiceCaption->Name = L"CharCommandsVoiceCaption";
		this->CharCommandsVoiceCaption->Size = System::Drawing::Size(192, 22);
		this->CharCommandsVoiceCaption->TabIndex = 3;
		//
		// CharCommandsCaption
		//
		this->CharCommandsCaption->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Enabled", this->CharacterDataBinding, L"Bound",
			true)));
		this->CharCommandsCaption->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Text", this->CommandsBindingSource, L"Caption",
			true)));
		this->CharCommandsCaption->Location = System::Drawing::Point(130, 12);
		this->CharCommandsCaption->Name = L"CharCommandsCaption";
		this->CharCommandsCaption->Size = System::Drawing::Size(192, 22);
		this->CharCommandsCaption->TabIndex = 1;
		//
		// PageMainControl
		//
		this->PageMainControl->Controls->Add(this->TabsControl);
		this->PageMainControl->Location = System::Drawing::Point(4, 25);
		this->PageMainControl->Name = L"PageMainControl";
		this->PageMainControl->Padding = System::Windows::Forms::Padding(3);
		this->PageMainControl->Size = System::Drawing::Size(656, 522);
		this->PageMainControl->TabIndex = 1;
		this->PageMainControl->Text = L"Control";
		this->PageMainControl->UseVisualStyleBackColor = true;
		//
		// PageMainEvents
		//
		this->PageMainEvents->Controls->Add(this->EventsGridClear);
		this->PageMainEvents->Controls->Add(this->EventsGrid);
		this->PageMainEvents->Location = System::Drawing::Point(4, 25);
		this->PageMainEvents->Name = L"PageMainEvents";
		this->PageMainEvents->Padding = System::Windows::Forms::Padding(3);
		this->PageMainEvents->Size = System::Drawing::Size(656, 522);
		this->PageMainEvents->TabIndex = 5;
		this->PageMainEvents->Text = L"Events";
		this->PageMainEvents->UseVisualStyleBackColor = true;
		//
		// EventsGridClear
		//
		this->EventsGridClear->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
		this->EventsGridClear->CausesValidation = false;
		this->EventsGridClear->Enabled = false;
		this->EventsGridClear->Location = System::Drawing::Point(564, 6);
		this->EventsGridClear->Name = L"EventsGridClear";
		this->EventsGridClear->Size = System::Drawing::Size(86, 27);
		this->EventsGridClear->TabIndex = 0;
		this->EventsGridClear->Text = L"Clear";
		this->EventsGridClear->UseVisualStyleBackColor = true;
		this->EventsGridClear->Click += gcnew System::EventHandler(this, &ControlTestForm::EventsGridClear_Click);
		//
		// EventsGrid
		//
		this->EventsGrid->AllowUserToAddRows = false;
		this->EventsGrid->AllowUserToResizeRows = false;
		this->EventsGrid->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
			| System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		this->EventsGrid->AutoGenerateColumns = false;
		this->EventsGrid->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::AllCells;
		this->EventsGrid->BackgroundColor = System::Drawing::SystemColors::Control;
		this->EventsGrid->CausesValidation = false;
		this->EventsGrid->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
		this->EventsGrid->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(3) {this->EventNameCol,
			this->EventCharacterIDCol, this->EventDataCol});
		this->EventsGrid->DataSource = this->EventsBindingSource;
		this->EventsGrid->Location = System::Drawing::Point(6, 39);
		this->EventsGrid->Name = L"EventsGrid";
		this->EventsGrid->RowHeadersVisible = false;
		this->EventsGrid->RowHeadersWidthSizeMode = System::Windows::Forms::DataGridViewRowHeadersWidthSizeMode::DisableResizing;
		this->EventsGrid->RowTemplate->Height = 24;
		this->EventsGrid->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
		this->EventsGrid->Size = System::Drawing::Size(644, 477);
		this->EventsGrid->TabIndex = 1;
		//
		// EventNameCol
		//
		this->EventNameCol->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::AllCells;
		this->EventNameCol->DataPropertyName = L"Name";
		this->EventNameCol->HeaderText = L"Event";
		this->EventNameCol->Name = L"EventNameCol";
		this->EventNameCol->ReadOnly = true;
		this->EventNameCol->Width = 69;
		//
		// EventCharacterIDCol
		//
		this->EventCharacterIDCol->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::AllCells;
		this->EventCharacterIDCol->DataPropertyName = L"CharacterID";
		this->EventCharacterIDCol->HeaderText = L"Character";
		this->EventCharacterIDCol->Name = L"EventCharacterIDCol";
		this->EventCharacterIDCol->ReadOnly = true;
		this->EventCharacterIDCol->Width = 95;
		//
		// EventDataCol
		//
		this->EventDataCol->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
		this->EventDataCol->DataPropertyName = L"Data";
		this->EventDataCol->HeaderText = L"Data";
		this->EventDataCol->Name = L"EventDataCol";
		this->EventDataCol->ReadOnly = true;
		//
		// EventsBindingSource
		//
		this->EventsBindingSource->ListChanged += gcnew System::ComponentModel::ListChangedEventHandler(this, &ControlTestForm::EventsBindingSource_ListChanged);
		//
		// UnloadButton
		//
		this->UnloadButton->Enabled = false;
		this->UnloadButton->Location = System::Drawing::Point(12, 374);
		this->UnloadButton->Name = L"UnloadButton";
		this->UnloadButton->Size = System::Drawing::Size(108, 32);
		this->UnloadButton->TabIndex = 5;
		this->UnloadButton->Text = L"Unload";
		this->UnloadButton->UseVisualStyleBackColor = true;
		this->UnloadButton->Click += gcnew System::EventHandler(this, &ControlTestForm::UnloadButton_Click);
		//
		// SetCurrentButton
		//
		this->SetCurrentButton->Enabled = false;
		this->SetCurrentButton->Location = System::Drawing::Point(12, 412);
		this->SetCurrentButton->Name = L"SetCurrentButton";
		this->SetCurrentButton->Size = System::Drawing::Size(108, 54);
		this->SetCurrentButton->TabIndex = 6;
		this->SetCurrentButton->Text = L"Select";
		this->SetCurrentButton->UseVisualStyleBackColor = true;
		this->SetCurrentButton->Click += gcnew System::EventHandler(this, &ControlTestForm::SetCurrentButton_Click);
		//
		// CollectButton
		//
		this->CollectButton->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
		this->CollectButton->Location = System::Drawing::Point(12, 509);
		this->CollectButton->Name = L"CollectButton";
		this->CollectButton->Size = System::Drawing::Size(108, 54);
		this->CollectButton->TabIndex = 8;
		this->CollectButton->Text = L"Collect garbage";
		this->CollectButton->UseVisualStyleBackColor = true;
		this->CollectButton->Click += gcnew System::EventHandler(this, &ControlTestForm::CollectButton_Click);
		//
		// ControlTestForm
		//
		this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
		this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		this->AutoValidate = System::Windows::Forms::AutoValidate::EnableAllowFocusChange;
		this->ClientSize = System::Drawing::Size(894, 575);
		this->Controls->Add(this->CollectButton);
		this->Controls->Add(this->TabsMain);
		this->Controls->Add(this->SetCurrentButton);
		this->Controls->Add(this->UnloadButton);
		this->Controls->Add(this->LoadButton);
		this->Controls->Add(this->TestDaControl);
		this->Controls->Add(this->RaiseRequestErrorsButton);
		this->Controls->Add(this->AutoConnectButton);
		this->Controls->Add(this->ControlConnectedButton);
		this->DataBindings->Add((gcnew System::Windows::Forms::Binding(L"Location", ::settings::Default, L"Location", true, System::Windows::Forms::DataSourceUpdateMode::OnPropertyChanged)));
		this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
		this->MinimumSize = System::Drawing::Size(912, 620);
		this->Name = L"ControlTestForm";
		this->Text = L"Control Test";
		this->Load += gcnew System::EventHandler(this, &ControlTestForm::ControlTestForm_Load);
		this->BindingContextChanged += gcnew System::EventHandler(this, &ControlTestForm::FormBindingContextChanged);
		this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &ControlTestForm::ControlTestForm_FormClosing);
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharacterDataBinding))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->SettingsDataBinding))->EndInit();
		PagePropertySheet->ResumeLayout(false);
		PagePropertySheet->PerformLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ShowDefaultCharacterY))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ShowDefaultCharacterX))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PropertySheetDataBinding))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PropSheetLeft))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PropSheetTop))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharacterFilesDataBinding))->EndInit();
		this->TabsControl->ResumeLayout(false);
		this->PageCharacters->ResumeLayout(false);
		this->PageCharacters->PerformLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharacterList))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharactersDataBinding))->EndInit();
		this->PageSettings->ResumeLayout(false);
		this->PageSettings->PerformLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->BalloonFontSize))->EndInit();
		this->PageCharacterFiles->ResumeLayout(false);
		this->PageCharacterFiles->PerformLayout();
		this->PageTTSEngines->ResumeLayout(false);
		this->PageTTSEngines->PerformLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TTSEnginesDataBinding))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TTSEngineList))->EndInit();
		this->PageSREngines->ResumeLayout(false);
		this->PageSREngines->PerformLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->SREnginesDataBinding))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->SREngineList))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TestDaControl))->EndInit();
		this->TabsMain->ResumeLayout(false);
		this->PageMainCharacter->ResumeLayout(false);
		this->TabsCharacter->ResumeLayout(false);
		this->PageCharacter->ResumeLayout(false);
		this->PageCharacter->PerformLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharHeight))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharacterBindingSource))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharWidth))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharTop))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharLeft))->EndInit();
		this->PageActions->ResumeLayout(false);
		this->PageActions->PerformLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharMoveToTop))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharShowMenuY))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharShowMenuX))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharGestureAtY))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharGestureAtX))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharMoveToLeft))->EndInit();
		this->PageBalloon->ResumeLayout(false);
		this->PageBalloon->PerformLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->BalloonBindingSource))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharBalloonFontSize))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharBalloonCharsPerLine))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharBalloonNumberOfLines))->EndInit();
		this->PageCommands->ResumeLayout(false);
		this->PageCommands->PerformLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharCommands))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CommandBindingSource))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CommandsBindingSource))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharCommandsFontSize))->EndInit();
		this->PageMainControl->ResumeLayout(false);
		this->PageMainEvents->ResumeLayout(false);
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->EventsGrid))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->EventsBindingSource))->EndInit();
		this->ResumeLayout(false);
		this->PerformLayout();

	}
#pragma endregion

//===========================================================================

private: System::Void ShowConnected ();
private: System::Void SelectCharacter (DoubleAgent::Control::Character^ pCharacter);
private: System::Void FormBindingContextChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void DataBoundChanged(System::Object^  sender, System::EventArgs^  e);

//===========================================================================

private: System::Void CollectButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		LogMessage (LogNormal, _T("GC::Collect..."));
		GC::Collect ();
		LogMessage (LogNormal, _T("GC::Collect done"));
	}
private: System::Void ControlTestForm_Load(System::Object^  sender, System::EventArgs^  e)
	{
		TabsMain->SelectTab (::settings::Default->MainPage);
		TabsControl->SelectTab (::settings::Default->ControlPage);
		TabsCharacter->SelectTab (::settings::Default->CharacterPage);
	}
private: System::Void ControlTestForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
	{
		::settings::Default->MainPage = TabsMain->SelectedIndex;
		::settings::Default->ControlPage = TabsControl->SelectedIndex;
		::settings::Default->CharacterPage = TabsCharacter->SelectedIndex;
		::settings::Default->Save();
	}

//===========================================================================

private: System::Void TestDaControl_IdleStart(System::Object^  sender, DoubleAgent::AxControl::IdleStartEvent^  e)
	{
		CharacterBindingSource->ResetCurrentItem ();
		EventsBindingSource->Add (gcnew EventData (e));
	}
private: System::Void TestDaControl_IdleComplete(System::Object^  sender, DoubleAgent::AxControl::IdleCompleteEvent^  e)
	{
		CharacterBindingSource->ResetCurrentItem ();
		EventsBindingSource->Add (gcnew EventData (e));
	}
private: System::Void TestDaControl_ListenStart(System::Object^  sender, DoubleAgent::AxControl::ListenStartEvent^  e)
	{
		CharacterBindingSource->ResetCurrentItem ();
		CharacterDataBinding->ResetCurrentItem ();
		EventsBindingSource->Add (gcnew EventData (e));
	}
private: System::Void TestDaControl_ListenComplete(System::Object^  sender, DoubleAgent::AxControl::ListenCompleteEvent^  e)
	{
		CharacterBindingSource->ResetCurrentItem ();
		CharacterDataBinding->ResetCurrentItem ();
		EventsBindingSource->Add (gcnew EventData (e));
	}
private: System::Void TestDaControl_PopupActivateInput(System::Object^  sender, DoubleAgent::AxControl::ActivateInputEvent^  e)
	{
		CharacterBindingSource->ResetCurrentItem ();
		CharacterDataBinding->ResetCurrentItem ();
		EventsBindingSource->Add (gcnew EventData (e));
	}
private: System::Void TestDaControl_PopupDeactivateInput(System::Object^  sender, DoubleAgent::AxControl::DeactivateInputEvent^  e)
	{
		CharacterBindingSource->ResetCurrentItem ();
		CharacterDataBinding->ResetCurrentItem ();
		EventsBindingSource->Add (gcnew EventData (e));
	}
private: System::Void TestDaControl_AgentShow(System::Object^  sender, DoubleAgent::AxControl::ShowEvent^  e)
	{
		CharacterBindingSource->ResetCurrentItem ();
		CharacterDataBinding->ResetCurrentItem ();
		EventsBindingSource->Add (gcnew EventData (e));
	}
private: System::Void TestDaControl_HideEvent(System::Object^  sender, DoubleAgent::AxControl::HideEvent^  e)
	{
		CharacterBindingSource->ResetCurrentItem ();
		CharacterDataBinding->ResetCurrentItem ();
		EventsBindingSource->Add (gcnew EventData (e));
	}
private: System::Void TestDaControl_MoveEvent(System::Object^  sender, DoubleAgent::AxControl::MoveEvent^  e)
	{
		CharacterBindingSource->ResetCurrentItem ();
		CharacterDataBinding->ResetCurrentItem ();
		EventsBindingSource->Add (gcnew EventData (e));
	}
private: System::Void TestDaControl_SizeEvent(System::Object^  sender, DoubleAgent::AxControl::SizeEvent^  e)
	{
		CharacterBindingSource->ResetCurrentItem ();
		CharacterDataBinding->ResetCurrentItem ();
		EventsBindingSource->Add (gcnew EventData (e));
	}
private: System::Void TestDaControl_RequestStart(System::Object^  sender, DoubleAgent::AxControl::RequestStartEvent^  e)
	{
		CharacterBindingSource->ResetCurrentItem ();
		EventsBindingSource->Add (gcnew EventData (e));
	}
private: System::Void TestDaControl_RequestComplete(System::Object^  sender, DoubleAgent::AxControl::RequestCompleteEvent^  e)
	{
		CharacterBindingSource->ResetCurrentItem ();
		EventsBindingSource->Add (gcnew EventData (e));
	}
private: System::Void TestDaControl_PopupDragStart(System::Object^  sender, DoubleAgent::AxControl::DragStartEvent^  e)
	{
		CharacterBindingSource->ResetCurrentItem ();
		EventsBindingSource->Add (gcnew EventData (e));
	}
private: System::Void TestDaControl_PopupDragComplete(System::Object^  sender, DoubleAgent::AxControl::DragCompleteEvent^  e)
	{
		CharacterBindingSource->ResetCurrentItem ();
		CharacterDataBinding->ResetCurrentItem ();
		EventsBindingSource->Add (gcnew EventData (e));
	}
private: System::Void TestDaControl_BalloonHide(System::Object^  sender, DoubleAgent::AxControl::BalloonHideEvent^  e)
	{
		EventsBindingSource->Add (gcnew EventData (e));
	}
private: System::Void TestDaControl_BalloonShow(System::Object^  sender, DoubleAgent::AxControl::BalloonShowEvent^  e)
	{
		EventsBindingSource->Add (gcnew EventData (e));
	}
private: System::Void TestDaControl_Bookmark(System::Object^  sender, DoubleAgent::AxControl::BookmarkEvent^  e)
	{
		EventsBindingSource->Add (gcnew EventData (e));
	}
private: System::Void TestDaControl_Command(System::Object^  sender, DoubleAgent::AxControl::CommandEvent^  e)
	{
		EventsBindingSource->Add (gcnew EventData (e));
	}
private: System::Void TestDaControl_PopupClick(System::Object^  sender, DoubleAgent::AxControl::ClickEvent^  e)
	{
		EventsBindingSource->Add (gcnew EventData (e));
	}
private: System::Void TestDaControl_PopupDblClick(System::Object^  sender, DoubleAgent::AxControl::DblClickEvent^  e)
	{
		EventsBindingSource->Add (gcnew EventData (e));
	}
private: System::Void TestDaControl_ActiveClientChange(System::Object^  sender, DoubleAgent::AxControl::ActiveClientChangeEvent^  e)
	{
		EventsBindingSource->Add (gcnew EventData (e));
	}
private: System::Void TestDaControl_AgentPropertyChange(System::Object^  sender, System::EventArgs^  e)
	{
		EventsBindingSource->Add (gcnew EventData ("AgentPropertyChange"));
	}
private: System::Void TestDaControl_DefaultCharacterChange(System::Object^  sender, DoubleAgent::AxControl::DefaultCharacterChangeEvent^  e)
	{
		EventsBindingSource->Add (gcnew EventData (e));
	}

//===========================================================================

private: System::Void LoadButton_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void UnloadButton_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void SetCurrentButton_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void CharacterList_SelectionChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void CharacterList_DataBindingComplete(System::Object^  sender, System::Windows::Forms::DataGridViewBindingCompleteEventArgs^  e);

private: System::Void CharShow_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void CharHide_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void CharMoveTo_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void CharGestureAt_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void CharSpeak_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void CharThink_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void CharListen_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void CharListenStop_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void CharShowPopupMenu_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void CharPlay_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void CharStopAll_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void CharWidthDefault_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void CharHeightDefault_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void CharPlayAnimation_DoubleClick(System::Object^  sender, System::EventArgs^  e) {CharPlay_Click (sender, e);}
private: System::Void CharGenerateIcon_Click(System::Object^  sender, System::EventArgs^  e);

//===========================================================================

private: System::Void EventsBindingSource_ListChanged(System::Object^  sender, System::ComponentModel::ListChangedEventArgs^  e)
	{
		try
		{
			EventsGrid->FirstDisplayedScrollingRowIndex = EventsGrid->RowCount-1;
		}
		catch (...)
		{}
		try
		{
			EventsGridClear->Enabled = (EventsGrid->RowCount > 0);
		}
		catch (...)
		{}
	}
private: System::Void EventsGridClear_Click(System::Object^  sender, System::EventArgs^  e)
	{
		EventsBindingSource->Clear();
	}

//===========================================================================

private: System::Void ShowDefaultCharacter_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if	(ShowDefaultCharacterPos->Checked)
		{
			TestDaControl->ShowDefaultCharacterProperties (ShowDefaultCharacterX->Value, ShowDefaultCharacterY->Value);
		}
		else
		{
			TestDaControl->ShowDefaultCharacterProperties ();
		}
		ShowConnected ();
	}
};
/////////////////////////////////////////////////////////////////////////////
} // namespace DoubleAgent
