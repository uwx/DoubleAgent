#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace DoubleAgent::Control;

namespace DoubleAgent {
/////////////////////////////////////////////////////////////////////////////

#pragma region FormData
public ref class FormData :  public System::Object
{
public:
	FormData ()
	{
	}
	~FormData()
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
	DoubleAgent::AxControl::AxControl^	mControl;
	System::EventHandler^				mBoundChanged;
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

	EventData (DoubleAgent::AxControl::ActivateInputEvent^ e)
	:	mName ("ActivateInput"), mCharacterID (e->CharacterID), mData (String::Empty)
	{}

	EventData (DoubleAgent::AxControl::DeactivateInputEvent^ e)
	:	mName ("DeactivateInput"), mCharacterID (e->CharacterID), mData (String::Empty)
	{}

	EventData (DoubleAgent::AxControl::ShowEvent^ e)
	:	mName ("Show"), mCharacterID (e->CharacterID), mData (e->Cause.ToString())
	{}

	EventData (DoubleAgent::AxControl::HideEvent^ e)
	:	mName ("Hide"), mCharacterID (e->CharacterID), mData (e->Cause.ToString())
	{}

	EventData (DoubleAgent::AxControl::MoveEvent^ e)
	:	mName ("Move"), mCharacterID (e->CharacterID)
	{
		mData = String::Format ("{0:D}, {1:D} ({2})", e->x, e->y, e->Cause.ToString());
	}

	EventData (DoubleAgent::AxControl::SizeEvent^ e)
	:	mName ("Size"), mCharacterID (e->CharacterID)
	{
		mData = String::Format ("{0:D}, {1:D}", e->Width, e->Height);
	}

	EventData (DoubleAgent::AxControl::ClickEvent^ e)
	:	mName ("Click"), mCharacterID (e->CharacterID)
	{
		mData = String::Format ("{0:D}, {1:D} (button {2:D})", e->x, e->y, e->Button);
	}

	EventData (DoubleAgent::AxControl::DblClickEvent^ e)
	:	mName ("DblClick"), mCharacterID (e->CharacterID)
	{
		mData = String::Format ("{0:D}, {1:D} (button {2:D})", e->x, e->y, e->Button);
	}

	EventData (DoubleAgent::AxControl::DragStartEvent^ e)
	:	mName ("DragStart"), mCharacterID (e->CharacterID)
	{
		mData = String::Format ("{0:D}, {1:D} (button {2:D})", e->x, e->y, e->Button);
	}

	EventData (DoubleAgent::AxControl::DragCompleteEvent^ e)
	:	mName ("DragComplete"), mCharacterID (e->CharacterID)
	{
		mData = String::Format ("{0:D}, {1:D} (button {2:D})", e->x, e->y, e->Button);
	}

	EventData (DoubleAgent::AxControl::ListenStartEvent^ e)
	:	mName ("ListenStart"), mCharacterID (e->CharacterID), mData (String::Empty)
	{}

	EventData (DoubleAgent::AxControl::ListenCompleteEvent^ e)
	:	mName ("ListenComplete"), mCharacterID (e->CharacterID), mData (e->Cause.ToString())
	{}

	EventData (DoubleAgent::AxControl::IdleStartEvent^ e)
	:	mName ("IdleStart"), mCharacterID (e->CharacterID), mData (String::Empty)
	{}

	EventData (DoubleAgent::AxControl::IdleCompleteEvent^ e)
	:	mName ("IdleComplete"), mCharacterID (e->CharacterID), mData (String::Empty)
	{}

	EventData (DoubleAgent::AxControl::BalloonShowEvent^ e)
	:	mName ("BalloonShow"), mCharacterID (e->CharacterID), mData (String::Empty)
	{}

	EventData (DoubleAgent::AxControl::BalloonHideEvent^ e)
	:	mName ("BalloonHide"), mCharacterID (e->CharacterID), mData (String::Empty)
	{}

	EventData (DoubleAgent::AxControl::BookmarkEvent^ e)
	:	mName ("Bookmark"), mCharacterID (String::Empty), mData (e->BookmarkID.ToString())
	{}

	EventData (DoubleAgent::AxControl::CommandEvent^ e)
	:	mName ("Command"), mCharacterID (e->UserInput->CharacterID), mData (e->UserInput->Name)
	{}

	EventData (DoubleAgent::AxControl::RequestStartEvent^ e)
	:	mName ("RequestStart"), mCharacterID (e->Request->default.ToString()), mData (((DoubleAgent::Control::RequestStatus)e->Request->Status).ToString())
	{
	}

	EventData (DoubleAgent::AxControl::RequestCompleteEvent^ e)
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

	EventData (DoubleAgent::AxControl::ActiveClientChangeEvent^ e)
	:	mName ("ActiveClientChange"), mCharacterID (e->CharacterID), mData (String::Empty)
	{}

	EventData (DoubleAgent::AxControl::DefaultCharacterChangeEvent^ e)
	:	mName ("DefaultCharacterChange"), mCharacterID (String::Empty), mData (e->CharGUID)
	{}

protected:
	String^	mName;
	String^	mCharacterID;
	String^	mData;
};
#pragma endregion

/////////////////////////////////////////////////////////////////////////////

#pragma region CharacterData
public ref class CharacterData : public FormData
{
public:
	property System::Boolean Bound
	{
		virtual System::Boolean get () override
		{
			return !!Character;
		}
	}

	property DoubleAgent::Control::Character^ Character
	{
		DoubleAgent::Control::Character^ get ()
		{
			DoubleAgent::Control::Character^	lCharacter;

			if	(ReferenceEquals (mCharacterBindingSource->DataSource->GetType(), DoubleAgent::Control::Character::typeid))
			{
				lCharacter = safe_cast <DoubleAgent::Control::Character^> (mCharacterBindingSource->DataSource);
			}
			return lCharacter;
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

	property Specialized::StringCollection^ Animations
	{
		Specialized::StringCollection^ get ()
		{
			DoubleAgent::Control::Character^	lCharacter;
			Specialized::StringCollection^		lAnimations = gcnew Specialized::StringCollection;
			String^								lAnimationName;

			if	(lCharacter = Character)
			{
				for each (lAnimationName in lCharacter->AnimationNames)
				{
					lAnimations->Add (lAnimationName);
				}
			}
			return lAnimations;
		}
	}

	property Generic::List <DoubleAgent::Control::Command^>^ CommandList
	{
		Generic::List <DoubleAgent::Control::Command^>^ get ()
		{
			Generic::List <DoubleAgent::Control::Command^>^	lList = gcnew Generic::List <DoubleAgent::Control::Command^>;
			DoubleAgent::Control::Character^				lCharacter;
			DoubleAgent::Control::Commands^					lCommands;

			if	(
					(lCharacter = Character)
				&&	(lCommands = lCharacter->Commands)
				&&	(lCommands->Count > 0)
				)
			{
				try
				{
					for	(int lNdx = 0; lNdx < lCommands->Count; lNdx++)
					{
						lList->Add (lCommands->Index [lNdx]);
					}
				}
				catch (...)
				{}
			}
			return lList;
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

protected:
	System::Windows::Forms::BindingSource^  mCharacterBindingSource;
public:
	property System::Windows::Forms::BindingSource^ CharacterBinding
	{
		System::Windows::Forms::BindingSource^ get ()
		{
			return mCharacterBindingSource;
		}
		System::Void set (System::Windows::Forms::BindingSource^ value)
		{
			mCharacterBindingSource = value;
		}
	}

public:
	DoubleAgent::Control::Request^	mLastRequest;
};
#pragma endregion

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#pragma region CharactersData
public ref class CharactersData : public FormData
{
protected:
	DoubleAgent::Control::Characters^	mCharacters;
	String^								mCharacterUnloading;
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
			BoundChanged::raise (this, gcnew System::EventArgs);
		}
	}

	property System::String^ CharacterUnloading
	{
		System::String^ get ()
		{
			return mCharacterUnloading;
		}
		System::Void set (System::String^ value)
		{
			mCharacterUnloading = value;
		}
	}

	property Generic::List <DoubleAgent::Control::Character^>^ CharactersList
	{
		Generic::List <DoubleAgent::Control::Character^>^ get ()
		{
			Generic::List <DoubleAgent::Control::Character^>^	lList = gcnew Generic::List <DoubleAgent::Control::Character^>;
			if	(
					(mCharacters)
				&&	(mCharacters->Count > 0)
				)
			{
				try
				{
					for	(int lNdx = 0; lNdx < mCharacters->Count; lNdx++)
					{
						if	(mCharacters->Index [lNdx]->CharacterID != mCharacterUnloading)
						{
							lList->Add (mCharacters->Index [lNdx]);
						}
					}
				}
				catch (...)
				{}
			}
			return lList;
		}
	}
};
#pragma endregion

/////////////////////////////////////////////////////////////////////////////

#pragma region SettingsData
public ref class SettingsData : public FormData
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
			BoundChanged::raise (this, gcnew System::EventArgs);
		}
	}

	property System::Boolean BalloonEnabled
	{
		System::Boolean get ()
		{
			return mSettings ? mSettings->BalloonEnabled : false;
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
	property System::Boolean SoundEffectsEnabled
	{
		System::Boolean get ()
		{
			return mSettings ? mSettings->SoundEffectsEnabled : false;
		}
	}
	property System::Boolean TTSEnabled
	{
		System::Boolean get ()
		{
			return mSettings ? mSettings->TTSEnabled : false;
		}
	}
	property System::Int16 TTSSpeed
	{
		System::Int16 get ()
		{
			return mSettings ? mSettings->TTSSpeed : 0;
		}
	}
	property System::Boolean SREnabled
	{
		System::Boolean get ()
		{
			return mSettings ? mSettings->SREnabled : false;
		}
	}
	property System::String^ SRHotKey
	{
		System::String^ get ()
		{
			return mSettings ? mSettings->SRHotKey : String::Empty;
		}
	}
	property System::Int16 SRHotKeyTime
	{
		System::Int16 get ()
		{
			return mSettings ? mSettings->SRHotKeyTime : 0;
		}
	}
	property System::Boolean SRListeningPrompt
	{
		System::Boolean get ()
		{
			return mSettings ? mSettings->SRListeningPrompt : false;
		}
	}
	property System::Boolean SRListeningTip
	{
		System::Boolean get ()
		{
			return mSettings ? mSettings->SRListeningTip : false;
		}
	}
};
#pragma endregion

/////////////////////////////////////////////////////////////////////////////

#pragma region PropertySheetData
public ref class PropertySheetData : public FormData
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
			BoundChanged::raise (this, gcnew System::EventArgs);
		}
	}

	property System::Boolean Visible
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
	property System::Int16 Left
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
	property System::Int16 Top
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
	property System::Int16 Width
	{
		System::Int16 get ()
		{
			return mPropertySheet ? mPropertySheet->Width : 0;
		}
	}
	property System::Int16 Height
	{
		System::Int16 get ()
		{
			return mPropertySheet ? mPropertySheet->Height : 0;
		}
	}
	property System::String^ Page
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
};
#pragma endregion

/////////////////////////////////////////////////////////////////////////////

#pragma region CharacterFilesData
public ref class CharacterFilesData : public FormData
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

#pragma region TTSEnginesData
public ref class TTSEnginesData : public FormData
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
			BoundChanged::raise (this, gcnew System::EventArgs);
		}
	}

	property Generic::List <DoubleAgent::Control::TTSEngine^>^ TTSEnginesList
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
};
#pragma endregion

/////////////////////////////////////////////////////////////////////////////

#pragma region SREnginesData
public ref class SREnginesData : public FormData
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
				mSREngines = mControl->SREngines;
			}
			BoundChanged::raise (this, gcnew System::EventArgs);
		}
	}

	property Generic::List <DoubleAgent::Control::SREngine^>^ SREnginesList
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
};
#pragma endregion

/////////////////////////////////////////////////////////////////////////////
} // namespace DoubleAgent