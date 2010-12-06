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
#include <AgtSvr.h>
#include "DaServerApp.h"
#include "DaServer.h"
#include "ServerNotify.h"
#include "ComMessageFilter.h"
#include "GuidStr.h"
#include "Registry.h"
#include "DebugStr.h"

#pragma comment(lib, "psapi.lib")

extern int LogCrashCode (unsigned int pCode, LPCSTR pFile = NULL, UINT pLine = 0, int pAction = EXCEPTION_CONTINUE_EXECUTION);

#ifdef	_DEBUG
#define	_DEBUG_NOTIFY		(GetProfileDebugInt(_T("DebugNotify"),LogVerbose,true)&0xFFFF|LogTime)
#define	_DEBUG_CONNECTIONS	(GetProfileDebugInt(_T("DebugNotifyConnections"),LogVerbose,true)&0xFFFF|LogTime|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Other"),LogVerbose,true)&0xFFFF|LogTime)
#endif

/////////////////////////////////////////////////////////////////////////////

_COM_SMARTPTR_TYPEDEF(IAgentNotifySink, __uuidof(IAgentNotifySink));
_COM_SMARTPTR_TYPEDEF(IAgentNotifySinkEx, __uuidof(IAgentNotifySinkEx));

/////////////////////////////////////////////////////////////////////////////

CServerNotify::CServerNotify ()
:	mOwner (NULL),
	CProxyIDaSvrNotifySink<CServerNotify> (mUnregisterDelayed),
	CProxyIDaSvrNotifySink2<CServerNotify> (mUnregisterDelayed),
	CProxyIAgentNotifySink<CServerNotify> (mUnregisterDelayed),
	CProxyIAgentNotifySinkEx<CServerNotify> (mUnregisterDelayed),
	CProxy_DaSvrEvents2<CServerNotify> (mUnregisterDelayed)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CServerNotify::CServerNotify (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
}

CServerNotify::~CServerNotify ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CServerNotify::~CServerNotify (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
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

HRESULT CServerNotify::Register (IUnknown * punkNotifySink, long * pdwSinkID)
{
	HRESULT					lResult = S_FALSE;
	IDaSvrNotifySink2Ptr	lDaSink2 (punkNotifySink);
	IDaSvrNotifySinkPtr		lDaSink (punkNotifySink);
	IAgentNotifySinkPtr		lMsSink (punkNotifySink);
	IAgentNotifySinkExPtr	lMsSinkEx (punkNotifySink);

	if	(lDaSink2 != NULL)
	{
		lResult = CProxyIDaSvrNotifySink2<CServerNotify>::Advise (punkNotifySink, (DWORD*)pdwSinkID);
#ifdef	_DEBUG_CONNECTIONS
		if	(SUCCEEDED (lResult))
		{
			LogMessage (_DEBUG_CONNECTIONS, _T("[%p] Register2 (DA) [%p] [%u]"), this, punkNotifySink, *pdwSinkID);
		}
#endif
	}
	else
	if	(lDaSink != NULL)
	{
		lResult = CProxyIDaSvrNotifySink<CServerNotify>::Advise (punkNotifySink, (DWORD*)pdwSinkID);
#ifdef	_DEBUG_CONNECTIONS
		if	(SUCCEEDED (lResult))
		{
			LogMessage (_DEBUG_CONNECTIONS, _T("[%p] Register (DA) [%p] [%u]"), this, punkNotifySink, *pdwSinkID);
		}
#endif
	}
	else
	if	(lMsSinkEx != NULL)
	{
		lResult = CProxyIAgentNotifySinkEx<CServerNotify>::Advise (punkNotifySink, (DWORD*)pdwSinkID);
#ifdef	_DEBUG_CONNECTIONS
		if	(SUCCEEDED (lResult))
		{
			LogMessage (_DEBUG_CONNECTIONS, _T("[%p] RegisterEx (MS) [%p] [%u]"), this, punkNotifySink, *pdwSinkID);
		}
#endif
	}
	else
	if	(lMsSink != NULL)
	{
		lResult = CProxyIAgentNotifySink<CServerNotify>::Advise (punkNotifySink, (DWORD*)pdwSinkID);
#ifdef	_DEBUG_CONNECTIONS
		if	(SUCCEEDED (lResult))
		{
			LogMessage (_DEBUG_CONNECTIONS, _T("[%p] Register (MS)   [%p] [%u]"), this, punkNotifySink, *pdwSinkID);
		}
#endif
	}
	else
	{
		lResult = CONNECT_E_CANNOTCONNECT;
	}
	return lResult;
}

HRESULT CServerNotify::Unregister (long dwSinkID, bool pDelay)
{
	HRESULT	lResult = S_FALSE;

	if	(pDelay)
	{
		lResult = (CProxyIDaSvrNotifySink2<CServerNotify>::m_vec.GetUnknown ((DWORD)dwSinkID)) ? S_OK : CONNECT_E_NOCONNECTION;
	}
	else
	{
		lResult = CProxyIDaSvrNotifySink2<CServerNotify>::Unadvise ((DWORD)dwSinkID);
	}
#ifdef	_DEBUG_CONNECTIONS
	if	(SUCCEEDED (lResult))
	{
		LogMessage (_DEBUG_CONNECTIONS, _T("[%p] Unregister2 (DA) [%u]"), this, dwSinkID);
	}
#endif

	if	(lResult == CONNECT_E_NOCONNECTION)
	{
		if	(pDelay)
		{
			lResult = (CProxyIDaSvrNotifySink<CServerNotify>::m_vec.GetUnknown ((DWORD)dwSinkID)) ? S_OK : CONNECT_E_NOCONNECTION;
		}
		else
		{
			lResult = CProxyIDaSvrNotifySink<CServerNotify>::Unadvise ((DWORD)dwSinkID);
		}
#ifdef	_DEBUG_CONNECTIONS
		if	(SUCCEEDED (lResult))
		{
			LogMessage (_DEBUG_CONNECTIONS, _T("[%p] Unregister (DA) [%u]"), this, dwSinkID);
		}
#endif
	}

	if	(lResult == CONNECT_E_NOCONNECTION)
	{
		if	(pDelay)
		{
			lResult = (CProxyIAgentNotifySinkEx<CServerNotify>::m_vec.GetUnknown ((DWORD)dwSinkID)) ? S_OK : CONNECT_E_NOCONNECTION;
		}
		else
		{
			lResult = CProxyIAgentNotifySinkEx<CServerNotify>::Unadvise ((DWORD)dwSinkID);
		}
#ifdef	_DEBUG_CONNECTIONS
		if	(SUCCEEDED (lResult))
		{
			LogMessage (_DEBUG_CONNECTIONS, _T("[%p] UnregisterEx (MS) [%u]"), this, dwSinkID);
		}
#endif
	}

	if	(lResult == CONNECT_E_NOCONNECTION)
	{
		if	(pDelay)
		{
			lResult = (CProxyIAgentNotifySink<CServerNotify>::m_vec.GetUnknown ((DWORD)dwSinkID)) ? S_OK : CONNECT_E_NOCONNECTION;
		}
		else
		{
			lResult = CProxyIAgentNotifySink<CServerNotify>::Unadvise ((DWORD)dwSinkID);
		}
#ifdef	_DEBUG_CONNECTIONS
		if	(SUCCEEDED (lResult))
		{
			LogMessage (_DEBUG_CONNECTIONS, _T("[%p] Unregister (MS) [%u]"), this, dwSinkID);
		}
#endif
	}

	if	(
			(pDelay)
		&&	(SUCCEEDED (lResult))
		)
	{
#ifdef	_DEBUG_CONNECTIONS
		if	(SUCCEEDED (lResult))
		{
			LogMessage (_DEBUG_CONNECTIONS, _T("[%p] UnregisterDelay [%u]"), this, dwSinkID);
		}
#endif
		mUnregisterDelayed.AddUnique (dwSinkID);
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

void CServerNotify::UnregisterDelayed ()
{
	int	lNdx;

	for	(lNdx = (int)mUnregisterDelayed.GetCount()-1; lNdx >= 0; lNdx--)
	{
#ifdef	_DEBUG_CONNECTIONS
		LogMessage (_DEBUG_CONNECTIONS, _T("[%p] UnregisterDelayed [%d]"), this, mUnregisterDelayed [lNdx]);
#endif
		Unregister (mUnregisterDelayed [lNdx], false);
	}
	mUnregisterDelayed.RemoveAll ();
}

void CServerNotify::UnregisterAll ()
{
#ifdef	_DEBUG_CONNECTIONS
	LogMessage (_DEBUG_CONNECTIONS, _T("[%p] UnregisterAll [%d] [%d] [%d] [%d] [%d]"), this, CountUnk(CProxyIDaSvrNotifySink2<CServerNotify>::m_vec), CountUnk(CProxyIDaSvrNotifySink<CServerNotify>::m_vec), CountUnk(CProxyIAgentNotifySinkEx<CServerNotify>::m_vec), CountUnk(CProxyIAgentNotifySink<CServerNotify>::m_vec), CountUnk(CProxy_DaSvrEvents2<CServerNotify>::m_vec));
#endif
	int	lNdx;

	UnregisterDelayed ();

	for	(lNdx = CProxyIDaSvrNotifySink2<CServerNotify>::m_vec.GetSize()-1; lNdx >= 0; lNdx--)
	{
		try
		{
			if	(CProxyIDaSvrNotifySink2<CServerNotify>::m_vec.GetAt (lNdx))
			{
				CProxyIDaSvrNotifySink2<CServerNotify>::Unadvise ((DWORD)lNdx);
#ifdef	_DEBUG_CONNECTIONS
				LogMessage (_DEBUG_CONNECTIONS, _T("[%p] Unregister2 (DA) [%u] (All)"), this, lNdx);
#endif
			}
		}
		catch AnyExceptionSilent
	}

	for	(lNdx = CProxyIDaSvrNotifySink<CServerNotify>::m_vec.GetSize()-1; lNdx >= 0; lNdx--)
	{
		try
		{
			if	(CProxyIDaSvrNotifySink<CServerNotify>::m_vec.GetAt (lNdx))
			{
				CProxyIDaSvrNotifySink<CServerNotify>::Unadvise ((DWORD)lNdx);
#ifdef	_DEBUG_CONNECTIONS
				LogMessage (_DEBUG_CONNECTIONS, _T("[%p] Unregister (DA) [%u] (All)"), this, lNdx);
#endif
			}
		}
		catch AnyExceptionSilent
	}

	for	(lNdx = CProxyIAgentNotifySinkEx<CServerNotify>::m_vec.GetSize()-1; lNdx >= 0; lNdx--)
	{
		try
		{
			if	(CProxyIAgentNotifySinkEx<CServerNotify>::m_vec.GetAt (lNdx))
			{
				CProxyIAgentNotifySinkEx<CServerNotify>::Unadvise ((DWORD)lNdx);
#ifdef	_DEBUG_CONNECTIONS
				LogMessage (_DEBUG_CONNECTIONS, _T("[%p] UnregisterEx (MS) [%u] (All)"), this, lNdx);
#endif
			}
		}
		catch AnyExceptionSilent
	}

	for	(lNdx = CProxyIAgentNotifySink<CServerNotify>::m_vec.GetSize()-1; lNdx >= 0; lNdx--)
	{
		try
		{
			if	(CProxyIAgentNotifySink<CServerNotify>::m_vec.GetAt (lNdx))
			{
				CProxyIAgentNotifySink<CServerNotify>::Unadvise ((DWORD)lNdx);
#ifdef	_DEBUG_CONNECTIONS
				LogMessage (_DEBUG_CONNECTIONS, _T("[%p] Unregister (MS)   [%u] (All)"), this, lNdx);
#endif
			}
		}
		catch AnyExceptionSilent
	}

	for	(lNdx = CProxy_DaSvrEvents2<CServerNotify>::m_vec.GetSize()-1; lNdx >= 0; lNdx--)
	{
		try
		{
			if	(CProxy_DaSvrEvents2<CServerNotify>::m_vec.GetAt (lNdx))
			{
				CProxy_DaSvrEvents2<CServerNotify>::Unadvise ((DWORD)lNdx);
#ifdef	_DEBUG_CONNECTIONS
				LogMessage (_DEBUG_CONNECTIONS, _T("[%p] Unregister (Dispatch) [%u] (All)"), this, lNdx);
#endif
			}
		}
		catch AnyExceptionSilent
	}
}

/////////////////////////////////////////////////////////////////////////////

void CServerNotify::AbandonAll ()
{
#ifdef	_DEBUG_CONNECTIONS
	LogMessage (_DEBUG_CONNECTIONS, _T("[%p] AbandonAll [%d] [%d] {%d] [%d] [%d]"), this, CountUnk(CProxyIDaSvrNotifySink2<CServerNotify>::m_vec), CountUnk(CProxyIDaSvrNotifySink<CServerNotify>::m_vec), CountUnk(CProxyIAgentNotifySinkEx<CServerNotify>::m_vec), CountUnk(CProxyIAgentNotifySink<CServerNotify>::m_vec), CountUnk(CProxy_DaSvrEvents2<CServerNotify>::m_vec));
#endif
	CProxyIDaSvrNotifySink2<CServerNotify>::m_vec.clear ();
	CProxyIDaSvrNotifySink<CServerNotify>::m_vec.clear ();
	CProxyIAgentNotifySinkEx<CServerNotify>::m_vec.clear ();
	CProxyIAgentNotifySink<CServerNotify>::m_vec.clear ();
	CProxy_DaSvrEvents2<CServerNotify>::m_vec.clear ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CServerNotify::PreFireEvent (LPCTSTR pEventName)
{
#ifdef	_DEBUG_NOTIFY
	if	(LogIsActive ())
	{
		LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] Fire %s (LockCount %d)"), mOwner, max(mOwner->m_dwRef,-1), pEventName, _AtlModule.GetLockCount());
	}
#endif
	if	(mOwner->PreNotify ())
	{
		CEventNotify::PreFireEvent (pEventName);
#if	FALSE // We're allowing incoming calls during outgoing events.  Clients expect this.
		CComMessageFilter *	lMessageFilter;
		if	(lMessageFilter = _AtlModule.GetMessageFilter())
		{
			try
			{
				lMessageFilter->DoNotDisturb (true);
			}
			catch AnyExceptionSilent
		}
#endif
		return true;
	}
	return false;
}

bool CServerNotify::PostFireEvent (LPCTSTR pEventName, UINT pEventSinkCount)
{
#ifdef	_DEBUG_NOTIFY
	if	(LogIsActive ())
	{
		LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] Fire %s done [%u] (LockCount %d)"), mOwner, max(mOwner->m_dwRef,-1), pEventName, pEventSinkCount, _AtlModule.GetLockCount());
	}
#endif
#if	FALSE // See above
	CComMessageFilter *	lMessageFilter;
	if	(lMessageFilter = _AtlModule.GetMessageFilter())
	{
		try
		{
			lMessageFilter->DoNotDisturb (false);
		}
		catch AnyExceptionSilent
	}
#endif
	CEventNotify::PostFireEvent (pEventName, pEventSinkCount);
	return mOwner->PostNotify ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CServerNotify::FireCommand(long CommandID, IDaSvrUserInput2* UserInput)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("Command")))
	{
		__try
		{
			lEventSinkCount = CProxyIDaSvrNotifySink<CServerNotify>::FireCommand (CommandID, UserInput)
							+ CProxyIDaSvrNotifySink2<CServerNotify>::FireCommand (CommandID, UserInput)
							+ CProxyIAgentNotifySink<CServerNotify>::FireCommand (CommandID, UserInput)
							+ CProxyIAgentNotifySinkEx<CServerNotify>::FireCommand (CommandID, UserInput)
							+ CProxy_DaSvrEvents2<CServerNotify>::FireCommand (CommandID, UserInput);
		}
		__except (LogCrashCode (GetExceptionCode(), __FILE__, __LINE__))
		{}
		PostFireEvent (_DEBUG_T("Command"), lEventSinkCount);
	}
	return S_OK;
}

HRESULT CServerNotify::FireActivateInputState(long CharacterID, long Activated)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("ActivateInputState")))
	{
		__try
		{
			lEventSinkCount = CProxyIDaSvrNotifySink<CServerNotify>::FireActivateInputState (CharacterID, Activated)
							+ CProxyIDaSvrNotifySink2<CServerNotify>::FireActivateInputState (CharacterID, Activated)
							+ CProxyIAgentNotifySink<CServerNotify>::FireActivateInputState (CharacterID, Activated)
							+ CProxyIAgentNotifySinkEx<CServerNotify>::FireActivateInputState (CharacterID, Activated)
							+ CProxy_DaSvrEvents2<CServerNotify>::FireActivateInputState (CharacterID, Activated);
		}
		__except (LogCrashCode (GetExceptionCode(), __FILE__, __LINE__))
		{}
		PostFireEvent (_DEBUG_T("ActivateInputState"), lEventSinkCount);
	}
	return S_OK;
}

HRESULT CServerNotify::FireVisibleState(long CharacterID, long Visible, long Cause)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("VisibleState")))
	{
		__try
		{
			lEventSinkCount = CProxyIDaSvrNotifySink<CServerNotify>::FireVisibleState (CharacterID, Visible, Cause)
							+ CProxyIDaSvrNotifySink2<CServerNotify>::FireVisibleState (CharacterID, Visible, Cause)
							+ CProxyIAgentNotifySink<CServerNotify>::FireVisibleState (CharacterID, Visible, Cause)
							+ CProxyIAgentNotifySinkEx<CServerNotify>::FireVisibleState (CharacterID, Visible, Cause)
							+ CProxy_DaSvrEvents2<CServerNotify>::FireVisibleState (CharacterID, Visible, Cause);
		}
		__except (LogCrashCode (GetExceptionCode(), __FILE__, __LINE__))
		{}
		PostFireEvent (_DEBUG_T("VisibleState"), lEventSinkCount);
	}
	return S_OK;
}

HRESULT CServerNotify::FireClick(long CharacterID, short Keys, long X, long Y)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("Click")))
	{
		__try
		{
			lEventSinkCount = CProxyIDaSvrNotifySink<CServerNotify>::FireClick (CharacterID, Keys, X, Y)
							+ CProxyIDaSvrNotifySink2<CServerNotify>::FireClick (CharacterID, Keys, X, Y)
							+ CProxyIAgentNotifySink<CServerNotify>::FireClick (CharacterID, Keys, X, Y)
							+ CProxyIAgentNotifySinkEx<CServerNotify>::FireClick (CharacterID, Keys, X, Y)
							+ CProxy_DaSvrEvents2<CServerNotify>::FireClick (CharacterID, Keys, X, Y);
		}
		__except (LogCrashCode (GetExceptionCode(), __FILE__, __LINE__))
		{}
		PostFireEvent (_DEBUG_T("Click"), lEventSinkCount);
	}
	return S_OK;
}

HRESULT CServerNotify::FireDblClick(long CharacterID, short Keys, long X, long Y)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("DblClick")))
	{
		__try
		{
			lEventSinkCount = CProxyIDaSvrNotifySink<CServerNotify>::FireDblClick (CharacterID, Keys, X, Y)
							+ CProxyIDaSvrNotifySink2<CServerNotify>::FireDblClick (CharacterID, Keys, X, Y)
							+ CProxyIAgentNotifySink<CServerNotify>::FireDblClick (CharacterID, Keys, X, Y)
							+ CProxyIAgentNotifySinkEx<CServerNotify>::FireDblClick (CharacterID, Keys, X, Y)
							+ CProxy_DaSvrEvents2<CServerNotify>::FireDblClick (CharacterID, Keys, X, Y);
		}
		__except (LogCrashCode (GetExceptionCode(), __FILE__, __LINE__))
		{}
		PostFireEvent (_DEBUG_T("DblClick"), lEventSinkCount);
	}
	return S_OK;
}

HRESULT CServerNotify::FireDragStart(long CharacterID, short Keys, long X, long Y)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("DragStart")))
	{
		__try
		{
			lEventSinkCount = CProxyIDaSvrNotifySink<CServerNotify>::FireDragStart (CharacterID, Keys, X, Y)
							+ CProxyIDaSvrNotifySink2<CServerNotify>::FireDragStart (CharacterID, Keys, X, Y)
							+ CProxyIAgentNotifySink<CServerNotify>::FireDragStart (CharacterID, Keys, X, Y)
							+ CProxyIAgentNotifySinkEx<CServerNotify>::FireDragStart (CharacterID, Keys, X, Y)
							+ CProxy_DaSvrEvents2<CServerNotify>::FireDragStart (CharacterID, Keys, X, Y);
		}
		__except (LogCrashCode (GetExceptionCode(), __FILE__, __LINE__))
		{}
		PostFireEvent (_DEBUG_T("DragStart"), lEventSinkCount);
	}
	return S_OK;
}

HRESULT CServerNotify::FireDragComplete(long CharacterID, short Keys, long X, long Y)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("DragComplete")))
	{
		__try
		{
			lEventSinkCount = CProxyIDaSvrNotifySink<CServerNotify>::FireDragComplete (CharacterID, Keys, X, Y)
							+ CProxyIDaSvrNotifySink2<CServerNotify>::FireDragComplete (CharacterID, Keys, X, Y)
							+ CProxyIAgentNotifySink<CServerNotify>::FireDragComplete (CharacterID, Keys, X, Y)
							+ CProxyIAgentNotifySinkEx<CServerNotify>::FireDragComplete (CharacterID, Keys, X, Y)
							+ CProxy_DaSvrEvents2<CServerNotify>::FireDragComplete (CharacterID, Keys, X, Y);
		}
		__except (LogCrashCode (GetExceptionCode(), __FILE__, __LINE__))
		{}
		PostFireEvent (_DEBUG_T("DragComplete"), lEventSinkCount);
	}
	return S_OK;
}

HRESULT CServerNotify::FireRequestStart(long RequestID)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("RequestStart")))
	{
		__try
		{
			lEventSinkCount = CProxyIDaSvrNotifySink<CServerNotify>::FireRequestStart (RequestID)
							+ CProxyIDaSvrNotifySink2<CServerNotify>::FireRequestStart (RequestID)
							+ CProxyIAgentNotifySink<CServerNotify>::FireRequestStart (RequestID)
							+ CProxyIAgentNotifySinkEx<CServerNotify>::FireRequestStart (RequestID)
							+ CProxy_DaSvrEvents2<CServerNotify>::FireRequestStart (RequestID);
		}
		__except (LogCrashCode (GetExceptionCode(), __FILE__, __LINE__))
		{}
		PostFireEvent (_DEBUG_T("RequestStart"), lEventSinkCount);
	}
	return S_OK;
}

HRESULT CServerNotify::FireRequestComplete(long RequestID, long Result)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("RequestComplete")))
	{
		__try
		{
			lEventSinkCount = CProxyIDaSvrNotifySink<CServerNotify>::FireRequestComplete (RequestID, Result)
							+ CProxyIDaSvrNotifySink2<CServerNotify>::FireRequestComplete (RequestID, Result)
							+ CProxyIAgentNotifySink<CServerNotify>::FireRequestComplete (RequestID, Result)
							+ CProxyIAgentNotifySinkEx<CServerNotify>::FireRequestComplete (RequestID, Result)
							+ CProxy_DaSvrEvents2<CServerNotify>::FireRequestComplete (RequestID, Result);
		}
		__except (LogCrashCode (GetExceptionCode(), __FILE__, __LINE__))
		{}
		PostFireEvent (_DEBUG_T("RequestComplete"), lEventSinkCount);
	}
	return S_OK;
}

HRESULT CServerNotify::FireBookMark(long BookMarkID)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("BookMark")))
	{
		__try
		{
			lEventSinkCount = CProxyIDaSvrNotifySink<CServerNotify>::FireBookMark (BookMarkID)
							+ CProxyIDaSvrNotifySink2<CServerNotify>::FireBookMark (BookMarkID)
							+ CProxyIAgentNotifySink<CServerNotify>::FireBookMark (BookMarkID)
							+ CProxyIAgentNotifySinkEx<CServerNotify>::FireBookMark (BookMarkID)
							+ CProxy_DaSvrEvents2<CServerNotify>::FireBookMark (BookMarkID);
		}
		__except (LogCrashCode (GetExceptionCode(), __FILE__, __LINE__))
		{}
		PostFireEvent (_DEBUG_T("BookMark"), lEventSinkCount);
	}
	return S_OK;
}

HRESULT CServerNotify::FireIdle(long CharacterID, long Start)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("Idle")))
	{
		__try
		{
			lEventSinkCount = CProxyIDaSvrNotifySink<CServerNotify>::FireIdle (CharacterID, Start)
							+ CProxyIDaSvrNotifySink2<CServerNotify>::FireIdle (CharacterID, Start)
							+ CProxyIAgentNotifySink<CServerNotify>::FireIdle (CharacterID, Start)
							+ CProxyIAgentNotifySinkEx<CServerNotify>::FireIdle (CharacterID, Start)
							+ CProxy_DaSvrEvents2<CServerNotify>::FireIdle (CharacterID, Start);
		}
		__except (LogCrashCode (GetExceptionCode(), __FILE__, __LINE__))
		{}
		PostFireEvent (_DEBUG_T("Idle"), lEventSinkCount);
	}
	return S_OK;
}

HRESULT CServerNotify::FireMove(long CharacterID, long X, long Y, long Cause)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("Move")))
	{
		__try
		{
			lEventSinkCount = CProxyIDaSvrNotifySink<CServerNotify>::FireMove (CharacterID, X, Y, Cause)
							+ CProxyIDaSvrNotifySink2<CServerNotify>::FireMove (CharacterID, X, Y, Cause)
							+ CProxyIAgentNotifySink<CServerNotify>::FireMove (CharacterID, X, Y, Cause)
							+ CProxyIAgentNotifySinkEx<CServerNotify>::FireMove (CharacterID, X, Y, Cause)
							+ CProxy_DaSvrEvents2<CServerNotify>::FireMove (CharacterID, X, Y, Cause);
		}
		__except (LogCrashCode (GetExceptionCode(), __FILE__, __LINE__))
		{}
		PostFireEvent (_DEBUG_T("Move"), lEventSinkCount);
	}
	return S_OK;
}

HRESULT CServerNotify::FireSize(long CharacterID, long Width, long Height)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("Size")))
	{
		__try
		{
			lEventSinkCount = CProxyIDaSvrNotifySink<CServerNotify>::FireSize (CharacterID, Width, Height)
							+ CProxyIDaSvrNotifySink2<CServerNotify>::FireSize (CharacterID, Width, Height)
							+ CProxyIAgentNotifySink<CServerNotify>::FireSize (CharacterID, Width, Height)
							+ CProxyIAgentNotifySinkEx<CServerNotify>::FireSize (CharacterID, Width, Height)
							+ CProxy_DaSvrEvents2<CServerNotify>::FireSize (CharacterID, Width, Height);
		}
		__except (LogCrashCode (GetExceptionCode(), __FILE__, __LINE__))
		{}
		PostFireEvent (_DEBUG_T("Size"), lEventSinkCount);
	}
	return S_OK;
}

HRESULT CServerNotify::FireBalloonVisibleState(long CharacterID, long Visible)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("BalloonVisibleState")))
	{
		__try
		{
			lEventSinkCount = CProxyIDaSvrNotifySink<CServerNotify>::FireBalloonVisibleState (CharacterID, Visible)
							+ CProxyIDaSvrNotifySink2<CServerNotify>::FireBalloonVisibleState (CharacterID, Visible)
							+ CProxyIAgentNotifySink<CServerNotify>::FireBalloonVisibleState (CharacterID, Visible)
							+ CProxyIAgentNotifySinkEx<CServerNotify>::FireBalloonVisibleState (CharacterID, Visible)
							+ CProxy_DaSvrEvents2<CServerNotify>::FireBalloonVisibleState (CharacterID, Visible);
		}
		__except (LogCrashCode (GetExceptionCode(), __FILE__, __LINE__))
		{}
		PostFireEvent (_DEBUG_T("BalloonVisibleState"), lEventSinkCount);
	}
	return S_OK;
}

HRESULT CServerNotify::FireListeningState(long CharacterID, long Listening, long Cause)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("ListeningState")))
	{
		__try
		{
			lEventSinkCount = CProxyIDaSvrNotifySink<CServerNotify>::FireListeningState (CharacterID, Listening, Cause)
							+ CProxyIDaSvrNotifySink2<CServerNotify>::FireListeningState (CharacterID, Listening, Cause)
							+ CProxyIAgentNotifySinkEx<CServerNotify>::FireListeningState (CharacterID, Listening, Cause)
							+ CProxy_DaSvrEvents2<CServerNotify>::FireListeningState (CharacterID, Listening, Cause);
		}
		__except (LogCrashCode (GetExceptionCode(), __FILE__, __LINE__))
		{}
		PostFireEvent (_DEBUG_T("ListeningState"), lEventSinkCount);
	}
	return S_OK;
}

HRESULT CServerNotify::FireDefaultCharacterChange(BSTR CharGUID)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("DefaultCharacterChange")))
	{
		__try
		{
			lEventSinkCount = CProxyIDaSvrNotifySink<CServerNotify>::FireDefaultCharacterChange (CharGUID)
							+ CProxyIDaSvrNotifySink2<CServerNotify>::FireDefaultCharacterChange (CharGUID)
							+ CProxyIAgentNotifySinkEx<CServerNotify>::FireDefaultCharacterChange (CharGUID)
							+ CProxy_DaSvrEvents2<CServerNotify>::FireDefaultCharacterChange (CharGUID);
		}
		__except (LogCrashCode (GetExceptionCode(), __FILE__, __LINE__))
		{}
		PostFireEvent (_DEBUG_T("DefaultCharacterChange"), lEventSinkCount);
	}
	return S_OK;
}

HRESULT CServerNotify::FireAgentPropertyChange()
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("AgentPropertyChange")))
	{
		__try
		{
			lEventSinkCount = CProxyIDaSvrNotifySink<CServerNotify>::FireAgentPropertyChange ()
							+ CProxyIDaSvrNotifySink2<CServerNotify>::FireAgentPropertyChange ()
							+ CProxyIAgentNotifySinkEx<CServerNotify>::FireAgentPropertyChange ()
							+ CProxy_DaSvrEvents2<CServerNotify>::FireAgentPropertyChange ();
		}
		__except (LogCrashCode (GetExceptionCode(), __FILE__, __LINE__))
		{}
		PostFireEvent (_DEBUG_T("AgentPropertyChange"), lEventSinkCount);
	}
	return S_OK;
}

HRESULT CServerNotify::FireActiveClientChange(long CharacterID, long Status)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("ActiveClientChange")))
	{
		__try
		{
			lEventSinkCount = CProxyIDaSvrNotifySink<CServerNotify>::FireActiveClientChange (CharacterID, Status)
							+ CProxyIDaSvrNotifySink2<CServerNotify>::FireActiveClientChange (CharacterID, Status)
							+ CProxyIAgentNotifySinkEx<CServerNotify>::FireActiveClientChange (CharacterID, Status)
							+ CProxy_DaSvrEvents2<CServerNotify>::FireActiveClientChange (CharacterID, Status);
		}
		__except (LogCrashCode (GetExceptionCode(), __FILE__, __LINE__))
		{}
		PostFireEvent (_DEBUG_T("ActiveClientChange"), lEventSinkCount);
	}
	return S_OK;
}

HRESULT CServerNotify::FireSpeechStart(long CharacterID, IDaSvrFormattedText* FormattedText)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("SpeechStart")))
	{
		__try
		{
			lEventSinkCount = CProxyIDaSvrNotifySink2<CServerNotify>::FireSpeechStart (CharacterID, FormattedText)
							+ CProxy_DaSvrEvents2<CServerNotify>::FireSpeechStart (CharacterID, FormattedText);
		}
		__except (LogCrashCode (GetExceptionCode(), __FILE__, __LINE__))
		{}
		PostFireEvent (_DEBUG_T("SpeechStart"), lEventSinkCount);
	}
	return S_OK;
}

HRESULT CServerNotify::FireSpeechEnd(long CharacterID, IDaSvrFormattedText* FormattedText, VARIANT_BOOL Stopped)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("SpeechEnd")))
	{
		__try
		{
			lEventSinkCount = CProxyIDaSvrNotifySink2<CServerNotify>::FireSpeechEnd (CharacterID, FormattedText, Stopped)
							+ CProxy_DaSvrEvents2<CServerNotify>::FireSpeechEnd (CharacterID, FormattedText, Stopped);
		}
		__except (LogCrashCode (GetExceptionCode(), __FILE__, __LINE__))
		{}
		PostFireEvent (_DEBUG_T("SpeechEnd"), lEventSinkCount);
	}
	return S_OK;
}

HRESULT CServerNotify::FireSpeechWord(long CharacterID, IDaSvrFormattedText* FormattedText, long WordNdx)
{
	int	lEventSinkCount = 0;

	if	(PreFireEvent (_DEBUG_T("SpeechWord")))
	{
		__try
		{
			lEventSinkCount = CProxyIDaSvrNotifySink2<CServerNotify>::FireSpeechWord (CharacterID, FormattedText, WordNdx)
							+ CProxy_DaSvrEvents2<CServerNotify>::FireSpeechWord (CharacterID, FormattedText, WordNdx);
		}
		__except (LogCrashCode (GetExceptionCode(), __FILE__, __LINE__))
		{}
		PostFireEvent (_DEBUG_T("SpeechWord"), lEventSinkCount);
	}
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CServerNotify::Command (long CommandID, IDaSvrUserInput2 *UserInput)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireCommand (CommandID, UserInput);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::ActivateInputState (long CharacterID, long Activated)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireActivateInputState (CharacterID, Activated);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::VisibleState (long CharacterID, long Visible, long Cause)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireVisibleState (CharacterID, Visible, Cause);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::Click (long CharacterID, short Keys, long X, long Y)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireClick (CharacterID, Keys, X, Y);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::DblClick (long CharacterID, short Keys, long X, long Y)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireDblClick (CharacterID, Keys, X, Y);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::DragStart (long CharacterID, short Keys, long X, long Y)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireDragStart (CharacterID, Keys, X, Y);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::DragComplete (long CharacterID, short Keys, long X, long Y)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireDragComplete (CharacterID, Keys, X, Y);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::RequestStart (long RequestID)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireRequestStart (RequestID);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::RequestComplete (long RequestID, long Result)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireRequestComplete (RequestID, Result);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::BookMark (long BookMarkID)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireBookMark (BookMarkID);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::Idle (long CharacterID, long Start)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireIdle (CharacterID, Start);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::Move (long CharacterID, long X, long Y, long Cause)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireMove (CharacterID, X, Y, Cause);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::Size (long CharacterID, long Width, long Height)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireSize (CharacterID, Width, Height);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::BalloonVisibleState (long CharacterID, long Visible)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireBalloonVisibleState (CharacterID, Visible);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::ListeningState (long CharacterID, long Listening, long Cause)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireListeningState (CharacterID, Listening, Cause);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::DefaultCharacterChange (BSTR CharGUID)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireDefaultCharacterChange (CharGUID);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::AgentPropertyChange (void)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireAgentPropertyChange ();
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::ActiveClientChange (long CharacterID, long Status)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireActiveClientChange (CharacterID, Status);
	}
	catch AnyExceptionSilent
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CServerNotify::SpeechStart (long CharacterID, IDaSvrFormattedText* FormattedText)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireSpeechStart (CharacterID, FormattedText);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::SpeechEnd (long CharacterID, IDaSvrFormattedText* FormattedText, VARIANT_BOOL Stopped)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireSpeechEnd (CharacterID, FormattedText, Stopped);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT STDMETHODCALLTYPE CServerNotify::SpeechWord (long CharacterID, IDaSvrFormattedText* FormattedText, long WordIndex)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		lResult = FireSpeechWord (CharacterID, FormattedText, WordIndex);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT CServerNotify::OnSpeechStart (long CharacterID, LPUNKNOWN FormattedText)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		IDaSvrFormattedTextPtr	lFormattedText (FormattedText);
		lResult = FireSpeechStart (CharacterID, lFormattedText);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT CServerNotify::OnSpeechEnd (long CharacterID, LPUNKNOWN FormattedText, VARIANT_BOOL Stopped)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		IDaSvrFormattedTextPtr	lFormattedText (FormattedText);
		lResult = FireSpeechEnd (CharacterID, lFormattedText, Stopped);
	}
	catch AnyExceptionSilent
	return lResult;
}

HRESULT CServerNotify::OnSpeechWord (long CharacterID, LPUNKNOWN FormattedText, long WordIndex)
{
	HRESULT	lResult = E_FAIL;
	try
	{
		IDaSvrFormattedTextPtr	lFormattedText (FormattedText);
		lResult = FireSpeechWord (CharacterID, lFormattedText, WordIndex);
	}
	catch AnyExceptionSilent
	return lResult;
}
