/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software:
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
#include "AgentAnchor.h"
#include "AgentFiles.h"
#include "DaCmnCharacter.h"
#include "GuidStr.h"
#include "Registry.h"

#ifdef	_DEBUG
#define	_DEBUG_NOTIFY_PATH		(GetProfileDebugInt(_T("DebugNotifyPath"),LogVerbose,true)&0xFFFF)
#define	_DEBUG_DEFAULT_CHAR		(GetProfileDebugInt(_T("DebugDefaultChar"),LogVerbose,true)&0xFFFF)
//#define	_DEBUG_INTERNAL		LogDebug
//#define	_DEBUG_ACTIVATE		LogNormal
#endif

/////////////////////////////////////////////////////////////////////////////

CEventNotify::CEventNotify ()
:	mAnchor (NULL),
	mGlobal (NULL)
{
}

CEventNotify::~CEventNotify ()
{
}

/////////////////////////////////////////////////////////////////////////////

void CEventNotify::RegisterEventReflect (_IEventReflect * pEventReflect, bool pRegister)
{
	try
	{
		if	(pEventReflect)
		{
#ifdef	_DEBUG_INTERNAL
			LogMessage (_DEBUG_INTERNAL, _T("[%p] RegisterEventReflect [%u]"), pEventReflect, pRegister);
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

void CEventNotify::RegisterEventLock (_IEventLock * pEventLock, bool pRegister)
{
	try
	{
		if	(pEventLock)
		{
#ifdef	_DEBUG_INTERNAL
			LogMessage (_DEBUG_INTERNAL, _T("[%p] RegisterEventLock [%u]"), pEventLock, pRegister);
#endif
			if	(pRegister)
			{
				mEventLock.AddUnique (pEventLock);
			}
			else
			{
				mEventLock.Remove (pEventLock);
			}
		}
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////

bool CEventNotify::PreFireEvent (LPCTSTR pEventName)
{
	if	(mEventLock.GetCount() > 0)
	{
		INT_PTR			lNdx;
		_IEventLock *	lEventLock;

		for	(lNdx = (INT_PTR)mEventLock.GetCount()-1; lNdx >= 0; lNdx--)
		{
			if	(lEventLock = mEventLock [lNdx])
			{
				try
				{
					lEventLock->_PreNotify ();
				}
				catch AnyExceptionSilent
			}
		}
	}
	return true;
}

bool CEventNotify::PostFireEvent (LPCTSTR pEventName)
{
	if	(mEventLock.GetCount() > 0)
	{
		INT_PTR			lNdx;
		_IEventLock *	lEventLock;

		for	(lNdx = (INT_PTR)mEventLock.GetCount()-1; lNdx >= 0; lNdx--)
		{
			if	(lEventLock = mEventLock [lNdx])
			{
				try
				{
					lEventLock->_PostNotify ();
				}
				catch AnyExceptionSilent
			}
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////

long CEventNotify::NextReqID ()
{
	return mAnchor->mAnchor.NextReqID ();
}

class CAgentWnd * CEventNotify::GetRequestOwner (long pReqID)
{
	return mAnchor->mAnchor.GetRequestOwner (pReqID);
}

class CAgentWnd * CEventNotify::GetAgentWnd (HWND pWindow)
{
	return mAnchor->mAnchor.GetAgentWnd (pWindow);
}

/////////////////////////////////////////////////////////////////////////////

long CEventNotify::GetActiveClient (long pCharID, bool pUseDefault)
{
	long	lRet = mAnchor->GetActiveClient (pCharID);

	if	(
			(lRet <= 0)
		&&	(pUseDefault)
		)
	{
		lRet = pCharID;
	}
	return lRet;
}

long CEventNotify::GetNotifyClient (long pCharID, bool pUseDefault)
{
	long	lRet = mAnchor->GetNotifyClient (pCharID);

	if	(
			(lRet <= 0)
		&&	(pUseDefault)
		)
	{
		lRet = pCharID;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CEventNotify::ActiveCharacterNotify (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID)
{
	bool	lRet = false;
	long	lActiveClientID = (pActiveCharID > 0) ? GetNotifyClient (pActiveCharID) : pActiveCharID-1;
	long	lInactiveClientID = (pInactiveCharID > 0) ? GetNotifyClient (pInactiveCharID) : pInactiveCharID-1;

#ifdef	_DEBUG_ACTIVATE
	LogMessage (_DEBUG_ACTIVATE, _T("[%p] ActiveCharacterNotify [%d] InputActive [%d] Inactive [%d] InputInactive [%d] (ActiveClient [%d] InactiveClient [%d])"), this, pActiveCharID, pInputActiveCharID, pInactiveCharID, pInputInactiveCharID, lActiveClientID, lInactiveClientID);
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

bool CEventNotify::ActiveCharacterChanged (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID)
{
#ifdef	_DEBUG_NOTIFY_PATH
	LogMessage (_DEBUG_NOTIFY_PATH, _T("CEventNotify::ActiveCharacterChanged [%d] {%d] [%d] [%d]"), pActiveCharID, pInputActiveCharID, pInactiveCharID, pInputInactiveCharID);
#endif
	try
	{
		mGlobal->_CharacterActivated (pActiveCharID, pInputActiveCharID, pInactiveCharID, pInputInactiveCharID);
	}
	catch AnyExceptionDebug

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
#ifdef	_DEBUG_NOTIFY_PATH
	LogMessage (_DEBUG_NOTIFY_PATH, _T("CEventNotify::_CharacterLoaded [%d]"), pCharID);
#endif
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
#ifdef	_DEBUG_NOTIFY_PATH
	LogMessage (_DEBUG_NOTIFY_PATH, _T("CEventNotify::_CharacterUnloaded [%d]"), pCharID);
#endif
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
#ifdef	_DEBUG_NOTIFY_PATH
	LogMessage (_DEBUG_NOTIFY_PATH, _T("CEventNotify::_CharacterNameChanged [%d]"), pCharID);
#endif
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
#ifdef	_DEBUG_NOTIFY_PATH
	LogMessage (_DEBUG_NOTIFY_PATH, _T("CEventNotify::_CharacterActivated [%d] {%d] [%d] [%d]"), pActiveCharID, pInputActiveCharID, pInactiveCharID, pInputInactiveCharID);
#endif
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

	if	(lCharID = GetNotifyClient (pCharID, false))
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

void CEventNotify::_AppActivated (bool pActive)
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
				LogMessage (_DEBUG_INTERNAL, _T("[%p] _OnAppActivated"), lReflect);
#endif
				lReflect->_OnAppActivated (pActive);
			}
			catch AnyExceptionDebug
		}
	}
	catch AnyExceptionDebug
}

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

void CEventNotify::_DefaultCharacterChanged (REFGUID pCharGuid, LPCTSTR pFilePath)
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
				lReflect->_OnDefaultCharacterChanged (pCharGuid, pFilePath);
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

CEventGlobal::CEventGlobal ()
{
}

CEventGlobal::~CEventGlobal ()
{
}

/////////////////////////////////////////////////////////////////////////////

void CEventGlobal::_CharacterLoaded (long pCharID)
{
#ifdef	_DEBUG_NOTIFY_PATH
	LogMessage (_DEBUG_NOTIFY_PATH, _T("CEventGlobal::_CharacterLoaded [%d]"), pCharID);
#endif
	try
	{
		INT_PTR			lNotifyNdx;
		CEventNotify *	lNotify;

		for	(lNotifyNdx = 0; lNotify = mInstanceNotify (lNotifyNdx); lNotifyNdx++)
		{
			lNotify->_CharacterLoaded (pCharID);
		}
	}
	catch AnyExceptionSilent
}

void CEventGlobal::_CharacterUnloaded (long pCharID)
{
#ifdef	_DEBUG_NOTIFY_PATH
	LogMessage (_DEBUG_NOTIFY_PATH, _T("CEventGlobal::_CharacterUnloaded [%d]"), pCharID);
#endif
	try
	{
		INT_PTR			lNotifyNdx;
		CEventNotify *	lNotify;

		for	(lNotifyNdx = 0; lNotify = mInstanceNotify (lNotifyNdx); lNotifyNdx++)
		{
			lNotify->_CharacterUnloaded (pCharID);
		}
	}
	catch AnyExceptionSilent
}

void CEventGlobal::_CharacterNameChanged (long pCharID)
{
#ifdef	_DEBUG_NOTIFY_PATH
	LogMessage (_DEBUG_NOTIFY_PATH, _T("CEventGlobal::_CharacterNameChanged [%d]"), pCharID);
#endif
	try
	{
		INT_PTR			lNotifyNdx;
		CEventNotify *	lNotify;

		for	(lNotifyNdx = 0; lNotify = mInstanceNotify (lNotifyNdx); lNotifyNdx++)
		{
			lNotify->_CharacterNameChanged (pCharID);
		}
	}
	catch AnyExceptionSilent
}

void CEventGlobal::_CharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID)
{
#ifdef	_DEBUG_NOTIFY_PATH
	LogMessage (_DEBUG_NOTIFY_PATH, _T("CEventGlobal::_CharacterActivated [%d] {%d] [%d] [%d]"), pActiveCharID, pInputActiveCharID, pInactiveCharID, pInputInactiveCharID);
#endif
	try
	{
		INT_PTR			lNotifyNdx;
		CEventNotify *	lNotify;

		for	(lNotifyNdx = 0; lNotify = mInstanceNotify (lNotifyNdx); lNotifyNdx++)
		{
			lNotify->_CharacterActivated (pActiveCharID, pInputActiveCharID, pInactiveCharID, pInputInactiveCharID);
		}
	}
	catch AnyExceptionSilent
}

void CEventGlobal::_CharacterListening (long pCharID, bool pListening, long pCause)
{
#ifdef	_DEBUG_NOTIFY_PATH
	LogMessage (_DEBUG_NOTIFY_PATH, _T("CEventGlobal::_CharacterListening [%d] [%d %d]"), pCharID, pListening, pCause);
#endif
//
//	This notification is slightly different in that it's sent to all of a character's
//	clients rather than just the active client.
//
	try
	{
		INT_PTR			lNotifyNdx;
		CEventNotify *	lNotify;

		for	(lNotifyNdx = 0; lNotify = mInstanceNotify (lNotifyNdx); lNotifyNdx++)
		{
			lNotify->_CharacterListening (pCharID, pListening, pCause);
		}
	}
	catch AnyExceptionSilent
}

/////////////////////////////////////////////////////////////////////////////

void CEventGlobal::_AppActivated (bool pActive)
{
	try
	{
		INT_PTR			lNotifyNdx;
		CEventNotify *	lNotify;

		for	(lNotifyNdx = 0; lNotify = mInstanceNotify (lNotifyNdx); lNotifyNdx++)
		{
			lNotify->_AppActivated (pActive);
		}
	}
	catch AnyExceptionDebug
}

void CEventGlobal::_OptionsChanged ()
{
	try
	{
		INT_PTR			lNotifyNdx;
		CEventNotify *	lNotify;

		for	(lNotifyNdx = 0; lNotify = mInstanceNotify (lNotifyNdx); lNotifyNdx++)
		{
			lNotify->_OptionsChanged ();
		}
	}
	catch AnyExceptionDebug
}

void CEventGlobal::_DefaultCharacterChanged ()
{
	try
	{
		INT_PTR				lNotifyNdx;
		CEventNotify *		lNotify;
		tPtr <CAgentFile>	lFile;
		CAtlString			lDefCharPath ((BSTR)CAgentFiles::GetDefCharPath());

		if	(
				(!lDefCharPath.IsEmpty ())
			&&	(lFile = CAgentFile::CreateInstance())
			&&	(SUCCEEDED (lFile->Open (lDefCharPath)))
			)
		{
#ifdef	_DEBUG_DEFAULT_CHAR
			LogMessage (_DEBUG_DEFAULT_CHAR, _T("CEventGlobal::_DefaultCharacterChanged [%s] [%s]"), CGuidStr::GuidName(lFile->GetGuid()), (BSTR)lFile->GetFileName());
#endif
			for	(lNotifyNdx = 0; lNotify = mInstanceNotify (lNotifyNdx); lNotifyNdx++)
			{
				if	(lNotify->mAnchor->GetDefaultCharacter ())
				{
#ifdef	_DEBUG_DEFAULT_CHAR
					LogMessage (_DEBUG_DEFAULT_CHAR, _T("[%p] CEventNotify::_DefaultCharacterChanged [%s] [%s]"), lNotify, CGuidStr::GuidName(lFile->GetGuid()), (BSTR)lFile->GetFileName());
#endif
					lNotify->_DefaultCharacterChanged (lFile->GetGuid(), lDefCharPath);
				}
#ifdef	_DEBUG_DEFAULT_CHAR
				else
				{
					LogMessage (_DEBUG_DEFAULT_CHAR, _T("[%p] CEventNotify::_DefaultCharacterChanged Skipped"), lNotify);
				}
#endif
			}
		}
	}
	catch AnyExceptionSilent
}
