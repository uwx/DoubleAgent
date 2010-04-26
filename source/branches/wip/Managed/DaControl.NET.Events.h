/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent ActiveX Control.

    The Double Agent ActiveX Control is free software:
    you can redistribute it and/or modify it under the terms of the
    GNU Lesser Public License as published by the Free Software Foundation,
    either version 3 of the License, or  (at your option) any later version.

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

namespace DoubleAgent {
namespace Control {

#if	FALSE
typedef DoubleAgentCtl::DaControl				IControl;
typedef DoubleAgentCtl::DaCtlCharacters			ICharacters;
typedef DoubleAgentCtl::DaCtlCharacterFiles		ICharacterFiles;
typedef DoubleAgentCtl::DaCtlRequest			IRequest;
typedef DoubleAgentCtl::DaCtlUserInput			IUserInput;
typedef DoubleAgentCtl::DaCtlAudioOutput		IAudioOutput;
typedef DoubleAgentCtl::DaCtlSpeechInput		ISpeechInput;
typedef DoubleAgentCtl::DaCtlPropertySheet		IPropertySheet;
typedef DoubleAgentCtl::DaCtlCommandsWindow		ICommandsWindow;
typedef DoubleAgentCtl::DaCtlTTSEngines			ITTSEngines;
typedef DoubleAgentCtl::DaCtlTTSEngine			ITTSEngine;
typedef DoubleAgentCtl::DaCtlSREngines			ISREngines;
typedef DoubleAgentCtl::DaCtlSREngine			ISREngine;
typedef DoubleAgentCtl::DaCtlSettings			ISettings;
typedef DoubleAgentCtl::VisibilityCauseType		VisibilityCauseType;
typedef DoubleAgentCtl::MoveCauseType			MoveCauseType;
typedef DoubleAgentCtl::ListenCompleteType		ListenCompleteType;
namespace Native {
typedef DoubleAgentCtl::_DaCtlEvents			_DaCtlEvents;
}
#endif

namespace Events {
/////////////////////////////////////////////////////////////////////////////

public ref class ActivateInputEvent sealed
{
public:
	String^	CharacterID;
	
internal:	
	ActivateInputEvent (String^ pCharacterID)
	{
		CharacterID = pCharacterID;
	}
};

/////////////////////////////////////////////////////////////////////////////

public ref class DeactivateInputEvent sealed
{
public:
	String^	CharacterID;
	
internal:	
	DeactivateInputEvent (String^ pCharacterID)
	{
		CharacterID = pCharacterID;
	}
};

/////////////////////////////////////////////////////////////////////////////

public ref class ClickEvent sealed
{
public:
	String^	CharacterID;
	short	Button;
	short	Shift;
	short	X;
	short	Y;
	
internal:	
	ClickEvent (String^ pCharacterID, short pButton, short pShift, short pX, short pY)
	{
		CharacterID = pCharacterID;
		Button = pButton;
		Shift = pShift;
		X = pX;
		Y = pY;
	}
};

/////////////////////////////////////////////////////////////////////////////

public ref class DblClickEvent sealed
{
public:
	String^	CharacterID;
	short	Button;
	short	Shift;
	short	X;
	short	Y;
	
internal:	
	DblClickEvent (String^ pCharacterID, short pButton, short pShift, short pX, short pY)
	{
		CharacterID = pCharacterID;
		Button = pButton;
		Shift = pShift;
		X = pX;
		Y = pY;
	}
};

/////////////////////////////////////////////////////////////////////////////

public ref class DragStartEvent sealed
{
public:
	String^	CharacterID;
	short	Button;
	short	Shift;
	short	X;
	short	Y;
	
internal:	
	DragStartEvent (String^ pCharacterID, short pButton, short pShift, short pX, short pY)
	{
		CharacterID = pCharacterID;
		Button = pButton;
		Shift = pShift;
		X = pX;
		Y = pY;
	}
};

/////////////////////////////////////////////////////////////////////////////

public ref class DragCompleteEvent sealed
{
public:
	String^	CharacterID;
	short	Button;
	short	Shift;
	short	X;
	short	Y;

internal:	
	DragCompleteEvent (String^ pCharacterID, short pButton, short pShift, short pX, short pY)
	{
		CharacterID = pCharacterID;
		Button = pButton;
		Shift = pShift;
		X = pX;
		Y = pY;
	}
};

/////////////////////////////////////////////////////////////////////////////

public ref class ShowEvent sealed
{
public:
	String^										CharacterID;
	DoubleAgent::Control::VisibilityCauseType	Cause;
	
internal:	
	ShowEvent (String^ pCharacterID, DoubleAgent::Control::VisibilityCauseType pCause)
	{
		CharacterID = pCharacterID;
		Cause = pCause;
	}
};

/////////////////////////////////////////////////////////////////////////////

public ref class HideEvent sealed
{
public:
	String^										CharacterID;
	DoubleAgent::Control::VisibilityCauseType	Cause;
	
internal:	
	HideEvent (String^ pCharacterID, DoubleAgent::Control::VisibilityCauseType pCause)
	{
		CharacterID = pCharacterID;
		Cause = pCause;
	}
};

/////////////////////////////////////////////////////////////////////////////

public ref class RequestStartEvent sealed
{
public:
	DoubleAgent::Control::IRequest^	Request;
	
internal:	
	RequestStartEvent (DoubleAgent::Control::IRequest^ pRequest)
	{
		Request = pRequest;
	}
};

/////////////////////////////////////////////////////////////////////////////

public ref class RequestCompleteEvent sealed
{
public:
	DoubleAgent::Control::IRequest^	Request;
	
internal:	
	RequestCompleteEvent (DoubleAgent::Control::IRequest^ pRequest)
	{
		Request = pRequest;
	}
};

/////////////////////////////////////////////////////////////////////////////

public ref class BookmarkEvent sealed
{
public:
	int	BookmarkID;
	
internal:	
	BookmarkEvent (int pBookmarkID)
	{
		BookmarkID = pBookmarkID;
	}
};

/////////////////////////////////////////////////////////////////////////////

public ref class CommandEvent sealed
{
public:
	DoubleAgent::Control::IUserInput^	UserInput;
	
internal:	
	CommandEvent (DoubleAgent::Control::IUserInput^ pUserInput)
	{
		UserInput = pUserInput;
	}
};

/////////////////////////////////////////////////////////////////////////////

public ref class IdleStartEvent sealed
{
public:
	String^	CharacterID;
	
internal:	
	IdleStartEvent (String^ pCharacterID)
	{
		CharacterID = pCharacterID;
	}
};

/////////////////////////////////////////////////////////////////////////////

public ref class IdleCompleteEvent sealed
{
public:
	String^	CharacterID;
	
internal:	
	IdleCompleteEvent (String^ pCharacterID)
	{
		CharacterID = pCharacterID;
	}
};

/////////////////////////////////////////////////////////////////////////////

public ref class MoveEvent sealed
{
public:
	String^								CharacterID;
	short								X;
	short								Y;
	DoubleAgent::Control::MoveCauseType	Cause;
	
internal:	
	MoveEvent (String^ pCharacterID, short pX, short pY, DoubleAgent::Control::MoveCauseType pCause)
	{
		CharacterID = pCharacterID;
		X = pX;
		Y = pY;
		Cause = pCause;
	}
};

/////////////////////////////////////////////////////////////////////////////

public ref class SizeEvent sealed
{
public:
	String^	CharacterID;
	short	Width;
	short	Height;
	
internal:	
	SizeEvent (String^ pCharacterID, short pWidth, short pHeight)
	{
		CharacterID = pCharacterID;
		Width = pWidth;
		Height = pHeight;
	}
};

/////////////////////////////////////////////////////////////////////////////

public ref class BalloonShowEvent sealed
{
public:
	String^	CharacterID;
	
internal:	
	BalloonShowEvent (String^ pCharacterID)
	{
		CharacterID = pCharacterID;
	}
};

/////////////////////////////////////////////////////////////////////////////

public ref class BalloonHideEvent sealed
{
public:
	String^	CharacterID;
	
internal:	
	BalloonHideEvent (String^ pCharacterID)
	{
		CharacterID = pCharacterID;
	}
};

/////////////////////////////////////////////////////////////////////////////

public ref class ListenStartEvent sealed
{
public:
	String^	CharacterID;
	
internal:	
	ListenStartEvent (String^ pCharacterID)
	{
		CharacterID = pCharacterID;
	}
};

/////////////////////////////////////////////////////////////////////////////

public ref class ListenCompleteEvent sealed
{
public:
	String^										CharacterID;
	DoubleAgent::Control::ListenCompleteType	Cause;
	
internal:	
	ListenCompleteEvent (String^ pCharacterID, DoubleAgent::Control::ListenCompleteType pCause)
	{
		CharacterID = pCharacterID;
		Cause = pCause;
	}
};

/////////////////////////////////////////////////////////////////////////////

public ref class DefaultCharacterChangeEvent sealed
{
public:
	String^	CharGUID;
	
internal:	
	DefaultCharacterChangeEvent (String^ pCharGUID)
	{
		CharGUID = pCharGUID;
	}
};

/////////////////////////////////////////////////////////////////////////////

public ref class ActiveClientChangeEvent sealed
{
public:
	String^	CharacterID;
	bool	Active;
	
internal:	
	ActiveClientChangeEvent (String^ pCharacterID, bool pActive)
	{
		CharacterID = pCharacterID;
		Active = pActive;
	}
};

}	// namespace Events
/////////////////////////////////////////////////////////////////////////////
//page
/////////////////////////////////////////////////////////////////////////////

private ref class AxControlEvents :
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
	virtual void RequestStart (DoubleAgent::Control::IRequest^ Request);
	virtual void RequestComplete (DoubleAgent::Control::IRequest^ Request);
	virtual void Restart () {}
	virtual void Shutdown () {}
	virtual void Bookmark (int BookmarkID);
	virtual void Command (DoubleAgent::Control::IUserInput^ UserInput);
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
}	// namespace Control
}	// namespace DoubleAgent