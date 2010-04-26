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
	int 					lNdx;
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
	int 					lNdx;
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
	int 					lNdx;
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
	int 					lNdx;
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
	int 					lNdx;
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
	int 					lNdx;
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

void CDaCtlEventDispatch::FireShow(LPCTSTR CharacterID, VisibilityCauseType Cause)
{
	_variant_t				lCharacterId (CharacterID);
	_variant_t				lCause (Cause);
	int 					lNdx;
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

void CDaCtlEventDispatch::FireHide(LPCTSTR CharacterID, VisibilityCauseType Cause)
{
	_variant_t				lCharacterId (CharacterID);
	_variant_t				lCause (Cause);
	int 					lNdx;
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
	int 					lNdx;
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
	int 					lNdx;
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

void CDaCtlEventDispatch::FireBookmark(long BookmarkID)
{
	_variant_t				lBookmarkID (BookmarkID);
	int 					lNdx;
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
	int 					lNdx;
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

void CDaCtlEventDispatch::FireMove(LPCTSTR CharacterID, short x, short y, MoveCauseType Cause)
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

void CDaCtlEventDispatch::FireListenComplete(LPCTSTR CharacterID, ListenCompleteType Cause)
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

void CDaCtlEventDispatch::FireDefaultCharacterChange(LPCTSTR CharGUID)
{
	_variant_t				lGUID (CharGUID);
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

void DaControl::FireActivateInput(LPCTSTR CharacterID)
{
	CProxy_DaCtlEvents<DaControl>::FireActivateInput (CharacterID);
	CProxy_AgentEvents<DaControl>::FireActivateInput (CharacterID);
}

void DaControl::FireDeactivateInput(LPCTSTR CharacterID)
{
	CProxy_DaCtlEvents<DaControl>::FireDeactivateInput (CharacterID);
	CProxy_AgentEvents<DaControl>::FireDeactivateInput (CharacterID);
}

void DaControl::FireClick(LPCTSTR CharacterID, short Button, short Shift, short x, short y)
{
	CProxy_DaCtlEvents<DaControl>::FireClick (CharacterID, Button, Shift, x, y);
	CProxy_AgentEvents<DaControl>::FireClick (CharacterID, Button, Shift, x, y);
}

void DaControl::FireDblClick(LPCTSTR CharacterID, short Button, short Shift, short x, short y)
{
	CProxy_DaCtlEvents<DaControl>::FireDblClick (CharacterID, Button, Shift, x, y);
	CProxy_AgentEvents<DaControl>::FireDblClick (CharacterID, Button, Shift, x, y);
}

void DaControl::FireDragStart(LPCTSTR CharacterID, short Button, short Shift, short x, short y)
{
	CProxy_DaCtlEvents<DaControl>::FireDragStart (CharacterID, Button, Shift, x, y);
	CProxy_AgentEvents<DaControl>::FireDragStart (CharacterID, Button, Shift, x, y);
}

void DaControl::FireDragComplete(LPCTSTR CharacterID, short Button, short Shift, short x, short y)
{
	CProxy_DaCtlEvents<DaControl>::FireDragComplete (CharacterID, Button, Shift, x, y);
	CProxy_AgentEvents<DaControl>::FireDragComplete (CharacterID, Button, Shift, x, y);
}

void DaControl::FireShow(LPCTSTR CharacterID, VisibilityCauseType Cause)
{
	CProxy_DaCtlEvents<DaControl>::FireShow (CharacterID, Cause);
	CProxy_AgentEvents<DaControl>::FireShow (CharacterID, Cause);
}

void DaControl::FireHide(LPCTSTR CharacterID, VisibilityCauseType Cause)
{
	CProxy_DaCtlEvents<DaControl>::FireHide (CharacterID, Cause);
	CProxy_AgentEvents<DaControl>::FireHide (CharacterID, Cause);
}

void DaControl::FireRequestStart(LPDISPATCH Request)
{
	CProxy_DaCtlEvents<DaControl>::FireRequestStart (Request);
	CProxy_AgentEvents<DaControl>::FireRequestStart (Request);
}

void DaControl::FireRequestComplete(LPDISPATCH Request)
{
	CProxy_DaCtlEvents<DaControl>::FireRequestComplete (Request);
	CProxy_AgentEvents<DaControl>::FireRequestComplete (Request);
}

void DaControl::FireBookmark(long BookmarkID)
{
	CProxy_DaCtlEvents<DaControl>::FireBookmark (BookmarkID);
	CProxy_AgentEvents<DaControl>::FireBookmark (BookmarkID);
}

void DaControl::FireCommand(LPDISPATCH UserInput)
{
	CProxy_DaCtlEvents<DaControl>::FireCommand (UserInput);
	CProxy_AgentEvents<DaControl>::FireCommand (UserInput);
}

void DaControl::FireIdleStart(LPCTSTR CharacterID)
{
	CProxy_DaCtlEvents<DaControl>::FireIdleStart (CharacterID);
	CProxy_AgentEvents<DaControl>::FireIdleStart (CharacterID);
}

void DaControl::FireIdleComplete(LPCTSTR CharacterID)
{
	CProxy_DaCtlEvents<DaControl>::FireIdleComplete (CharacterID);
	CProxy_AgentEvents<DaControl>::FireIdleComplete (CharacterID);
}

void DaControl::FireMove(LPCTSTR CharacterID, short x, short y, MoveCauseType Cause)
{
	CProxy_DaCtlEvents<DaControl>::FireMove (CharacterID, x, y, Cause);
	CProxy_AgentEvents<DaControl>::FireMove (CharacterID, x, y, Cause);
}

void DaControl::FireSize(LPCTSTR CharacterID, short Width, short Height)
{
	CProxy_DaCtlEvents<DaControl>::FireSize (CharacterID, Width, Height);
	CProxy_AgentEvents<DaControl>::FireSize (CharacterID, Width, Height);
}

void DaControl::FireBalloonShow(LPCTSTR CharacterID)
{
	CProxy_DaCtlEvents<DaControl>::FireBalloonShow (CharacterID);
	CProxy_AgentEvents<DaControl>::FireBalloonShow (CharacterID);
}

void DaControl::FireBalloonHide(LPCTSTR CharacterID)
{
	CProxy_DaCtlEvents<DaControl>::FireBalloonHide (CharacterID);
	CProxy_AgentEvents<DaControl>::FireBalloonHide (CharacterID);
}

void DaControl::FireListenStart(LPCTSTR CharacterID)
{
	CProxy_DaCtlEvents<DaControl>::FireListenStart (CharacterID);
	CProxy_AgentEvents<DaControl>::FireListenStart (CharacterID);
}

void DaControl::FireListenComplete(LPCTSTR CharacterID, ListenCompleteType Cause)
{
	CProxy_DaCtlEvents<DaControl>::FireListenComplete (CharacterID, Cause);
	CProxy_AgentEvents<DaControl>::FireListenComplete (CharacterID, Cause);
}

void DaControl::FireDefaultCharacterChange(LPCTSTR CharGUID)
{
	CProxy_DaCtlEvents<DaControl>::FireDefaultCharacterChange (CharGUID);
	CProxy_AgentEvents<DaControl>::FireDefaultCharacterChange (CharGUID);
}

void DaControl::FireAgentPropertyChange()
{
	CProxy_DaCtlEvents<DaControl>::FireAgentPropertyChange ();
	CProxy_AgentEvents<DaControl>::FireAgentPropertyChange ();
}

void DaControl::FireActiveClientChange(LPCTSTR CharacterID, BOOL Active)
{
	CProxy_DaCtlEvents<DaControl>::FireActiveClientChange (CharacterID, Active);
	CProxy_AgentEvents<DaControl>::FireActiveClientChange (CharacterID, Active);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

DaControl::CServerNotifySink::CServerNotifySink ()
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

DaControl::CServerNotifySink::~CServerNotifySink ()
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

HRESULT DaControl::CServerNotifySink::Initialize (DaControl * pOwner)
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

HRESULT DaControl::CServerNotifySink::Terminate ()
{
	HRESULT	lResult = S_FALSE;

	if	(this)
	{
		DaControl *	lOwner = mOwner;
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

HRESULT STDMETHODCALLTYPE DaControl::CServerNotifySink::Command (long CommandID, IDaSvrUserInput2 *UserInput)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[[%p(%d)] DaControl::CServerNotifySink::Command"), mOwner, mOwner->m_dwRef);
#endif
	CAtlString						lActiveCharacterID;
	DaCtlCharacter *				lActiveCharacter;
	CComObject <DaCtlUserInput> *	lUserInput = NULL;
	IDaCtlUserInputPtr				lInterface;

	if	(SUCCEEDED (CComObject <DaCtlUserInput>::CreateInstance (&lUserInput)))
	{
		try
		{
			DaCtlCommands *		lCommands;
			IDaCtlCommands2Ptr	lInterface;

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
		lUserInput->mServerObject = UserInput;
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

HRESULT STDMETHODCALLTYPE DaControl::CServerNotifySink::ActivateInputState (long CharacterID, long Activated)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] DaControl::CServerNotifySink::ActivateInputState [%d] [%d]"), mOwner, mOwner->m_dwRef, CharacterID, Activated);
#endif
	if	(Activated == ActiveState_InputActive)
	{
		if	(_AtlModule.PreNotify ())
		{
			try
			{
				mOwner->FireActivateInput (mOwner->GetControlCharacterID (CharacterID));
			}
			catch AnyExceptionDebug
			_AtlModule.PostNotify ();
		}
	}
	else
	if	(Activated == ActiveState_Inactive)
	{
		if	(_AtlModule.PreNotify ())
		{
			try
			{
				mOwner->FireDeactivateInput (mOwner->GetControlCharacterID (CharacterID));
			}
			catch AnyExceptionDebug
			_AtlModule.PostNotify ();
		}
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DaControl::CServerNotifySink::Restart (void)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] DaControl::CServerNotifySink::Restart"), mOwner, mOwner->m_dwRef);
#endif
	// Obsolete
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DaControl::CServerNotifySink::Shutdown (void)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] DaControl::CServerNotifySink::Shutdown"), mOwner, mOwner->m_dwRef);
#endif
	// Obsolete
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DaControl::CServerNotifySink::VisibleState (long CharacterID, long Visible, long Cause)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] DaControl::CServerNotifySink::VisibleState [%d] [%d] cause [%d]"), mOwner, mOwner->m_dwRef, CharacterID, Visible, Cause);
#endif
	if	(_AtlModule.PreNotify ())
	{
		try
		{
			if	(Visible)
			{
				mOwner->FireShow (mOwner->GetControlCharacterID (CharacterID), (VisibilityCauseType)Cause);
			}
			else
			{
				mOwner->FireHide (mOwner->GetControlCharacterID (CharacterID), (VisibilityCauseType)Cause);
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DaControl::CServerNotifySink::Click (long CharacterID, short Keys, long x, long y)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[[%p(%d)] DaControl::CServerNotifySink::Click [%d] [%4.4X] [%d %d]"), mOwner, mOwner->m_dwRef, CharacterID, Keys, x, y);
#endif
	if	(_AtlModule.PreNotify ())
	{
		try
		{
			mOwner->FireClick (mOwner->GetControlCharacterID (CharacterID), KeyButtonBits(Keys), KeyShiftBits(Keys), (short)x, (short)y);
		}
		catch AnyExceptionDebug
		_AtlModule.PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DaControl::CServerNotifySink::DblClick (long CharacterID, short Keys, long x, long y)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] DaControl::CServerNotifySink::DblClick [%d] [%4.4X] [%d %d]"), mOwner, mOwner->m_dwRef, CharacterID, Keys, x, y);
#endif
	if	(_AtlModule.PreNotify ())
	{
		try
		{
			mOwner->FireDblClick (mOwner->GetControlCharacterID (CharacterID), KeyButtonBits(Keys), KeyShiftBits(Keys), (short)x, (short)y);
		}
		catch AnyExceptionDebug
		_AtlModule.PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DaControl::CServerNotifySink::DragStart (long CharacterID, short Keys, long x, long y)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] DaControl::CServerNotifySink::DragStart [%d] [%4.4X] [%d %d]"), mOwner, mOwner->m_dwRef, CharacterID, Keys, x, y);
#endif
	if	(_AtlModule.PreNotify ())
	{
		try
		{
			mOwner->FireDragStart (mOwner->GetControlCharacterID (CharacterID), KeyButtonBits(Keys), KeyShiftBits(Keys), (short)x, (short)y);
		}
		catch AnyExceptionDebug
		_AtlModule.PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DaControl::CServerNotifySink::DragComplete (long CharacterID, short Keys, long x, long y)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] DaControl::CServerNotifySink::DragComplete [%d] [%4.4X] [%d %d]"), mOwner, mOwner->m_dwRef, CharacterID, Keys, x, y);
#endif
	if	(_AtlModule.PreNotify ())
	{
		try
		{
			mOwner->FireDragComplete (mOwner->GetControlCharacterID (CharacterID), KeyButtonBits(Keys), KeyShiftBits(Keys), (short)x, (short)y);
		}
		catch AnyExceptionDebug
		_AtlModule.PostNotify ();
	}
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaControl::CServerNotifySink::RequestStart (long RequestID)
{
#ifdef	_DEBUG_REQUEST_NOTIFY
	LogMessage (_DEBUG_REQUEST_NOTIFY, _T("[%p(%d)] DaControl::CServerNotifySink::RequestStart [%d]"), mOwner, mOwner->m_dwRef, RequestID);
#endif
	IDaCtlRequestPtr	lInterface;

	lInterface.Attach (mOwner->PutRequest (DaRequestNotifyStart, RequestID, S_OK));

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
		LogMessage (_DEBUG_REQUEST_NOTIFY, _T("[%p(%d)] DaControl::CServerNotifySink::RequestStart [%d] IGNORED"), mOwner, mOwner->m_dwRef, RequestID);
	}
#endif
	return S_OK;
}

STDMETHODIMP DaControl::CServerNotifySink::RequestComplete (long RequestID, long Result)
{
#ifdef	_DEBUG_REQUEST_NOTIFY
	LogMessage (_DEBUG_REQUEST_NOTIFY, _T("[%p(%d)] DaControl::CServerNotifySink::RequestComplete [%d] [%8.8X]"), mOwner, mOwner->m_dwRef, RequestID, Result);
#endif
	IDaCtlRequestPtr	lInterface;

	lInterface.Attach (mOwner->PutRequest (DaRequestNotifyComplete, RequestID, (HRESULT)Result));

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
		LogMessage (_DEBUG_REQUEST_NOTIFY, _T("[%p(%d)] DaControl::CServerNotifySink::RequestComplete [%d] [%8.8X] IGNORED"), mOwner, mOwner->m_dwRef, RequestID, Result);
	}
#endif
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaControl::CServerNotifySink::BookMark (long BookMarkID)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] DaControl::CServerNotifySink::BookMark [%d]"), mOwner, mOwner->m_dwRef, BookMarkID);
#endif
	if	(_AtlModule.PreNotify ())
	{
		try
		{
			mOwner->FireBookmark (BookMarkID);
		}
		catch AnyExceptionDebug
		_AtlModule.PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DaControl::CServerNotifySink::Idle (long CharacterID, long Start)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] DaControl::CServerNotifySink::Idle [%d] [%d]"), mOwner, mOwner->m_dwRef, CharacterID, Start);
#endif
	if	(_AtlModule.PreNotify ())
	{
		try
		{
			if	(Start)
			{
				mOwner->FireIdleStart (mOwner->GetControlCharacterID (CharacterID));
			}
			else
			{
				mOwner->FireIdleComplete (mOwner->GetControlCharacterID (CharacterID));
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DaControl::CServerNotifySink::Move (long CharacterID, long x, long y, long Cause)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] DaControl::CServerNotifySink::Move [%d] [%d %d] cause [%d]"), mOwner, mOwner->m_dwRef, CharacterID, x, y, Cause);
#endif
	if	(_AtlModule.PreNotify ())
	{
		try
		{
			mOwner->FireMove (mOwner->GetControlCharacterID (CharacterID), (short)x, (short)y, (MoveCauseType)Cause);
		}
		catch AnyExceptionDebug
		_AtlModule.PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DaControl::CServerNotifySink::Size (long CharacterID, long Width, long Height)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] DaControl::CServerNotifySink::Size [%d] [%d %d]"), mOwner, mOwner->m_dwRef, CharacterID, Width, Height);
#endif
	if	(_AtlModule.PreNotify ())
	{
		try
		{
			mOwner->FireSize (mOwner->GetControlCharacterID (CharacterID), (short)Width, (short)Height);
		}
		catch AnyExceptionDebug
		_AtlModule.PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DaControl::CServerNotifySink::BalloonVisibleState (long CharacterID, long Visible)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] DaControl::CServerNotifySink::BalloonVisibleState [%d] [%d]"), mOwner, mOwner->m_dwRef, CharacterID, Visible);
#endif
	if	(_AtlModule.PreNotify ())
	{
		try
		{
			if	(Visible)
			{
				mOwner->FireBalloonShow (mOwner->GetControlCharacterID (CharacterID));
			}
			else
			{
				mOwner->FireBalloonHide (mOwner->GetControlCharacterID (CharacterID));
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DaControl::CServerNotifySink::HelpComplete (long CharacterID, long CommandID, long Cause)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] DaControl::CServerNotifySink::HelpComplete"), mOwner, mOwner->m_dwRef);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DaControl::CServerNotifySink::ListeningState (long CharacterID, long Listening, long Cause)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] DaControl::CServerNotifySink::ListeningState"), mOwner, mOwner->m_dwRef);
#endif
	if	(_AtlModule.PreNotify ())
	{
		try
		{
			if	(Listening)
			{
				mOwner->FireListenStart (mOwner->GetControlCharacterID (CharacterID));
			}
			else
			{
				mOwner->FireListenComplete (mOwner->GetControlCharacterID (CharacterID), (ListenCompleteType)Cause);
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostNotify ();
	}
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaControl::CServerNotifySink::DefaultCharacterChange (BSTR CharGUID)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] DaControl::CServerNotifySink::DefaultCharacterChange [%ls]"), mOwner, mOwner->m_dwRef, CharGUID);
#endif
	if	(_AtlModule.PreNotify ())
	{
		try
		{
			mOwner->FireDefaultCharacterChange (CAtlString (CharGUID));
		}
		catch AnyExceptionDebug
		_AtlModule.PostNotify ();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DaControl::CServerNotifySink::AgentPropertyChange(void)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] DaControl::CServerNotifySink::AgentPropertyChange"), mOwner, mOwner->m_dwRef);
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

HRESULT STDMETHODCALLTYPE DaControl::CServerNotifySink::ActiveClientChange (long CharacterID, long Status)
{
#ifdef	_DEBUG_NOTIFY
	LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] DaControl::CServerNotifySink::ActiveClientChange [%d] [%8.8X]"), mOwner, mOwner->m_dwRef, CharacterID, Status);
#endif
	if	(Status == ActiveState_InputActive)
	{
		if	(_AtlModule.PreNotify ())
		{
			try
			{
				mOwner->FireActiveClientChange (mOwner->GetControlCharacterID (CharacterID), VARIANT_TRUE);
			}
			catch AnyExceptionDebug
			_AtlModule.PostNotify ();
		}
	}
	else
	if	(Status == ActiveState_Inactive)
	{
		if	(_AtlModule.PreNotify ())
		{
			try
			{
				mOwner->FireActiveClientChange (mOwner->GetControlCharacterID (CharacterID), VARIANT_FALSE);
			}
			catch AnyExceptionDebug
			_AtlModule.PostNotify ();
		}
	}
	return S_OK;
}
