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
#include "DaControl.h"
#include "DaControlObj.h"
#include "DaCtlUserInput.h"
#include "DaCtlCommands.h"
#include "Registry.h"

#ifdef	_DEBUG
#define	_DEBUG_NOTIFY	(GetProfileDebugInt(_T("DebugNotify"),LogVerbose,true)&0xFFFF)
#define	_DEBUG_REQUEST	(GetProfileDebugInt(_T("DebugRequests"),LogVerbose,true)&0xFFFF)
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
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaCtlEventDispatch::FireActivateInput(LPCTSTR CharacterID)
{
	_variant_t				lCharacterId (CharacterID);
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;
	
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.Invoke1 (DISPID_AgentEvents_ActivateInput, &lCharacterId);
		}		
	}
}

void CDaCtlEventDispatch::FireDeactivateInput(LPCTSTR CharacterID)
{
	_variant_t				lCharacterId (CharacterID);
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;
	
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.Invoke1 (DISPID_AgentEvents_DeactivateInput, &lCharacterId);
		}		
	}
}

void CDaCtlEventDispatch::FireClick(LPCTSTR CharacterID, short Button, short Shift, short x, short y)
{
	_variant_t				lParams [5] = {CharacterID, Button, Shift, x, y};
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;
	
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.InvokeN (DISPID_AgentEvents_Click, lParams, 5);
		}		
	}
}

void CDaCtlEventDispatch::FireDblClick(LPCTSTR CharacterID, short Button, short Shift, short x, short y)
{
	_variant_t				lParams [5] = {CharacterID, Button, Shift, x, y};
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;
	
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.InvokeN (DISPID_AgentEvents_DblClick, lParams, 5);
		}		
	}
}

void CDaCtlEventDispatch::FireDragStart(LPCTSTR CharacterID, short Button, short Shift, short x, short y)
{
	_variant_t				lParams [5] = {CharacterID, Button, Shift, x, y};
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;
	
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.InvokeN (DISPID_AgentEvents_DragStart, lParams, 5);
		}		
	}
}

void CDaCtlEventDispatch::FireDragComplete(LPCTSTR CharacterID, short Button, short Shift, short x, short y)
{
	_variant_t				lParams [5] = {CharacterID, Button, Shift, x, y};
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;
	
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.InvokeN (DISPID_AgentEvents_DragComplete, lParams, 5);
		}		
	}
}

void CDaCtlEventDispatch::FireShow(LPCTSTR CharacterID, short Cause)
{
	_variant_t				lCharacterId (CharacterID);
	_variant_t				lCause (Cause);
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;
	
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.Invoke2 (DISPID_AgentEvents_Show, &lCharacterId, &lCause);
		}		
	}
}

void CDaCtlEventDispatch::FireHide(LPCTSTR CharacterID, short Cause)
{
	_variant_t				lCharacterId (CharacterID);
	_variant_t				lCause (Cause);
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;
	
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.Invoke2 (DISPID_AgentEvents_Hide, &lCharacterId, &lCause);
		}		
	}
}

void CDaCtlEventDispatch::FireRequestStart(LPDISPATCH Request)
{
	_variant_t				lRequest (Request);
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;
	
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.Invoke1 (DISPID_AgentEvents_RequestStart, &lRequest);
		}		
	}
}

void CDaCtlEventDispatch::FireRequestComplete(LPDISPATCH Request)
{
	_variant_t				lRequest (Request);
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;
	
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.Invoke1 (DISPID_AgentEvents_RequestComplete, &lRequest);
		}		
	}
}

void CDaCtlEventDispatch::FireRestart()
{
// Obsolete
}

void CDaCtlEventDispatch::FireShutdown()
{
// Obsolete
}

void CDaCtlEventDispatch::FireBookmark(long BookmarkID)
{
	_variant_t				lBookmarkID (BookmarkID);
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;
	
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.Invoke1 (DISPID_AgentEvents_Bookmark, &lBookmarkID);
		}		
	}
}

void CDaCtlEventDispatch::FireCommand(LPDISPATCH UserInput)
{
	_variant_t				lUserInput (UserInput);
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;
	
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.Invoke1 (DISPID_AgentEvents_Command, &lUserInput);
		}		
	}
}

void CDaCtlEventDispatch::FireIdleStart(LPCTSTR CharacterID)
{
	_variant_t				lCharacterID (CharacterID);
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;
	
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.Invoke1 (DISPID_AgentEvents_IdleStart, &lCharacterID);
		}		
	}
}

void CDaCtlEventDispatch::FireIdleComplete(LPCTSTR CharacterID)
{
	_variant_t				lCharacterID (CharacterID);
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;
	
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.Invoke1 (DISPID_AgentEvents_IdleComplete, &lCharacterID);
		}		
	}
}

void CDaCtlEventDispatch::FireMove(LPCTSTR CharacterID, short x, short y, short Cause)
{
	_variant_t				lParams [4] = {CharacterID, x, y, Cause};
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;
	
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.InvokeN (DISPID_AgentEvents_Move, lParams, 4);
		}		
	}
}

void CDaCtlEventDispatch::FireSize(LPCTSTR CharacterID, short Width, short Height)
{
	_variant_t				lParams [3] = {CharacterID, Width, Height};
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;
	
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.InvokeN (DISPID_AgentEvents_Size, lParams, 3);
		}		
	}
}

void CDaCtlEventDispatch::FireBalloonShow(LPCTSTR CharacterID)
{
	_variant_t				lCharacterID (CharacterID);
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;
	
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.Invoke1 (DISPID_AgentEvents_BalloonShow, &lCharacterID);
		}		
	}
}

void CDaCtlEventDispatch::FireBalloonHide(LPCTSTR CharacterID)
{
	_variant_t				lCharacterID (CharacterID);
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;
	
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.Invoke1 (DISPID_AgentEvents_BalloonHide, &lCharacterID);
		}		
	}
}

void CDaCtlEventDispatch::FireHelpComplete(LPCTSTR CharacterID, LPCTSTR Name, short Cause)
{
	_variant_t				lParams [3] = {CharacterID, Name, Cause};
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;
	
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.InvokeN (DISPID_AgentEvents_HelpComplete, lParams, 3);
		}		
	}
}

void CDaCtlEventDispatch::FireListenStart(LPCTSTR CharacterID)
{
	_variant_t				lCharacterID (CharacterID);
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;
	
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.Invoke1 (DISPID_AgentEvents_ListenStart, &lCharacterID);
		}		
	}
}

void CDaCtlEventDispatch::FireListenComplete(LPCTSTR CharacterID, short Cause)
{
	_variant_t				lCharacterID (CharacterID);
	_variant_t				lCause (Cause);
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;
	
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.Invoke2 (DISPID_AgentEvents_ListenComplete, &lCharacterID, &lCause);
		}		
	}
}

void CDaCtlEventDispatch::FireDefaultCharacterChange(LPCTSTR GUID)
{
	_variant_t				lGUID (GUID);
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;
	
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.Invoke1 (DISPID_AgentEvents_DefaultCharacterChange, &lGUID);
		}		
	}
}

void CDaCtlEventDispatch::FireAgentPropertyChange()
{
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;
	
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.Invoke0 (DISPID_AgentEvents_AgentPropertyChange);
		}		
	}
}

void CDaCtlEventDispatch::FireActiveClientChange(LPCTSTR CharacterID, BOOL Active)
{
	_variant_t				lCharacterID (CharacterID);
	_variant_t				lActive (Active);
	int						lNdx;
	CComQIPtr <IDispatch>	lEventSink;
	
	for	(lNdx = 0; lNdx < mUnkArray.GetSize(); lNdx++)
	{
		lEventSink = mUnkArray.GetAt (lNdx);
		if	(lEventSink != NULL)
		{
			lEventSink.Invoke2 (DISPID_AgentEvents_ActiveClientChange, &lCharacterID, &lActive);
		}		
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaControlObj::FireActivateInput(LPCTSTR CharacterID)
{
	CProxy_DaCtlEvents<CDaControlObj>::FireActivateInput (CharacterID);
	CProxy_AgentEvents<CDaControlObj>::FireActivateInput (CharacterID);
}

void CDaControlObj::FireDeactivateInput(LPCTSTR CharacterID)
{
	CProxy_DaCtlEvents<CDaControlObj>::FireDeactivateInput (CharacterID);
	CProxy_AgentEvents<CDaControlObj>::FireDeactivateInput (CharacterID);
}

void CDaControlObj::FireClick(LPCTSTR CharacterID, short Button, short Shift, short x, short y)
{
	CProxy_DaCtlEvents<CDaControlObj>::FireClick (CharacterID, Button, Shift, x, y);
	CProxy_AgentEvents<CDaControlObj>::FireClick (CharacterID, Button, Shift, x, y);
}

void CDaControlObj::FireDblClick(LPCTSTR CharacterID, short Button, short Shift, short x, short y)
{
	CProxy_DaCtlEvents<CDaControlObj>::FireDblClick (CharacterID, Button, Shift, x, y);
	CProxy_AgentEvents<CDaControlObj>::FireDblClick (CharacterID, Button, Shift, x, y);
}

void CDaControlObj::FireDragStart(LPCTSTR CharacterID, short Button, short Shift, short x, short y)
{
	CProxy_DaCtlEvents<CDaControlObj>::FireDragStart (CharacterID, Button, Shift, x, y);
	CProxy_AgentEvents<CDaControlObj>::FireDragStart (CharacterID, Button, Shift, x, y);
}

void CDaControlObj::FireDragComplete(LPCTSTR CharacterID, short Button, short Shift, short x, short y)
{
	CProxy_DaCtlEvents<CDaControlObj>::FireDragComplete (CharacterID, Button, Shift, x, y);
	CProxy_AgentEvents<CDaControlObj>::FireDragComplete (CharacterID, Button, Shift, x, y);
}

void CDaControlObj::FireShow(LPCTSTR CharacterID, short Cause)
{
	CProxy_DaCtlEvents<CDaControlObj>::FireShow (CharacterID, Cause);
	CProxy_AgentEvents<CDaControlObj>::FireShow (CharacterID, Cause);
}

void CDaControlObj::FireHide(LPCTSTR CharacterID, short Cause)
{
	CProxy_DaCtlEvents<CDaControlObj>::FireHide (CharacterID, Cause);
	CProxy_AgentEvents<CDaControlObj>::FireHide (CharacterID, Cause);
}

void CDaControlObj::FireRequestStart(LPDISPATCH Request)
{
	CProxy_DaCtlEvents<CDaControlObj>::FireRequestStart (Request);
	CProxy_AgentEvents<CDaControlObj>::FireRequestStart (Request);
}

void CDaControlObj::FireRequestComplete(LPDISPATCH Request)
{
	CProxy_DaCtlEvents<CDaControlObj>::FireRequestComplete (Request);
	CProxy_AgentEvents<CDaControlObj>::FireRequestComplete (Request);
}

void CDaControlObj::FireRestart()
{
// Obsolete
}

void CDaControlObj::FireShutdown()
{
// Obsolete
}

void CDaControlObj::FireBookmark(long BookmarkID)
{
	CProxy_DaCtlEvents<CDaControlObj>::FireBookmark (BookmarkID);
	CProxy_AgentEvents<CDaControlObj>::FireBookmark (BookmarkID);
}

void CDaControlObj::FireCommand(LPDISPATCH UserInput)
{
	CProxy_DaCtlEvents<CDaControlObj>::FireCommand (UserInput);
	CProxy_AgentEvents<CDaControlObj>::FireCommand (UserInput);
}

void CDaControlObj::FireIdleStart(LPCTSTR CharacterID)
{
	CProxy_DaCtlEvents<CDaControlObj>::FireIdleStart (CharacterID);
	CProxy_AgentEvents<CDaControlObj>::FireIdleStart (CharacterID);
}

void CDaControlObj::FireIdleComplete(LPCTSTR CharacterID)
{
	CProxy_DaCtlEvents<CDaControlObj>::FireIdleComplete (CharacterID);
	CProxy_AgentEvents<CDaControlObj>::FireIdleComplete (CharacterID);
}

void CDaControlObj::FireMove(LPCTSTR CharacterID, short x, short y, short Cause)
{
	CProxy_DaCtlEvents<CDaControlObj>::FireMove (CharacterID, x, y, Cause);
	CProxy_AgentEvents<CDaControlObj>::FireMove (CharacterID, x, y, Cause);
}

void CDaControlObj::FireSize(LPCTSTR CharacterID, short Width, short Height)
{
	CProxy_DaCtlEvents<CDaControlObj>::FireSize (CharacterID, Width, Height);
	CProxy_AgentEvents<CDaControlObj>::FireSize (CharacterID, Width, Height);
}

void CDaControlObj::FireBalloonShow(LPCTSTR CharacterID)
{
	CProxy_DaCtlEvents<CDaControlObj>::FireBalloonShow (CharacterID);
	CProxy_AgentEvents<CDaControlObj>::FireBalloonShow (CharacterID);
}

void CDaControlObj::FireBalloonHide(LPCTSTR CharacterID)
{
	CProxy_DaCtlEvents<CDaControlObj>::FireBalloonHide (CharacterID);
	CProxy_AgentEvents<CDaControlObj>::FireBalloonHide (CharacterID);
}

void CDaControlObj::FireHelpComplete(LPCTSTR CharacterID, LPCTSTR Name, short Cause)
{
	CProxy_DaCtlEvents<CDaControlObj>::FireHelpComplete (CharacterID, Name, Cause);
	CProxy_AgentEvents<CDaControlObj>::FireHelpComplete (CharacterID, Name, Cause);
}

void CDaControlObj::FireListenStart(LPCTSTR CharacterID)
{
	CProxy_DaCtlEvents<CDaControlObj>::FireListenStart (CharacterID);
	CProxy_AgentEvents<CDaControlObj>::FireListenStart (CharacterID);
}

void CDaControlObj::FireListenComplete(LPCTSTR CharacterID, short Cause)
{
	CProxy_DaCtlEvents<CDaControlObj>::FireListenComplete (CharacterID, Cause);
	CProxy_AgentEvents<CDaControlObj>::FireListenComplete (CharacterID, Cause);
}

void CDaControlObj::FireDefaultCharacterChange(LPCTSTR GUID)
{
	CProxy_DaCtlEvents<CDaControlObj>::FireDefaultCharacterChange (GUID);
	CProxy_AgentEvents<CDaControlObj>::FireDefaultCharacterChange (GUID);
}

void CDaControlObj::FireAgentPropertyChange()
{
	CProxy_DaCtlEvents<CDaControlObj>::FireAgentPropertyChange ();
	CProxy_AgentEvents<CDaControlObj>::FireAgentPropertyChange ();
}

void CDaControlObj::FireActiveClientChange(LPCTSTR CharacterID, BOOL Active)
{
	CProxy_DaCtlEvents<CDaControlObj>::FireActiveClientChange (CharacterID, Active);
	CProxy_AgentEvents<CDaControlObj>::FireActiveClientChange (CharacterID, Active);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDaControlObj::CServerNotifySink::CServerNotifySink ()
:	mOwner (NULL),
	mServerNotifyId (0)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CServerNotifySink"), this, m_dwRef);
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Add (this);
#endif
}

CDaControlObj::CServerNotifySink::~CServerNotifySink ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CServerNotifySink::~CServerNotifySink"), this, m_dwRef);
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

HRESULT CDaControlObj::CServerNotifySink::Initialize (CDaControlObj * pOwner)
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
						(SUCCEEDED (LogComErr (LogNormal, lResult = mOwner->mServer->Register (this, &mServerNotifyId), _T("Register Server [%p]"), mOwner->mServer.GetInterfacePtr())))
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

HRESULT CDaControlObj::CServerNotifySink::Terminate ()
{
	HRESULT	lResult = S_FALSE;

	if	(this)
	{
		CDaControlObj *	lOwner = mOwner;
		long			lServerNotifyId = mServerNotifyId;

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
					LogComErr (LogNormal, lResult = lOwner->mServer->Unregister (lServerNotifyId), _T("Unregister Server [%p] [%u]"), lOwner->mServer.GetInterfacePtr(), lServerNotifyId);
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

HRESULT STDMETHODCALLTYPE CDaControlObj::CServerNotifySink::Command (long dwCommandID, IUnknown *punkUserInput)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[[%p(%d)] CDaControlObj::CServerNotifySink::Command"), mOwner, mOwner->m_dwRef);
#endif
	CString							lActiveCharacterID;
	CDaCtlCharacter *				lActiveCharacter;
	CComObject <CDaCtlUserInput> *	lUserInput = NULL;
	IDaCtlUserInputPtr				lInterface;

	if	(SUCCEEDED (CComObject <CDaCtlUserInput>::CreateInstance (&lUserInput)))
	{
		try
		{
			CDaCtlCommands *	lCommands;
			IDaCtlCommandsPtr	lInterface;

			lActiveCharacterID = mOwner->GetActiveCharacterID ();

			if	(
					(lActiveCharacter = mOwner->GetActiveCharacter ())
				&&	(lCommands = lActiveCharacter->GetCommands (lInterface))
				)
			{
				lUserInput->mServerCommands = lCommands->mServerObject;
			}
		}
		catch AnyExceptionSilent

		lUserInput->mCharacterID = lActiveCharacterID;
		lUserInput->mServerObject = punkUserInput;
		lInterface = (LPDISPATCH) lUserInput;
	}

	if	(_AtlModule.PreNotify ())
	{
		try
		{
			mOwner->FireCommand (lInterface);
		}
		catch AnyExceptionDebug
		_AtlModule.PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaControlObj::CServerNotifySink::ActivateInputState (long dwCharID, long bActivated)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CDaControlObj::CServerNotifySink::ActivateInputState [%d] [%d]"), mOwner, mOwner->m_dwRef, dwCharID, bActivated);
#endif
	if	(bActivated == ActiveType_InputActive)
	{
		if	(_AtlModule.PreNotify ())
		{
			try
			{
				mOwner->FireActivateInput (mOwner->GetControlCharacterID (dwCharID));
			}
			catch AnyExceptionDebug
			_AtlModule.PostNotify ();
		}
	}
	else
	if	(bActivated == ActiveType_Inactive)
	{
		if	(_AtlModule.PreNotify ())
		{
			try
			{
				mOwner->FireDeactivateInput (mOwner->GetControlCharacterID (dwCharID));
			}
			catch AnyExceptionDebug
			_AtlModule.PostNotify ();
		}
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaControlObj::CServerNotifySink::Restart (void)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CDaControlObj::CServerNotifySink::Restart"), mOwner, mOwner->m_dwRef);
#endif
	// Obsolete
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaControlObj::CServerNotifySink::Shutdown (void)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CDaControlObj::CServerNotifySink::Shutdown"), mOwner, mOwner->m_dwRef);
#endif
	// Obsolete
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaControlObj::CServerNotifySink::VisibleState (long dwCharID, long bVisible, long dwCause)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CDaControlObj::CServerNotifySink::VisibleState [%d] [%d] cause [%d]"), mOwner, mOwner->m_dwRef, dwCharID, bVisible, dwCause);
#endif
	if	(_AtlModule.PreNotify ())
	{
		try
		{
			if	(bVisible)
			{
				mOwner->FireShow (mOwner->GetControlCharacterID (dwCharID), (short)dwCause);
			}
			else
			{
				mOwner->FireHide (mOwner->GetControlCharacterID (dwCharID), (short)dwCause);
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaControlObj::CServerNotifySink::Click (long dwCharID, short fwKeys, long x, long y)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[[%p(%d)] CDaControlObj::CServerNotifySink::Click [%d] [%4.4X] [%d %d]"), mOwner, mOwner->m_dwRef, dwCharID, fwKeys, x, y);
#endif
	if	(_AtlModule.PreNotify ())
	{
		try
		{
			mOwner->FireClick (mOwner->GetControlCharacterID (dwCharID), KeyButtonBits(fwKeys), KeyShiftBits(fwKeys), (short)x, (short)y);
		}
		catch AnyExceptionDebug
		_AtlModule.PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaControlObj::CServerNotifySink::DblClick (long dwCharID, short fwKeys, long x, long y)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CDaControlObj::CServerNotifySink::DblClick [%d] [%4.4X] [%d %d]"), mOwner, mOwner->m_dwRef, dwCharID, fwKeys, x, y);
#endif
	if	(_AtlModule.PreNotify ())
	{
		try
		{
			mOwner->FireDblClick (mOwner->GetControlCharacterID (dwCharID), KeyButtonBits(fwKeys), KeyShiftBits(fwKeys), (short)x, (short)y);
		}
		catch AnyExceptionDebug
		_AtlModule.PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaControlObj::CServerNotifySink::DragStart (long dwCharID, short fwKeys, long x, long y)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CDaControlObj::CServerNotifySink::DragStart [%d] [%4.4X] [%d %d]"), mOwner, mOwner->m_dwRef, dwCharID, fwKeys, x, y);
#endif
	if	(_AtlModule.PreNotify ())
	{
		try
		{
			mOwner->FireDragStart (mOwner->GetControlCharacterID (dwCharID), KeyButtonBits(fwKeys), KeyShiftBits(fwKeys), (short)x, (short)y);
		}
		catch AnyExceptionDebug
		_AtlModule.PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaControlObj::CServerNotifySink::DragComplete (long dwCharID, short fwKeys, long x, long y)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CDaControlObj::CServerNotifySink::DragComplete [%d] [%4.4X] [%d %d]"), mOwner, mOwner->m_dwRef, dwCharID, fwKeys, x, y);
#endif
	if	(_AtlModule.PreNotify ())
	{
		try
		{
			mOwner->FireDragComplete (mOwner->GetControlCharacterID (dwCharID), KeyButtonBits(fwKeys), KeyShiftBits(fwKeys), (short)x, (short)y);
		}
		catch AnyExceptionDebug
		_AtlModule.PostNotify ();
	}
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CDaControlObj::CServerNotifySink::RequestStart (long dwRequestID)
{
#ifdef	_DEBUG_REQUEST_NOTIFY
	LogMessage (_DEBUG_REQUEST_NOTIFY, _T("[%p(%d)] CDaControlObj::CServerNotifySink::RequestStart [%d]"), mOwner, mOwner->m_dwRef, dwRequestID);
#endif
	IDaCtlRequestPtr	lInterface;

	lInterface.Attach (mOwner->PutRequest (DaRequestNotifyStart, dwRequestID, S_OK));

	if	(
			(lInterface != NULL)
		&&	(_AtlModule.PreNotify ())
		)
	{
		try
		{
			mOwner->FireRequestStart (lInterface);
		}
		catch AnyExceptionDebug
		_AtlModule.PostNotify ();
	}
#ifdef	_DEBUG_REQUEST_NOTIFY
	else
	{
		LogMessage (_DEBUG_REQUEST_NOTIFY, _T("[%p(%d)] CDaControlObj::CServerNotifySink::RequestStart [%d] IGNORED"), mOwner, mOwner->m_dwRef, dwRequestID);
	}
#endif
	return S_OK;
}

STDMETHODIMP CDaControlObj::CServerNotifySink::RequestComplete (long dwRequestID, long hrStatus)
{
#ifdef	_DEBUG_REQUEST_NOTIFY
	LogMessage (_DEBUG_REQUEST_NOTIFY, _T("[%p(%d)] CDaControlObj::CServerNotifySink::RequestComplete [%d] [%8.8X]"), mOwner, mOwner->m_dwRef, dwRequestID, hrStatus);
#endif
	IDaCtlRequestPtr	lInterface;

	lInterface.Attach (mOwner->PutRequest (DaRequestNotifyComplete, dwRequestID, (HRESULT)hrStatus));

	if	(
			(lInterface != NULL)
		&&	(_AtlModule.PreNotify ())
		)
	{
		try
		{
			mOwner->FireRequestComplete (lInterface);
		}
		catch AnyExceptionDebug
		_AtlModule.PostNotify ();
	}
#ifdef	_DEBUG_REQUEST_NOTIFY
	else
	{
		LogMessage (_DEBUG_REQUEST_NOTIFY, _T("[%p(%d)] CDaControlObj::CServerNotifySink::RequestComplete [%d] [%8.8X] IGNORED"), mOwner, mOwner->m_dwRef, dwRequestID, hrStatus);
	}
#endif
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaControlObj::CServerNotifySink::BookMark (long dwBookMarkID)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CDaControlObj::CServerNotifySink::BookMark [%d]"), mOwner, mOwner->m_dwRef, dwBookMarkID);
#endif
	if	(_AtlModule.PreNotify ())
	{
		try
		{
			mOwner->FireBookmark (dwBookMarkID);
		}
		catch AnyExceptionDebug
		_AtlModule.PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaControlObj::CServerNotifySink::Idle (long dwCharID, long bStart)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CDaControlObj::CServerNotifySink::Idle [%d] [%d]"), mOwner, mOwner->m_dwRef, dwCharID, bStart);
#endif
	if	(_AtlModule.PreNotify ())
	{
		try
		{
			if	(bStart)
			{
				mOwner->FireIdleStart (mOwner->GetControlCharacterID (dwCharID));
			}
			else
			{
				mOwner->FireIdleComplete (mOwner->GetControlCharacterID (dwCharID));
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaControlObj::CServerNotifySink::Move (long dwCharID, long x, long y, long dwCause)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CDaControlObj::CServerNotifySink::Move [%d] [%d %d] cause [%d]"), mOwner, mOwner->m_dwRef, dwCharID, x, y, dwCause);
#endif
	if	(_AtlModule.PreNotify ())
	{
		try
		{
			mOwner->FireMove (mOwner->GetControlCharacterID (dwCharID), (short)x, (short)y, (short)dwCause);
		}
		catch AnyExceptionDebug
		_AtlModule.PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaControlObj::CServerNotifySink::Size (long dwCharID, long lWidth, long lHeight)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CDaControlObj::CServerNotifySink::Size [%d] [%d %d]"), mOwner, mOwner->m_dwRef, dwCharID, lWidth, lHeight);
#endif
	if	(_AtlModule.PreNotify ())
	{
		try
		{
			mOwner->FireSize (mOwner->GetControlCharacterID (dwCharID), (short)lWidth, (short)lHeight);
		}
		catch AnyExceptionDebug
		_AtlModule.PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaControlObj::CServerNotifySink::BalloonVisibleState (long dwCharID, long bVisible)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CDaControlObj::CServerNotifySink::BalloonVisibleState [%d] [%d]"), mOwner, mOwner->m_dwRef, dwCharID, bVisible);
#endif
	if	(_AtlModule.PreNotify ())
	{
		try
		{
			if	(bVisible)
			{
				mOwner->FireBalloonShow (mOwner->GetControlCharacterID (dwCharID));
			}
			else
			{
				mOwner->FireBalloonHide (mOwner->GetControlCharacterID (dwCharID));
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaControlObj::CServerNotifySink::HelpComplete (long dwCharID, long dwCommandID, long dwCause)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CDaControlObj::CServerNotifySink::HelpComplete"), mOwner, mOwner->m_dwRef);
#endif
	if	(_AtlModule.PreNotify ())
	{
		try
		{
			mOwner->FireHelpComplete (mOwner->GetControlCharacterID (dwCharID), _T(""), (short)dwCause);
		}
		catch AnyExceptionDebug
		_AtlModule.PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaControlObj::CServerNotifySink::ListeningState (long dwCharID, long bListening, long dwCause)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CDaControlObj::CServerNotifySink::ListeningState"), mOwner, mOwner->m_dwRef);
#endif
	if	(_AtlModule.PreNotify ())
	{
		try
		{
			if	(bListening)
			{
				mOwner->FireListenStart (mOwner->GetControlCharacterID (dwCharID));
			}
			else
			{
				mOwner->FireListenComplete (mOwner->GetControlCharacterID (dwCharID), (short)dwCause);
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostNotify ();
	}
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaControlObj::CServerNotifySink::DefaultCharacterChange (BSTR bszGUID)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CDaControlObj::CServerNotifySink::DefaultCharacterChange [%ls]"), mOwner, mOwner->m_dwRef, bszGUID);
#endif
	if	(_AtlModule.PreNotify ())
	{
		try
		{
			mOwner->FireDefaultCharacterChange (CString (bszGUID));
		}
		catch AnyExceptionDebug
		_AtlModule.PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaControlObj::CServerNotifySink::AgentPropertyChange(void)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CDaControlObj::CServerNotifySink::AgentPropertyChange"), mOwner, mOwner->m_dwRef);
#endif
	if	(_AtlModule.PreNotify ())
	{
		try
		{
			mOwner->FireAgentPropertyChange ();
		}
		catch AnyExceptionDebug
		_AtlModule.PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDaControlObj::CServerNotifySink::ActiveClientChange (long dwCharID, long lStatus)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] CDaControlObj::CServerNotifySink::ActiveClientChange [%d] [%8.8X]"), mOwner, mOwner->m_dwRef, dwCharID, lStatus);
#endif
	if	(lStatus == ActiveType_InputActive)
	{
		if	(_AtlModule.PreNotify ())
		{
			try
			{
				mOwner->FireActiveClientChange (mOwner->GetControlCharacterID (dwCharID), VARIANT_TRUE);
			}
			catch AnyExceptionDebug
			_AtlModule.PostNotify ();
		}
	}
	else
	if	(lStatus == ActiveType_Inactive)
	{
		if	(_AtlModule.PreNotify ())
		{
			try
			{
				mOwner->FireActiveClientChange (mOwner->GetControlCharacterID (dwCharID), VARIANT_FALSE);
			}
			catch AnyExceptionDebug
			_AtlModule.PostNotify ();
		}
	}
	return S_OK;
}
