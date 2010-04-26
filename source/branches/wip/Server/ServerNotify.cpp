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
#include "DaServer.h"
#include "ServerNotify.h"
#include "DaSvrCharacter.h"
#include "AgentPopupWnd.h"
#include "GuidStr.h"
#include <psapi.h>
#ifdef	_DEBUG
#include "Registry.h"
#endif

#pragma comment(lib, "psapi.lib")

#ifdef	_DEBUG
//#define	_DEBUG_INTERNAL		LogDebug
//#define	_DEBUG_CONNECTIONS	LogNormal
//#define	_DEBUG_ACTIVATE		LogNormal
#define	_DEBUG_NOTIFY			(GetProfileDebugInt(_T("DebugNotify"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Other"),LogVerbose,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

_COM_SMARTPTR_TYPEDEF(IAgentNotifySink, __uuidof(IAgentNotifySink));
_COM_SMARTPTR_TYPEDEF(IAgentNotifySinkEx, __uuidof(IAgentNotifySinkEx));

/////////////////////////////////////////////////////////////////////////////

CServerNotify::CServerNotify (DaServer & pOwner)
:	mOwner (pOwner),
	mNextReqID (100)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CServerNotify::CServerNotify (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
}

CServerNotify::~CServerNotify ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CServerNotify::~CServerNotify (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
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
	IDaSvrNotifySinkPtr		lDaSinkEx (punkNotifySink);
	IAgentNotifySinkPtr		lMsSink (punkNotifySink);
	IAgentNotifySinkExPtr	lMsSinkEx (punkNotifySink);

	if	(lDaSinkEx != NULL)
	{
		lResult = tDaSvrNotifySink::Advise (punkNotifySink, (DWORD*)pdwSinkID);
#ifdef	_DEBUG_CONNECTIONS
		if	(SUCCEEDED (lResult))
		{
			LogMessage (_DEBUG_CONNECTIONS, _T("[%p] RegisterEx (DA) [%p] [%u]"), this, punkNotifySink, *pdwSinkID);
		}
#endif
	}
	else
	if	(lMsSinkEx != NULL)
	{
		lResult = tAgentNotifySinkEx::Advise (punkNotifySink, (DWORD*)pdwSinkID);
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
		lResult = tAgentNotifySink::Advise (punkNotifySink, (DWORD*)pdwSinkID);
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

HRESULT CServerNotify::Unregister (long dwSinkID)
{
	HRESULT	lResult = S_FALSE;

	lResult = tDaSvrNotifySink::Unadvise ((DWORD)dwSinkID);
#ifdef	_DEBUG_CONNECTIONS
	if	(SUCCEEDED (lResult))
	{
		LogMessage (_DEBUG_CONNECTIONS, _T("[%p] UnregisterEx (DA) [%u]"), this, dwSinkID);
	}
#endif
	if	(lResult == CONNECT_E_NOCONNECTION)
	{
		lResult = tAgentNotifySinkEx::Unadvise ((DWORD)dwSinkID);
#ifdef	_DEBUG_CONNECTIONS
		if	(SUCCEEDED (lResult))
		{
			LogMessage (_DEBUG_CONNECTIONS, _T("[%p] UnregisterEx (MS) [%u]"), this, dwSinkID);
		}
#endif
	}
	if	(lResult == CONNECT_E_NOCONNECTION)
	{
		lResult = tAgentNotifySink::Unadvise ((DWORD)dwSinkID);
#ifdef	_DEBUG_CONNECTIONS
		if	(SUCCEEDED (lResult))
		{
			LogMessage (_DEBUG_CONNECTIONS, _T("[%p] Unregister (MS) [%u]"), this, dwSinkID);
		}
#endif
	}
	return lResult;
}

void CServerNotify::UnregisterAll ()
{
#ifdef	_DEBUG_CONNECTIONS
	LogMessage (_DEBUG_CONNECTIONS, _T("[%p] UnregisterAll [%d] [%d] [%d]"), this, CountUnk(tDaSvrNotifySink::m_vec), CountUnk(tAgentNotifySinkEx::m_vec), CountUnk(tAgentNotifySink::m_vec));
#endif
	int	lNdx;

	for	(lNdx = tDaSvrNotifySink::m_vec.GetSize()-1; lNdx >= 0; lNdx--)
	{
		try
		{
			if	(tDaSvrNotifySink::m_vec.GetAt (lNdx))
			{
				tDaSvrNotifySink::Unadvise ((DWORD)lNdx);
#ifdef	_DEBUG_CONNECTIONS
				LogMessage (_DEBUG_CONNECTIONS, _T("[%p] UnregisterEx (DA) [%u] (All)"), this, lNdx);
#endif
			}
		}
		catch AnyExceptionSilent
	}

	for	(lNdx = tAgentNotifySinkEx::m_vec.GetSize()-1; lNdx >= 0; lNdx--)
	{
		try
		{
			if	(tAgentNotifySinkEx::m_vec.GetAt (lNdx))
			{
				tAgentNotifySinkEx::Unadvise ((DWORD)lNdx);
#ifdef	_DEBUG_CONNECTIONS
				LogMessage (_DEBUG_CONNECTIONS, _T("[%p] UnregisterEx (MS) [%u] (All)"), this, lNdx);
#endif
			}
		}
		catch AnyExceptionSilent
	}

	for	(lNdx = tAgentNotifySink::m_vec.GetSize()-1; lNdx >= 0; lNdx--)
	{
		try
		{
			if	(tAgentNotifySink::m_vec.GetAt (lNdx))
			{
				tAgentNotifySink::Unadvise ((DWORD)lNdx);
#ifdef	_DEBUG_CONNECTIONS
				LogMessage (_DEBUG_CONNECTIONS, _T("[%p] Unregister (MS)   [%u] (All)"), this, lNdx);
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
	LogMessage (_DEBUG_CONNECTIONS, _T("[%p] AbandonAll [%d] {%d] [%d]"), this, CountUnk(tDaSvrNotifySink::m_vec), CountUnk(tAgentNotifySinkEx::m_vec), CountUnk(tAgentNotifySink::m_vec));
#endif
	tDaSvrNotifySink::m_vec.clear ();
	tAgentNotifySinkEx::m_vec.clear ();
	tAgentNotifySink::m_vec.clear ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CServerNotify::PreFireEvent (LPCTSTR pEventName)
{
#ifdef	_DEBUG_NOTIFY
	if	(LogIsActive ())
	{
		LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] Fire %s (%d %d)"), &mOwner, mOwner.m_dwRef, pEventName, _AtlModule.GetLockCount(), (_AtlModule.GetLockCount()==0));
	}
#endif
	return mOwner.PreNotify ();
}

bool CServerNotify::PostFireEvent (LPCTSTR pEventName)
{
#ifdef	_DEBUG_NOTIFY
	if	(LogIsActive ())
	{
		LogMessage (_DEBUG_NOTIFY, _T("[%p(%d)] Fire %s done (%d %d)"), &mOwner, mOwner.m_dwRef, pEventName, _AtlModule.GetLockCount(), (_AtlModule.GetLockCount()==0));
	}
#endif
	return mOwner.PostNotify ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

long CServerNotify::NextReqID ()
{
	return mNextReqID++;
}

CAgentWnd * CServerNotify::_GetRequestOwner (long pReqID)
{
	CAgentWnd *	lRet = NULL;

	try
	{
		INT_PTR			lFileNdx;
		CAgentFile *	lFile;

		for	(lFileNdx = 0; lFile = _AtlModule.GetCachedFile (lFileNdx); lFileNdx++)
		{
			CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
			INT_PTR								lClientNdx;
			CAgentWnd *							lAgentWnd;

			if	(_AtlModule.GetFileClients (lFile, lFileClients))
			{
				for	(lClientNdx = lFileClients.GetCount()-1; lClientNdx >= 0; lClientNdx--)
				{
					if	(
							(lAgentWnd = dynamic_cast <CAgentWnd *> (lFileClients [lClientNdx]))
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

CAgentWnd * CServerNotify::_GetAgentWnd (HWND pWindow)
{
	CAgentWnd *	lRet = NULL;

	try
	{
		INT_PTR			lFileNdx;
		CAgentFile *	lFile;

		for	(lFileNdx = 0; lFile = _AtlModule.GetCachedFile (lFileNdx); lFileNdx++)
		{
			CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
			INT_PTR								lClientNdx;
			CAgentWnd *							lAgentWnd;

			if	(_AtlModule.GetFileClients (lFile, lFileClients))
			{
				for	(lClientNdx = lFileClients.GetCount()-1; lClientNdx >= 0; lClientNdx--)
				{
					if	(
							(lAgentWnd = dynamic_cast <CAgentWnd *> (lFileClients [lClientNdx]))
						&&	(lAgentWnd->m_hWnd == pWindow)
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

DaSvrCharacter * CServerNotify::_GetCharacter (long pCharID)
{
	DaSvrCharacter *	lRet = NULL;

	try
	{
		INT_PTR			lFileNdx;
		CAgentFile *	lFile;

		for	(lFileNdx = 0; lFile = GetCachedFile (lFileNdx); lFileNdx++)
		{
			CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
			INT_PTR								lClientNdx;
			DaSvrCharacter *					lCharacter;

			if	(GetFileClients (lFile, lFileClients))
			{
				for	(lClientNdx = lFileClients.GetCount()-1; lClientNdx >= 0; lClientNdx--)
				{
					if	(
							(lCharacter = dynamic_cast <DaSvrCharacter *> (lFileClients [lClientNdx]))
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

long CServerNotify::_GetActiveCharacter ()
{
	long	lRet = 0;

	try
	{
		INT_PTR			lFileNdx;
		CAgentFile *	lFile;

		for	(lFileNdx = 0; lFile = _AtlModule.GetCachedFile (lFileNdx); lFileNdx++)
		{
			CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
			INT_PTR								lClientNdx;
			CAgentPopupWnd *					lAgentWnd;

			if	(_AtlModule.GetFileClients (lFile, lFileClients))
			{
				for	(lClientNdx = lFileClients.GetCount()-1; lClientNdx >= 0; lClientNdx--)
				{
					if	(
							(lAgentWnd = dynamic_cast <CAgentPopupWnd *> (lFileClients [lClientNdx]))
						&&	(lAgentWnd->IsWindow ())
						&&	(lAgentWnd->GetLastActive() == lAgentWnd->m_hWnd)
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

long CServerNotify::_GetActiveClient (long pCharID, bool pUseDefault)
{
	long	lRet = pUseDefault ? pCharID : 0;

	try
	{
		INT_PTR			lFileNdx;
		CAgentFile *	lFile;

//
//	Find the associated file in the global cache
//
		for	(lFileNdx = 0; lFile = _AtlModule.GetCachedFile (lFileNdx); lFileNdx++)
		{
			CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
			INT_PTR								lClientNdx;
			DaSvrCharacter *					lCharacter;

			if	(_AtlModule.GetFileClients (lFile, lFileClients))
			{
				for	(lClientNdx = lFileClients.GetCount()-1; lClientNdx >= 0; lClientNdx--)
				{
					if	(
							(lCharacter = dynamic_cast <DaSvrCharacter *> (lFileClients [lClientNdx]))
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

long CServerNotify::_GetNotifyClient (long pCharID, bool pUseDefault)
{
	long	lRet = pUseDefault ? pCharID : 0;

	try
	{
		INT_PTR			lFileNdx;
		CAgentFile *	lFile;

//
//	Find the associated file in the global cache
//
		for	(lFileNdx = 0; lFile = _AtlModule.GetCachedFile (lFileNdx); lFileNdx++)
		{
			CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
			INT_PTR								lClientNdx;
			DaSvrCharacter *					lCharacter;

			if	(_AtlModule.GetFileClients (lFile, lFileClients))
			{
				for	(lClientNdx = lFileClients.GetCount()-1; lClientNdx >= 0; lClientNdx--)
				{
					if	(
							(lCharacter = dynamic_cast <DaSvrCharacter *> (lFileClients [lClientNdx]))
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
						for	(lClientNdx = lFileClients.GetCount()-1; lClientNdx >= 0; lClientNdx--)
						{
							if	(lCharacter = dynamic_cast <DaSvrCharacter *> (lFileClients [lClientNdx]))
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

bool CServerNotify::_ActiveCharacterNotify (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID)
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
			LogMessage (_DEBUG_ACTIVATE, _T("[%p] ActiveClientChange [%d] ActiveState_Inactive"), this, pInactiveCharID);
#endif
			ActiveClientChange (pInactiveCharID, ActiveState_Inactive);
			lRet = true;
		}

		if	(lActiveClientID == pActiveCharID)
		{
			if	(pInputActiveCharID == pActiveCharID)
			{
#ifdef	_DEBUG_ACTIVATE
				LogMessage (_DEBUG_ACTIVATE, _T("[%p] ActiveClientChange [%d] ActiveState_InputActive"), this, pActiveCharID);
#endif
				ActiveClientChange (pActiveCharID, ActiveState_InputActive);
			}
			else
			{
#ifdef	_DEBUG_ACTIVATE
				LogMessage (_DEBUG_ACTIVATE, _T("[%p] ActiveClientChange [%d] ActiveState_Active"), this, pActiveCharID);
#endif
				ActiveClientChange (pActiveCharID, ActiveState_Active);
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

bool CServerNotify::_ActiveCharacterChanged (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID)
{
	try
	{
		_AtlModule._OnCharacterActivated (pActiveCharID, pInputActiveCharID, pInactiveCharID, pInputInactiveCharID);
	}
	catch AnyExceptionDebug

	return true;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CServerNotify::_CharacterLoaded (long pCharID)
{
	try
	{
		INT_PTR					lNotifyNdx;
		_IServerNotifySink *	lNotify;

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

void CServerNotify::_CharacterUnloaded (long pCharID)
{
	try
	{
		INT_PTR					lNotifyNdx;
		_IServerNotifySink *	lNotify;

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

void CServerNotify::_CharacterNameChanged (long pCharID)
{
	try
	{
		INT_PTR					lNotifyNdx;
		_IServerNotifySink *	lNotify;

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

void CServerNotify::_CharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID)
{
	try
	{
		INT_PTR					lNotifyNdx;
		_IServerNotifySink *	lNotify;

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

void CServerNotify::_CharacterListening (long pCharID, bool pListening, long pCause)
{
	long	lCharID;

	if	(lCharID = _GetNotifyClient (pCharID, false))
	{
		ListeningState (lCharID, (pListening!=false), pCause);
	}
}

bool CServerNotify::_DownloadComplete (class CFileDownload * pDownload)
{
	bool	lRet = false;

	try
	{
		INT_PTR					lNotifyNdx;
		_IServerNotifySink *	lNotify;

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

class CFileDownload * CServerNotify::_FindSoundDownload (LPCTSTR pSoundUrl)
{
	class CFileDownload *	lRet = NULL;

	try
	{
		INT_PTR					lNotifyNdx;
		_IServerNotifySink *	lNotify;

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

VisibilityCauseType CServerNotify::_GetVisibilityCause (long pCharID)
{
	long lVisibilityCause = VisibilityCause_NeverShown;
	mVisibilityCause.Lookup (pCharID, lVisibilityCause);
	return (VisibilityCauseType)lVisibilityCause;
}

void CServerNotify::_PutVisibilityCause (long pCharID, VisibilityCauseType pVisibilityCause)
{
	mVisibilityCause [pCharID] = (long)pVisibilityCause;

	if	(pVisibilityCause == VisibilityCause_ProgramHid)
	{
		try
		{
			EmptyWorkingSet (GetCurrentProcess ());
		}
		catch AnyExceptionSilent
	}
}

MoveCauseType CServerNotify::_GetMoveCause (long pCharID)
{
	long lMoveCause = MoveCause_NeverMoved;
	mMoveCause.Lookup (pCharID, lMoveCause);
	return (MoveCauseType)lMoveCause;
}

void CServerNotify::_PutMoveCause (long pCharID, MoveCauseType pMoveCause)
{
	mMoveCause [pCharID] = (long)pMoveCause;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CServerNotify::_DoContextMenu (long pCharID, HWND pOwner, const CPoint & pPosition)
{
	bool	lRet = false;

	try
	{
		INT_PTR					lNotifyNdx;
		_IServerNotifySink *	lNotify;

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

bool CServerNotify::_DoDefaultCommand (long pCharID, HWND pOwner, const CPoint & pPosition)
{
	bool	lRet = false;

	try
	{
		INT_PTR					lNotifyNdx;
		_IServerNotifySink *	lNotify;

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

void CServerNotify::_OptionsChanged ()
{
	try
	{
		INT_PTR					lNotifyNdx;
		_IServerNotifySink *	lNotify;

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

void CServerNotify::_DefaultCharacterChanged (REFGUID pCharGuid)
{
	try
	{
		INT_PTR					lNotifyNdx;
		_IServerNotifySink *	lNotify;

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

void CServerNotify::_RegisterInternalNotify (_IServerNotifySink * pNotify, bool pRegister)
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
