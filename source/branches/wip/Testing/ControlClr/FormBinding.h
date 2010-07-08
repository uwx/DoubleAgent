#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace DoubleAgent::Control;

namespace DoubleAgent {
/////////////////////////////////////////////////////////////////////////////

#pragma region FormDataBinding
public ref class FormDataBinding : public System::Object
{
public:
	FormDataBinding ()
	{
	}
	~FormDataBinding()
	{
	}

	property DoubleAgent::AxControl::AxControl^ Control
	{
		DoubleAgent::AxControl::AxControl^ get ()
		{
			return mControl;
		}
		System::Void set (DoubleAgent::AxControl::AxControl^ value)
		{
			mControl = value;
		}
	}

	property System::Windows::Forms::BindingSource^ BindingSource
	{
		System::Windows::Forms::BindingSource^ get ()
		{
			return mBindingSource;
		}
		System::Void set (System::Windows::Forms::BindingSource^ value)
		{
			mBindingSource = value;
		}
	}

	property System::Boolean Bound
	{
		virtual System::Boolean get () {return false;}
		virtual System::Void set (System::Boolean value) {}
	}

	event System::EventHandler^ BoundChanged
	{
		void add (System::EventHandler^ pDelegate)
		{
			mBoundChanged = static_cast<System::EventHandler^> (Delegate::Combine (mBoundChanged, pDelegate));
		}
		void remove (System::EventHandler^ pDelegate)
		{
			mBoundChanged = static_cast<System::EventHandler^> (Delegate::Remove (mBoundChanged, pDelegate));
		}
		void raise (Object^ pSender, System::EventArgs^ pEvent)
		{
			if  (mBoundChanged)
			{
				mBoundChanged (pSender, pEvent);
			}
		}
	}

protected:
	DoubleAgent::AxControl::AxControl^		mControl;
	System::Windows::Forms::BindingSource^	mBindingSource;
	System::EventHandler^					mBoundChanged;
};
#pragma endregion

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#pragma region CommandsWrapper
public ref class CommandsWrapper
{
protected:
	DoubleAgent::Control::Commands^	mCommands;
public:
	CommandsWrapper (DoubleAgent::Control::Commands^ pCommands)
	:	mCommands (pCommands)
	{}

	property System::String^ Caption
	{
		System::String^ get ()
		{
			if	(mCommands)
			{
				return mCommands->Caption;
			}
			return String::Empty;
		}
		System::Void set (System::String^ value)
		{
			if	(mCommands)
			{
				mCommands->Caption = value;
			}
		}
	}

	property System::String^ VoiceCaption
	{
		System::String^ get ()
		{
			if	(mCommands)
			{
				return mCommands->VoiceCaption;
			}
			return String::Empty;
		}
		System::Void set (System::String^ value)
		{
			if	(mCommands)
			{
				mCommands->VoiceCaption = value;
			}
		}
	}

	property System::String^ VoiceGrammar
	{
		System::String^ get ()
		{
			if	(mCommands)
			{
				return mCommands->VoiceGrammar;
			}
			return String::Empty;
		}
		System::Void set (System::String^ value)
		{
			if	(mCommands)
			{
				mCommands->VoiceGrammar = value;
			}
		}
	}

	property System::String^ DefaultCommand
	{
		System::String^ get ()
		{
			if	(mCommands)
			{
				return mCommands->DefaultCommand;
			}
			return String::Empty;
		}
		System::Void set (System::String^ value)
		{
			if	(mCommands)
			{
				mCommands->DefaultCommand = value;
			}
		}
	}

	property System::String^ FontName
	{
		System::String^ get ()
		{
			if	(mCommands)
			{
				return mCommands->FontName;
			}
			return String::Empty;
		}
		System::Void set (System::String^ value)
		{
			if	(mCommands)
			{
				mCommands->FontName = value;
			}
		}
	}

	property int FontSize
	{
		int get ()
		{
			if	(mCommands)
			{
				return mCommands->FontSize;
			}
			return false;
		}
		System::Void set (int value)
		{
			if	(mCommands)
			{
				mCommands->FontSize = value;
			}
		}
	}

	property System::Boolean Visible
	{
		System::Boolean get ()
		{
			if	(mCommands)
			{
				return mCommands->Visible;
			}
			return false;
		}
		System::Void set (System::Boolean value)
		{
			if	(mCommands)
			{
				mCommands->Visible = value;
			}
		}
	}

	property System::Boolean GlobalVoiceCommandsEnabled
	{
		System::Boolean get ()
		{
			if	(mCommands)
			{
				return mCommands->GlobalVoiceCommandsEnabled;
			}
			return false;
		}
		System::Void set (System::Boolean value)
		{
			if	(mCommands)
			{
				mCommands->GlobalVoiceCommandsEnabled = value;
			}
		}
	}
};
#pragma endregion

/////////////////////////////////////////////////////////////////////////////

#pragma region EventData
public ref class EventData
{
public:
	property System::String^ Name
	{
		System::String^ get ()
		{
			return mName;
		}
		System::Void set (System::String^ value)
		{
			mName = value;
		}
	}
	property System::String^ CharacterID
	{
		System::String^ get ()
		{
			return mCharacterID;
		}
		System::Void set (System::String^ value)
		{
			mCharacterID = value;
		}
	}
	property System::String^ Data
	{
		System::String^ get ()
		{
			return mData;
		}
		System::Void set (System::String^ value)
		{
			mData = value;
		}
	}

public:
	EventData () : mName (String::Empty), mCharacterID (String::Empty), mData (String::Empty)
	{}

	EventData (String^ pName) : mName (pName), mCharacterID (String::Empty), mData (String::Empty)
	{}

	EventData (DoubleAgent::AxControl::EventArgs_ActivateInput^ e)
	:	mName ("ActivateInput"), mCharacterID (e->CharacterID), mData (String::Empty)
	{}

	EventData (DoubleAgent::AxControl::EventArgs_DeactivateInput^ e)
	:	mName ("DeactivateInput"), mCharacterID (e->CharacterID), mData (String::Empty)
	{}

	EventData (DoubleAgent::AxControl::EventArgs_Show^ e)
	:	mName ("Show"), mCharacterID (e->CharacterID), mData (e->Cause.ToString())
	{}

	EventData (DoubleAgent::AxControl::EventArgs_Hide^ e)
	:	mName ("Hide"), mCharacterID (e->CharacterID), mData (e->Cause.ToString())
	{}

	EventData (DoubleAgent::AxControl::EventArgs_Move^ e)
	:	mName ("Move"), mCharacterID (e->CharacterID)
	{
		mData = String::Format ("{0:D}, {1:D} ({2})", e->x, e->y, e->Cause.ToString());
	}

	EventData (DoubleAgent::AxControl::EventArgs_Size^ e)
	:	mName ("Size"), mCharacterID (e->CharacterID)
	{
		mData = String::Format ("{0:D}, {1:D}", e->Width, e->Height);
	}

	EventData (DoubleAgent::AxControl::EventArgs_Click^ e)
	:	mName ("Click"), mCharacterID (e->CharacterID)
	{
		mData = String::Format ("{0:D}, {1:D} (button {2:D})", e->x, e->y, e->Button);
	}

	EventData (DoubleAgent::AxControl::EventArgs_DblClick^ e)
	:	mName ("DblClick"), mCharacterID (e->CharacterID)
	{
		mData = String::Format ("{0:D}, {1:D} (button {2:D})", e->x, e->y, e->Button);
	}

	EventData (DoubleAgent::AxControl::EventArgs_DragStart^ e)
	:	mName ("DragStart"), mCharacterID (e->CharacterID)
	{
		mData = String::Format ("{0:D}, {1:D} (button {2:D})", e->x, e->y, e->Button);
	}

	EventData (DoubleAgent::AxControl::EventArgs_DragComplete^ e)
	:	mName ("DragComplete"), mCharacterID (e->CharacterID)
	{
		mData = String::Format ("{0:D}, {1:D} (button {2:D})", e->x, e->y, e->Button);
	}

	EventData (DoubleAgent::AxControl::EventArgs_ListenStart^ e)
	:	mName ("ListenStart"), mCharacterID (e->CharacterID), mData (String::Empty)
	{}

	EventData (DoubleAgent::AxControl::EventArgs_ListenComplete^ e)
	:	mName ("ListenComplete"), mCharacterID (e->CharacterID), mData (e->Cause.ToString())
	{}

	EventData (DoubleAgent::AxControl::EventArgs_IdleStart^ e)
	:	mName ("IdleStart"), mCharacterID (e->CharacterID), mData (String::Empty)
	{}

	EventData (DoubleAgent::AxControl::EventArgs_IdleComplete^ e)
	:	mName ("IdleComplete"), mCharacterID (e->CharacterID), mData (String::Empty)
	{}

	EventData (DoubleAgent::AxControl::EventArgs_BalloonShow^ e)
	:	mName ("BalloonShow"), mCharacterID (e->CharacterID), mData (String::Empty)
	{}

	EventData (DoubleAgent::AxControl::EventArgs_BalloonHide^ e)
	:	mName ("BalloonHide"), mCharacterID (e->CharacterID), mData (String::Empty)
	{}

	EventData (DoubleAgent::AxControl::EventArgs_Bookmark^ e)
	:	mName ("Bookmark"), mCharacterID (String::Empty), mData (e->BookmarkID.ToString())
	{}

	EventData (DoubleAgent::AxControl::EventArgs_Command^ e)
	:	mName ("Command"), mCharacterID (e->UserInput->CharacterID), mData (e->UserInput->Name)
	{}

	EventData (DoubleAgent::AxControl::EventArgs_RequestStart^ e)
	:	mName ("RequestStart"), mCharacterID (e->Request->default.ToString()), mData (((DoubleAgent::Control::RequestStatus)e->Request->Status).ToString())
	{
	}

	EventData (DoubleAgent::AxControl::EventArgs_RequestComplete^ e)
	:	mName ("RequestComplete"), mCharacterID (e->Request->default.ToString())
	{
		if	(String::IsNullOrEmpty (e->Request->Description))
		{
			mData = ((DoubleAgent::Control::RequestStatus)e->Request->Status).ToString();
		}
		else
		{
			mData = String::Format ("{0} ({1:X8} {2})", ((DoubleAgent::Control::RequestStatus)e->Request->Status).ToString(), e->Request->Number, e->Request->Description);
		}
	}

	EventData (DoubleAgent::AxControl::EventArgs_ActiveClientChange^ e)
	:	mName ("ActiveClientChange"), mCharacterID (e->CharacterID), mData (String::Empty)
	{}

	EventData (DoubleAgent::AxControl::EventArgs_DefaultCharacterChange^ e)
	:	mName ("DefaultCharacterChange"), mCharacterID (String::Empty), mData (e->CharGUID)
	{}

protected:
	String^	mName;
	String^	mCharacterID;
	String^	mData;
};
#pragma endregion

/////////////////////////////////////////////////////////////////////////////

#pragma region CharacterPageData
public ref class CharacterPageData : public FormDataBinding
{
public:
	property System::Boolean Bound
	{
		virtual System::Boolean get () override
		{
			return (Character != nullptr);
		}
	}

	property DoubleAgent::Control::Character^ Character
	{
		DoubleAgent::Control::Character^ get ()
		{
			DoubleAgent::Control::Character^	lCharacter;

			try
			{
				lCharacter = safe_cast <DoubleAgent::Control::Character^> (mBindingSource->DataSource);
			}
			catch AnyExceptionSilent

			return lCharacter;
		}
		void set (DoubleAgent::Control::Character^ pCharacter)
		{
			if	(mBindingSource)
			{
				try
				{
					if	(pCharacter)
					{
						mBindingSource->DataSource = pCharacter;
					}
					else
					{
						mBindingSource->DataSource = DoubleAgent::Control::Character::typeid;
					}
					mBindingSource->ResetCurrentItem ();
				}
				catch AnyExceptionDebug
			}

			if	(mBalloonBinding)
			{
				try
				{
					if	(pCharacter)
					{
						mBalloonBinding->DataSource = pCharacter->Balloon;
					}
					else
					{
						mBalloonBinding->DataSource = DoubleAgent::Control::Balloon::typeid;
					}
					mBindingSource->ResetCurrentItem ();
				}
				catch AnyExceptionDebug
			}

			if	(mAnimationsBinding)
			{
				try
				{
					if	(pCharacter)
					{
						mAnimationsBinding->DataSource = DoubleAgent::Control::Character::typeid;
						mAnimationsBinding->DataMember = "AnimationNames";
						mAnimationsBinding->DataSource = pCharacter->AnimationNames;
					}
					else
					{
						mAnimationsBinding->DataSource = DoubleAgent::Control::Character::typeid;
					}
				}
				catch AnyExceptionDebug
			}
		}
	}

	property System::Boolean IsListening
	{
		System::Boolean get ()
		{
			if	(Character)
			{
				DoubleAgent::Control::Settings^	lSettings = mControl->Settings;
				return (lSettings->AudioStatus == AudioStatusType::CharacterListening);
			}
			return false;
		}
	}
	property System::Boolean IsNotDefaultWidth
	{
		System::Boolean get ()
		{
			DoubleAgent::Control::Character^	lCharacter;

			if	(lCharacter = Character)
			{
				return (lCharacter->Width != lCharacter->OriginalWidth);
			}
			return false;
		}
	}
	property System::Boolean IsNotDefaultHeight
	{
		System::Boolean get ()
		{
			DoubleAgent::Control::Character^	lCharacter;

			if	(lCharacter = Character)
			{
				return (lCharacter->Height != lCharacter->OriginalHeight);
			}
			return false;
		}
	}

protected:
	Generic::List <EventData^>^  mEventList;
public:
	property Generic::List <EventData^>^ EventList
	{
		Generic::List <EventData^>^ get ()
		{
			if	(!mEventList)
			{
				mEventList = gcnew Generic::List <EventData^>;
			}
			return mEventList;
		}
	}

public:
	System::Windows::Forms::BindingSource^	mBalloonBinding;
	System::Windows::Forms::BindingSource^	mAnimationsBinding;
	DoubleAgent::Control::Request^			mLastRequest;
};
#pragma endregion

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#pragma region CharactersPageData
public ref class CharactersPageData : public FormDataBinding
{
protected:
	DoubleAgent::Control::Characters^	mCharacters;
public:
	property System::Boolean Bound
	{
		virtual System::Boolean get () override
		{
			return !!mCharacters;
		}
		virtual System::Void set (System::Boolean value) override
		{
			if	(!value)
			{
				mCharacters = nullptr;
			}
			else
			if	(!mCharacters)
			{
				mCharacters = mControl->Characters;
			}
			if	(mBindingSource)
			{
				if	(mCharacters)
				{
					mBindingSource->DataSource = gcnew Generic::List<DoubleAgent::Control::Character^> (mCharacters);
				}
				else
				{
					mBindingSource->DataSource = DoubleAgent::Control::Character::typeid;
				}
				mBindingSource->ResetCurrentItem ();
			}
			BoundChanged::raise (this, gcnew System::EventArgs);
		}
	}

	void CharacterLoaded ()
	{
		if	(
				(mBindingSource)
			&&	(mCharacters)
			)
		{
			mBindingSource->SuspendBinding ();
			mBindingSource->DataSource = gcnew Generic::List<DoubleAgent::Control::Character^> (mCharacters);
			mBindingSource->ResumeBinding ();
		}
	}

	void CharacterUnloaded ()
	{
		if	(
				(mBindingSource)
			&&	(mCharacters)
			)
		{
			mBindingSource->SuspendBinding ();
			mBindingSource->DataSource = gcnew Generic::List<DoubleAgent::Control::Character^> (mCharacters);
			mBindingSource->ResumeBinding ();
		}
	}
};
#pragma endregion

/////////////////////////////////////////////////////////////////////////////

#pragma region SettingsPageData
public ref class SettingsPageData : public FormDataBinding
{
protected:
	DoubleAgent::Control::Settings^	mSettings;
public:
	property System::Boolean Bound
	{
		virtual System::Boolean get () override
		{
			return !!mSettings;
		}
		virtual System::Void set (System::Boolean value) override
		{
			if	(!value)
			{
				mSettings = nullptr;
			}
			else
			if	(!mSettings)
			{
				mSettings = mControl->Settings;
			}
			if	(mBindingSource)
			{
				if	(mSettings)
				{
					mBindingSource->DataSource = mSettings;
				}
				else
				{
					mBindingSource->DataSource = DoubleAgent::Control::Settings::typeid;
				}
				mBindingSource->ResetCurrentItem ();
			}
			BoundChanged::raise (this, gcnew System::EventArgs);
		}
	}

	property System::String^ BalloonFontName
	{
		System::String^ get ()
		{
			return mSettings ? mSettings->BalloonFont ? safe_cast <stdole::StdFontClass^> (mSettings->BalloonFont)->Name : "<none>" : String::Empty;
		}
	}
	property System::Decimal BalloonFontSize
	{
		System::Decimal get ()
		{
			return (mSettings && mSettings->BalloonFont) ? safe_cast <stdole::StdFontClass^> (mSettings->BalloonFont)->Size : 0;
		}
	}
	property System::Boolean BalloonFontBold
	{
		System::Boolean get ()
		{
			return (mSettings && mSettings->BalloonFont) ? safe_cast <stdole::StdFontClass^> (mSettings->BalloonFont)->Bold : false;
		}
	}
	property System::Boolean BalloonFontItalic
	{
		System::Boolean get ()
		{
			return (mSettings && mSettings->BalloonFont) ? safe_cast <stdole::StdFontClass^> (mSettings->BalloonFont)->Italic : false;
		}
	}
	property System::Boolean BalloonFontUnderline
	{
		System::Boolean get ()
		{
			return (mSettings && mSettings->BalloonFont) ? safe_cast <stdole::StdFontClass^> (mSettings->BalloonFont)->Underline : false;
		}
	}
};
#pragma endregion

/////////////////////////////////////////////////////////////////////////////

#pragma region PropertySheetPageData
public ref class PropertySheetPageData : public FormDataBinding
{
protected:
	DoubleAgent::Control::PropertySheet^	mPropertySheet;
public:
	property System::Boolean Bound
	{
		virtual System::Boolean get () override
		{
			return !!mPropertySheet;
		}
		virtual System::Void set (System::Boolean value) override
		{
			if	(!value)
			{
				mPropertySheet = nullptr;
			}
			else
			if	(!mPropertySheet)
			{
				mPropertySheet = mControl->PropertySheet;
			}
			if	(mBindingSource)
			{
				if	(mPropertySheet)
				{
					mBindingSource->DataSource = mPropertySheet;
				}
				else
				{
					mBindingSource->DataSource = DoubleAgent::Control::PropertySheet::typeid;
				}
				mBindingSource->ResetCurrentItem ();
			}
			BoundChanged::raise (this, gcnew System::EventArgs);
		}
	}
};
#pragma endregion

/////////////////////////////////////////////////////////////////////////////

#pragma region CharacterFilesPageData
public ref class CharacterFilesPageData : public FormDataBinding
{
protected:
	DoubleAgent::Control::CharacterFiles^	mCharacterFiles;
public:
	property System::Boolean Bound
	{
		virtual System::Boolean get () override
		{
			return !!mCharacterFiles;
		}
		virtual System::Void set (System::Boolean value) override
		{
			if	(!value)
			{
				mCharacterFiles = nullptr;
			}
			else
			if	(!mCharacterFiles)
			{
				mCharacterFiles = mControl->CharacterFiles;
			}
			BoundChanged::raise (this, gcnew System::EventArgs);
		}
	}

	property System::Boolean ShowDaFiles
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
	property System::Boolean ShowMaFiles
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
	property System::Boolean ShowOfficeFiles
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
	property System::Boolean ShowSpeakingFiles
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
	property System::Boolean ShowSilentFiles
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
	property System::Boolean VerifyFileVersion
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
	property System::String^ DefaultFilePath
	{
		System::String^ get ()
		{
			return mCharacterFiles ? mCharacterFiles->DefaultFilePath : System::String::Empty;
		}
	}
	property System::String^ DefaultSearchPath
	{
		System::String^ get ()
		{
			return mCharacterFiles ? mCharacterFiles->DefaultSearchPath : System::String::Empty;
		}
	}
	property System::String^ CurrentSearchPath
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
	property cli::array<String^>^ FilePaths
	{
		cli::array<String^>^ get ()
		{
			return mCharacterFiles ? mCharacterFiles->FilePaths : gcnew cli::array<String^>(0);
		}
	}
};
#pragma endregion

/////////////////////////////////////////////////////////////////////////////

#pragma region TTSEnginesPageData
public ref class TTSEnginesPageData : public FormDataBinding
{
protected:
	DoubleAgent::Control::TTSEngines^	mTTSEngines;
public:
	property System::Boolean Bound
	{
		virtual System::Boolean get () override
		{
			return !!mTTSEngines;
		}
		virtual System::Void set (System::Boolean value) override
		{
			if	(!value)
			{
				mTTSEngines = nullptr;
			}
			else
			if	(!mTTSEngines)
			{
				mTTSEngines = mControl->TTSEngines;
			}
			if	(mBindingSource)
			{
				if	(mTTSEngines)
				{
					mBindingSource->DataSource = gcnew Generic::List<DoubleAgent::Control::TTSEngine^> (mTTSEngines);
				}
				else
				{
					mBindingSource->DataSource = DoubleAgent::Control::TTSEngine::typeid;
				}
				mBindingSource->ResetCurrentItem ();
			}
			BoundChanged::raise (this, gcnew System::EventArgs);
		}
	}
};
#pragma endregion

/////////////////////////////////////////////////////////////////////////////

#pragma region SREnginesPageData
public ref class SREnginesPageData : public FormDataBinding
{
protected:
	DoubleAgent::Control::SREngines^	mSREngines;
public:
	property System::Boolean Bound
	{
		virtual System::Boolean get () override
		{
			return !!mSREngines;
		}
		virtual System::Void set (System::Boolean value) override
		{
			if	(!value)
			{
				mSREngines = nullptr;
			}
			else
			if	(!mSREngines)
			{
				mSREngines =  mControl->SREngines;
			}
			if	(mBindingSource)
			{
				if	(mSREngines)
				{
					mBindingSource->DataSource = gcnew Generic::List<DoubleAgent::Control::SREngine^> (mSREngines);
				}
				else
				{
					mBindingSource->DataSource = DoubleAgent::Control::SREngine::typeid;
				}
				mBindingSource->ResetCurrentItem ();
			}
			BoundChanged::raise (this, gcnew System::EventArgs);
		}
	}
};
#pragma endregion

/////////////////////////////////////////////////////////////////////////////
} // namespace DoubleAgent