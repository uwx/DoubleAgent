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
#include "EventNotify.h"
#include "DaCmnCharacter.h"
#include "AgentPopupWnd.h"
#include "GuidStr.h"

#ifdef	_DEBUG
//#define	_DEBUG_INTERNAL		LogDebug
//#define	_DEBUG_ACTIVATE		LogNormal
#endif

/////////////////////////////////////////////////////////////////////////////

CEventNotify::CEventNotify (_IEventNotify & pGlobalNotify, CAgentFileCache & pFileCache)
:	mGlobalNotify (pGlobalNotify),
	mGlobalFileCache (pFileCache),
	mNextReqID (100)
{
}

CEventNotify::~CEventNotify ()
{
}

/////////////////////////////////////////////////////////////////////////////

void CEventNotify::_RegisterEventReflect (_IEventReflect * pEventReflect, bool pRegister)
{
	try
	{
		if	(pEventReflect)
		{
#ifdef	_DEBUG_INTERNAL
			LogMessage (_DEBUG_INTERNAL, _T("[%p] _RegisterEventReflect [%u]"), pEventReflect, pRegister);
#endif
			if	(pRegister)
			{
				mEventReflect.AddUnique (pEventReflect);
			}
			else
			{
				mEventReflect.Remove (pEventReflect);
			}
		}
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

long CEventNotify::NextReqID ()
{
	return mNextReqID++;
}

bool CEventNotify::PreFireEvent (LPCTSTR pEventName)
{
	return true;
}

bool CEventNotify::PostFireEvent (LPCTSTR pEventName)
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////

CAgentWnd * CEventNotify::_GetRequestOwner (long pReqID)
{
	CAgentWnd *	lRet = NULL;

	try
	{
		INT_PTR			lFileNdx;
		CAgentFile *	lFile;

		for	(lFileNdx = 0; lFile = mGlobalFileCache.GetCachedFile (lFileNdx); lFileNdx++)
		{
			CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
			INT_PTR								lClientNdx;
			CAgentWnd *							lAgentWnd;

			if	(mGlobalFileCache.GetFileClients (lFile, lFileClients))
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

CAgentWnd * CEventNotify::_GetAgentWnd (HWND pWindow)
{
	CAgentWnd *	lRet = NULL;

	try
	{
		INT_PTR			lFileNdx;
		CAgentFile *	lFile;

		for	(lFileNdx = 0; lFile = mGlobalFileCache.GetCachedFile (lFileNdx); lFileNdx++)
		{
			CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
			INT_PTR								lClientNdx;
			CAgentWnd *							lAgentWnd;

			if	(mGlobalFileCache.GetFileClients (lFile, lFileClients))
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

CDaCmnCharacter * CEventNotify::_GetAppCharacter (long pCharID)
{
	return _GetCharacter (pCharID, mGlobalFileCache);
}

CDaCmnCharacter * CEventNotify::_GetCharacter (long pCharID)
{
	return _GetCharacter (pCharID, *this);
}

long CEventNotify::_GetActiveCharacter ()
{
	return _GetActiveCharacter (mGlobalFileCache);
}

long CEventNotify::_GetListenCharacter ()
{
	return _GetListenCharacter (mGlobalFileCache);
}

/////////////////////////////////////////////////////////////////////////////

long CEventNotify::_GetActiveClient (long pCharID, bool pUseDefault)
{
	long	lRet = pUseDefault ? pCharID : 0;

	try
	{
		INT_PTR			lFileNdx;
		CAgentFile *	lFile;

//
//	Find the associated file in the global cache
//
		for	(lFileNdx = 0; lFile = mGlobalFileCache.GetCachedFile (lFileNdx); lFileNdx++)
		{
			CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
			INT_PTR								lClientNdx;
			CDaCmnCharacter *						lCharacter;

			if	(mGlobalFileCache.GetFileClients (lFile, lFileClients))
			{
				for	(lClientNdx = lFileClients.GetCount()-1; lClientNdx >= 0; lClientNdx--)
				{
					if	(
							(lCharacter = dynamic_cast <CDaCmnCharacter *> (lFileClients [lClientNdx]))
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

long CEventNotify::_GetNotifyClient (long pCharID, bool pUseDefault)
{
	long	lRet = pUseDefault ? pCharID : 0;

	try
	{
		INT_PTR			lFileNdx;
		CAgentFile *	lFile;

//
//	Find the associated file in the global cache
//
		for	(lFileNdx = 0; lFile = mGlobalFileCache.GetCachedFile (lFileNdx); lFileNdx++)
		{
			CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
			INT_PTR								lClientNdx;
			CDaCmnCharacter *						lCharacter;

			if	(mGlobalFileCache.GetFileClients (lFile, lFileClients))
			{
				for	(lClientNdx = lFileClients.GetCount()-1; lClientNdx >= 0; lClientNdx--)
				{
					if	(
							(lCharacter = dynamic_cast <CDaCmnCharacter *> (lFileClients [lClientNdx]))
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
							if	(lCharacter = dynamic_cast <CDaCmnCharacter *> (lFileClients [lClientNdx]))
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
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDaCmnCharacter * CEventNotify::_GetCharacter (long pCharID, CAgentFileCache & pFileCache)
{
	CDaCmnCharacter *	lRet = NULL;

	try
	{
		INT_PTR			lFileNdx;
		CAgentFile *	lFile;

		for	(lFileNdx = 0; lFile = pFileCache.GetCachedFile (lFileNdx); lFileNdx++)
		{
			CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
			INT_PTR								lClientNdx;
			CDaCmnCharacter *						lCharacter;

			if	(pFileCache.GetFileClients (lFile, lFileClients))
			{
				for	(lClientNdx = lFileClients.GetCount()-1; lClientNdx >= 0; lClientNdx--)
				{
					if	(
							(lCharacter = dynamic_cast <CDaCmnCharacter *> (lFileClients [lClientNdx]))
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

long CEventNotify::_GetActiveCharacter (CAgentFileCache & pFileCache)
{
	long	lRet = 0;

	try
	{
		INT_PTR			lFileNdx;
		CAgentFile *	lFile;

		for	(lFileNdx = 0; lFile = pFileCache.GetCachedFile (lFileNdx); lFileNdx++)
		{
			CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
			INT_PTR								lClientNdx;
			CAgentPopupWnd *					lAgentWnd;

			if	(pFileCache.GetFileClients (lFile, lFileClients))
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

long CEventNotify::_GetListenCharacter (CAgentFileCache & pFileCache)
{
	long	lRet = 0;

	try
	{
		CDaCmnCharacter *	lActiveCharacter = NULL;
		CDaCmnCharacter *	lClientActiveCharacter = NULL;
		INT_PTR			lFileNdx;
		CAgentFile *	lFile;

		for	(lFileNdx = 0; lFile = pFileCache.GetCachedFile (lFileNdx); lFileNdx++)
		{
			CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
			INT_PTR								lClientNdx;
			CDaCmnCharacter *						lCharacter;

			if	(pFileCache.GetFileClients (lFile, lFileClients))
			{
				for	(lClientNdx = lFileClients.GetCount()-1; lClientNdx >= 0; lClientNdx--)
				{
					if	(lCharacter = dynamic_cast <CDaCmnCharacter *> (lFileClients [lClientNdx]))
					{
						if	(lCharacter->IsInputActive ())
						{
							lActiveCharacter = lCharacter;
							break;
						}
						else
						if	(
								(!lClientActiveCharacter)
							&&	(lCharacter->IsClientActive ())
							)
						{
							lClientActiveCharacter = lCharacter;
						}
					}
				}
			}
			if	(lActiveCharacter)
			{
				break;
			}
		}

		if	(lActiveCharacter)
		{
			lRet = lActiveCharacter->GetCharID();
		}
		else
		{
			lRet = lClientActiveCharacter->GetCharID();
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CEventNotify::_ActiveCharacterNotify (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID)
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

bool CEventNotify::_ActiveCharacterChanged (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID)
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

VisibilityCauseType CEventNotify::_GetVisibilityCause (long pCharID)
{
	long lVisibilityCause = VisibilityCause_NeverShown;
	mVisibilityCause.Lookup (pCharID, lVisibilityCause);
	return (VisibilityCauseType)lVisibilityCause;
}

void CEventNotify::_PutVisibilityCause (long pCharID, VisibilityCauseType pVisibilityCause)
{
	mVisibilityCause [pCharID] = (long)pVisibilityCause;

	//if	(pVisibilityCause == VisibilityCause_ProgramHid)
	//{
	//	try
	//	{
	//		EmptyWorkingSet (GetCurrentProcess ());
	//	}
	//	catch AnyExceptionSilent
	//}
}

MoveCauseType CEventNotify::_GetMoveCause (long pCharID)
{
	long lMoveCause = MoveCause_NeverMoved;
	mMoveCause.Lookup (pCharID, lMoveCause);
	return (MoveCauseType)lMoveCause;
}

void CEventNotify::_PutMoveCause (long pCharID, MoveCauseType pMoveCause)
{
	mMoveCause [pCharID] = (long)pMoveCause;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CEventNotify::_CharacterLoaded (long pCharID)
{
	try
	{
		INT_PTR				lReflectNdx;
		_IEventReflect *	lReflect;

		for	(lReflectNdx = 0; lReflect = mEventReflect (lReflectNdx); lReflectNdx++)
		{
			try
			{
#ifdef	_DEBUG_INTERNAL
				LogMessage (_DEBUG_INTERNAL, _T("[%p] _OnCharacterLoaded [%d]"), lReflect, pCharID);
#endif
				lReflect->_OnCharacterLoaded (pCharID);
			}
			catch AnyExceptionDebug
		}
	}
	catch AnyExceptionDebug
}

void CEventNotify::_CharacterUnloaded (long pCharID)
{
	try
	{
		INT_PTR				lReflectNdx;
		_IEventReflect *	lReflect;

		for	(lReflectNdx = 0; lReflect = mEventReflect (lReflectNdx); lReflectNdx++)
		{
			try
			{
#ifdef	_DEBUG_INTERNAL
				LogMessage (_DEBUG_INTERNAL, _T("[%p] _OnCharacterUnloaded [%d]"), lReflect, pCharID);
#endif
				lReflect->_OnCharacterUnloaded (pCharID);
			}
			catch AnyExceptionDebug
		}
	}
	catch AnyExceptionDebug
}

void CEventNotify::_CharacterNameChanged (long pCharID)
{
	try
	{
		INT_PTR				lReflectNdx;
		_IEventReflect *	lReflect;

		for	(lReflectNdx = 0; lReflect = mEventReflect (lReflectNdx); lReflectNdx++)
		{
			try
			{
#ifdef	_DEBUG_INTERNAL
				LogMessage (_DEBUG_INTERNAL, _T("[%p] _OnCharacterNameChanged [%d]"), lReflect, pCharID);
#endif
				lReflect->_OnCharacterNameChanged (pCharID);
			}
			catch AnyExceptionDebug
		}
	}
	catch AnyExceptionDebug
}

void CEventNotify::_CharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID)
{
	try
	{
		INT_PTR				lReflectNdx;
		_IEventReflect *	lReflect;

		for	(lReflectNdx = 0; lReflect = mEventReflect (lReflectNdx); lReflectNdx++)
		{
			try
			{
#ifdef	_DEBUG_INTERNAL
				LogMessage (_DEBUG_INTERNAL, _T("[%p] _OnCharacterActivated [%d] [%d] [%d] [%d]"), lReflect, pActiveCharID, pInputActiveCharID, pInactiveCharID, pInputInactiveCharID);
#endif
				lReflect->_OnCharacterActivated (pActiveCharID, pInputActiveCharID, pInactiveCharID, pInputInactiveCharID);
			}
			catch AnyExceptionDebug
		}
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////

void CEventNotify::_CharacterListening (long pCharID, bool pListening, long pCause)
{
	long	lCharID;

	if	(lCharID = _GetNotifyClient (pCharID, false))
	{
		ListeningState (lCharID, (pListening!=false), pCause);
	}
}

bool CEventNotify::_DownloadComplete (class CFileDownload * pDownload)
{
	bool	lRet = false;

	try
	{
		INT_PTR				lReflectNdx;
		_IEventReflect *	lReflect;

		for	(lReflectNdx = 0; lReflect = mEventReflect (lReflectNdx); lReflectNdx++)
		{
			try
			{
#ifdef	_DEBUG_INTERNAL
				LogMessage (_DEBUG_INTERNAL, _T("[%p] _OnDownloadComplete"), lReflect);
#endif
				if	(lReflect->_OnDownloadComplete (pDownload))
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

class CFileDownload * CEventNotify::_FindSoundDownload (LPCTSTR pSoundUrl)
{
	class CFileDownload *	lRet = NULL;

	try
	{
		INT_PTR				lReflectNdx;
		_IEventReflect *	lReflect;

		for	(lReflectNdx = 0; lReflect = mEventReflect (lReflectNdx); lReflectNdx++)
		{
			try
			{
				if	(lRet = lReflect->_FindSoundDownload (pSoundUrl))
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

bool CEventNotify::_ContextMenu (long pCharID, HWND pOwner, const CPoint & pPosition)
{
	bool	lRet = false;

	try
	{
		INT_PTR				lReflectNdx;
		_IEventReflect *	lReflect;

		for	(lReflectNdx = 0; lReflect = mEventReflect (lReflectNdx); lReflectNdx++)
		{
			try
			{
				if	(lReflect->_OnContextMenu (pCharID, pOwner, pPosition))
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

bool CEventNotify::_DefaultCommand (long pCharID, HWND pOwner, const CPoint & pPosition)
{
	bool	lRet = false;

	try
	{
		INT_PTR				lReflectNdx;
		_IEventReflect *	lReflect;

		for	(lReflectNdx = 0; lReflect = mEventReflect (lReflectNdx); lReflectNdx++)
		{
			try
			{
				if	(lReflect->_OnDefaultCommand (pCharID, pOwner, pPosition))
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

void CEventNotify::_OptionsChanged ()
{
	try
	{
		INT_PTR				lReflectNdx;
		_IEventReflect *	lReflect;

		for	(lReflectNdx = 0; lReflect = mEventReflect (lReflectNdx); lReflectNdx++)
		{
			try
			{
#ifdef	_DEBUG_INTERNAL
				LogMessage (_DEBUG_INTERNAL, _T("[%p] _OnOptionsChanged"), lReflect);
#endif
				lReflect->_OnOptionsChanged ();
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

void CEventNotify::_DefaultCharacterChanged (REFGUID pCharGuid)
{
	try
	{
		INT_PTR				lReflectNdx;
		_IEventReflect *	lReflect;

		for	(lReflectNdx = 0; lReflect = mEventReflect (lReflectNdx); lReflectNdx++)
		{
			try
			{
#ifdef	_DEBUG_INTERNAL
				LogMessage (_DEBUG_INTERNAL, _T("[%p] _OnDefaultCharacterChanged"), lReflect);
#endif
				lReflect->_OnDefaultCharacterChanged ();
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
