#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Diagnostics;

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

private: DoubleAgent::Control::AxControl^  TestDaControl;
private: System::Windows::Forms::CheckBox^  RaiseRequestErrorsButton;
private: System::Windows::Forms::CheckBox^  AutoConnectButton;
private: System::Windows::Forms::CheckBox^  ControlConnectedButton;
private: System::Windows::Forms::TabControl^  TestTabControl;

private: System::Windows::Forms::TabPage^  TabCharacters;
private: System::Windows::Forms::CheckBox^  CharactersConnected;
private: System::Windows::Forms::DataGridView^  CharacterList;

private: System::Windows::Forms::TabPage^  TabSettings;
private: System::Windows::Forms::CheckBox^  SettingsConnected;
private: System::Windows::Forms::CheckBox^  BalloonEnabled;
private: System::Windows::Forms::Label^  BalloonFontLabel;
private: System::Windows::Forms::TextBox^  BalloonFontName;
private: System::Windows::Forms::TextBox^  BalloonFontSize;
private: System::Windows::Forms::Label^  BalloonFontSizeLabel;
private: System::Windows::Forms::CheckBox^  BalloonFontUnderline;
private: System::Windows::Forms::CheckBox^  BalloonFontItalic;
private: System::Windows::Forms::CheckBox^  BalloonFontBold;
private: System::Windows::Forms::CheckBox^  SoundEffectsEnabled;
private: System::Windows::Forms::CheckBox^  TTSEnabled;
private: System::Windows::Forms::Label^  TTSSpeedLabel;
private: System::Windows::Forms::TextBox^  TTSSpeed;
private: System::Windows::Forms::CheckBox^  SREnabled;
private: System::Windows::Forms::TextBox^  SRHotKey;
private: System::Windows::Forms::Label^  SRHotKeyLabel;
private: System::Windows::Forms::TextBox^  SRHotKeyTime;
private: System::Windows::Forms::Label^  SRHotKeyTimeLabel;
private: System::Windows::Forms::CheckBox^  SRListeningTip;
private: System::Windows::Forms::CheckBox^  SRListeningPrompt;

private: System::Windows::Forms::TabPage^  TabPropertySheet;
private: System::Windows::Forms::CheckBox^  PropertySheetConnected;
private: System::Windows::Forms::TextBox^  PropSheetPage;
private: System::Windows::Forms::Label^  PropSheetHeightLabel;
private: System::Windows::Forms::Label^  PropSheetWidthLabel;
private: System::Windows::Forms::Label^  PropSheetLeftLabel;
private: System::Windows::Forms::Label^  PropSheetPageLabel;
private: System::Windows::Forms::Label^  PropSheetTopLabel;
private: System::Windows::Forms::CheckBox^  PropSheetVisible;
private: System::Windows::Forms::TextBox^  PropSheetWidth;
private: System::Windows::Forms::TextBox^  PropSheetHeight;
private: System::Windows::Forms::TextBox^  PropSheetLeft;
private: System::Windows::Forms::TextBox^  PropSheetTop;

private: System::Windows::Forms::TabPage^  TabCharacterFiles;
private: System::Windows::Forms::CheckBox^  FilesConnected;
private: System::Windows::Forms::CheckBox^  ShowDaFiles;
private: System::Windows::Forms::CheckBox^  ShowMaFiles;
private: System::Windows::Forms::CheckBox^  ShowOfficeFiles;
private: System::Windows::Forms::CheckBox^  ShowSpeakingFiles;
private: System::Windows::Forms::CheckBox^  ShowSilentFiles;
private: System::Windows::Forms::CheckBox^  VerifyFileVersion;
private: System::Windows::Forms::Label^  DefaultFileLabel;
private: System::Windows::Forms::TextBox^  DefaultFilePath;
private: System::Windows::Forms::Label^  DefaultSearchLabel;
private: System::Windows::Forms::TextBox^  DefaultSearchPath;
private: System::Windows::Forms::Label^  CurrentSearchLabel;
private: System::Windows::Forms::TextBox^  CurrentSearchPath;
private: System::Windows::Forms::ListBox^  FilesListBox;

private: System::Windows::Forms::TabPage^  TabTTSEngines;
private: System::Windows::Forms::CheckBox^  TTSEnginesConnected;
private: System::Windows::Forms::DataGridView^  TTSEngineList;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  TTSModeID;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  TTSDisplayName;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  TTSManufacturer;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  TTSGender;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  TTSLanguageID;

private: System::Windows::Forms::TabPage^  TabSREngines;
private: System::Windows::Forms::CheckBox^  SREnginesConnected;
private: System::Windows::Forms::DataGridView^  SREngineList;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  SRModeID;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  SRDisplayName;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  SRManufacturer;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  SRLanguageID;
private: System::Windows::Forms::Button^  LoadButton;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  CharListID;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  CharListName;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  CharListFileName;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  CharListFilePath;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  CharListVersion;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  CharListUniqueID;







private: System::ComponentModel::IContainer^  components;

//===========================================================================

#pragma region Windows Form Designer generated code
	/// <summary>
	/// Required method for Designer support - do not modify
	/// the contents of this method with the code editor.
	/// </summary>
	void InitializeComponent(void)
	{
		System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(ControlTestForm::typeid));
		this->TestDaControl = (gcnew DoubleAgent::Control::AxControl());
		this->RaiseRequestErrorsButton = (gcnew System::Windows::Forms::CheckBox());
		this->AutoConnectButton = (gcnew System::Windows::Forms::CheckBox());
		this->ControlConnectedButton = (gcnew System::Windows::Forms::CheckBox());
		this->TestTabControl = (gcnew System::Windows::Forms::TabControl());
		this->TabCharacters = (gcnew System::Windows::Forms::TabPage());
		this->CharacterList = (gcnew System::Windows::Forms::DataGridView());
		this->CharactersConnected = (gcnew System::Windows::Forms::CheckBox());
		this->TabSettings = (gcnew System::Windows::Forms::TabPage());
		this->SettingsConnected = (gcnew System::Windows::Forms::CheckBox());
		this->BalloonEnabled = (gcnew System::Windows::Forms::CheckBox());
		this->BalloonFontLabel = (gcnew System::Windows::Forms::Label());
		this->BalloonFontName = (gcnew System::Windows::Forms::TextBox());
		this->BalloonFontSizeLabel = (gcnew System::Windows::Forms::Label());
		this->BalloonFontSize = (gcnew System::Windows::Forms::TextBox());
		this->BalloonFontBold = (gcnew System::Windows::Forms::CheckBox());
		this->BalloonFontItalic = (gcnew System::Windows::Forms::CheckBox());
		this->BalloonFontUnderline = (gcnew System::Windows::Forms::CheckBox());
		this->SoundEffectsEnabled = (gcnew System::Windows::Forms::CheckBox());
		this->TTSEnabled = (gcnew System::Windows::Forms::CheckBox());
		this->TTSSpeedLabel = (gcnew System::Windows::Forms::Label());
		this->TTSSpeed = (gcnew System::Windows::Forms::TextBox());
		this->SREnabled = (gcnew System::Windows::Forms::CheckBox());
		this->SRHotKeyLabel = (gcnew System::Windows::Forms::Label());
		this->SRHotKey = (gcnew System::Windows::Forms::TextBox());
		this->SRHotKeyTimeLabel = (gcnew System::Windows::Forms::Label());
		this->SRHotKeyTime = (gcnew System::Windows::Forms::TextBox());
		this->SRListeningPrompt = (gcnew System::Windows::Forms::CheckBox());
		this->SRListeningTip = (gcnew System::Windows::Forms::CheckBox());
		this->TabPropertySheet = (gcnew System::Windows::Forms::TabPage());
		this->PropertySheetConnected = (gcnew System::Windows::Forms::CheckBox());
		this->PropSheetVisible = (gcnew System::Windows::Forms::CheckBox());
		this->PropSheetPageLabel = (gcnew System::Windows::Forms::Label());
		this->PropSheetPage = (gcnew System::Windows::Forms::TextBox());
		this->PropSheetLeftLabel = (gcnew System::Windows::Forms::Label());
		this->PropSheetLeft = (gcnew System::Windows::Forms::TextBox());
		this->PropSheetTopLabel = (gcnew System::Windows::Forms::Label());
		this->PropSheetTop = (gcnew System::Windows::Forms::TextBox());
		this->PropSheetWidthLabel = (gcnew System::Windows::Forms::Label());
		this->PropSheetWidth = (gcnew System::Windows::Forms::TextBox());
		this->PropSheetHeightLabel = (gcnew System::Windows::Forms::Label());
		this->PropSheetHeight = (gcnew System::Windows::Forms::TextBox());
		this->TabCharacterFiles = (gcnew System::Windows::Forms::TabPage());
		this->FilesConnected = (gcnew System::Windows::Forms::CheckBox());
		this->ShowDaFiles = (gcnew System::Windows::Forms::CheckBox());
		this->ShowMaFiles = (gcnew System::Windows::Forms::CheckBox());
		this->ShowOfficeFiles = (gcnew System::Windows::Forms::CheckBox());
		this->ShowSpeakingFiles = (gcnew System::Windows::Forms::CheckBox());
		this->ShowSilentFiles = (gcnew System::Windows::Forms::CheckBox());
		this->VerifyFileVersion = (gcnew System::Windows::Forms::CheckBox());
		this->DefaultFileLabel = (gcnew System::Windows::Forms::Label());
		this->DefaultFilePath = (gcnew System::Windows::Forms::TextBox());
		this->DefaultSearchLabel = (gcnew System::Windows::Forms::Label());
		this->DefaultSearchPath = (gcnew System::Windows::Forms::TextBox());
		this->CurrentSearchLabel = (gcnew System::Windows::Forms::Label());
		this->CurrentSearchPath = (gcnew System::Windows::Forms::TextBox());
		this->FilesListBox = (gcnew System::Windows::Forms::ListBox());
		this->TabTTSEngines = (gcnew System::Windows::Forms::TabPage());
		this->TTSEnginesConnected = (gcnew System::Windows::Forms::CheckBox());
		this->TTSEngineList = (gcnew System::Windows::Forms::DataGridView());
		this->TTSModeID = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->TTSDisplayName = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->TTSManufacturer = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->TTSGender = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->TTSLanguageID = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->TabSREngines = (gcnew System::Windows::Forms::TabPage());
		this->SREnginesConnected = (gcnew System::Windows::Forms::CheckBox());
		this->SREngineList = (gcnew System::Windows::Forms::DataGridView());
		this->SRModeID = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->SRDisplayName = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->SRManufacturer = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->SRLanguageID = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->LoadButton = (gcnew System::Windows::Forms::Button());
		this->CharListID = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->CharListName = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->CharListFileName = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->CharListFilePath = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->CharListVersion = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		this->CharListUniqueID = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TestDaControl))->BeginInit();
		this->TestTabControl->SuspendLayout();
		this->TabCharacters->SuspendLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharacterList))->BeginInit();
		this->TabSettings->SuspendLayout();
		this->TabPropertySheet->SuspendLayout();
		this->TabCharacterFiles->SuspendLayout();
		this->TabTTSEngines->SuspendLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TTSEngineList))->BeginInit();
		this->TabSREngines->SuspendLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->SREngineList))->BeginInit();
		this->SuspendLayout();
		// 
		// TestDaControl
		// 
		this->TestDaControl->AutoConnect = true;
		this->TestDaControl->BorderColor = System::Drawing::SystemColors::WindowText;
		this->TestDaControl->BorderStyle = 1;
		this->TestDaControl->BorderVisible = true;
		this->TestDaControl->BorderWidth = 1;
		this->TestDaControl->Enabled = true;
		this->TestDaControl->Location = System::Drawing::Point(12, 12);
		this->TestDaControl->Name = L"TestDaControl";
		this->TestDaControl->OcxState = (cli::safe_cast<System::Windows::Forms::AxHost::State^  >(resources->GetObject(L"TestDaControl.OcxState")));
		this->TestDaControl->RaiseRequestErrors = true;
		this->TestDaControl->Size = System::Drawing::Size(200, 200);
		this->TestDaControl->TabIndex = 0;
		this->TestDaControl->TabStop = false;
		// 
		// RaiseRequestErrorsButton
		// 
		this->RaiseRequestErrorsButton->AutoSize = true;
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
		// TestTabControl
		// 
		this->TestTabControl->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
			| System::Windows::Forms::AnchorStyles::Left) 
			| System::Windows::Forms::AnchorStyles::Right));
		this->TestTabControl->Controls->Add(this->TabCharacters);
		this->TestTabControl->Controls->Add(this->TabSettings);
		this->TestTabControl->Controls->Add(this->TabPropertySheet);
		this->TestTabControl->Controls->Add(this->TabCharacterFiles);
		this->TestTabControl->Controls->Add(this->TabTTSEngines);
		this->TestTabControl->Controls->Add(this->TabSREngines);
		this->TestTabControl->Location = System::Drawing::Point(229, 12);
		this->TestTabControl->Name = L"TestTabControl";
		this->TestTabControl->SelectedIndex = 0;
		this->TestTabControl->Size = System::Drawing::Size(654, 529);
		this->TestTabControl->TabIndex = 0;
		// 
		// TabCharacters
		// 
		this->TabCharacters->Controls->Add(this->CharacterList);
		this->TabCharacters->Controls->Add(this->CharactersConnected);
		this->TabCharacters->Location = System::Drawing::Point(4, 25);
		this->TabCharacters->Name = L"TabCharacters";
		this->TabCharacters->Padding = System::Windows::Forms::Padding(3);
		this->TabCharacters->Size = System::Drawing::Size(646, 500);
		this->TabCharacters->TabIndex = 0;
		this->TabCharacters->Text = L"Characters";
		this->TabCharacters->UseVisualStyleBackColor = true;
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
		this->CharacterList->Location = System::Drawing::Point(6, 39);
		this->CharacterList->MultiSelect = false;
		this->CharacterList->Name = L"CharacterList";
		this->CharacterList->ReadOnly = true;
		this->CharacterList->RowHeadersVisible = false;
		this->CharacterList->RowTemplate->Height = 24;
		this->CharacterList->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
		this->CharacterList->Size = System::Drawing::Size(634, 455);
		this->CharacterList->StandardTab = true;
		this->CharacterList->TabIndex = 2;
		// 
		// CharactersConnected
		// 
		this->CharactersConnected->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
		this->CharactersConnected->Appearance = System::Windows::Forms::Appearance::Button;
		this->CharactersConnected->AutoSize = true;
		this->CharactersConnected->Location = System::Drawing::Point(554, 6);
		this->CharactersConnected->Name = L"CharactersConnected";
		this->CharactersConnected->Size = System::Drawing::Size(86, 27);
		this->CharactersConnected->TabIndex = 1;
		this->CharactersConnected->Text = L"Connected";
		this->CharactersConnected->UseVisualStyleBackColor = true;
		// 
		// TabSettings
		// 
		this->TabSettings->Controls->Add(this->SettingsConnected);
		this->TabSettings->Controls->Add(this->BalloonEnabled);
		this->TabSettings->Controls->Add(this->BalloonFontLabel);
		this->TabSettings->Controls->Add(this->BalloonFontName);
		this->TabSettings->Controls->Add(this->BalloonFontSizeLabel);
		this->TabSettings->Controls->Add(this->BalloonFontSize);
		this->TabSettings->Controls->Add(this->BalloonFontBold);
		this->TabSettings->Controls->Add(this->BalloonFontItalic);
		this->TabSettings->Controls->Add(this->BalloonFontUnderline);
		this->TabSettings->Controls->Add(this->SoundEffectsEnabled);
		this->TabSettings->Controls->Add(this->TTSEnabled);
		this->TabSettings->Controls->Add(this->TTSSpeedLabel);
		this->TabSettings->Controls->Add(this->TTSSpeed);
		this->TabSettings->Controls->Add(this->SREnabled);
		this->TabSettings->Controls->Add(this->SRHotKeyLabel);
		this->TabSettings->Controls->Add(this->SRHotKey);
		this->TabSettings->Controls->Add(this->SRHotKeyTimeLabel);
		this->TabSettings->Controls->Add(this->SRHotKeyTime);
		this->TabSettings->Controls->Add(this->SRListeningPrompt);
		this->TabSettings->Controls->Add(this->SRListeningTip);
		this->TabSettings->Location = System::Drawing::Point(4, 25);
		this->TabSettings->Name = L"TabSettings";
		this->TabSettings->Padding = System::Windows::Forms::Padding(3);
		this->TabSettings->Size = System::Drawing::Size(646, 500);
		this->TabSettings->TabIndex = 0;
		this->TabSettings->Text = L"Settings";
		this->TabSettings->UseVisualStyleBackColor = true;
		// 
		// SettingsConnected
		// 
		this->SettingsConnected->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
		this->SettingsConnected->Appearance = System::Windows::Forms::Appearance::Button;
		this->SettingsConnected->AutoSize = true;
		this->SettingsConnected->Location = System::Drawing::Point(554, 6);
		this->SettingsConnected->Name = L"SettingsConnected";
		this->SettingsConnected->Size = System::Drawing::Size(86, 27);
		this->SettingsConnected->TabIndex = 0;
		this->SettingsConnected->Text = L"Connected";
		this->SettingsConnected->UseVisualStyleBackColor = true;
		// 
		// BalloonEnabled
		// 
		this->BalloonEnabled->AutoCheck = false;
		this->BalloonEnabled->AutoSize = true;
		this->BalloonEnabled->Enabled = false;
		this->BalloonEnabled->Location = System::Drawing::Point(6, 7);
		this->BalloonEnabled->Name = L"BalloonEnabled";
		this->BalloonEnabled->Size = System::Drawing::Size(132, 21);
		this->BalloonEnabled->TabIndex = 1;
		this->BalloonEnabled->Text = L"Balloon enabled";
		this->BalloonEnabled->UseVisualStyleBackColor = true;
		// 
		// BalloonFontLabel
		// 
		this->BalloonFontLabel->AutoSize = true;
		this->BalloonFontLabel->Location = System::Drawing::Point(154, 9);
		this->BalloonFontLabel->Name = L"BalloonFontLabel";
		this->BalloonFontLabel->Size = System::Drawing::Size(36, 17);
		this->BalloonFontLabel->TabIndex = 2;
		this->BalloonFontLabel->Text = L"Font";
		this->BalloonFontLabel->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// BalloonFontName
		// 
		this->BalloonFontName->Enabled = false;
		this->BalloonFontName->Location = System::Drawing::Point(244, 6);
		this->BalloonFontName->Name = L"BalloonFontName";
		this->BalloonFontName->ReadOnly = true;
		this->BalloonFontName->Size = System::Drawing::Size(285, 22);
		this->BalloonFontName->TabIndex = 3;
		// 
		// BalloonFontSizeLabel
		// 
		this->BalloonFontSizeLabel->AutoSize = true;
		this->BalloonFontSizeLabel->Location = System::Drawing::Point(238, 37);
		this->BalloonFontSizeLabel->Name = L"BalloonFontSizeLabel";
		this->BalloonFontSizeLabel->Size = System::Drawing::Size(35, 17);
		this->BalloonFontSizeLabel->TabIndex = 4;
		this->BalloonFontSizeLabel->Text = L"Size";
		// 
		// BalloonFontSize
		// 
		this->BalloonFontSize->Enabled = false;
		this->BalloonFontSize->Location = System::Drawing::Point(282, 37);
		this->BalloonFontSize->Name = L"BalloonFontSize";
		this->BalloonFontSize->ReadOnly = true;
		this->BalloonFontSize->Size = System::Drawing::Size(87, 22);
		this->BalloonFontSize->TabIndex = 5;
		// 
		// BalloonFontBold
		// 
		this->BalloonFontBold->AutoCheck = false;
		this->BalloonFontBold->AutoSize = true;
		this->BalloonFontBold->Enabled = false;
		this->BalloonFontBold->Location = System::Drawing::Point(241, 62);
		this->BalloonFontBold->Name = L"BalloonFontBold";
		this->BalloonFontBold->Size = System::Drawing::Size(58, 21);
		this->BalloonFontBold->TabIndex = 6;
		this->BalloonFontBold->Text = L"Bold";
		this->BalloonFontBold->UseVisualStyleBackColor = true;
		// 
		// BalloonFontItalic
		// 
		this->BalloonFontItalic->AutoCheck = false;
		this->BalloonFontItalic->AutoSize = true;
		this->BalloonFontItalic->Enabled = false;
		this->BalloonFontItalic->Location = System::Drawing::Point(305, 62);
		this->BalloonFontItalic->Name = L"BalloonFontItalic";
		this->BalloonFontItalic->Size = System::Drawing::Size(58, 21);
		this->BalloonFontItalic->TabIndex = 7;
		this->BalloonFontItalic->Text = L"Italic";
		this->BalloonFontItalic->UseVisualStyleBackColor = true;
		// 
		// BalloonFontUnderline
		// 
		this->BalloonFontUnderline->AutoCheck = false;
		this->BalloonFontUnderline->AutoSize = true;
		this->BalloonFontUnderline->Enabled = false;
		this->BalloonFontUnderline->Location = System::Drawing::Point(369, 62);
		this->BalloonFontUnderline->Name = L"BalloonFontUnderline";
		this->BalloonFontUnderline->Size = System::Drawing::Size(91, 21);
		this->BalloonFontUnderline->TabIndex = 8;
		this->BalloonFontUnderline->Text = L"Underline";
		this->BalloonFontUnderline->UseVisualStyleBackColor = true;
		// 
		// SoundEffectsEnabled
		// 
		this->SoundEffectsEnabled->AutoCheck = false;
		this->SoundEffectsEnabled->AutoSize = true;
		this->SoundEffectsEnabled->Enabled = false;
		this->SoundEffectsEnabled->Location = System::Drawing::Point(6, 98);
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
		this->TTSEnabled->Enabled = false;
		this->TTSEnabled->Location = System::Drawing::Point(6, 142);
		this->TTSEnabled->Name = L"TTSEnabled";
		this->TTSEnabled->Size = System::Drawing::Size(112, 21);
		this->TTSEnabled->TabIndex = 10;
		this->TTSEnabled->Text = L"TTS enabled";
		this->TTSEnabled->UseVisualStyleBackColor = true;
		// 
		// TTSSpeedLabel
		// 
		this->TTSSpeedLabel->AutoSize = true;
		this->TTSSpeedLabel->Location = System::Drawing::Point(154, 144);
		this->TTSSpeedLabel->Name = L"TTSSpeedLabel";
		this->TTSSpeedLabel->Size = System::Drawing::Size(78, 17);
		this->TTSSpeedLabel->TabIndex = 11;
		this->TTSSpeedLabel->Text = L"TTS speed";
		this->TTSSpeedLabel->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// TTSSpeed
		// 
		this->TTSSpeed->Enabled = false;
		this->TTSSpeed->Location = System::Drawing::Point(244, 144);
		this->TTSSpeed->Name = L"TTSSpeed";
		this->TTSSpeed->ReadOnly = true;
		this->TTSSpeed->Size = System::Drawing::Size(87, 22);
		this->TTSSpeed->TabIndex = 12;
		// 
		// SREnabled
		// 
		this->SREnabled->AutoCheck = false;
		this->SREnabled->AutoSize = true;
		this->SREnabled->Enabled = false;
		this->SREnabled->Location = System::Drawing::Point(6, 186);
		this->SREnabled->Name = L"SREnabled";
		this->SREnabled->Size = System::Drawing::Size(104, 21);
		this->SREnabled->TabIndex = 13;
		this->SREnabled->Text = L"SR enabled";
		this->SREnabled->UseVisualStyleBackColor = true;
		// 
		// SRHotKeyLabel
		// 
		this->SRHotKeyLabel->AutoSize = true;
		this->SRHotKeyLabel->Location = System::Drawing::Point(154, 188);
		this->SRHotKeyLabel->Name = L"SRHotKeyLabel";
		this->SRHotKeyLabel->Size = System::Drawing::Size(56, 17);
		this->SRHotKeyLabel->TabIndex = 14;
		this->SRHotKeyLabel->Text = L"Hot key";
		this->SRHotKeyLabel->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// SRHotKey
		// 
		this->SRHotKey->Enabled = false;
		this->SRHotKey->Location = System::Drawing::Point(244, 188);
		this->SRHotKey->Name = L"SRHotKey";
		this->SRHotKey->ReadOnly = true;
		this->SRHotKey->Size = System::Drawing::Size(87, 22);
		this->SRHotKey->TabIndex = 15;
		// 
		// SRHotKeyTimeLabel
		// 
		this->SRHotKeyTimeLabel->AutoSize = true;
		this->SRHotKeyTimeLabel->Location = System::Drawing::Point(348, 188);
		this->SRHotKeyTimeLabel->Name = L"SRHotKeyTimeLabel";
		this->SRHotKeyTimeLabel->Size = System::Drawing::Size(39, 17);
		this->SRHotKeyTimeLabel->TabIndex = 16;
		this->SRHotKeyTimeLabel->Text = L"Time";
		this->SRHotKeyTimeLabel->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// SRHotKeyTime
		// 
		this->SRHotKeyTime->Enabled = false;
		this->SRHotKeyTime->Location = System::Drawing::Point(406, 188);
		this->SRHotKeyTime->Name = L"SRHotKeyTime";
		this->SRHotKeyTime->ReadOnly = true;
		this->SRHotKeyTime->Size = System::Drawing::Size(87, 22);
		this->SRHotKeyTime->TabIndex = 17;
		// 
		// SRListeningPrompt
		// 
		this->SRListeningPrompt->AutoCheck = false;
		this->SRListeningPrompt->AutoSize = true;
		this->SRListeningPrompt->Enabled = false;
		this->SRListeningPrompt->Location = System::Drawing::Point(157, 213);
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
		this->SRListeningTip->Enabled = false;
		this->SRListeningTip->Location = System::Drawing::Point(157, 240);
		this->SRListeningTip->Name = L"SRListeningTip";
		this->SRListeningTip->Size = System::Drawing::Size(106, 21);
		this->SRListeningTip->TabIndex = 19;
		this->SRListeningTip->Text = L"Listening tip";
		this->SRListeningTip->UseVisualStyleBackColor = true;
		// 
		// TabPropertySheet
		// 
		this->TabPropertySheet->Controls->Add(this->PropertySheetConnected);
		this->TabPropertySheet->Controls->Add(this->PropSheetVisible);
		this->TabPropertySheet->Controls->Add(this->PropSheetPageLabel);
		this->TabPropertySheet->Controls->Add(this->PropSheetPage);
		this->TabPropertySheet->Controls->Add(this->PropSheetLeftLabel);
		this->TabPropertySheet->Controls->Add(this->PropSheetLeft);
		this->TabPropertySheet->Controls->Add(this->PropSheetTopLabel);
		this->TabPropertySheet->Controls->Add(this->PropSheetTop);
		this->TabPropertySheet->Controls->Add(this->PropSheetWidthLabel);
		this->TabPropertySheet->Controls->Add(this->PropSheetWidth);
		this->TabPropertySheet->Controls->Add(this->PropSheetHeightLabel);
		this->TabPropertySheet->Controls->Add(this->PropSheetHeight);
		this->TabPropertySheet->Location = System::Drawing::Point(4, 25);
		this->TabPropertySheet->Name = L"TabPropertySheet";
		this->TabPropertySheet->Padding = System::Windows::Forms::Padding(3);
		this->TabPropertySheet->Size = System::Drawing::Size(646, 500);
		this->TabPropertySheet->TabIndex = 1;
		this->TabPropertySheet->Text = L"Property Sheet";
		this->TabPropertySheet->UseVisualStyleBackColor = true;
		// 
		// PropertySheetConnected
		// 
		this->PropertySheetConnected->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
		this->PropertySheetConnected->Appearance = System::Windows::Forms::Appearance::Button;
		this->PropertySheetConnected->AutoSize = true;
		this->PropertySheetConnected->Location = System::Drawing::Point(554, 6);
		this->PropertySheetConnected->Name = L"PropertySheetConnected";
		this->PropertySheetConnected->Size = System::Drawing::Size(86, 27);
		this->PropertySheetConnected->TabIndex = 0;
		this->PropertySheetConnected->Text = L"Connected";
		this->PropertySheetConnected->UseVisualStyleBackColor = true;
		// 
		// PropSheetVisible
		// 
		this->PropSheetVisible->AutoSize = true;
		this->PropSheetVisible->Enabled = false;
		this->PropSheetVisible->Location = System::Drawing::Point(6, 7);
		this->PropSheetVisible->Name = L"PropSheetVisible";
		this->PropSheetVisible->Size = System::Drawing::Size(71, 21);
		this->PropSheetVisible->TabIndex = 1;
		this->PropSheetVisible->Text = L"Visible";
		this->PropSheetVisible->UseVisualStyleBackColor = true;
		// 
		// PropSheetPageLabel
		// 
		this->PropSheetPageLabel->AutoSize = true;
		this->PropSheetPageLabel->Location = System::Drawing::Point(116, 9);
		this->PropSheetPageLabel->Name = L"PropSheetPageLabel";
		this->PropSheetPageLabel->Size = System::Drawing::Size(41, 17);
		this->PropSheetPageLabel->TabIndex = 2;
		this->PropSheetPageLabel->Text = L"Page";
		this->PropSheetPageLabel->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
		// 
		// PropSheetPage
		// 
		this->PropSheetPage->Location = System::Drawing::Point(163, 6);
		this->PropSheetPage->Name = L"PropSheetPage";
		this->PropSheetPage->Size = System::Drawing::Size(213, 22);
		this->PropSheetPage->TabIndex = 3;
		// 
		// PropSheetLeftLabel
		// 
		this->PropSheetLeftLabel->AutoSize = true;
		this->PropSheetLeftLabel->Location = System::Drawing::Point(45, 102);
		this->PropSheetLeftLabel->Name = L"PropSheetLeftLabel";
		this->PropSheetLeftLabel->Size = System::Drawing::Size(32, 17);
		this->PropSheetLeftLabel->TabIndex = 4;
		this->PropSheetLeftLabel->Text = L"Left";
		this->PropSheetLeftLabel->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
		// 
		// PropSheetLeft
		// 
		this->PropSheetLeft->Location = System::Drawing::Point(83, 99);
		this->PropSheetLeft->Name = L"PropSheetLeft";
		this->PropSheetLeft->Size = System::Drawing::Size(80, 22);
		this->PropSheetLeft->TabIndex = 5;
		// 
		// PropSheetTopLabel
		// 
		this->PropSheetTopLabel->AutoSize = true;
		this->PropSheetTopLabel->Location = System::Drawing::Point(124, 57);
		this->PropSheetTopLabel->Name = L"PropSheetTopLabel";
		this->PropSheetTopLabel->Size = System::Drawing::Size(33, 17);
		this->PropSheetTopLabel->TabIndex = 6;
		this->PropSheetTopLabel->Text = L"Top";
		this->PropSheetTopLabel->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
		// 
		// PropSheetTop
		// 
		this->PropSheetTop->Location = System::Drawing::Point(163, 54);
		this->PropSheetTop->Name = L"PropSheetTop";
		this->PropSheetTop->Size = System::Drawing::Size(80, 22);
		this->PropSheetTop->TabIndex = 7;
		// 
		// PropSheetWidthLabel
		// 
		this->PropSheetWidthLabel->AutoSize = true;
		this->PropSheetWidthLabel->Location = System::Drawing::Point(113, 151);
		this->PropSheetWidthLabel->Name = L"PropSheetWidthLabel";
		this->PropSheetWidthLabel->Size = System::Drawing::Size(44, 17);
		this->PropSheetWidthLabel->TabIndex = 8;
		this->PropSheetWidthLabel->Text = L"Width";
		this->PropSheetWidthLabel->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
		// 
		// PropSheetWidth
		// 
		this->PropSheetWidth->Location = System::Drawing::Point(163, 148);
		this->PropSheetWidth->Name = L"PropSheetWidth";
		this->PropSheetWidth->ReadOnly = true;
		this->PropSheetWidth->Size = System::Drawing::Size(80, 22);
		this->PropSheetWidth->TabIndex = 9;
		// 
		// PropSheetHeightLabel
		// 
		this->PropSheetHeightLabel->AutoSize = true;
		this->PropSheetHeightLabel->Location = System::Drawing::Point(188, 102);
		this->PropSheetHeightLabel->Name = L"PropSheetHeightLabel";
		this->PropSheetHeightLabel->Size = System::Drawing::Size(49, 17);
		this->PropSheetHeightLabel->TabIndex = 10;
		this->PropSheetHeightLabel->Text = L"Height";
		this->PropSheetHeightLabel->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
		// 
		// PropSheetHeight
		// 
		this->PropSheetHeight->Location = System::Drawing::Point(243, 99);
		this->PropSheetHeight->Name = L"PropSheetHeight";
		this->PropSheetHeight->ReadOnly = true;
		this->PropSheetHeight->Size = System::Drawing::Size(80, 22);
		this->PropSheetHeight->TabIndex = 11;
		// 
		// TabCharacterFiles
		// 
		this->TabCharacterFiles->Controls->Add(this->FilesConnected);
		this->TabCharacterFiles->Controls->Add(this->ShowDaFiles);
		this->TabCharacterFiles->Controls->Add(this->ShowMaFiles);
		this->TabCharacterFiles->Controls->Add(this->ShowOfficeFiles);
		this->TabCharacterFiles->Controls->Add(this->ShowSpeakingFiles);
		this->TabCharacterFiles->Controls->Add(this->ShowSilentFiles);
		this->TabCharacterFiles->Controls->Add(this->VerifyFileVersion);
		this->TabCharacterFiles->Controls->Add(this->DefaultFileLabel);
		this->TabCharacterFiles->Controls->Add(this->DefaultFilePath);
		this->TabCharacterFiles->Controls->Add(this->DefaultSearchLabel);
		this->TabCharacterFiles->Controls->Add(this->DefaultSearchPath);
		this->TabCharacterFiles->Controls->Add(this->CurrentSearchLabel);
		this->TabCharacterFiles->Controls->Add(this->CurrentSearchPath);
		this->TabCharacterFiles->Controls->Add(this->FilesListBox);
		this->TabCharacterFiles->Location = System::Drawing::Point(4, 25);
		this->TabCharacterFiles->Name = L"TabCharacterFiles";
		this->TabCharacterFiles->Padding = System::Windows::Forms::Padding(3);
		this->TabCharacterFiles->Size = System::Drawing::Size(646, 500);
		this->TabCharacterFiles->TabIndex = 2;
		this->TabCharacterFiles->Text = L"Files";
		this->TabCharacterFiles->UseVisualStyleBackColor = true;
		// 
		// FilesConnected
		// 
		this->FilesConnected->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
		this->FilesConnected->Appearance = System::Windows::Forms::Appearance::Button;
		this->FilesConnected->AutoSize = true;
		this->FilesConnected->Location = System::Drawing::Point(554, 6);
		this->FilesConnected->Name = L"FilesConnected";
		this->FilesConnected->Size = System::Drawing::Size(86, 27);
		this->FilesConnected->TabIndex = 0;
		this->FilesConnected->Text = L"Connected";
		this->FilesConnected->UseVisualStyleBackColor = true;
		// 
		// ShowDaFiles
		// 
		this->ShowDaFiles->AutoSize = true;
		this->ShowDaFiles->Location = System::Drawing::Point(6, 6);
		this->ShowDaFiles->Name = L"ShowDaFiles";
		this->ShowDaFiles->Size = System::Drawing::Size(149, 21);
		this->ShowDaFiles->TabIndex = 1;
		this->ShowDaFiles->Text = L"Double Agent Files";
		this->ShowDaFiles->UseVisualStyleBackColor = true;
		// 
		// ShowMaFiles
		// 
		this->ShowMaFiles->AutoSize = true;
		this->ShowMaFiles->Location = System::Drawing::Point(161, 6);
		this->ShowMaFiles->Name = L"ShowMaFiles";
		this->ShowMaFiles->Size = System::Drawing::Size(124, 21);
		this->ShowMaFiles->TabIndex = 2;
		this->ShowMaFiles->Text = L"MS Agent Files";
		this->ShowMaFiles->UseVisualStyleBackColor = true;
		// 
		// ShowOfficeFiles
		// 
		this->ShowOfficeFiles->AutoSize = true;
		this->ShowOfficeFiles->Location = System::Drawing::Point(291, 6);
		this->ShowOfficeFiles->Name = L"ShowOfficeFiles";
		this->ShowOfficeFiles->Size = System::Drawing::Size(124, 21);
		this->ShowOfficeFiles->TabIndex = 3;
		this->ShowOfficeFiles->Text = L"MS Office Files";
		this->ShowOfficeFiles->UseVisualStyleBackColor = true;
		// 
		// ShowSpeakingFiles
		// 
		this->ShowSpeakingFiles->AutoSize = true;
		this->ShowSpeakingFiles->Location = System::Drawing::Point(6, 33);
		this->ShowSpeakingFiles->Name = L"ShowSpeakingFiles";
		this->ShowSpeakingFiles->Size = System::Drawing::Size(162, 21);
		this->ShowSpeakingFiles->TabIndex = 4;
		this->ShowSpeakingFiles->Text = L"Speaking Characters";
		this->ShowSpeakingFiles->UseVisualStyleBackColor = true;
		// 
		// ShowSilentFiles
		// 
		this->ShowSilentFiles->AutoSize = true;
		this->ShowSilentFiles->Location = System::Drawing::Point(174, 33);
		this->ShowSilentFiles->Name = L"ShowSilentFiles";
		this->ShowSilentFiles->Size = System::Drawing::Size(191, 21);
		this->ShowSilentFiles->TabIndex = 5;
		this->ShowSilentFiles->Text = L"Non-speaking Characters";
		this->ShowSilentFiles->UseVisualStyleBackColor = true;
		// 
		// VerifyFileVersion
		// 
		this->VerifyFileVersion->AutoSize = true;
		this->VerifyFileVersion->Location = System::Drawing::Point(371, 33);
		this->VerifyFileVersion->Name = L"VerifyFileVersion";
		this->VerifyFileVersion->Size = System::Drawing::Size(118, 21);
		this->VerifyFileVersion->TabIndex = 6;
		this->VerifyFileVersion->Text = L"Verify Version";
		this->VerifyFileVersion->UseVisualStyleBackColor = true;
		// 
		// DefaultFileLabel
		// 
		this->DefaultFileLabel->AutoSize = true;
		this->DefaultFileLabel->Location = System::Drawing::Point(3, 63);
		this->DefaultFileLabel->Name = L"DefaultFileLabel";
		this->DefaultFileLabel->Size = System::Drawing::Size(119, 17);
		this->DefaultFileLabel->TabIndex = 7;
		this->DefaultFileLabel->Text = L"Default Character";
		// 
		// DefaultFilePath
		// 
		this->DefaultFilePath->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
			| System::Windows::Forms::AnchorStyles::Right));
		this->DefaultFilePath->Location = System::Drawing::Point(143, 60);
		this->DefaultFilePath->Name = L"DefaultFilePath";
		this->DefaultFilePath->ReadOnly = true;
		this->DefaultFilePath->Size = System::Drawing::Size(495, 22);
		this->DefaultFilePath->TabIndex = 8;
		// 
		// DefaultSearchLabel
		// 
		this->DefaultSearchLabel->AutoSize = true;
		this->DefaultSearchLabel->Location = System::Drawing::Point(3, 95);
		this->DefaultSearchLabel->Name = L"DefaultSearchLabel";
		this->DefaultSearchLabel->Size = System::Drawing::Size(102, 17);
		this->DefaultSearchLabel->TabIndex = 9;
		this->DefaultSearchLabel->Text = L"Default Search";
		// 
		// DefaultSearchPath
		// 
		this->DefaultSearchPath->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
			| System::Windows::Forms::AnchorStyles::Right));
		this->DefaultSearchPath->Location = System::Drawing::Point(142, 92);
		this->DefaultSearchPath->Name = L"DefaultSearchPath";
		this->DefaultSearchPath->ReadOnly = true;
		this->DefaultSearchPath->Size = System::Drawing::Size(496, 22);
		this->DefaultSearchPath->TabIndex = 10;
		// 
		// CurrentSearchLabel
		// 
		this->CurrentSearchLabel->AutoSize = true;
		this->CurrentSearchLabel->Location = System::Drawing::Point(3, 125);
		this->CurrentSearchLabel->Name = L"CurrentSearchLabel";
		this->CurrentSearchLabel->Size = System::Drawing::Size(104, 17);
		this->CurrentSearchLabel->TabIndex = 11;
		this->CurrentSearchLabel->Text = L"Current Search";
		// 
		// CurrentSearchPath
		// 
		this->CurrentSearchPath->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
			| System::Windows::Forms::AnchorStyles::Right));
		this->CurrentSearchPath->Location = System::Drawing::Point(143, 122);
		this->CurrentSearchPath->Name = L"CurrentSearchPath";
		this->CurrentSearchPath->ReadOnly = true;
		this->CurrentSearchPath->Size = System::Drawing::Size(495, 22);
		this->CurrentSearchPath->TabIndex = 12;
		// 
		// FilesListBox
		// 
		this->FilesListBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
			| System::Windows::Forms::AnchorStyles::Left) 
			| System::Windows::Forms::AnchorStyles::Right));
		this->FilesListBox->FormattingEnabled = true;
		this->FilesListBox->ItemHeight = 16;
		this->FilesListBox->Location = System::Drawing::Point(6, 154);
		this->FilesListBox->Name = L"FilesListBox";
		this->FilesListBox->Size = System::Drawing::Size(632, 340);
		this->FilesListBox->TabIndex = 13;
		// 
		// TabTTSEngines
		// 
		this->TabTTSEngines->Controls->Add(this->TTSEnginesConnected);
		this->TabTTSEngines->Controls->Add(this->TTSEngineList);
		this->TabTTSEngines->Location = System::Drawing::Point(4, 25);
		this->TabTTSEngines->Name = L"TabTTSEngines";
		this->TabTTSEngines->Padding = System::Windows::Forms::Padding(3);
		this->TabTTSEngines->Size = System::Drawing::Size(646, 500);
		this->TabTTSEngines->TabIndex = 3;
		this->TabTTSEngines->Text = L"TTS Engines";
		this->TabTTSEngines->UseVisualStyleBackColor = true;
		// 
		// TTSEnginesConnected
		// 
		this->TTSEnginesConnected->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
		this->TTSEnginesConnected->Appearance = System::Windows::Forms::Appearance::Button;
		this->TTSEnginesConnected->AutoSize = true;
		this->TTSEnginesConnected->Location = System::Drawing::Point(554, 6);
		this->TTSEnginesConnected->Name = L"TTSEnginesConnected";
		this->TTSEnginesConnected->Size = System::Drawing::Size(86, 27);
		this->TTSEnginesConnected->TabIndex = 0;
		this->TTSEnginesConnected->Text = L"Connected";
		this->TTSEnginesConnected->UseVisualStyleBackColor = true;
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
		this->TTSEngineList->Location = System::Drawing::Point(6, 39);
		this->TTSEngineList->MultiSelect = false;
		this->TTSEngineList->Name = L"TTSEngineList";
		this->TTSEngineList->ReadOnly = true;
		this->TTSEngineList->RowHeadersVisible = false;
		this->TTSEngineList->RowTemplate->Height = 24;
		this->TTSEngineList->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
		this->TTSEngineList->Size = System::Drawing::Size(634, 455);
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
		// TabSREngines
		// 
		this->TabSREngines->Controls->Add(this->SREnginesConnected);
		this->TabSREngines->Controls->Add(this->SREngineList);
		this->TabSREngines->Location = System::Drawing::Point(4, 25);
		this->TabSREngines->Name = L"TabSREngines";
		this->TabSREngines->Padding = System::Windows::Forms::Padding(3);
		this->TabSREngines->Size = System::Drawing::Size(646, 500);
		this->TabSREngines->TabIndex = 4;
		this->TabSREngines->Text = L"SR Engines";
		this->TabSREngines->UseVisualStyleBackColor = true;
		// 
		// SREnginesConnected
		// 
		this->SREnginesConnected->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
		this->SREnginesConnected->Appearance = System::Windows::Forms::Appearance::Button;
		this->SREnginesConnected->AutoSize = true;
		this->SREnginesConnected->Location = System::Drawing::Point(554, 6);
		this->SREnginesConnected->Name = L"SREnginesConnected";
		this->SREnginesConnected->Size = System::Drawing::Size(86, 27);
		this->SREnginesConnected->TabIndex = 0;
		this->SREnginesConnected->Text = L"Connected";
		this->SREnginesConnected->UseVisualStyleBackColor = true;
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
		this->SREngineList->Location = System::Drawing::Point(6, 39);
		this->SREngineList->MultiSelect = false;
		this->SREngineList->Name = L"SREngineList";
		this->SREngineList->ReadOnly = true;
		this->SREngineList->RowHeadersVisible = false;
		this->SREngineList->RowTemplate->Height = 24;
		this->SREngineList->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
		this->SREngineList->Size = System::Drawing::Size(634, 455);
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
		// ControlTestForm
		// 
		this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
		this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		this->ClientSize = System::Drawing::Size(895, 553);
		this->Controls->Add(this->LoadButton);
		this->Controls->Add(this->TestDaControl);
		this->Controls->Add(this->RaiseRequestErrorsButton);
		this->Controls->Add(this->AutoConnectButton);
		this->Controls->Add(this->ControlConnectedButton);
		this->Controls->Add(this->TestTabControl);
		this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
		this->Name = L"ControlTestForm";
		this->Text = L"Control Test";
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TestDaControl))->EndInit();
		this->TestTabControl->ResumeLayout(false);
		this->TabCharacters->ResumeLayout(false);
		this->TabCharacters->PerformLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CharacterList))->EndInit();
		this->TabSettings->ResumeLayout(false);
		this->TabSettings->PerformLayout();
		this->TabPropertySheet->ResumeLayout(false);
		this->TabPropertySheet->PerformLayout();
		this->TabCharacterFiles->ResumeLayout(false);
		this->TabCharacterFiles->PerformLayout();
		this->TabTTSEngines->ResumeLayout(false);
		this->TabTTSEngines->PerformLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TTSEngineList))->EndInit();
		this->TabSREngines->ResumeLayout(false);
		this->TabSREngines->PerformLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->SREngineList))->EndInit();
		this->ResumeLayout(false);
		this->PerformLayout();

	}
#pragma endregion

//===========================================================================

private: 
	DoubleAgent::Control::Characters^		mCharacters;
	DoubleAgent::Control::Settings^			mSettings;
	DoubleAgent::Control::PropertySheet^	mPropertySheet;
	DoubleAgent::Control::TTSEngines^		mTTSEngines;
	DoubleAgent::Control::SREngines^		mSREngines;
	DoubleAgent::Control::CharacterFiles^	mCharacterFiles;

//---------------------------------------------------------------------------

public:
	property System::Boolean Characters_Connected
	{
		System::Boolean get ()
		{
			return !!mCharacters;
		}
		System::Void set (System::Boolean value)
		{
			if	(!value)
			{
				mCharacters = nullptr;
			}
			else
			if	(!mCharacters)
			{
				mCharacters = TestDaControl->Characters;
			}
			ShowConnected ();
		}
	} 
	property Generic::List <DoubleAgent::Control::Character^>^ Characters_List
	{
		Generic::List <DoubleAgent::Control::Character^>^ get ()
		{
			Generic::List <DoubleAgent::Control::Character^>^	lList = gcnew  Generic::List <DoubleAgent::Control::Character^>;
			if	(
					(mCharacters)
				&&	(mCharacters->Count > 0)
				)
			{
				try
				{
					for	(int lNdx = 0; lNdx < mCharacters->Count; lNdx++)
					{
						lList->Add (safe_cast <DoubleAgent::Control::Character^> (mCharacters->Index [lNdx]));
					}
				}
				catch (...)
				{}
			}
			return lList;
		}
	}

//---------------------------------------------------------------------------

public:
	property System::Boolean Settings_Connected
	{
		System::Boolean get ()
		{
			return !!mSettings;
		}
		System::Void set (System::Boolean value)
		{
			if	(!value)
			{
				mSettings = nullptr;
			}
			else
			if	(!mSettings)
			{
				mSettings = TestDaControl->Settings;
			}
			ShowConnected ();
		}
	} 
	property System::Boolean Settings_BalloonEnabled
	{
		System::Boolean get ()
		{
			return mSettings ? mSettings->BalloonEnabled : false;
		}
	} 
	property System::String^ Settings_BalloonFontName
	{
		System::String^ get ()
		{
			return mSettings ? mSettings->BalloonFont ? safe_cast <stdole::StdFontClass^> (mSettings->BalloonFont)->Name : "<none>" : String::Empty;
		}
	} 
	property System::Decimal Settings_BalloonFontSize
	{
		System::Decimal get ()
		{
			return (mSettings && mSettings->BalloonFont) ? safe_cast <stdole::StdFontClass^> (mSettings->BalloonFont)->Size : 0;
		}
	} 
	property System::Boolean Settings_BalloonFontBold
	{
		System::Boolean get ()
		{
			return (mSettings && mSettings->BalloonFont) ? safe_cast <stdole::StdFontClass^> (mSettings->BalloonFont)->Bold : false;
		}
	} 
	property System::Boolean Settings_BalloonFontItalic
	{
		System::Boolean get ()
		{
			return (mSettings && mSettings->BalloonFont) ? safe_cast <stdole::StdFontClass^> (mSettings->BalloonFont)->Italic : false;
		}
	} 
	property System::Boolean Settings_BalloonFontUnderline
	{
		System::Boolean get ()
		{
			return (mSettings && mSettings->BalloonFont) ? safe_cast <stdole::StdFontClass^> (mSettings->BalloonFont)->Underline : false;
		}
	} 
	property System::Boolean Settings_SoundEffectsEnabled
	{
		System::Boolean get ()
		{
			return mSettings ? mSettings->SoundEffectsEnabled : false;
		}
	} 
	property System::Boolean Settings_TTSEnabled
	{
		System::Boolean get ()
		{
			return mSettings ? mSettings->TTSEnabled : false;
		}
	} 
	property System::Int16 Settings_TTSSpeed
	{
		System::Int16 get ()
		{
			return mSettings ? mSettings->TTSSpeed : 0;
		}
	} 
	property System::Boolean Settings_SREnabled
	{
		System::Boolean get ()
		{
			return mSettings ? mSettings->SREnabled : false;
		}
	} 
	property System::String^ Settings_SRHotKey
	{
		System::String^ get ()
		{
			return mSettings ? mSettings->SRHotKey : String::Empty;
		}
	} 
	property System::Int16 Settings_SRHotKeyTime
	{
		System::Int16 get ()
		{
			return mSettings ? mSettings->SRHotKeyTime : 0;
		}
	} 
	property System::Boolean Settings_SRListeningPrompt
	{
		System::Boolean get ()
		{
			return mSettings ? mSettings->SRListeningPrompt : false;
		}
	} 
	property System::Boolean Settings_SRListeningTip
	{
		System::Boolean get ()
		{
			return mSettings ? mSettings->SRListeningTip : false;
		}
	} 

//---------------------------------------------------------------------------

public:
	property System::Boolean PropertySheet_Connected
	{
		System::Boolean get ()
		{
			return !!mPropertySheet;
		}
		System::Void set (System::Boolean value)
		{
			if	(!value)
			{
				mPropertySheet = nullptr;
			}
			else
			if	(!mPropertySheet)
			{
				mPropertySheet = TestDaControl->PropertySheet;
			}
			ShowConnected ();
		}
	} 
	property System::Boolean PropertySheet_Visible
	{
		System::Boolean get ()
		{
			return mPropertySheet ? mPropertySheet->Visible : 0;
		}
		void set (System::Boolean value)
		{
			if (mPropertySheet) mPropertySheet->Visible = value;
		}
	} 
	property System::Int16 PropertySheet_Left
	{
		System::Int16 get ()
		{
			return mPropertySheet ? mPropertySheet->Left : 0;
		}
		void set (System::Int16 value)
		{
			if (mPropertySheet) mPropertySheet->Left = value;
		}
	} 
	property System::Int16 PropertySheet_Top
	{
		System::Int16 get ()
		{
			return mPropertySheet ? mPropertySheet->Top : 0;
		}
		void set (System::Int16 value)
		{
			if (mPropertySheet) mPropertySheet->Top = value;
		}
	} 
	property System::Int16 PropertySheet_Width
	{
		System::Int16 get ()
		{
			return mPropertySheet ? mPropertySheet->Width : 0;
		}
	} 
	property System::Int16 PropertySheet_Height
	{
		System::Int16 get ()
		{
			return mPropertySheet ? mPropertySheet->Height : 0;
		}
	} 
	property System::String^ PropertySheet_Page
	{
		System::String^ get ()
		{
			return mPropertySheet ? mPropertySheet->Page : String::Empty;
		}
		void set (System::String^ value)
		{
			if (mPropertySheet) mPropertySheet->Page = value;
		}
	} 

//---------------------------------------------------------------------------

public:
	property System::Boolean CharacterFiles_Connected
	{
		System::Boolean get ()
		{
			return !!mCharacterFiles;
		}
		System::Void set (System::Boolean value)
		{
			if	(!value)
			{
				mCharacterFiles = nullptr;
			}
			else
			if	(!mCharacterFiles)
			{
				mCharacterFiles = TestDaControl->CharacterFiles;
			}
			ShowConnected ();
		}
	} 
	property System::Boolean CharacterFiles_ShowDaFiles
	{
		System::Boolean get ()
		{
			return mCharacterFiles ? mCharacterFiles->DoubleAgentFiles : false;
		}
		System::Void set (System::Boolean value)
		{
			if	(mCharacterFiles)
			{
				mCharacterFiles->DoubleAgentFiles = value;
			}
		}
	} 
	property System::Boolean CharacterFiles_ShowMaFiles
	{
		System::Boolean get ()
		{
			return mCharacterFiles ? mCharacterFiles->MsAgentFiles : false;
		}
		System::Void set (System::Boolean value)
		{
			if	(mCharacterFiles)
			{
				mCharacterFiles->MsAgentFiles = value;
			}
		}
	} 
	property System::Boolean CharacterFiles_ShowOfficeFiles
	{
		System::Boolean get ()
		{
			return mCharacterFiles ? mCharacterFiles->MsOfficeFiles : false;
		}
		System::Void set (System::Boolean value)
		{
			if	(mCharacterFiles)
			{
				mCharacterFiles->MsOfficeFiles = value;
			}
		}
	} 
	property System::Boolean CharacterFiles_ShowSpeakingFiles
	{
		System::Boolean get ()
		{
			return mCharacterFiles ? mCharacterFiles->SpeakingCharacters : false;
		}
		System::Void set (System::Boolean value)
		{
			if	(mCharacterFiles)
			{
				mCharacterFiles->SpeakingCharacters = value;
			}
		}
	} 
	property System::Boolean CharacterFiles_ShowSilentFiles
	{
		System::Boolean get ()
		{
			return mCharacterFiles ? mCharacterFiles->NonSpeakingCharacters : false;
		}
		System::Void set (System::Boolean value)
		{
			if	(mCharacterFiles)
			{
				mCharacterFiles->NonSpeakingCharacters = value;
			}
		}
	} 
	property System::Boolean CharacterFiles_VerifyFileVersion
	{
		System::Boolean get ()
		{
			return mCharacterFiles ? mCharacterFiles->VerifyVersion : false;
		}
		System::Void set (System::Boolean value)
		{
			if	(mCharacterFiles)
			{
				mCharacterFiles->VerifyVersion = value;
			}
		}
	} 
	property System::String^ CharacterFiles_DefaultFilePath
	{
		System::String^ get ()
		{
			return mCharacterFiles ? mCharacterFiles->DefaultFilePath : System::String::Empty;
		}
	} 
	property System::String^ CharacterFiles_DefaultSearchPath
	{
		System::String^ get ()
		{
			return mCharacterFiles ? mCharacterFiles->DefaultSearchPath : System::String::Empty;
		}
	} 
	property System::String^ CharacterFiles_CurrentSearchPath
	{
		System::String^ get ()
		{
			return mCharacterFiles ? mCharacterFiles->SearchPath : System::String::Empty;
		}
		System::Void set (System::String^ value)
		{
			if	(mCharacterFiles) mCharacterFiles->SearchPath = value;
		}
	} 
	property cli::array<String^>^ CharacterFiles_FilePaths
	{
		cli::array<String^>^ get ()
		{
			return mCharacterFiles ? mCharacterFiles->FilePaths : gcnew cli::array<String^>(0);
		}
	} 

//---------------------------------------------------------------------------

public:
	property System::Boolean TTSEngines_Connected
	{
		System::Boolean get ()
		{
			return !!mTTSEngines;
		}
		System::Void set (System::Boolean value)
		{
			if	(!value)
			{
				mTTSEngines = nullptr;
			}
			else
			if	(!mTTSEngines)
			{
				mTTSEngines = TestDaControl->TTSEngines;
			}
			ShowConnected ();
		}
	} 
	property Generic::List <DoubleAgent::Control::TTSEngine^>^ TTSEngines_List
	{
		Generic::List <DoubleAgent::Control::TTSEngine^>^ get ()
		{
			Generic::List <DoubleAgent::Control::TTSEngine^>^	lList = gcnew  Generic::List <DoubleAgent::Control::TTSEngine^>;
			if	(
					(mTTSEngines)
				&&	(mTTSEngines->Count > 0)
				)
			{
				try
				{
					for	(int lNdx = 0; lNdx < mTTSEngines->Count; lNdx++)
					{
						lList->Add (mTTSEngines [lNdx]);
					}
				}
				catch (...)
				{}
			}
			return lList;
		}
	}

//---------------------------------------------------------------------------

public:
	property System::Boolean SREngines_Connected
	{
		System::Boolean get ()
		{
			return !!mSREngines;
		}
		System::Void set (System::Boolean value)
		{
			if	(!value)
			{
				mSREngines = nullptr;
			}
			else
			if	(!mSREngines)
			{
				mSREngines = TestDaControl->SREngines;
			}
			ShowConnected ();
		}
	} 
	property Generic::List <DoubleAgent::Control::SREngine^>^ SREngines_List
	{
		Generic::List <DoubleAgent::Control::SREngine^>^ get ()
		{
			Generic::List <DoubleAgent::Control::SREngine^>^	lList = gcnew  Generic::List <DoubleAgent::Control::SREngine^>;
			if	(
					(mSREngines)
				&&	(mSREngines->Count > 0)
				)
			{
				try
				{
					for	(int lNdx = 0; lNdx < mSREngines->Count; lNdx++)
					{
						lList->Add (mSREngines [lNdx]);
					}
				}
				catch (...)
				{}
			}
			return lList;
		}
	}

//---------------------------------------------------------------------------

private: System::Void ShowConnected ();
private: System::Void LoadButton_Click(System::Object^  sender, System::EventArgs^  e);
};
/////////////////////////////////////////////////////////////////////////////
}	// namespace DoubleAgent
