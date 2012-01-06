/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
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
#if	!defined (_M_CEE_PURE) && !defined (_M_CEE_SAFE)
#pragma managed(push,off)
#ifdef	_DEBUG
#define _LOG_LEVEL_DEBUG	LogNormal
#endif
#define	_LOG_ROOT_PATH		_T("Software\\")_T(_DOUBLEAGENT_NAME)_T("\\")
#define	_LOG_SECTION_NAME	_T(_CONTROL_REGNAME)
#define _LOG_DEF_LOGNAME	_T(_DOUBLEAGENT_NAME) _T(".log")
#define	_LOG_PREFIX			_T(".NET ")
#include "LogAccess.inl"
#include "Log.inl"
#pragma comment(lib, "advapi32.lib")
#pragma managed(pop)
#endif	// _M_CEE_XXXX

#ifdef	_DEBUG
#define	_DEBUG_INTERNALS	LogNormal
//#define	_DEBUG_EVENTS		LogNormal
//#define	_DEBUG_AFX_PROPS	LogNormal
//#define	_DEBUG_CTL_PROPS	LogNormal
//#define	_DEBUG_OBJ_PROPS	LogNormal
#endif

/////////////////////////////////////////////////////////////////////////////

namespace DoubleAgent {
namespace AxControl {
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
	DetachSink ();
	try
	{
		if	(AxInterface)
		{
			AxInterface = nullptr;
		}
	}
	catch AnyExceptionSilent
	{}

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
		AxInterface = safe_cast <DoubleAgent::Control::IControl^> (GetOcx());
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
		mEventCookie = gcnew AxHost::ConnectionPointCookie (AxInterface, mEventMulticaster, DoubleAgent::Control::Native::_DaCtlEvents::typeid);
    }
    catch AnyExceptionDebug
    {}
#ifdef	_DEBUG_INTERNALS
	LogMessage (_DEBUG_INTERNALS, _T("AxControl::CreateSink [%u]"), !!mEventCookie);
#endif
}

void AxControl::DetachSink ()
{
#ifdef	_DEBUG_INTERNALS
	LogMessage (_DEBUG_INTERNALS, _T("AxControl::DetachSink [%u]"), !!mEventCookie);
#endif
	try
	{
		if	(mEventCookie)
		{
			mEventCookie->Disconnect ();
		}
		mEventCookie = nullptr;
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
	LogMessage (_DEBUG_AFX_PROPS, _T("(%u) AxControl::AutoSize::get"), (AxControl!=nullptr));
#endif
	if	(AxInterface != nullptr)
	{
		try
		{
			return AxInterface->AutoSize;
		}
		catch AnyExceptionDebug
		{}
	}
	return AxHost::AutoSize::get ();
}
void AxControl::AutoSize::set (bool value)
{
#ifdef	_DEBUG_AFX_PROPS
	LogMessage (_DEBUG_AFX_PROPS, _T("(%u) AxControl::AutoSize::set"), (AxControl!=nullptr));
#endif
	if	(AxInterface != nullptr)
	{
		try
		{
			AxInterface->AutoSize = value;
		}
		catch AnyExceptionDebug
		{}
	}
	AxHost::AutoSize::set (value);
}

/////////////////////////////////////////////////////////////////////////////

System::Drawing::Color AxControl::BackColor::get ()
{
#ifdef	_DEBUG_AFX_PROPS
	LogMessage (_DEBUG_AFX_PROPS, _T("(%u) AxControl::BackColor::get"), (AxControl!=nullptr));
#endif
	if	(AxInterface != nullptr)
	{
		try
		{
			return GetColorFromOleColor ((UInt32)AxInterface->BackColor);
		}
		catch AnyExceptionDebug
		{}
	}
	return AxHost::BackColor::get ();
}
void AxControl::BackColor::set (System::Drawing::Color value)
{
#ifdef	_DEBUG_AFX_PROPS
	LogMessage (_DEBUG_AFX_PROPS, _T("(%u) AxControl::BackColor::set"), (AxControl!=nullptr));
#endif
	if	(AxInterface != nullptr)
	{
		try
		{
			AxInterface->BackColor = (UInt32)GetOleColorFromColor (value);
		}
		catch AnyExceptionDebug
		{}
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
	LogMessage (_DEBUG_AFX_PROPS, _T("(%u) AxControl::BackColor::get"), (AxControl!=nullptr));
#endif
	if	(AxInterface != nullptr)
	{
		try
		{
			return GetColorFromOleColor ((UInt32)AxInterface->BorderColor);
		}
		catch AnyExceptionDebug
		{}
	}
	return DefaultBorderColor::get ();
}
void AxControl::BorderColor::set (System::Drawing::Color value)
{
#ifdef	_DEBUG_AFX_PROPS
	LogMessage (_DEBUG_AFX_PROPS, _T("(%u) AxControl::BackColor::set"), (AxControl!=nullptr));
#endif
	if	(AxInterface != nullptr)
	{
		try
		{
			AxInterface->BorderColor = (UInt32)GetOleColorFromColor (value);
		}
		catch AnyExceptionDebug
		{}
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
	LogMessage (_DEBUG_AFX_PROPS, _T("(%u) AxControl::BorderStyle::get"), (AxControl!=nullptr));
#endif
	if	(AxInterface != nullptr)
	{
		try
		{
			return (Int32) AxInterface->BorderStyle;
		}
		catch AnyExceptionDebug
		{}
	}
	return DefaultBorderStyle::get ();
}
void AxControl::BorderStyle::set (System::Int32 value)
{
#ifdef	_DEBUG_AFX_PROPS
	LogMessage (_DEBUG_AFX_PROPS, _T("(%u) AxControl::BorderStyle::set"), (AxControl!=nullptr));
#endif
	if	(AxInterface != nullptr)
	{
		try
		{
			AxInterface->BorderStyle = (int) value;
		}
		catch AnyExceptionDebug
		{}
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
	LogMessage (_DEBUG_AFX_PROPS, _T("(%u) AxControl::BackColor::get"), (AxControl!=nullptr));
#endif
	if	(AxInterface != nullptr)
	{
		return (Int32) AxInterface->BorderWidth;
	}
	return DefaultBorderWidth::get ();
}
void AxControl::BorderWidth::set (System::Int32 value)
{
#ifdef	_DEBUG_AFX_PROPS
	LogMessage (_DEBUG_AFX_PROPS, _T("(%u) AxControl::BackColor::set"), (AxControl!=nullptr));
#endif
	if	(AxInterface != nullptr)
	{
		try
		{
			AxInterface->BorderWidth = (int) value;
		}
		catch AnyExceptionDebug
		{}
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
	LogMessage (_DEBUG_AFX_PROPS, _T("(%u) AxControl::BackColor::get"), (AxControl!=nullptr));
#endif
	if	(AxInterface != nullptr)
	{
		try
		{
			return (Boolean) AxInterface->BorderVisible;
		}
		catch AnyExceptionDebug
		{}
	}
	return DefaultBorderVisible::get ();
}
void AxControl::BorderVisible::set (System::Boolean value)
{
#ifdef	_DEBUG_AFX_PROPS
	LogMessage (_DEBUG_AFX_PROPS, _T("(%u) AxControl::BackColor::set"), (AxControl!=nullptr));
#endif
	if	(AxInterface != nullptr)
	{
		try
		{
			AxInterface->BorderVisible = (bool) value;
		}
		catch AnyExceptionDebug
		{}
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
	LogMessage (_DEBUG_AFX_PROPS, _T("(%u) AxControl::BackColor::get"), (AxControl!=nullptr));
#endif
	return AxHost::Cursor::get ();
}
void AxControl::Cursor::set (System::Windows::Forms::Cursor^ value)
{
#ifdef	_DEBUG_AFX_PROPS
	LogMessage (_DEBUG_AFX_PROPS, _T("(%u) AxControl::BackColor::set"), (AxControl!=nullptr));
#endif
    if	(AxInterface != nullptr)
    {
		try
		{
			AxInterface->MouseIcon = (stdole::StdPicture^) GetIPictureFromCursor (value);
		}
		catch AnyExceptionDebug
		{}
    }
	AxHost::Cursor::set (value);
}

/////////////////////////////////////////////////////////////////////////////
//page
/////////////////////////////////////////////////////////////////////////////

bool AxControl::Connected::get ()
{
#ifdef	_DEBUG_CTL_PROPS
	LogMessage (_DEBUG_CTL_PROPS, _T("(%u) AxControl::Connected::get"), (AxControl!=nullptr));
#endif
	if  (AxInterface == nullptr)
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
			return AxInterface->Connected;
		}
		catch AnyExceptionDebug
		{}
	}
	return false;
}
void AxControl::Connected::set  (bool value)
{
#ifdef	_DEBUG_CTL_PROPS
	LogMessage (_DEBUG_CTL_PROPS, _T("(%u) AxControl::Connected::set"), (AxControl!=nullptr));
#endif
	if  (AxInterface == nullptr)
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
			AxInterface->Connected = value;
		}
		catch AnyExceptionDebug
		{}
	}
}

/////////////////////////////////////////////////////////////////////////////

bool AxControl::AutoConnect::get ()
{
#ifdef	_DEBUG_CTL_PROPS
	LogMessage (_DEBUG_CTL_PROPS, _T("(%u) AxControl::AutoConnect::get"), (AxControl!=nullptr));
#endif
	if  (AxInterface == nullptr)
	{
#ifdef	_DEBUG
		System::Diagnostics::Debug::Print ("AxControl::AutoConnect::get - InvalidActiveXState");
#else
		throw gcnew AxHost::InvalidActiveXStateException ("AutoConnect", AxHost::ActiveXInvokeKind::PropertyGet);
#endif
	}
	else
	{
		try
		{
			return AxInterface->AutoConnect;
		}
		catch AnyExceptionDebug
		{}
	}
	return false;
}
void AxControl::AutoConnect::set (bool value)
{
#ifdef	_DEBUG_CTL_PROPS
	LogMessage (_DEBUG_CTL_PROPS, _T("(%u) AxControl::AutoConnect::set"), (AxControl!=nullptr));
#endif
	if  (AxInterface == nullptr)
	{
#ifdef	_DEBUG
		System::Diagnostics::Debug::Print ("AxControl::AutoConnect::set - InvalidActiveXState");
#else
		throw gcnew AxHost::InvalidActiveXStateException ("AutoConnect", AxHost::ActiveXInvokeKind::PropertySet);
#endif
	}
	else
	{
		try
		{
			AxInterface->AutoConnect = value;
		}
		catch AnyExceptionDebug
		{}
	}
}

/////////////////////////////////////////////////////////////////////////////

bool AxControl::RaiseRequestErrors::get ()
{
#ifdef	_DEBUG_CTL_PROPS
	LogMessage (_DEBUG_CTL_PROPS, _T("(%u) AxControl::RaiseRequestErrors::get"), (AxControl!=nullptr));
#endif
	if  (AxInterface == nullptr)
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
			return AxInterface->RaiseRequestErrors;
		}
		catch AnyExceptionDebug
		{}
	}
	return false;
}
void AxControl::RaiseRequestErrors::set (bool value)
{
#ifdef	_DEBUG_CTL_PROPS
	LogMessage (_DEBUG_CTL_PROPS, _T("(%u) AxControl::RaiseRequestErrors::set"), (AxControl!=nullptr));
#endif
	if  (AxInterface == nullptr)
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
			AxInterface->RaiseRequestErrors = value;
		}
		catch AnyExceptionDebug
		{}
	}
}

/////////////////////////////////////////////////////////////////////////////

unsigned long AxControl::CharacterStyle::get ()
{
#ifdef	_DEBUG_CTL_PROPS
	LogMessage (_DEBUG_CTL_PROPS, _T("(%u) AxControl::CharacterStyle::get"), (AxControl!=nullptr));
#endif
	if  (AxInterface == nullptr)
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
			return AxInterface->CharacterStyle;
		}
		catch AnyExceptionDebug
		{}
	}
	return 0;
}
void AxControl::CharacterStyle::set (unsigned long value)
{
#ifdef	_DEBUG_CTL_PROPS
	LogMessage (_DEBUG_CTL_PROPS, _T("(%u) AxControl::CharacterStyle::set"), (AxControl!=nullptr));
#endif
	if  (AxInterface == nullptr)
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
			AxInterface->CharacterStyle = value;
		}
		catch AnyExceptionDebug
		{}
	}
}

/////////////////////////////////////////////////////////////////////////////
//page
/////////////////////////////////////////////////////////////////////////////

DoubleAgent::Control::Characters^ AxControl::Characters::get ()
{
#ifdef	_DEBUG_OBJ_PROPS
	LogMessage (_DEBUG_OBJ_PROPS, _T("(%u) AxControl::Characters::get"), (AxControl!=nullptr));
#endif
	if	(AxInterface == nullptr)
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
			return AxInterface->Characters;
		}
		catch AnyExceptionDebug
		{}
	}
	return nullptr;
}

DoubleAgent::Control::AudioOutput^ AxControl::AudioOutput::get ()
{
#ifdef	_DEBUG_OBJ_PROPS
	LogMessage (_DEBUG_OBJ_PROPS, _T("(%u) AxControl::AudioOutput::get"), (AxControl!=nullptr));
#endif
	if	(AxInterface == nullptr)
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
			return AxInterface->AudioOutput;
		}
		catch AnyExceptionDebug
		{}
	}
	return nullptr;
}

DoubleAgent::Control::SpeechInput^ AxControl::SpeechInput::get ()
{
#ifdef	_DEBUG_OBJ_PROPS
	LogMessage (_DEBUG_OBJ_PROPS, _T("(%u) AxControl::SpeechInput::get"), (AxControl!=nullptr));
#endif
	if  (AxInterface == nullptr)
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
			return AxInterface->SpeechInput;
		}
		catch AnyExceptionDebug
		{}
	}
	return nullptr;
}

DoubleAgent::Control::PropertySheet^ AxControl::PropertySheet::get ()
{
#ifdef	_DEBUG_OBJ_PROPS
	LogMessage (_DEBUG_OBJ_PROPS, _T("(%u) AxControl::PropertySheet::get"), (AxControl!=nullptr));
#endif
	if  (AxInterface == nullptr)
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
			return AxInterface->PropertySheet;
		}
		catch AnyExceptionDebug
		{}
	}
	return nullptr;
}

DoubleAgent::Control::CommandsWindow^ AxControl::CommandsWindow::get ()
{
#ifdef	_DEBUG_OBJ_PROPS
	LogMessage (_DEBUG_OBJ_PROPS, _T("(%u) AxControl::CommandsWindow::get"), (AxControl!=nullptr));
#endif
	if  (AxInterface == nullptr)
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
			return AxInterface->CommandsWindow;
		}
		catch AnyExceptionDebug
		{}
	}
	return nullptr;
}

DoubleAgent::Control::CharacterFiles^ AxControl::CharacterFiles::get ()
{
#ifdef	_DEBUG_OBJ_PROPS
	LogMessage (_DEBUG_OBJ_PROPS, _T("(%u) AxControl::CharacterFiles::get"), (AxControl!=nullptr));
#endif
	if  (AxInterface == nullptr)
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
			return AxInterface->CharacterFiles;
		}
		catch AnyExceptionDebug
		{}
	}
	return nullptr;
}

DoubleAgent::Control::TTSEngines^ AxControl::TTSEngines::get ()
{
#ifdef	_DEBUG_OBJ_PROPS
	LogMessage (_DEBUG_OBJ_PROPS, _T("(%u) AxControl::TTSEngines::get"), (AxControl!=nullptr));
#endif
	if  (AxInterface == nullptr)
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
			return AxInterface->TTSEngines;
		}
		catch AnyExceptionDebug
		{}
	}
	return nullptr;
}

DoubleAgent::Control::SREngines^ AxControl::SREngines::get ()
{
#ifdef	_DEBUG_OBJ_PROPS
	LogMessage (_DEBUG_OBJ_PROPS, _T("(%u) AxControl::SREngines::get"), (AxControl!=nullptr));
#endif
	if  (AxInterface == nullptr)
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
			return AxInterface->SREngines;
		}
		catch AnyExceptionDebug
		{}
	}
	return nullptr;
}

DoubleAgent::Control::Settings^ AxControl::Settings::get ()
{
#ifdef	_DEBUG_OBJ_PROPS
	LogMessage (_DEBUG_OBJ_PROPS, _T("(%u) AxControl::Settings::get"), (AxControl!=nullptr));
#endif
	if  (AxInterface == nullptr)
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
			return AxInterface->Settings;
		}
		catch AnyExceptionDebug
		{}
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////
//page
/////////////////////////////////////////////////////////////////////////////

DoubleAgent::Control::TTSEngines^ AxControl::FindTTSEngines (Object^ LanguageID, Object^ Gender)
{
    if  (AxInterface == nullptr)
    {
        throw gcnew AxHost::InvalidActiveXStateException ("FindTTSEngines", AxHost::ActiveXInvokeKind::MethodInvoke);
    }
    return AxInterface->FindTTSEngines (LanguageID, Gender);
}

DoubleAgent::Control::TTSEngine^ AxControl::GetCharacterTTSEngine (Object^ LoadKey)
{
    if  (AxInterface == nullptr)
    {
        throw gcnew AxHost::InvalidActiveXStateException ("GetCharacterTTSEngine", AxHost::ActiveXInvokeKind::MethodInvoke);
    }
    return AxInterface->GetCharacterTTSEngine (LoadKey);
}

DoubleAgent::Control::TTSEngines^ AxControl::FindCharacterTTSEngines (Object^ LoadKey, Object^ LanguageID)
{
    if  (AxInterface == nullptr)
    {
        throw gcnew AxHost::InvalidActiveXStateException ("FindCharacterTTSEngines", AxHost::ActiveXInvokeKind::MethodInvoke);
    }
    return AxInterface->FindCharacterTTSEngines (LoadKey, LanguageID);
}

DoubleAgent::Control::SREngines^ AxControl::FindSREngines (Object^ LanguageID)
{
    if  (AxInterface == nullptr)
    {
        throw gcnew AxHost::InvalidActiveXStateException ("FindSREngines", AxHost::ActiveXInvokeKind::MethodInvoke);
    }
    return AxInterface->FindSREngines (LanguageID);
}

DoubleAgent::Control::SREngine^ AxControl::GetCharacterSREngine (Object^ LoadKey)
{
	if  (AxInterface == nullptr)
	{
		throw gcnew AxHost::InvalidActiveXStateException ("GetCharacterSREngine", AxHost::ActiveXInvokeKind::MethodInvoke);
	}
	return AxInterface->GetCharacterSREngine (LoadKey);
}

DoubleAgent::Control::SREngines^ AxControl::FindCharacterSREngines (Object^ LoadKey, Object^ LanguageID)
{
    if  (AxInterface == nullptr)
    {
        throw gcnew AxHost::InvalidActiveXStateException ("FindCharacterSREngines", AxHost::ActiveXInvokeKind::MethodInvoke);
    }
    return AxInterface->FindCharacterSREngines (LoadKey, LanguageID);
}

void AxControl::ShowDefaultCharacterProperties (Object^ X, Object^ Y)
{
    if  (AxInterface == nullptr)
    {
        throw gcnew AxHost::InvalidActiveXStateException ("ShowDefaultCharacterProperties", AxHost::ActiveXInvokeKind::MethodInvoke);
    }
    AxInterface->ShowDefaultCharacterProperties (X, Y);
}

void AxControl::ShowDefaultCharacterProperties ()
{
    AxInterface->ShowDefaultCharacterProperties (Type::Missing, Type::Missing);
}

/////////////////////////////////////////////////////////////////////////////
//page
/////////////////////////////////////////////////////////////////////////////

void AxControl::PopupActivateInput::add (AxPopupActivateInputHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::PopupActivateInput::add"));
#endif
	mActivateInputEvent = static_cast<AxPopupActivateInputHandler^> (Delegate::Combine (mActivateInputEvent, pDelegate));
}

void AxControl::PopupActivateInput::remove (AxPopupActivateInputHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::PopupActivateInput::remove"));
#endif
	mActivateInputEvent = static_cast<AxPopupActivateInputHandler^> (Delegate::Remove (mActivateInputEvent, pDelegate));
}

void AxControl::PopupActivateInput::raise (Object^ pSender, AxPopupActivateInputEvent^ pEvent)
{
	if  (mActivateInputEvent != nullptr)
	{
#ifdef	_DEBUG_EVENTS
		LogMessage (_DEBUG_EVENTS, _T("AxControl::PopupActivateInput::raise"));
#endif
		mActivateInputEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::PopupDeactivateInput::add (AxPopupDeactivateInputHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::PopupDeactivateInput::add"));
#endif
	mDeactivateInputEvent = static_cast<AxPopupDeactivateInputHandler^> (Delegate::Combine (mDeactivateInputEvent, pDelegate));
}

void AxControl::PopupDeactivateInput::remove (AxPopupDeactivateInputHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::PopupDeactivateInput::remove"));
#endif
	mDeactivateInputEvent = static_cast<AxPopupDeactivateInputHandler^> (Delegate::Remove (mDeactivateInputEvent, pDelegate));
}

void AxControl::PopupDeactivateInput::raise (Object^ pSender, AxPopupDeactivateInputEvent^ pEvent)
{
	if  (mDeactivateInputEvent != nullptr)
	{
#ifdef	_DEBUG_EVENTS
		LogMessage (_DEBUG_EVENTS, _T("AxControl::PopupDeactivateInput::raise"));
#endif
		mDeactivateInputEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::PopupClick::add (AxPopupClickHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::PopupClick::add"));
#endif
	mClickEvent = static_cast<AxPopupClickHandler^> (Delegate::Combine (mClickEvent, pDelegate));
}

void AxControl::PopupClick::remove (AxPopupClickHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::PopupClick::remove"));
#endif
	mClickEvent = static_cast<AxPopupClickHandler^> (Delegate::Remove (mClickEvent, pDelegate));
}

void AxControl::PopupClick::raise (Object^ pSender, AxPopupClickEvent^ pEvent)
{
	if  (mClickEvent != nullptr)
	{
#ifdef	_DEBUG_EVENTS
		LogMessage (_DEBUG_EVENTS, _T("AxControl::PopupClick::raise"));
#endif
		mClickEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::PopupDblClick::add (AxPopupDblClickHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::PopupDblClick::add"));
#endif
	mDblClickEvent = static_cast<AxPopupDblClickHandler^> (Delegate::Combine (mDblClickEvent, pDelegate));
}

void AxControl::PopupDblClick::remove (AxPopupDblClickHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::PopupDblClick::remove"));
#endif
	mDblClickEvent = static_cast<AxPopupDblClickHandler^> (Delegate::Remove (mDblClickEvent, pDelegate));
}

void AxControl::PopupDblClick::raise (Object^ pSender, AxPopupDblClickEvent^ pEvent)
{
	if  (mDblClickEvent != nullptr)
	{
#ifdef	_DEBUG_EVENTS
		LogMessage (_DEBUG_EVENTS, _T("AxControl::PopupDblClick::raise"));
#endif
		mDblClickEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::PopupDragStart::add (AxPopupDragStartHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::PopupDragStart::add"));
#endif
	mDragStartEvent = static_cast<AxPopupDragStartHandler^> (Delegate::Combine (mDragStartEvent, pDelegate));
}

void AxControl::PopupDragStart::remove (AxPopupDragStartHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::PopupDragStart::remove"));
#endif
	mDragStartEvent = static_cast<AxPopupDragStartHandler^> (Delegate::Remove (mDragStartEvent, pDelegate));
}

void AxControl::PopupDragStart::raise (Object^ pSender, AxPopupDragStartEvent^ pEvent)
{
	if  (mDragStartEvent != nullptr)
	{
#ifdef	_DEBUG_EVENTS
		LogMessage (_DEBUG_EVENTS, _T("AxControl::PopupDragStart::raise"));
#endif
		mDragStartEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::PopupDragComplete::add (AxPopupDragCompleteHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::PopupDragComplete::add"));
#endif
	mDragCompleteEvent = static_cast<AxPopupDragCompleteHandler^> (Delegate::Combine (mDragCompleteEvent, pDelegate));
}

void AxControl::PopupDragComplete::remove (AxPopupDragCompleteHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::PopupDragComplete::remove"));
#endif
	mDragCompleteEvent = static_cast<AxPopupDragCompleteHandler^> (Delegate::Remove (mDragCompleteEvent, pDelegate));
}

void AxControl::PopupDragComplete::raise (Object^ pSender, AxPopupDragCompleteEvent^ pEvent)
{
	if  (mDragCompleteEvent != nullptr)
	{
#ifdef	_DEBUG_EVENTS
		LogMessage (_DEBUG_EVENTS, _T("AxControl::PopupDragComplete::raise"));
#endif
		mDragCompleteEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::ShowEvent::add (AxShowHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::ShowEvent::add"));
#endif
	mShowEvent = static_cast<AxShowHandler^> (Delegate::Combine (mShowEvent, pDelegate));
}

void AxControl::ShowEvent::remove (AxShowHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::ShowEvent::remove"));
#endif
	mShowEvent = static_cast<AxShowHandler^> (Delegate::Remove (mShowEvent, pDelegate));
}

void AxControl::ShowEvent::raise (Object^ pSender, AxShowEvent^ pEvent)
{
	if  (mShowEvent != nullptr)
	{
#ifdef	_DEBUG_EVENTS
		LogMessage (_DEBUG_EVENTS, _T("AxControl::ShowEvent::raise"));
#endif
		mShowEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::HideEvent::add (AxHideHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::HideEvent::add"));
#endif
	mHideEvent = static_cast<AxHideHandler^> (Delegate::Combine (mHideEvent, pDelegate));
}

void AxControl::HideEvent::remove (AxHideHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::HideEvent::remove"));
#endif
	mHideEvent = static_cast<AxHideHandler^> (Delegate::Remove (mHideEvent, pDelegate));
}

void AxControl::HideEvent::raise (Object^ pSender, AxHideEvent^ pEvent)
{
	if  (mHideEvent != nullptr)
	{
#ifdef	_DEBUG_EVENTS
		LogMessage (_DEBUG_EVENTS, _T("AxControl::HideEvent::raise"));
#endif
		mHideEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::RequestStart::add (AxRequestStartHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::RequestStart::add"));
#endif
	mRequestStartEvent = static_cast<AxRequestStartHandler^> (Delegate::Combine (mRequestStartEvent, pDelegate));
}

void AxControl::RequestStart::remove (AxRequestStartHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::RequestStart::remove"));
#endif
	mRequestStartEvent = static_cast<AxRequestStartHandler^> (Delegate::Remove (mRequestStartEvent, pDelegate));
}

void AxControl::RequestStart::raise (Object^ pSender, AxRequestStartEvent^ pEvent)
{
	if  (mRequestStartEvent != nullptr)
	{
#ifdef	_DEBUG_EVENTS
		LogMessage (_DEBUG_EVENTS, _T("AxControl::RequestStart::raise"));
#endif
		mRequestStartEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::RequestComplete::add (AxRequestCompleteHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::RequestComplete::add"));
#endif
	mRequestCompleteEvent = static_cast<AxRequestCompleteHandler^> (Delegate::Combine (mRequestCompleteEvent, pDelegate));
}

void AxControl::RequestComplete::remove (AxRequestCompleteHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::RequestComplete::remove"));
#endif
	mRequestCompleteEvent = static_cast<AxRequestCompleteHandler^> (Delegate::Remove (mRequestCompleteEvent, pDelegate));
}

void AxControl::RequestComplete::raise (Object^ pSender, AxRequestCompleteEvent^ pEvent)
{
	if  (mRequestCompleteEvent != nullptr)
	{
#ifdef	_DEBUG_EVENTS
		LogMessage (_DEBUG_EVENTS, _T("AxControl::RequestComplete::raise"));
#endif
		mRequestCompleteEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::Bookmark::add (AxBookmarkHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::Bookmark::add"));
#endif
	mBookmarkEvent = static_cast<AxBookmarkHandler^> (Delegate::Combine (mBookmarkEvent, pDelegate));
}

void AxControl::Bookmark::remove (AxBookmarkHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::Bookmark::remove"));
#endif
	mBookmarkEvent = static_cast<AxBookmarkHandler^> (Delegate::Remove (mBookmarkEvent, pDelegate));
}

void AxControl::Bookmark::raise (Object^ pSender, AxBookmarkEvent^ pEvent)
{
	if  (mBookmarkEvent != nullptr)
	{
#ifdef	_DEBUG_EVENTS
		LogMessage (_DEBUG_EVENTS, _T("AxControl::Bookmark::raise"));
#endif
		mBookmarkEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::Command::add (AxCommandHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::Command::add"));
#endif
	mCommandEvent = static_cast<AxCommandHandler^> (Delegate::Combine (mCommandEvent, pDelegate));
}

void AxControl::Command::remove (AxCommandHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::Command::remove"));
#endif
	mCommandEvent = static_cast<AxCommandHandler^> (Delegate::Remove (mCommandEvent, pDelegate));
}

void AxControl::Command::raise (Object^ pSender, AxCommandEvent^ pEvent)
{
	if  (mCommandEvent != nullptr)
	{
#ifdef	_DEBUG_EVENTS
		LogMessage (_DEBUG_EVENTS, _T("AxControl::Command::raise"));
#endif
		mCommandEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::IdleStart::add (AxIdleStartHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::IdleStart::add"));
#endif
	mIdleStartEvent = static_cast<AxIdleStartHandler^> (Delegate::Combine (mIdleStartEvent, pDelegate));
}

void AxControl::IdleStart::remove (AxIdleStartHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::IdleStart::remove"));
#endif
	mIdleStartEvent = static_cast<AxIdleStartHandler^> (Delegate::Remove (mIdleStartEvent, pDelegate));
}

void AxControl::IdleStart::raise (Object^ pSender, AxIdleStartEvent^ pEvent)
{
	if  (mIdleStartEvent != nullptr)
	{
#ifdef	_DEBUG_EVENTS
		LogMessage (_DEBUG_EVENTS, _T("AxControl::IdleStart::raise"));
#endif
		mIdleStartEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::IdleComplete::add (AxIdleCompleteHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::IdleComplete::add"));
#endif
	mIdleCompleteEvent = static_cast<AxIdleCompleteHandler^> (Delegate::Combine (mIdleCompleteEvent, pDelegate));
}

void AxControl::IdleComplete::remove (AxIdleCompleteHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::IdleComplete::remove"));
#endif
	mIdleCompleteEvent = static_cast<AxIdleCompleteHandler^> (Delegate::Remove (mIdleCompleteEvent, pDelegate));
}

void AxControl::IdleComplete::raise (Object^ pSender, AxIdleCompleteEvent^ pEvent)
{
	if  (mIdleCompleteEvent != nullptr)
	{
#ifdef	_DEBUG_EVENTS
		LogMessage (_DEBUG_EVENTS, _T("AxControl::IdleComplete::raise"));
#endif
		mIdleCompleteEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::MoveEvent::add (AxMoveHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::MoveEvent::add"));
#endif
	mMoveEvent = static_cast<AxMoveHandler^> (Delegate::Combine (mMoveEvent, pDelegate));
}

void AxControl::MoveEvent::remove (AxMoveHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::MoveEvent::remove"));
#endif
	mMoveEvent = static_cast<AxMoveHandler^> (Delegate::Remove (mMoveEvent, pDelegate));
}

void AxControl::MoveEvent::raise (Object^ pSender, AxMoveEvent^ pEvent)
{
	if  (mMoveEvent != nullptr)
	{
#ifdef	_DEBUG_EVENTS
		LogMessage (_DEBUG_EVENTS, _T("AxControl::MoveEvent::raise"));
#endif
		mMoveEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::SizeEvent::add (AxSizeHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::SizeEvent::add"));
#endif
	mSizeEvent = static_cast<AxSizeHandler^> (Delegate::Combine (mSizeEvent, pDelegate));
}

void AxControl::SizeEvent::remove (AxSizeHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::SizeEvent::remove"));
#endif
	mSizeEvent = static_cast<AxSizeHandler^> (Delegate::Remove (mSizeEvent, pDelegate));
}

void AxControl::SizeEvent::raise (Object^ pSender, AxSizeEvent^ pEvent)
{
	if  (mSizeEvent != nullptr)
	{
#ifdef	_DEBUG_EVENTS
		LogMessage (_DEBUG_EVENTS, _T("AxControl::SizeEvent::raise"));
#endif
		mSizeEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::BalloonShow::add (AxBalloonShowHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::BalloonShow::add"));
#endif
	mBalloonShowEvent = static_cast<AxBalloonShowHandler^> (Delegate::Combine (mBalloonShowEvent, pDelegate));
}

void AxControl::BalloonShow::remove (AxBalloonShowHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::BalloonShow::remove"));
#endif
	mBalloonShowEvent = static_cast<AxBalloonShowHandler^> (Delegate::Remove (mBalloonShowEvent, pDelegate));
}

void AxControl::BalloonShow::raise (Object^ pSender, AxBalloonShowEvent^ pEvent)
{
	if  (mBalloonShowEvent != nullptr)
	{
#ifdef	_DEBUG_EVENTS
		LogMessage (_DEBUG_EVENTS, _T("AxControl::BalloonShow::raise"));
#endif
		mBalloonShowEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::BalloonHide::add (AxBalloonHideHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::BalloonHide::add"));
#endif
	mBalloonHideEvent = static_cast<AxBalloonHideHandler^> (Delegate::Combine (mBalloonHideEvent, pDelegate));
}

void AxControl::BalloonHide::remove (AxBalloonHideHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::BalloonHide::remove"));
#endif
	mBalloonHideEvent = static_cast<AxBalloonHideHandler^> (Delegate::Remove (mBalloonHideEvent, pDelegate));
}

void AxControl::BalloonHide::raise (Object^ pSender, AxBalloonHideEvent^ pEvent)
{
	if  (mBalloonHideEvent != nullptr)
	{
#ifdef	_DEBUG_EVENTS
		LogMessage (_DEBUG_EVENTS, _T("AxControl::BalloonHide::raise"));
#endif
		mBalloonHideEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::ListenStart::add (AxListenStartHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::ListenStart::add"));
#endif
	mListenStartEvent = static_cast<AxListenStartHandler^> (Delegate::Combine (mListenStartEvent, pDelegate));
}

void AxControl::ListenStart::remove (AxListenStartHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::ListenStart::remove"));
#endif
	mListenStartEvent = static_cast<AxListenStartHandler^> (Delegate::Remove (mListenStartEvent, pDelegate));
}

void AxControl::ListenStart::raise (Object^ pSender, AxListenStartEvent^ pEvent)
{
	if  (mListenStartEvent != nullptr)
	{
#ifdef	_DEBUG_EVENTS
		LogMessage (_DEBUG_EVENTS, _T("AxControl::ListenStart::raise"));
#endif
		mListenStartEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::ListenComplete::add (AxListenCompleteHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::ListenComplete::add"));
#endif
	mListenCompleteEvent = static_cast<AxListenCompleteHandler^> (Delegate::Combine (mListenCompleteEvent, pDelegate));
}

void AxControl::ListenComplete::remove (AxListenCompleteHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::ListenComplete::remove"));
#endif
	mListenCompleteEvent = static_cast<AxListenCompleteHandler^> (Delegate::Remove (mListenCompleteEvent, pDelegate));
}

void AxControl::ListenComplete::raise (Object^ pSender, AxListenCompleteEvent^ pEvent)
{
	if  (mListenCompleteEvent != nullptr)
	{
#ifdef	_DEBUG_EVENTS
		LogMessage (_DEBUG_EVENTS, _T("AxControl::ListenComplete::raise"));
#endif
		mListenCompleteEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::DefaultCharacterChange::add (AxDefaultCharacterChangeHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::DefaultCharacterChange::add"));
#endif
	mDefaultCharacterChangeEvent = static_cast<AxDefaultCharacterChangeHandler^> (Delegate::Combine (mDefaultCharacterChangeEvent, pDelegate));
}

void AxControl::DefaultCharacterChange::remove (AxDefaultCharacterChangeHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::DefaultCharacterChange::remove"));
#endif
	mDefaultCharacterChangeEvent = static_cast<AxDefaultCharacterChangeHandler^> (Delegate::Remove (mDefaultCharacterChangeEvent, pDelegate));
}

void AxControl::DefaultCharacterChange::raise (Object^ pSender, AxDefaultCharacterChangeEvent^ pEvent)
{
	if  (mDefaultCharacterChangeEvent != nullptr)
	{
#ifdef	_DEBUG_EVENTS
		LogMessage (_DEBUG_EVENTS, _T("AxControl::DefaultCharacterChange::raise"));
#endif
		mDefaultCharacterChangeEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::AgentPropertyChange::add (System::EventHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::AgentPropertyChange::add"));
#endif
	mAgentPropertyChangeEvent = static_cast<System::EventHandler^> (Delegate::Combine (mAgentPropertyChangeEvent, pDelegate));
}

void AxControl::AgentPropertyChange::remove (System::EventHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::AgentPropertyChange::remove"));
#endif
	mAgentPropertyChangeEvent = static_cast<System::EventHandler^> (Delegate::Remove (mAgentPropertyChangeEvent, pDelegate));
}

void AxControl::AgentPropertyChange::raise (Object^ pSender, System::EventArgs^ pEvent)
{
	if  (mAgentPropertyChangeEvent != nullptr)
	{
#ifdef	_DEBUG_EVENTS
		LogMessage (_DEBUG_EVENTS, _T("AxControl::AgentPropertyChange::raise"));
#endif
		mAgentPropertyChangeEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AxControl::ActiveClientChange::add (AxActiveClientChangeHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::ActiveClientChange::add"));
#endif
	mActiveClientChangeEvent = static_cast<AxActiveClientChangeHandler^> (Delegate::Combine (mActiveClientChangeEvent, pDelegate));
}

void AxControl::ActiveClientChange::remove (AxActiveClientChangeHandler^ pDelegate)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControl::ActiveClientChange::remove"));
#endif
	mActiveClientChangeEvent = static_cast<AxActiveClientChangeHandler^> (Delegate::Remove (mActiveClientChangeEvent, pDelegate));
}

void AxControl::ActiveClientChange::raise (Object^ pSender, AxActiveClientChangeEvent^ pEvent)
{
	if  (mActiveClientChangeEvent != nullptr)
	{
#ifdef	_DEBUG_EVENTS
		LogMessage (_DEBUG_EVENTS, _T("AxControl::ActiveClientChange::raise"));
#endif
		mActiveClientChangeEvent (pSender, pEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////
//page
/////////////////////////////////////////////////////////////////////////////

void AxControlEvents::ActivateInput (String^ CharacterID)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControlEvents::ActivateInput [%s]"), PtrToStringChars(CharacterID));
#endif
	DoubleAgent::AxControl::AxControl::AxPopupActivateInputEvent^ lEvent = gcnew DoubleAgent::AxControl::AxControl::AxPopupActivateInputEvent (CharacterID);
	mParent->PopupActivateInput::raise (mParent, lEvent);
}

void AxControlEvents::DeactivateInput (String^ CharacterID)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControlEvents::DeactivateInput [%s]"), PtrToStringChars(CharacterID));
#endif
	DoubleAgent::AxControl::AxControl::AxPopupDeactivateInputEvent^ lEvent = gcnew DoubleAgent::AxControl::AxControl::AxPopupDeactivateInputEvent (CharacterID);
	mParent->PopupDeactivateInput::raise (mParent, lEvent);
}

void AxControlEvents::Click (String^ CharacterID, short Button, short Shift, short X, short Y)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControlEvents::Click [%s]"), PtrToStringChars(CharacterID));
#endif
	DoubleAgent::AxControl::AxControl::AxPopupClickEvent^ lEvent = gcnew DoubleAgent::AxControl::AxControl::AxPopupClickEvent (CharacterID, Button, Shift, X, Y);
	mParent->PopupClick::raise (mParent, lEvent);
}

void AxControlEvents::DblClick (String^ CharacterID, short Button, short Shift, short X, short Y)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControlEvents::DblClick [%s]"), PtrToStringChars(CharacterID));
#endif
	DoubleAgent::AxControl::AxControl::AxPopupDblClickEvent^ lEvent = gcnew DoubleAgent::AxControl::AxControl::AxPopupDblClickEvent (CharacterID, Button, Shift, X, Y);
	mParent->PopupDblClick::raise (mParent, lEvent);
}

void AxControlEvents::DragStart (String^ CharacterID, short Button, short Shift, short X, short Y)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControlEvents::DragStart [%s]"), PtrToStringChars(CharacterID));
#endif
	DoubleAgent::AxControl::AxControl::AxPopupDragStartEvent^ lEvent = gcnew DoubleAgent::AxControl::AxControl::AxPopupDragStartEvent (CharacterID, Button, Shift, X, Y);
	mParent->PopupDragStart::raise (mParent, lEvent);
}

void AxControlEvents::DragComplete (String^ CharacterID, short Button, short Shift, short X, short Y)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControlEvents::DragComplete [%s]"), PtrToStringChars(CharacterID));
#endif
	DoubleAgent::AxControl::AxControl::AxPopupDragCompleteEvent^ lEvent = gcnew DoubleAgent::AxControl::AxControl::AxPopupDragCompleteEvent (CharacterID, Button, Shift, X, Y);
	mParent->PopupDragComplete::raise (mParent, lEvent);
}

void AxControlEvents::Show (String^ CharacterID, DoubleAgent::Control::VisibilityCauseType Cause)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControlEvents::Show [%s]"), PtrToStringChars(CharacterID));
#endif
	DoubleAgent::AxControl::AxControl::AxShowEvent^ lEvent = gcnew DoubleAgent::AxControl::AxControl::AxShowEvent (CharacterID, Cause);
	mParent->ShowEvent::raise (mParent, lEvent);
}

void AxControlEvents::Hide (String^ CharacterID, DoubleAgent::Control::VisibilityCauseType Cause)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControlEvents::Hide [%s]"), PtrToStringChars(CharacterID));
#endif
	DoubleAgent::AxControl::AxControl::AxHideEvent^ lEvent = gcnew DoubleAgent::AxControl::AxControl::AxHideEvent (CharacterID, Cause);
	mParent->HideEvent::raise (mParent, lEvent);
}

void AxControlEvents::RequestStart (DoubleAgent::Control::Request^ Request)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControlEvents::RequestStart [%d]"), Request->default);
#endif
	DoubleAgent::AxControl::AxControl::AxRequestStartEvent^ lEvent = gcnew DoubleAgent::AxControl::AxControl::AxRequestStartEvent (Request);
	mParent->RequestStart::raise (mParent, lEvent);
}

void AxControlEvents::RequestComplete (DoubleAgent::Control::Request^ Request)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControlEvents::RequestComplete [%d]"), Request->default);
#endif
	DoubleAgent::AxControl::AxControl::AxRequestCompleteEvent^ lEvent = gcnew DoubleAgent::AxControl::AxControl::AxRequestCompleteEvent (Request);
	mParent->RequestComplete::raise (mParent, lEvent);
}

void AxControlEvents::Bookmark (int BookmarkID)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControlEvents::Bookmark [%d]"), BookmarkID);
#endif
	DoubleAgent::AxControl::AxControl::AxBookmarkEvent^ lEvent = gcnew DoubleAgent::AxControl::AxControl::AxBookmarkEvent (BookmarkID);
	mParent->Bookmark::raise (mParent, lEvent);
}

void AxControlEvents::Command (DoubleAgent::Control::UserInput^ UserInput)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControlEvents::Command [%s]"), PtrToStringChars(UserInput->Name));
#endif
	DoubleAgent::AxControl::AxControl::AxCommandEvent^ lEvent = gcnew DoubleAgent::AxControl::AxControl::AxCommandEvent (UserInput);
	mParent->Command::raise (mParent, lEvent);
}

void AxControlEvents::IdleStart (String^ CharacterID)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControlEvents::IdleStart [%s]"), PtrToStringChars(CharacterID));
#endif
	DoubleAgent::AxControl::AxControl::AxIdleStartEvent^ lEvent = gcnew DoubleAgent::AxControl::AxControl::AxIdleStartEvent (CharacterID);
	mParent->IdleStart::raise (mParent, lEvent);
}

void AxControlEvents::IdleComplete (String^ CharacterID)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControlEvents::IdleComplete [%d]"), PtrToStringChars(CharacterID));
#endif
	DoubleAgent::AxControl::AxControl::AxIdleCompleteEvent^ lEvent = gcnew DoubleAgent::AxControl::AxControl::AxIdleCompleteEvent (CharacterID);
	mParent->IdleComplete::raise (mParent, lEvent);
}

void AxControlEvents::Move (String^ CharacterID, short X, short Y, DoubleAgent::Control::MoveCauseType Cause)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControlEvents::Move [%s]"), PtrToStringChars(CharacterID));
#endif
	DoubleAgent::AxControl::AxControl::AxMoveEvent^ lEvent = gcnew DoubleAgent::AxControl::AxControl::AxMoveEvent (CharacterID, X, Y, Cause);
	mParent->MoveEvent::raise (mParent, lEvent);
}

void AxControlEvents::Size (String^ CharacterID, short Width, short Height)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControlEvents::Size [%s]"), PtrToStringChars(CharacterID));
#endif
	DoubleAgent::AxControl::AxControl::AxSizeEvent^ lEvent = gcnew DoubleAgent::AxControl::AxControl::AxSizeEvent (CharacterID, Width, Height);
	mParent->SizeEvent::raise (mParent, lEvent);
}

void AxControlEvents::BalloonShow (String^ CharacterID)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControlEvents::BalloonShow [%s]"), PtrToStringChars(CharacterID));
#endif
	DoubleAgent::AxControl::AxControl::AxBalloonShowEvent^ lEvent = gcnew DoubleAgent::AxControl::AxControl::AxBalloonShowEvent (CharacterID);
	mParent->BalloonShow::raise (mParent, lEvent);
}

void AxControlEvents::BalloonHide (String^ CharacterID)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControlEvents::BalloonHide [%s]"), PtrToStringChars(CharacterID));
#endif
	DoubleAgent::AxControl::AxControl::AxBalloonHideEvent^ lEvent = gcnew DoubleAgent::AxControl::AxControl::AxBalloonHideEvent (CharacterID);
	mParent->BalloonHide::raise (mParent, lEvent);
}

void AxControlEvents::ListenStart (String^ CharacterID)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControlEvents::ListenStart [%s]"), PtrToStringChars(CharacterID));
#endif
	DoubleAgent::AxControl::AxControl::AxListenStartEvent^ lEvent = gcnew DoubleAgent::AxControl::AxControl::AxListenStartEvent (CharacterID);
	mParent->ListenStart::raise (mParent, lEvent);
}

void AxControlEvents::ListenComplete (String^ CharacterID, DoubleAgent::Control::ListenCompleteType Cause)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControlEvents::ListenComplete [%s]"), PtrToStringChars(CharacterID));
#endif
	DoubleAgent::AxControl::AxControl::AxListenCompleteEvent^ lEvent = gcnew DoubleAgent::AxControl::AxControl::AxListenCompleteEvent (CharacterID, Cause);
	mParent->ListenComplete::raise (mParent, lEvent);
}

void AxControlEvents::DefaultCharacterChange (String^ CharGUID)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControlEvents::DefaultCharacterChange [%s]"), PtrToStringChars(CharGUID));
#endif
	DoubleAgent::AxControl::AxControl::AxDefaultCharacterChangeEvent^ lEvent = gcnew DoubleAgent::AxControl::AxControl::AxDefaultCharacterChangeEvent (CharGUID);
	mParent->DefaultCharacterChange::raise (mParent, lEvent);
}

void AxControlEvents::AgentPropertyChange ()
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControlEvents::AgentPropertyChange"));
#endif
	System::EventArgs^ lEvent = gcnew System::EventArgs ();
	mParent->AgentPropertyChange::raise (mParent, lEvent);
}

void AxControlEvents::ActiveClientChange (String^ CharacterID, bool Active)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("AxControlEvents::ActiveClientChange [%s]"), PtrToStringChars(CharacterID));
#endif
	DoubleAgent::AxControl::AxControl::AxActiveClientChangeEvent^ lEvent = gcnew DoubleAgent::AxControl::AxControl::AxActiveClientChangeEvent (CharacterID, Active);
	mParent->ActiveClientChange::raise (mParent, lEvent);
}

/////////////////////////////////////////////////////////////////////////////
}	// namespace AxControl
}	// namespace DoubleAgent