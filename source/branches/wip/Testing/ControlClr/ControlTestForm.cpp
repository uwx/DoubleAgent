#include "StdAfx.h"
#include "ControlTestForm.h"
#include "LoadCharacter.h"

using namespace DoubleAgent;
using namespace DoubleAgent::Control;

/////////////////////////////////////////////////////////////////////////////

ControlTestForm::ControlTestForm ()
{
	InitializeComponent();

	RaiseRequestErrorsButton->DataBindings->Add ("Checked", TestDaControl, "RaiseRequestErrors", true, DataSourceUpdateMode::OnPropertyChanged);
	AutoConnectButton->DataBindings->Add ("Checked", TestDaControl, "AutoConnect", true, DataSourceUpdateMode::OnPropertyChanged);
	ControlConnectedButton->DataBindings->Add ("Checked", TestDaControl, "Connected", true, DataSourceUpdateMode::OnPropertyChanged);

// 
// TestTabControl
// 
	CharactersConnected->DataBindings->Add ("Checked", this, "Characters_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	SettingsConnected->DataBindings->Add ("Checked", this, "Settings_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	PropertySheetConnected->DataBindings->Add ("Checked", this, "PropertySheet_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	TTSEnginesConnected->DataBindings->Add ("Checked", this, "TTSEngines_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	SREnginesConnected->DataBindings->Add ("Checked", this, "SREngines_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	FilesConnected->DataBindings->Add ("Checked", this, "CharacterFiles_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
// 
// TabCharacters
// 
	CharacterList->AutoGenerateColumns = false;
	CharacterList->DataBindings->Add ("Enabled", this, "Characters_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	CharacterList->DataBindings->Add ("DataSource", this, "Characters_List", true, DataSourceUpdateMode::OnValidation);
// 
// TabPropertySheet
// 
	PropSheetVisible->DataBindings->Add ("Enabled", this, "PropertySheet_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	PropSheetPageLabel->DataBindings->Add ("Enabled", this, "PropertySheet_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	PropSheetPage->DataBindings->Add ("Enabled", this, "PropertySheet_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	PropSheetLeftLabel->DataBindings->Add ("Enabled", this, "PropertySheet_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	PropSheetLeft->DataBindings->Add ("Enabled", this, "PropertySheet_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	PropSheetTopLabel->DataBindings->Add ("Enabled", this, "PropertySheet_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	PropSheetTop->DataBindings->Add ("Enabled", this, "PropertySheet_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	PropSheetWidthLabel->DataBindings->Add ("Enabled", this, "PropertySheet_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	PropSheetWidth->DataBindings->Add ("Enabled", this, "PropertySheet_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	PropSheetHeightLabel->DataBindings->Add ("Enabled", this, "PropertySheet_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	PropSheetHeight->DataBindings->Add ("Enabled", this, "PropertySheet_Connected", true, DataSourceUpdateMode::OnPropertyChanged);

	PropSheetVisible->DataBindings->Add ("Checked", this, "PropertySheet_Visible", true, DataSourceUpdateMode::OnPropertyChanged);
	PropSheetPage->DataBindings->Add ("Text", this, "PropertySheet_Page", true, DataSourceUpdateMode::OnValidation);
	PropSheetLeft->DataBindings->Add ("Text", this, "PropertySheet_Left", true, DataSourceUpdateMode::OnValidation);
	PropSheetTop->DataBindings->Add ("Text", this, "PropertySheet_Top", true, DataSourceUpdateMode::OnValidation);
	PropSheetWidth->DataBindings->Add ("Text", this, "PropertySheet_Width", true, DataSourceUpdateMode::OnPropertyChanged);
	PropSheetHeight->DataBindings->Add ("Text", this, "PropertySheet_Height", true, DataSourceUpdateMode::OnPropertyChanged);
// 
// TabSettings
// 
	BalloonEnabled->DataBindings->Add ("Enabled", this, "Settings_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	BalloonFontLabel->DataBindings->Add ("Enabled", this, "Settings_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	BalloonFontName->DataBindings->Add ("Enabled", this, "Settings_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	BalloonFontSize->DataBindings->Add ("Enabled", this, "Settings_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	BalloonFontBold->DataBindings->Add ("Enabled", this, "Settings_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	BalloonFontItalic->DataBindings->Add ("Enabled", this, "Settings_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	BalloonFontUnderline->DataBindings->Add ("Enabled", this, "Settings_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	SoundEffectsEnabled->DataBindings->Add ("Enabled", this, "Settings_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	TTSEnabled->DataBindings->Add ("Enabled", this, "Settings_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	TTSSpeedLabel->DataBindings->Add ("Enabled", this, "Settings_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	TTSSpeed->DataBindings->Add ("Enabled", this, "Settings_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	SREnabled->DataBindings->Add ("Enabled", this, "Settings_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	SRHotKeyLabel->DataBindings->Add ("Enabled", this, "Settings_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	SRHotKey->DataBindings->Add ("Enabled", this, "Settings_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	SRHotKeyTimeLabel->DataBindings->Add ("Enabled", this, "Settings_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	SRHotKeyTime->DataBindings->Add ("Enabled", this, "Settings_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	SRListeningPrompt->DataBindings->Add ("Enabled", this, "Settings_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	SRListeningTip->DataBindings->Add ("Enabled", this, "Settings_Connected", true, DataSourceUpdateMode::OnPropertyChanged);

	BalloonEnabled->DataBindings->Add ("Checked", this, "Settings_BalloonEnabled", true, DataSourceUpdateMode::OnPropertyChanged);
	BalloonFontName->DataBindings->Add ("Text", this, "Settings_BalloonFontName", true, DataSourceUpdateMode::OnValidation);
	BalloonFontSize->DataBindings->Add ("Text", this, "Settings_BalloonFontSize", true, DataSourceUpdateMode::OnValidation);
	BalloonFontBold->DataBindings->Add ("Checked", this, "Settings_BalloonFontBold", true, DataSourceUpdateMode::OnPropertyChanged);
	BalloonFontItalic->DataBindings->Add ("Checked", this, "Settings_BalloonFontItalic", true, DataSourceUpdateMode::OnPropertyChanged);
	BalloonFontUnderline->DataBindings->Add ("Checked", this, "Settings_BalloonFontUnderline", true, DataSourceUpdateMode::OnPropertyChanged);
	SoundEffectsEnabled->DataBindings->Add ("Checked", this, "Settings_SoundEffectsEnabled", true, DataSourceUpdateMode::OnPropertyChanged);
	TTSEnabled->DataBindings->Add ("Checked", this, "Settings_TTSEnabled", true, DataSourceUpdateMode::OnPropertyChanged);
	TTSSpeed->DataBindings->Add ("Text", this, "Settings_TTSSpeed", true, DataSourceUpdateMode::OnValidation);
	SREnabled->DataBindings->Add ("Checked", this, "Settings_SREnabled", true, DataSourceUpdateMode::OnPropertyChanged);
	SRHotKey->DataBindings->Add ("Text", this, "Settings_SRHotKey", true, DataSourceUpdateMode::OnValidation);
	SRHotKeyTime->DataBindings->Add ("Text", this, "Settings_SRHotKeyTime", true, DataSourceUpdateMode::OnValidation);
	SRListeningPrompt->DataBindings->Add ("Checked", this, "Settings_SRListeningPrompt", true, DataSourceUpdateMode::OnPropertyChanged);
	SRListeningTip->DataBindings->Add ("Checked", this, "Settings_SRListeningTip", true, DataSourceUpdateMode::OnPropertyChanged);
// 
// TabCharacterFiles
// 
	ShowDaFiles->DataBindings->Add ("Enabled", this, "CharacterFiles_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	ShowMaFiles->DataBindings->Add ("Enabled", this, "CharacterFiles_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	ShowOfficeFiles->DataBindings->Add ("Enabled", this, "CharacterFiles_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	ShowSpeakingFiles->DataBindings->Add ("Enabled", this, "CharacterFiles_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	ShowSilentFiles->DataBindings->Add ("Enabled", this, "CharacterFiles_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	VerifyFileVersion->DataBindings->Add ("Enabled", this, "CharacterFiles_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	DefaultFileLabel->DataBindings->Add ("Enabled", this, "CharacterFiles_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	DefaultFilePath->DataBindings->Add ("Enabled", this, "CharacterFiles_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	DefaultSearchLabel->DataBindings->Add ("Enabled", this, "CharacterFiles_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	DefaultSearchPath->DataBindings->Add ("Enabled", this, "CharacterFiles_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	CurrentSearchLabel->DataBindings->Add ("Enabled", this, "CharacterFiles_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	CurrentSearchPath->DataBindings->Add ("Enabled", this, "CharacterFiles_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	FilesListBox->DataBindings->Add ("Enabled", this, "CharacterFiles_Connected", true, DataSourceUpdateMode::OnPropertyChanged);

	ShowDaFiles->DataBindings->Add ("Checked", this, "CharacterFiles_ShowDaFiles", true, DataSourceUpdateMode::OnPropertyChanged);
	ShowMaFiles->DataBindings->Add ("Checked", this, "CharacterFiles_ShowMaFiles", true, DataSourceUpdateMode::OnPropertyChanged);
	ShowOfficeFiles->DataBindings->Add ("Checked", this, "CharacterFiles_ShowOfficeFiles", true, DataSourceUpdateMode::OnPropertyChanged);
	ShowSpeakingFiles->DataBindings->Add ("Checked", this, "CharacterFiles_ShowSpeakingFiles", true, DataSourceUpdateMode::OnPropertyChanged);
	ShowSilentFiles->DataBindings->Add ("Checked", this, "CharacterFiles_ShowSilentFiles", true, DataSourceUpdateMode::OnPropertyChanged);
	VerifyFileVersion->DataBindings->Add ("Checked", this, "CharacterFiles_VerifyFileVersion", true, DataSourceUpdateMode::OnPropertyChanged);
	DefaultFilePath->DataBindings->Add ("Text", this, "CharacterFiles_DefaultFilePath", true, DataSourceUpdateMode::OnValidation);
	DefaultSearchPath->DataBindings->Add ("Text", this, "CharacterFiles_DefaultSearchPath", true, DataSourceUpdateMode::OnValidation);
	CurrentSearchPath->DataBindings->Add ("Text", this, "CharacterFiles_CurrentSearchPath", true, DataSourceUpdateMode::OnValidation);
	FilesListBox->DataBindings->Add ("DataSource", this, "CharacterFiles_FilePaths", true, DataSourceUpdateMode::OnValidation);
// 
// TabTTSEngines
// 
	TTSEngineList->AutoGenerateColumns = false;
	TTSEngineList->DataBindings->Add ("Enabled", this, "TTSEngines_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	TTSEngineList->DataBindings->Add ("DataSource", this, "TTSEngines_List", true, DataSourceUpdateMode::OnValidation);
// 
// TabSREngines
// 
	SREngineList->AutoGenerateColumns = false;
	SREngineList->DataBindings->Add ("Enabled", this, "SREngines_Connected", true, DataSourceUpdateMode::OnPropertyChanged);
	SREngineList->DataBindings->Add ("DataSource", this, "SREngines_List", true, DataSourceUpdateMode::OnValidation);
}

ControlTestForm::~ControlTestForm ()
{
	if	(components)
	{
		delete components;
	}
}

/////////////////////////////////////////////////////////////////////////////

System::Void ControlTestForm::ShowConnected ()
{
	try
	{
		ControlConnectedButton->DataBindings["Checked"]->ReadValue();
	}
	catch (...)
	{}
#ifdef	_DEBUG	
	GC::Collect ();			
#endif	
}

System::Void ControlTestForm::LoadButton_Click(System::Object^  sender, System::EventArgs^  e)
{
	LoadCharacter^	lLoad = gcnew LoadCharacter (TestDaControl);
	
	if	(	
			(lLoad->ShowDialog () == System::Windows::Forms::DialogResult::OK)
		&&	(!String::IsNullOrEmpty (lLoad->mFilePath))
		&&	(!String::IsNullOrEmpty (lLoad->mCharacterID))
		)
	{
		try
		{
			TestDaControl->Characters->Load (lLoad->mCharacterID, lLoad->mFilePath);
			if	(Characters_Connected)
			{
				CharacterList->DataBindings ["DataSource"]->ReadValue();
			}
		}
		catch (...)
		{}
	}
	ShowConnected ();
}
