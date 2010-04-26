/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent Server.

    The Double Agent Server is free software:
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
#include "DaServerApp.h"
#include "DaSvrEvents.h"

/////////////////////////////////////////////////////////////////////////////

void CDaSvrEventDispatch::FireCommand(long CommandID, IDaSvrUserInput2* UserInput)
{
	_variant_t				lCommandID (CommandID);
	_variant_t				lUserInput ((LPDISPATCH)UserInput);
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;

	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.Invoke2 (DISPID_IAgentNotifySink_Command, &lCommandID, &lUserInput);
		}
	}
}

void CDaSvrEventDispatch::FireActivateInputState(long CharacterID, long Activated)
{
	_variant_t				lCharacterId (CharacterID);
	_variant_t				lActivated (Activated);
	int 					lNdx;
	CComQIPtr <IDispatch>	lEventSink;

	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.Invoke2 (DISPID_IAgentNotifySink_ActivateInputState, &lCharacterId, &lActivated);
		}
	}
}

void CDaSvrEventDispatch::FireVisibleState(long CharacterID, long Visible, long Cause)
{
	_variant_t				lParams [3] = {CharacterID, Visible, Cause};
	int 					lNdx;
	CComQIPtr <IDispatch>	lEventSink;

	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.InvokeN (DISPID_IAgentNotifySink_VisibleState, lParams, 3);
		}
	}
}

void CDaSvrEventDispatch::FireClick(long CharacterID, short Keys, long x, long y)
{
	_variant_t				lParams [4] = {CharacterID, Keys, x, y};
	int 					lNdx;
	CComQIPtr <IDispatch>	lEventSink;

	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.InvokeN (DISPID_IAgentNotifySink_Click, lParams, 4);
		}
	}
}

void CDaSvrEventDispatch::FireDblClick(long CharacterID, short Keys, long x, long y)
{
	_variant_t				lParams [4] = {CharacterID, Keys, x, y};
	int 					lNdx;
	CComQIPtr <IDispatch>	lEventSink;

	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.InvokeN (DISPID_IAgentNotifySink_DblClick, lParams, 4);
		}
	}
}

void CDaSvrEventDispatch::FireDragStart(long CharacterID, short Keys, long x, long y)
{
	_variant_t				lParams [4] = {CharacterID, Keys, x, y};
	int 					lNdx;
	CComQIPtr <IDispatch>	lEventSink;

	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.InvokeN (DISPID_IAgentNotifySink_DragStart, lParams, 4);
		}
	}
}

void CDaSvrEventDispatch::FireDragComplete(long CharacterID, short Keys, long x, long y)
{
	_variant_t				lParams [4] = {CharacterID, Keys, x, y};
	int 					lNdx;
	CComQIPtr <IDispatch>	lEventSink;

	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.InvokeN (DISPID_IAgentNotifySink_DragComplete, lParams, 4);
		}
	}
}

void CDaSvrEventDispatch::FireRequestStart(long RequestID)
{
	_variant_t				lRequest (RequestID);
	int 					lNdx;
	CComQIPtr <IDispatch>	lEventSink;

	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.Invoke1 (DISPID_IAgentNotifySink_RequestStart, &lRequest);
		}
	}
}

void CDaSvrEventDispatch::FireRequestComplete(long RequestID, long Result)
{
	_variant_t				lRequest (RequestID);
	_variant_t				lResult (Result);
	int 					lNdx;
	CComQIPtr <IDispatch>	lEventSink;

	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.Invoke2 (DISPID_IAgentNotifySink_RequestComplete, &lRequest, &lResult);
		}
	}
}

void CDaSvrEventDispatch::FireBookMark(long BookMarkID)
{
	_variant_t				lBookMarkID (BookMarkID);
	int 					lNdx;
	CComQIPtr <IDispatch>	lEventSink;

	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.Invoke1 (DISPID_IAgentNotifySink_BookMark, &lBookMarkID);
		}
	}
}

void CDaSvrEventDispatch::FireIdle(long CharacterID, long Start)
{
	_variant_t				lCharacterID (CharacterID);
	_variant_t				lStart (Start);
	int 					lNdx;
	CComQIPtr <IDispatch>	lEventSink;

	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.Invoke2 (DISPID_IAgentNotifySink_Idle, &lCharacterID, &lStart);
		}
	}
}

void CDaSvrEventDispatch::FireMove(long CharacterID, long x, long y, long Cause)
{
	_variant_t				lParams [4] = {CharacterID, x, y, Cause};
	int 					lNdx;
	CComQIPtr <IDispatch>	lEventSink;

	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.InvokeN (DISPID_IAgentNotifySink_Move, lParams, 4);
		}
	}
}

void CDaSvrEventDispatch::FireSize(long CharacterID, long Width, long Height)
{
	_variant_t				lParams [3] = {CharacterID, Width, Height};
	int 					lNdx;
	CComQIPtr <IDispatch>	lEventSink;

	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.InvokeN (DISPID_IAgentNotifySink_Size, lParams, 3);
		}
	}
}

void CDaSvrEventDispatch::FireBalloonVisibleState(long CharacterID, long Visible)
{
	_variant_t				lCharacterID (CharacterID);
	_variant_t				lVisible (Visible);
	int 					lNdx;
	CComQIPtr <IDispatch>	lEventSink;

	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.Invoke2 (DISPID_IAgentNotifySink_BalloonVisibleState, &lCharacterID, &lVisible);
		}
	}
}

void CDaSvrEventDispatch::FireListeningState(long CharacterID, long Listening, long Cause)
{
	_variant_t				lParams [3] = {CharacterID, Listening, Cause};
	int 					lNdx;
	CComQIPtr <IDispatch>	lEventSink;

	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.InvokeN (DISPID_IAgentNotifySinkEx_ListeningState, lParams, 3);
		}
	}
}

void CDaSvrEventDispatch::FireDefaultCharacterChange(LPCTSTR CharGUID)
{
	_variant_t				lGUID (CharGUID);
	int 					lNdx;
	CComQIPtr <IDispatch>	lEventSink;

	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.Invoke1 (DISPID_IAgentNotifySinkEx_DefaultCharacterChange, &lGUID);
		}
	}
}

void CDaSvrEventDispatch::FireAgentPropertyChange()
{
	int 					lNdx;
	CComQIPtr <IDispatch>	lEventSink;

	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.Invoke0 (DISPID_IAgentNotifySinkEx_AgentPropertyChange);
		}
	}
}

void CDaSvrEventDispatch::FireActiveClientChange(long CharacterID, long Status)
{
	_variant_t				lCharacterID (CharacterID);
	_variant_t				lStatus (Status);
	int 					lNdx;
	CComQIPtr <IDispatch>	lEventSink;

	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.Invoke2 (DISPID_IAgentNotifySinkEx_ActiveClientChange, &lCharacterID, &lStatus);
		}
	}
}
