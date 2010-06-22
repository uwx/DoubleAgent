#include "StdAfx.h"
#include "ControlTestForm.h"
#include "LoadCharacter.h"

using namespace System::Configuration;

namespace DoubleAgent {
/////////////////////////////////////////////////////////////////////////////

ControlTestForm::ControlTestForm ()
{
	try
	{
		InitializeComponent();
	}
	catch AnyExceptionDebug
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

System::Boolean ControlTestForm::CharStyleSoundEffects::get ()
{
	if	(
			(TestDaControl)
		&&	(TestDaControl->CharacterStyle & (int)CharacterStyleFlags::SoundEffects)
		)
	{
		return true;
	}
	return false;
}
System::Void ControlTestForm::CharStyleSoundEffects::set (System::Boolean value)
{
	if	(TestDaControl)
	{
		if	(value)
		{
			TestDaControl->CharacterStyle |= (int)CharacterStyleFlags::SoundEffects;
		}
		else
		{
			TestDaControl->CharacterStyle &= ~(int)CharacterStyleFlags::SoundEffects;
		}
	}
}

System::Boolean ControlTestForm::CharStyleIdleEnabled::get ()
{
	if	(
			(TestDaControl)
		&&	(TestDaControl->CharacterStyle & (int)CharacterStyleFlags::IdleEnabled)
		)
	{
		return true;
	}
	return false;
}
System::Void ControlTestForm::CharStyleIdleEnabled::set (System::Boolean value)
{
	if	(TestDaControl)
	{
		if	(value)
		{
			TestDaControl->CharacterStyle |= (int)CharacterStyleFlags::IdleEnabled;
		}
		else
		{
			TestDaControl->CharacterStyle &= ~(int)CharacterStyleFlags::IdleEnabled;
		}
	}
}

System::Boolean ControlTestForm::CharStyleAutoPopupMenu::get ()
{
	if	(
			(TestDaControl)
		&&	(TestDaControl->CharacterStyle & (int)CharacterStyleFlags::AutoPopupMenu)
		)
	{
		return true;
	}
	return false;
}
System::Void ControlTestForm::CharStyleAutoPopupMenu::set (System::Boolean value)
{
	if	(TestDaControl)
	{
		if	(value)
		{
			TestDaControl->CharacterStyle |= (int)CharacterStyleFlags::AutoPopupMenu;
		}
		else
		{
			TestDaControl->CharacterStyle &= ~(int)CharacterStyleFlags::AutoPopupMenu;
		}
	}
}

System::Boolean ControlTestForm::CharStyleIconShown::get ()
{
	if	(
			(TestDaControl)
		&&	(TestDaControl->CharacterStyle & (int)CharacterStyleFlags::IconShown)
		)
	{
		return true;
	}
	return false;
}
System::Void ControlTestForm::CharStyleIconShown::set (System::Boolean value)
{
	if	(TestDaControl)
	{
		if	(value)
		{
			TestDaControl->CharacterStyle |= (int)CharacterStyleFlags::IconShown;
		}
		else
		{
			TestDaControl->CharacterStyle &= ~(int)CharacterStyleFlags::IconShown;
		}
	}
}

System::Boolean ControlTestForm::CharStyleSmoothed::get ()
{
	if	(
			(TestDaControl)
		&&	((TestDaControl->CharacterStyle & (int)CharacterStyleFlags::Smoothed) == (int)CharacterStyleFlags::Smoothed)
		)
	{
		return true;
	}
	return false;
}
System::Void ControlTestForm::CharStyleSmoothed::set (System::Boolean value)
{
	if	(TestDaControl)
	{
		if	(value)
		{
			TestDaControl->CharacterStyle |= (int)CharacterStyleFlags::Smoothed;
		}
		else
		{
			TestDaControl->CharacterStyle &= ~(int)CharacterStyleFlags::Smoothed;
		}
	}
}

System::Boolean ControlTestForm::CharStyleSmoothEdges::get ()
{
	if	(
			(TestDaControl)
		&&	(TestDaControl->CharacterStyle & (int)CharacterStyleFlags::SmoothEdges)
		)
	{
		return true;
	}
	return false;
}
System::Void ControlTestForm::CharStyleSmoothEdges::set (System::Boolean value)
{
	if	(TestDaControl)
	{
		if	(value)
		{
			TestDaControl->CharacterStyle |= (int)CharacterStyleFlags::SmoothEdges;
		}
		else
		{
			TestDaControl->CharacterStyle &= ~(int)CharacterStyleFlags::Smoothed;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

System::Void ControlTestForm::ShowConnected ()
{
	try
	{
		ConnectedButton->Checked = TestDaControl->Connected;
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
		CharacterPageData->Character = pCharacter;
		CharacterPageBinding->ResetCurrentItem ();
	}
	catch (...)
	{}

	try
	{
		if	(pCharacter)
		{
			CommandsBinding->DataSource = gcnew CommandsWrapper (pCharacter->Commands);
		}
		else
		{
			CommandsBinding->DataSource = nullptr;
		}
	}
	catch AnyExceptionDebug
	{}

	try
	{
		if	(pCharacter)
		{
			CommandBinding->DataSource = pCharacter->Commands;
			CharCommands->DataSource = CommandBinding;
		}
		else
		{
			CharCommands->DataSource = nullptr;
			CommandBinding->DataSource = nullptr;
		}
	}
	catch AnyExceptionDebug
	{}

	try
	{
		DoubleAgent::Control::Character^	lContained = TestDaControl->ControlCharacter;

		if	(
				(!TestDaControl->Connected)
			&&	(pCharacter)
			&&	(
					(!lContained)
				||	(String::Compare (pCharacter->CharacterID, lContained->CharacterID) != 0)
				)
			)
		{
			SetContainedButton->Enabled = true;
			SetContainedButton->Text = gcnew String("Contain\n") + pCharacter->CharacterID;
		}
		else
		if	(lContained)
		{
			SetContainedButton->Enabled = true;
			SetContainedButton->Text = gcnew String("Uncontain\n") + lContained->CharacterID;
		}
		else
		{
			SetContainedButton->Enabled = false;
			SetContainedButton->Text = "Contain";
		}
	}
	catch (...)
	{}
}

/////////////////////////////////////////////////////////////////////////////

System::Void ControlTestForm::FormBindingContextChanged(System::Object^  sender, System::EventArgs^  e)
{
	if	(!CharacterPageData)
	{
		CharacterPageData = gcnew DoubleAgent::CharacterPageData;
		CharacterPageData->Control = TestDaControl;
		CharacterPageData->BindingSource = CharacterBinding;
		CharacterPageData->mBalloonBinding = BalloonBinding;
		CharacterPageData->mAnimationsBinding = AnimationsBinding;
		CharacterPageBinding->DataSource = CharacterPageData;

		CommandsBinding->DataSource = gcnew CommandsWrapper (nullptr);
		EventsBinding->DataSource = CharacterPageData->EventList;
	}

	if	(!CharactersPageData)
	{
		CharactersPageData = gcnew DoubleAgent::CharactersPageData;
		CharactersPageData->Control = TestDaControl;
		CharactersPageData->BoundChanged += gcnew System::EventHandler(this, &ControlTestForm::DataBoundChanged);
		CharactersPageData->BindingSource = CharactersBinding;
		CharactersPageBinding->DataSource = CharactersPageData;
	}
	if	(!SettingsPageData)
	{
		SettingsPageData = gcnew DoubleAgent::SettingsPageData;
		SettingsPageData->Control = TestDaControl;
		SettingsPageData->BoundChanged += gcnew System::EventHandler(this, &ControlTestForm::DataBoundChanged);
		SettingsPageData->BindingSource = SettingsBinding;
		SettingsPageBinding->DataSource = SettingsPageData;
	}
	if	(!PropertySheetPageData)
	{
		PropertySheetPageData = gcnew DoubleAgent::PropertySheetPageData;
		PropertySheetPageData->Control = TestDaControl;
		PropertySheetPageData->BoundChanged += gcnew System::EventHandler(this, &ControlTestForm::DataBoundChanged);
		PropertySheetPageData->BindingSource = PropertySheetBinding;
		PropertySheetPageBinding->DataSource = PropertySheetPageData;
	}
	if	(!CharacterFilesPageData)
	{
		CharacterFilesPageData = gcnew DoubleAgent::CharacterFilesPageData;
		CharacterFilesPageData->Control = TestDaControl;
		CharacterFilesPageData->BoundChanged += gcnew System::EventHandler(this, &ControlTestForm::DataBoundChanged);
		CharacterFilesPageBinding->DataSource = CharacterFilesPageData;
	}
	if	(!TTSEnginesPageData)
	{
		TTSEnginesPageData = gcnew DoubleAgent::TTSEnginesPageData;
		TTSEnginesPageData->Control = TestDaControl;
		TTSEnginesPageData->BoundChanged += gcnew System::EventHandler(this, &ControlTestForm::DataBoundChanged);
		TTSEnginesPageData->BindingSource = TTSEnginesBinding;
		TTSEnginesPageBinding->DataSource = TTSEnginesPageData;
	}
	if	(!SREnginesPageData)
	{
		SREnginesPageData = gcnew DoubleAgent::SREnginesPageData;
		SREnginesPageData->Control = TestDaControl;
		SREnginesPageData->BoundChanged += gcnew System::EventHandler(this, &ControlTestForm::DataBoundChanged);
		SREnginesPageData->BindingSource = SREnginesBinding;
		SREnginesPageBinding->DataSource = SREnginesPageData;
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
			if	(CharactersPageData->Bound)
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

			CharactersPageData->CharacterUnloading = lCharacterID;
			CharacterList->DataBindings ["DataSource"]->ReadValue();
			TestDaControl->Characters->Unload (lCharacterID);
			CharactersPageData->CharacterUnloading = nullptr;
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

System::Void ControlTestForm::SetContainedButton_Click(System::Object^  sender, System::EventArgs^  e)
{
	DoubleAgent::Control::Character^	lCharacter;

	if	(lCharacter = CharacterPageData->Character)
	{
		try
		{
			if	(
					(TestDaControl->ControlCharacter)
				&&	(String::Compare (TestDaControl->ControlCharacter->CharacterID, lCharacter->CharacterID) == 0)
				)
			{
				TestDaControl->ControlCharacter = nullptr;
			}
			else
			{
				TestDaControl->ControlCharacter = lCharacter;
			}
		}
		catch AnyExceptionDebug
		try
		{
			SelectCharacter (lCharacter);
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////

System::Void ControlTestForm::CharacterList_SelectionChanged(System::Object^  sender, System::EventArgs^  e)
{
	DoubleAgent::Control::Character^	lCharacter = nullptr;

	if	(
			(CharacterList->RowCount > 1)
		&&	(CharacterList->SelectedRows->Count == 1)
		)
	{
		lCharacter = TestDaControl->Characters->Index [CharacterList->SelectedRows[0]->Index];
	}
	if	(lCharacter)
	{
		UnloadButton->Enabled = true;
		SetCurrentButton->Enabled = true;
		SetCurrentButton->Text = gcnew String("Select\n") + lCharacter->CharacterID;
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

	if	(lCharacter = CharacterPageData->Character)
	{
		try
		{
			CharacterPageData->mLastRequest = lCharacter->Show (CharShowFast->Checked);
		}
		catch AnyExceptionDebug
	}
}

System::Void ControlTestForm::CharHide_Click(System::Object^  sender, System::EventArgs^  e)
{
	DoubleAgent::Control::Character^	lCharacter;

	if	(lCharacter = CharacterPageData->Character)
	{
		try
		{
			CharacterPageData->mLastRequest = lCharacter->Hide (CharHideFast->Checked);
		}
		catch AnyExceptionDebug
	}
}

System::Void ControlTestForm::CharMoveTo_Click(System::Object^  sender, System::EventArgs^  e)
{
	DoubleAgent::Control::Character^	lCharacter;

	if	(lCharacter = CharacterPageData->Character)
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

	if	(lCharacter = CharacterPageData->Character)
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

	if	(lCharacter = CharacterPageData->Character)
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

	if	(lCharacter = CharacterPageData->Character)
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

	if	(lCharacter = CharacterPageData->Character)
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

	if	(lCharacter = CharacterPageData->Character)
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

	if	(lCharacter = CharacterPageData->Character)
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

	if	(lCharacter = CharacterPageData->Character)
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

	if	(lCharacter = CharacterPageData->Character)
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

	if	(lCharacter = CharacterPageData->Character)
	{
		try
		{
			lCharacter->Width = lCharacter->OriginalWidth;
			CharWidth->Value = lCharacter->Width;
		}
		catch AnyExceptionDebug
	}
}

System::Void ControlTestForm::CharHeightDefault_Click(System::Object^  sender, System::EventArgs^  e)
{
	DoubleAgent::Control::Character^	lCharacter;

	if	(lCharacter = CharacterPageData->Character)
	{
		try
		{
			lCharacter->Height = lCharacter->OriginalHeight;
			CharHeight->Value = lCharacter->Height;
		}
		catch AnyExceptionDebug
	}
}

System::Void ControlTestForm::CharGenerateIcon_Click(System::Object^  sender, System::EventArgs^  e)
{
	DoubleAgent::Control::Character^	lCharacter;

	if	(lCharacter = CharacterPageData->Character)
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
