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
#include "DaControlMod.h"
#include "DaControl.h"
#include "DaCtlUserInput.h"
#include "DaCtlFormattedText.h"
#include "DaCtlCommands.h"
#include "Registry.h"
#include "DebugStr.h"

#ifdef	_DEBUG
#define	_DEBUG_NOTIFY			(GetProfileDebugInt(_T("DebugNotify"),LogVerbose,true)&0xFFFF|LogTime)
#define	_DEBUG_NOTIFY_DISPATCH	(GetProfileDebugInt(_T("DebugNotifyDispatch"),LogVerbose,true)&0xFFFF|LogTime)
#define	_DEBUG_REQUEST			(GetProfileDebugInt(_T("DebugRequests"),LogVerbose,true)&0xFFFF|LogTime)
#endif

#ifdef	_DEBUG_REQUEST
#ifdef  _DEBUG_NOTIFY
#define	_DEBUG_REQUEST_NOTIFY	MinLogLevel(_DEBUG_REQUEST,_DEBUG_NOTIFY)
#else
#define	_DEBUG_REQUEST_NOTIFY	_DEBUG_REQUEST
#endif
#else
#ifdef  _DEBUG_NOTIFY
#define	_DEBUG_REQUEST_NOTIFY	_DEBUG_NOTIFY
#endif
#endif

/////////////////////////////////////////////////////////////////////////////

static INT_PTR CountUnk (CComDynamicUnkArray & pUnkArray)
{
	int	lRet = 0;
	int	lNdx;

	for	(lNdx = 0; lNdx < pUnkArray.GetSize(); lNdx++)
	{
		if	(pUnkArray.GetAt (lNdx))
		{
			lRet++;
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

int CDaCtlEventDispatch::FireActivateInput(LPCTSTR CharacterID)
{
	int						lCount = 0;
	_variant_t				lCharacterId (CharacterID);
	int 					lNdx;
	CComQIPtr <IDispatch>	lEventSink;

#ifdef	_DEBUG_NOTIFY_DISPATCH
	if	(CountUnk(mUnkArray) > 0)
	{
		LogMessage (_DEBUG_NOTIFY_DISPATCH, _T("[%d] CDaCtlEventDispatch::FireActivateInput [%s]"), CountUnk(mUnkArray), CharacterID);
	}
#endif
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			LogComErr (LogNormal|LogTime, lEventSink.Invoke1 (DISPID_AgentEvents_ActivateInput, &lCharacterId));
			lCount++;
		}
	}
	return lCount;
}

int CDaCtlEventDispatch::FireDeactivateInput(LPCTSTR CharacterID)
{
	int						lCount = 0;
	_variant_t				lCharacterId (CharacterID);
	int 					lNdx;
	CComQIPtr <IDispatch>	lEventSink;

#ifdef	_DEBUG_NOTIFY_DISPATCH
	if	(CountUnk(mUnkArray) > 0)
	{
		LogMessage (_DEBUG_NOTIFY_DISPATCH, _T("[%d] CDaCtlEventDispatch::FireDeactivateInput [%s]"), CountUnk(mUnkArray), CharacterID);
	}
#endif
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			LogComErr (LogNormal|LogTime, lEventSink.Invoke1 (DISPID_AgentEvents_DeactivateInput, &lCharacterId));
			lCount++;
		}
	}
	return lCount;
}

int CDaCtlEventDispatch::FireClick(LPCTSTR CharacterID, short Button, short Shift, short X, short Y)
{
	int						lCount = 0;
	_variant_t				lParams [5] = {Y, X, Shift, Button, CharacterID};
	int 					lNdx;
	CComQIPtr <IDispatch>	lEventSink;

#ifdef	_DEBUG_NOTIFY_DISPATCH
	if	(CountUnk(mUnkArray) > 0)
	{
		LogMessage (_DEBUG_NOTIFY_DISPATCH, _T("[%d] CDaCtlEventDispatch::FireClick [%s]"), CountUnk(mUnkArray), CharacterID);
	}
#endif
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			LogComErr (LogNormal|LogTime, lEventSink.InvokeN (DISPID_AgentEvents_Click, lParams, 5));
			lCount++;
		}
	}
	return lCount;
}

int CDaCtlEventDispatch::FireDblClick(LPCTSTR CharacterID, short Button, short Shift, short X, short Y)
{
	int						lCount = 0;
	_variant_t				lParams [5] = {Y, X, Shift, Button, CharacterID};
	int 					lNdx;
	CComQIPtr <IDispatch>	lEventSink;

#ifdef	_DEBUG_NOTIFY_DISPATCH
	if	(CountUnk(mUnkArray) > 0)
	{
		LogMessage (_DEBUG_NOTIFY_DISPATCH, _T("[%d] CDaCtlEventDispatch::FireDblClick [%s]"), CountUnk(mUnkArray), CharacterID);
	}
#endif
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			LogComErr (LogNormal|LogTime, lEventSink.InvokeN (DISPID_AgentEvents_DblClick, lParams, 5));
			lCount++;
		}
	}
	return lCount;
}

int CDaCtlEventDispatch::FireDragStart(LPCTSTR CharacterID, short Button, short Shift, short X, short Y)
{
	int						lCount = 0;
	_variant_t				lParams [5] = {Y, X, Shift, Button, CharacterID};
	int 					lNdx;
	CComQIPtr <IDispatch>	lEventSink;

#ifdef	_DEBUG_NOTIFY_DISPATCH
	if	(CountUnk(mUnkArray) > 0)
	{
		LogMessage (_DEBUG_NOTIFY_DISPATCH, _T("[%d] CDaCtlEventDispatch::FireDragStart [%s]"), CountUnk(mUnkArray), CharacterID);
	}
#endif
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			LogComErr (LogNormal|LogTime, lEventSink.InvokeN (DISPID_AgentEvents_DragStart, lParams, 5));
			lCount++;
		}
	}
	return lCount;
}

int CDaCtlEventDispatch::FireDragComplete(LPCTSTR CharacterID, short Button, short Shift, short X, short Y)
{
	int						lCount = 0;
	_variant_t				lParams [5] = {Y, X, Shift, Button, CharacterID};
	int 					lNdx;
	CComQIPtr <IDispatch>	lEventSink;

#ifdef	_DEBUG_NOTIFY_DISPATCH
	if	(CountUnk(mUnkArray) > 0)
	{
		LogMessage (_DEBUG_NOTIFY_DISPATCH, _T("[%d] CDaCtlEventDispatch::FireDragComplete [%s]"), CountUnk(mUnkArray), CharacterID);
	}
#endif
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			LogComErr (LogNormal|LogTime, lEventSink.InvokeN (DISPID_AgentEvents_DragComplete, lParams, 5));
			lCount++;
		}
	}
	return lCount;
}

int CDaCtlEventDispatch::FireShow(LPCTSTR CharacterID, VisibilityCauseType Cause)
{
	int						lCount = 0;
	_variant_t				lCharacterId (CharacterID);
	_variant_t				lCause (Cause);
	int 					lNdx;
	CComQIPtr <IDispatch>	lEventSink;

#ifdef	_DEBUG_NOTIFY_DISPATCH
	if	(CountUnk(mUnkArray) > 0)
	{
		LogMessage (_DEBUG_NOTIFY_DISPATCH, _T("[%d] CDaCtlEventDispatch::FireShow [%s]"), CountUnk(mUnkArray), CharacterID);
	}
#endif
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			LogComErr (LogNormal|LogTime, lEventSink.Invoke2 (DISPID_AgentEvents_Show, &lCharacterId, &lCause));
			lCount++;
		}
	}
	return lCount;
}

int CDaCtlEventDispatch::FireHide(LPCTSTR CharacterID, VisibilityCauseType Cause)
{
	int						lCount = 0;
	_variant_t				lCharacterId (CharacterID);
	_variant_t				lCause (Cause);
	int 					lNdx;
	CComQIPtr <IDispatch>	lEventSink;

#ifdef	_DEBUG_NOTIFY_DISPATCH
	if	(CountUnk(mUnkArray) > 0)
	{
		LogMessage (_DEBUG_NOTIFY_DISPATCH, _T("[%d] CDaCtlEventDispatch::FireHide [%s]"), CountUnk(mUnkArray), CharacterID);
	}
#endif
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			LogComErr (LogNormal|LogTime, lEventSink.Invoke2 (DISPID_AgentEvents_Hide, &lCharacterId, &lCause));
			lCount++;
		}
	}
	return lCount;
}

int CDaCtlEventDispatch::FireRequestStart(LPDISPATCH Request)
{
	int						lCount = 0;
	_variant_t				lRequest (Request);
	int 					lNdx;
	CComQIPtr <IDispatch>	lEventSink;

#ifdef	_DEBUG_NOTIFY_DISPATCH
	if	(CountUnk(mUnkArray) > 0)
	{
		LogMessage (_DEBUG_NOTIFY_DISPATCH, _T("[%d] CDaCtlEventDispatch::FireRequestStart [%p]"), CountUnk(mUnkArray), Request);
	}
#endif
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			LogComErr (LogNormal|LogTime, lEventSink.Invoke1 (DISPID_AgentEvents_RequestStart, &lRequest));
			lCount++;
		}
	}
	return lCount;
}

int CDaCtlEventDispatch::FireRequestComplete(LPDISPATCH Request)
{
	int						lCount = 0;
	_variant_t				lRequest (Request);
	int 					lNdx;
	CComQIPtr <IDispatch>	lEventSink;

#ifdef	_DEBUG_NOTIFY_DISPATCH
	if	(CountUnk(mUnkArray) > 0)
	{
		LogMessage (_DEBUG_NOTIFY_DISPATCH, _T("[%d] CDaCtlEventDispatch::FireRequestComplete [%p]"), CountUnk(mUnkArray), Request);
	}
#endif
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			LogComErr (LogNormal|LogTime, lEventSink.Invoke1 (DISPID_AgentEvents_RequestComplete, &lRequest));
			lCount++;
		}
	}
	return lCount;
}

int CDaCtlEventDispatch::FireBookmark(long BookmarkID)
{
	int						lCount = 0;
	_variant_t				lBookmarkID (BookmarkID);
	int 					lNdx;
	CComQIPtr <IDispatch>	lEventSink;

#ifdef	_DEBUG_NOTIFY_DISPATCH
	if	(CountUnk(mUnkArray) > 0)
	{
		LogMessage (_DEBUG_NOTIFY_DISPATCH, _T("[%d] CDaCtlEventDispatch::FireBookmark [%d]"), CountUnk(mUnkArray), BookmarkID);
	}
#endif
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			LogComErr (LogNormal|LogTime, lEventSink.Invoke1 (DISPID_AgentEvents_Bookmark, &lBookmarkID));
			lCount++;
		}
	}
	return lCount;
}

int CDaCtlEventDispatch::FireCommand(LPDISPATCH UserInput)
{
	int						lCount = 0;
	_variant_t				lUserInput (UserInput);
	int 					lNdx;
	CComQIPtr <IDispatch>	lEventSink;

#ifdef	_DEBUG_NOTIFY_DISPATCH
	if	(CountUnk(mUnkArray) > 0)
	{
		LogMessage (_DEBUG_NOTIFY_DISPATCH, _T("[%d] CDaCtlEventDispatch::FireCommand [%p]"), CountUnk(mUnkArray), UserInput);
	}
#endif
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			LogComErr (LogNormal|LogTime, lEventSink.Invoke1 (DISPID_AgentEvents_Command, &lUserInput));
			lCount++;
		}
	}
	return lCount;
}

int CDaCtlEventDispatch::FireIdleStart(LPCTSTR CharacterID)
{
	int						lCount = 0;
	_variant_t				lCharacterID (CharacterID);
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;

#ifdef	_DEBUG_NOTIFY_DISPATCH
	if	(CountUnk(mUnkArray) > 0)
	{
		LogMessage (_DEBUG_NOTIFY_DISPATCH, _T("[%d] CDaCtlEventDispatch::FireIdleStart [%s]"), CountUnk(mUnkArray), CharacterID);
	}
#endif
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			LogComErr (LogNormal|LogTime, lEventSink.Invoke1 (DISPID_AgentEvents_IdleStart, &lCharacterID));
			lCount++;
		}
	}
	return lCount;
}

int CDaCtlEventDispatch::FireIdleComplete(LPCTSTR CharacterID)
{
	int						lCount = 0;
	_variant_t				lCharacterID (CharacterID);
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;

#ifdef	_DEBUG_NOTIFY_DISPATCH
	if	(CountUnk(mUnkArray) > 0)
	{
		LogMessage (_DEBUG_NOTIFY_DISPATCH, _T("[%d] CDaCtlEventDispatch::FireIdleComplete [%s]"), CountUnk(mUnkArray), CharacterID);
	}
#endif
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			LogComErr (LogNormal|LogTime, lEventSink.Invoke1 (DISPID_AgentEvents_IdleComplete, &lCharacterID));
			lCount++;
		}
	}
	return lCount;
}

int CDaCtlEventDispatch::FireMove(LPCTSTR CharacterID, short X, short Y, MoveCauseType Cause)
{
	int						lCount = 0;
	_variant_t				lParams [4] = {Cause, Y, X, CharacterID};
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;

#ifdef	_DEBUG_NOTIFY_DISPATCH
	if	(CountUnk(mUnkArray) > 0)
	{
		LogMessage (_DEBUG_NOTIFY_DISPATCH, _T("[%d] CDaCtlEventDispatch::FireMove [%s]"), CountUnk(mUnkArray), CharacterID);
	}
#endif
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			LogComErr (LogNormal|LogTime, lEventSink.InvokeN (DISPID_AgentEvents_Move, lParams, 4));
			lCount++;
		}
	}
	return lCount;
}

int CDaCtlEventDispatch::FireSize(LPCTSTR CharacterID, short Width, short Height)
{
	int						lCount = 0;
	_variant_t				lParams [3] = {Height, Width, CharacterID};
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;

#ifdef	_DEBUG_NOTIFY_DISPATCH
	if	(CountUnk(mUnkArray) > 0)
	{
		LogMessage (_DEBUG_NOTIFY_DISPATCH, _T("[%d] CDaCtlEventDispatch::FireSize [%s]"), CountUnk(mUnkArray), CharacterID);
	}
#endif
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			LogComErr (LogNormal|LogTime, lEventSink.InvokeN (DISPID_AgentEvents_Size, lParams, 3));
			lCount++;
		}
	}
	return lCount;
}

int CDaCtlEventDispatch::FireBalloonShow(LPCTSTR CharacterID)
{
	int						lCount = 0;
	_variant_t				lCharacterID (CharacterID);
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;

#ifdef	_DEBUG_NOTIFY_DISPATCH
	if	(CountUnk(mUnkArray) > 0)
	{
		LogMessage (_DEBUG_NOTIFY_DISPATCH, _T("[%d] CDaCtlEventDispatch::FireBalloonShow [%s]"), CountUnk(mUnkArray), CharacterID);
	}
#endif
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			LogComErr (LogNormal|LogTime, lEventSink.Invoke1 (DISPID_AgentEvents_BalloonShow, &lCharacterID));
			lCount++;
		}
	}
	return lCount;
}

int CDaCtlEventDispatch::FireBalloonHide(LPCTSTR CharacterID)
{
	int						lCount = 0;
	_variant_t				lCharacterID (CharacterID);
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;

#ifdef	_DEBUG_NOTIFY_DISPATCH
	if	(CountUnk(mUnkArray) > 0)
	{
		LogMessage (_DEBUG_NOTIFY_DISPATCH, _T("[%d] CDaCtlEventDispatch::FireBalloonHide [%s]"), CountUnk(mUnkArray), CharacterID);
	}
#endif
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			LogComErr (LogNormal|LogTime, lEventSink.Invoke1 (DISPID_AgentEvents_BalloonHide, &lCharacterID));
			lCount++;
		}
	}
	return lCount;
}

int CDaCtlEventDispatch::FireListenStart(LPCTSTR CharacterID)
{
	int						lCount = 0;
	_variant_t				lCharacterID (CharacterID);
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;

#ifdef	_DEBUG_NOTIFY_DISPATCH
	if	(CountUnk(mUnkArray) > 0)
	{
		LogMessage (_DEBUG_NOTIFY_DISPATCH, _T("[%d] CDaCtlEventDispatch::FireListenStart [%s]"), CountUnk(mUnkArray), CharacterID);
	}
#endif
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			LogComErr (LogNormal|LogTime, lEventSink.Invoke1 (DISPID_AgentEvents_ListenStart, &lCharacterID));
			lCount++;
		}
	}
	return lCount;
}

int CDaCtlEventDispatch::FireListenComplete(LPCTSTR CharacterID, ListenCompleteType Cause)
{
	int						lCount = 0;
	_variant_t				lCharacterID (CharacterID);
	_variant_t				lCause (Cause);
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;

#ifdef	_DEBUG_NOTIFY_DISPATCH
	if	(CountUnk(mUnkArray) > 0)
	{
		LogMessage (_DEBUG_NOTIFY_DISPATCH, _T("[%d] CDaCtlEventDispatch::FireListenComplete [%s]"), CountUnk(mUnkArray), CharacterID);
	}
#endif
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			LogComErr (LogNormal|LogTime, lEventSink.Invoke2 (DISPID_AgentEvents_ListenComplete, &lCharacterID, &lCause));
			lCount++;
		}
	}
	return lCount;
}

int CDaCtlEventDispatch::FireDefaultCharacterChange(LPCTSTR CharGUID)
{
	int						lCount = 0;
	_variant_t				lGUID (CharGUID);
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;

#ifdef	_DEBUG_NOTIFY_DISPATCH
	if	(CountUnk(mUnkArray) > 0)
	{
		LogMessage (_DEBUG_NOTIFY_DISPATCH, _T("[%d] CDaCtlEventDispatch::FireDefaultCharacterChange [%s]"), CountUnk(mUnkArray), CharGUID);
	}
#endif
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			LogComErr (LogNormal|LogTime, lEventSink.Invoke1 (DISPID_AgentEvents_DefaultCharacterChange, &lGUID));
			lCount++;
		}
	}
	return lCount;
}

int CDaCtlEventDispatch::FireAgentPropertyChange()
{
	int						lCount = 0;
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;

#ifdef	_DEBUG_NOTIFY_DISPATCH
	if	(CountUnk(mUnkArray) > 0)
	{
		LogMessage (_DEBUG_NOTIFY_DISPATCH, _T("[%d] CDaCtlEventDispatch::FireAgentPropertyChange"), CountUnk(mUnkArray));
	}
#endif
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			LogComErr (LogNormal|LogTime, lEventSink.Invoke0 (DISPID_AgentEvents_AgentPropertyChange));
			lCount++;
		}
	}
	return lCount;
}

int CDaCtlEventDispatch::FireActiveClientChange(LPCTSTR CharacterID, BOOL Active)
{
	int						lCount = 0;
	_variant_t				lCharacterID (CharacterID);
	_variant_t				lActive (Active);
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;

#ifdef	_DEBUG_NOTIFY_DISPATCH
	if	(CountUnk(mUnkArray) > 0)
	{
		LogMessage (_DEBUG_NOTIFY_DISPATCH, _T("[%d] CDaCtlEventDispatch::FireActiveClientChange [%s]"), CountUnk(mUnkArray), CharacterID);
	}
#endif
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			LogComErr (LogNormal|LogTime, lEventSink.Invoke2 (DISPID_AgentEvents_ActiveClientChange, &lCharacterID, &lActive));
			lCount++;
		}
	}
	return lCount;
}

/////////////////////////////////////////////////////////////////////////////

int CDaCtlEventDispatch::FireSpeechStart(LPCTSTR CharacterID, LPDISPATCH FormattedText)
{
	int						lCount = 0;
	_variant_t				lCharacterID (CharacterID);
	_variant_t				lFormattedText (FormattedText);
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;

#ifdef	_DEBUG_NOTIFY_DISPATCH
	if	(CountUnk(mUnkArray) > 0)
	{
		LogMessage (_DEBUG_NOTIFY_DISPATCH, _T("[%d] CDaCtlEventDispatch::FireSpeechStart [%s] [%p]"), CountUnk(mUnkArray), CharacterID, FormattedText);
	}
#endif
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			LogComErr (LogNormal|LogTime, lEventSink.Invoke2 (DISPID_DaCtlEvents2_SpeechStart, &lCharacterID, &lFormattedText));
			lCount++;
		}
	}
	return lCount;
}

int CDaCtlEventDispatch::FireSpeechEnd(LPCTSTR CharacterID, LPDISPATCH FormattedText, VARIANT_BOOL Stopped)
{
	int						lCount = 0;
	_variant_t				lParams [3] = {Stopped, FormattedText, CharacterID};
	_variant_t				lStopped (Stopped);
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;

#ifdef	_DEBUG_NOTIFY_DISPATCH
	if	(CountUnk(mUnkArray) > 0)
	{
		LogMessage (_DEBUG_NOTIFY_DISPATCH, _T("[%d] CDaCtlEventDispatch::FireSpeechEnd [%s] [%p] [%d]"), CountUnk(mUnkArray), CharacterID, FormattedText, Stopped);
	}
#endif
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			LogComErr (LogNormal|LogTime, lEventSink.InvokeN (DISPID_DaCtlEvents2_SpeechEnd, lParams, 3));
			lCount++;
		}
	}
	return lCount;
}

int CDaCtlEventDispatch::FireSpeechWord(LPCTSTR CharacterID, LPDISPATCH FormattedText, long WordIndex)
{
	int						lCount = 0;
	_variant_t				lParams [3] = {WordIndex, FormattedText, CharacterID};
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;

#ifdef	_DEBUG_NOTIFY_DISPATCH
	if	(CountUnk(mUnkArray) > 0)
	{
		LogMessage (_DEBUG_NOTIFY_DISPATCH, _T("[%d] CDaCtlEventDispatch::FireSpeechWord [%s] [%p] {%d]"), CountUnk(mUnkArray), CharacterID, FormattedText, WordIndex);
	}
#endif
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			LogComErr (LogNormal|LogTime, lEventSink.InvokeN (DISPID_DaCtlEvents2_SpeechWord, lParams, 3));
			lCount++;
		}
	}
	return lCount;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

int DaControl::FireActivateInput(LPCTSTR CharacterID)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("ActivateInput")))
	{
		try
		{
			lEventSinkCount = CProxy_DaCtlEvents<DaControl>::FireActivateInput (CharacterID)
							+ CProxy_DaCtlEvents2<DaControl>::FireActivateInput (CharacterID)
							+ CProxy_AgentEvents<DaControl>::FireActivateInput (CharacterID);
		}
		catch AnyExceptionDebug
		PostFireEvent (_DEBUG_T("ActivateInput"), lEventSinkCount);
	}
	return lEventSinkCount;
}

int DaControl::FireDeactivateInput(LPCTSTR CharacterID)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("DeactivateInput")))
	{
		try
		{
			lEventSinkCount = CProxy_DaCtlEvents<DaControl>::FireDeactivateInput (CharacterID)
							+ CProxy_DaCtlEvents2<DaControl>::FireDeactivateInput (CharacterID)
							+ CProxy_AgentEvents<DaControl>::FireDeactivateInput (CharacterID);
		}
		catch AnyExceptionDebug
		PostFireEvent (_DEBUG_T("DeactivateInput"), lEventSinkCount);
	}
	return lEventSinkCount;
}

int DaControl::FireClick(LPCTSTR CharacterID, short Button, short Shift, short X, short Y)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("Click")))
	{
		try
		{
			lEventSinkCount = CProxy_DaCtlEvents<DaControl>::FireClick (CharacterID, Button, Shift, X, Y)
							+ CProxy_DaCtlEvents2<DaControl>::FireClick (CharacterID, Button, Shift, X, Y)
							+ CProxy_AgentEvents<DaControl>::FireClick (CharacterID, Button, Shift, X, Y);
		}
		catch AnyExceptionDebug
		PostFireEvent (_DEBUG_T("Click"), lEventSinkCount);
	}
	return lEventSinkCount;
}

int DaControl::FireDblClick(LPCTSTR CharacterID, short Button, short Shift, short X, short Y)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("DblClick")))
	{
		try
		{
			lEventSinkCount = CProxy_DaCtlEvents<DaControl>::FireDblClick (CharacterID, Button, Shift, X, Y)
							+ CProxy_DaCtlEvents2<DaControl>::FireDblClick (CharacterID, Button, Shift, X, Y)
							+ CProxy_AgentEvents<DaControl>::FireDblClick (CharacterID, Button, Shift, X, Y);
		}
		catch AnyExceptionDebug
		PostFireEvent (_DEBUG_T("DblClick"), lEventSinkCount);
	}
	return lEventSinkCount;
}

int DaControl::FireDragStart(LPCTSTR CharacterID, short Button, short Shift, short X, short Y)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("DragStart")))
	{
		try
		{
			lEventSinkCount = CProxy_DaCtlEvents<DaControl>::FireDragStart (CharacterID, Button, Shift, X, Y)
							+ CProxy_DaCtlEvents2<DaControl>::FireDragStart (CharacterID, Button, Shift, X, Y)
							+ CProxy_AgentEvents<DaControl>::FireDragStart (CharacterID, Button, Shift, X, Y);
		}
		catch AnyExceptionDebug
		PostFireEvent (_DEBUG_T("DragStart"), lEventSinkCount);
	}
	return lEventSinkCount;
}

int DaControl::FireDragComplete(LPCTSTR CharacterID, short Button, short Shift, short X, short Y)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("DragComplete")))
	{
		try
		{
			lEventSinkCount = CProxy_DaCtlEvents<DaControl>::FireDragComplete (CharacterID, Button, Shift, X, Y)
							+ CProxy_DaCtlEvents2<DaControl>::FireDragComplete (CharacterID, Button, Shift, X, Y)
							+ CProxy_AgentEvents<DaControl>::FireDragComplete (CharacterID, Button, Shift, X, Y);
		}
		catch AnyExceptionDebug
		PostFireEvent (_DEBUG_T("DragComplete"), lEventSinkCount);
	}
	return lEventSinkCount;
}

int DaControl::FireShow(LPCTSTR CharacterID, VisibilityCauseType Cause)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("Show")))
	{
		try
		{
			lEventSinkCount = CProxy_DaCtlEvents<DaControl>::FireShow (CharacterID, Cause)
							+ CProxy_DaCtlEvents2<DaControl>::FireShow (CharacterID, Cause)
							+ CProxy_AgentEvents<DaControl>::FireShow (CharacterID, Cause);
		}
		catch AnyExceptionDebug
		PostFireEvent (_DEBUG_T("Show"), lEventSinkCount);
	}
	return lEventSinkCount;
}

int DaControl::FireHide(LPCTSTR CharacterID, VisibilityCauseType Cause)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("Hide")))
	{
		try
		{
			lEventSinkCount = CProxy_DaCtlEvents<DaControl>::FireHide (CharacterID, Cause)
							+ CProxy_DaCtlEvents2<DaControl>::FireHide (CharacterID, Cause)
							+ CProxy_AgentEvents<DaControl>::FireHide (CharacterID, Cause);
		}
		catch AnyExceptionDebug
		PostFireEvent (_DEBUG_T("Hide"), lEventSinkCount);
	}
	return lEventSinkCount;
}

int DaControl::FireRequestStart(LPDISPATCH Request)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("RequestStart")))
	{
		try
		{
			lEventSinkCount = CProxy_DaCtlEvents<DaControl>::FireRequestStart (Request)
							+ CProxy_DaCtlEvents2<DaControl>::FireRequestStart (Request)
							+ CProxy_AgentEvents<DaControl>::FireRequestStart (Request);
		}
		catch AnyExceptionDebug
		PostFireEvent (_DEBUG_T("RequestStart"), lEventSinkCount);
	}
	return lEventSinkCount;
}

int DaControl::FireRequestComplete(LPDISPATCH Request)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("RequestComplete")))
	{
		try
		{
			lEventSinkCount = CProxy_DaCtlEvents<DaControl>::FireRequestComplete (Request)
							+ CProxy_DaCtlEvents2<DaControl>::FireRequestComplete (Request)
							+ CProxy_AgentEvents<DaControl>::FireRequestComplete (Request);
		}
		catch AnyExceptionDebug
		PostFireEvent (_DEBUG_T("RequestComplete"), lEventSinkCount);
	}
	return lEventSinkCount;
}

int DaControl::FireBookmark(long BookmarkID)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("Bookmark")))
	{
		try
		{
			lEventSinkCount = CProxy_DaCtlEvents<DaControl>::FireBookmark (BookmarkID)
							+ CProxy_DaCtlEvents2<DaControl>::FireBookmark (BookmarkID)
							+ CProxy_AgentEvents<DaControl>::FireBookmark (BookmarkID);
		}
		catch AnyExceptionDebug
		PostFireEvent (_DEBUG_T("Bookmark"), lEventSinkCount);
	}
	return lEventSinkCount;
}

int DaControl::FireCommand(LPDISPATCH UserInput)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("Command")))
	{
		try
		{
			lEventSinkCount = CProxy_DaCtlEvents<DaControl>::FireCommand (UserInput)
							+ CProxy_DaCtlEvents2<DaControl>::FireCommand (UserInput)
							+ CProxy_AgentEvents<DaControl>::FireCommand (UserInput);
		}
		catch AnyExceptionDebug
		PostFireEvent (_DEBUG_T("Command"), lEventSinkCount);
	}
	return lEventSinkCount;
}

int DaControl::FireIdleStart(LPCTSTR CharacterID)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("IdleStart")))
	{
		try
		{
			lEventSinkCount = CProxy_DaCtlEvents<DaControl>::FireIdleStart (CharacterID)
							+ CProxy_DaCtlEvents2<DaControl>::FireIdleStart (CharacterID)
							+ CProxy_AgentEvents<DaControl>::FireIdleStart (CharacterID);
		}
		catch AnyExceptionDebug
		PostFireEvent (_DEBUG_T("IdleStart"), lEventSinkCount);
	}
	return lEventSinkCount;
}

int DaControl::FireIdleComplete(LPCTSTR CharacterID)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("IdleComplete")))
	{
		try
		{
			lEventSinkCount = CProxy_DaCtlEvents<DaControl>::FireIdleComplete (CharacterID)
							+ CProxy_DaCtlEvents2<DaControl>::FireIdleComplete (CharacterID)
							+ CProxy_AgentEvents<DaControl>::FireIdleComplete (CharacterID);
		}
		catch AnyExceptionDebug
		PostFireEvent (_DEBUG_T("IdleComplete"), lEventSinkCount);
	}
	return lEventSinkCount;
}

int DaControl::FireMove(LPCTSTR CharacterID, short X, short Y, MoveCauseType Cause)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("Move")))
	{
		try
		{
			lEventSinkCount = CProxy_DaCtlEvents<DaControl>::FireMove (CharacterID, X, Y, Cause)
							+ CProxy_DaCtlEvents2<DaControl>::FireMove (CharacterID, X, Y, Cause)
							+ CProxy_AgentEvents<DaControl>::FireMove (CharacterID, X, Y, Cause);
		}
		catch AnyExceptionDebug
		PostFireEvent (_DEBUG_T("Move"), lEventSinkCount);
	}
	return lEventSinkCount;
}

int DaControl::FireSize(LPCTSTR CharacterID, short Width, short Height)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("Size")))
	{
		try
		{
			lEventSinkCount = CProxy_DaCtlEvents<DaControl>::FireSize (CharacterID, Width, Height)
							+ CProxy_DaCtlEvents2<DaControl>::FireSize (CharacterID, Width, Height)
							+ CProxy_AgentEvents<DaControl>::FireSize (CharacterID, Width, Height);
		}
		catch AnyExceptionDebug
		PostFireEvent (_DEBUG_T("Size"), lEventSinkCount);
	}
	return lEventSinkCount;
}

int DaControl::FireBalloonShow(LPCTSTR CharacterID)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("BalloonShow")))
	{
		try
		{
			lEventSinkCount = CProxy_DaCtlEvents<DaControl>::FireBalloonShow (CharacterID)
							+ CProxy_DaCtlEvents2<DaControl>::FireBalloonShow (CharacterID)
							+ CProxy_AgentEvents<DaControl>::FireBalloonShow (CharacterID);
		}
		catch AnyExceptionDebug
		PostFireEvent (_DEBUG_T("BalloonShow"), lEventSinkCount);
	}
	return lEventSinkCount;
}

int DaControl::FireBalloonHide(LPCTSTR CharacterID)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("BalloonHide")))
	{
		try
		{
			lEventSinkCount = CProxy_DaCtlEvents<DaControl>::FireBalloonHide (CharacterID)
							+ CProxy_DaCtlEvents2<DaControl>::FireBalloonHide (CharacterID)
							+ CProxy_AgentEvents<DaControl>::FireBalloonHide (CharacterID);
		}
		catch AnyExceptionDebug
		PostFireEvent (_DEBUG_T("BalloonHide"), lEventSinkCount);
	}
	return lEventSinkCount;
}

int DaControl::FireListenStart(LPCTSTR CharacterID)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("ListenStart")))
	{
		try
		{
			lEventSinkCount = CProxy_DaCtlEvents<DaControl>::FireListenStart (CharacterID)
							+ CProxy_DaCtlEvents2<DaControl>::FireListenStart (CharacterID)
							+ CProxy_AgentEvents<DaControl>::FireListenStart (CharacterID);
		}
		catch AnyExceptionDebug
		PostFireEvent (_DEBUG_T("ListenStart"), lEventSinkCount);
	}
	return lEventSinkCount;
}

int DaControl::FireListenComplete(LPCTSTR CharacterID, ListenCompleteType Cause)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("ListenComplete")))
	{
		try
		{
			lEventSinkCount = CProxy_DaCtlEvents<DaControl>::FireListenComplete (CharacterID, Cause)
							+ CProxy_DaCtlEvents2<DaControl>::FireListenComplete (CharacterID, Cause)
							+ CProxy_AgentEvents<DaControl>::FireListenComplete (CharacterID, Cause);
		}
		catch AnyExceptionDebug
		PostFireEvent (_DEBUG_T("ListenComplete"), lEventSinkCount);
	}
	return lEventSinkCount;
}

int DaControl::FireDefaultCharacterChange(LPCTSTR CharGUID)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("DefaultCharacterChange")))
	{
		try
		{
			lEventSinkCount = CProxy_DaCtlEvents<DaControl>::FireDefaultCharacterChange (CharGUID)
							+ CProxy_DaCtlEvents2<DaControl>::FireDefaultCharacterChange (CharGUID)
							+ CProxy_AgentEvents<DaControl>::FireDefaultCharacterChange (CharGUID);
		}
		catch AnyExceptionDebug
		PostFireEvent (_DEBUG_T("DefaultCharacterChange"), lEventSinkCount);
	}
	return lEventSinkCount;
}

int DaControl::FireAgentPropertyChange()
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("AgentPropertyChange")))
	{
		try
		{
			lEventSinkCount = CProxy_DaCtlEvents<DaControl>::FireAgentPropertyChange ()
							+ CProxy_DaCtlEvents2<DaControl>::FireAgentPropertyChange ()
							+ CProxy_AgentEvents<DaControl>::FireAgentPropertyChange ();
		}
		catch AnyExceptionDebug
		PostFireEvent (_DEBUG_T("AgentPropertyChange"), lEventSinkCount);
	}
	return lEventSinkCount;
}

int DaControl::FireActiveClientChange(LPCTSTR CharacterID, BOOL Active)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("ActiveClientChange")))
	{
		try
		{
			lEventSinkCount = CProxy_DaCtlEvents<DaControl>::FireActiveClientChange (CharacterID, Active)
							+ CProxy_DaCtlEvents2<DaControl>::FireActiveClientChange (CharacterID, Active)
							+ CProxy_AgentEvents<DaControl>::FireActiveClientChange (CharacterID, Active);
		}
		catch AnyExceptionDebug
		PostFireEvent (_DEBUG_T("ActiveClientChange"), lEventSinkCount);
	}
	return lEventSinkCount;
}

/////////////////////////////////////////////////////////////////////////////

int DaControl::FireSpeechStart(LPCTSTR CharacterID, IDaCtlFormattedText* FormattedText)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("SpeechStart")))
	{
		try
		{
			lEventSinkCount = CProxy_DaCtlEvents2<DaControl>::FireSpeechStart (CharacterID, FormattedText);
		}
		catch AnyExceptionDebug
		PostFireEvent (_DEBUG_T("SpeechStart"), lEventSinkCount);
	}
	return lEventSinkCount;
}

int DaControl::FireSpeechEnd(LPCTSTR CharacterID, IDaCtlFormattedText* FormattedText, VARIANT_BOOL Stopped)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("SpeechEnd")))
	{
		try
		{
			lEventSinkCount = CProxy_DaCtlEvents2<DaControl>::FireSpeechEnd (CharacterID, FormattedText, Stopped);
		}
		catch AnyExceptionDebug
		PostFireEvent (_DEBUG_T("SpeechEnd"), lEventSinkCount);
	}
	return lEventSinkCount;
}

int DaControl::FireSpeechWord(LPCTSTR CharacterID, IDaCtlFormattedText* FormattedText, long WordIndex)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("SpeechWord")))
	{
		try
		{
			lEventSinkCount = CProxy_DaCtlEvents2<DaControl>::FireSpeechWord (CharacterID, FormattedText, WordIndex);
		}
		catch AnyExceptionDebug
		PostFireEvent (_DEBUG_T("SpeechWord"), lEventSinkCount);
	}
	return lEventSinkCount;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CServerNotifySink::CServerNotifySink ()
:	mOwner (NULL),
	mServerNotifyId (0)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CServerNotifySink"), this, max(m_dwRef,-1));
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Add (this);
#endif
}

CServerNotifySink::~CServerNotifySink ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CServerNotifySink::~CServerNotifySink"), this, max(m_dwRef,-1));
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Remove (this);
#endif
	try
	{
		if	(
				(mOwner)
			&&	(mOwner->mServerNotifySink.Ptr() == this)
			)
		{
			mOwner->mServerNotifySink.Detach ();
		}
	}
	catch AnyExceptionSilent
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CServerNotifySink::Initialize (DaControl * pOwner)
{
	HRESULT	lResult = E_FAIL;

	if	(this)
	{
		if	(
				(mOwner = pOwner)
			&&	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mOwner->mServer)))
			)
		{
			try
			{
				if	(
						(SUCCEEDED (LogComErr (LogNormal|LogTime, lResult = mOwner->mServer->Register (this, &mServerNotifyId), _T("Register Server [%p]"), mOwner->mServer.GetInterfacePtr())))
					&&	(mServerNotifyId)
					)
				{
					mOwner->mServer->AddRef ();
				}
			}
			catch AnyExceptionSilent
			_AtlModule.PostServerCall (mOwner->mServer);
		}
	}
	return lResult;
}

HRESULT CServerNotifySink::Terminate ()
{
	HRESULT	lResult = S_FALSE;

	if	(this)
	{
		DaControl *	lOwner = mOwner;
		long		lServerNotifyId = mServerNotifyId;

		mOwner = NULL;
		mServerNotifyId = 0;

		if	(
				(lOwner)
			&&	(lOwner->mServer != NULL)
			&&	(lServerNotifyId)
			)
		{
			try
			{

				_AtlModule.PreServerCall (lOwner->mServer);
				try
				{
					m_dwRef++;
					LogComErr (LogNormal|LogTime, lResult = lOwner->mServer->Unregister (lServerNotifyId), _T("Unregister Server [%p] [%u]"), lOwner->mServer.GetInterfacePtr(), lServerNotifyId);
					m_dwRef--;
				}
				catch AnyExceptionSilent
				_AtlModule.PostServerCall (lOwner->mServer);
			}
			catch AnyExceptionSilent

			try
			{
				lOwner->mServer->Release ();
			}
			catch AnyExceptionSilent
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#define KeyButtonBits(KeyMask)	((KeyMask & 0x1003) | ((KeyMask & 0x0010) >> 2))
#define KeyShiftBits(KeyMask)	(((KeyMask & 0x000C) >> 2) | ((KeyMask & 0x0020) >> 3))

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CServerNotifySink::Command (long CommandID, IDaSvrUserInput2 *UserInput)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CServerNotifySink::Command"), mOwner, max(mOwner->m_dwRef,-1));
#endif
	CAtlString						lActiveCharacterID;
	DaCtlCharacter *				lActiveCharacter;
	CComObject <DaCtlUserInput> *	lUserInput = NULL;
	IDaCtlUserInputPtr				lInterface;

	if	(SUCCEEDED (CComObject <DaCtlUserInput>::CreateInstance (&lUserInput)))
	{
		try
		{
			lActiveCharacterID = mOwner->GetActiveCharacterID ();
			if	(lActiveCharacter = mOwner->GetActiveCharacter ())
			{
				lUserInput->mCommands = lActiveCharacter->GetCommands ();
			}
		}
		catch AnyExceptionSilent

		lUserInput->mCharacterID = lActiveCharacterID;
		lUserInput->mCommandID = CommandID;
		lUserInput->mServerObject = UserInput;
		lInterface = (LPDISPATCH) lUserInput;
	}

	mOwner->FireCommand (lInterface);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CServerNotifySink::ActivateInputState (long CharacterID, long Activated)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CServerNotifySink::ActivateInputState [%d] [%d]"), mOwner, max(mOwner->m_dwRef,-1), CharacterID, Activated);
#endif
	if	(Activated == ActiveState_InputActive)
	{
		mOwner->FireActivateInput (mOwner->GetControlCharacterID (CharacterID));
	}
	else
	if	(Activated == ActiveState_Inactive)
	{
		mOwner->FireDeactivateInput (mOwner->GetControlCharacterID (CharacterID));
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CServerNotifySink::Restart (void)
{
	// Obsolete
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CServerNotifySink::Shutdown (void)
{
	// Obsolete
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CServerNotifySink::VisibleState (long CharacterID, long Visible, long Cause)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CServerNotifySink::VisibleState [%d] [%d] cause [%d]"), mOwner, max(mOwner->m_dwRef,-1), CharacterID, Visible, Cause);
#endif
	if	(Visible)
	{
		mOwner->FireShow (mOwner->GetControlCharacterID (CharacterID), (VisibilityCauseType)Cause);
	}
	else
	{
		mOwner->FireHide (mOwner->GetControlCharacterID (CharacterID), (VisibilityCauseType)Cause);
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CServerNotifySink::Click (long CharacterID, short Keys, long X, long Y)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CServerNotifySink::Click [%d] [%4.4X] [%d %d]"), mOwner, max(mOwner->m_dwRef,-1), CharacterID, Keys, X, Y);
#endif
	mOwner->FireClick (mOwner->GetControlCharacterID (CharacterID), KeyButtonBits(Keys), KeyShiftBits(Keys), (short)X, (short)Y);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CServerNotifySink::DblClick (long CharacterID, short Keys, long X, long Y)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CServerNotifySink::DblClick [%d] [%4.4X] [%d %d]"), mOwner, max(mOwner->m_dwRef,-1), CharacterID, Keys, X, Y);
#endif
	mOwner->FireDblClick (mOwner->GetControlCharacterID (CharacterID), KeyButtonBits(Keys), KeyShiftBits(Keys), (short)X, (short)Y);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CServerNotifySink::DragStart (long CharacterID, short Keys, long X, long Y)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CServerNotifySink::DragStart [%d] [%4.4X] [%d %d]"), mOwner, max(mOwner->m_dwRef,-1), CharacterID, Keys, X, Y);
#endif
	mOwner->FireDragStart (mOwner->GetControlCharacterID (CharacterID), KeyButtonBits(Keys), KeyShiftBits(Keys), (short)X, (short)Y);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CServerNotifySink::DragComplete (long CharacterID, short Keys, long X, long Y)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CServerNotifySink::DragComplete [%d] [%4.4X] [%d %d]"), mOwner, max(mOwner->m_dwRef,-1), CharacterID, Keys, X, Y);
#endif
	mOwner->FireDragComplete (mOwner->GetControlCharacterID (CharacterID), KeyButtonBits(Keys), KeyShiftBits(Keys), (short)X, (short)Y);
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CServerNotifySink::RequestStart (long RequestID)
{
#ifdef	_DEBUG_REQUEST_NOTIFY
	LogMessage (_DEBUG_REQUEST_NOTIFY, _T("[%p(%d)] CServerNotifySink::RequestStart [%d]"), mOwner, max(mOwner->m_dwRef,-1), RequestID);
#endif
	IDaCtlRequestPtr	lInterface;

	lInterface.Attach (mOwner->PutRequest (DaRequestNotifyStart, RequestID, S_OK));

	if	(lInterface != NULL)
	{
		mOwner->FireRequestStart (lInterface);
	}
#ifdef	_DEBUG_REQUEST_NOTIFY
	else
	{
		LogMessage (_DEBUG_REQUEST_NOTIFY, _T("[%p(%d)] CServerNotifySink::RequestStart [%d] IGNORED [%p]"), mOwner, max(mOwner->m_dwRef,-1), RequestID, lInterface.GetInterfacePtr());
	}
#endif
	return S_OK;
}

STDMETHODIMP CServerNotifySink::RequestComplete (long RequestID, long Result)
{
#ifdef	_DEBUG_REQUEST_NOTIFY
	LogMessage (_DEBUG_REQUEST_NOTIFY, _T("[%p(%d)] CServerNotifySink::RequestComplete [%d] [%8.8X]"), mOwner, max(mOwner->m_dwRef,-1), RequestID, Result);
#endif
	IDaCtlRequestPtr	lInterface;

	lInterface.Attach (mOwner->PutRequest (DaRequestNotifyComplete, RequestID, (HRESULT)Result));

	if	(lInterface != NULL)
	{
		mOwner->FireRequestComplete (lInterface);
	}
#ifdef	_DEBUG_REQUEST_NOTIFY
	else
	{
		LogMessage (_DEBUG_REQUEST_NOTIFY, _T("[%p(%d)] CServerNotifySink::RequestComplete [%d] [%8.8X] IGNORED"), mOwner, max(mOwner->m_dwRef,-1), RequestID, Result);
	}
#endif
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CServerNotifySink::BookMark (long BookMarkID)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CServerNotifySink::BookMark [%d]"), mOwner, max(mOwner->m_dwRef,-1), BookMarkID);
#endif
	mOwner->FireBookmark (BookMarkID);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CServerNotifySink::Idle (long CharacterID, long Start)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CServerNotifySink::Idle [%d] [%d]"), mOwner, max(mOwner->m_dwRef,-1), CharacterID, Start);
#endif
	if	(Start)
	{
		mOwner->FireIdleStart (mOwner->GetControlCharacterID (CharacterID));
	}
	else
	{
		mOwner->FireIdleComplete (mOwner->GetControlCharacterID (CharacterID));
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CServerNotifySink::Move (long CharacterID, long X, long Y, long Cause)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CServerNotifySink::Move [%d] [%d %d] cause [%d]"), mOwner, max(mOwner->m_dwRef,-1), CharacterID, X, Y, Cause);
#endif
	mOwner->FireMove (mOwner->GetControlCharacterID (CharacterID), (short)X, (short)Y, (MoveCauseType)Cause);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CServerNotifySink::Size (long CharacterID, long Width, long Height)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CServerNotifySink::Size [%d] [%d %d]"), mOwner, max(mOwner->m_dwRef,-1), CharacterID, Width, Height);
#endif
	mOwner->FireSize (mOwner->GetControlCharacterID (CharacterID), (short)Width, (short)Height);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CServerNotifySink::BalloonVisibleState (long CharacterID, long Visible)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CServerNotifySink::BalloonVisibleState [%d] [%d]"), mOwner, max(mOwner->m_dwRef,-1), CharacterID, Visible);
#endif
	if	(Visible)
	{
		mOwner->FireBalloonShow (mOwner->GetControlCharacterID (CharacterID));
	}
	else
	{
		mOwner->FireBalloonHide (mOwner->GetControlCharacterID (CharacterID));
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CServerNotifySink::HelpComplete (long CharacterID, long CommandID, long Cause)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CServerNotifySink::HelpComplete"), mOwner, max(mOwner->m_dwRef,-1));
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CServerNotifySink::ListeningState (long CharacterID, long Listening, long Cause)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CServerNotifySink::ListeningState"), mOwner, max(mOwner->m_dwRef,-1));
#endif
	if	(Listening)
	{
		mOwner->FireListenStart (mOwner->GetControlCharacterID (CharacterID));
	}
	else
	{
		mOwner->FireListenComplete (mOwner->GetControlCharacterID (CharacterID), (ListenCompleteType)Cause);
	}
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CServerNotifySink::DefaultCharacterChange (BSTR CharGUID)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CServerNotifySink::DefaultCharacterChange [%ls]"), mOwner, max(mOwner->m_dwRef,-1), CharGUID);
#endif
	mOwner->FireDefaultCharacterChange (CAtlString (CharGUID));
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CServerNotifySink::AgentPropertyChange(void)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CServerNotifySink::AgentPropertyChange"), mOwner, max(mOwner->m_dwRef,-1));
#endif
	mOwner->FireAgentPropertyChange ();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CServerNotifySink::ActiveClientChange (long CharacterID, long Status)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CServerNotifySink::ActiveClientChange [%d] [%8.8X]"), mOwner, max(mOwner->m_dwRef,-1), CharacterID, Status);
#endif
	if	(Status == ActiveState_InputActive)
	{
		mOwner->FireActiveClientChange (mOwner->GetControlCharacterID (CharacterID), VARIANT_TRUE);
	}
	else
	if	(Status == ActiveState_Inactive)
	{
		mOwner->FireActiveClientChange (mOwner->GetControlCharacterID (CharacterID), VARIANT_FALSE);
	}
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CServerNotifySink::SpeechStart (long CharacterID, IDaSvrFormattedText* FormattedText)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CServerNotifySink::SpeechStart [%d] [%p]"), mOwner, max(mOwner->m_dwRef,-1), CharacterID, FormattedText);
#endif
	HRESULT									lResult = S_OK;
	tPtr <CComObject <DaCtlFormattedText> >	lObject;
	IDaCtlFormattedTextPtr					lFormattedText;

	if	(FormattedText)
	{
		try
		{
			if	(SUCCEEDED (lResult = CComObject <DaCtlFormattedText>::CreateInstance (lObject.Free())))
			{
				lObject->mServerObject = FormattedText;
				if	(SUCCEEDED (lResult = lObject->SetOwner (mOwner)))
				{
					lFormattedText = lObject.Detach ();
				}
			}
		}
		catch AnyExceptionSilent
	}

	if	(SUCCEEDED (lResult))
	{
		mOwner->FireSpeechStart (mOwner->GetControlCharacterID (CharacterID), lFormattedText);
	}
#ifdef	_DEBUG_NOTIFY_NOT
	if	(LogIsActive (_DEBUG_NOTIFY))
	{
		LogComErrAnon (MinLogLevel(_DEBUG_NOTIFY,LogAlways), lResult, _T("[%p(%d)] CServerNotifySink::SpeechStart [%d] [%p]"), mOwner, max(mOwner->m_dwRef,-1), CharacterID, lFormattedText.GetInterfacePtr());
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotifySink::SpeechEnd (long CharacterID, IDaSvrFormattedText* FormattedText, VARIANT_BOOL Stopped)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CServerNotifySink::SpeechEnd [%d] [%p] [%d]"), mOwner, max(mOwner->m_dwRef,-1), CharacterID, FormattedText, Stopped);
#endif
	HRESULT									lResult = S_OK;
	tPtr <CComObject <DaCtlFormattedText> >	lObject;
	IDaCtlFormattedTextPtr					lFormattedText;

	if	(FormattedText)
	{
		try
		{
			if	(SUCCEEDED (lResult = CComObject <DaCtlFormattedText>::CreateInstance (lObject.Free())))
			{
				lObject->mServerObject = FormattedText;
				if	(SUCCEEDED (lResult = lObject->SetOwner (mOwner)))
				{
					lFormattedText = lObject.Detach ();
				}
			}
		}
		catch AnyExceptionSilent
	}

	if	(SUCCEEDED (lResult))
	{
		mOwner->FireSpeechEnd (mOwner->GetControlCharacterID (CharacterID), lFormattedText, Stopped);
	}
#ifdef	_DEBUG_NOTIFY_NOT
	if	(LogIsActive (_DEBUG_NOTIFY))
	{
		LogComErrAnon (MinLogLevel(_DEBUG_NOTIFY,LogAlways), lResult, _T("[%p(%d)] CServerNotifySink::SpeechEnd [%d] [%p] [%d]"), mOwner, max(mOwner->m_dwRef,-1), CharacterID, lFormattedText.GetInterfacePtr(), Stopped);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotifySink::SpeechWord (long CharacterID, IDaSvrFormattedText* FormattedText, long WordIndex)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CServerNotifySink::SpeechWord [%d] [%p] [%d]"), mOwner, max(mOwner->m_dwRef,-1), CharacterID, FormattedText, WordIndex);
#endif
	HRESULT									lResult = S_OK;
	tPtr <CComObject <DaCtlFormattedText> >	lObject;
	IDaCtlFormattedTextPtr					lFormattedText;

	if	(FormattedText)
	{
		try
		{
			if	(SUCCEEDED (lResult = CComObject <DaCtlFormattedText>::CreateInstance (lObject.Free())))
			{
				lObject->mServerObject = FormattedText;
				if	(SUCCEEDED (lResult = lObject->SetOwner (mOwner)))
				{
					lFormattedText = lObject.Detach ();
				}
			}
		}
		catch AnyExceptionSilent
	}

	if	(SUCCEEDED (lResult))
	{
		mOwner->FireSpeechWord (mOwner->GetControlCharacterID (CharacterID), lFormattedText, WordIndex);
	}
#ifdef	_DEBUG_NOTIFY_NOT
	if	(LogIsActive (_DEBUG_NOTIFY))
	{
		LogComErrAnon (MinLogLevel(_DEBUG_NOTIFY,LogAlways), lResult, _T("[%p(%d)] CServerNotifySink::SpeechWord [%d] [%p] [%d]"), mOwner, max(mOwner->m_dwRef,-1), CharacterID, lFormattedText.GetInterfacePtr(), WordIndex);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CEventNotifyReflect::CEventNotifyReflect (DaControl * pOwner)
{
	mOwner = pOwner;
	mAnchor = pOwner;
	mGlobal = (CEventGlobal*)&_AtlModule;
}

CEventNotifyReflect::~CEventNotifyReflect ()
{
	mOwner = NULL;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CEventNotifyReflect::OnSpeechStart (long CharacterID, LPUNKNOWN FormattedText)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CEventNotifyReflect::OnSpeechStart [%d] [%p]"), mOwner, max(mOwner->m_dwRef,-1), CharacterID, FormattedText);
#endif
	HRESULT	lResult = S_OK;
	int		lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("SpeechStart")))
	{
		try
		{
			IDaCtlFormattedTextPtr	lFormattedText (FormattedText);
			lEventSinkCount = mOwner->FireSpeechStart (mOwner->GetControlCharacterID (CharacterID), lFormattedText);
		}
		catch AnyExceptionDebug
		PostFireEvent (_DEBUG_T("SpeechStart"), lEventSinkCount);
	}
	return lResult;
}

HRESULT CEventNotifyReflect::OnSpeechEnd (long CharacterID, LPUNKNOWN FormattedText, VARIANT_BOOL Stopped)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CEventNotifyReflect::OnSpeechEnd [%d] [%p] [%d]"), mOwner, max(mOwner->m_dwRef,-1), CharacterID, FormattedText, Stopped);
#endif
	HRESULT	lResult = S_OK;
	int		lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("SpeechEnd")))
	{
		try
		{
			IDaCtlFormattedTextPtr	lFormattedText (FormattedText);
			lEventSinkCount = mOwner->FireSpeechEnd (mOwner->GetControlCharacterID (CharacterID), lFormattedText, Stopped);
		}
		catch AnyExceptionDebug
		PostFireEvent (_DEBUG_T("SpeechEnd"), lEventSinkCount);
	}
	return lResult;
}

HRESULT CEventNotifyReflect::OnSpeechWord (long CharacterID, LPUNKNOWN FormattedText, long WordIndex)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CEventNotifyReflect::OnSpeechWord [%d] [%p] [%d]"), mOwner, max(mOwner->m_dwRef,-1), CharacterID, FormattedText, WordIndex);
#endif
	HRESULT	lResult = S_OK;
	int		lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("SpeechWord")))
	{
		try
		{
			IDaCtlFormattedTextPtr	lFormattedText (FormattedText);
			lEventSinkCount = mOwner->FireSpeechWord (mOwner->GetControlCharacterID (CharacterID), lFormattedText, WordIndex);
		}
		catch AnyExceptionDebug
		PostFireEvent (_DEBUG_T("SpeechWord"), lEventSinkCount);
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
