/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent ActiveX Control.

    The Double Agent ActiveX Control is free software:
    you can redistribute it and/or modify it under the terms of the
    GNU Lesser Public License as published by the Free Software Foundation,
    either version 3 of the License, or (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser Public License for more details.

    You should have received a copy of the GNU Lesser Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#pragma once

using namespace System;
using namespace System::Reflection;
using namespace System::Runtime::CompilerServices;
using namespace System::Runtime::InteropServices;
using namespace System::Security::Permissions;
using namespace System::ComponentModel;
using namespace System::Windows::Forms;
using namespace System::Collections;

/////////////////////////////////////////////////////////////////////////////

[assembly:AssemblyTitleAttribute ("DaControl.NET")];
[assembly:AssemblyDescriptionAttribute (_CONTROL_PROGID_NAME)];
[assembly:AssemblyConfigurationAttribute ("")];
[assembly:AssemblyCompanyAttribute (_DOUBLEAGENT_COMPANY)];
[assembly:AssemblyProductAttribute (_DOUBLEAGENT_NAME)];
[assembly:AssemblyCopyrightAttribute (_DOUBLEAGENT_COPYRIGHT)];
[assembly:AssemblyVersionAttribute (_DOUBLEAGENT_VERSION_STR)];

[assembly:CLSCompliantAttribute(true)];
[assembly:RuntimeCompatibilityAttribute(WrapNonExceptionThrows = true)];
[assembly:SecurityPermission(SecurityAction::RequestMinimum, UnmanagedCode = true)];
[assembly:PrimaryInteropAssembly(_CONTROL_VER_MAJOR,_CONTROL_VER_MINOR)];

/////////////////////////////////////////////////////////////////////////////

namespace DoubleAgent {
namespace AxControl {
/////////////////////////////////////////////////////////////////////////////

[System::Windows::Forms::AxHost::ClsidAttribute("{1147E530-A208-11DE-ABF2-002421116FB2}")]
[System::ComponentModel::DesignTimeVisibleAttribute(true)]
[System::ComponentModel::DefaultEvent("RequestComplete")]
#ifndef	_DEBUG
[System::Diagnostics::DebuggerNonUserCodeAttribute()]
#endif
public ref class AxControl :
	public System::Windows::Forms::AxHost
{
public:
	AxControl (void);
protected:
	~AxControl ();

//===========================================================================

public:
		[System::ComponentModel::Browsable(true)]
		[System::ComponentModel::EditorBrowsable(System::ComponentModel::EditorBrowsableState::Always)]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Visible)]
		[System::ComponentModel::Bindable(System::ComponentModel::BindableSupport::No)]
		[System::ComponentModel::CategoryAttribute("Layout")]
		[System::Runtime::InteropServices::DispIdAttribute(-500)]
	property virtual bool AutoSize
	{
		virtual bool get () new sealed;
		virtual void set (bool value) new sealed;
	}

		[System::ComponentModel::Browsable(true)]
		[System::ComponentModel::EditorBrowsable(System::ComponentModel::EditorBrowsableState::Always)]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Visible)]
		[System::ComponentModel::Bindable(System::ComponentModel::BindableSupport::No)]
		[System::ComponentModel::CategoryAttribute("Appearance")]
		[System::Runtime::InteropServices::DispIdAttribute(-501)]
	property virtual System::Drawing::Color BackColor
	{
		virtual System::Drawing::Color get () new sealed;
		virtual void set (System::Drawing::Color value) new sealed;
	}
	static property System::Drawing::Color DefaultBackColor
	{
		System::Drawing::Color get ();
	}

		[System::ComponentModel::Browsable(true)]
		[System::ComponentModel::EditorBrowsable(System::ComponentModel::EditorBrowsableState::Always)]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Visible)]
		[System::ComponentModel::Bindable(System::ComponentModel::BindableSupport::No)]
		[System::ComponentModel::CategoryAttribute("Appearance")]
		[System::Runtime::InteropServices::DispIdAttribute(-503)]
	property System::Drawing::Color BorderColor
	{
		System::Drawing::Color get ();
		void set (System::Drawing::Color value);
	}
	static property System::Drawing::Color DefaultBorderColor
	{
		System::Drawing::Color get ();
	}

		[System::ComponentModel::Browsable(true)]
		[System::ComponentModel::EditorBrowsable(System::ComponentModel::EditorBrowsableState::Always)]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Visible)]
		[System::ComponentModel::Bindable(System::ComponentModel::BindableSupport::No)]
		[System::ComponentModel::CategoryAttribute("Appearance")]
		[System::Runtime::InteropServices::DispIdAttribute(-504)]
	property System::Int32 BorderStyle
	{
		System::Int32 get ();
		void set (System::Int32 value);
	}
	static property System::Int32 DefaultBorderStyle
	{
		System::Int32 get ();
	}

		[System::ComponentModel::Browsable(true)]
		[System::ComponentModel::EditorBrowsable(System::ComponentModel::EditorBrowsableState::Always)]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Visible)]
		[System::ComponentModel::Bindable(System::ComponentModel::BindableSupport::No)]
		[System::ComponentModel::CategoryAttribute("Appearance")]
		[System::Runtime::InteropServices::DispIdAttribute(-505)]
	property System::Int32 BorderWidth
	{
		System::Int32 get ();
		void set (System::Int32 value);
	}
	static property System::Int32 DefaultBorderWidth
	{
		System::Int32 get ();
	}

		[System::ComponentModel::Browsable(true)]
		[System::ComponentModel::EditorBrowsable(System::ComponentModel::EditorBrowsableState::Always)]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Visible)]
		[System::ComponentModel::Bindable(System::ComponentModel::BindableSupport::No)]
		[System::ComponentModel::CategoryAttribute("Appearance")]
		[System::Runtime::InteropServices::DispIdAttribute(-519)]
	property System::Boolean BorderVisible
	{
		System::Boolean get ();
		void set (System::Boolean value);
	}
	static property System::Boolean DefaultBorderVisible
	{
		System::Boolean get ();
	}

	//	[System::ComponentModel::Browsable(true)]
	//	[System::ComponentModel::EditorBrowsable(System::ComponentModel::EditorBrowsableState::Always)]
	//	[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Visible)]
	//	[System::ComponentModel::Bindable(System::ComponentModel::BindableSupport::No)]
	//	[System::ComponentModel::CategoryAttribute("Appearance")]
	//	[System::Runtime::InteropServices::DispIdAttribute(-521)]
	//property System::Int32 CursorType
	//{
	//	System::Int32 get ();
	//	void set (System::Int32 value);
	//}

		[System::ComponentModel::Browsable(true)]
		[System::ComponentModel::EditorBrowsable(System::ComponentModel::EditorBrowsableState::Always)]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Visible)]
		[System::ComponentModel::Bindable(System::ComponentModel::BindableSupport::No)]
		[System::ComponentModel::CategoryAttribute("Appearance")]
		[System::Runtime::InteropServices::DispIdAttribute(-522)]
	property virtual System::Windows::Forms::Cursor^ Cursor
	{
		virtual System::Windows::Forms::Cursor^ get () new sealed;
		virtual void set (System::Windows::Forms::Cursor^ value) new sealed;
	}

public:
		[System::ComponentModel::Browsable(true)]
		[System::ComponentModel::EditorBrowsable(System::ComponentModel::EditorBrowsableState::Never)]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
		[System::ComponentModel::CategoryAttribute("Behavior")]
    property virtual bool Connected
    {
        bool get () sealed;
        void set (bool value) sealed;
    }

		[System::ComponentModel::Browsable(true)]
		[System::ComponentModel::EditorBrowsable(System::ComponentModel::EditorBrowsableState::Always)]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Visible)]
		[System::ComponentModel::Bindable(System::ComponentModel::BindableSupport::No)]
		[System::ComponentModel::CategoryAttribute("Behavior")]
		[System::Runtime::InteropServices::DispIdAttribute(41)]
    property virtual bool AutoConnect
    {
        bool get () sealed;
        void set (bool value) sealed;
    }

		[System::ComponentModel::Browsable(true)]
		[System::ComponentModel::EditorBrowsable(System::ComponentModel::EditorBrowsableState::Always)]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Visible)]
		[System::ComponentModel::Bindable(System::ComponentModel::BindableSupport::No)]
		[System::ComponentModel::CategoryAttribute("Behavior")]
		[System::Runtime::InteropServices::DispIdAttribute(21)]
    property virtual bool RaiseRequestErrors
    {
        bool get () sealed;
        void set (bool value) sealed;
    }

		[System::ComponentModel::Browsable(false)]
		[System::ComponentModel::EditorBrowsable(System::ComponentModel::EditorBrowsableState::Never)]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
    property virtual unsigned long CharacterStyle
    {
        unsigned long get () sealed;
        void set (unsigned long value) sealed;
    }

public:
		[System::ComponentModel::Browsable(false)]
		[System::ComponentModel::EditorBrowsable(System::ComponentModel::EditorBrowsableState::Never)]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
	property DoubleAgent::Control::Characters^ Characters
	{
		DoubleAgent::Control::Characters^ get ();
	}

		[System::ComponentModel::Browsable(false)]
		[System::ComponentModel::EditorBrowsable(System::ComponentModel::EditorBrowsableState::Never)]
		[System::Diagnostics::DebuggerHiddenAttribute()]
		[System::Diagnostics::DebuggerBrowsableAttribute(System::Diagnostics::DebuggerBrowsableState::Never)]
		[System::Diagnostics::DebuggerDisplayAttribute("deprecated")]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
		[System::Runtime::InteropServices::TypeLibVarAttribute(System::Runtime::InteropServices::TypeLibVarFlags::FHidden|System::Runtime::InteropServices::TypeLibVarFlags::FRestricted|System::Runtime::InteropServices::TypeLibVarFlags::FNonBrowsable)]
	property DoubleAgent::Control::AudioOutput^ AudioOutput
	{
			[System::Runtime::InteropServices::TypeLibFuncAttribute(System::Runtime::InteropServices::TypeLibFuncFlags::FHidden|System::Runtime::InteropServices::TypeLibFuncFlags::FRestricted|System::Runtime::InteropServices::TypeLibFuncFlags::FNonBrowsable)]
		DoubleAgent::Control::AudioOutput^ get ();
	}

		[System::ComponentModel::Browsable(false)]
		[System::ComponentModel::EditorBrowsable(System::ComponentModel::EditorBrowsableState::Never)]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
    property DoubleAgent::Control::SpeechInput^ SpeechInput
    {
			[System::Runtime::InteropServices::TypeLibFuncAttribute(System::Runtime::InteropServices::TypeLibFuncFlags::FHidden|System::Runtime::InteropServices::TypeLibFuncFlags::FRestricted|System::Runtime::InteropServices::TypeLibFuncFlags::FNonBrowsable)]
        DoubleAgent::Control::SpeechInput^ get ();
    }

		[System::ComponentModel::Browsable(false)]
		[System::ComponentModel::EditorBrowsable(System::ComponentModel::EditorBrowsableState::Never)]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
    property DoubleAgent::Control::PropertySheet^ PropertySheet
    {
        DoubleAgent::Control::PropertySheet^ get ();
    }

		[System::ComponentModel::Browsable(false)]
		[System::ComponentModel::EditorBrowsable(System::ComponentModel::EditorBrowsableState::Never)]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
    property DoubleAgent::Control::CommandsWindow^ CommandsWindow
    {
        DoubleAgent::Control::CommandsWindow^ get ();
    }

		[System::ComponentModel::Browsable(false)]
		[System::ComponentModel::EditorBrowsable(System::ComponentModel::EditorBrowsableState::Never)]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
    property virtual DoubleAgent::Control::CharacterFiles^ CharacterFiles
    {
        DoubleAgent::Control::CharacterFiles^ get () sealed;
    }

		[System::ComponentModel::Browsable(false)]
		[System::ComponentModel::EditorBrowsable(System::ComponentModel::EditorBrowsableState::Never)]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
    property virtual DoubleAgent::Control::TTSEngines^ TTSEngines
    {
        DoubleAgent::Control::TTSEngines^ get () sealed;
    }

		[System::ComponentModel::Browsable(false)]
		[System::ComponentModel::EditorBrowsable(System::ComponentModel::EditorBrowsableState::Never)]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
    property virtual DoubleAgent::Control::SREngines^ SREngines
    {
        DoubleAgent::Control::SREngines^ get () sealed;
    }

		[System::ComponentModel::Browsable(false)]
		[System::ComponentModel::EditorBrowsable(System::ComponentModel::EditorBrowsableState::Never)]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
    property virtual DoubleAgent::Control::Settings^ Settings
    {
        DoubleAgent::Control::Settings^ get () sealed;
    }

//===========================================================================

public:
    virtual DoubleAgent::Control::TTSEngines^ FindTTSEngines (Object^ LanguageID, Object^ Gender) sealed;
    virtual DoubleAgent::Control::TTSEngine^ GetCharacterTTSEngine (Object^ LoadKey) sealed;
    virtual DoubleAgent::Control::TTSEngines^ FindCharacterTTSEngines (Object^ LoadKey, Object^ LanguageID) sealed;
    virtual DoubleAgent::Control::SREngines^ FindSREngines (Object^ LanguageID) sealed;
    virtual DoubleAgent::Control::SREngine^ GetCharacterSREngine (Object^ LoadKey) sealed;
    virtual DoubleAgent::Control::SREngines^ FindCharacterSREngines (Object^ LoadKey, Object^ LanguageID) sealed;
		[System::Runtime::InteropServices::TypeLibFuncAttribute(System::Runtime::InteropServices::TypeLibFuncFlags::FHidden)]
    virtual void ShowDefaultCharacterProperties (Object^ X, Object^ Y) sealed;
		[System::Runtime::InteropServices::TypeLibFuncAttribute(System::Runtime::InteropServices::TypeLibFuncFlags::FHidden)]
    virtual void ShowDefaultCharacterProperties () sealed;

//===========================================================================

public:
	ref class AxPopupActivateInputEvent sealed
	{
	public:
		String^	CharacterID;

	internal:
		AxPopupActivateInputEvent (String^ pCharacterID)
		{
			CharacterID = pCharacterID;
		}
	};

	//-----------------------------------------------------------------------

	ref class AxPopupDeactivateInputEvent sealed
	{
	public:
		String^	CharacterID;

	internal:
		AxPopupDeactivateInputEvent (String^ pCharacterID)
		{
			CharacterID = pCharacterID;
		}
	};

	//-----------------------------------------------------------------------

	ref class AxPopupClickEvent sealed
	{
	public:
		String^	CharacterID;
		short	Button;
		short	Shift;
		short	X;
		short	Y;

	internal:
		AxPopupClickEvent (String^ pCharacterID, short pButton, short pShift, short pX, short pY)
		{
			CharacterID = pCharacterID;
			Button = pButton;
			Shift = pShift;
			X = pX;
			Y = pY;
		}
	};

	//-----------------------------------------------------------------------

	ref class AxPopupDblClickEvent sealed
	{
	public:
		String^	CharacterID;
		short	Button;
		short	Shift;
		short	X;
		short	Y;

	internal:
		AxPopupDblClickEvent (String^ pCharacterID, short pButton, short pShift, short pX, short pY)
		{
			CharacterID = pCharacterID;
			Button = pButton;
			Shift = pShift;
			X = pX;
			Y = pY;
		}
	};

	//-----------------------------------------------------------------------

	ref class AxPopupDragStartEvent sealed
	{
	public:
		String^	CharacterID;
		short	Button;
		short	Shift;
		short	X;
		short	Y;

	internal:
		AxPopupDragStartEvent (String^ pCharacterID, short pButton, short pShift, short pX, short pY)
		{
			CharacterID = pCharacterID;
			Button = pButton;
			Shift = pShift;
			X = pX;
			Y = pY;
		}
	};

	//-----------------------------------------------------------------------

	ref class AxPopupDragCompleteEvent sealed
	{
	public:
		String^	CharacterID;
		short	Button;
		short	Shift;
		short	X;
		short	Y;

	internal:
		AxPopupDragCompleteEvent (String^ pCharacterID, short pButton, short pShift, short pX, short pY)
		{
			CharacterID = pCharacterID;
			Button = pButton;
			Shift = pShift;
			X = pX;
			Y = pY;
		}
	};

	//-----------------------------------------------------------------------

	ref class AxShowEvent sealed
	{
	public:
		String^										CharacterID;
		DoubleAgent::Control::VisibilityCauseType	Cause;

	internal:
		AxShowEvent (String^ pCharacterID, DoubleAgent::Control::VisibilityCauseType pCause)
		{
			CharacterID = pCharacterID;
			Cause = pCause;
		}
	};

	//-----------------------------------------------------------------------

	ref class AxHideEvent sealed
	{
	public:
		String^										CharacterID;
		DoubleAgent::Control::VisibilityCauseType	Cause;

	internal:
		AxHideEvent (String^ pCharacterID, DoubleAgent::Control::VisibilityCauseType pCause)
		{
			CharacterID = pCharacterID;
			Cause = pCause;
		}
	};

	//-----------------------------------------------------------------------

	ref class AxRequestStartEvent sealed
	{
	public:
		DoubleAgent::Control::Request^	Request;

	internal:
		AxRequestStartEvent (DoubleAgent::Control::Request^ pRequest)
		{
			Request = pRequest;
		}
	};

	//-----------------------------------------------------------------------

	ref class AxRequestCompleteEvent sealed
	{
	public:
		DoubleAgent::Control::Request^	Request;

	internal:
		AxRequestCompleteEvent (DoubleAgent::Control::Request^ pRequest)
		{
			Request = pRequest;
		}
	};

	//-----------------------------------------------------------------------

	ref class AxBookmarkEvent sealed
	{
	public:
		int	BookmarkID;

	internal:
		AxBookmarkEvent (int pBookmarkID)
		{
			BookmarkID = pBookmarkID;
		}
	};

	//-----------------------------------------------------------------------

	ref class AxCommandEvent sealed
	{
	public:
		DoubleAgent::Control::UserInput^	UserInput;

	internal:
		AxCommandEvent (DoubleAgent::Control::UserInput^ pUserInput)
		{
			UserInput = pUserInput;
		}
	};

	//-----------------------------------------------------------------------

	ref class AxIdleStartEvent sealed
	{
	public:
		String^	CharacterID;

	internal:
		AxIdleStartEvent (String^ pCharacterID)
		{
			CharacterID = pCharacterID;
		}
	};

	//-----------------------------------------------------------------------

	ref class AxIdleCompleteEvent sealed
	{
	public:
		String^	CharacterID;

	internal:
		AxIdleCompleteEvent (String^ pCharacterID)
		{
			CharacterID = pCharacterID;
		}
	};

	//-----------------------------------------------------------------------

	ref class AxMoveEvent sealed
	{
	public:
		String^								CharacterID;
		short								X;
		short								Y;
		DoubleAgent::Control::MoveCauseType	Cause;

	internal:
		AxMoveEvent (String^ pCharacterID, short pX, short pY, DoubleAgent::Control::MoveCauseType pCause)
		{
			CharacterID = pCharacterID;
			X = pX;
			Y = pY;
			Cause = pCause;
		}
	};

	//-----------------------------------------------------------------------

	ref class AxSizeEvent sealed
	{
	public:
		String^	CharacterID;
		short	Width;
		short	Height;

	internal:
		AxSizeEvent (String^ pCharacterID, short pWidth, short pHeight)
		{
			CharacterID = pCharacterID;
			Width = pWidth;
			Height = pHeight;
		}
	};

	//-----------------------------------------------------------------------

	ref class AxBalloonShowEvent sealed
	{
	public:
		String^	CharacterID;

	internal:
		AxBalloonShowEvent (String^ pCharacterID)
		{
			CharacterID = pCharacterID;
		}
	};

	//-----------------------------------------------------------------------

	ref class AxBalloonHideEvent sealed
	{
	public:
		String^	CharacterID;

	internal:
		AxBalloonHideEvent (String^ pCharacterID)
		{
			CharacterID = pCharacterID;
		}
	};

	//-----------------------------------------------------------------------

	ref class AxListenStartEvent sealed
	{
	public:
		String^	CharacterID;

	internal:
		AxListenStartEvent (String^ pCharacterID)
		{
			CharacterID = pCharacterID;
		}
	};

	//-----------------------------------------------------------------------

	ref class AxListenCompleteEvent sealed
	{
	public:
		String^										CharacterID;
		DoubleAgent::Control::ListenCompleteType	Cause;

	internal:
		AxListenCompleteEvent (String^ pCharacterID, DoubleAgent::Control::ListenCompleteType pCause)
		{
			CharacterID = pCharacterID;
			Cause = pCause;
		}
	};

	//-----------------------------------------------------------------------

	ref class AxDefaultCharacterChangeEvent sealed
	{
	public:
		String^	CharGUID;

	internal:
		AxDefaultCharacterChangeEvent (String^ pCharGUID)
		{
			CharGUID = pCharGUID;
		}
	};

	//-----------------------------------------------------------------------

	ref class AxActiveClientChangeEvent sealed
	{
	public:
		String^	CharacterID;
		bool	Active;

	internal:
		AxActiveClientChangeEvent (String^ pCharacterID, bool pActive)
		{
			CharacterID = pCharacterID;
			Active = pActive;
		}
	};

//===========================================================================

public:
	delegate void AxPopupActivateInputHandler (Object^ sender, AxPopupActivateInputEvent^ e);
	delegate void AxPopupDeactivateInputHandler (Object^ sender, AxPopupDeactivateInputEvent^ e);
	delegate void AxPopupClickHandler (Object^ sender, AxPopupClickEvent^ e);
	delegate void AxPopupDblClickHandler (Object^ sender, AxPopupDblClickEvent^ e);
	delegate void AxPopupDragStartHandler (Object^ sender, AxPopupDragStartEvent^ e);
	delegate void AxPopupDragCompleteHandler (Object^ sender, AxPopupDragCompleteEvent^ e);
	delegate void AxShowHandler (Object^ sender, AxShowEvent^ e);
	delegate void AxHideHandler (Object^ sender, AxHideEvent^ e);
	delegate void AxRequestStartHandler (Object^ sender, AxRequestStartEvent^ e);
	delegate void AxRequestCompleteHandler (Object^ sender, AxRequestCompleteEvent^ e);
	delegate void AxBookmarkHandler (Object^ sender, AxBookmarkEvent^ e);
	delegate void AxCommandHandler (Object^ sender, AxCommandEvent^ e);
	delegate void AxIdleStartHandler (Object^ sender, AxIdleStartEvent^ e);
	delegate void AxIdleCompleteHandler (Object^ sender, AxIdleCompleteEvent^ e);
	delegate void AxMoveHandler (Object^ sender, AxMoveEvent^ e);
	delegate void AxSizeHandler (Object^ sender, AxSizeEvent^ e);
	delegate void AxBalloonShowHandler (Object^ sender, AxBalloonShowEvent^ e);
	delegate void AxBalloonHideHandler (Object^ sender, AxBalloonHideEvent^ e);
	delegate void AxListenStartHandler (Object^ sender, AxListenStartEvent^ e);
	delegate void AxListenCompleteHandler (Object^ sender, AxListenCompleteEvent^ e);
	delegate void AxDefaultCharacterChangeHandler (Object^ sender, AxDefaultCharacterChangeEvent^ e);
	delegate void AxActiveClientChangeHandler (Object^ sender, AxActiveClientChangeEvent^ e);

//===========================================================================

public:
		[System::ComponentModel::CategoryAttribute("Focus")]
	event AxPopupActivateInputHandler^ PopupActivateInput
	{
		void add (AxPopupActivateInputHandler^ pDelegate);
		void remove (AxPopupActivateInputHandler^ pDelegate);
		internal: void raise (Object^ pSender, AxPopupActivateInputEvent^ pEvent);
	}

		[System::ComponentModel::CategoryAttribute("Focus")]
	event AxPopupDeactivateInputHandler^ PopupDeactivateInput
	{
		void add (AxPopupDeactivateInputHandler^ pDelegate);
		void remove (AxPopupDeactivateInputHandler^ pDelegate);
		internal: void raise (Object^ pSender, AxPopupDeactivateInputEvent^ pEvent);
	}

		[System::ComponentModel::CategoryAttribute("Action")]
	event AxPopupClickHandler^ PopupClick
	{
		void add (AxPopupClickHandler^ pDelegate);
		void remove (AxPopupClickHandler^ pDelegate);
		internal: void raise (Object^ pSender, AxPopupClickEvent^ pEvent);
	}

		[System::ComponentModel::CategoryAttribute("Action")]
	event AxPopupDblClickHandler^ PopupDblClick
	{
		void add (AxPopupDblClickHandler^ pDelegate);
		void remove (AxPopupDblClickHandler^ pDelegate);
		internal: void raise (Object^ pSender, AxPopupDblClickEvent^ pEvent);
	}

		[System::ComponentModel::CategoryAttribute("Layout")]
	event AxPopupDragStartHandler^ PopupDragStart
	{
		void add (AxPopupDragStartHandler^ pDelegate);
		void remove (AxPopupDragStartHandler^ pDelegate);
		internal: void raise (Object^ pSender, AxPopupDragStartEvent^ pEvent);
	}

		[System::ComponentModel::CategoryAttribute("Layout")]
	event AxPopupDragCompleteHandler^ PopupDragComplete
	{
		void add (AxPopupDragCompleteHandler^ pDelegate);
		void remove (AxPopupDragCompleteHandler^ pDelegate);
		internal: void raise (Object^ pSender, AxPopupDragCompleteEvent^ pEvent);
	}

		[System::ComponentModel::CategoryAttribute("Action")]
	event AxShowHandler^ ShowEvent
	{
		void add (AxShowHandler^ pDelegate);
		void remove (AxShowHandler^ pDelegate);
		internal: void raise (Object^ pSender, AxShowEvent^ pEvent);
	}

		[System::ComponentModel::CategoryAttribute("Action")]
	event AxHideHandler^ HideEvent
	{
		void add (AxHideHandler^ pDelegate);
		void remove (AxHideHandler^ pDelegate);
		internal: void raise (Object^ pSender, AxHideEvent^ pEvent);
	}

		[System::ComponentModel::CategoryAttribute("Action")]
	event AxRequestStartHandler^ RequestStart
	{
		void add (AxRequestStartHandler^ pDelegate);
		void remove (AxRequestStartHandler^ pDelegate);
		internal: void raise (Object^ pSender, AxRequestStartEvent^ pEvent);
	}

		[System::ComponentModel::CategoryAttribute("Action")]
	event AxRequestCompleteHandler^ RequestComplete
	{
		void add (AxRequestCompleteHandler^ pDelegate);
		void remove (AxRequestCompleteHandler^ pDelegate);
		internal: void raise (Object^ pSender, AxRequestCompleteEvent^ pEvent);
	}

		[System::ComponentModel::CategoryAttribute("Action")]
	event AxBookmarkHandler^ Bookmark
	{
		void add (AxBookmarkHandler^ pDelegate);
		void remove (AxBookmarkHandler^ pDelegate);
		internal: void raise (Object^ pSender, AxBookmarkEvent^ pEvent);
	}

		[System::ComponentModel::CategoryAttribute("Action")]
	event AxCommandHandler^ Command
	{
		void add (AxCommandHandler^ pDelegate);
		void remove (AxCommandHandler^ pDelegate);
		internal: void raise (Object^ pSender, AxCommandEvent^ pEvent);
	}

		[System::ComponentModel::CategoryAttribute("Behavior")]
	event AxIdleStartHandler^ IdleStart
	{
		void add (AxIdleStartHandler^ pDelegate);
		void remove (AxIdleStartHandler^ pDelegate);
		internal: void raise (Object^ pSender, AxIdleStartEvent^ pEvent);
	}

		[System::ComponentModel::CategoryAttribute("Behavior")]
	event AxIdleCompleteHandler^ IdleComplete
	{
		void add (AxIdleCompleteHandler^ pDelegate);
		void remove (AxIdleCompleteHandler^ pDelegate);
		internal: void raise (Object^ pSender, AxIdleCompleteEvent^ pEvent);
	}

		[System::ComponentModel::CategoryAttribute("Layout")]
	event AxMoveHandler^ MoveEvent
	{
		void add (AxMoveHandler^ pDelegate);
		void remove (AxMoveHandler^ pDelegate);
		internal: void raise (Object^ pSender, AxMoveEvent^ pEvent);
	}

		[System::ComponentModel::CategoryAttribute("Layout")]
	event AxSizeHandler^ SizeEvent
	{
		void add (AxSizeHandler^ pDelegate);
		void remove (AxSizeHandler^ pDelegate);
		internal: void raise (Object^ pSender, AxSizeEvent^ pEvent);
	}

		[System::ComponentModel::CategoryAttribute("Action")]
	event AxBalloonShowHandler^ BalloonShow
	{
		void add (AxBalloonShowHandler^ pDelegate);
		void remove (AxBalloonShowHandler^ pDelegate);
		internal: void raise (Object^ pSender, AxBalloonShowEvent^ pEvent);
	}

		[System::ComponentModel::CategoryAttribute("Action")]
	event AxBalloonHideHandler^ BalloonHide
	{
		void add (AxBalloonHideHandler^ pDelegate);
		void remove (AxBalloonHideHandler^ pDelegate);
		internal: void raise (Object^ pSender, AxBalloonHideEvent^ pEvent);
	}

		[System::ComponentModel::CategoryAttribute("Action")]
	event AxListenStartHandler^ ListenStart
	{
		void add (AxListenStartHandler^ pDelegate);
		void remove (AxListenStartHandler^ pDelegate);
		internal: void raise (Object^ pSender, AxListenStartEvent^ pEvent);
	}

		[System::ComponentModel::CategoryAttribute("Action")]
	event AxListenCompleteHandler^ ListenComplete
	{
		void add (AxListenCompleteHandler^ pDelegate);
		void remove (AxListenCompleteHandler^ pDelegate);
		internal: void raise (Object^ pSender, AxListenCompleteEvent^ pEvent);
	}

		[System::ComponentModel::CategoryAttribute("Behavior")]
	event AxDefaultCharacterChangeHandler^ DefaultCharacterChange
	{
		void add (AxDefaultCharacterChangeHandler^ pDelegate);
		void remove (AxDefaultCharacterChangeHandler^ pDelegate);
		internal: void raise (Object^ pSender, AxDefaultCharacterChangeEvent^ pEvent);
	}

		[System::ComponentModel::CategoryAttribute("Behavior")]
	event System::EventHandler^ AgentPropertyChange
	{
		void add (System::EventHandler^ pDelegate);
		void remove (System::EventHandler^ pDelegate);
		internal: void raise (Object^ pSender, System::EventArgs^ pEvent);
	}

		[System::ComponentModel::CategoryAttribute("Behavior")]
	event AxActiveClientChangeHandler^ ActiveClientChange
	{
		void add (AxActiveClientChangeHandler^ pDelegate);
		void remove (AxActiveClientChangeHandler^ pDelegate);
		internal: void raise (Object^ pSender, AxActiveClientChangeEvent^ pEvent);
	}

//===========================================================================

private:
	AxControl::AxPopupActivateInputHandler^		mActivateInputEvent;
	AxControl::AxPopupDeactivateInputHandler^	mDeactivateInputEvent;
	AxControl::AxPopupClickHandler^				mClickEvent;
	AxControl::AxPopupDblClickHandler^			mDblClickEvent;
	AxControl::AxPopupDragStartHandler^			mDragStartEvent;
	AxControl::AxPopupDragCompleteHandler^		mDragCompleteEvent;
	AxControl::AxShowHandler^					mShowEvent;
	AxControl::AxHideHandler^					mHideEvent;
	AxControl::AxRequestStartHandler^			mRequestStartEvent;
	AxControl::AxRequestCompleteHandler^		mRequestCompleteEvent;
	AxControl::AxBookmarkHandler^				mBookmarkEvent;
	AxControl::AxCommandHandler^				mCommandEvent;
	AxControl::AxIdleStartHandler^				mIdleStartEvent;
	AxControl::AxIdleCompleteHandler^			mIdleCompleteEvent;
	AxControl::AxMoveHandler^					mMoveEvent;
	AxControl::AxSizeHandler^					mSizeEvent;
	AxControl::AxBalloonShowHandler^			mBalloonShowEvent;
	AxControl::AxBalloonHideHandler^			mBalloonHideEvent;
	AxControl::AxListenStartHandler^			mListenStartEvent;
	AxControl::AxListenCompleteHandler^			mListenCompleteEvent;
	AxControl::AxDefaultCharacterChangeHandler^	mDefaultCharacterChangeEvent;
	System::EventHandler^						mAgentPropertyChangeEvent;
	AxControl::AxActiveClientChangeHandler^		mActiveClientChangeEvent;

//===========================================================================

protected:
	virtual void AttachInterfaces () override sealed;
	virtual void CreateSink () override sealed;
	virtual void DetachSink () override sealed;

public:
    DoubleAgent::Control::IControl^	AxInterface;
private:
    ref class AxControlEvents^		mEventMulticaster;
    AxHost::ConnectionPointCookie^	mEventCookie;
};

/////////////////////////////////////////////////////////////////////////////

[System::Runtime::InteropServices::TypeLibTypeAttribute(System::Runtime::InteropServices::TypeLibTypeFlags::FHidden)]
[System::Runtime::InteropServices::ClassInterfaceAttribute(System::Runtime::InteropServices::ClassInterfaceType::None)]
#ifndef	_DEBUG
[System::Diagnostics::DebuggerNonUserCodeAttribute()]
#endif
public ref class AxControlEvents :
		public DoubleAgent::Control::Native::_DaCtlEvents
{
public:
	AxControlEvents (ref class AxControl^ pParent) : mParent (pParent) {}

public:
	virtual void ActivateInput (String^ CharacterID);
	virtual void DeactivateInput (String^ CharacterID);
	virtual void Click (String^ CharacterID, short Button, short Shift, short X, short Y);
	virtual void DblClick (String^ CharacterID, short Button, short Shift, short X, short Y);
	virtual void DragStart (String^ CharacterID, short Button, short Shift, short X, short Y);
	virtual void DragComplete (String^ CharacterID, short Button, short Shift, short X, short Y);
	virtual void Show (String^ CharacterID, DoubleAgent::Control::VisibilityCauseType Cause);
	virtual void Hide (String^ CharacterID, DoubleAgent::Control::VisibilityCauseType Cause);
	virtual void RequestStart (DoubleAgent::Control::Request^ Request);
	virtual void RequestComplete (DoubleAgent::Control::Request^ Request);
	virtual void Restart () {}
	virtual void Shutdown () {}
	virtual void Bookmark (int BookmarkID);
	virtual void Command (DoubleAgent::Control::UserInput^ UserInput);
	virtual void IdleStart (String^ CharacterID);
	virtual void IdleComplete (String^ CharacterID);
	virtual void Move (String^ CharacterID, short X, short Y, DoubleAgent::Control::MoveCauseType Cause);
	virtual void Size (String^ CharacterID, short Width, short Height);
	virtual void BalloonShow (String^ CharacterID);
	virtual void BalloonHide (String^ CharacterID);
	virtual void HelpComplete (String^ CharacterID, String^ Name, short Cause) {}
	virtual void ListenStart (String^ CharacterID);
	virtual void ListenComplete (String^ CharacterID, DoubleAgent::Control::ListenCompleteType Cause);
	virtual void DefaultCharacterChange (String^ CharGUID);
	virtual void AgentPropertyChange ();
	virtual void ActiveClientChange (String^ CharacterID, bool Active);

private:
	ref class AxControl^	mParent;
};

/////////////////////////////////////////////////////////////////////////////
}	// namespace AxControl
}	// namespace DoubleAgent