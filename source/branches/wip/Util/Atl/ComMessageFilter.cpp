/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is a utility used by Double Agent but not specific to
	Double Agent.  However, it is included as part of the Double Agent
	source code under the following conditions:

    This is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this file.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include <oledlg.h>
#include "ComMessageFilter.h"
#ifdef	_DEBUG
#include "Registry.h"
#include "GuidStr.h"
#include "DebugWin.h"
#endif

#pragma comment(lib, "oledlg.lib")

#ifdef	_DEBUG
#define	_DEBUG_INCOMING		((GetProfileDebugInt(_T("DebugCom_Incoming"),LogVerbose,true)&0xFFFF)|LogTimeMs)
#define	_DEBUG_MESSAGE		((GetProfileDebugInt(_T("DebugCom_Message"),LogVerbose,true)&0xFFFF)|LogTimeMs|LogHighVolume)
#define	_DEBUG_BUSY_DLG		((GetProfileDebugInt(_T("DebugCom_Busy"),LogNormal,true)&0xFFFF)|LogTimeMs)
#endif

/////////////////////////////////////////////////////////////////////////////

CComMessageFilter::CComMessageFilter ()
:	mDndLevel (0),
	mCheckedOut (false),
	mRetryTimeout (0),
	mRetryLater (0),
	mRetryImmediate (0),
	mRetrySoon (200),
	mMessageTimeout (0),
	mRegistered (false),
	mInMessagePending (false)
{
	SetRetryTimeout (0);
	SetRetryLater (0);
	SetMessageTimeout (0);
}

CComMessageFilter::~CComMessageFilter ()
{
	Revoke ();
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CComMessageFilter::Register ()
{
	HRESULT	lResult = S_FALSE;

	if	(
			(!mRegistered)
		&&	((lResult = CoRegisterMessageFilter (this, &mPrevFilter)) == S_OK)
		)
	{
		mRegistered = true;
	}
	return lResult;
}

HRESULT CComMessageFilter::Revoke ()
{
	HRESULT	lResult = S_FALSE;

	if	(mRegistered)
	{
		InternalFinalConstructAddRef ();
		lResult = CoRegisterMessageFilter (mPrevFilter, NULL);
		InternalFinalConstructRelease ();
	}
	mRegistered = false;
	mPrevFilter = NULL;
	mDndLevel = 0;
	mCheckedOut = false;

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

UINT CComMessageFilter::DoNotDisturb (bool pDoNotDisturb)
{
#ifdef	_DEBUG
	if	(mCheckedOut)
	{
		LogMessage (LogIfActive|LogTime, _T("CComMessageFilter setting DoNotDisturb after CheckOut"));
	}
#endif
	if	(pDoNotDisturb)
	{
		mDndLevel++;
	}
	else
	if	(mDndLevel > 0)
	{
		mDndLevel--;
	}
#ifdef	DEBUG
	else
	{
		LogMessage (LogIfActive|LogTime, _T("CComMessageFilter mismatched DoNotDisturb"));
	}
#endif
	return mDndLevel;
}

void CComMessageFilter::CheckOut ()
{
	mCheckedOut = true;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

DWORD CComMessageFilter::GetRetryTimeout () const
{
	return mRetryTimeout;
}

DWORD CComMessageFilter::GetRetryLater () const
{
	return mRetryLater;
}

DWORD CComMessageFilter::GetMessageTimeout () const
{
	return mMessageTimeout;
}

/////////////////////////////////////////////////////////////////////////////

bool CComMessageFilter::SetRetryTimeout (DWORD pRetryTimeout)
{
	DWORD	lRetryTimeout = mRetryTimeout;

	if	(pRetryTimeout == 0)
	{
		pRetryTimeout = 10000;
	}
	if	((long) pRetryTimeout > 0)
	{
		mRetryTimeout = min (max (pRetryTimeout, 5000), 60000);
	}
	else
	{
		mRetryTimeout = (DWORD)-1;
	}

	return (mRetryTimeout != lRetryTimeout);
}

bool CComMessageFilter::SetRetryLater (DWORD pRetryLater)
{
	DWORD	lRetryLater = mRetryLater;

	if	(pRetryLater == 0)
	{
		pRetryLater = 10000;
	}
	mRetryLater = min (max (pRetryLater, 5000), 60000);

	return (mRetryLater != lRetryLater);
}

bool CComMessageFilter::SetMessageTimeout (DWORD pMessageTimeout)
{
	DWORD	lMessageTimeout = mMessageTimeout;

	if	(pMessageTimeout == 0)
	{
		pMessageTimeout = 8000;
	}
	mMessageTimeout = min (max (pMessageTimeout, 5000), 60000);

	return (mMessageTimeout != lMessageTimeout);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG
/////////////////////////////////////////////////////////////////////////////

static CAtlString CallTypeStr (DWORD pCallType)
{
	CAtlString	lCallTypeStr;

	switch (pCallType)
	{
		case CALLTYPE_TOPLEVEL:				lCallTypeStr = _T("TOPLEVEL"); break;
		case CALLTYPE_NESTED:				lCallTypeStr = _T("NESTED"); break;
		case CALLTYPE_ASYNC:				lCallTypeStr = _T("ASYNC"); break;
		case CALLTYPE_TOPLEVEL_CALLPENDING:	lCallTypeStr = _T("TOPLEVEL_CALLPENDING"); break;
		case CALLTYPE_ASYNC_CALLPENDING:	lCallTypeStr = _T("ASYNC_CALLPENDING"); break;
		default:							lCallTypeStr.Format (_T("%u"), pCallType);
	}
	return lCallTypeStr;
}

static CAtlString ServerCallStr (DWORD pServerCall)
{
	CAtlString	lServerCallStr;

	switch (pServerCall)
	{
		case SERVERCALL_ISHANDLED:	lServerCallStr = _T("ISHANDLED"); break;
		case SERVERCALL_REJECTED:	lServerCallStr = _T("REJECTED"); break;
		case SERVERCALL_RETRYLATER:	lServerCallStr = _T("RETRYLATER"); break;
		default:					lServerCallStr.Format (_T("%u"), pServerCall);
	}
	return lServerCallStr;
}

static CAtlString PendingTypeStr (DWORD pPendingType)
{
	CAtlString	lPendingTypeStr;

	switch (pPendingType)
	{
		case PENDINGTYPE_TOPLEVEL:	lPendingTypeStr = _T("TOPLEVEL"); break;
		case PENDINGTYPE_NESTED:	lPendingTypeStr = _T("NESTED"); break;
		default:					lPendingTypeStr.Format (_T("%u"), pPendingType);
	}
	return lPendingTypeStr;
}

static CAtlString PendingMsgStr (DWORD pPendingMsg)
{
	CAtlString	lPendingMsgStr;

	switch (pPendingMsg)
	{
		case PENDINGMSG_CANCELCALL:		lPendingMsgStr = _T("CANCELCALL"); break;
		case PENDINGMSG_WAITNOPROCESS:	lPendingMsgStr = _T("WAITNOPROCESS"); break;
		case PENDINGMSG_WAITDEFPROCESS:	lPendingMsgStr = _T("WAITDEFPROCESS"); break;
		default:						lPendingMsgStr.Format (_T("%u"), pPendingMsg);
	}
	return lPendingMsgStr;
}

static CAtlString InterfaceInfoStr (const INTERFACEINFO * pInterfaceInfo)
{
	CAtlString	lInterfaceInfoStr;

	if	(pInterfaceInfo)
	{
		lInterfaceInfoStr.Format (_T("%s (%hu)"), CGuidStr::GuidName (pInterfaceInfo->iid), pInterfaceInfo->wMethod);
	}
	else
	{
		lInterfaceInfoStr = _T("<null>");
	}
	return lInterfaceInfoStr;
}

static CAtlString OleUiResultStr (UINT pResult)
{
	CAtlString	lOleUiResultStr;

	switch (pResult)
	{
		case OLEUI_FALSE:					lOleUiResultStr = _T("FALSE"); break;
		case OLEUI_OK:						lOleUiResultStr = _T("OK"); break;
		case OLEUI_CANCEL:					lOleUiResultStr = _T("CANCEL"); break;
		case OLEUI_BZ_SWITCHTOSELECTED:		lOleUiResultStr = _T("BZ_SWITCHTOSELECTED"); break;
		case OLEUI_BZ_RETRYSELECTED:		lOleUiResultStr = _T("BZ_RETRYSELECTED"); break;
		case OLEUI_BZ_CALLUNBLOCKED:		lOleUiResultStr = _T("BZ_CALLUNBLOCKED"); break;
		case OLEUI_BZERR_HTASKINVALID:		lOleUiResultStr = _T("BZERR_HTASKINVALID"); break;
		case OLEUI_ERR_STRUCTUREINVALID:	lOleUiResultStr = _T("ERR_STRUCTUREINVALID"); break;
		case OLEUI_ERR_CBSTRUCTINCORRECT:	lOleUiResultStr = _T("ERR_CBSTRUCTINCORRECT"); break;
		case OLEUI_ERR_HWNDOWNERINVALID:	lOleUiResultStr = _T("ERR_HWNDOWNERINVALID"); break;
		case OLEUI_ERR_LPSZCAPTIONINVALID:	lOleUiResultStr = _T("ERR_LPSZCAPTIONINVALID"); break;
		default:							lOleUiResultStr.Format (_T("%u"), pResult);
	}
	return lOleUiResultStr;
}

/////////////////////////////////////////////////////////////////////////////
#endif	// _DEBUG
/////////////////////////////////////////////////////////////////////////////

UINT CComMessageFilter::ShowBusyDlg (HTASK pBlockedTask, bool pNotResponding)
{
	UINT					lRet = OLEUI_FALSE;
	tSS <OLEUIBUSY, DWORD>	lUiBusy;
	HWND					lUiBusyWnd = NULL;

	lUiBusy.hTask = pBlockedTask;
	lUiBusy.lphWndDialog = &lUiBusyWnd;

	if	(pNotResponding)
	{
		lUiBusy.dwFlags |= BZ_NOTRESPONDINGDIALOG;
	}

	lUiBusy.hWndOwner = ::GetActiveWindow ();
	if	(
			(!lUiBusy.hWndOwner)
		||	(!::IsWindowVisible (lUiBusy.hWndOwner))
		)
	{
		lUiBusy.hWndOwner = ::GetForegroundWindow ();
	}
	if	(
			(!lUiBusy.hWndOwner)
		||	(!::IsWindowVisible (lUiBusy.hWndOwner))
		)
	{
		lUiBusy.hWndOwner = ::GetDesktopWindow ();
	}

#ifdef	_DEBUG_BUSY_DLG
	if	(LogIsActive (_DEBUG_BUSY_DLG))
	{
		if	(pNotResponding)
		{
			LogMessage (_DEBUG_BUSY_DLG, _T("CComMessageFilter show NotResponding dialog (Timeout %u)"), mMessageTimeout);
		}
		else
		{
			LogMessage (_DEBUG_BUSY_DLG, _T("CComMessageFilter show Busy dialog (Timeout %d %d %d)"), mRetryTimeout, mRetrySoon, mRetryLater);
		}
		LogWindow (_DEBUG_BUSY_DLG, lUiBusy.hWndOwner, _T("  Owner"));
	}
#endif

	lRet = ::OleUIBusy (&lUiBusy);

#ifdef	_DEBUG_BUSY_DLG
	if	(LogIsActive (_DEBUG_BUSY_DLG))
	{
		LogMessage (_DEBUG_BUSY_DLG, _T("CComMessageFilter result [%s]"), OleUiResultStr (lRet));
	}
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

DWORD STDMETHODCALLTYPE CComMessageFilter::HandleInComingCall (DWORD dwCallType, HTASK htaskCaller, DWORD dwTickCount, LPINTERFACEINFO lpInterfaceInfo)
{
	DWORD	lRet = SERVERCALL_ISHANDLED;

	if	(
			(dwCallType == CALLTYPE_TOPLEVEL)
		||	(dwCallType == CALLTYPE_TOPLEVEL_CALLPENDING)
		)
	{
		if	(mCheckedOut)
		{
			lRet = SERVERCALL_REJECTED;
		}
		else
		if	(mDndLevel > 0)
		{
			lRet = SERVERCALL_RETRYLATER;
		}
	}
//
//	Other call types must be handled
//
#ifdef	_DEBUG_INCOMING
	if	(LogIsActive (_DEBUG_INCOMING))
	{
		LogMessage (_DEBUG_INCOMING, _T("CComMessageFilter [%s] Incoming [%s] Ticks [%u] DoNotDisturb [%u] CheckedOut [%u] For [%s]"), ServerCallStr(lRet), CallTypeStr(dwCallType), dwTickCount, mDndLevel, mCheckedOut, InterfaceInfoStr(lpInterfaceInfo));
	}
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

DWORD STDMETHODCALLTYPE CComMessageFilter::RetryRejectedCall (HTASK htaskCallee, DWORD dwTickCount, DWORD dwRejectType)
{
	DWORD	lRetryTime = mRetrySoon;

#ifdef	_DEBUG_INCOMING
	if	(LogIsActive (_DEBUG_INCOMING))
	{
		LogMessage (_DEBUG_INCOMING, _T("CComMessageFilter RetryRejected [%s] Ticks [%u] Timeout [%d %d %d] DoNotDisturb [%u] CheckedOut [%u]"), ServerCallStr(dwRejectType), dwTickCount, mRetryTimeout, mRetrySoon, mRetryLater, mDndLevel, mCheckedOut);
	}
#endif

	if	(dwRejectType == SERVERCALL_REJECTED)
	{
		lRetryTime = (DWORD)-1;
	}
	else
	if	(
			((long) mRetryTimeout > 0)
		&&	(dwTickCount > mRetryTimeout)
		)
	{
		if	(mDndLevel > 0)
		{
			lRetryTime = (DWORD)-1;
		}
		else
		{
			switch (ShowBusyDlg (htaskCallee, false))
			{
				case OLEUI_CANCEL:				lRetryTime = (DWORD)-1; break;
				case OLEUI_BZ_RETRYSELECTED:	lRetryTime = mRetryImmediate; break;
				case OLEUI_BZ_CALLUNBLOCKED:	lRetryTime = mRetryImmediate; break;
				default:						lRetryTime = mRetryLater;
			}
		}
	}

#ifdef	_DEBUG_INCOMING
	if	(LogIsActive (_DEBUG_INCOMING))
	{
		LogMessage (_DEBUG_INCOMING, _T("CComMessageFilter RetryRejected [%s] Ticks [%u] Timeout [%d %d %d] DoNotDisturb [%u] CheckedOut [%u] Retry [%d]"), ServerCallStr(dwRejectType), dwTickCount, mRetryTimeout, mRetrySoon, mRetryLater, mDndLevel, mCheckedOut, lRetryTime);
	}
#endif
	return lRetryTime;
}

/////////////////////////////////////////////////////////////////////////////

DWORD STDMETHODCALLTYPE CComMessageFilter::MessagePending (HTASK htaskCallee, DWORD dwTickCount, DWORD dwPendingType)
{
	DWORD	lRet = PENDINGMSG_WAITNOPROCESS;

#ifdef	_DEBUG_MESSAGE_NOT
	if	(LogIsActive (_DEBUG_MESSAGE))
	{
		LogMessage (_DEBUG_MESSAGE, _T("CComMessageFilter MessagePending [%s] Ticks [%u] Timeout [%u] DoNotDisturb [%u] CheckedOut [%u] Reentrant [%u]"), PendingTypeStr(dwPendingType), dwTickCount, mMessageTimeout, mDndLevel, mCheckedOut, mInMessagePending);
	}
#endif

	if	(mInMessagePending)
	{
		lRet = PENDINGMSG_WAITDEFPROCESS;
	}
	else
	if	(mCheckedOut)
	{
		lRet = PENDINGMSG_WAITDEFPROCESS;
	}
	else
	if	(
			(dwTickCount > mMessageTimeout)
		&&	(mDndLevel == 0)
		&&	(GetQueueStatus (QS_INPUT|QS_TIMER|QS_INPUT) != 0)
		)
	{
		mInMessagePending = true;

		try
		{
			MSG	lMsg;

#ifdef	_DEBUG_MESSAGE
			if	(LogIsActive (_DEBUG_MESSAGE))
			{
				LogMessage (_DEBUG_MESSAGE, _T("CComMessageFilter MessagePending [%s] Ticks [%u] Timeout [%u] DoNotDisturb [%u] CheckedOut [%u]"), PendingTypeStr(dwPendingType), dwTickCount, mMessageTimeout, mDndLevel, mCheckedOut);
			}
#endif
			while (PeekMessage (&lMsg, NULL, WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE|PM_NOYIELD))
			{;}
			while (PeekMessage (&lMsg, NULL, WM_KEYFIRST, WM_KEYFIRST, PM_REMOVE|PM_NOYIELD))
			{;}

			ShowBusyDlg (htaskCallee, true);
		}
		catch AnyExceptionDebug

		mInMessagePending = false;
	}

#ifdef	_DEBUG_MESSAGE
	if	(LogIsActive (_DEBUG_MESSAGE))
	{
		LogMessage (_DEBUG_MESSAGE, _T("CComMessageFilter MessagePending [%s] Ticks [%u] Timeout [%u] DoNotDisturb [%u] CheckedOut [%u] Result [%s]"), PendingTypeStr(dwPendingType), dwTickCount, mMessageTimeout, mDndLevel, mCheckedOut, PendingMsgStr (lRet));
	}
#endif
	return lRet;
}
