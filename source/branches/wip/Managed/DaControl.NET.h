/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
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
#include "DaControl.NET.Events.h"

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
namespace Control {
/////////////////////////////////////////////////////////////////////////////

[System::Windows::Forms::AxHost::ClsidAttribute("{1147E530-A208-11DE-ABF2-002421116FB2}")]
[System::ComponentModel::DesignTimeVisibleAttribute(true)]
[System::ComponentModel::DefaultEvent("RequestComplete")]
public ref class AxControl :
	public System::Windows::Forms::AxHost
{
public:
	AxControl (void);
protected:
	~AxControl ();

public:        
	[System::ComponentModel::Browsable(true)]
	[System::ComponentModel::EditorBrowsable(EditorBrowsableState::Always)]
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
	[System::ComponentModel::EditorBrowsable(EditorBrowsableState::Always)]
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
	[System::ComponentModel::EditorBrowsable(EditorBrowsableState::Always)]
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
	[System::ComponentModel::EditorBrowsable(EditorBrowsableState::Always)]
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
	[System::ComponentModel::EditorBrowsable(EditorBrowsableState::Always)]
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
	[System::ComponentModel::EditorBrowsable(EditorBrowsableState::Always)]
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
	
	//[System::ComponentModel::Browsable(true)]
	//[System::ComponentModel::EditorBrowsable(EditorBrowsableState::Always)]
	//[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Visible)]
	//[System::ComponentModel::Bindable(System::ComponentModel::BindableSupport::No)]
	//[System::ComponentModel::CategoryAttribute("Appearance")]
	//[System::Runtime::InteropServices::DispIdAttribute(-521)]
	//property System::Int32 CursorType
	//{
	//	System::Int32 get ();
	//	void set (System::Int32 value);
	//}
	
	[System::ComponentModel::Browsable(true)]
	[System::ComponentModel::EditorBrowsable(EditorBrowsableState::Always)]
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
	[System::ComponentModel::EditorBrowsable(EditorBrowsableState::Never)]
    [System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
	[System::ComponentModel::CategoryAttribute("Behavior")]
    property virtual bool Connected
    {
        bool get () sealed;
        void set (bool value) sealed;
    }
    
    [System::ComponentModel::Browsable(true)]
	[System::ComponentModel::EditorBrowsable(EditorBrowsableState::Always)]
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
	[System::ComponentModel::EditorBrowsable(EditorBrowsableState::Never)]
    [System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
    property virtual unsigned long CharacterStyle
    {
        unsigned long get () sealed;
        void set (unsigned long value) sealed;
    }

public:        
    [System::ComponentModel::Browsable(false)]
	[System::ComponentModel::EditorBrowsable(EditorBrowsableState::Never)]
	[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
	property DoubleAgent::Control::ICharacters^ Characters
	{
		DoubleAgent::Control::ICharacters^ get ();
	}

	[System::ComponentModel::Browsable(false)]
	[System::ComponentModel::EditorBrowsable(EditorBrowsableState::Never)]
	[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
//	[System::Diagnostics::DebuggerBrowsableAttribute(System::Diagnostics::DebuggerBrowsableState::Never)]
	[System::Diagnostics::DebuggerHiddenAttribute()]
	property DoubleAgent::Control::IAudioOutput^ AudioOutput
	{
		[System::Runtime::InteropServices::TypeLibFuncAttribute(System::Runtime::InteropServices::TypeLibFuncFlags::FHidden|System::Runtime::InteropServices::TypeLibFuncFlags::FRestricted|System::Runtime::InteropServices::TypeLibFuncFlags::FNonBrowsable)]
		DoubleAgent::Control::IAudioOutput^ get ();
	}
        
    [System::ComponentModel::Browsable(false)]
	[System::ComponentModel::EditorBrowsable(EditorBrowsableState::Never)]
    [System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
	[System::Diagnostics::DebuggerHiddenAttribute()]
    property DoubleAgent::Control::ISpeechInput^ SpeechInput
    {
		[System::Runtime::InteropServices::TypeLibFuncAttribute(System::Runtime::InteropServices::TypeLibFuncFlags::FHidden|System::Runtime::InteropServices::TypeLibFuncFlags::FRestricted|System::Runtime::InteropServices::TypeLibFuncFlags::FNonBrowsable)]
        DoubleAgent::Control::ISpeechInput^ get ();
    }
    
    [System::ComponentModel::Browsable(false)]
	[System::ComponentModel::EditorBrowsable(EditorBrowsableState::Never)]
    [System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
    property DoubleAgent::Control::IPropertySheet^ PropertySheet
    {
        DoubleAgent::Control::IPropertySheet^ get ();
    }
    
    [System::ComponentModel::Browsable(false)]
	[System::ComponentModel::EditorBrowsable(EditorBrowsableState::Never)]
    [System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
    property DoubleAgent::Control::ICommandsWindow^ CommandsWindow
    {
        DoubleAgent::Control::ICommandsWindow^ get ();
    }
    
    [System::ComponentModel::Browsable(false)]
	[System::ComponentModel::EditorBrowsable(EditorBrowsableState::Never)]
    [System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
    property virtual DoubleAgent::Control::ICharacterFiles^ CharacterFiles
    {
        DoubleAgent::Control::ICharacterFiles^ get () sealed;
    }
    
    [System::ComponentModel::Browsable(false)]
	[System::ComponentModel::EditorBrowsable(EditorBrowsableState::Never)]
    [System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
    property virtual DoubleAgent::Control::ITTSEngines^ TTSEngines
    {
        DoubleAgent::Control::ITTSEngines^ get () sealed;
    }
    
    [System::ComponentModel::Browsable(false)]
	[System::ComponentModel::EditorBrowsable(EditorBrowsableState::Never)]
    [System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
    property virtual DoubleAgent::Control::ISREngines^ SREngines
    {
        DoubleAgent::Control::ISREngines^ get () sealed;
    }
    
    [System::ComponentModel::Browsable(false)]
	[System::ComponentModel::EditorBrowsable(EditorBrowsableState::Never)]
    [System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
    property virtual DoubleAgent::Control::ISettings^ Settings
    {
        DoubleAgent::Control::ISettings^ get () sealed;
    }

public:        
    virtual DoubleAgent::Control::ITTSEngines^ FindTTSEngines (Object^ LanguageID, Object^ Gender) sealed;
    virtual DoubleAgent::Control::ITTSEngine^ GetCharacterTTSEngine (Object^ LoadKey) sealed;
    virtual DoubleAgent::Control::ITTSEngines^ FindCharacterTTSEngines (Object^ LoadKey, Object^ LanguageID) sealed;
    virtual DoubleAgent::Control::ISREngines^ FindSREngines (Object^ LanguageID) sealed;
    virtual DoubleAgent::Control::ISREngine^ GetCharacterSREngine (Object^ LoadKey) sealed;
    virtual DoubleAgent::Control::ISREngines^ FindCharacterSREngines (Object^ LoadKey, Object^ LanguageID) sealed;
	[System::Runtime::InteropServices::TypeLibFuncAttribute(System::Runtime::InteropServices::TypeLibFuncFlags::FHidden)]
    virtual void ShowDefaultCharacterProperties (Object^ X, Object^ Y) sealed;
	[System::Runtime::InteropServices::TypeLibFuncAttribute(System::Runtime::InteropServices::TypeLibFuncFlags::FHidden)]
    virtual void ShowDefaultCharacterProperties () sealed;

public:        
	delegate void ActivateInputEventHandler (Object^ sender, DoubleAgent::Control::Events::ActivateInputEvent^ e);
	delegate void DeactivateInputEventHandler (Object^ sender, DoubleAgent::Control::Events::DeactivateInputEvent^ e);
	delegate void ClickEventHandler (Object^ sender, DoubleAgent::Control::Events::ClickEvent^ e);
	delegate void DblClickEventHandler (Object^ sender, DoubleAgent::Control::Events::DblClickEvent^ e);
	delegate void DragStartEventHandler (Object^ sender, DoubleAgent::Control::Events::DragStartEvent^ e);
	delegate void DragCompleteEventHandler (Object^ sender, DoubleAgent::Control::Events::DragCompleteEvent^ e);
	delegate void ShowEventHandler (Object^ sender, DoubleAgent::Control::Events::ShowEvent^ e);
	delegate void HideEventHandler (Object^ sender, DoubleAgent::Control::Events::HideEvent^ e);
	delegate void RequestStartEventHandler (Object^ sender, DoubleAgent::Control::Events::RequestStartEvent^ e);
	delegate void RequestCompleteEventHandler (Object^ sender, DoubleAgent::Control::Events::RequestCompleteEvent^ e);
	delegate void BookmarkEventHandler (Object^ sender, DoubleAgent::Control::Events::BookmarkEvent^ e);
	delegate void CommandEventHandler (Object^ sender, DoubleAgent::Control::Events::CommandEvent^ e);
	delegate void IdleStartEventHandler (Object^ sender, DoubleAgent::Control::Events::IdleStartEvent^ e);
	delegate void IdleCompleteEventHandler (Object^ sender, DoubleAgent::Control::Events::IdleCompleteEvent^ e);
	delegate void MoveEventHandler (Object^ sender, DoubleAgent::Control::Events::MoveEvent^ e);
	delegate void SizeEventHandler (Object^ sender, DoubleAgent::Control::Events::SizeEvent^ e);
	delegate void BalloonShowEventHandler (Object^ sender, DoubleAgent::Control::Events::BalloonShowEvent^ e);
	delegate void BalloonHideEventHandler (Object^ sender, DoubleAgent::Control::Events::BalloonHideEvent^ e);
	delegate void ListenStartEventHandler (Object^ sender, DoubleAgent::Control::Events::ListenStartEvent^ e);
	delegate void ListenCompleteEventHandler (Object^ sender, DoubleAgent::Control::Events::ListenCompleteEvent^ e);
	delegate void DefaultCharacterChangeEventHandler (Object^ sender, DoubleAgent::Control::Events::DefaultCharacterChangeEvent^ e);
	delegate void ActiveClientChangeEventHandler (Object^ sender, DoubleAgent::Control::Events::ActiveClientChangeEvent^ e);

public:        
	event ActivateInputEventHandler^ PopupActivateInput
	{
		void add (ActivateInputEventHandler^ pDelegate);
		void remove (ActivateInputEventHandler^ pDelegate);
		internal: void raise (Object^ pSender, DoubleAgent::Control::Events::ActivateInputEvent^ pEvent);
	}
	
	event DeactivateInputEventHandler^ PopupDeactivateInput
	{
		void add (DeactivateInputEventHandler^ pDelegate);
		void remove (DeactivateInputEventHandler^ pDelegate);
		internal: void raise (Object^ pSender, DoubleAgent::Control::Events::DeactivateInputEvent^ pEvent);
	}
	
	event ClickEventHandler^ PopupClick
	{
		void add (ClickEventHandler^ pDelegate);
		void remove (ClickEventHandler^ pDelegate);
		internal: void raise (Object^ pSender, DoubleAgent::Control::Events::ClickEvent^ pEvent);
	}
	
	event DblClickEventHandler^ PopupDblClick
	{
		void add (DblClickEventHandler^ pDelegate);
		void remove (DblClickEventHandler^ pDelegate);
		internal: void raise (Object^ pSender, DoubleAgent::Control::Events::DblClickEvent^ pEvent);
	}
	
	event DragStartEventHandler^ PopupDragStart
	{
		void add (DragStartEventHandler^ pDelegate);
		void remove (DragStartEventHandler^ pDelegate);
		internal: void raise (Object^ pSender, DoubleAgent::Control::Events::DragStartEvent^ pEvent);
	}
	
	event DragCompleteEventHandler^ PopupDragComplete
	{
		void add (DragCompleteEventHandler^ pDelegate);
		void remove (DragCompleteEventHandler^ pDelegate);
		internal: void raise (Object^ pSender, DoubleAgent::Control::Events::DragCompleteEvent^ pEvent);
	}
	
	event ShowEventHandler^ PopupShow
	{
		void add (ShowEventHandler^ pDelegate);
		void remove (ShowEventHandler^ pDelegate);
		internal: void raise (Object^ pSender, DoubleAgent::Control::Events::ShowEvent^ pEvent);
	}
	
	event HideEventHandler^ PopupHide
	{
		void add (HideEventHandler^ pDelegate);
		void remove (HideEventHandler^ pDelegate);
		internal: void raise (Object^ pSender, DoubleAgent::Control::Events::HideEvent^ pEvent);
	}
	
	event RequestStartEventHandler^ RequestStart
	{
		void add (RequestStartEventHandler^ pDelegate);
		void remove (RequestStartEventHandler^ pDelegate);
		internal: void raise (Object^ pSender, DoubleAgent::Control::Events::RequestStartEvent^ pEvent);
	}
	
	event RequestCompleteEventHandler^ RequestComplete
	{
		void add (RequestCompleteEventHandler^ pDelegate);
		void remove (RequestCompleteEventHandler^ pDelegate);
		internal: void raise (Object^ pSender, DoubleAgent::Control::Events::RequestCompleteEvent^ pEvent);
	}
	
	event BookmarkEventHandler^ Bookmark
	{
		void add (BookmarkEventHandler^ pDelegate);
		void remove (BookmarkEventHandler^ pDelegate);
		internal: void raise (Object^ pSender, DoubleAgent::Control::Events::BookmarkEvent^ pEvent);
	}
	
	event CommandEventHandler^ Command
	{
		void add (CommandEventHandler^ pDelegate);
		void remove (CommandEventHandler^ pDelegate);
		internal: void raise (Object^ pSender, DoubleAgent::Control::Events::CommandEvent^ pEvent);
	}
	
	event IdleStartEventHandler^ IdleStart
	{
		void add (IdleStartEventHandler^ pDelegate);
		void remove (IdleStartEventHandler^ pDelegate);
		internal: void raise (Object^ pSender, DoubleAgent::Control::Events::IdleStartEvent^ pEvent);
	}
	
	event IdleCompleteEventHandler^ IdleComplete
	{
		void add (IdleCompleteEventHandler^ pDelegate);
		void remove (IdleCompleteEventHandler^ pDelegate);
		internal: void raise (Object^ pSender, DoubleAgent::Control::Events::IdleCompleteEvent^ pEvent);
	}
	
	event MoveEventHandler^ PopupMove
	{
		void add (MoveEventHandler^ pDelegate);
		void remove (MoveEventHandler^ pDelegate);
		internal: void raise (Object^ pSender, DoubleAgent::Control::Events::MoveEvent^ pEvent);
	}
	
	event SizeEventHandler^ PopupSize
	{
		void add (SizeEventHandler^ pDelegate);
		void remove (SizeEventHandler^ pDelegate);
		internal: void raise (Object^ pSender, DoubleAgent::Control::Events::SizeEvent^ pEvent);
	}
	
	event BalloonShowEventHandler^ BalloonShow
	{
		void add (BalloonShowEventHandler^ pDelegate);
		void remove (BalloonShowEventHandler^ pDelegate);
		internal: void raise (Object^ pSender, DoubleAgent::Control::Events::BalloonShowEvent^ pEvent);
	}
	
	event BalloonHideEventHandler^ BalloonHide
	{
		void add (BalloonHideEventHandler^ pDelegate);
		void remove (BalloonHideEventHandler^ pDelegate);
		internal: void raise (Object^ pSender, DoubleAgent::Control::Events::BalloonHideEvent^ pEvent);
	}
	
	event ListenStartEventHandler^ ListenStart
	{
		void add (ListenStartEventHandler^ pDelegate);
		void remove (ListenStartEventHandler^ pDelegate);
		internal: void raise (Object^ pSender, DoubleAgent::Control::Events::ListenStartEvent^ pEvent);
	}
	
	event ListenCompleteEventHandler^ ListenComplete
	{
		void add (ListenCompleteEventHandler^ pDelegate);
		void remove (ListenCompleteEventHandler^ pDelegate);
		internal: void raise (Object^ pSender, DoubleAgent::Control::Events::ListenCompleteEvent^ pEvent);
	}
	
	event DefaultCharacterChangeEventHandler^ DefaultCharacterChange
	{
		void add (DefaultCharacterChangeEventHandler^ pDelegate);
		void remove (DefaultCharacterChangeEventHandler^ pDelegate);
		internal: void raise (Object^ pSender, DoubleAgent::Control::Events::DefaultCharacterChangeEvent^ pEvent);
	}
	
	event System::EventHandler^ AgentPropertyChange
	{
		void add (System::EventHandler^ pDelegate);
		void remove (System::EventHandler^ pDelegate);
		internal: void raise (Object^ pSender, System::EventArgs^ pEvent);
	}
	
	event ActiveClientChangeEventHandler^ ActiveClientChange
	{
		void add (ActiveClientChangeEventHandler^ pDelegate);
		void remove (ActiveClientChangeEventHandler^ pDelegate);
		internal: void raise (Object^ pSender, DoubleAgent::Control::Events::ActiveClientChangeEvent^ pEvent);
	}

private:        
	ActivateInputEventHandler^			mActivateInputEvent;
	DeactivateInputEventHandler^		mDeactivateInputEvent;
	ClickEventHandler^					mClickEvent;
	DblClickEventHandler^				mDblClickEvent;
	DragStartEventHandler^				mDragStartEvent;
	DragCompleteEventHandler^			mDragCompleteEvent;
	ShowEventHandler^					mShowEvent;
	HideEventHandler^					mHideEvent;
	RequestStartEventHandler^			mRequestStartEvent;
	RequestCompleteEventHandler^		mRequestCompleteEvent;
	BookmarkEventHandler^				mBookmarkEvent;
	CommandEventHandler^				mCommandEvent;
	IdleStartEventHandler^				mIdleStartEvent;
	IdleCompleteEventHandler^			mIdleCompleteEvent;
	MoveEventHandler^					mMoveEvent;
	SizeEventHandler^					mSizeEvent;
	BalloonShowEventHandler^			mBalloonShowEvent;
	BalloonHideEventHandler^			mBalloonHideEvent;
	ListenStartEventHandler^			mListenStartEvent;
	ListenCompleteEventHandler^			mListenCompleteEvent;
	DefaultCharacterChangeEventHandler^	mDefaultCharacterChangeEvent;
	System::EventHandler^				mAgentPropertyChangeEvent;
	ActiveClientChangeEventHandler^		mActiveClientChangeEvent;

protected:
	virtual void AttachInterfaces () override sealed;
	virtual void CreateSink () override sealed;
	virtual void DetachSink () override sealed;

public:
    DoubleAgent::Control::IControl^	mControl;
private:
    AxControlEvents^				mEventMulticaster;
    AxHost::ConnectionPointCookie^	mEventCookie;
};

/////////////////////////////////////////////////////////////////////////////
}	// namespace Control
}	// namespace DoubleAgent