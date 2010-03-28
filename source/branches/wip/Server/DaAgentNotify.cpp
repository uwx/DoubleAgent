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
#include "DaServer.h"
#include "DaAgent.h"
#include "DaAgentNotify.h"
#include "DaAgentCharacter.h"
#include "AgentPopupWnd.h"
#include "GuidStr.h"
#include <psapi.h>
#ifdef	_DEBUG
#include "Registry.h"
#endif

#pragma comment(lib, "psapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_INTERNAL		LogDebug
//#define	_DEBUG_CONNECTIONS	LogNormal
//#define	_DEBUG_ACTIVATE		LogNormal
#define	_DEBUG_NOTIFY			(GetProfileDebugInt(_T("DebugNotify"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Other"),LogVerbose,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

interface IAgentNotifySink : public IDispatch {};
interface IAgentNotifySinkEx : public IDispatch {};

_COM_SMARTPTR_TYPEDEF(IAgentNotifySink, __uuidof(IAgentNotifySink));
_COM_SMARTPTR_TYPEDEF(IAgentNotifySinkEx, __uuidof(IAgentNotifySinkEx));

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC (CDaAgentNotify, CCmdTarget)

CDaAgentNotify::CDaAgentNotify (CDaAgent & pOwner)
:	mOwner (pOwner),
	mNextReqID (100)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaAgentNotify::CDaAgentNotify (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	EnableConnections();
}

CDaAgentNotify::~CDaAgentNotify ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaAgentNotify::~CDaAgentNotify (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_CONNECTION_MAP(CDaAgentNotify, CCmdTarget)
	CONNECTION_PART(CDaAgentNotify, __uuidof(IDaSvrNotifySink), DaNotifySink)
	CONNECTION_PART(CDaAgentNotify, __uuidof(IAgentNotifySink), MsNotifySink)
	CONNECTION_PART(CDaAgentNotify, __uuidof(IAgentNotifySinkEx), MsNotifySinkEx)
END_CONNECTION_MAP()

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaAgentNotify::Register (IUnknown * punkNotifySink, long * pdwSinkID)
{
	HRESULT					lResult;
	IDaSvrNotifySink15Ptr	lDaSink (punkNotifySink);
	IDaSvrNotifySinkPtr	lDaSinkEx (punkNotifySink);
	IAgentNotifySinkPtr		lMsSink (punkNotifySink);
	IAgentNotifySinkExPtr	lMsSinkEx (punkNotifySink);

	if	(lDaSinkEx != NULL)
	{
		lResult = m_xDaNotifySink.m_xConnPt.Advise (punkNotifySink, (DWORD*)pdwSinkID);
#ifdef	_DEBUG_CONNECTIONS
		if	(SUCCEEDED (lResult))
		{
			LogMessage (_DEBUG_CONNECTIONS, _T("[%p] RegisterEx (DA) [%p] [%u]"), this, punkNotifySink, *pdwSinkID);
		}
#endif
	}
	else
	if	(lDaSink != NULL)
	{
		lResult = m_xDaNotifySink15.m_xConnPt.Advise (punkNotifySink, (DWORD*)pdwSinkID);
#ifdef	_DEBUG_CONNECTIONS
		if	(SUCCEEDED (lResult))
		{
			LogMessage (_DEBUG_CONNECTIONS, _T("[%p] Register (DA)   [%p] [%u]"), this, punkNotifySink, *pdwSinkID);
		}
#endif
	}
	else
	if	(lMsSinkEx != NULL)
	{
		lResult = m_xMsNotifySinkEx.m_xConnPt.Advise (punkNotifySink, (DWORD*)pdwSinkID);
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
		lResult = m_xMsNotifySink.m_xConnPt.Advise (punkNotifySink, (DWORD*)pdwSinkID);
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

HRESULT CDaAgentNotify::Unregister (long dwSinkID)
{
	HRESULT	lResult;

	lResult = m_xDaNotifySink.m_xConnPt.Unadvise ((DWORD)dwSinkID);
#ifdef	_DEBUG_CONNECTIONS
	if	(SUCCEEDED (lResult))
	{
		LogMessage (_DEBUG_CONNECTIONS, _T("[%p] UnregisterEx (DA) [%u]"), this, dwSinkID);
	}
#endif
	if	(lResult == CONNECT_E_NOCONNECTION)
	{
		lResult = m_xDaNotifySink15.m_xConnPt.Unadvise ((DWORD)dwSinkID);
#ifdef	_DEBUG_CONNECTIONS
		if	(SUCCEEDED (lResult))
		{
			LogMessage (_DEBUG_CONNECTIONS, _T("[%p] Unregister (DA)   [%u]"), this, dwSinkID);
		}
#endif
	}
	if	(lResult == CONNECT_E_NOCONNECTION)
	{
		lResult = m_xMsNotifySinkEx.m_xConnPt.Unadvise ((DWORD)dwSinkID);
#ifdef	_DEBUG_CONNECTIONS
		if	(SUCCEEDED (lResult))
		{
			LogMessage (_DEBUG_CONNECTIONS, _T("[%p] UnregisterEx (MS) [%u]"), this, dwSinkID);
		}
#endif
	}
	if	(lResult == CONNECT_E_NOCONNECTION)
	{
		lResult = m_xMsNotifySink.m_xConnPt.Unadvise ((DWORD)dwSinkID);
#ifdef	_DEBUG_CONNECTIONS
		if	(SUCCEEDED (lResult))
		{
			LogMessage (_DEBUG_CONNECTIONS, _T("[%p] Unregister (MS) [%u]"), this, dwSinkID);
		}
#endif
	}
	return lResult;
}

void CDaAgentNotify::UnregisterAll ()
{
	int	lCount;
	int	lNdx;

#ifdef	_DEBUG_CONNECTIONS
	LogMessage (_DEBUG_CONNECTIONS, _T("[%p] UnregisterAll [%d] [%d] {%d] [%d]"), this, m_xDaNotifySink.GetConnectionCount(), m_xDaNotifySink15.GetConnectionCount(), m_xMsNotifySinkEx.GetConnectionCount(), m_xMsNotifySink.GetConnectionCount());
#endif

	lCount = m_xDaNotifySink.GetConnectionCount();
	for	(lNdx = lCount-1; lNdx >= 0; lNdx--)
	{
		try
		{
			m_xDaNotifySink.m_xConnPt.Unadvise ((DWORD)lNdx);
#ifdef	_DEBUG_CONNECTIONS
			LogMessage (_DEBUG_CONNECTIONS, _T("[%p] UnregisterEx (DA) [%u] (All)"), this, lNdx);
#endif
		}
		catch AnyExceptionSilent
	}

	lCount = m_xDaNotifySink15.GetConnectionCount();
	for	(lNdx = lCount-1; lNdx >= 0; lNdx--)
	{
		try
		{
			m_xDaNotifySink15.m_xConnPt.Unadvise ((DWORD)lNdx);
#ifdef	_DEBUG_CONNECTIONS
			LogMessage (_DEBUG_CONNECTIONS, _T("[%p] Unregister (DA)   [%u] (All)"), this, lNdx);
#endif
		}
		catch AnyExceptionSilent
	}

	lCount = m_xMsNotifySinkEx.GetConnectionCount();
	for	(lNdx = lCount-1; lNdx >= 0; lNdx--)
	{
		try
		{
			m_xMsNotifySinkEx.m_xConnPt.Unadvise ((DWORD)lNdx);
#ifdef	_DEBUG_CONNECTIONS
			LogMessage (_DEBUG_CONNECTIONS, _T("[%p] UnregisterEx (MS) [%u] (All)"), this, lNdx);
#endif
		}
		catch AnyExceptionSilent
	}

	lCount = m_xMsNotifySink.GetConnectionCount();
	for	(lNdx = lCount-1; lNdx >= 0; lNdx--)
	{
		try
		{
			m_xMsNotifySink.m_xConnPt.Unadvise ((DWORD)lNdx);
#ifdef	_DEBUG_CONNECTIONS
			LogMessage (_DEBUG_CONNECTIONS, _T("[%p] Unregister (MS)   [%u] (All)"), this, lNdx);
#endif
		}
		catch AnyExceptionSilent
	}
}

/////////////////////////////////////////////////////////////////////////////

void CDaAgentNotify::AbandonAll ()
{
#ifdef	_DEBUG_CONNECTIONS
	LogMessage (_DEBUG_CONNECTIONS, _T("[%p] AbandonAll [%d] [%d] {%d] [%d]"), this, m_xDaNotifySink.GetConnectionCount(), m_xDaNotifySink15.GetConnectionCount(), m_xMsNotifySinkEx.GetConnectionCount(), m_xMsNotifySink.GetConnectionCount());
#endif
	m_xDaNotifySink.AbandonAll ();
	m_xDaNotifySink15.AbandonAll ();
	m_xMsNotifySink.AbandonAll ();
	m_xMsNotifySinkEx.AbandonAll ();
}

void CDaAgentNotify::XDaNotifySink::AbandonAll ()
{
	m_pUnkFirstConnection = NULL;
	if	(m_pConnections != NULL)
	{
		try
		{
			delete m_pConnections;
		}
		catch AnyExceptionSilent
	}
	m_pConnections = NULL;
}

void CDaAgentNotify::XDaNotifySink15::AbandonAll ()
{
	m_pUnkFirstConnection = NULL;
	if	(m_pConnections != NULL)
	{
		try
		{
			delete m_pConnections;
		}
		catch AnyExceptionSilent
	}
	m_pConnections = NULL;
}

void CDaAgentNotify::XMsNotifySink::AbandonAll ()
{
	m_pUnkFirstConnection = NULL;
	if	(m_pConnections != NULL)
	{
		try
		{
			delete m_pConnections;
		}
		catch AnyExceptionSilent
	}
	m_pConnections = NULL;
}

void CDaAgentNotify::XMsNotifySinkEx::AbandonAll ()
{
	m_pUnkFirstConnection = NULL;
	if	(m_pConnections != NULL)
	{
		try
		{
			delete m_pConnections;
		}
		catch AnyExceptionSilent
	}
	m_pConnections = NULL;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CDaAgentNotify::PreFireEvent (LPCTSTR pEventName)
{
#ifdef	_DEBUG_NOTIFY
	if	(LogIsActive ())
	{
		LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] Fire %s [%u] (%d %d)"), &mOwner, mOwner.m_dwRef, pEventName, AfxGetModuleState()->m_nObjectCount, AfxOleCanExitApp());
	}
#endif
	return mOwner.PreNotify ();
}

bool CDaAgentNotify::PostFireEvent (LPCTSTR pEventName)
{
#ifdef	_DEBUG_NOTIFY
	if	(LogIsActive ())
	{
		LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] Fire %s done (%d %d)"), &mOwner, mOwner.m_dwRef, pEventName, AfxGetModuleState()->m_nObjectCount, AfxOleCanExitApp());
	}
#endif
	return mOwner.PostNotify ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

long CDaAgentNotify::NextReqID ()
{
	return mNextReqID++;
}

CAgentWnd * CDaAgentNotify::_GetRequestOwner (long pReqID)
{
	CAgentWnd *	lRet = NULL;

	try
	{
		CDaServerApp *	lApp = TheServerApp;
		INT_PTR			lFileNdx;
		CAgentFile *	lFile;

		for	(lFileNdx = 0; lFile = lApp->GetCachedFile (lFileNdx); lFileNdx++)
		{
			CObTypeArray <CObject>	lFileClients;
			INT_PTR					lClientNdx;
			CAgentWnd *				lAgentWnd;

			if	(TheServerApp->GetFileClients (lFile, lFileClients))
			{
				for	(lClientNdx = lFileClients.GetUpperBound(); lClientNdx >= 0; lClientNdx--)
				{
					if	(
							(lAgentWnd = DYNAMIC_DOWNCAST (CAgentPopupWnd, lFileClients [lClientNdx]))
						&&	(lAgentWnd->FindQueuedAction (pReqID))
						)
					{
						lRet = lAgentWnd;
						break;
					}
				}
			}
			if	(lRet)
			{
				break;
			}
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDaAgentCharacter * CDaAgentNotify::_GetCharacter (long pCharID)
{
	CDaAgentCharacter *	lRet = NULL;

	try
	{
		INT_PTR			lFileNdx;
		CAgentFile *	lFile;

		for	(lFileNdx = 0; lFile = GetCachedFile (lFileNdx); lFileNdx++)
		{
			CObTypeArray <CObject>	lFileClients;
			INT_PTR					lClientNdx;
			CDaAgentCharacter *		lCharacter;

			if	(GetFileClients (lFile, lFileClients))
			{
				for	(lClientNdx = lFileClients.GetUpperBound(); lClientNdx >= 0; lClientNdx--)
				{
					if	(
							(lCharacter = DYNAMIC_DOWNCAST (CDaAgentCharacter, lFileClients [lClientNdx]))
						&&	(lCharacter->GetCharID() == pCharID)
						)
					{
						lRet = lCharacter;
						break;
					}
				}
			}
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

long CDaAgentNotify::_GetActiveCharacter ()
{
	long	lRet = 0;

	try
	{
		CDaServerApp *	lApp = TheServerApp;
		INT_PTR			lFileNdx;
		CAgentFile *	lFile;

		for	(lFileNdx = 0; lFile = lApp->GetCachedFile (lFileNdx); lFileNdx++)
		{
			CObTypeArray <CObject>	lFileClients;
			INT_PTR					lClientNdx;
			CAgentPopupWnd *		lAgentWnd;

			if	(TheServerApp->GetFileClients (lFile, lFileClients))
			{
				for	(lClientNdx = lFileClients.GetUpperBound(); lClientNdx >= 0; lClientNdx--)
				{
					if	(
							(lAgentWnd = DYNAMIC_DOWNCAST (CAgentPopupWnd, lFileClients [lClientNdx]))
						&&	(lAgentWnd->GetLastActive() == lAgentWnd->GetSafeHwnd())
						)
					{
						lRet = lAgentWnd->GetCharID();
						break;
					}
				}
			}
			if	(lRet)
			{
				break;
			}
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

long CDaAgentNotify::_GetActiveClient (long pCharID, bool pUseDefault)
{
	long	lRet = pUseDefault ? pCharID : 0;

	try
	{
		CDaServerApp *	lApp = TheServerApp;
		INT_PTR			lFileNdx;
		CAgentFile *	lFile;

//
//	Find the associated file in the global cache
//
		for	(lFileNdx = 0; lFile = lApp->GetCachedFile (lFileNdx); lFileNdx++)
		{
			CObTypeArray <CObject>	lFileClients;
			INT_PTR					lClientNdx;
			CDaAgentCharacter *		lCharacter;

			if	(TheServerApp->GetFileClients (lFile, lFileClients))
			{
				for	(lClientNdx = lFileClients.GetUpperBound(); lClientNdx >= 0; lClientNdx--)
				{
					if	(
							(lCharacter = DYNAMIC_DOWNCAST (CDaAgentCharacter, lFileClients [lClientNdx]))
						&&	(lCharacter->GetCharID() == pCharID)
						)
					{
						lRet = lCharacter->GetActiveClient ();
						break;
					}
				}
				if	(lClientNdx >= 0)
				{
					break;
				}
			}
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

long CDaAgentNotify::_GetNotifyClient (long pCharID, bool pUseDefault)
{
	long	lRet = pUseDefault ? pCharID : 0;

	try
	{
		CDaServerApp *	lApp = TheServerApp;
		INT_PTR			lFileNdx;
		CAgentFile *	lFile;

//
//	Find the associated file in the global cache
//
		for	(lFileNdx = 0; lFile = lApp->GetCachedFile (lFileNdx); lFileNdx++)
		{
			CObTypeArray <CObject>	lFileClients;
			INT_PTR					lClientNdx;
			CDaAgentCharacter *		lCharacter;

			if	(TheServerApp->GetFileClients (lFile, lFileClients))
			{
				for	(lClientNdx = lFileClients.GetUpperBound(); lClientNdx >= 0; lClientNdx--)
				{
					if	(
							(lCharacter = DYNAMIC_DOWNCAST (CDaAgentCharacter, lFileClients [lClientNdx]))
						&&	(lCharacter->GetCharID() == pCharID)
						)
					{
						break;
					}
				}

				if	(lClientNdx >= 0)
				{
//
//	Find the character for this file in the local cache
//
					if	(GetFileClients (lFile, lFileClients))
					{
						for	(lClientNdx = lFileClients.GetUpperBound(); lClientNdx >= 0; lClientNdx--)
						{
							if	(lCharacter = DYNAMIC_DOWNCAST (CDaAgentCharacter, lFileClients [lClientNdx]))
							{
								lRet = lCharacter->GetCharID ();
								break;
							}
						}
					}
					break;
				}
			}
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CDaAgentNotify::_ActiveCharacterNotify (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID)
{
	bool	lRet = false;
	long	lActiveClientID = (pActiveCharID > 0) ? _GetNotifyClient (pActiveCharID) : pActiveCharID-1;
	long	lInactiveClientID = (pInactiveCharID > 0) ? _GetNotifyClient (pInactiveCharID) : pInactiveCharID-1;

#ifdef	_DEBUG_ACTIVATE
	LogMessage (_DEBUG_ACTIVATE, _T("[%p] _ActiveCharacterNotify [%d] InputActive [%d] Inactive [%d] InputInactive [%d] (ActiveClient [%d] InactiveClient [%d])"), this, pActiveCharID, pInputActiveCharID, pInactiveCharID, pInputInactiveCharID, lActiveClientID, lInactiveClientID);
#endif

	if	(
			(pInputInactiveCharID > 0)
		&&	(pInputInactiveCharID == pInactiveCharID)
		&&	(pInputInactiveCharID != pActiveCharID)
		&&	(lInactiveClientID == pInactiveCharID)
		)
	{
#ifdef	_DEBUG_ACTIVATE
		LogMessage (_DEBUG_ACTIVATE, _T("[%p] ActivateInputState [%d] FALSE"), this, pInactiveCharID);
#endif
		ActivateInputState (pInactiveCharID, FALSE);
		lRet = true;
	}

	if	(
			(pActiveCharID >= 0)
		&&	(pInactiveCharID >= 0)
		&&	(pActiveCharID != pInactiveCharID)
#ifdef	_STRICT_COMPATIBILITY
		&&	(
				(lActiveClientID != pActiveCharID)
			||	(lInactiveClientID != pInactiveCharID)
			)
#endif
		)
	{
		if	(lInactiveClientID == pInactiveCharID)
		{
#ifdef	_DEBUG_ACTIVATE
			LogMessage (_DEBUG_ACTIVATE, _T("[%p] ActiveClientChange [%d] ActiveType_Inactive"), this, pInactiveCharID);
#endif
			ActiveClientChange (pInactiveCharID, ActiveType_Inactive);
			lRet = true;
		}

		if	(lActiveClientID == pActiveCharID)
		{
			if	(pInputActiveCharID == pActiveCharID)
			{
#ifdef	_DEBUG_ACTIVATE
				LogMessage (_DEBUG_ACTIVATE, _T("[%p] ActiveClientChange [%d] ActiveType_InputActive"), this, pActiveCharID);
#endif
				ActiveClientChange (pActiveCharID, ActiveType_InputActive);
			}
			else
			{
#ifdef	_DEBUG_ACTIVATE
				LogMessage (_DEBUG_ACTIVATE, _T("[%p] ActiveClientChange [%d] ActiveType_Active"), this, pActiveCharID);
#endif
				ActiveClientChange (pActiveCharID, ActiveType_Active);
			}
			lRet = true;
		}
	}

	if	(
			(pInputActiveCharID > 0)
		&&	(pInputActiveCharID == pActiveCharID)
		&&	(pInputActiveCharID != pInactiveCharID)
		&&	(lActiveClientID == pActiveCharID)
		)
	{
#ifdef	_DEBUG_ACTIVATE
		LogMessage (_DEBUG_ACTIVATE, _T("[%p] ActivateInputState [%d] TRUE"), this, pInputActiveCharID);
#endif
		ActivateInputState (pInputActiveCharID, TRUE);
		lRet = true;
	}
	return lRet;
}

bool CDaAgentNotify::_ActiveCharacterChanged (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID)
{
	try
	{
		TheServerApp->_OnCharacterActivated (pActiveCharID, pInputActiveCharID, pInactiveCharID, pInputInactiveCharID);
	}
	catch AnyExceptionDebug

	return true;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaAgentNotify::_CharacterLoaded (long pCharID)
{
	try
	{
		int					lNotifyNdx;
		IDaInternalNotify *	lNotify;

		for	(lNotifyNdx = 0; lNotify = mInternalNotify (lNotifyNdx); lNotifyNdx++)
		{
			try
			{
#ifdef	_DEBUG_INTERNAL
				LogMessage (_DEBUG_INTERNAL, _T("[%p] _OnCharacterLoaded [%d]"), lNotify, pCharID);
#endif
				lNotify->_OnCharacterLoaded (pCharID);
			}
			catch AnyExceptionDebug
		}
	}
	catch AnyExceptionDebug
}

void CDaAgentNotify::_CharacterUnloaded (long pCharID)
{
	try
	{
		int					lNotifyNdx;
		IDaInternalNotify *	lNotify;

		for	(lNotifyNdx = 0; lNotify = mInternalNotify (lNotifyNdx); lNotifyNdx++)
		{
			try
			{
#ifdef	_DEBUG_INTERNAL
				LogMessage (_DEBUG_INTERNAL, _T("[%p] _OnCharacterUnloaded [%d]"), lNotify, pCharID);
#endif
				lNotify->_OnCharacterUnloaded (pCharID);
			}
			catch AnyExceptionDebug
		}
	}
	catch AnyExceptionDebug
}

void CDaAgentNotify::_CharacterNameChanged (long pCharID)
{
	try
	{
		int					lNotifyNdx;
		IDaInternalNotify *	lNotify;

		for	(lNotifyNdx = 0; lNotify = mInternalNotify (lNotifyNdx); lNotifyNdx++)
		{
			try
			{
#ifdef	_DEBUG_INTERNAL
				LogMessage (_DEBUG_INTERNAL, _T("[%p] _OnCharacterNameChanged [%d]"), lNotify, pCharID);
#endif
				lNotify->_OnCharacterNameChanged (pCharID);
			}
			catch AnyExceptionDebug
		}
	}
	catch AnyExceptionDebug
}

void CDaAgentNotify::_CharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID)
{
	try
	{
		int					lNotifyNdx;
		IDaInternalNotify *	lNotify;

		for	(lNotifyNdx = 0; lNotify = mInternalNotify (lNotifyNdx); lNotifyNdx++)
		{
			try
			{
#ifdef	_DEBUG_INTERNAL
				LogMessage (_DEBUG_INTERNAL, _T("[%p] _OnCharacterActivated [%d] [%d] [%d] [%d]"), lNotify, pActiveCharID, pInputActiveCharID, pInactiveCharID, pInputInactiveCharID);
#endif
				lNotify->_OnCharacterActivated (pActiveCharID, pInputActiveCharID, pInactiveCharID, pInputInactiveCharID);
			}
			catch AnyExceptionDebug
		}
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////

void CDaAgentNotify::_CharacterListening (long pCharID, bool pListening, long pCause)
{
	long	lCharID;

	if	(lCharID = _GetNotifyClient (pCharID, false))
	{
		ListeningState (lCharID, (pListening!=false), pCause);
	}
}

bool CDaAgentNotify::_DownloadComplete (class CFileDownload * pDownload)
{
	bool	lRet = false;

	try
	{
		int					lNotifyNdx;
		IDaInternalNotify *	lNotify;

		for	(lNotifyNdx = 0; lNotify = mInternalNotify (lNotifyNdx); lNotifyNdx++)
		{
			try
			{
#ifdef	_DEBUG_INTERNAL
				LogMessage (_DEBUG_INTERNAL, _T("[%p] _OnDownloadComplete"), lNotify);
#endif
				if	(lNotify->_OnDownloadComplete (pDownload))
				{
					lRet = true;
					break;
				}
			}
			catch AnyExceptionDebug
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

class CFileDownload * CDaAgentNotify::_FindSoundDownload (LPCTSTR pSoundUrl)
{
	class CFileDownload *	lRet = NULL;

	try
	{
		int					lNotifyNdx;
		IDaInternalNotify *	lNotify;

		for	(lNotifyNdx = 0; lNotify = mInternalNotify (lNotifyNdx); lNotifyNdx++)
		{
			try
			{
				if	(lRet = lNotify->_FindSoundDownload (pSoundUrl))
				{
					break;
				}
			}
			catch AnyExceptionDebug
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

long CDaAgentNotify::_GetVisibilityCause (long pCharID)
{
	long lVisibilityCause = VisibilityCause_NeverShown;
	mVisibilityCause.Lookup (pCharID, lVisibilityCause);
	return lVisibilityCause;
}

void CDaAgentNotify::_PutVisibilityCause (long pCharID, long pVisibilityCause)
{
	mVisibilityCause [pCharID] = pVisibilityCause;

	if	(pVisibilityCause == VisibilityCause_ProgramHid)
	{
		try
		{
			EmptyWorkingSet (GetCurrentProcess ());
		}
		catch AnyExceptionSilent
	}
}

long CDaAgentNotify::_GetMoveCause (long pCharID)
{
	long lMoveCause = MoveCause_NeverMoved;
	mMoveCause.Lookup (pCharID, lMoveCause);
	return lMoveCause;
}

void CDaAgentNotify::_PutMoveCause (long pCharID, long pMoveCause)
{
	mMoveCause [pCharID] = pMoveCause;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CDaAgentNotify::_DoContextMenu (long pCharID, HWND pOwner, const CPoint & pPosition)
{
	bool	lRet = false;

	try
	{
		int					lNotifyNdx;
		IDaInternalNotify *	lNotify;

		for	(lNotifyNdx = 0; lNotify = mInternalNotify (lNotifyNdx); lNotifyNdx++)
		{
			try
			{
				if	(lNotify->_OnContextMenu (pCharID, pOwner, pPosition))
				{
					lRet = true;
				}
			}
			catch AnyExceptionDebug
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

bool CDaAgentNotify::_DoDefaultCommand (long pCharID, HWND pOwner, const CPoint & pPosition)
{
	bool	lRet = false;

	try
	{
		int					lNotifyNdx;
		IDaInternalNotify *	lNotify;

		for	(lNotifyNdx = 0; lNotify = mInternalNotify (lNotifyNdx); lNotifyNdx++)
		{
			try
			{
				if	(lNotify->_OnDefaultCommand (pCharID, pOwner, pPosition))
				{
					lRet = true;
				}
			}
			catch AnyExceptionDebug
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

void CDaAgentNotify::_OptionsChanged ()
{
	try
	{
		int					lNotifyNdx;
		IDaInternalNotify *	lNotify;

		for	(lNotifyNdx = 0; lNotify = mInternalNotify (lNotifyNdx); lNotifyNdx++)
		{
			try
			{
#ifdef	_DEBUG_INTERNAL
				LogMessage (_DEBUG_INTERNAL, _T("[%p] _OnOptionsChanged"), lNotify);
#endif
				lNotify->_OnOptionsChanged ();
			}
			catch AnyExceptionDebug
		}
	}
	catch AnyExceptionDebug

	try
	{
		AgentPropertyChange ();
	}
	catch AnyExceptionSilent
}

void CDaAgentNotify::_DefaultCharacterChanged (REFGUID pCharGuid)
{
	try
	{
		int					lNotifyNdx;
		IDaInternalNotify *	lNotify;

		for	(lNotifyNdx = 0; lNotify = mInternalNotify (lNotifyNdx); lNotifyNdx++)
		{
			try
			{
#ifdef	_DEBUG_INTERNAL
				LogMessage (_DEBUG_INTERNAL, _T("[%p] _OnDefaultCharacterChanged"), lNotify);
#endif
				lNotify->_OnDefaultCharacterChanged ();
			}
			catch AnyExceptionDebug
		}
	}
	catch AnyExceptionDebug

	try
	{
		DefaultCharacterChange (_bstr_t ((CString)CGuidStr (pCharGuid)));
	}
	catch AnyExceptionSilent
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaAgentNotify::_RegisterInternalNotify (IDaInternalNotify * pNotify, bool pRegister)
{
	try
	{
		if	(pNotify)
		{
#ifdef	_DEBUG_INTERNAL
			LogMessage (_DEBUG_INTERNAL, _T("[%p] _RegisterInternalNotify [%u]"), pNotify, pRegister);
#endif
			if	(pRegister)
			{
				mInternalNotify.AddUnique (pNotify);
			}
			else
			{
				mInternalNotify.Remove (pNotify);
			}
		}
	}
	catch AnyExceptionDebug
}
