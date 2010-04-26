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
#include "StdAfx.h"
#include "DaControl.NET.h"
#include "DaControl.NET.Events.h"

namespace DoubleAgent {
namespace Control {
/////////////////////////////////////////////////////////////////////////////

void AxControlEvents::ActivateInput (String^ CharacterID)
{
	DoubleAgent::Control::Events::ActivateInputEvent^ lEvent = gcnew DoubleAgent::Control::Events::ActivateInputEvent (CharacterID);
	mParent->PopupActivateInput::raise (mParent, lEvent);
}
	
void AxControlEvents::DeactivateInput (String^ CharacterID)
{
	DoubleAgent::Control::Events::DeactivateInputEvent^ lEvent = gcnew DoubleAgent::Control::Events::DeactivateInputEvent (CharacterID);
	mParent->PopupDeactivateInput::raise (mParent, lEvent);
}

void AxControlEvents::Click (String^ CharacterID, short Button, short Shift, short X, short Y)
{
	DoubleAgent::Control::Events::ClickEvent^ lEvent = gcnew DoubleAgent::Control::Events::ClickEvent (CharacterID, Button, Shift, X, Y);
	mParent->PopupClick::raise (mParent, lEvent);
}

void AxControlEvents::DblClick (String^ CharacterID, short Button, short Shift, short X, short Y)
{
	DoubleAgent::Control::Events::DblClickEvent^ lEvent = gcnew DoubleAgent::Control::Events::DblClickEvent (CharacterID, Button, Shift, X, Y);
	mParent->PopupDblClick::raise (mParent, lEvent);
}

void AxControlEvents::DragStart (String^ CharacterID, short Button, short Shift, short X, short Y)
{
	DoubleAgent::Control::Events::DragStartEvent^ lEvent = gcnew DoubleAgent::Control::Events::DragStartEvent (CharacterID, Button, Shift, X, Y);
	mParent->PopupDragStart::raise (mParent, lEvent);
}

void AxControlEvents::DragComplete (String^ CharacterID, short Button, short Shift, short X, short Y)
{
	DoubleAgent::Control::Events::DragCompleteEvent^ lEvent = gcnew DoubleAgent::Control::Events::DragCompleteEvent (CharacterID, Button, Shift, X, Y);
	mParent->PopupDragComplete::raise (mParent, lEvent);
}

void AxControlEvents::Show (String^ CharacterID, DoubleAgent::Control::VisibilityCauseType Cause)
{
	DoubleAgent::Control::Events::ShowEvent^ lEvent = gcnew DoubleAgent::Control::Events::ShowEvent (CharacterID, Cause);
	mParent->PopupShow::raise (mParent, lEvent);
}

void AxControlEvents::Hide (String^ CharacterID, DoubleAgent::Control::VisibilityCauseType Cause)
{
	DoubleAgent::Control::Events::HideEvent^ lEvent = gcnew DoubleAgent::Control::Events::HideEvent (CharacterID, Cause);
	mParent->PopupHide::raise (mParent, lEvent);
}

void AxControlEvents::RequestStart (DoubleAgent::Control::IRequest^ Request)
{
	DoubleAgent::Control::Events::RequestStartEvent^ lEvent = gcnew DoubleAgent::Control::Events::RequestStartEvent (Request);
	mParent->RequestStart::raise (mParent, lEvent);
}

void AxControlEvents::RequestComplete (DoubleAgent::Control::IRequest^ Request)
{
	DoubleAgent::Control::Events::RequestCompleteEvent^ lEvent = gcnew DoubleAgent::Control::Events::RequestCompleteEvent (Request);
	mParent->RequestComplete::raise (mParent, lEvent);
}

void AxControlEvents::Bookmark (int BookmarkID)
{
	DoubleAgent::Control::Events::BookmarkEvent^ lEvent = gcnew DoubleAgent::Control::Events::BookmarkEvent (BookmarkID);
	mParent->Bookmark::raise (mParent, lEvent);
}

void AxControlEvents::Command (DoubleAgent::Control::IUserInput^ UserInput)
{
	DoubleAgent::Control::Events::CommandEvent^ lEvent = gcnew DoubleAgent::Control::Events::CommandEvent (UserInput);
	mParent->Command::raise (mParent, lEvent);
}

void AxControlEvents::IdleStart (String^ CharacterID)
{
	DoubleAgent::Control::Events::IdleStartEvent^ lEvent = gcnew DoubleAgent::Control::Events::IdleStartEvent (CharacterID);
	mParent->IdleStart::raise (mParent, lEvent);
}

void AxControlEvents::IdleComplete (String^ CharacterID)
{
	DoubleAgent::Control::Events::IdleCompleteEvent^ lEvent = gcnew DoubleAgent::Control::Events::IdleCompleteEvent (CharacterID);
	mParent->IdleComplete::raise (mParent, lEvent);
}

void AxControlEvents::Move (String^ CharacterID, short X, short Y, DoubleAgent::Control::MoveCauseType Cause)
{
	DoubleAgent::Control::Events::MoveEvent^ lEvent = gcnew DoubleAgent::Control::Events::MoveEvent (CharacterID, X, Y, Cause);
	mParent->PopupMove::raise (mParent, lEvent);
}

void AxControlEvents::Size (String^ CharacterID, short Width, short Height)
{
	DoubleAgent::Control::Events::SizeEvent^ lEvent = gcnew DoubleAgent::Control::Events::SizeEvent (CharacterID, Width, Height);
	mParent->PopupSize::raise (mParent, lEvent);
}

void AxControlEvents::BalloonShow (String^ CharacterID)
{
	DoubleAgent::Control::Events::BalloonShowEvent^ lEvent = gcnew DoubleAgent::Control::Events::BalloonShowEvent (CharacterID);
	mParent->BalloonShow::raise (mParent, lEvent);
}

void AxControlEvents::BalloonHide (String^ CharacterID)
{
	DoubleAgent::Control::Events::BalloonHideEvent^ lEvent = gcnew DoubleAgent::Control::Events::BalloonHideEvent (CharacterID);
	mParent->BalloonHide::raise (mParent, lEvent);
}

void AxControlEvents::ListenStart (String^ CharacterID)
{
	DoubleAgent::Control::Events::ListenStartEvent^ lEvent = gcnew DoubleAgent::Control::Events::ListenStartEvent (CharacterID);
	mParent->ListenStart::raise (mParent, lEvent);
}

void AxControlEvents::ListenComplete (String^ CharacterID, DoubleAgent::Control::ListenCompleteType Cause)
{
	DoubleAgent::Control::Events::ListenCompleteEvent^ lEvent = gcnew DoubleAgent::Control::Events::ListenCompleteEvent (CharacterID, Cause);
	mParent->ListenComplete::raise (mParent, lEvent);
}

void AxControlEvents::DefaultCharacterChange (String^ CharGUID)
{
	DoubleAgent::Control::Events::DefaultCharacterChangeEvent^ lEvent = gcnew DoubleAgent::Control::Events::DefaultCharacterChangeEvent (CharGUID);
	mParent->DefaultCharacterChange::raise (mParent, lEvent);
}

void AxControlEvents::AgentPropertyChange ()
{
	System::EventArgs^ lEvent = gcnew System::EventArgs ();
	mParent->AgentPropertyChange::raise (mParent, lEvent);
}

void AxControlEvents::ActiveClientChange (String^ CharacterID, bool Active)
{
	DoubleAgent::Control::Events::ActiveClientChangeEvent^ lEvent = gcnew DoubleAgent::Control::Events::ActiveClientChangeEvent (CharacterID, Active);
	mParent->ActiveClientChange::raise (mParent, lEvent);
}

/////////////////////////////////////////////////////////////////////////////
}	// namespace Control
}	// namespace DoubleAgent