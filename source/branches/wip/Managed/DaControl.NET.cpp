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
#include "StdAfx.h"
#include "DaControl.NET.h"

/////////////////////////////////////////////////////////////////////////////
#pragma managed(push,off)
#ifdef	_DEBUG
#define _LOG_LEVEL_DEBUG	LogNormal
#endif
#define	_LOG_ROOT_PATH		_T("Software\\")_T(_DOUBLEAGENT_NAME)_T("\\")
#define	_LOG_SECTION_NAME	_T(_CONTROL_REGNAME)
#define _LOG_DEF_LOGNAME	_T(_DOUBLEAGENT_NAME) _T(".log")
#define	_LOG_PREFIX			_T("CNet ")
#include "LogAccess.inl"
#include "Log.inl"
#pragma comment(lib, "advapi32.lib")
#pragma managed(pop)

#ifdef	_DEBUG
#define	_DEBUG_INTERNALS	LogNormal
//#define	_DEBUG_AFX_PROPS	LogNormal
//#define	_DEBUG_CTL_PROPS	LogNormal
//#define	_DEBUG_OBJ_PROPS	LogNormal
#endif

/////////////////////////////////////////////////////////////////////////////

namespace DoubleAgent {
namespace Control {
/////////////////////////////////////////////////////////////////////////////

AxControl::AxControl (void)
:	AxHost ("1147E530-A208-11DE-ABF2-002421116FB2")
{
	LogStart ();
#ifdef	_DEBUG_INTERNALS	
	LogMessage (_DEBUG_INTERNALS, _T("AxControl::AxControl"));
#endif	
}

AxControl::~AxControl (void)
{
#ifdef	_DEBUG_INTERNALS	
	LogMessage (_DEBUG_INTERNALS, _T("AxControl::~AxControl"));
#endif	
	LogStop (LogIfActive);
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::AttachInterfaces ()
{
#ifdef	_DEBUG_INTERNALS	
	LogMessage (_DEBUG_INTERNALS, _T("AxControl::AttachInterfaces"));
#endif	
	try
	{
		mControl = safe_cast <DoubleAgent::Control::IControl^> (GetOcx());
    }
	catch AnyExceptionDebug
	{}
}

void AxControl::CreateSink ()
{
#ifdef	_DEBUG_INTERNALS	
	LogMessage (_DEBUG_INTERNALS, _T("AxControl::CreateSink"));
#endif	
	try
	{
		mEventMulticaster = gcnew AxControlEvents (this);
		mEventCookie = gcnew AxHost::ConnectionPointCookie (mControl, mEventMulticaster, DoubleAgent::Control::Native::_DaCtlEvents::typeid);
    }
    catch AnyExceptionDebug
    {}
}

void AxControl::DetachSink ()
{
#ifdef	_DEBUG_INTERNALS	
	LogMessage (_DEBUG_INTERNALS, _T("AxControl::DetachSink"));
#endif	
	try
	{
		if	(mEventCookie)
		{
			mEventCookie->Disconnect ();
		}
	}
    catch AnyExceptionDebug
    {}
}

/////////////////////////////////////////////////////////////////////////////
//page
/////////////////////////////////////////////////////////////////////////////

bool AxControl::AutoSize::get ()
{
#ifdef	_DEBUG_AFX_PROPS
	LogMessage (_DEBUG_AFX_PROPS, _T("(%u) AxControl::AutoSize::get"), (mControl!=nullptr));
#endif
	if	(mControl != nullptr)
	{
		try
		{
			return mControl->AutoSize;
		}
		catch AnyExceptionDebug
	}
	return AxHost::AutoSize::get ();
}
void AxControl::AutoSize::set (bool value)
{
#ifdef	_DEBUG_AFX_PROPS
	LogMessage (_DEBUG_AFX_PROPS, _T("(%u) AxControl::AutoSize::set"), (mControl!=nullptr));
#endif
	if	(mControl != nullptr)
	{
		try
		{
			mControl->AutoSize = value;
		}
		catch AnyExceptionDebug
	}
	AxHost::AutoSize::set (value);
}

/////////////////////////////////////////////////////////////////////////////

System::Drawing::Color AxControl::BackColor::get ()
{
#ifdef	_DEBUG_AFX_PROPS
	LogMessage (_DEBUG_AFX_PROPS, _T("(%u) AxControl::BackColor::get"), (mControl!=nullptr));
#endif
	if	(mControl != nullptr)
	{
		try
		{
			return GetColorFromOleColor ((UInt32)mControl->BackColor);
		}
		catch AnyExceptionDebug
	}
	return AxHost::BackColor::get ();
}
void AxControl::BackColor::set (System::Drawing::Color value)
{
#ifdef	_DEBUG_AFX_PROPS
	LogMessage (_DEBUG_AFX_PROPS, _T("(%u) AxControl::BackColor::set"), (mControl!=nullptr));
#endif
	if	(mControl != nullptr)
	{
		try
		{
			mControl->BackColor = (UInt32)GetOleColorFromColor (value);
		}
		catch AnyExceptionDebug
	}
	AxHost::BackColor::set (value);
}

System::Drawing::Color AxControl::DefaultBackColor::get ()
{
	return System::Drawing::SystemColors::Window;
}

/////////////////////////////////////////////////////////////////////////////

System::Drawing::Color AxControl::BorderColor::get () 
{
#ifdef	_DEBUG_AFX_PROPS
	LogMessage (_DEBUG_AFX_PROPS, _T("(%u) AxControl::BackColor::get"), (mControl!=nullptr));
#endif
	if	(mControl != nullptr)
	{
		try
		{
			return GetColorFromOleColor ((UInt32)mControl->BorderColor);
		}
		catch AnyExceptionDebug
		{}
	}
	return DefaultBorderColor::get ();
}
void AxControl::BorderColor::set (System::Drawing::Color value)
{
#ifdef	_DEBUG_AFX_PROPS
	LogMessage (_DEBUG_AFX_PROPS, _T("(%u) AxControl::BackColor::set"), (mControl!=nullptr));
#endif
	if	(mControl != nullptr)
	{
		try
		{
			mControl->BorderColor = (UInt32)GetOleColorFromColor (value);
		}
		catch AnyExceptionDebug
	}
}

System::Drawing::Color AxControl::DefaultBorderColor::get ()
{
	return System::Drawing::SystemColors::WindowText;
}

/////////////////////////////////////////////////////////////////////////////

System::Int32 AxControl::BorderStyle::get ()
{
#ifdef	_DEBUG_AFX_PROPS
	LogMessage (_DEBUG_AFX_PROPS, _T("(%u) AxControl::BorderStyle::get"), (mControl!=nullptr));
#endif
	if	(mControl != nullptr)
	{
		try
		{
			return (Int32) mControl->BorderStyle;
		}
		catch AnyExceptionDebug
	}
	return DefaultBorderStyle::get ();
}
void AxControl::BorderStyle::set (System::Int32 value)
{
#ifdef	_DEBUG_AFX_PROPS
	LogMessage (_DEBUG_AFX_PROPS, _T("(%u) AxControl::BorderStyle::set"), (mControl!=nullptr));
#endif
	if	(mControl != nullptr)
	{
		try
		{
			mControl->BorderStyle = (int) value;
		}
		catch AnyExceptionDebug
	}
}

System::Int32 AxControl::DefaultBorderStyle::get ()
{
	return 1;
}

/////////////////////////////////////////////////////////////////////////////

System::Int32 AxControl::BorderWidth::get ()
{
#ifdef	_DEBUG_AFX_PROPS
	LogMessage (_DEBUG_AFX_PROPS, _T("(%u) AxControl::BackColor::get"), (mControl!=nullptr));
#endif
	if	(mControl != nullptr)
	{
		return (Int32) mControl->BorderWidth;
	}
	return DefaultBorderWidth::get ();
}
void AxControl::BorderWidth::set (System::Int32 value)
{
#ifdef	_DEBUG_AFX_PROPS
	LogMessage (_DEBUG_AFX_PROPS, _T("(%u) AxControl::BackColor::set"), (mControl!=nullptr));
#endif
	if	(mControl != nullptr)
	{
		try
		{
			mControl->BorderWidth = (int) value;
		}
		catch AnyExceptionDebug
	}
}

System::Int32 AxControl::DefaultBorderWidth::get ()
{
	return 1;
}

/////////////////////////////////////////////////////////////////////////////

System::Boolean AxControl::BorderVisible::get ()
{
#ifdef	_DEBUG_AFX_PROPS
	LogMessage (_DEBUG_AFX_PROPS, _T("(%u) AxControl::BackColor::get"), (mControl!=nullptr));
#endif
	if	(mControl != nullptr)
	{
		try
		{
			return (Boolean) mControl->BorderVisible;
		}
		catch AnyExceptionDebug
	}
	return DefaultBorderVisible::get ();
}
void AxControl::BorderVisible::set (System::Boolean value)
{
#ifdef	_DEBUG_AFX_PROPS
	LogMessage (_DEBUG_AFX_PROPS, _T("(%u) AxControl::BackColor::set"), (mControl!=nullptr));
#endif
	if	(mControl != nullptr)
	{
		try
		{
			mControl->BorderVisible = (bool) value;
		}
		catch AnyExceptionDebug
	}
}

System::Boolean AxControl::DefaultBorderVisible::get ()
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////

System::Windows::Forms::Cursor^ AxControl::Cursor::get ()
{
#ifdef	_DEBUG_AFX_PROPS
	LogMessage (_DEBUG_AFX_PROPS, _T("(%u) AxControl::BackColor::get"), (mControl!=nullptr));
#endif
	return AxHost::Cursor::get ();
}
void AxControl::Cursor::set (System::Windows::Forms::Cursor^ value)
{
#ifdef	_DEBUG_AFX_PROPS
	LogMessage (_DEBUG_AFX_PROPS, _T("(%u) AxControl::BackColor::set"), (mControl!=nullptr));
#endif
    if	(mControl != nullptr)
    {
		try
		{
			mControl->MouseIcon = (stdole::StdPicture^) GetIPictureFromCursor (value);
		}
		catch AnyExceptionDebug
    }
	AxHost::Cursor::set (value);            
}

/////////////////////////////////////////////////////////////////////////////
//page
/////////////////////////////////////////////////////////////////////////////

bool AxControl::Connected::get ()
{
#ifdef	_DEBUG_CTL_PROPS
	LogMessage (_DEBUG_CTL_PROPS, _T("(%u) AxControl::Connected::get"), (mControl!=nullptr));
#endif
	if  (mControl == nullptr)
	{
#ifdef	_DEBUG
		System::Diagnostics::Debug::Print ("AxControl::Connected::get - InvalidActiveXState");
#else
		throw gcnew AxHost::InvalidActiveXStateException ("Connected", AxHost::ActiveXInvokeKind::PropertyGet);
#endif		
	}
	else
	{
		try
		{
			return mControl->Connected;
		}
		catch AnyExceptionDebug
	}
	return false;
}
void AxControl::Connected::set  (bool value)
{
#ifdef	_DEBUG_CTL_PROPS
	LogMessage (_DEBUG_CTL_PROPS, _T("(%u) AxControl::Connected::set"), (mControl!=nullptr));
#endif
	if  (mControl == nullptr)
	{
#ifdef	_DEBUG
		System::Diagnostics::Debug::Print ("AxControl::Connected::set - InvalidActiveXState");
#else
		throw gcnew AxHost::InvalidActiveXStateException ("Connected", AxHost::ActiveXInvokeKind::PropertySet);
#endif		
	}
	else
	{
		try
		{
			mControl->Connected = value;
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////

bool AxControl::RaiseRequestErrors::get ()
{
#ifdef	_DEBUG_CTL_PROPS
	LogMessage (_DEBUG_CTL_PROPS, _T("(%u) AxControl::RaiseRequestErrors::get"), (mControl!=nullptr));
#endif
	if  (mControl == nullptr)
	{
#ifdef	_DEBUG
		System::Diagnostics::Debug::Print ("AxControl::RaiseRequestErrors::get - InvalidActiveXState");
#else
		throw gcnew AxHost::InvalidActiveXStateException ("RaiseRequestErrors", AxHost::ActiveXInvokeKind::PropertyGet);
#endif
	}
	else
	{
		try
		{
			return mControl->RaiseRequestErrors;
		}
		catch AnyExceptionDebug
	}
	return false;
}
void AxControl::RaiseRequestErrors::set (bool value)
{
#ifdef	_DEBUG_CTL_PROPS
	LogMessage (_DEBUG_CTL_PROPS, _T("(%u) AxControl::RaiseRequestErrors::set"), (mControl!=nullptr));
#endif
	if  (mControl == nullptr)
	{
#ifdef	_DEBUG
		System::Diagnostics::Debug::Print ("AxControl::RaiseRequestErrors::set - InvalidActiveXState");
#else
		throw gcnew AxHost::InvalidActiveXStateException ("RaiseRequestErrors", AxHost::ActiveXInvokeKind::PropertySet);
#endif		
	}
	else
	{
		try
		{
			mControl->RaiseRequestErrors = value;
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////

unsigned long AxControl::CharacterStyle::get ()
{
#ifdef	_DEBUG_CTL_PROPS
	LogMessage (_DEBUG_CTL_PROPS, _T("(%u) AxControl::CharacterStyle::get"), (mControl!=nullptr));
#endif
	if  (mControl == nullptr)
	{
#ifdef	_DEBUG
		System::Diagnostics::Debug::Print ("AxControl::CharacterStyle::get - InvalidActiveXState");
#else
		throw gcnew AxHost::InvalidActiveXStateException ("CharacterStyle", AxHost::ActiveXInvokeKind::PropertyGet);
#endif		
	}
	else
	{
		try
		{
			return mControl->CharacterStyle;
		}
		catch AnyExceptionDebug
	}
	return 0;
}
void AxControl::CharacterStyle::set (unsigned long value)
{
#ifdef	_DEBUG_CTL_PROPS
	LogMessage (_DEBUG_CTL_PROPS, _T("(%u) AxControl::CharacterStyle::set"), (mControl!=nullptr));
#endif
	if  (mControl == nullptr)
	{
#ifdef	_DEBUG
		System::Diagnostics::Debug::Print ("AxControl::CharacterStyle::set - InvalidActiveXState");
#else
		throw gcnew AxHost::InvalidActiveXStateException ("CharacterStyle", AxHost::ActiveXInvokeKind::PropertySet);
#endif		
	}
	else
	{
		try
		{
			mControl->CharacterStyle = value;
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////
//page
/////////////////////////////////////////////////////////////////////////////

DoubleAgent::Control::ICharacters^ AxControl::Characters::get ()
{
#ifdef	_DEBUG_OBJ_PROPS
	LogMessage (_DEBUG_OBJ_PROPS, _T("(%u) AxControl::Characters::get"), (mControl!=nullptr));
#endif
	if	(mControl == nullptr)
	{
#ifdef	_DEBUG
		System::Diagnostics::Debug::Print ("AxControl::Characters::get - InvalidActiveXState");
#else
		throw gcnew AxHost::InvalidActiveXStateException ("Characters", AxHost::ActiveXInvokeKind::PropertyGet);
#endif
	}
	else
	{
		try
		{
			return mControl->Characters;
		}
		catch AnyExceptionDebug
	}
	return nullptr;
}

DoubleAgent::Control::IAudioOutput^ AxControl::AudioOutput::get ()
{
	DoubleAgent::Control::IAudioOutput^ lRet = nullptr;
#ifdef	_DEBUG_OBJ_PROPS
	LogMessage (_DEBUG_OBJ_PROPS, _T("(%u) AxControl::AudioOutput::get"), (mControl!=nullptr));
#endif
	if	(mControl == nullptr)
	{
#ifdef	_DEBUG
		System::Diagnostics::Debug::Print ("AxControl::AudioOutput::get - InvalidActiveXState");
#else
		throw gcnew AxHost::InvalidActiveXStateException ("AudioOutput", AxHost::ActiveXInvokeKind::PropertyGet);
#endif		
	}
	else
	{
		try
		{
			lRet = mControl->AudioOutput;
		}
		catch AnyExceptionDebug
	}
#ifdef	_DEBUG_OBJ_PROPS
	LogMessage (_DEBUG_OBJ_PROPS, _T("(%u) AxControl::AudioOutput::get [%u]"), (mControl!=nullptr), (lRet!=nullptr));
#endif
	return lRet;
}

DoubleAgent::Control::ISpeechInput^ AxControl::SpeechInput::get ()
{
#ifdef	_DEBUG_OBJ_PROPS
	LogMessage (_DEBUG_OBJ_PROPS, _T("(%u) AxControl::SpeechInput::get"), (mControl!=nullptr));
#endif
	if  (mControl == nullptr)
	{
#ifdef	_DEBUG
		System::Diagnostics::Debug::Print ("AxControl::SpeechInput::get - InvalidActiveXState");
#else
		throw gcnew AxHost::InvalidActiveXStateException ("SpeechInput", AxHost::ActiveXInvokeKind::PropertyGet);
#endif
	}
	else
	{
		try
		{
			return mControl->SpeechInput;
		}
		catch AnyExceptionDebug
	}
	return nullptr;
}

DoubleAgent::Control::IPropertySheet^ AxControl::PropertySheet::get ()
{
#ifdef	_DEBUG_OBJ_PROPS
	LogMessage (_DEBUG_OBJ_PROPS, _T("(%u) AxControl::PropertySheet::get"), (mControl!=nullptr));
#endif
	if  (mControl == nullptr)
	{
#ifdef	_DEBUG
		System::Diagnostics::Debug::Print ("AxControl::PropertySheet::get - InvalidActiveXState");
#else
		throw gcnew AxHost::InvalidActiveXStateException ("PropertySheet", AxHost::ActiveXInvokeKind::PropertyGet);
#endif		
	}
	else
	{
		try
		{
			return mControl->PropertySheet;
		}
		catch AnyExceptionDebug
	}
	return nullptr;
}

DoubleAgent::Control::ICommandsWindow^ AxControl::CommandsWindow::get ()
{
#ifdef	_DEBUG_OBJ_PROPS
	LogMessage (_DEBUG_OBJ_PROPS, _T("(%u) AxControl::CommandsWindow::get"), (mControl!=nullptr));
#endif
	if  (mControl == nullptr)
	{
#ifdef	_DEBUG
		System::Diagnostics::Debug::Print ("AxControl::CommandsWindow::get - InvalidActiveXState");
#else
		throw gcnew AxHost::InvalidActiveXStateException ("CommandsWindow", AxHost::ActiveXInvokeKind::PropertyGet);
#endif		
	}
	else
	{
		try
		{
			return mControl->CommandsWindow;
		}
		catch AnyExceptionDebug
	}
	return nullptr;
}

DoubleAgent::Control::ICharacterFiles^ AxControl::CharacterFiles::get ()
{
#ifdef	_DEBUG_OBJ_PROPS
	LogMessage (_DEBUG_OBJ_PROPS, _T("(%u) AxControl::CharacterFiles::get"), (mControl!=nullptr));
#endif
	if  (mControl == nullptr)
	{
#ifdef	_DEBUG
		System::Diagnostics::Debug::Print ("AxControl::CommandsWindow::get - InvalidActiveXState");
#else
		throw gcnew AxHost::InvalidActiveXStateException ("CharacterFiles", AxHost::ActiveXInvokeKind::PropertyGet);
#endif		
	}
	else
	{
		try
		{
			return mControl->CharacterFiles;
		}
		catch AnyExceptionDebug
	}
	return nullptr;
}

DoubleAgent::Control::ITTSEngines^ AxControl::TTSEngines::get ()
{
#ifdef	_DEBUG_OBJ_PROPS
	LogMessage (_DEBUG_OBJ_PROPS, _T("(%u) AxControl::TTSEngines::get"), (mControl!=nullptr));
#endif
	if  (mControl == nullptr)
	{
#ifdef	_DEBUG
		System::Diagnostics::Debug::Print ("AxControl::CommandsWindow::get - InvalidActiveXState");
#else
		throw gcnew AxHost::InvalidActiveXStateException ("TTSEngines", AxHost::ActiveXInvokeKind::PropertyGet);
#endif		
	}
	else
	{
		try
		{
			return mControl->TTSEngines;
		}
		catch AnyExceptionDebug
	}
	return nullptr;
}

DoubleAgent::Control::ISREngines^ AxControl::SREngines::get ()
{
#ifdef	_DEBUG_OBJ_PROPS
	LogMessage (_DEBUG_OBJ_PROPS, _T("(%u) AxControl::SREngines::get"), (mControl!=nullptr));
#endif
	if  (mControl == nullptr)
	{
#ifdef	_DEBUG
		System::Diagnostics::Debug::Print ("AxControl::CommandsWindow::get - InvalidActiveXState");
#else
		throw gcnew AxHost::InvalidActiveXStateException ("SREngines", AxHost::ActiveXInvokeKind::PropertyGet);
#endif		
	}
	else
	{
		try
		{
			return mControl->SREngines;
		}
		catch AnyExceptionDebug
	}
	return nullptr;
}

DoubleAgent::Control::ISettings^ AxControl::Settings::get ()
{
#ifdef	_DEBUG_OBJ_PROPS
	LogMessage (_DEBUG_OBJ_PROPS, _T("(%u) AxControl::Settings::get"), (mControl!=nullptr));
#endif
	if  (mControl == nullptr)
	{
#ifdef	_DEBUG
		System::Diagnostics::Debug::Print ("AxControl::CommandsWindow::get - InvalidActiveXState");
#else
		throw gcnew AxHost::InvalidActiveXStateException ("Settings", AxHost::ActiveXInvokeKind::PropertyGet);
#endif		
	}
	else
	{
		try
		{
			return mControl->Settings;
		}
		catch AnyExceptionDebug
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////
//page
/////////////////////////////////////////////////////////////////////////////
    
DoubleAgent::Control::ITTSEngines^ AxControl::FindTTSEngines (Object^ LanguageID, Object^ Gender)
{
    if  (mControl == nullptr)
    {
        throw gcnew AxHost::InvalidActiveXStateException ("FindTTSEngines", AxHost::ActiveXInvokeKind::MethodInvoke);
    }
    return mControl->FindTTSEngines (LanguageID, Gender);
}

DoubleAgent::Control::ITTSEngine^ AxControl::GetCharacterTTSEngine (Object^ LoadKey)
{
    if  (mControl == nullptr)
    {
        throw gcnew AxHost::InvalidActiveXStateException ("GetCharacterTTSEngine", AxHost::ActiveXInvokeKind::MethodInvoke);
    }
    return mControl->GetCharacterTTSEngine (LoadKey);
}

DoubleAgent::Control::ITTSEngines^ AxControl::FindCharacterTTSEngines (Object^ LoadKey, Object^ LanguageID)
{
    if  (mControl == nullptr)
    {
        throw gcnew AxHost::InvalidActiveXStateException ("FindCharacterTTSEngines", AxHost::ActiveXInvokeKind::MethodInvoke);
    }
    return mControl->FindCharacterTTSEngines (LoadKey, LanguageID);
}

DoubleAgent::Control::ISREngines^ AxControl::FindSREngines (Object^ LanguageID)
{
    if  (mControl == nullptr)
    {
        throw gcnew AxHost::InvalidActiveXStateException ("FindSREngines", AxHost::ActiveXInvokeKind::MethodInvoke);
    }
    return mControl->FindSREngines (LanguageID);
}

DoubleAgent::Control::ISREngine^ AxControl::GetCharacterSREngine (Object^ LoadKey)
{
	if  (mControl == nullptr)
	{
		throw gcnew AxHost::InvalidActiveXStateException ("GetCharacterSREngine", AxHost::ActiveXInvokeKind::MethodInvoke);
	}
	return mControl->GetCharacterSREngine (LoadKey);
}

DoubleAgent::Control::ISREngines^ AxControl::FindCharacterSREngines (Object^ LoadKey, Object^ LanguageID)
{
    if  (mControl == nullptr)
    {
        throw gcnew AxHost::InvalidActiveXStateException ("FindCharacterSREngines", AxHost::ActiveXInvokeKind::MethodInvoke);
    }
    return mControl->FindCharacterSREngines (LoadKey, LanguageID);
}

void AxControl::ShowDefaultCharacterProperties (Object^ X, Object^ Y)
{
    if  (mControl == nullptr)
    {
        throw gcnew AxHost::InvalidActiveXStateException ("ShowDefaultCharacterProperties", AxHost::ActiveXInvokeKind::MethodInvoke);
    }
    mControl->ShowDefaultCharacterProperties (X, Y);
}

void AxControl::ShowDefaultCharacterProperties ()
{
    mControl->ShowDefaultCharacterProperties (Type::Missing, Type::Missing);
}

/////////////////////////////////////////////////////////////////////////////
//page
/////////////////////////////////////////////////////////////////////////////

void AxControl::PopupActivateInput::add (ActivateInputEventHandler^ pDelegate)
{
	mActivateInputEvent = static_cast<ActivateInputEventHandler^> (Delegate::Combine (mActivateInputEvent, pDelegate)); 
}

void AxControl::PopupActivateInput::remove (ActivateInputEventHandler^ pDelegate)
{
	mActivateInputEvent = static_cast<ActivateInputEventHandler^> (Delegate::Remove (mActivateInputEvent, pDelegate)); 
}

void AxControl::PopupActivateInput::raise (Object^ pSender, DoubleAgent::Control::Events::ActivateInputEvent^ pEvent)
{
	if  (mActivateInputEvent != nullptr)
	{
		mActivateInputEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::PopupDeactivateInput::add (DeactivateInputEventHandler^ pDelegate)
{
	mDeactivateInputEvent = static_cast<DeactivateInputEventHandler^> (Delegate::Combine (mDeactivateInputEvent, pDelegate)); 
}

void AxControl::PopupDeactivateInput::remove (DeactivateInputEventHandler^ pDelegate)
{
	mDeactivateInputEvent = static_cast<DeactivateInputEventHandler^> (Delegate::Remove (mDeactivateInputEvent, pDelegate)); 
}

void AxControl::PopupDeactivateInput::raise (Object^ pSender, DoubleAgent::Control::Events::DeactivateInputEvent^ pEvent)
{
	if  (mDeactivateInputEvent != nullptr)
	{
		mDeactivateInputEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::PopupClick::add (ClickEventHandler^ pDelegate)
{
	mClickEvent = static_cast<ClickEventHandler^> (Delegate::Combine (mClickEvent, pDelegate)); 
}

void AxControl::PopupClick::remove (ClickEventHandler^ pDelegate)
{
	mClickEvent = static_cast<ClickEventHandler^> (Delegate::Remove (mClickEvent, pDelegate)); 
}

void AxControl::PopupClick::raise (Object^ pSender, DoubleAgent::Control::Events::ClickEvent^ pEvent)
{
	if  (mClickEvent != nullptr)
	{
		mClickEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::PopupDblClick::add (DblClickEventHandler^ pDelegate)
{
	mDblClickEvent = static_cast<DblClickEventHandler^> (Delegate::Combine (mDblClickEvent, pDelegate)); 
}

void AxControl::PopupDblClick::remove (DblClickEventHandler^ pDelegate)
{
	mDblClickEvent = static_cast<DblClickEventHandler^> (Delegate::Remove (mDblClickEvent, pDelegate)); 
}

void AxControl::PopupDblClick::raise (Object^ pSender, DoubleAgent::Control::Events::DblClickEvent^ pEvent)
{
	if  (mDblClickEvent != nullptr)
	{
		mDblClickEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::PopupDragStart::add (DragStartEventHandler^ pDelegate)
{
	mDragStartEvent = static_cast<DragStartEventHandler^> (Delegate::Combine (mDragStartEvent, pDelegate)); 
}

void AxControl::PopupDragStart::remove (DragStartEventHandler^ pDelegate)
{
	mDragStartEvent = static_cast<DragStartEventHandler^> (Delegate::Remove (mDragStartEvent, pDelegate)); 
}

void AxControl::PopupDragStart::raise (Object^ pSender, DoubleAgent::Control::Events::DragStartEvent^ pEvent)
{
	if  (mDragStartEvent != nullptr)
	{
		mDragStartEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::PopupDragComplete::add (DragCompleteEventHandler^ pDelegate)
{
	mDragCompleteEvent = static_cast<DragCompleteEventHandler^> (Delegate::Combine (mDragCompleteEvent, pDelegate)); 
}

void AxControl::PopupDragComplete::remove (DragCompleteEventHandler^ pDelegate)
{
	mDragCompleteEvent = static_cast<DragCompleteEventHandler^> (Delegate::Remove (mDragCompleteEvent, pDelegate)); 
}

void AxControl::PopupDragComplete::raise (Object^ pSender, DoubleAgent::Control::Events::DragCompleteEvent^ pEvent)
{
	if  (mDragCompleteEvent != nullptr)
	{
		mDragCompleteEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::PopupShow::add (ShowEventHandler^ pDelegate)
{
	mShowEvent = static_cast<ShowEventHandler^> (Delegate::Combine (mShowEvent, pDelegate)); 
}

void AxControl::PopupShow::remove (ShowEventHandler^ pDelegate)
{
	mShowEvent = static_cast<ShowEventHandler^> (Delegate::Remove (mShowEvent, pDelegate)); 
}

void AxControl::PopupShow::raise (Object^ pSender, DoubleAgent::Control::Events::ShowEvent^ pEvent)
{
	if  (mShowEvent != nullptr)
	{
		mShowEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::PopupHide::add (HideEventHandler^ pDelegate)
{
	mHideEvent = static_cast<HideEventHandler^> (Delegate::Combine (mHideEvent, pDelegate)); 
}

void AxControl::PopupHide::remove (HideEventHandler^ pDelegate)
{
	mHideEvent = static_cast<HideEventHandler^> (Delegate::Remove (mHideEvent, pDelegate)); 
}

void AxControl::PopupHide::raise (Object^ pSender, DoubleAgent::Control::Events::HideEvent^ pEvent)
{
	if  (mHideEvent != nullptr)
	{
		mHideEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::RequestStart::add (RequestStartEventHandler^ pDelegate)
{
	mRequestStartEvent = static_cast<RequestStartEventHandler^> (Delegate::Combine (mRequestStartEvent, pDelegate)); 
}

void AxControl::RequestStart::remove (RequestStartEventHandler^ pDelegate)
{
	mRequestStartEvent = static_cast<RequestStartEventHandler^> (Delegate::Remove (mRequestStartEvent, pDelegate)); 
}

void AxControl::RequestStart::raise (Object^ pSender, DoubleAgent::Control::Events::RequestStartEvent^ pEvent)
{
	if  (mRequestStartEvent != nullptr)
	{
		mRequestStartEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::RequestComplete::add (RequestCompleteEventHandler^ pDelegate)
{
	mRequestCompleteEvent = static_cast<RequestCompleteEventHandler^> (Delegate::Combine (mRequestCompleteEvent, pDelegate)); 
}

void AxControl::RequestComplete::remove (RequestCompleteEventHandler^ pDelegate)
{
	mRequestCompleteEvent = static_cast<RequestCompleteEventHandler^> (Delegate::Remove (mRequestCompleteEvent, pDelegate)); 
}

void AxControl::RequestComplete::raise (Object^ pSender, DoubleAgent::Control::Events::RequestCompleteEvent^ pEvent)
{
	if  (mRequestCompleteEvent != nullptr)
	{
		mRequestCompleteEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::Bookmark::add (BookmarkEventHandler^ pDelegate)
{
	mBookmarkEvent = static_cast<BookmarkEventHandler^> (Delegate::Combine (mBookmarkEvent, pDelegate)); 
}

void AxControl::Bookmark::remove (BookmarkEventHandler^ pDelegate)
{
	mBookmarkEvent = static_cast<BookmarkEventHandler^> (Delegate::Remove (mBookmarkEvent, pDelegate)); 
}

void AxControl::Bookmark::raise (Object^ pSender, DoubleAgent::Control::Events::BookmarkEvent^ pEvent)
{
	if  (mBookmarkEvent != nullptr)
	{
		mBookmarkEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::Command::add (CommandEventHandler^ pDelegate)
{
	mCommandEvent = static_cast<CommandEventHandler^> (Delegate::Combine (mCommandEvent, pDelegate)); 
}

void AxControl::Command::remove (CommandEventHandler^ pDelegate)
{
	mCommandEvent = static_cast<CommandEventHandler^> (Delegate::Remove (mCommandEvent, pDelegate)); 
}

void AxControl::Command::raise (Object^ pSender, DoubleAgent::Control::Events::CommandEvent^ pEvent)
{
	if  (mCommandEvent != nullptr)
	{
		mCommandEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::IdleStart::add (IdleStartEventHandler^ pDelegate)
{
	mIdleStartEvent = static_cast<IdleStartEventHandler^> (Delegate::Combine (mIdleStartEvent, pDelegate)); 
}

void AxControl::IdleStart::remove (IdleStartEventHandler^ pDelegate)
{
	mIdleStartEvent = static_cast<IdleStartEventHandler^> (Delegate::Remove (mIdleStartEvent, pDelegate)); 
}

void AxControl::IdleStart::raise (Object^ pSender, DoubleAgent::Control::Events::IdleStartEvent^ pEvent)
{
	if  (mIdleStartEvent != nullptr)
	{
		mIdleStartEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::IdleComplete::add (IdleCompleteEventHandler^ pDelegate)
{
	mIdleCompleteEvent = static_cast<IdleCompleteEventHandler^> (Delegate::Combine (mIdleCompleteEvent, pDelegate)); 
}

void AxControl::IdleComplete::remove (IdleCompleteEventHandler^ pDelegate)
{
	mIdleCompleteEvent = static_cast<IdleCompleteEventHandler^> (Delegate::Remove (mIdleCompleteEvent, pDelegate)); 
}

void AxControl::IdleComplete::raise (Object^ pSender, DoubleAgent::Control::Events::IdleCompleteEvent^ pEvent)
{
	if  (mIdleCompleteEvent != nullptr)
	{
		mIdleCompleteEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::PopupMove::add (MoveEventHandler^ pDelegate)
{
	mMoveEvent = static_cast<MoveEventHandler^> (Delegate::Combine (mMoveEvent, pDelegate)); 
}

void AxControl::PopupMove::remove (MoveEventHandler^ pDelegate)
{
	mMoveEvent = static_cast<MoveEventHandler^> (Delegate::Remove (mMoveEvent, pDelegate)); 
}

void AxControl::PopupMove::raise (Object^ pSender, DoubleAgent::Control::Events::MoveEvent^ pEvent)
{
	if  (mMoveEvent != nullptr)
	{
		mMoveEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::PopupSize::add (SizeEventHandler^ pDelegate)
{
	mSizeEvent = static_cast<SizeEventHandler^> (Delegate::Combine (mSizeEvent, pDelegate)); 
}

void AxControl::PopupSize::remove (SizeEventHandler^ pDelegate)
{
	mSizeEvent = static_cast<SizeEventHandler^> (Delegate::Remove (mSizeEvent, pDelegate)); 
}

void AxControl::PopupSize::raise (Object^ pSender, DoubleAgent::Control::Events::SizeEvent^ pEvent)
{
	if  (mSizeEvent != nullptr)
	{
		mSizeEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::BalloonShow::add (BalloonShowEventHandler^ pDelegate)
{
	mBalloonShowEvent = static_cast<BalloonShowEventHandler^> (Delegate::Combine (mBalloonShowEvent, pDelegate)); 
}

void AxControl::BalloonShow::remove (BalloonShowEventHandler^ pDelegate)
{
	mBalloonShowEvent = static_cast<BalloonShowEventHandler^> (Delegate::Remove (mBalloonShowEvent, pDelegate)); 
}

void AxControl::BalloonShow::raise (Object^ pSender, DoubleAgent::Control::Events::BalloonShowEvent^ pEvent)
{
	if  (mBalloonShowEvent != nullptr)
	{
		mBalloonShowEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::BalloonHide::add (BalloonHideEventHandler^ pDelegate)
{
	mBalloonHideEvent = static_cast<BalloonHideEventHandler^> (Delegate::Combine (mBalloonHideEvent, pDelegate)); 
}

void AxControl::BalloonHide::remove (BalloonHideEventHandler^ pDelegate)
{
	mBalloonHideEvent = static_cast<BalloonHideEventHandler^> (Delegate::Remove (mBalloonHideEvent, pDelegate)); 
}

void AxControl::BalloonHide::raise (Object^ pSender, DoubleAgent::Control::Events::BalloonHideEvent^ pEvent)
{
	if  (mBalloonHideEvent != nullptr)
	{
		mBalloonHideEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::ListenStart::add (ListenStartEventHandler^ pDelegate)
{
	mListenStartEvent = static_cast<ListenStartEventHandler^> (Delegate::Combine (mListenStartEvent, pDelegate)); 
}

void AxControl::ListenStart::remove (ListenStartEventHandler^ pDelegate)
{
	mListenStartEvent = static_cast<ListenStartEventHandler^> (Delegate::Remove (mListenStartEvent, pDelegate)); 
}

void AxControl::ListenStart::raise (Object^ pSender, DoubleAgent::Control::Events::ListenStartEvent^ pEvent)
{
	if  (mListenStartEvent != nullptr)
	{
		mListenStartEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::ListenComplete::add (ListenCompleteEventHandler^ pDelegate)
{
	mListenCompleteEvent = static_cast<ListenCompleteEventHandler^> (Delegate::Combine (mListenCompleteEvent, pDelegate)); 
}

void AxControl::ListenComplete::remove (ListenCompleteEventHandler^ pDelegate)
{
	mListenCompleteEvent = static_cast<ListenCompleteEventHandler^> (Delegate::Remove (mListenCompleteEvent, pDelegate)); 
}

void AxControl::ListenComplete::raise (Object^ pSender, DoubleAgent::Control::Events::ListenCompleteEvent^ pEvent)
{
	if  (mListenCompleteEvent != nullptr)
	{
		mListenCompleteEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::DefaultCharacterChange::add (DefaultCharacterChangeEventHandler^ pDelegate)
{
	mDefaultCharacterChangeEvent = static_cast<DefaultCharacterChangeEventHandler^> (Delegate::Combine (mDefaultCharacterChangeEvent, pDelegate)); 
}

void AxControl::DefaultCharacterChange::remove (DefaultCharacterChangeEventHandler^ pDelegate)
{
	mDefaultCharacterChangeEvent = static_cast<DefaultCharacterChangeEventHandler^> (Delegate::Remove (mDefaultCharacterChangeEvent, pDelegate)); 
}

void AxControl::DefaultCharacterChange::raise (Object^ pSender, DoubleAgent::Control::Events::DefaultCharacterChangeEvent^ pEvent)
{
	if  (mDefaultCharacterChangeEvent != nullptr)
	{
		mDefaultCharacterChangeEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::AgentPropertyChange::add (System::EventHandler^ pDelegate)
{
	mAgentPropertyChangeEvent = static_cast<System::EventHandler^> (Delegate::Combine (mAgentPropertyChangeEvent, pDelegate)); 
}

void AxControl::AgentPropertyChange::remove (System::EventHandler^ pDelegate)
{
	mAgentPropertyChangeEvent = static_cast<System::EventHandler^> (Delegate::Remove (mAgentPropertyChangeEvent, pDelegate)); 
}

void AxControl::AgentPropertyChange::raise (Object^ pSender, System::EventArgs^ pEvent)
{
	if  (mAgentPropertyChangeEvent != nullptr)
	{
		mAgentPropertyChangeEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::ActiveClientChange::add (ActiveClientChangeEventHandler^ pDelegate)
{
	mActiveClientChangeEvent = static_cast<ActiveClientChangeEventHandler^> (Delegate::Combine (mActiveClientChangeEvent, pDelegate)); 
}

void AxControl::ActiveClientChange::remove (ActiveClientChangeEventHandler^ pDelegate)
{
	mActiveClientChangeEvent = static_cast<ActiveClientChangeEventHandler^> (Delegate::Remove (mActiveClientChangeEvent, pDelegate)); 
}

void AxControl::ActiveClientChange::raise (Object^ pSender, DoubleAgent::Control::Events::ActiveClientChangeEvent^ pEvent)
{
	if  (mActiveClientChangeEvent != nullptr)
	{
		mActiveClientChangeEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////
}	// namespace Control
}	// namespace DoubleAgent