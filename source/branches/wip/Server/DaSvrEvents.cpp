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

int CDaSvrEventDispatch::FireCommand(long CommandID, IDaSvrUserInput2* UserInput)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		_variant_t				lCommandID (CommandID);
		_variant_t				lUserInput ((LPDISPATCH)UserInput);
		int						lNdx;
		IUnknown *				lUnknown;
		CComQIPtr <IDispatch>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink.Invoke2 (DISPID_IAgentNotifySink_Command, &lCommandID, &lUserInput));
				lCount++;
			}
		}
	}
	return lCount;
}

int CDaSvrEventDispatch::FireActivateInputState(long CharacterID, long Activated)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		_variant_t				lCharacterId (CharacterID);
		_variant_t				lActivated (Activated);
		int 					lNdx;
		IUnknown *				lUnknown;
		CComQIPtr <IDispatch>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink.Invoke2 (DISPID_IAgentNotifySink_ActivateInputState, &lCharacterId, &lActivated));
				lCount++;
			}
		}
	}
	return lCount;
}

int CDaSvrEventDispatch::FireVisibleState(long CharacterID, long Visible, long Cause)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		_variant_t				lParams [3] = {CharacterID, Visible, Cause};
		int 					lNdx;
		IUnknown *				lUnknown;
		CComQIPtr <IDispatch>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink.InvokeN (DISPID_IAgentNotifySink_VisibleState, lParams, 3));
				lCount++;
			}
		}
	}
	return lCount;
}

int CDaSvrEventDispatch::FireClick(long CharacterID, short Keys, long X, long Y)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		_variant_t				lParams [4] = {CharacterID, Keys, X, Y};
		int 					lNdx;
		IUnknown *				lUnknown;
		CComQIPtr <IDispatch>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink.InvokeN (DISPID_IAgentNotifySink_Click, lParams, 4));
				lCount++;
			}
		}
	}
	return lCount;
}

int CDaSvrEventDispatch::FireDblClick(long CharacterID, short Keys, long X, long Y)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		_variant_t				lParams [4] = {CharacterID, Keys, X, Y};
		int 					lNdx;
		IUnknown *				lUnknown;
		CComQIPtr <IDispatch>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink.InvokeN (DISPID_IAgentNotifySink_DblClick, lParams, 4));
				lCount++;
			}
		}
	}
	return lCount;
}

int CDaSvrEventDispatch::FireDragStart(long CharacterID, short Keys, long X, long Y)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		_variant_t				lParams [4] = {CharacterID, Keys, X, Y};
		int 					lNdx;
		IUnknown *				lUnknown;
		CComQIPtr <IDispatch>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink.InvokeN (DISPID_IAgentNotifySink_DragStart, lParams, 4));
				lCount++;
			}
		}
	}
	return lCount;
}

int CDaSvrEventDispatch::FireDragComplete(long CharacterID, short Keys, long X, long Y)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		_variant_t				lParams [4] = {CharacterID, Keys, X, Y};
		int 					lNdx;
		IUnknown *				lUnknown;
		CComQIPtr <IDispatch>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink.InvokeN (DISPID_IAgentNotifySink_DragComplete, lParams, 4));
				lCount++;
			}
		}
	}
	return lCount;
}

int CDaSvrEventDispatch::FireRequestStart(long RequestID)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		_variant_t				lRequest (RequestID);
		int 					lNdx;
		IUnknown *				lUnknown;
		CComQIPtr <IDispatch>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink.Invoke1 (DISPID_IAgentNotifySink_RequestStart, &lRequest));
				lCount++;
			}
		}
	}
	return lCount;
}

int CDaSvrEventDispatch::FireRequestComplete(long RequestID, long Result)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		_variant_t				lRequest (RequestID);
		_variant_t				lResult (Result);
		int 					lNdx;
		IUnknown *				lUnknown;
		CComQIPtr <IDispatch>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink.Invoke2 (DISPID_IAgentNotifySink_RequestComplete, &lRequest, &lResult));
				lCount++;
			}
		}
	}
	return lCount;
}

int CDaSvrEventDispatch::FireBookMark(long BookMarkID)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		_variant_t				lBookMarkID (BookMarkID);
		int 					lNdx;
		IUnknown *				lUnknown;
		CComQIPtr <IDispatch>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink.Invoke1 (DISPID_IAgentNotifySink_BookMark, &lBookMarkID));
				lCount++;
			}
		}
	}
	return lCount;
}

int CDaSvrEventDispatch::FireIdle(long CharacterID, long Start)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		_variant_t				lCharacterID (CharacterID);
		_variant_t				lStart (Start);
		int 					lNdx;
		IUnknown *				lUnknown;
		CComQIPtr <IDispatch>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink.Invoke2 (DISPID_IAgentNotifySink_Idle, &lCharacterID, &lStart));
				lCount++;
			}
		}
	}
	return lCount;
}

int CDaSvrEventDispatch::FireMove(long CharacterID, long X, long Y, long Cause)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		_variant_t				lParams [4] = {CharacterID, X, Y, Cause};
		int 					lNdx;
		IUnknown *				lUnknown;
		CComQIPtr <IDispatch>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink.InvokeN (DISPID_IAgentNotifySink_Move, lParams, 4));
				lCount++;
			}
		}
	}
	return lCount;
}

int CDaSvrEventDispatch::FireSize(long CharacterID, long Width, long Height)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		_variant_t				lParams [3] = {CharacterID, Width, Height};
		int 					lNdx;
		IUnknown *				lUnknown;
		CComQIPtr <IDispatch>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink.InvokeN (DISPID_IAgentNotifySink_Size, lParams, 3));
				lCount++;
			}
		}
	}
	return lCount;
}

int CDaSvrEventDispatch::FireBalloonVisibleState(long CharacterID, long Visible)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		_variant_t				lCharacterID (CharacterID);
		_variant_t				lVisible (Visible);
		int 					lNdx;
		IUnknown *				lUnknown;
		CComQIPtr <IDispatch>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink.Invoke2 (DISPID_IAgentNotifySink_BalloonVisibleState, &lCharacterID, &lVisible));
				lCount++;
			}
		}
	}
	return lCount;
}

int CDaSvrEventDispatch::FireListeningState(long CharacterID, long Listening, long Cause)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		_variant_t				lParams [3] = {CharacterID, Listening, Cause};
		int 					lNdx;
		IUnknown *				lUnknown;
		CComQIPtr <IDispatch>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink.InvokeN (DISPID_IAgentNotifySinkEx_ListeningState, lParams, 3));
				lCount++;
			}
		}
	}
	return lCount;
}

int CDaSvrEventDispatch::FireDefaultCharacterChange(BSTR CharGUID)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		_variant_t				lGUID (_bstr_t (CharGUID, true));
		int 					lNdx;
		IUnknown *				lUnknown;
		CComQIPtr <IDispatch>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink.Invoke1 (DISPID_IAgentNotifySinkEx_DefaultCharacterChange, &lGUID));
				lCount++;
			}
		}
	}
	return lCount;
}

int CDaSvrEventDispatch::FireAgentPropertyChange()
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		int 					lNdx;
		IUnknown *				lUnknown;
		CComQIPtr <IDispatch>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink.Invoke0 (DISPID_IAgentNotifySinkEx_AgentPropertyChange));
				lCount++;
			}
		}
	}
	return lCount;
}

int CDaSvrEventDispatch::FireActiveClientChange(long CharacterID, long Status)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		_variant_t				lCharacterID (CharacterID);
		_variant_t				lStatus (Status);
		int 					lNdx;
		IUnknown *				lUnknown;
		CComQIPtr <IDispatch>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink.Invoke2 (DISPID_IAgentNotifySinkEx_ActiveClientChange, &lCharacterID, &lStatus));
				lCount++;
			}
		}
	}
	return lCount;
}

/////////////////////////////////////////////////////////////////////////////

int CDaSvrEventDispatch::FireSpeechStart(long CharacterID, IDaSvrFormattedText* FormattedText)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		_variant_t				lCharacterID (CharacterID);
		_variant_t				lFormattedText ((LPDISPATCH)FormattedText);
		int 					lNdx;
		IUnknown *				lUnknown;
		CComQIPtr <IDispatch>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink.Invoke2 (DISPID_IDaSvrNotifySink2_SpeechStart, &lCharacterID, &lFormattedText));
				lCount++;
			}
		}
	}
	return lCount;
}

int CDaSvrEventDispatch::FireSpeechEnd(long CharacterID, IDaSvrFormattedText* FormattedText, VARIANT_BOOL Stopped)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		_variant_t				lParams [3] = {CharacterID, (LPDISPATCH)FormattedText, Stopped};
		int 					lNdx;
		IUnknown *				lUnknown;
		CComQIPtr <IDispatch>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink.InvokeN (DISPID_IDaSvrNotifySink2_SpeechEnd, lParams, 3));
				lCount++;
			}
		}
	}
	return lCount;
}

int CDaSvrEventDispatch::FireSpeechWord(long CharacterID, IDaSvrFormattedText* FormattedText, long WordIndex)
{
	int	lCount = 0;

	if	(mUnkArray.GetSize() > 0)
	{
		_variant_t				lParams [3] = {CharacterID, (LPDISPATCH)FormattedText, WordIndex};
		int 					lNdx;
		IUnknown *				lUnknown;
		CComQIPtr <IDispatch>	lEventSink;

		for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
		{
			if	(
					(lUnknown = mUnkArray.GetAt (lNdx))
				&&	(
						(mUnregisterDelayed.GetCount() <= 0)
					||	(mUnregisterDelayed.Find (mUnkArray.GetCookie (&lUnknown)) < 0)
					)
				&&	(lEventSink = lUnknown)
				)
			{
				LogComErr (LogNormal|LogTime, lEventSink.InvokeN (DISPID_IDaSvrNotifySink2_SpeechWord, lParams, 3));
				lCount++;
			}
		}
	}
	return lCount;
}
