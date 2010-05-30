#include "StdAfx.h"
#include "ControlTestForm.h"
#include "LoadCharacter.h"

using namespace System::Configuration;

namespace DoubleAgent {
/////////////////////////////////////////////////////////////////////////////

ControlTestForm::ControlTestForm ()
{
	InitializeComponent();

	//ShowDefaultCharacterY->DataBindings->Add ("Enabled", ShowDefaultCharacterPos, "Checked", true, DataSourceUpdateMode::Never);
	//ShowDefaultCharacterY->DataBindings->Add ("Enabled", ShowDefaultCharacterPos, "Checked", true, DataSourceUpdateMode::Never);
}

ControlTestForm::~ControlTestForm ()
{
	if	(components)
	{
		delete components;
	}
#ifdef	_DEBUG
	GC::Collect ();
#endif
}

/////////////////////////////////////////////////////////////////////////////

System::Void ControlTestForm::ShowConnected ()
{
	try
	{
		ControlConnectedButton->Checked = TestDaControl->Connected;
	}
	catch (...)
	{}
#ifdef	_DEBUG
	GC::Collect ();
#endif
}

System::Void ControlTestForm::SelectCharacter (DoubleAgent::Control::Character^ pCharacter)
{
	try
	{
		if	(pCharacter)
		{
			CharacterBindingSource->DataSource = pCharacter;
		}
	}
	catch (...)
	{}
	try
	{
		CharacterDataBinding->ResetCurrentItem ();
	}
	catch (...)
	{}
	try
	{
		CharPlayAnimation->DataSource = CharacterData->Animations;
		CharCommands->DataSource = CharacterData->CommandList;
	}
	catch (...)
	{}

	try
	{
		if	(pCharacter)
		{
			BalloonBindingSource->DataSource = pCharacter->Balloon;
			CommandsBindingSource->DataSource = gcnew CommandsWrapper (pCharacter->Commands);
//			CommandBindingSource->DataSource = CharacterData->CommandList;
		}
	}
	catch (...)
	{}
}

/////////////////////////////////////////////////////////////////////////////

System::Void ControlTestForm::FormBindingContextChanged(System::Object^  sender, System::EventArgs^  e)
{
	if	(!CharacterData)
	{
		CharacterData = gcnew DoubleAgent::CharacterData;
		CharacterData->CharacterBinding = CharacterBindingSource;
		CharacterData->Control = TestDaControl;
		CharacterDataBinding->DataSource = CharacterData;

		CommandsBindingSource->DataSource = gcnew CommandsWrapper (nullptr);
		CommandBindingSource->DataSource = CharacterData->CommandList;
		EventsBindingSource->DataSource = CharacterData->EventList;
	}

	if	(!CharactersData)
	{
		CharactersData = gcnew DoubleAgent::CharactersData;
		CharactersData->Control = TestDaControl;
		CharactersData->BoundChanged += gcnew System::EventHandler(this, &ControlTestForm::DataBoundChanged);
		CharactersDataBinding->DataSource = CharactersData;
	}
	if	(!SettingsData)
	{
		SettingsData = gcnew DoubleAgent::SettingsData;
		SettingsData->Control = TestDaControl;
		SettingsData->BoundChanged += gcnew System::EventHandler(this, &ControlTestForm::DataBoundChanged);
		SettingsDataBinding->DataSource = SettingsData;
	}
	if	(!PropertySheetData)
	{
		PropertySheetData = gcnew DoubleAgent::PropertySheetData;
		PropertySheetData->Control = TestDaControl;
		PropertySheetData->BoundChanged += gcnew System::EventHandler(this, &ControlTestForm::DataBoundChanged);
		PropertySheetDataBinding->DataSource = PropertySheetData;
	}
	if	(!CharacterFilesData)
	{
		CharacterFilesData = gcnew DoubleAgent::CharacterFilesData;
		CharacterFilesData->Control = TestDaControl;
		CharacterFilesData->BoundChanged += gcnew System::EventHandler(this, &ControlTestForm::DataBoundChanged);
		CharacterFilesDataBinding->DataSource = CharacterFilesData;
	}
	if	(!TTSEnginesData)
	{
		TTSEnginesData = gcnew DoubleAgent::TTSEnginesData;
		TTSEnginesData->Control = TestDaControl;
		TTSEnginesData->BoundChanged += gcnew System::EventHandler(this, &ControlTestForm::DataBoundChanged);
		TTSEnginesDataBinding->DataSource = TTSEnginesData;
	}
	if	(!SREnginesData)
	{
		SREnginesData = gcnew DoubleAgent::SREnginesData;
		SREnginesData->Control = TestDaControl;
		SREnginesData->BoundChanged += gcnew System::EventHandler(this, &ControlTestForm::DataBoundChanged);
		SREnginesDataBinding->DataSource = SREnginesData;
	}

	if	(CharCommands)
	{
		CharCommands->AutoGenerateColumns = false;
	}
	if	(EventsGrid)
	{
		EventsGrid->AutoGenerateColumns = false;
	}
	if	(CharacterList)
	{
		CharacterList->AutoGenerateColumns = false;
	}
	if	(TTSEngineList)
	{
		TTSEngineList->AutoGenerateColumns = false;
	}
	if	(SREngineList)
	{
		SREngineList->AutoGenerateColumns = false;
	}
}

/////////////////////////////////////////////////////////////////////////////

System::Void ControlTestForm::DataBoundChanged(System::Object^  sender, System::EventArgs^  e)
{
	ShowConnected ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

System::Void ControlTestForm::LoadButton_Click(System::Object^  sender, System::EventArgs^  e)
{
	LoadCharacter^						lLoad = gcnew LoadCharacter (TestDaControl);
	DoubleAgent::Control::Character^	lCharacter;

	if	(
			(lLoad->ShowDialog () == System::Windows::Forms::DialogResult::OK)
		&&	(!String::IsNullOrEmpty (lLoad->mFilePath))
		&&	(!String::IsNullOrEmpty (lLoad->mCharacterID))
		)
	{
		try
		{
			TestDaControl->Characters->Load (lLoad->mCharacterID, lLoad->mFilePath);
			if	(CharactersData->Bound)
			{
				CharacterList->DataBindings ["DataSource"]->ReadValue();
			}
		}
		catch (...)
		{}
		try
		{
			DoubleAgent::Control::Commands^	lCommands;

			lCharacter = TestDaControl->Characters [lLoad->mCharacterID];
			lCommands = lCharacter->Commands;

			lCommands->Caption = lCharacter->Name + " Commands";
			lCommands->VoiceCaption = lCommands->Caption;
			lCommands->Visible = true;
			lCommands->GlobalVoiceCommandsEnabled = true;
			lCommands->Add ("One", "First Command", "one", true, true);
			lCommands->Add ("Three", "Third Command", "three", true, true);
			lCommands->Insert ("Two", "Three", true, "Second Command", "two", true, true);
		}
		catch (...)
		{}
		try
		{
			SelectCharacter (lCharacter);
		}
		catch (...)
		{}
	}
	ShowConnected ();
};

/////////////////////////////////////////////////////////////////////////////

System::Void ControlTestForm::UnloadButton_Click(System::Object^  sender, System::EventArgs^  e)
{
	if	(
			(CharacterList->RowCount > 1)
		&&	(CharacterList->SelectedRows->Count == 1)
		)
	{
		try
		{
			int									lCharacterNdx;
			DoubleAgent::Control::Character^	lCharacter;
			String^								lCharacterID;

			lCharacterNdx = CharacterList->SelectedRows[0]->Index;
			lCharacter = TestDaControl->Characters->Index [lCharacterNdx];
			lCharacterID = lCharacter->CharacterID;

			if	(lCharacterNdx < CharacterList->RowCount-1)
			{
				SelectCharacter (TestDaControl->Characters->Index [lCharacterNdx+1]);
			}
			else
			{
				SelectCharacter (TestDaControl->Characters->Index [lCharacterNdx-1]);
			}

			CharactersData->CharacterUnloading = lCharacterID;
			CharacterList->DataBindings ["DataSource"]->ReadValue();
			TestDaControl->Characters->Unload (lCharacterID);
			CharactersData->CharacterUnloading = nullptr;
		}
		catch (...)
		{}

		CharacterList->DataBindings ["DataSource"]->ReadValue();
	}
}

System::Void ControlTestForm::SetCurrentButton_Click(System::Object^  sender, System::EventArgs^  e)
{
	int									lCharacterNdx;
	DoubleAgent::Control::Character^	lCharacter;

	if	(CharacterList->SelectedRows->Count == 1)
	{
		try
		{
			lCharacterNdx = CharacterList->SelectedRows[0]->Index;
			lCharacter = TestDaControl->Characters->Index [lCharacterNdx];
			SelectCharacter (lCharacter);
		}
		catch (...)
		{}
	}
}

/////////////////////////////////////////////////////////////////////////////

System::Void ControlTestForm::CharacterList_SelectionChanged(System::Object^  sender, System::EventArgs^  e)
{
	if	(
			(CharacterList->RowCount > 1)
		&&	(CharacterList->SelectedRows->Count == 1)
		)
	{
		UnloadButton->Enabled = true;
		SetCurrentButton->Enabled = true;
		SetCurrentButton->Text = gcnew String("Select\n") + TestDaControl->Characters->Index [CharacterList->SelectedRows[0]->Index]->CharacterID;
	}
	else
	{
		UnloadButton->Enabled = false;
		SetCurrentButton->Enabled = false;
		SetCurrentButton->Text = "Select";
	}
}

System::Void ControlTestForm::CharacterList_DataBindingComplete(System::Object^  sender, System::Windows::Forms::DataGridViewBindingCompleteEventArgs^  e)
{
	CharacterList_SelectionChanged (sender, e);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

System::Void ControlTestForm::CharShow_Click(System::Object^  sender, System::EventArgs^  e)
{
	DoubleAgent::Control::Character^	lCharacter;

	if	(lCharacter = CharacterData->Character)
	{
		try
		{
			CharacterData->mLastRequest = lCharacter->Show (CharShowFast->Checked);
		}
		catch AnyExceptionDebug
	}
}

System::Void ControlTestForm::CharHide_Click(System::Object^  sender, System::EventArgs^  e)
{
	DoubleAgent::Control::Character^	lCharacter;

	if	(lCharacter = CharacterData->Character)
	{
		try
		{
			CharacterData->mLastRequest = lCharacter->Hide (CharHideFast->Checked);
		}
		catch AnyExceptionDebug
	}
}

System::Void ControlTestForm::CharMoveTo_Click(System::Object^  sender, System::EventArgs^  e)
{
	DoubleAgent::Control::Character^	lCharacter;

	if	(lCharacter = CharacterData->Character)
	{
		try
		{
			lCharacter->MoveTo ((short)CharMoveToLeft->Value, (short)CharMoveToTop->Value, 1000);
		}
		catch AnyExceptionDebug
	}
}

System::Void ControlTestForm::CharGestureAt_Click(System::Object^  sender, System::EventArgs^  e)
{
	DoubleAgent::Control::Character^	lCharacter;

	if	(lCharacter = CharacterData->Character)
	{
		try
		{
			lCharacter->GestureAt ((short)CharGestureAtX->Value, (short)CharGestureAtY->Value);
		}
		catch AnyExceptionDebug
	}
}

System::Void ControlTestForm::CharSpeak_Click(System::Object^  sender, System::EventArgs^  e)
{
	DoubleAgent::Control::Character^	lCharacter;

	if	(lCharacter = CharacterData->Character)
	{
		try
		{
			lCharacter->Speak (CharSpeakText->Text, nullptr);
		}
		catch AnyExceptionDebug
	}
}

System::Void ControlTestForm::CharThink_Click(System::Object^  sender, System::EventArgs^  e)
{
	DoubleAgent::Control::Character^	lCharacter;

	if	(lCharacter = CharacterData->Character)
	{
		try
		{
			lCharacter->Think (CharThinkText->Text);
		}
		catch AnyExceptionDebug
	}
}

System::Void ControlTestForm::CharListen_Click(System::Object^  sender, System::EventArgs^  e)
{
	DoubleAgent::Control::Character^	lCharacter;

	if	(lCharacter = CharacterData->Character)
	{
		try
		{
			lCharacter->Listen (true);
		}
		catch AnyExceptionDebug
	}
}

System::Void ControlTestForm::CharListenStop_Click(System::Object^  sender, System::EventArgs^  e)
{
	DoubleAgent::Control::Character^	lCharacter;

	if	(lCharacter = CharacterData->Character)
	{
		try
		{
			lCharacter->Listen (false);
		}
		catch AnyExceptionDebug
	}
}

System::Void ControlTestForm::CharShowPopupMenu_Click(System::Object^  sender, System::EventArgs^  e)
{
	DoubleAgent::Control::Character^	lCharacter;

	if	(lCharacter = CharacterData->Character)
	{
		try
		{
			lCharacter->ShowPopupMenu ((short)CharShowMenuX->Value, (short)CharShowMenuY->Value);
		}
		catch AnyExceptionDebug
	}
}

System::Void ControlTestForm::CharPlay_Click(System::Object^  sender, System::EventArgs^  e)
{
	DoubleAgent::Control::Character^	lCharacter;

	if	(lCharacter = CharacterData->Character)
	{
		try
		{
			lCharacter->Play (CharPlayAnimation->SelectedValue->ToString());
		}
		catch AnyExceptionDebug
	}
}

System::Void ControlTestForm::CharStopAll_Click(System::Object^  sender, System::EventArgs^  e)
{
	DoubleAgent::Control::Character^	lCharacter;

	if	(lCharacter = CharacterData->Character)
	{
		try
		{
			lCharacter->StopAll (nullptr);
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////

System::Void ControlTestForm::CharWidthDefault_Click(System::Object^  sender, System::EventArgs^  e)
{
	DoubleAgent::Control::Character^	lCharacter;

	if	(lCharacter = CharacterData->Character)
	{
		try
		{
			lCharacter->Width = lCharacter->OriginalWidth;
		}
		catch AnyExceptionDebug
	}
}

System::Void ControlTestForm::CharHeightDefault_Click(System::Object^  sender, System::EventArgs^  e)
{
	DoubleAgent::Control::Character^	lCharacter;

	if	(lCharacter = CharacterData->Character)
	{
		try
		{
			lCharacter->Height = lCharacter->OriginalHeight;
		}
		catch AnyExceptionDebug
	}
}

System::Void ControlTestForm::CharGenerateIcon_Click(System::Object^  sender, System::EventArgs^  e)
{
	DoubleAgent::Control::Character^	lCharacter;

	if	(lCharacter = CharacterData->Character)
	{
		try
		{
			lCharacter->GenerateIcon (-1,-1,-1,-1);
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////
} // namespace DoubleAgent
