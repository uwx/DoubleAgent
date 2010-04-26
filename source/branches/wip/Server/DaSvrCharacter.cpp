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
#include "DaSvrCharacter.h"
#include "DaSvrCharacter.h"
#include "DaSvrBalloon.h"
#include "DaSvrCommands.h"
#include "DaSvrAnimationNames.h"
#include "DaSvrTTSEngine.h"
#include "DaSvrTTSEngines.h"
#include "DaSvrSREngine.h"
#include "DaSvrSREngines.h"
#include "DaActionTrace.h"
#include "ServerNotify.h"
#include "AgentPopupWnd.h"
#include "AgentBalloonWnd.h"
#include "AgentListeningWnd.h"
#include "VoiceCommandsWnd.h"
#include "ListeningState.h"
#include "QueuedPrepare.h"
#include "FileDownload.h"
#include "SapiVoice.h"
#include "SapiVoiceCache.h"
#include "SapiInputCache.h"
#include "Sapi5Voices.h"
#include "Sapi5Inputs.h"
#include "Sapi5Input.h"
#ifndef	_WIN64
#include "Sapi4Voices.h"
#endif
#include "Registry.h"
#include "Localize.h"
#include "GuidStr.h"
#include "MallocPtr.h"
#include "DebugStr.h"
#ifdef	_DEBUG
#include "DebugProcess.h"
#endif

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Character"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_DEBUG_LANGUAGE			(GetProfileDebugInt(_T("DebugLanguage"),LogVerbose,true)&0xFFFF)
#define	_DEBUG_ACTIVE			(GetProfileDebugInt(_T("DebugActive"),LogVerbose,true)&0xFFFF|LogTimeMs)
#define	_DEBUG_PREPARE			(GetProfileDebugInt(_T("DebugPrepare"),LogVerbose,true)&0xFFFF|LogTimeMs)
#define	_DEBUG_LISTEN			(GetProfileDebugInt(_T("DebugListen"),LogVerbose,true)&0xFFFF|LogTimeMs)
#define	_DEBUG_REQUESTS			(GetProfileDebugInt(_T("DebugRequests"),LogVerbose,true)&0xFFFF|LogTimeMs)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Character"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

DaSvrCharacter::DaSvrCharacter ()
:	mCharID (0),
	mLangID (LANG_USER_DEFAULT),
	mFile (NULL),
	mUsedFileCache (NULL),
	mNotify (NULL),
	mWnd (NULL),
	mSapiVoice (NULL),
	mSapiInput (NULL),
	mIdleEnabled (true),
	mSoundEnabled (true),
	mAutoPopupMenu (true),
	mSvrBalloon (NULL),
	mSvrCommands (NULL),
	mInNotify (0)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%d] DaSvrCharacter::DaSvrCharacter (%d)"), this, m_dwRef, mCharID, _AtlModule.GetLockCount());
	}
#endif
}

DaSvrCharacter::~DaSvrCharacter()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%d] DaSvrCharacter::~DaSvrCharacter (%d)"), this, m_dwRef, mCharID, _AtlModule.GetLockCount());
	}
#endif
	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

DaSvrCharacter * DaSvrCharacter::CreateInstance (long pCharID, CAgentFileCache * pUsedFileCache, _IServerNotify * pNotify, LPCTSTR pClientMutexName)
{
	CComObject<DaSvrCharacter> *	lInstance = NULL;

	if	(SUCCEEDED (LogComErr (LogIfActive, CComObject<DaSvrCharacter>::CreateInstance (&lInstance))))
	{
		lInstance->mCharID = pCharID;
		lInstance->mUsedFileCache = pUsedFileCache;
		lInstance->mNotify = pNotify;
		lInstance->mNotify->_RegisterInternalNotify (lInstance, true);

#ifdef	_LOG_INSTANCE
		if	(LogIsActive (_LOG_INSTANCE))
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%d] DaSvrCharacter::CreateInstance (%d)"), lInstance, lInstance->m_dwRef, lInstance->mCharID, _AtlModule.GetLockCount());
		}
#endif

		if	(FAILED (lInstance->SetLangID (GetUserDefaultUILanguage ())))
		{
			lInstance->SetLangID (MAKELANGID (LANG_ENGLISH, SUBLANG_DEFAULT));
		}
#ifdef	_DEBUG_LANGUAGE
		if	(LogIsActive (_DEBUG_LANGUAGE))
		{
			LogMessage (_DEBUG_LANGUAGE, _T("[%p] [%d] DaSvrCharacter Default LangID [%4.4hX]"), lInstance, lInstance->mCharID, lInstance->mLangID);
		}
#endif
		lInstance->ManageObjectLifetime (lInstance, pClientMutexName);
	}
	return lInstance;
}

/////////////////////////////////////////////////////////////////////////////

void DaSvrCharacter::Terminate (bool pFinal, bool pAbandonned)
{
	if	(this)
	{
#ifdef	_LOG_INSTANCE
		if	(LogIsActive (_LOG_INSTANCE))
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%d] DaSvrCharacter::Terminate [%u %u] [%ls]"), this, m_dwRef, mCharID, pFinal, pAbandonned, (mFile ? (BSTR)mFile->GetPath() : NULL));
			if	(
					(mWnd)
				&&	(mWnd->IsWindow ())
				)
			{
				LogMessage (_LOG_INSTANCE, _T("[%p] [%d] [%u]   PopupWnd [%p(%d)] [%d]"), this, m_dwRef, mCharID, mWnd, mWnd->m_dwRef, mWnd->GetCharID());
			}
		}
#endif
		if	(
				(pFinal)
			&&	(m_dwRef > 0)
			)
		{
			if	(!pAbandonned)
			{
				try
				{
					CoDisconnectObject (GetUnknown(), 0);
				}
				catch AnyExceptionDebug
			}
			m_dwRef = 0;
		}

		try
		{
			if	(IsClientActive ())
			{
				SetClientActive (false, false);
			}
		}
		catch AnyExceptionDebug

		if	(
				(mFile)
			&&	(GetClientCount (mCharID) <= 0)
			)
		{
			try
			{
				if	(
						(mWnd)
					&&	(mWnd->GetBalloonWnd())
					)
				{
					_AtlModule.RemoveFileClient (mFile, mWnd->GetBalloonWnd());
				}
			}
			catch AnyExceptionDebug
			try
			{
				if	(mWnd)
				{
					_AtlModule.RemoveFileClient (mFile, mWnd);
				}
			}
			catch AnyExceptionDebug
		}

		if	(GetClientCount (mCharID) <= 0)
		{
			ReleaseSapiVoice ();
			ReleaseSapiInput ();
		}

		try
		{
			if	(mWnd)
			{
				mWnd->Detach (mCharID, mNotify);
			}
			mWnd = NULL;
		}
		catch AnyExceptionDebug

		if	(!mPrepares.IsEmpty ())
		{
			try
			{
				mPrepares.RemoveAll ();
			}
			catch AnyExceptionDebug
		}

		if	(mSvrCommands != NULL)
		{
			try
			{
				DaSvrCommands *	lCommands;

				if	(
						(pFinal)
					&&	(pAbandonned)
					&&	(lCommands = GetCommandsObj (false))
					)
				{
					lCommands->Terminate (pFinal, pAbandonned);
					try
					{
						delete lCommands;
					}
					catch AnyExceptionSilent
				}
				else
				if	(pFinal)
				{
					mSvrCommands->Release();
				}
			}
			catch AnyExceptionSilent

			if	(pFinal)
			{
				mSvrCommands = NULL;
			}
		}

		if	(mSvrBalloon != NULL)
		{
			try
			{
				DaSvrBalloon *	lBalloon;

				if	(
						(pFinal)
					&&	(pAbandonned)
					&&	(lBalloon = GetBalloonObj (false))
					)
				{
					lBalloon->Terminate (pFinal, pAbandonned);
					try
					{
						delete lBalloon;
					}
					catch AnyExceptionSilent
				}
				else
				if	(pFinal)
				{
					mSvrBalloon->Release ();
				}
			}
			catch AnyExceptionSilent

			if	(pFinal)
			{
				mSvrBalloon = NULL;
			}
		}

		SafeFreeSafePtr (mWndRefHolder);

		try
		{
			if	(mFile)
			{
				mNotify->_RegisterInternalNotify (this, false);
			}
		}
		catch AnyExceptionDebug
		try
		{
			if	(mFile)
			{
				mUsedFileCache->RemoveFileClient (mFile, this, false);
			}
		}
		catch AnyExceptionDebug
		try
		{
			if	(mFile)
			{
				_AtlModule.RemoveFileClient (mFile, this);
			}
		}
		catch AnyExceptionDebug

#ifdef	_LOG_INSTANCE
		if	(LogIsActive (_LOG_INSTANCE))
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%d] DaSvrCharacter::Terminate [%u %u] Done"), this, m_dwRef, mCharID, pFinal, pAbandonned);
		}
#endif
		mFile = NULL;

		if	(pFinal)
		{
			UnmanageObjectLifetime (this);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

void DaSvrCharacter::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%d] DaSvrCharacter::FinalRelease [%u]"), this, m_dwRef, mCharID, IsInNotify());
	}
#endif
	Terminate (false);
}

bool DaSvrCharacter::CanFinalRelease ()
{
	return (IsInNotify() == 0);
}

void DaSvrCharacter::OnClientEnded()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%d] DaSvrCharacter::OnClientEnded"), this, m_dwRef, mCharID);
	}
#endif
	Terminate (true, true);
	try
	{
		delete this;
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT WINAPI DaSvrCharacter::DelegateIDaSvrBalloon (void* pv, REFIID iid, LPVOID* ppvObject, DWORD_PTR dw)
{
	HRESULT				lResult = E_NOINTERFACE;
	DaSvrCharacter *	lThis = (DaSvrCharacter *) pv;
	DaSvrBalloon *		lBalloon = NULL;

	if	(
			(lThis->mSvrBalloon == NULL)
		&&	(lThis->mFile)
		&&	(lBalloon = DaSvrBalloon::CreateInstance (lThis->mCharID, lThis->mWnd, lThis->mFile, lThis->mNotify))
		)
	{
		lBalloon->SetLangID (lThis->mLangID);
		lThis->mSvrBalloon = lBalloon->GetControllingUnknown();
		lThis->mSvrBalloon->AddRef ();
	}
	if	(lThis->mSvrBalloon)
	{
		lResult = lThis->mSvrBalloon->QueryInterface (iid, ppvObject);
	}
	return lResult;
}

HRESULT WINAPI DaSvrCharacter::DelegateIDaSvrCommands (void* pv, REFIID iid, LPVOID* ppvObject, DWORD_PTR dw)
{
	HRESULT				lResult = E_NOINTERFACE;
	DaSvrCharacter *	lThis = (DaSvrCharacter *) pv;
	DaSvrCommands *		lCommands = NULL;

	if	(
			(lThis->mSvrCommands == NULL)
		&&	(lCommands = DaSvrCommands::CreateInstance (lThis->mCharID, lThis->mNotify))
		)
	{
		lCommands->SetLangID (lThis->mLangID);
		lThis->mSvrCommands = lCommands->GetControllingUnknown();
		lThis->mSvrCommands->AddRef ();
	}
	if	(lThis->mSvrCommands)
	{
		lResult = lThis->mSvrCommands->QueryInterface (iid, ppvObject);
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool DaSvrCharacter::PreNotify ()
{
	if	(
			(this)
		&&	(m_dwRef > 0)
		)
	{
		mInNotify++;
		return true;
	}
	return false;
}

bool DaSvrCharacter::PostNotify ()
{
	if	(this)
	{
		if	(mInNotify > 0)
		{
			mInNotify--;
		}
		if	(
				(CanFinalRelease ())
			&&	(HasFinalReleased ())
			)
		{
#ifdef	_LOG_INSTANCE
			if	(LogIsActive (_LOG_INSTANCE))
			{
				LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%d] DaServer PostNotify -> Release"), this, m_dwRef, mCharID);
			}
#endif
			m_dwRef = 1;
			Release ();
			return false;
		}
		return true;
	}
	return false;
}

UINT DaSvrCharacter::IsInNotify () const
{
	if	(this)
	{
		return mInNotify;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

BSTR DaSvrCharacter::GetName () const
{
	CAgentFileName *	lFileName;

	if	(
			(mFile)
		&&	(lFileName = mFile->FindName (mLangID))
		)
	{
		return lFileName->mName;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT DaSvrCharacter::OpenFile (CAgentFile * pFile, DWORD pInitialStyle)
{
	HRESULT								lResult = S_OK;
	CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
	INT_PTR								lClientNdx;

	if	(mFile)
	{
		lResult = E_UNEXPECTED;
	}
	else
	if	(mFile = pFile)
	{
#ifdef	_LOG_INSTANCE
		if	(LogIsActive (_LOG_INSTANCE))
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%d] DaSvrCharacter::OpenFile [%s]"), this, m_dwRef, mCharID, CAtlString(pFile->GetPath()));
		}
#endif
		if	(!mUsedFileCache->AddFileClient (mFile, this))
		{
			mUsedFileCache->CacheFile (mFile, this);
		}
		if	(!_AtlModule.AddFileClient (mFile, this))
		{
			_AtlModule.CacheFile (mFile, this);
		}
	}
	else
	{
		lResult = E_INVALIDARG;
	}

	if	(
			(SUCCEEDED (lResult))
		&&	(_AtlModule.GetFileClients (mFile, lFileClients))
		)
	{
		for	(lClientNdx = 0; lClientNdx < (INT_PTR)lFileClients.GetCount(); lClientNdx++)
		{
			if	(
					(mWnd = dynamic_cast <CAgentPopupWnd *> (lFileClients [lClientNdx]))
				&&	(mWnd->IsWindow ())
				)
			{
				break;
			}
			mWnd = NULL;
		}
	}

	if	(
			(SUCCEEDED (lResult))
		&&	(
				(!mWnd)
			||	(!mWnd->IsWindow ())
			)
		)
	{
		if	(mWnd)
		{
			delete mWnd;
		}
		if	(mWnd = CAgentPopupWnd::CreateInstance())
		{
			SetStyle (~pInitialStyle, pInitialStyle);

			if	(mWnd->Create (NULL))
			{
				mWnd->ModifyStyle (WS_CAPTION|WS_THICKFRAME|WS_SYSMENU, 0, SWP_FRAMECHANGED);
				mWnd->ModifyStyleEx (0, WS_EX_TOOLWINDOW);

				if	(mWnd->Open (mFile))
				{
					mWnd->EnableIdle (mIdleEnabled);
					mWnd->EnableSound (mSoundEnabled);
				}
				else
				{
					lResult = E_FAIL;
				}
			}
			else
			{
				lResult = E_FAIL;
			}
			if	(FAILED (lResult))
			{
				delete mWnd;
				mWnd = NULL;
			}
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
	}

	if	(
			(SUCCEEDED (lResult))
		&&	(mWnd)
		&&	(mWnd->IsWindow ())
		)
	{
		SetStyle (~pInitialStyle, pInitialStyle);
		_AtlModule.AddFileClient (mFile, mWnd);
		mWndRefHolder = mWnd->GetControllingUnknown();
		if	(GetActiveClient () <= 0)
		{
			SetClientActive (true, false);
		}
		else
		{
			mWnd->Attach (mCharID, mNotify, &mIconData, false);
		}
	}
	else
	if	(SUCCEEDED (lResult))
	{
		lResult = E_FAIL;
	}

	if	(SUCCEEDED (lResult))
	{
		ShowIcon (mIconData.mShowIcon);
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool DaSvrCharacter::IsVisible (bool pOrIsShowing) const
{
	bool	lRet = false;

	if	(
			(mWnd)
		&&	(mWnd->IsWindow ())
		&&	(
				(mWnd->GetStyle() & WS_VISIBLE)
			||	(
					(pOrIsShowing)
				&&	(mWnd->IsShowingQueued ())
				)
			)
		)
	{
		lRet = true;
	}
	return lRet;
}

bool DaSvrCharacter::IsShowing () const
{
	bool	lRet = false;

	if	(
			(mWnd)
		&&	(mWnd->IsWindow ())
		&&	(
				(mWnd->IsShowingQueued ())
			||	(mWnd->IsShowingState (_T("SHOWING")))
			)
		)
	{
		lRet = true;
	}
	return lRet;
}

bool DaSvrCharacter::IsHiding () const
{
	bool	lRet = false;

	if	(
			(mWnd)
		&&	(mWnd->IsWindow ())
		&&	(
				(mWnd->IsHidingQueued ())
			||	(mWnd->IsShowingState (_T("HIDING")))
			)
		)
	{
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

long DaSvrCharacter::Show (bool pFast, bool pImmediate)
{
	long	lReqID = 0;

	if	(
			(mWnd)
		&&	(mWnd->IsWindow ())
		)
	{
		if	(pImmediate)
		{
			mWnd->ShowPopup (mCharID, VisibilityCause_ProgramShowed);
		}
		else
		{
			lReqID = mWnd->QueueShow (mCharID, pFast);
			mWnd->ActivateQueue (true);
		}
	}
	return lReqID;
}

long DaSvrCharacter::Hide (bool pFast, bool pImmediate)
{
	long	lReqID = 0;

	StopListening (false, ListenComplete_CharacterClientDeactivated);

	if	(
			(mWnd)
		&&	(mWnd->IsWindow ())
		)
	{
		if	(pImmediate)
		{
			mWnd->HidePopup (mCharID, VisibilityCause_ProgramHid);
		}
		else
		{
			lReqID = mWnd->QueueHide (mCharID, pFast);
			mWnd->ActivateQueue (true);
		}
	}
	return lReqID;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool DaSvrCharacter::IsInputActive () const
{
	bool	lRet = false;

	if	(
			(mWnd)
		&&	(mWnd->IsWindow ())
		&&	(mWnd->GetCharID () == GetCharID())
		&&	(mWnd->GetLastActive () == mWnd->m_hWnd)
		)
	{
		lRet = true;
	}
	return lRet;
}

bool DaSvrCharacter::IsClientActive () const
{
	bool	lRet = false;

	if	(
			(mWnd)
		&&	(mWnd->IsWindow ())
		&&	(mWnd->GetCharID() == GetCharID())
		)
	{
		lRet = true;
	}
	return lRet;
}

long DaSvrCharacter::GetActiveClient () const
{
	long	lRet = 0;

	if	(
			(mWnd)
		&&	(mWnd->IsWindow ())
		)
	{
		lRet = mWnd->GetCharID ();
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool DaSvrCharacter::SetClientActive (bool pActive, bool pInputActive)
{
	bool				lRet = false;
	long				lPrevCharId = 0;
	DaSvrCharacter *	lNextCharacter = NULL;
	CAgentBalloonWnd *	lBalloonWnd = NULL;

#ifdef	_DEBUG_ACTIVE
	if	(LogIsActive (_DEBUG_ACTIVE))
	{
		LogMessage (_DEBUG_ACTIVE, _T("[%d] SetClientActive [%u] InputActive [%u] - IsVisible [%u] IsClientActive [%u] IsInputActive [%u] IsListening [%u] - ClientActive [%d] InputActive [%d] Listening [%d]"), mCharID, pActive, pInputActive, IsVisible(), IsClientActive(), IsInputActive(), IsListening(), GetActiveClient(), mNotify->_GetActiveCharacter(), (_AtlModule.GetListenCharacter()?_AtlModule.GetListenCharacter()->GetCharID():0));
	}
#endif
	if	(
			(mWnd)
		&&	(mWnd->IsWindow ())
		)
	{
		lPrevCharId = mWnd->GetCharID ();
	}

	if	(
			(pActive)
		&&	(lPrevCharId != mCharID)
		)
	{
		if	(
				(mWnd)
			&&	(mWnd->IsWindow ())
			&&	(mWnd->Attach (mCharID, mNotify, &mIconData, true))
			)
		{
			if	(lBalloonWnd = GetBalloonWnd (false))
			{
				lBalloonWnd->SetOptions (mFile->GetBalloon(), GetBalloonInterface (false), mLangID);
			}
			lNextCharacter = this;
			lRet = true;
		}
	}
	else
	if	(
			(!pActive)
		&&	(lPrevCharId == mCharID)
		)
	{
		try
		{
			CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
			INT_PTR								lClientNdx;

			if	(
					(mFile)
				&&	(_AtlModule.GetFileClients (mFile, lFileClients))
				)
			{
				for	(lClientNdx = 0; lClientNdx < (INT_PTR)lFileClients.GetCount(); lClientNdx++)
				{
					if	(
							(lNextCharacter = dynamic_cast <DaSvrCharacter *> (lFileClients [lClientNdx]))
						&&	(lNextCharacter->GetCharID() != mCharID)
						)
					{
						break;
					}
					lNextCharacter = NULL;
				}
			}

			if	(
					(lNextCharacter)
				&&	(lNextCharacter->mWnd == mWnd)
				&&	(mWnd->Attach (lNextCharacter->mCharID, lNextCharacter->mNotify, &lNextCharacter->mIconData, true))
				)
			{
				if	(lBalloonWnd = lNextCharacter->GetBalloonWnd (false))
				{
					lBalloonWnd->SetOptions (lNextCharacter->mFile->GetBalloon(), lNextCharacter->GetBalloonInterface (false), lNextCharacter->mLangID);
				}
				lRet = true;
			}
		}
		catch AnyExceptionDebug
	}

	if	(
			(pActive)
		&&	(pInputActive)
		&&	(mWnd)
		&&	(mWnd->IsWindow ())
		&&	(mWnd->GetStyle() & WS_VISIBLE)
		)
	{
		mWnd->SetLastActive (mWnd->m_hWnd);
	}
#ifdef	_DEBUG_ACTIVE
	if	(LogIsActive (_DEBUG_ACTIVE))
	{
		LogMessage (_DEBUG_ACTIVE, _T("[%d] %u  ClientActive [%u] InputActive [%u] - IsVisible [%u] IsClientActive [%u] IsInputActive [%u] IsListening [%u] - ClientActive [%d] InputActive [%d] Listening [%d]"), mCharID, lRet, pActive, pInputActive, IsVisible(), IsClientActive(), IsInputActive(), IsListening(), mNotify->_GetActiveClient(mCharID), mNotify->_GetActiveCharacter(), (_AtlModule.GetListenCharacter()?_AtlModule.GetListenCharacter()->GetCharID():0));
	}
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

INT_PTR DaSvrCharacter::GetClientCount (int pSkipCharID) const
{
	INT_PTR	lRet = -1;

	try
	{
		CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
		INT_PTR								lClientNdx;
		DaSvrCharacter *					lCharacter;

		if	(
				(mFile)
			&&	(_AtlModule.GetFileClients (mFile, lFileClients))
			)
		{
			lRet = 0;

			for	(lClientNdx = 0; lClientNdx < (INT_PTR)lFileClients.GetCount(); lClientNdx++)
			{
				if	(
						(lCharacter = dynamic_cast <DaSvrCharacter *> (lFileClients [lClientNdx]))
					&&	(
							(pSkipCharID <= 0)
						||	(lCharacter->GetCharID() != pSkipCharID)
						)
					)
				{
					lRet++;
				}
			}
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT DaSvrCharacter::SetStyle (DWORD pRemoveStyle, DWORD pAddStyle)
{
	HRESULT	lResult = S_FALSE;

	if	(
			(pRemoveStyle & CharacterStyle_IdleEnabled)
		||	(pAddStyle & CharacterStyle_IdleEnabled)
		)
	{
		if	(pRemoveStyle & CharacterStyle_IdleEnabled)
		{
			mIdleEnabled = false;
		}
		if	(pAddStyle & CharacterStyle_IdleEnabled)
		{
			mIdleEnabled = true;
		}
		if	(
				(mWnd)
			&&	(mWnd->IsWindow ())
			&&	(mWnd->GetCharID() == mCharID)
			)
		{
			mWnd->EnableIdle (mIdleEnabled);
			lResult = S_OK;
		}
	}

	if	(
			(pRemoveStyle & CharacterStyle_SoundEffects)
		||	(pAddStyle & CharacterStyle_SoundEffects)
		)
	{
		if	(pRemoveStyle & CharacterStyle_SoundEffects)
		{
			mSoundEnabled = false;
		}
		if	(pAddStyle & CharacterStyle_SoundEffects)
		{
			mSoundEnabled = true;
		}
		if	(
				(mWnd)
			&&	(mWnd->IsWindow ())
			&&	(mWnd->GetCharID() == mCharID)
			)
		{
			mWnd->EnableSound (mSoundEnabled);
			lResult = S_OK;
		}
	}

	if	(
			(pRemoveStyle & CharacterStyle_AutoPopupMenu)
		||	(pAddStyle & CharacterStyle_AutoPopupMenu)
		)
	{
		if	(pRemoveStyle & CharacterStyle_AutoPopupMenu)
		{
			if	(mAutoPopupMenu)
			{
				lResult = S_OK;
			}
			mAutoPopupMenu = false;
		}
		if	(pAddStyle & CharacterStyle_AutoPopupMenu)
		{
			if	(!mAutoPopupMenu)
			{
				lResult = S_OK;
			}
			mAutoPopupMenu = true;
		}
	}

	if	(
			(pRemoveStyle & CharacterStyle_IconShown)
		||	(pAddStyle & CharacterStyle_IconShown)
		)
	{
		if	(pRemoveStyle & CharacterStyle_IconShown)
		{
			mIconData.mShowIcon = false;
		}
		if	(pAddStyle & CharacterStyle_IconShown)
		{
			mIconData.mShowIcon = true;
		}

		if	(
				(mWnd)
			&&	(mWnd->IsWindow ())
			&&	(mWnd->GetCharID() == GetCharID())
			)
		{
			mWnd->UpdateNotifyIcon (&mIconData);
			lResult = S_OK;
		}
	}

	if	(
			(mWnd)
		&&	(!mWnd->m_hWnd)
		)
	{
		if	((pAddStyle & CharacterStyle_Smoothed) == CharacterStyle_Smoothed)
		{
			mWnd->mAlphaSmoothing = RenderSmoothAll;
		}
		else
		if	((pAddStyle & CharacterStyle_SmoothEdges) == CharacterStyle_SmoothEdges)
		{
			mWnd->mAlphaSmoothing = RenderSmoothEdges;
		}
		else
		{
			mWnd->mAlphaSmoothing = 0;
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT DaSvrCharacter::SetLangID (LANGID pLangID)
{
	HRESULT	lResult = S_OK;
	LANGID	lLangID;

	if	(pLangID == LANG_USER_DEFAULT)
	{
#ifdef	_DEBUG
		pLangID = LANGIDFROMLCID (GetThreadLocale ());
#else
		pLangID = GetUserDefaultUILanguage();
#endif
	}
	else
	if	(pLangID == LANG_SYSTEM_DEFAULT)
	{
		pLangID = GetSystemDefaultUILanguage();
	}

	lLangID = pLangID;

	if	(
			(lLangID == 0)
		||	(
				(!::IsValidLocale (MAKELCID (lLangID, SORT_DEFAULT), LCID_SUPPORTED))
#ifdef	_DEBUG
			&&	(GetProfileDebugInt(_T("DebugAnyLanguage")) <= 0)
#endif
			)
		)
	{
#ifdef	_DEBUG_LANGUAGE
		if	(LogIsActive (_DEBUG_LANGUAGE))
		{
			LogMessage (_DEBUG_LANGUAGE, _T("[%p] [%d] DaSvrCharacter LangID [%4.4hX] not supported"), this, mCharID, pLangID);
		}
#endif
		lResult = E_INVALIDARG;
	}
	else
	if	(
			(PRIMARYLANGID (lLangID) == LANG_ENGLISH)
		||	(CLocalize::GetMuiModule (lLangID))
		||	(CLocalize::GetMuiModule (lLangID = MAKELANGID (PRIMARYLANGID (lLangID), SUBLANG_DEFAULT)))
		||	(CLocalize::GetMuiModule (lLangID = MAKELANGID (PRIMARYLANGID (lLangID), SUBLANG_NEUTRAL)))
		)
	{
#ifdef	_DEBUG_LANGUAGE
		if	(LogIsActive (_DEBUG_LANGUAGE))
		{
			LogMessage (_DEBUG_LANGUAGE, _T("[%p] [%d] DaSvrCharacter LangID from [%4.4hX] to [%4.4hX]"), this, mCharID, mLangID, lLangID);
		}
#endif
		if	(
				(mWnd)
			&&	(mWnd->GetCharID() == mCharID)
			)
		{
			mWnd->RemoveQueuedSpeak (mCharID, AGENTREQERR_INTERRUPTEDCODE, _T("SetLangID"));
			mWnd->RemoveQueuedThink (mCharID, AGENTREQERR_INTERRUPTEDCODE, _T("SetLangID"));
		}
		ReleaseSapiVoice ();
		ReleaseSapiInput ();

		mLangID = lLangID;
		PropagateLangID ();
	}
	else
	{
#ifdef	_DEBUG_LANGUAGE
		if	(LogIsActive (_DEBUG_LANGUAGE))
		{
			LogMessage (_DEBUG_LANGUAGE, _T("[%p] [%d] DaSvrCharacter LangID [%4.4hX] not installed"), this, mCharID, pLangID);
		}
#endif
		lResult = AGENTERR_LANGUAGENOTFOUND;
	}
	return lResult;
}

void DaSvrCharacter::PropagateLangID ()
{
	if	(mFile)
	{
		try
		{
			DaSvrBalloon *			lBalloon = NULL;
			DaSvrCommands *			lCommands = NULL;
			CAgentBalloonWnd *		lBalloonWnd;
			CVoiceCommandsWnd *		lVoiceCommandsWnd;
			CAgentListeningWnd *	lListeningWnd;

			if	(lCommands = GetCommandsObj (false))
			{
				lCommands->SetLangID (mLangID);
			}

			if	(
					(lVoiceCommandsWnd = _AtlModule.GetVoiceCommandsWnd (false))
				&&	(lVoiceCommandsWnd->IsWindow ())
				&&	(lVoiceCommandsWnd->GetCharID () == mCharID)
				&&	(lVoiceCommandsWnd->GetLangID () != mLangID)
				)
			{
				lVoiceCommandsWnd->SetLangID (mLangID);

				if	(lCommands)
				{
					lCommands->ShowVoiceCommands (lVoiceCommandsWnd);
				}
				else
				{
					lVoiceCommandsWnd->ShowGlobalCommands ();
				}
				lVoiceCommandsWnd->ShowOtherClients (mCharID);
			}

			if	(
					(lListeningWnd = GetListeningWnd (false))
				&&	(lListeningWnd->GetCharID () == mCharID)
				&&	(lListeningWnd->GetLangID () != mLangID)
				)
			{
				lListeningWnd->SetLangID (mLangID);
			}

			if	(
					(mWnd)
				&&	(mWnd->GetCharID() == mCharID)
				)
			{
				mWnd->SetNotifyIconTip (&mIconData, mFile, mLangID);
			}

			if	(lBalloon = GetBalloonObj (false))
			{
				lBalloon->SetLangID (mLangID);
			}

			if	(
					(lBalloonWnd = GetBalloonWnd (false))
				&&	(lBalloonWnd->GetCharID() == mCharID)
				&&	(lBalloonWnd->GetLangID() != mLangID)
				)
			{
				lBalloonWnd->SetOptions (mFile->GetBalloon(), GetBalloonInterface (false), mLangID);
			}
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool DaSvrCharacter::IsIdleEnabled () const
{
	if	(
			(mWnd)
		&&	(mWnd->IsWindow ())
		)
	{
		return mWnd->IsIdleEnabled ();
	}
	return mIdleEnabled;
}

bool DaSvrCharacter::IsSoundEnabled (bool pIgnoreGlobalConfig) const
{
	if	(
			(mWnd)
		&&	(mWnd->IsWindow ())
		)
	{
		return mWnd->IsSoundEnabled (pIgnoreGlobalConfig);
	}
	return	(
				(mSoundEnabled)
			&&	(
					(pIgnoreGlobalConfig)
				||	(CDaSettingsConfig().LoadConfig().mEffectsEnabled)
				)
			);
}

bool DaSvrCharacter::IsAutoPopupMenu () const
{
	return mAutoPopupMenu;
}

/////////////////////////////////////////////////////////////////////////////

bool DaSvrCharacter::IsIconShown () const
{
	return mIconData.mShowIcon;
}

bool DaSvrCharacter::IsIconVisible () const
{
	if	(
			(mWnd)
		&&	(mWnd->IsWindow ())
		&&	(mWnd->IsIconVisible ())
		)
	{
		return true;
	}
	return false;
}

bool DaSvrCharacter::ShowIcon (bool pShow)
{
	bool	lRet = false;

	if	(mIconData.mShowIcon != pShow)
	{
		mIconData.mShowIcon = pShow;
		lRet = true;
	}
	if	(
			(mWnd)
		&&	(mWnd->IsWindow() )
		&&	(mWnd->GetCharID() == GetCharID())
		)
	{
		mWnd->UpdateNotifyIcon (&mIconData);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool DaSvrCharacter::IsSpeaking () const
{
	bool			lRet = false;
	CSapiVoice *	lVoice;

//
//	Note that if the voice is shared by multiple characters, we don't check
//	which character is speaking.  That's ok because this method is used
//	to check if ANY character is speaking, so it doesn't matter.
//
	if	(
			(lVoice = const_cast <DaSvrCharacter *> (this)->GetSapiVoice (false))
		&&	(lVoice->SafeIsSpeaking ())
		)
	{
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

CSapiVoice * DaSvrCharacter::GetSapiVoice (bool pCreateObject, LPCTSTR pVoiceName)
{
	if	(
			(!mSapiVoice)
		&&	(pCreateObject)
		&&	(mFile)
		)
	{
		try
		{
			CSapiVoiceCache *	lVoiceCache;
			CString				lVoiceName (pVoiceName);

			if	(lVoiceCache = CSapiVoiceCache::GetStaticInstance ())
			{
				if	(lVoiceName.IsEmpty ())
				{
					CAgentFileTts	lFileTts;

					lFileTts = mFile->GetTts ();
					lFileTts.mLanguage = mLangID;
					mSapiVoice = lVoiceCache->GetAgentVoice (lFileTts, true);
				}
				else
				{
					mSapiVoice = lVoiceCache->GetAgentVoice (lVoiceName, mLangID, true);
				}

				if	(mSapiVoice)
				{
					if	(!mSapiVoice->SafeIsPrepared ())
					{
						mSapiVoice->PrepareToSpeak ();
					}
					if	(!lVoiceCache->CacheVoice (mSapiVoice, this))
					{
						lVoiceCache->AddVoiceClient (mSapiVoice, this);
					}
				}
			}
		}
		catch AnyExceptionDebug
	}
	return mSapiVoice;
}

void DaSvrCharacter::ReleaseSapiVoice ()
{
	try
	{
		CSapiVoiceCache *	lVoiceCache;

		if	(
				(mSapiVoice)
			&&	(lVoiceCache = CSapiVoiceCache::GetStaticInstance ())
			)
		{
			lVoiceCache->RemoveVoiceClient (mSapiVoice, this);
		}
	}
	catch AnyExceptionDebug

	mSapiVoice = NULL;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool DaSvrCharacter::IsListening () const
{
	bool	lRet = false;

	if	(
			(mListeningState)
		&&	(mListeningState->IsListening ())
		)
	{
		lRet = true;
	}
	return lRet;
}

bool DaSvrCharacter::IsHearing () const
{
	bool	lRet = false;

	if	(
			(mListeningState)
		&&	(mListeningState->IsHearing ())
		)
	{
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT DaSvrCharacter::StartListening (bool pManual)
{
	HRESULT	lResult = S_OK;

	try
	{
#ifdef	_DEBUG_LISTEN
		if	(
				(LogIsActive (_DEBUG_LISTEN))
			&&	(
					(pManual)
				||	(!mListeningState)
				||	(!mListeningState->IsAutomatic ())
				)
			)
		{
			LogMessage (_DEBUG_LISTEN, _T("[%d] StartListening Manual [%u] Listening [%u] Active [%u] Enabled [%u]"), mCharID, pManual, IsListening(), (mNotify->_GetActiveCharacter()==GetCharID()), CDaSettingsConfig().LoadConfig().mSrEnabled);
		}
#endif

		if	(
				(!pManual)
			&&	(mListeningState)
			&&	(mListeningState->IsAutomatic ())
			)
		{
			lResult = mListeningState->KeepListening (pManual);
		}
		else
		if	(
				(!mWnd)
			||	(!mWnd->IsWindow ())
			)
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
		else
		if	(
				(pManual)
			?	(mNotify->_GetActiveCharacter() != GetCharID())
			:	(!IsClientActive ())
			)
		{
			lResult = AGENTERR_CHARACTERNOTACTIVE;
		}
		else
		if	(
				(mListeningState)
			||	(mListeningState = new CListeningState (*this))
			)
		{
			lResult = mListeningState->StartListening (pManual);
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT DaSvrCharacter::StopListening (bool pManual, long pCause)
{
	HRESULT	lResult = S_FALSE;

	try
	{
		if	(mListeningState)
		{
#ifdef	_DEBUG_LISTEN
			if	(LogIsActive (_DEBUG_LISTEN))
			{
				LogMessage (_DEBUG_LISTEN, _T("[%d] StopListening Manual [%u] Listening [%u] Active [%u]"), mCharID, pManual, IsListening(), (mNotify->_GetActiveCharacter()==GetCharID()));
			}
#endif
			lResult = mListeningState->StopListening (pManual, pCause);
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void DaSvrCharacter::TransferListeningState (DaSvrCharacter * pOtherCharacter)
{
	if	(
			(pOtherCharacter)
		&&	(pOtherCharacter != this)
		&&	(IsListening ())
		&&	(pOtherCharacter->GetSapiInput (true))
		)
	{
#ifdef	_DEBUG_LISTEN
		if	(LogIsActive (_DEBUG_LISTEN))
		{
			LogMessage (_DEBUG_LISTEN, _T("[%d] TransferListening [%d]"), mCharID, pOtherCharacter->GetCharID());
		}
#endif
		if	(
				(pOtherCharacter->mListeningState)
			||	(pOtherCharacter->mListeningState = new CListeningState (*pOtherCharacter))
			)
		{
			mListeningState->TransferState (pOtherCharacter->mListeningState);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

bool DaSvrCharacter::ShowListeningState (bool pShow)
{
	bool	lRet = false;

#ifdef	_DEBUG_LISTEN
	if	(LogIsActive (_DEBUG_LISTEN))
	{
		LogMessage (_DEBUG_LISTEN, _T("[%d] ShowListeningState [%u] IsVisible [%u] IsShowing [%u] IsHiding {%u]"), mCharID, pShow, IsVisible(false), IsShowing(), IsHiding());
	}
#endif
	if	(
			(mWnd)
		&&	(mWnd->IsWindow ())
		)
	{
		if	(pShow)
		{
			if	(
					(IsVisible (false))
				&&	(!IsShowing ())
				&&	(!IsHiding ())
				)
			{
				StopAll (StopAll_Play|StopAll_Move|StopAll_Speak|StopAll_QueuedPrepare, AGENTREQERR_INTERRUPTEDLISTENKEY);
				if	(mWnd->QueueState (0, _T("LISTENING")))
				{
					mWnd->ActivateQueue (true);
					lRet = true;
				}
			}
		}
		else
		{
			if	(mWnd->RemoveQueuedState (0, _T("LISTENING")))
			{
				lRet = true;
			}
			if	(mWnd->IsShowingState (_T("LISTENING")))
			{
				mWnd->ShowGesture (NULL);
				lRet = true;
			}
			mWnd->ActivateQueue (true);
		}
	}
#ifdef	_DEBUG_LISTEN
	if	(LogIsActive (_DEBUG_LISTEN))
	{
		LogMessage (_DEBUG_LISTEN, _T("[%d] ShowListeningState [%u] Done [%u]"), mCharID, pShow, lRet);
	}
#endif
	return lRet;
}

bool DaSvrCharacter::ShowHearingState (bool pShow)
{
	bool	lRet = false;

#ifdef	_DEBUG_LISTEN
	if	(LogIsActive (_DEBUG_LISTEN))
	{
		LogMessage (_DEBUG_LISTEN, _T("[%d] ShowHearingState [%u] IsVisible [%u] IsShowing [%u] IsHiding {%u]"), mCharID, pShow, IsVisible(false), IsShowing(), IsHiding());
	}
#endif
	if	(
			(mWnd)
		&&	(mWnd->IsWindow ())
		)
	{
		if	(pShow)
		{
			if	(
					(IsVisible (false))
				&&	(!IsShowing ())
				&&	(!IsHiding ())
				)
			{
				StopAll (StopAll_Play|StopAll_Move|StopAll_Speak|StopAll_QueuedPrepare, AGENTREQERR_INTERRUPTEDHEARING);

				if	(mWnd->QueueState (0, _T("HEARING")))
				{
					mWnd->ActivateQueue (true);
					lRet = true;
				}
			}
		}
		else
		{
			if	(mWnd->RemoveQueuedState (0, _T("HEARING")))
			{
				lRet = true;
			}
			if	(mWnd->IsShowingState (_T("HEARING")))
			{
				mWnd->ShowGesture (NULL);
				lRet = true;
			}
			mWnd->ActivateQueue (true);
		}
	}
#ifdef	_DEBUG_LISTEN
	if	(LogIsActive (_DEBUG_LISTEN))
	{
		LogMessage (_DEBUG_LISTEN, _T("[%d] ShowHearingState [%u] Done [%u]"), mCharID, pShow, lRet);
	}
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CSapi5Input * DaSvrCharacter::GetSapiInput (bool pCreateObject, LPCTSTR pEngineName)
{
	if	(
			(!mSapiInput)
		&&	(pCreateObject)
		)
	{
		try
		{
			CSapiInputCache *	lInputCache;
			CString				lEngineName (pEngineName);

			if	(lInputCache = CSapiInputCache::GetStaticInstance ())
			{
				if	(lEngineName.IsEmpty ())
				{
					mSapiInput = lInputCache->GetAgentInput (mLangID, false);
				}
				else
				{
					mSapiInput = lInputCache->GetAgentInput (lEngineName, mLangID, false);
				}

				if	(mSapiInput)
				{
					if	(!mSapiInput->SafeIsPrepared ())
					{
						mSapiInput->PrepareToListen ();
					}
					if	(!lInputCache->CacheInput (mSapiInput, this))
					{
						lInputCache->AddInputClient (mSapiInput, this);
					}
				}
			}
		}
		catch AnyExceptionDebug
	}
	return mSapiInput;
}

void DaSvrCharacter::ReleaseSapiInput ()
{
	SafeFreeSafePtr (mListeningState);

	try
	{
		CSapiInputCache *	lInputCache;

		if	(
				(mSapiInput)
			&&	(lInputCache = CSapiInputCache::GetStaticInstance ())
			)
		{
			lInputCache->RemoveInputClient (mSapiInput, this);
		}
	}
	catch AnyExceptionDebug

	mSapiInput = NULL;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT DaSvrCharacter::StopAll (long pStopTypes, HRESULT pReqStatus)
{
	HRESULT	lResult = S_FALSE;
	bool	lExcludeActive = false;

	if	(pStopTypes & StopAll_Play)
	{
		if	(mWnd->ClearQueuedStates (mCharID, pReqStatus, _T("StopAll"), lExcludeActive, _T("SHOWING"), _T("HIDING"), NULL))
		{
			lResult = S_OK;
		}
		if	(mWnd->ClearQueuedGestures (mCharID, pReqStatus, _T("StopAll"), lExcludeActive, _T("SHOWING"), _T("HIDING"), NULL))
		{
			lResult = S_OK;
		}
	}

	if	(pStopTypes & StopAll_Speak)
	{
		if	(mWnd->RemoveQueuedSpeak (mCharID, pReqStatus, _T("StopAll"), lExcludeActive))
		{
			lResult = S_OK;
		}
		if	(mWnd->RemoveQueuedThink (mCharID, pReqStatus, _T("StopAll"), lExcludeActive))
		{
			lResult = S_OK;
		}
//
//	The balloon may still be visible even if there are no more Speak or Think actions queued.  Either the balloon
//	is not AUTOHIDE, or it's AUTOHIDE timer is set and it will disappear soon.  We could hide it immediately if
//	we wanted, but for now we won't.
//
//		CAgentBalloonWnd *	lBalloonWnd;
//		if	(
//				(lBalloonWnd = GetBalloonWnd (false))
//			&&	(lBalloonWnd->IsAutoHide ())
//			)
//		{
//			lBalloonWnd->HideBalloon (true);
//		}
	}

	if	(pStopTypes & StopAll_Move)
	{
		if	(mWnd->RemoveQueuedMove (mCharID, pReqStatus, _T("StopAll"), lExcludeActive))
		{
			lResult = S_OK;
		}
	}

	if	(pStopTypes & StopAll_Visibility)
	{
		if	(mWnd->RemoveQueuedShow (mCharID, pReqStatus, _T("StopAll"), lExcludeActive))
		{
			lResult = S_OK;
		}
		if	(mWnd->RemoveQueuedHide (mCharID, pReqStatus, _T("StopAll"), lExcludeActive))
		{
			lResult = S_OK;
		}
	}

	if	(pStopTypes & StopAll_QueuedPrepare)
	{
		if	(mWnd->RemoveQueuedPrepare (mCharID, pReqStatus, _T("StopAll"), lExcludeActive))
		{
			lResult = S_OK;
		}
	}

	if	(
			(pStopTypes & StopAll_ImmediatePrepare)
		&&	(!mPrepares.IsEmpty ())
		)
	{
		try
		{
			POSITION			lPos;
			long				lReqID;
			CQueuedPrepare *	lPrepare;

			for	(lPos = mPrepares.GetStartPosition(); lPos;)
			{
				mPrepares.GetNextAssoc (lPos, lReqID=0, lPrepare=NULL);

				if	(
						(lPrepare)
					&&	(!lPrepare->IsComplete ())
					)
				{
					try
					{
						lPrepare->CancelDownloads ();
					}
					catch AnyExceptionSilent

#ifdef	_DEBUG_REQUESTS
					if	(LogIsActive (_DEBUG_REQUESTS))
					{
						LogMessage (_DEBUG_REQUESTS, _T("[%d] RequestStart    [%d]"), mCharID, lReqID);
					}
#endif
					PreNotify ();
					try
					{
						mNotify->RequestComplete (lReqID, pReqStatus);
					}
					catch AnyExceptionSilent
					PostNotify ();
				}
			}
		}
		catch AnyExceptionDebug
	}

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT DaSvrCharacter::DoPrepare (long pType, LPCTSTR pName, bool pQueue, long & pReqID)
{
	HRESULT					lResult = E_FAIL;
	tPtr <CQueuedPrepare>	lPrepare;

#ifdef	_DEBUG_PREPARE
	if	(LogIsActive (_DEBUG_PREPARE))
	{
		LogMessage (_DEBUG_PREPARE, _T("[%p] [%d] DaSvrCharacter Prepare [%d %u] [%s]"), this, mCharID, pType, pQueue, pName);
	}
#endif

	if	(
			(mWnd)
		&&	(mFile)
		)
	{
		if	(
				(
					(pType == PrepareResource_Animation)
				||	(pType == PrepareResource_State)
				)
			&&	(!mFile->IsAcfFile ())
			)
		{
			lResult = S_FALSE;
		}
		else
		if	(
				(pType != PrepareResource_Animation)
			&&	(pType != PrepareResource_State)
			&&	(pType != PrepareResource_Wave)
			)
		{
			lResult = E_INVALIDARG;
		}
		else
		{
			pReqID = mNotify->NextReqID ();

			if	(!pQueue)
			{
#ifdef	_DEBUG_REQUESTS
				if	(LogIsActive (_DEBUG_REQUESTS))
				{
					LogMessage (_DEBUG_REQUESTS, _T("[%d] RequestStart    [%d]"), mCharID, pReqID);
				}
#endif
				PreNotify ();
				try
				{
					mNotify->RequestStart (pReqID);
				}
				catch AnyExceptionSilent
				PostNotify ();
			}

			if	(lPrepare = CQueuedPrepare::CreateInstance (mCharID, pReqID))
			{
				if	(pType == PrepareResource_Animation)
				{
					lResult = lPrepare->PutAnimationNames (mFile, pName, mNotify);
				}
				else
				if	(pType == PrepareResource_State)
				{
					lResult = lPrepare->PutStateNames (mFile, pName, mNotify);
				}
				else
				{
					lResult = lPrepare->PutSoundUrl (mFile, pName, mNotify);
				}

				if	(pQueue)
				{
					if	(SUCCEEDED (lResult))
					{
						mWnd->PutQueuedAction (lPrepare.Detach ());
					}
				}
				else
				{
					mPrepares.SetAt (pReqID, lPrepare);

					if	(lResult == S_OK)
					{
						lPrepare->StartDownloads ();
#ifdef	_DEBUG_PREPARE
						if	(LogIsActive (_DEBUG_PREPARE))
						{
							LogMessage (_DEBUG_PREPARE, _T("[%p] [%d] DaSvrCharacter Preparing [%p] [%d] [%s]"), this, mCharID, lPrepare.Ptr(), lPrepare->mReqID, (BSTR)lPrepare->GetAnimationNames());
						}
#endif
					}
					else
					{
						mPrepares.RemoveKey (pReqID);
					}
					lPrepare.Detach ();
				}
			}
			else
			{
				lResult = E_OUTOFMEMORY;
			}

			if	(
					(!pQueue)
				&&	(FAILED (lResult))
				)
			{
#ifdef	_DEBUG_REQUESTS
				if	(LogIsActive (_DEBUG_REQUESTS))
				{
					LogMessage (_DEBUG_REQUESTS, _T("[%d] RequestComplete [%d] [%8.8X]"), mCharID, pReqID, lResult);
				}
#endif
				PreNotify ();
				try
				{
					mNotify->RequestComplete (pReqID, lResult);
				}
				catch AnyExceptionSilent
				PostNotify ();
			}
		}
	}
	return lResult;
}

bool DaSvrCharacter::_OnDownloadComplete (CFileDownload * pDownload)
{
	bool	lRet = false;

	if	(pDownload)
	{
		try
		{
			POSITION			lPos;
			long				lReqID;
			CQueuedPrepare *	lPrepare;
			HRESULT				lResult = S_OK;

#ifdef	_DEBUG_PREPARE
			if	(LogIsActive (_DEBUG_PREPARE))
			{
				LogMessage (_DEBUG_PREPARE, _T("[%p] [%d] DaSvrCharacter _OnDownloadComplete [%ls]"), this, mCharID, (BSTR)pDownload->GetURL());
			}
#endif
			for	(lPos = mPrepares.GetStartPosition(); lPos;)
			{
				mPrepares.GetNextAssoc (lPos, lReqID=0, lPrepare=NULL);

				if	(
						(lPrepare)
					&&	(lPrepare->FindDownload (pDownload))
					)
				{
					lRet = true;
					break;
				}
			}

			if	(lRet)
			{
				if	(!lPrepare->IsSoundDownload ())
				{
					lResult = lPrepare->PutAnimation (mFile, pDownload);
#ifdef	_DEBUG_PREPARE
					if	(LogIsActive (_DEBUG_PREPARE))
					{
						LogComErrAnon (MinLogLevel(_DEBUG_PREPARE,LogAlways), lResult, _T("[%p] [%d] DaSvrCharacter PutAnimation [%ls] [%ls]"), this, mCharID, (BSTR)pDownload->GetURL(), (BSTR)pDownload->GetCacheName());
					}
#endif
				}

				if	(!lPrepare->IsComplete ())
				{
					lPrepare->StartDownloads ();
				}

				if	(lPrepare->IsComplete ())
				{
#ifdef	_DEBUG_PREPARE
					if	(LogIsActive (_DEBUG_PREPARE))
					{
						LogMessage (_DEBUG_PREPARE, _T("[%p] [%d] DaSvrCharacter Prepare Complete [%p] [%d] [%ls]"), this, mCharID, lPrepare, lPrepare->mReqID, (BSTR)lPrepare->GetAnimationNames());
					}
#endif
#ifdef	_DEBUG_REQUESTS
					if	(LogIsActive (_DEBUG_REQUESTS))
					{
						LogMessage (_DEBUG_REQUESTS, _T("[%d] RequestComplete [%d] [%8.8X]"), mCharID, lPrepare->mReqID, lResult);
					}
#endif
					PreNotify ();
					try
					{
						mNotify->RequestComplete (lPrepare->mReqID, lResult);
					}
					catch AnyExceptionSilent
					PostNotify ();
				}
			}
		}
		catch AnyExceptionSilent
	}
	return lRet;
}

CFileDownload * DaSvrCharacter::_FindSoundDownload (LPCTSTR pSoundUrl)
{
	CFileDownload *	lRet = NULL;

	try
	{
		POSITION			lPos;
		long				lReqID;
		CQueuedPrepare *	lPrepare;
		CFileDownload *		lDownload;

		for	(lPos = mPrepares.GetStartPosition(); lPos;)
		{
			mPrepares.GetNextAssoc (lPos, lReqID=0, lPrepare=NULL);

			if	(
					(lPrepare)
				&&	(lPrepare->IsSoundDownload ())
				&&	(lDownload = lPrepare->GetDownload ())
				&&	(CString (lDownload->GetURL()).CompareNoCase (pSoundUrl) == 0)
				)
			{
				lRet = lDownload;
				break;
			}
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void DaSvrCharacter::_OnCharacterNameChanged (long pCharID)
{
	if	(
			(mWnd)
		&&	(mWnd->IsWindow ())
		&&	(mWnd->GetCharID() == GetCharID())
		)
	{
		mWnd->SetNotifyIconTip (&mIconData, mFile, mLangID);
	}
}

void DaSvrCharacter::_OnCharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID)
{
	CVoiceCommandsWnd *	lVoiceCommandsWnd;

#ifdef	_DEBUG_ACTIVE
	if	(LogIsActive (_DEBUG_ACTIVE))
	{
		LogMessage (_DEBUG_ACTIVE, _T("[%d] OnCharacterActive [%d] InputActive [%d] Inactive [%d] InputInactive [%d] - IsVisible [%u] IsClientActive [%u] IsInputActive [%u] IsListening [%u] - ActiveClient [%d] InputActive [%d] Listening [%d]"), mCharID, pActiveCharID, pInputActiveCharID, pInactiveCharID, pInputInactiveCharID, IsVisible(), IsClientActive(), IsInputActive(), IsListening(), GetActiveClient(), mNotify->_GetActiveCharacter(), (_AtlModule.GetListenCharacter()?_AtlModule.GetListenCharacter()->GetCharID():0));
	}
#endif

	if	(pActiveCharID == mCharID)
	{
		PropagateLangID ();
		if	(
				(mWnd)
			&&	(mWnd->IsWindow ())
			)
		{
			mWnd->EnableIdle (mIdleEnabled);
			mWnd->EnableSound (mSoundEnabled);
		}
	}

	if	(
			(pInactiveCharID == mCharID)
		&&	(pActiveCharID != GetActiveClient ())
		)
	{
		DaSvrCharacter *	lListeningCharacter = NULL;

		if	(
				(IsListening ())
			&&	(
					(
						(pInputActiveCharID > 0)
					&&	(pInputActiveCharID == pActiveCharID)
					&&	(lListeningCharacter = _AtlModule.GetAppCharacter (pInputActiveCharID))
					)
				||	(
						(lListeningCharacter = _AtlModule.GetListenCharacter ())
					)
				)
			&&	(lListeningCharacter != this)
			)
		{
			TransferListeningState (lListeningCharacter);
		}

		if	(lListeningCharacter != this)
		{
			StopListening (false, ListenComplete_CharacterClientDeactivated);
		}
	}

	if	(
			(
				(pActiveCharID == mCharID)
			||	(pInactiveCharID == mCharID)
			)
		&&	(lVoiceCommandsWnd = _AtlModule.GetVoiceCommandsWnd (false))
		&&	(lVoiceCommandsWnd->IsWindow ())
		)
	{
		DaSvrCommands *	lCommands;

		if	(
				(pActiveCharID == mCharID)
			&&	(pInputActiveCharID == mCharID)
			)
		{
			if	(lVoiceCommandsWnd = _AtlModule.GetVoiceCommandsWnd (true, mCharID))
			{
				lVoiceCommandsWnd->SetLangID (mLangID);

				if	(lCommands = GetCommandsObj (true))
				{
					lCommands->ShowVoiceCommands (lVoiceCommandsWnd);
				}
				else
				{
					lVoiceCommandsWnd->ShowGlobalCommands ();
				}
				lVoiceCommandsWnd->ShowOtherClients (mCharID);
			}
		}
		else
		{
			if	(lVoiceCommandsWnd->GetCharID() == mCharID)
			{
				lVoiceCommandsWnd->SetCharacter (0);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool DaSvrCharacter::_OnContextMenu (long pCharID, HWND pOwner, const CPoint & pPosition)
{
	if	(pCharID == mCharID)
	{
		if	(mAutoPopupMenu)
		{
			DoContextMenu (pOwner, pPosition);
		}
		return true;
	}
	return false;
}

bool DaSvrCharacter::_OnDefaultCommand (long pCharID, HWND pOwner, const CPoint & pPosition)
{
	if	(pCharID == mCharID)
	{
		if	(mAutoPopupMenu)
		{
			DoDefaultCommand (pOwner, pPosition);
		}
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

bool DaSvrCharacter::DoContextMenu (HWND pOwner, const CPoint & pPosition)
{
	DaSvrCommands *	lCommands;
	DaSvrCommand *	lCommand;
	USHORT			lCommandId;

	if	(
			(lCommands = GetCommandsObj (true))
		&&	(lCommandId = lCommands->DoContextMenu (pOwner, pPosition))
		)
	{
		if	(
				(!DoMenuCommand (lCommandId))
			&&	(lCommand = lCommands->GetCommand (lCommandId))
			&&	(lCommand->mEnabled)
			)
		{
			PreNotify ();
			try
			{
				mNotify->Command ((long)lCommandId, NULL);
			}
			catch AnyExceptionDebug
			PostNotify ();
		}
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

bool DaSvrCharacter::DoDefaultCommand (HWND pOwner, const CPoint & pPosition)
{
//
//	MS Agent leaves default command processing to the calling application
//
#ifndef	_STRICT_COMPATIBILITY
	DaSvrCommands *	lCommands;
	USHORT			lCommand;

	if	(
			(lCommands = GetCommandsObj (true))
		&&	(lCommand = lCommands->mDefaultId)
		)
	{
		PreNotify ();
		try
		{
			mNotify->Command ((long)lCommand, NULL);
		}
		catch AnyExceptionDebug
		PostNotify ();
		return true;
	}
#endif
	return false;
}

/////////////////////////////////////////////////////////////////////////////

bool DaSvrCharacter::DoMenuCommand (USHORT pCommandId)
{
	bool			lRet = false;
	DaSvrCommands *	lCommands;

	if	(lCommands = GetCommandsObj (true))
	{
		if	(pCommandId == lCommands->mHideCharacterCmdId)
		{
			StopAll (StopAll_Play|StopAll_Move|StopAll_Speak|StopAll_QueuedPrepare, AGENTREQERR_INTERRUPTEDUSER);
			mWnd->QueueHide (mCharID, false, VisibilityCause_UserHid);
			mWnd->ActivateQueue (true);
			lRet = true;
		}
		else
		if	(pCommandId == lCommands->mShowCharacterCmdId)
		{
			StopAll (StopAll_Play|StopAll_Move|StopAll_Speak|StopAll_QueuedPrepare, AGENTREQERR_INTERRUPTEDUSER);
			mWnd->QueueShow (mCharID, false, VisibilityCause_UserShowed);
			mWnd->ActivateQueue (true);
			lRet = true;
		}
		else
		if	(pCommandId == lCommands->mHideCommandsCmdId)
		{
			CVoiceCommandsWnd *	lVoiceCommandsWnd;

			if	(lVoiceCommandsWnd = _AtlModule.GetVoiceCommandsWnd (false))
			{
				lVoiceCommandsWnd->Hide ();
			}
			lRet = true;
		}
		else
		if	(pCommandId == lCommands->mShowCommandsCmdId)
		{
			CVoiceCommandsWnd *	lVoiceCommandsWnd;

			if	(lVoiceCommandsWnd = _AtlModule.GetVoiceCommandsWnd (true, mCharID))
			{
				lVoiceCommandsWnd->SetLangID (mLangID);
				lCommands->ShowVoiceCommands (lVoiceCommandsWnd);
				lVoiceCommandsWnd->ShowOtherClients (mCharID);
				lVoiceCommandsWnd->Show ();
			}
			lRet = true;
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

DaSvrCommands * DaSvrCharacter::GetCommandsObj (bool pCreateObject)
{
	DaSvrCommands *		lRet = NULL;
	IDaSvrCommandsPtr	lInterface;

	if	(pCreateObject)
	{
		lInterface = GetUnknown ();
	}
	if	(mSvrCommands != NULL)
	{
		lRet = dynamic_cast <DaSvrCommands *> (mSvrCommands);
	}
	return lRet;
}

IDaSvrCommands2 * DaSvrCharacter::GetCommandsInterface (bool pCreateObject)
{
	IDaSvrCommands2Ptr	lInterface;

	if	(pCreateObject)
	{
		lInterface = GetUnknown ();
	}
	else
	{
		lInterface = mSvrCommands;
	}
	return lInterface;
}

/////////////////////////////////////////////////////////////////////////////

DaSvrBalloon * DaSvrCharacter::GetBalloonObj (bool pCreateObject)
{
	DaSvrBalloon *		lRet = NULL;
	IDaSvrBalloon2Ptr	lInterface;

	if	(pCreateObject)
	{
		lInterface = GetUnknown ();
	}
	if	(mSvrBalloon != NULL)
	{
		lRet = dynamic_cast <DaSvrBalloon *> (mSvrBalloon);
	}
	return lRet;
}

IDaSvrBalloon2 * DaSvrCharacter::GetBalloonInterface (bool pCreateObject)
{
	IDaSvrBalloon2Ptr	lInterface;

	if	(pCreateObject)
	{
		lInterface = GetUnknown ();
	}
	else
	{
		lInterface = mSvrBalloon;
	}
	return lInterface;
}

/////////////////////////////////////////////////////////////////////////////

CAgentPopupWnd * DaSvrCharacter::GetAgentWnd ()
{
	return mWnd;
}

CAgentBalloonWnd * DaSvrCharacter::GetBalloonWnd (bool pCreateObject)
{
	CAgentBalloonWnd *	lBalloonWnd = NULL;

	if	(
			(mWnd)
		&&	(mWnd->IsWindow ())
		)
	{
		lBalloonWnd = mWnd->GetBalloonWnd ();

		if	(
				(
					(!lBalloonWnd)
				||	(!lBalloonWnd->IsWindow ())
				)
			&&	(pCreateObject)
			)
		{
			IDaSvrBalloon2Ptr	lBalloonInterface (GetBalloonInterface (true));

			if	(
					(lBalloonInterface->get_Enabled (NULL) == S_OK)
				&&	(lBalloonWnd = mWnd->GetBalloonWnd (true))
				&&	(lBalloonWnd->IsWindow ())
				)
			{
				mWnd->GetBalloonWnd()->SetOptions (mFile->GetBalloon(), lBalloonInterface, mLangID);
			}
		}

		if	(
				(lBalloonWnd)
			&&	(!lBalloonWnd->IsWindow ())
			)
		{
			lBalloonWnd = NULL;
		}
	}
	return lBalloonWnd;
}

CAgentListeningWnd * DaSvrCharacter::GetListeningWnd (bool pCreateObject)
{
	CAgentListeningWnd *	lListeningWnd = NULL;

	if	(
			(mWnd)
		&&	(mWnd->IsWindow ())
		&&	(
				(!pCreateObject)
			||	(CDaSettingsConfig().LoadConfig().mSrListeningTip)
			)
		)
	{
		lListeningWnd = mWnd->GetListeningWnd (pCreateObject);

		if	(
				(lListeningWnd)
			&&	(lListeningWnd->IsWindow ())
			)
		{
			if	(pCreateObject)
			{
				lListeningWnd->Attach (mCharID, CString (GetName()));
				lListeningWnd->SetLangID (mLangID);
			}
		}
		else
		{
			lListeningWnd = NULL;
		}
	}
	return lListeningWnd;
}

/////////////////////////////////////////////////////////////////////////////

LPVOID DaSvrCharacter::FindOtherRequest (long pReqID, DaSvrCharacter *& pOtherCharacter)
{
	LPVOID			lRet = NULL;
	CAgentFile *	lFile;
	INT_PTR			lFileNdx;

	pOtherCharacter = NULL;

	for	(lFileNdx = 0; lFile = mUsedFileCache->GetCachedFile (lFileNdx); lFileNdx++)
	{
		CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
		INT_PTR								lClientNdx;
		DaSvrCharacter *					lCharacter;

		if	(mUsedFileCache->GetFileClients (lFile, lFileClients))
		{
			for	(lClientNdx = lFileClients.GetCount()-1; lClientNdx >= 0; lClientNdx--)
			{
				if	(
						(lCharacter = dynamic_cast <DaSvrCharacter *> (lFileClients [lClientNdx]))
					&&	(lCharacter->mWnd)
					&&	(lRet = lCharacter->mWnd->FindQueuedAction (pReqID))
					)
				{
					pOtherCharacter = lCharacter;
					break;
				}
			}
			if	(lRet)
			{
				break;
			}
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void DaSvrCharacter::_OnOptionsChanged ()
{
	CDaSettingsConfig		lSettingsConfig;
	CAgentListeningWnd *	lListeningWnd;
	CAgentBalloonWnd *		lBalloonWnd;

	lSettingsConfig.LoadConfig ();

	if	(!lSettingsConfig.mSrEnabled)
	{
		StopListening (false, ListenComplete_UserDisabled);
		ReleaseSapiVoice ();
	}
	if	(
			(!lSettingsConfig.mSrListeningTip)
		&&	(lListeningWnd = GetListeningWnd (false))
		)
	{
		lListeningWnd->Detach (mCharID);
	}

	if	(
			(mWnd)
		&&	(mWnd->IsWindow ())
		&&	(mWnd->GetCharID() == GetCharID())
		)
	{
		mWnd->EnableSound (mSoundEnabled);
	}

	if	(
			(mFile)
		&&	(lBalloonWnd = GetBalloonWnd (false))
		&&	(lBalloonWnd->GetCharID() == GetCharID())
		)
	{
		IDaSvrBalloon2Ptr	lBalloonInterface (GetBalloonInterface (true));

		if	(lBalloonInterface->get_Enabled (NULL) == S_FALSE)
		{
			lBalloonWnd->HideBalloon (true);
			mWnd->RemoveQueuedThink (mCharID, AGENTREQERR_INTERRUPTEDCODE, _T("OptionsChanged"));
		}
		lBalloonWnd->SetOptions (mFile->GetBalloon(), lBalloonInterface, mLangID);
	}
}

void DaSvrCharacter::_OnDefaultCharacterChanged ()
{
	StopListening (false, ListenComplete_DefaultCharacterChanged);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT DaSvrCharacter::GetDefaultTTSEngine (CAgentFile * pFile, IDaSvrTTSEngine ** pTTSEngine, LPCTSTR pClientMutexName)
{
	HRESULT				lResult = S_FALSE;
	DaSvrTTSEngine *	lTTSEngine = NULL;
	IDaSvrTTSEnginePtr	lInterface;
	CSapiVoiceCache *	lVoiceCache;
	CSapi5Voices *		lSapi5Voices;
	CSapi5VoiceInfo *	lSapi5Voice;
#ifndef	_WIN64
	CSapi4Voices *		lSapi4Voices;
	CSapi4VoiceInfo *	lSapi4Voice;
#endif

	if	(
			(pFile)
		&&	(pTTSEngine)
		&&	(lVoiceCache = CSapiVoiceCache::GetStaticInstance ())
		)
	{
		tPtr <CSapiVoice>	lSapiVoice;

		if	(lSapiVoice = lVoiceCache->GetAgentVoice (pFile->GetTts(), true, false))
		{
			if	(
					(lSapi5Voices = lVoiceCache->GetSapi5Voices ())
				&&	(lSapi5Voice = lSapi5Voices->GetVoiceId (CString (lSapiVoice->GetUniqueId ())))
				)
			{
				lTTSEngine = DaSvrTTSEngine::CreateInstance (lSapi5Voice, pClientMutexName);
			}
#ifndef	_WIN64
			else
			if	(
					(lSapi4Voices = lVoiceCache->GetSapi4Voices ())
				&&	(lSapi4Voice = lSapi4Voices->GetModeId (CGuidStr::Parse (lSapiVoice->GetUniqueId ())))
				)
			{
				lTTSEngine = DaSvrTTSEngine::CreateInstance (lSapi4Voice, pClientMutexName);
			}
#endif
		}

		if	(lTTSEngine)
		{
			lInterface = lTTSEngine->GetControllingUnknown ();
			(*pTTSEngine) = lInterface.Detach();
			lResult = S_OK;
		}
	}
	else
	{
		lResult = E_FAIL;
	}
	return lResult;
}

HRESULT DaSvrCharacter::FindTTSEngines (CAgentFile * pFile, LANGID pLangId, short pGender, IDaSvrTTSEngines ** pTTSEngines, LPCTSTR pClientMutexName)
{
	HRESULT				lResult = S_FALSE;
	DaSvrTTSEngines *	lTTSEngines;
	IDaSvrTTSEnginesPtr	lInterface;
	CSapiVoiceCache *	lVoiceCache;
	CSapi5Voices *		lSapi5Voices;
	INT_PTR				lSapi5VoiceNdx = -1;
#ifndef	_WIN64
	CSapi4Voices *		lSapi4Voices;
	INT_PTR				lSapi4VoiceNdx = -1;
#endif

	if	(
			(pTTSEngines)
		&&	(lVoiceCache = CSapiVoiceCache::GetStaticInstance ())
		)
	{
		if	(lTTSEngines = DaSvrTTSEngines::CreateInstance (pClientMutexName))
		{
			tS <CAgentFileTts>	lFileTts;

			if	(pFile)
			{
				lFileTts = pFile->GetTts ();
				lFileTts.mMode = GUID_NULL;
				lFileTts.mModeId = NULL;
			}
			else
			{
				lFileTts.mGender = pGender;
			}

			lFileTts.mLanguage = pLangId;

			if	(lSapi5Voices = lVoiceCache->GetSapi5Voices ())
			{
				while ((lSapi5VoiceNdx = lSapi5Voices->FindVoice (lFileTts, (pLangId==0), lSapi5VoiceNdx)) >= 0)
				{
					lTTSEngines->mSapi5Voices.Add (lSapi5Voices->GetAt (lSapi5VoiceNdx));
				}
			}
#ifndef	_WIN64
			if	(lSapi4Voices = lVoiceCache->GetSapi4Voices ())
			{
				while ((lSapi4VoiceNdx = lSapi4Voices->FindVoice (lFileTts, (pLangId==0), lSapi4VoiceNdx)) >= 0)
				{
					lTTSEngines->mSapi4Voices.Add (lSapi4Voices->GetAt (lSapi4VoiceNdx));
				}
			}
#endif
			lInterface = lTTSEngines->GetControllingUnknown();
			(*pTTSEngines) = lInterface.Detach();
			lResult = S_OK;
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
	}
	else
	{
		lResult = E_FAIL;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT DaSvrCharacter::GetDefaultSREngine (CAgentFile * pFile, IDaSvrSREngine ** pSREngine, LPCTSTR pClientMutexName)
{
	HRESULT				lResult = S_FALSE;
	DaSvrSREngine *		lSREngine = NULL;
	IDaSvrSREnginePtr	lInterface;
	CSapiInputCache *	lInputCache;
	CSapi5Inputs *		lSapi5Inputs;
	CSapi5InputInfo *	lSapi5Input;

	if	(
			(pFile)
		&&	(pSREngine)
		&&	(lInputCache = CSapiInputCache::GetStaticInstance ())
		)
	{
		tPtr <CSapi5Input>	lSapiInput;

		if	(
				(lSapiInput = lInputCache->GetAgentInput (pFile->GetTts().mLanguage, true, false))
			&&	(lSapi5Inputs = lInputCache->GetSapi5Inputs ())
			&&	(lSapi5Input = lSapi5Inputs->GetEngineId (CString (lSapiInput->GetEngineId ())))
			)
		{
			lSREngine = DaSvrSREngine::CreateInstance (lSapi5Input, pClientMutexName);
		}

		if	(lSREngine)
		{
			lInterface = lSREngine->GetControllingUnknown();
			(*pSREngine) = lInterface.Detach();
			lResult = S_OK;
		}
	}
	else
	{
		lResult = E_FAIL;
	}
	return lResult;
}

HRESULT DaSvrCharacter::FindSREngines (CAgentFile * pFile, LANGID pLangId, IDaSvrSREngines ** pSREngines, LPCTSTR pClientMutexName)
{
	HRESULT				lResult = S_FALSE;
	DaSvrSREngines *	lSREngines;
	IDaSvrSREnginesPtr	lInterface;
	CSapiInputCache *	lInputCache;
	CSapi5Inputs *		lSapi5Inputs;
	INT_PTR				lSapi5InputNdx = -1;

	if	(
			(pSREngines)
		&&	(lInputCache = CSapiInputCache::GetStaticInstance ())
		)
	{
		if	(lSREngines = DaSvrSREngines::CreateInstance (pClientMutexName))
		{
			if	(
					(pFile)
				&&	(pLangId == 0)
				)
			{
				pLangId = pFile->GetTts().mLanguage;
			}

			if	(lSapi5Inputs = lInputCache->GetSapi5Inputs ())
			{
				while ((lSapi5InputNdx = lSapi5Inputs->FindInput (pLangId, (pLangId==0), lSapi5InputNdx)) >= 0)
				{
					lSREngines->mSapi5Inputs.Add (lSapi5Inputs->GetAt (lSapi5InputNdx));
				}
			}

			lInterface = lSREngines->GetControllingUnknown();
			(*pSREngines) = lInterface.Detach();
			lResult = S_OK;
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
	}
	else
	{
		lResult = E_FAIL;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaSvrCharacter::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(
			(InlineIsEqualGUID (__uuidof(IDaSvrCharacter2), riid))
		||	(InlineIsEqualGUID (__uuidof(IDaSvrCharacter), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentCharacter), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentCharacterEx), riid))
		)
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetClassForHandler (DWORD dwDestContext, void *pvDestContext, CLSID *pClsid)
{
	if	(!pClsid)
	{
		return E_POINTER;
	}
	*pClsid = __uuidof(DaServerHandler);
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p(%d)] DaSvrCharacter::GetClassForHandler [%8.8X] [%p] [%s]"), this, m_dwRef, dwDestContext, pvDestContext, CGuidStr::GuidName(*pClsid));
#endif
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_Balloon (IDaSvrBalloon2 **Balloon)
{
#ifdef	_DEBUG_INTERFACE_NOT
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_Balloon"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT				lResult = S_OK;
	IDaSvrBalloon2Ptr	lSvrBalloon;

	if	(!Balloon)
	{
		lResult = E_POINTER;
	}
	else
	{
		lSvrBalloon = GetBalloonInterface (true);
		(*Balloon) = lSvrBalloon.Detach ();
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_Balloon"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_Commands (IDaSvrCommands2 **Commands)
{
#ifdef	_DEBUG_INTERFACE_NOT
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_Commands"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT				lResult = S_OK;
	IDaSvrCommands2Ptr	lSvrCommands;

	if	(!Commands)
	{
		lResult = E_POINTER;
	}
	else
	{
		lSvrCommands = GetCommandsInterface (true);
		(*Commands) = lSvrCommands.Detach ();
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_Commands"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetPosition (long Left, long Top)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::SetPosition [%d %d]"), this, m_dwRef, mCharID, Left, Top);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(
			(!mWnd)
		||	(!mWnd->IsWindow ())
		)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("SetPosition"), _T("%d\t%d"), Left, Top);
#endif
#ifdef	_STRICT_COMPATIBILITY
		if	(!mWnd->MovePopup (CPoint (Left, Top), 0, MoveCause_ProgramMoved, true))
#else
		if	(!mWnd->MovePopup (CPoint (Left, Top), 0, MoveCause_ProgramMoved))
#endif
		{
			lResult = S_FALSE;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::SetPosition"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetPosition (long *Left, long *Top)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetPosition"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;
	CRect	lWindowRect (0,0,0,0);

	if	(
			(!mWnd)
		||	(!mWnd->IsWindow ())
		)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		mWnd->GetWindowRect (&lWindowRect);
	}
	if	(Left)
	{
		(*Left) = lWindowRect.left;
	}
	if	(Top)
	{
		(*Top) = lWindowRect.top;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetPosition"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetSize (long Width, long Height)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::SetSize"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(
			(!mWnd)
		||	(!mWnd->IsWindow ())
		)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("SetSize"), _T("%d\t%d"), Width, Height);
#endif
		if	(!mWnd->SizePopup (CSize (Width, Height), 0))
		{
#ifndef	_STRICT_COMPATIBILITY
			lResult = S_FALSE;
#endif
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::SetSize"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetSize (long *Width, long *Height)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetSize"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;
	CRect	lWindowRect (0,0,0,0);

	if	(
			(!mWnd)
		||	(!mWnd->IsWindow ())
		)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		mWnd->GetWindowRect (&lWindowRect);
	}
	if	(Width)
	{
		(*Width) = lWindowRect.Width();
	}
	if	(Height)
	{
		(*Height) = lWindowRect.Height();
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetSize"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetOriginalSize (long *Width, long *Height)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetOriginalSize"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;
	CSize	lImageSize (0,0);

	if	(!mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		lImageSize = mFile->GetImageSize ();
	}

	if	(Width)
	{
		(*Width) = lImageSize.cx;
	}
	if	(Height)
	{
		(*Height) = lImageSize.cy;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetOriginalSize"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetGUID (BSTR *CharGUID)
{
	return get_UniqueID (CharGUID);
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetVersion(short *MajorVersion, short *MinorVersion)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetVersion"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		if	(MajorVersion)
		{
			(*MajorVersion) = HIWORD (mFile->GetVersion());
		}
		if	(MinorVersion)
		{
			(*MinorVersion) = LOWORD (mFile->GetVersion());
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetVersion"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetName (BSTR *Name)
{
	return get_Name (Name);
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetName (BSTR Name)
{
	return put_Name (Name);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetDescription (BSTR *Description)
{
	return get_Description (Description);
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetDescription (BSTR Description)
{
	return put_Description (Description);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetLanguageID (long LanguageID)
{
	return put_LanguageID (LanguageID);
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetLanguageID (long *LanguageID)
{
	return get_LanguageID (LanguageID);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetIdleOn (long On)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::SetIdleOn [%d]"), this, m_dwRef, mCharID, On);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("SetIdleOn"), _T("%d"), On);
#endif
		if	(On)
		{
			lResult = SetStyle (0, CharacterStyle_IdleEnabled);
		}
		else
		{
			lResult = SetStyle (CharacterStyle_IdleEnabled, 0);
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::SetIdleOn"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetIdleOn (long *On)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetIdleOn"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = IsIdleEnabled() ? S_OK : S_FALSE;

	if	(!mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	if	(!On)
	{
#ifdef _STRICT_COMPATIBILITY
		lResult = E_POINTER;
#endif
	}
	else
	{
		(*On) = (IsIdleEnabled()!=false);
#ifdef _STRICT_COMPATIBILITY
		lResult = S_OK;
#endif
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetIdleOn"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetSoundEffectsOn (long On)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::SetSoundEffectsOn [%d]"), this, m_dwRef, mCharID, On);
	}
#endif
	HRESULT	lResult = S_FALSE;

	if	(!mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("SetSoundEffectsOn"), _T("%d"), On);
#endif
		if	(On)
		{
			lResult = SetStyle (0, CharacterStyle_SoundEffects);
		}
		else
		{
			lResult = SetStyle (CharacterStyle_SoundEffects, 0);
		}
#ifdef	_STRICT_COMPATIBILITY
		lResult = S_OK;
#endif
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::SetSoundEffectsOn [%d]"), this, m_dwRef, mCharID, On);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetSoundEffectsOn (long *On)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetSoundEffectsOn"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = IsSoundEnabled() ? S_OK : S_FALSE;

	if	(!mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	if	(!On)
	{
#ifdef _STRICT_COMPATIBILITY
		lResult = E_POINTER;
#endif
	}
	else
	{
		(*On) = (IsSoundEnabled(true)!=false);
#ifdef _STRICT_COMPATIBILITY
		lResult = S_OK;
#endif
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetSoundEffectsOn"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetExtraData (BSTR *ExtraData)
{
	return get_ExtraData (ExtraData);
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetAnimationNames (IUnknown **punkEnum)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetAnimationNames"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT					lResult = S_OK;
	IDaSvrAnimationNames *	lInterface = NULL;

	if	(!punkEnum)
	{
		lResult = E_POINTER;
	}
	else
	{
		lResult = get_AnimationNames (&lInterface);
		(*punkEnum) = lInterface;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetAnimationNames"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_AnimationNames (IDaSvrAnimationNames **AnimationNames)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_AnimationNames"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT					lResult = S_OK;
	DaSvrAnimationNames *	lAnimationNames;
	IDaSvrAnimationNamesPtr	lInterface;

	if	(!AnimationNames)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*AnimationNames) = NULL;

		if	(!mFile)
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
		else
		if	(lAnimationNames = DaSvrAnimationNames::CreateInstance (*mFile))
		{
			lInterface = lAnimationNames->GetControllingUnknown ();
			(*AnimationNames) = lInterface.Detach();
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_AnimationNames"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Activate (short State)
{
	return put_ActiveState ((ActiveStateType)State);
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetActive (short *State)
{
	return get_ActiveState ((ActiveStateType*)State);
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::HasOtherClients (long *OtherClientCount)
{
	return get_OtherClientCount (OtherClientCount);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Show (long Fast, long *RequestID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::Show [%d]"), this, m_dwRef, mCharID, Fast);
	}
#endif
	HRESULT	lResult = S_OK;
	long	lReqID = 0;

	if	(
			(!mWnd)
		||	(!mWnd->IsWindow ())
		)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		lReqID = Show (Fast!=0);
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("Show"), _T("%d\t%d"), Fast, lReqID);
#endif
	}
	if	(RequestID)
	{
		(*RequestID) = lReqID;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::Show"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Hide (long Fast, long *RequestID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::Hide [%d]"), this, m_dwRef, mCharID, Fast);
	}
#endif
	HRESULT	lResult = S_OK;
	long	lReqID = 0;

	if	(
			(!mWnd)
		||	(!mWnd->IsWindow ())
		)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		lReqID = Hide (Fast!=0);
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("Hide"), _T("%d\t%d"), Fast, lReqID);
#endif
	}
	if	(RequestID)
	{
		(*RequestID) = lReqID;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::Hide"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetVisible (long *Visible)
{
	VARIANT_BOOL	lVisible;
	HRESULT			lResult = get_Visible (&lVisible);
	
	if	(Visible)
	{
		(*Visible) = (lVisible != VARIANT_FALSE);
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetVisibilityCause (long *Cause)
{
	VisibilityCauseType	lCause;
	HRESULT				lResult = get_VisibilityCause (&lCause);
	
	if	(Cause)
	{
		(*Cause) = (long)lCause;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Prepare (long Type, BSTR Name, long Queue, long *RequestID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::Prepare [%d] [%ls] [%d]"), this, m_dwRef, mCharID, Type, Name, Queue);
	}
#endif
	HRESULT	lResult;
	long	lReqID = 0;

	if	(!Name)
	{
		lResult = E_POINTER;
	}
	else
	if	(
			(!mWnd)
		||	(!mWnd->IsWindow ())
		)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		lResult = DoPrepare (Type, CString (Name), (Queue != 0), lReqID);
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("Prepare"), _T("%d\t%ls\t%d\t%d"), Type, Name, Queue, lReqID);
#endif
	}

	if	(RequestID)
	{
		(*RequestID) = lReqID;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::Prepare"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Play (BSTR Animation, long *RequestID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::Play [%s]"), this, m_dwRef, mCharID, DebugStr(CString(Animation)));
	}
#endif
	HRESULT	lResult = S_OK;
	long	lReqID = 0;

	if	(!Animation)
	{
		lResult = E_POINTER;
	}
	else
	if	(
			(!mWnd)
		||	(!mWnd->IsWindow ())
		)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		lReqID = mWnd->QueueGesture (mCharID, Animation);
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("Play"), _T("%ls\t%d"), Animation, lReqID);
#endif
		if	(lReqID)
		{
			mWnd->ActivateQueue (true);
		}
		else
		{
			lResult = AGENTERR_ANIMATIONNOTFOUND;
		}
	}
	if	(RequestID)
	{
		(*RequestID) = lReqID;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::Play"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Stop (long RequestID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::Stop"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT				lResult = S_OK;
	LPVOID				lRequest = NULL;
	DaSvrCharacter *	lOtherCharacter = NULL;

	if	(
			(!mWnd)
		||	(!mWnd->IsWindow ())
		)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	if	(RequestID <= 0)
	{
		lResult = AGENTREQERR_OBJECTINVALID;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("Stop"), _T("%d"), RequestID);
#endif
		if	(lRequest = mWnd->FindQueuedAction (RequestID))
		{
			if	(!mWnd->RemoveQueuedAction ((CQueuedAction *) lRequest, AGENTREQERR_INTERRUPTEDCODE, _T("Stop")))
			{
				lResult = AGENTREQERR_REMOVED;
			}
		}
		else
		if	(
				(lRequest = FindOtherRequest (RequestID, lOtherCharacter))
			&&	(lOtherCharacter != this)
			)
		{
			lResult = AGENTREQERR_CANTSTOPOTHERS;
		}
		else
		{
#ifndef	_STRICT_COMPATIBILITY
			lResult = AGENTREQERR_OBJECTNOTFOUND;
#endif
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::Stop"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::StopAll (long Types)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::StopAll [%8.8X]"), this, m_dwRef, mCharID, Types);
	}
#endif
	HRESULT	lResult = S_FALSE;

	if	(
			(!mWnd)
		||	(!mWnd->IsWindow ())
		)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("StopAll"), _T("0x%8.8X"), Types);
#endif
		lResult = StopAll (Types, AGENTREQERR_INTERRUPTEDCODE);
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::StopAll"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::MoveTo (short x, short y, long Speed, long *RequestID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::MoveTo"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;
	long	lReqID = 0;

	if	(
			(!mWnd)
		||	(!mWnd->IsWindow ())
		)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	if	(mWnd->IsDragging())
	{
		lResult = AGENTERR_CANTMOVEDURINGDRAG;
	}
	else
	if	(Speed > 0)
	{
		lReqID = mWnd->QueueMove (mCharID, CPoint (x, y), Speed);
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("MoveTo"), _T("%hd\t%hd\t%d\t%d"), x, y, Speed, lReqID);
#endif
		if	(lReqID)
		{
			mWnd->ActivateQueue (true);
		}
		else
		{
			lResult = SetPosition (x, y);
		}
	}
	else
	{
		lResult = SetPosition (x, y);
	}
	if	(RequestID)
	{
		(*RequestID) = lReqID;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::MoveTo"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetMoveCause (long *Cause)
{
	MoveCauseType	lCause;
	HRESULT			lResult = get_MoveCause (&lCause);
	
	if	(Cause)
	{
		(*Cause) = (long)lCause;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GestureAt (short x, short y, long *RequestID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GestureAt [%hd %hd]"), this, m_dwRef, mCharID, x, y);
	}
#endif
	HRESULT	lResult = S_OK;
	long	lReqID = 0;

	if	(
			(!mWnd)
		||	(!mWnd->IsWindow ())
		)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		CPoint	lOffset (x, y);
		CRect	lWinRect;
		CString	lStateName;

		mWnd->GetWindowRect (&lWinRect);
		lOffset -= lWinRect.CenterPoint ();

		if	(lOffset.x < 0)
		{
			if	(lOffset.y < lOffset.x)
			{
				lStateName = _T("GESTURINGUP");
			}
			else
			if	(lOffset.y > -lOffset.x)
			{
				lStateName = _T("GESTURINGDOWN");
			}
			else
			{
				lStateName = _T("GESTURINGRIGHT");
			}
		}
		else
		{
			if	(lOffset.y > lOffset.x)
			{
				lStateName = _T("GESTURINGDOWN");
			}
			else
			if	(lOffset.y < -lOffset.x)
			{
				lStateName = _T("GESTURINGUP");
			}
			else
			{
				lStateName = _T("GESTURINGLEFT");
			}
		}

		lReqID = mWnd->QueueState (mCharID, lStateName);
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("GestureAt"), _T("%hd\t%hd\t%d"), x, y, lReqID);
#endif
		if	(lReqID)
		{
			mWnd->ActivateQueue (true);
		}
#ifndef	_STRICT_COMPATIBILITY
		else
		{
			lResult = AGENTERR_STATENOTFOUND;
		}
#endif
	}
	if	(RequestID)
	{
		(*RequestID) = lReqID;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GestureAt"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Think (BSTR Text, long *RequestID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::Think [%s]"), this, m_dwRef, mCharID, DebugStr(Text));
	}
#endif
	HRESULT	lResult = S_OK;
	long	lReqID = 0;

	if	(
			(!mWnd)
		||	(!mWnd->IsWindow ())
		)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		IDaSvrBalloon2Ptr	lSvrBalloon (GetBalloonInterface (true));

		if	(lSvrBalloon->get_Enabled (NULL) == S_OK)
		{
			GetBalloonWnd (true);
			lReqID = mWnd->QueueThink (mCharID, CString (Text));
			mWnd->ActivateQueue (true);
		}
		else
		{
			lResult = AGENTERR_NOBALLOON;
		}
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("Think"), _T("%s\t%d"), EncodeTraceString(Text), lReqID);
#endif
	}
	if	(RequestID)
	{
		(*RequestID) = lReqID;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::Think"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Wait (long WaitForRequestID, long *RequestID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::Wait"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT				lResult = S_OK;
	LPVOID				lOtherRequest = NULL;
	DaSvrCharacter *	lOtherCharacter = NULL;
	long				lReqID = 0;

	if	(
			(!mWnd)
		||	(!mWnd->IsWindow ())
		)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	if	(RequestID <= 0)
	{
		lResult = AGENTREQERR_OBJECTINVALID;
	}
	else
	{
		if	(lOtherRequest = FindOtherRequest (WaitForRequestID, lOtherCharacter))
		{
			if	(
					(lOtherCharacter == this)
				||	(lOtherCharacter->mWnd == mWnd)
				)
			{
				lResult = AGENTREQERR_CANTWAITONSELF;
			}
			else
			{
				lReqID = mWnd->QueueWait (mCharID, lOtherCharacter->mCharID, WaitForRequestID);
				mWnd->ActivateQueue (true);
			}
#ifdef	_TRACE_CHARACTER_ACTIONS
			_AtlModule.TraceCharacterAction (mCharID, _T("Wait"), _T("%d\t%d"), RequestID, lReqID);
#endif
		}
		else
		{
#ifndef	_STRICT_COMPATIBILITY
			lResult = AGENTREQERR_OBJECTNOTFOUND;
#endif
		}
	}
	if	(RequestID)
	{
		(*RequestID) = lReqID;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::Wait"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Interrupt (long InterruptRequestID, long *RequestID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::Interrupt"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT				lResult = S_OK;
	LPVOID				lOtherRequest = NULL;
	DaSvrCharacter *	lOtherCharacter = NULL;
	long				lReqID = 0;

	if	(
			(!mWnd)
		||	(!mWnd->IsWindow ())
		)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	if	(RequestID <= 0)
	{
		lResult = AGENTREQERR_OBJECTINVALID;
	}
	else
	{
		if	(lOtherRequest = FindOtherRequest (InterruptRequestID, lOtherCharacter))
		{
			if	(
					(lOtherCharacter == this)
				||	(lOtherCharacter->mWnd == mWnd)
				)
			{
				lResult = AGENTREQERR_CANTINTERRUPTSELF;
			}
			else
			{
				lReqID = mWnd->QueueInterrupt (mCharID, lOtherCharacter->mCharID, InterruptRequestID);
				mWnd->ActivateQueue (true);
			}
#ifdef	_TRACE_CHARACTER_ACTIONS
			_AtlModule.TraceCharacterAction (mCharID, _T("Interrupt"), _T("%d\t%d"), RequestID, lReqID);
#endif
		}
		else
		{
#ifndef	_STRICT_COMPATIBILITY
			lResult = AGENTREQERR_OBJECTNOTFOUND;
#endif
		}
	}
	if	(RequestID)
	{
		(*RequestID) = lReqID;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::Interrupt"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::ShowPopupMenu (short x, short y)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::ShowPopupMenu"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(
			(!mWnd)
		||	(!mWnd->IsWindow ())
		)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	if	(mNotify->_GetActiveCharacter() != GetCharID())
	{
		lResult = AGENTERR_CHARACTERNOTACTIVE;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("ShowPopupMenu"), _T("%hd\t%hd"), x, y);
#endif
		if	(!DoContextMenu (mWnd->m_hWnd, CPoint (x, y)))
		{
			lResult = S_FALSE;
		}
	}
	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::ShowPopupMenu"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetAutoPopupMenu (long AutoPopupMenu)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::SetAutoPopupMenu [%d]"), this, m_dwRef, mCharID, AutoPopupMenu);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(
			(!mWnd)
		||	(!mWnd->IsWindow ())
		)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("SetAutoPopupMenu"), _T("%d"), AutoPopupMenu);
#endif
		if	(AutoPopupMenu)
		{
			lResult = SetStyle (0, CharacterStyle_AutoPopupMenu);
		}
		else
		{
			lResult = SetStyle (CharacterStyle_AutoPopupMenu, 0);
		}
#ifdef	_STRICT_COMPATIBILITY
		lResult = S_OK;
#endif
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::SetAutoPopupMenu"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetAutoPopupMenu (long *AutoPopupMenu)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetAutoPopupMenu"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = IsAutoPopupMenu() ? S_OK : S_FALSE;

	if	(
			(!mWnd)
		||	(!mWnd->IsWindow ())
		)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	if	(!AutoPopupMenu)
	{
#ifdef _STRICT_COMPATIBILITY
		lResult = E_POINTER;
#endif
	}
	else
	{
		(*AutoPopupMenu) = (IsAutoPopupMenu()!=false);
#ifdef _STRICT_COMPATIBILITY
		lResult = S_OK;
#endif
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetAutoPopupMenu"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetHelpFileName (BSTR *Name)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetHelpFileName"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = E_NOTIMPL;
/**/
	lResult = S_FALSE;
	if	(Name)
	{
		(*Name) = NULL;
	}
/**/
	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetHelpFileName"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetHelpFileName (BSTR Name)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::SetHelpFileName"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = E_NOTIMPL;
/**/
	lResult = S_FALSE;
/**/
	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::SetHelpFileName"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetHelpModeOn (long HelpModeOn)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::SetHelpModeOn"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = E_NOTIMPL;
/**/
	lResult = S_FALSE;
/**/
	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::SetHelpModeOn"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetHelpModeOn (long *HelpModeOn)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetHelpModeOn"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = E_NOTIMPL;
/**/
	lResult = S_FALSE;
	if	(HelpModeOn)
	{
		(*HelpModeOn) = FALSE;
	}
/**/
	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetHelpModeOn"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetHelpContextID (long ContextID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::SetHelpContextID"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = E_NOTIMPL;
/**/
	lResult = S_FALSE;
/**/
	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::SetHelpContextID"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetHelpContextID (long *ContextID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetHelpContextID"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = E_NOTIMPL;
/**/
	lResult = S_FALSE;
	if	(ContextID)
	{
		(*ContextID) = 0;
	}
/**/
	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetHelpContextID"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetTTSSpeed (long *Speed)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetTTSSpeed"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!Speed)
	{
		lResult = E_POINTER;
	}
	else
	if	(!mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
#ifndef	_WIN64
	if	(
			(GetSapiVoice (true))
		&&	(mSapiVoice->SafeIsValid () == 4)
		)
	{
		(*Speed) = CDaSettingsConfig().ApplyVoiceRate (mFile->GetTts().mSpeed, 4);
	}
	else
#endif
	{
		(*Speed) = CDaSettingsConfig().ApplyVoiceRate (mFile->GetTts().mSpeed);
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetTTSSpeed"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetTTSPitch (short *Pitch)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetTTSPitch"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!Pitch)
	{
		lResult = E_POINTER;
	}
	else
	if	(!mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
#ifndef	_WIN64
	if	(
			(GetSapiVoice (true))
		&&	(mSapiVoice->SafeIsValid () == 4)
		)
	{
		(*Pitch) = mSapiVoice->GetPitch ();
	}
	else
#endif
	{
		(*Pitch) = mFile->GetTts().mPitch;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetTTSPitch"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetTTSModeID (BSTR *ModeID)
{
	return get_TTSModeID (ModeID);
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetTTSModeID (BSTR ModeID)
{
	return put_TTSModeID (ModeID);
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Speak (BSTR Text, BSTR Url, long *RequestID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::Speak [%s] [%s]"), this, m_dwRef, mCharID, DebugStr(CString(Text)), DebugStr(CString(Url)));
	}
#endif
	HRESULT	lResult = S_OK;
	long	lReqID = 0;

	if	(
			(!mWnd)
		||	(!mWnd->IsWindow ())
		)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	if	(!CDaSettingsConfig().LoadConfig().mTtsEnabled)
	{
		lResult = AGENTERR_SPEAKINGDISABLED;
	}
	else
#if	FALSE
	if	(
			(
				(Url == NULL)
			||	(*Url == NULL)
			)
		&&	(
				(Text == NULL)
			||	(*Text == NULL)
			)
		)
	{
		lResult = E_INVALIDARG;
	}
	else
#endif
	{
		IDaSvrBalloon2Ptr	lSvrBalloon (GetBalloonInterface (true));
		bool				lShowBalloon = (lSvrBalloon->get_Enabled (NULL) == S_OK);
		CString				lText (Text);
		CString				lSoundUrl (Url);
		CSapiVoice *		lVoice = NULL;
//
//	MS Agent shows the speech balloon silently when the character is listening.
//	For now, we'll just stop listening.
//
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("PreSpeak"), _T("%s\t%s"), EncodeTraceString(Text), DebugStr(Url));
#endif
		StopListening (false, ListenComplete_CharacterClientDeactivated);

		if	(lSoundUrl.IsEmpty ())
		{
			lVoice = GetSapiVoice (true);
			if	(!lVoice)
			{
				lResult = AGENTERR_TTSLANGUAGENOTFOUND;
			}
		}
		else
		{
			lVoice = GetSapiVoice (false);
			if	(lText.IsEmpty ())
			{
				lShowBalloon = false;
			}

			if	(mFile->IsRelativeFilePath (lSoundUrl))
			{
				lSoundUrl = mFile->ParseRelativePath (lSoundUrl);
			}
			else
			{
				lSoundUrl = mFile->ParseFilePath (lSoundUrl);
			}
			if	(mFile->IsProperFilePath (lSoundUrl))
			{
				if	(
						(PathIsURL (lSoundUrl))
					&&	(!_FindSoundDownload (lSoundUrl))
					)
				{
					lResult = DoPrepare (PrepareResource_Wave, lSoundUrl, false, lReqID);
				}
			}
			else
			{
				lResult = E_INVALIDARG;
			}
		}

		if	(SUCCEEDED (lResult))
		{
			if	(lShowBalloon)
			{
				GetBalloonWnd (true);
			}
			lReqID = mWnd->QueueSpeak (mCharID, lText, lSoundUrl, lVoice, lShowBalloon);
#ifdef	_TRACE_CHARACTER_ACTIONS
			_AtlModule.TraceCharacterAction (mCharID, _T("Speak"), _T("%s\t%s\t%d"), EncodeTraceString(Text), DebugStr(Url), lReqID);
#endif
			mWnd->ActivateQueue (true);
		}
	}
	if	(RequestID)
	{
		(*RequestID) = lReqID;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::Speak"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetSRModeID (BSTR *ModeID)
{
	return get_SRModeID (ModeID);
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetSRModeID (BSTR ModeID)
{
	return put_SRModeID (ModeID);
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Listen (long Listen)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::Listen [%d]"), this, m_dwRef, mCharID, Listen);
	}
#endif
	HRESULT	lResult;

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("Listen"), _T("%d"), Listen);
#endif
	if	(Listen)
	{
		lResult = StartListening (true);
	}
	else
	{
		lResult = StopListening (true, ListenComplete_ProgramDisabled);
	}
	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::Listen [%d]"), this, m_dwRef, mCharID, Listen);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetSRStatus (long *Status)
{
	ListeningStatusType	lStatus;
	HRESULT				lResult = get_ListeningStatus (&lStatus);
	
	if	(Status)
	{
		(*Status) = (long)lStatus;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_CharacterID (long *CharacterID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_CharacterID"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!CharacterID)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*CharacterID) = mCharID;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_CharacterID"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_UniqueID (BSTR *CharGUID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_UniqueID"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!CharGUID)
	{
		lResult = E_POINTER;
	}
	else
	if	(!mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		(*CharGUID) = ((CString)CGuidStr (mFile->GetGuid())).AllocSysString();
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_UniqueID"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_Name (BSTR *Name)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_Name"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT				lResult = S_OK;
	CAgentFileName *	lFileName;

	if	(!Name)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Name) = NULL;
		
		if	(!mFile)
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
		else
		if	(
				(mFile->GetNames().GetCount() <= 0)
			&&	(!mFile->ReadNames ())
			)
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
		else
		if	(lFileName = mFile->FindName (mLangID))
		{
			(*Name) = tBstrPtr (lFileName->mName).Detach();
		}
		else
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_Name"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::put_Name (BSTR Name)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::put_Name"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT				lResult = S_OK;
	CString				lName (Name);
	CAgentFileName *	lFileName;

	lName.TrimLeft ();
	lName.TrimRight ();

	if	(!mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	if	(lName.IsEmpty ())
	{
		lResult = E_INVALIDARG;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("put_Name"), _T("%ls"), Name);
#endif
		if	(lFileName = mFile->FindName (mLangID))
		{
			if	(CString ((BSTR)lFileName->mName) != lName)
			{
				lFileName->mName = lName.AllocSysString ();
				_AtlModule._OnCharacterNameChanged (mCharID);
			}
		}
		else
		{
			lResult = S_FALSE;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::put_Name"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_Description (BSTR *Description)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_Description"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT				lResult = S_OK;
	CAgentFileName *	lFileName;

	if	(!Description)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Description) = NULL;
		
		if	(!mFile)
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
		else
		if	(
				(mFile->GetNames().GetCount() <= 0)
			&&	(!mFile->ReadNames ())
			)
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
		else
		if	(lFileName = mFile->FindName (mLangID))
		{
			(*Description) = tBstrPtr (lFileName->mDesc1).Detach();
		}
		else
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_Description"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::put_Description (BSTR Description)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::put_Description"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT				lResult = S_OK;
	CString				lDescription (Description);
	CAgentFileName *	lFileName;

	lDescription.TrimLeft ();
	lDescription.TrimRight ();

	if	(!mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("put_Description"), _T("%ls"), Description);
#endif
		if	(lFileName = mFile->FindName (mLangID))
		{
			lFileName->mDesc1 = lDescription.AllocSysString ();
		}
		else
		{
			lResult = S_FALSE;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::put_Description"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_ExtraData (BSTR *ExtraData)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_ExtraData"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT				lResult = S_OK;
	CAgentFileName *	lFileName;

	if	(!ExtraData)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*ExtraData) = NULL;
		
		if	(!mFile)
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
		else
		if	(
				(mFile->GetNames().GetCount() <= 0)
			&&	(!mFile->ReadNames ())
			)
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
		else
		if	(lFileName = mFile->FindName (mLangID))
		{
			(*ExtraData) = tBstrPtr (lFileName->mDesc2).Detach();
		}
		else
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_ExtraData"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_FileName (BSTR *FileName)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_FileName"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!FileName)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*FileName) = NULL;
		
		if	(!mFile)
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
		else
		{
			(*FileName) = mFile->GetFileName().Detach();
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_FileName"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_FilePath (BSTR *FilePath)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_FilePath"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!FilePath)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*FilePath) = NULL;
		
		if	(!mFile)
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
		else
		{
			(*FilePath) = mFile->GetPath().Detach();
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_FilePath"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_LanguageID (long *LanguageID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_LanguageID"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!LanguageID)
	{
		lResult = E_POINTER;
	}
	else
	if	(!mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		(*LanguageID) = (long)mLangID;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_LanguageID"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::put_LanguageID (long LanguageID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::put_LanguageID"), this, m_dwRef, mCharID);
	}
#endif
#ifdef	_DEBUG_LANGUAGE
	if	(LogIsActive (_DEBUG_LANGUAGE))
	{
		LogMessage (_DEBUG_LANGUAGE, _T("[%p] [%d] DaSvrCharacter put_LanguageID [%4.4hX]"), this, mCharID, LanguageID);
	}
#endif
	HRESULT	lResult = S_OK;
	LANGID	lLangID = LOWORD (LanguageID);

	if	(!mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("put_LanguageID"), _T("%d"), LanguageID);
#endif
		if	(mLangID != lLangID)
		{
			lResult = SetLangID (lLangID);
			if	(SUCCEEDED (lResult))
			{
				_AtlModule._OnCharacterNameChanged (mCharID);
			}
		}
#ifndef	_STRICT_COMPATIBILITY
		else
		{
			lResult = S_FALSE;
		}
#endif
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::put_LanguageID"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_TTSModeID (BSTR *TTSModeID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_TTSModeID"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!TTSModeID)
	{
		lResult = E_POINTER;
	}
	else
	if	(GetSapiVoice (true))
	{
		(*TTSModeID) = mSapiVoice->GetUniqueId().Detach();
	}
	else
	{
		(*TTSModeID) = CString().AllocSysString();
		lResult = S_FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_TTSModeID"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::put_TTSModeID (BSTR TTSModeID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::put_TTSModeID [%ls]"), this, m_dwRef, mCharID, TTSModeID);
	}
#endif
	HRESULT				lResult = E_FAIL;
	CSapiVoiceCache *	lVoiceCache;

	if	(!CDaSettingsConfig().LoadConfig().mTtsEnabled)
	{
		lResult = AGENTERR_SPEAKINGDISABLED;
	}
	else
	if	(lVoiceCache = CSapiVoiceCache::GetStaticInstance ())
	{
		try
		{
			CSapiVoice *	lPrevVoice = mSapiVoice;

#ifdef	_TRACE_CHARACTER_ACTIONS
			_AtlModule.TraceCharacterAction (mCharID, _T("put_TTSModeID"), _T("%ls"), TTSModeID);
#endif
#ifdef	_STRICT_COMPATIBILITY
			try
			{
				if	(
						(mWnd->NextQueuedAction (mCharID))
					&&	(mWnd->IsSpeakQueued (mCharID) == mWnd->NextQueuedAction (mCharID))
					)
				{
					mWnd->RemoveQueuedAction (mWnd->NextQueuedAction (mCharID), AGENTREQERR_INTERRUPTEDCODE, _T("put_TTSModeID"));
				}
				if	(mSapiVoice)
				{
					mSapiVoice->Stop ();
				}
			}
			catch AnyExceptionDebug
#endif
			mSapiVoice = NULL;

			if	(GetSapiVoice (true, CString (TTSModeID)))
			{
				mWnd->UpdateQueuedSpeak (mCharID, mSapiVoice);

				if	(
						(lPrevVoice)
					&&	(lPrevVoice != mSapiVoice)
					)
				{
					lVoiceCache->RemoveVoiceClient (lPrevVoice, this);
				}
				lResult = S_OK;
			}
			else
			{
				mSapiVoice = lPrevVoice;
#ifdef	_STRICT_COMPATIBILITY_NOT
				lResult = AGENTERR_TTSLANGUAGENOTFOUND;
#else
				lResult = AGENTWARNING_TTSENGINENOTFOUND;
#endif
			}
		}
		catch AnyExceptionDebug
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::put_TTSModeID"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_SRModeID (BSTR *SRModeID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_SRModeID"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!SRModeID)
	{
		lResult = E_POINTER;
	}
	else
	if	(GetSapiInput (true))
	{
		(*SRModeID) = mSapiInput->GetEngineName().Detach();
	}
	else
	{
		(*SRModeID) = CString().AllocSysString();
		lResult = S_FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_SRModeID"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::put_SRModeID (BSTR SRModeID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::put_SRModeID [%ls]"), this, m_dwRef, mCharID, SRModeID);
	}
#endif
	HRESULT				lResult = E_FAIL;
	CSapiInputCache *	lInputCache;

	if	(!CDaSettingsConfig().LoadConfig().mTtsEnabled)
	{
		lResult = AGENTERR_SPEAKINGDISABLED;
	}
	else
	if	(lInputCache = CSapiInputCache::GetStaticInstance ())
	{
		try
		{
			CSapi5Input *	lPrevInput = mSapiInput;

#ifdef	_TRACE_CHARACTER_ACTIONS
			_AtlModule.TraceCharacterAction (mCharID, _T("put_SRModeID"), _T("%ls"), SRModeID);
#endif
			mSapiInput = NULL;

			if	(GetSapiInput (true, CString (SRModeID)))
			{
				StopListening (false, ListenComplete_CharacterClientDeactivated);
				SafeFreeSafePtr (mListeningState);

				if	(
						(lPrevInput)
					&&	(lPrevInput != mSapiInput)
					)
				{
					lInputCache->RemoveInputClient (lPrevInput, this);
				}
				lResult = S_OK;
			}
			else
			{
				mSapiInput = lPrevInput;
				lResult = AGENTERR_SRLANGUAGENOTFOUND;
			}
		}
		catch AnyExceptionDebug
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::put_SRModeID"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_Left (short *Left)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_Left"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;
	CRect	lWindowRect (0,0,0,0);

	if	(!Left)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(
				(!mWnd)
			||	(!mWnd->IsWindow ())
			)
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
		else
		{
			mWnd->GetWindowRect (&lWindowRect);
		}
		(*Left) = (short)lWindowRect.left;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_Left"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::put_Left (short Left)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::put_Left [%hd]"), this, m_dwRef, mCharID, Left);
	}
#endif
	HRESULT	lResult = S_OK;
	CRect	lWindowRect;

	if	(
			(!mWnd)
		||	(!mWnd->IsWindow ())
		)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("put_Left"), _T("%hd"), Left);
#endif
		mWnd->GetWindowRect (&lWindowRect);
		if	(!mWnd->MovePopup (CPoint (Left, lWindowRect.top), 0, MoveCause_ProgramMoved))
		{
			lResult = S_FALSE;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::put_Left"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_Top (short *Top)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_Top"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;
	CRect	lWindowRect (0,0,0,0);

	if	(!Top)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(
				(!mWnd)
			||	(!mWnd->IsWindow ())
			)
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
		else
		{
			mWnd->GetWindowRect (&lWindowRect);
		}
		(*Top) = (short)lWindowRect.top;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_Top"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::put_Top (short Top)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::put_Top [%hd]"), this, m_dwRef, mCharID, Top);
	}
#endif
	HRESULT	lResult = S_OK;
	CRect	lWindowRect;

	if	(
			(!mWnd)
		||	(!mWnd->IsWindow ())
		)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("put_Top"), _T("%hd"), Top);
#endif
		mWnd->GetWindowRect (&lWindowRect);
		if	(!mWnd->MovePopup (CPoint (lWindowRect.left, Top), 0, MoveCause_ProgramMoved))
		{
			lResult = S_FALSE;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::put_Top"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_Width (short *Width)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_Width"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;
	CRect	lWindowRect (0,0,0,0);

	if	(!Width)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(
				(!mWnd)
			||	(!mWnd->IsWindow ())
			)
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
		else
		{
			mWnd->GetWindowRect (&lWindowRect);
		}
		(*Width) = (short)lWindowRect.Width();
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_Width"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::put_Width (short Width)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::put_Width [%hd]"), this, m_dwRef, mCharID, Width);
	}
#endif
	HRESULT	lResult = S_OK;
	CRect	lWindowRect;

	if	(
			(!mWnd)
		||	(!mWnd->IsWindow ())
		)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("put_Width"), _T("%hd"), Width);
#endif
		mWnd->GetWindowRect (&lWindowRect);
		if	(!mWnd->SizePopup (CSize (Width, lWindowRect.Height()), 0))
		{
			lResult = S_FALSE;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::put_Width"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_Height (short *Height)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_Height"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;
	CRect	lWindowRect (0,0,0,0);

	if	(!Height)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(
				(!mWnd)
			||	(!mWnd->IsWindow ())
			)
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
		else
		{
			mWnd->GetWindowRect (&lWindowRect);
		}
		(*Height) = (short)lWindowRect.Height();
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_Height"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::put_Height (short Height)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::put_Height [%hd]"), this, m_dwRef, mCharID, Height);
	}
#endif
	HRESULT	lResult = S_OK;
	CRect	lWindowRect;

	if	(
			(!mWnd)
		||	(!mWnd->IsWindow ())
		)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("put_Height"), _T("%hd"), Height);
#endif
		mWnd->GetWindowRect (&lWindowRect);
		if	(!mWnd->SizePopup (CSize (lWindowRect.Width(), Height), 0))
		{
			lResult = S_FALSE;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::put_Height"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_OriginalWidth (short *OriginalWidth)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_OriginalWidth"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!OriginalWidth)
	{
		lResult = E_POINTER;
	}
	else
	if	(!mFile)
	{
		(*OriginalWidth) = 0;
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		(*OriginalWidth) = (short)mFile->GetImageSize ().cx;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_OriginalWidth"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_OriginalHeight (short *OriginalHeight)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_OriginalHeight"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!OriginalHeight)
	{
		lResult = E_POINTER;
	}
	else
	if	(!mFile)
	{
		(*OriginalHeight) = 0;
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		(*OriginalHeight) = (short)mFile->GetImageSize ().cy;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_OriginalHeight"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_Visible (VARIANT_BOOL *Visible)
{
#ifdef	_DEBUG_INTERFACE_NOT
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_Visible"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!Visible)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Visible) = VARIANT_FALSE;
		
		if	(
				(!mWnd)
			||	(!mWnd->IsWindow ())
			)
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
		else
		{
			(*Visible) = IsVisible (true) ? VARIANT_TRUE : VARIANT_FALSE;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_Visible"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_ActiveState (ActiveStateType *ActiveState)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_ActiveState"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT			lResult = S_OK;
	ActiveStateType	lActiveState = ActiveState_Inactive;

	if	(!mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		if	(IsClientActive ())
		{
			if	(
					(mWnd)
				&&	(mWnd->IsWindow ())
				&&	(mWnd->m_hWnd == CAgentPopupWnd::GetLastActive())
				)
			{
				lActiveState = ActiveState_InputActive;
			}
			else
			{
				lActiveState = ActiveState_Active;
			}
		}
		else
		{
			lResult = S_FALSE;
		}
	}
	if	(ActiveState)
	{
		(*ActiveState) = lActiveState;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_ActiveState"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::put_ActiveState (ActiveStateType ActiveState)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::put_ActiveState [%hd]"), this, m_dwRef, mCharID, ActiveState);
	}
#endif
#ifdef	_DEBUG_ACTIVE
	if	(LogIsActive (_DEBUG_ACTIVE))
	{
		LogMessage (_DEBUG_ACTIVE, _T("[%d] put_ActiveState [%hd]"), mCharID, ActiveState);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(ActiveState == ActiveState_Inactive)
	{
		if	(!SetClientActive (false, false))
		{
			if	(
					(mWnd)
				&&	(mWnd->IsWindow ())
				)
			{
				lResult = S_FALSE;
			}
			else
			{
				lResult = E_INVALIDARG;
			}
		}
	}
	else
	if	(
			(!mWnd)
		||	(!mWnd->IsWindow ())
		)
	{
#ifdef	_STRICT_COMPATIBILITY
		lResult = E_INVALIDARG;
#else
		lResult = AGENTERR_CHARACTERINVALID;
#endif
	}
	else
	if	(ActiveState == ActiveState_Active)
	{
		SetClientActive (true, false);
	}
	else
	if	(ActiveState == ActiveState_InputActive)
	{
		if	(mWnd->GetStyle() & WS_VISIBLE)
		{
			SetClientActive (true, true);
		}
		else
		{
			lResult = S_FALSE;
		}
	}
	else
	{
		lResult = E_INVALIDARG;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::put_ActiveState"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_IdleState (VARIANT_BOOL *IdleState)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_IdleState"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult;

	if	(
			(!mWnd)
		||	(!mWnd->IsWindow ())
		)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		lResult = (mWnd->IsIdle()) ? S_OK : S_FALSE;
	}
	if	(IdleState)
	{
		(*IdleState) = (lResult == S_OK) ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_IdleState"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_OtherClientCount (long *OtherClientCount)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_OtherClientCount"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;
	long	lClientCount = 0;

	if	(!mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		lClientCount = (long)GetClientCount (mCharID);
		if	(lClientCount == 0)
		{
#ifndef	_STRICT_COMPATIBILITY
			lResult = AGENTWARNING_ONLYCLIENT;
#endif
		}
		else
		if	(lClientCount < 0)
		{
			lClientCount = 0;
			lResult = E_FAIL;
		}
		if	(OtherClientCount)
		{
			(*OtherClientCount) = lClientCount;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_OtherClientCount"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_MoveCause (MoveCauseType *MoveCause)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_MoveCause"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!MoveCause)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*MoveCause) = MoveCause_NeverMoved;
			
		if	(
				(!mWnd)
			||	(!mWnd->IsWindow ())
			)
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
		else
		{
			(*MoveCause) = mNotify->_GetMoveCause (mCharID);
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_MoveCause"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_VisibilityCause (VisibilityCauseType *VisibilityCause)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_VisibilityCause"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!VisibilityCause)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*VisibilityCause) = VisibilityCause_NeverShown;

		if	(
				(!mWnd)
			||	(!mWnd->IsWindow ())
			)
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
		else
		{
			(*VisibilityCause) = mNotify->_GetVisibilityCause (mCharID);
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_VisibilityCause"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_ListeningStatus (ListeningStatusType *ListeningStatus)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_ListeningStatus"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!ListeningStatus)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*ListeningStatus) = ListeningStatus_Available;

		if	(!CDaSettingsConfig().LoadConfig().mSrEnabled)
		{
			(*ListeningStatus) = ListeningStatus_InputDisabled;
		}
		else
		if	(mNotify->_GetActiveCharacter() != GetCharID())
		{
			(*ListeningStatus) = ListeningStatus_CharacterInactive;
		}
		else
		if	(!GetSapiInput (true))
		{
			(*ListeningStatus) = ListeningStatus_InitializeFailed;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_ListeningStatus"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_HasIcon (VARIANT_BOOL *HasIcon)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_HasIcon"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(HasIcon)
	{
		(*HasIcon) = FALSE;
		if	(
				(mWnd)
			&&	(mWnd->IsWindow ())
			)
		{
			(*HasIcon) = mWnd->IsNotifyIconValid()?VARIANT_TRUE:VARIANT_FALSE;
		}
	}
	else
	{
		lResult = E_POINTER;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_HasIcon"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_Style (long *Style)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_Style"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(Style)
	{
		(*Style) = 0;
		if	(IsIdleEnabled ())
		{
			(*Style) |= CharacterStyle_IdleEnabled;
		}
		if	(IsSoundEnabled ())
		{
			(*Style) |= CharacterStyle_SoundEffects;
		}
		if	(IsAutoPopupMenu ())
		{
			(*Style) |= CharacterStyle_AutoPopupMenu;
		}
		if	(IsIconShown ())
		{
			(*Style) |= CharacterStyle_IconShown;
		}
		if	(mWnd)
		{
			if	(mWnd->mAlphaSmoothing == RenderSmoothAll)
			{
				(*Style) |= CharacterStyle_Smoothed;
			}
			else
			if	(mWnd->mAlphaSmoothing == RenderSmoothEdges)
			{
				(*Style) |= CharacterStyle_SmoothEdges;
			}
		}
	}
	else
	{
		lResult = E_POINTER;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_Style"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::put_Style (long Style)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::put_IconState [%u]"), this, m_dwRef, mCharID, Style);
	}
#endif
	HRESULT	lResult = S_OK;

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("put_Style"), _T("%8.8X"), Style);
#endif
	lResult = SetStyle (~Style, Style);

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::put_IconState"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_IconVisible (VARIANT_BOOL *IconVisible)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_IconVisible"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(IconVisible)
	{
		(*IconVisible) = IsIconVisible()?VARIANT_TRUE:VARIANT_FALSE;
	}
	else
	{
		lResult = E_POINTER;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_IconVisible"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GenerateIcon (long ClipLeft, long ClipTop, long ClipWidth, long ClipHeight)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GenerateIcon"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!mIconData.mGenerateIcon)
	{
		bool	lIconShown = IsIconShown();

		mIconData.mGenerateIcon = true;
		mIconData.mGenerateIconClip = CRect (CPoint (ClipLeft, ClipTop), CSize (ClipWidth, ClipHeight));
		ShowIcon (false);
		lResult = ShowIcon (lIconShown);
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GenerateIcon"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetIconIdentity (GUID *IconIdentity)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetIconIdentity"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!IconIdentity)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*IconIdentity) = mIconData.mIdentity;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetIconIdentity"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetIconIdentity (const GUID *IconIdentity)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::SetIconIdentity"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult;

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("SetIconIdentity"), _T("%s"), (CString)CGuidStr(IconIdentity?*IconIdentity:GUID_NULL));
#endif

	if	(IconIdentity)
	{
		mIconData.mIdentity = (*IconIdentity);
	}
	else
	{
		mIconData.mIdentity = GUID_NULL;
	}
	lResult = ShowIcon (IsIconShown());

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::SetIconIdentity"), this, m_dwRef, mCharID);
	}

#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_IconIdentity (BSTR *IconIdentity)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_IconIdentity"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!IconIdentity)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*IconIdentity) = ((CString)CGuidStr(mIconData.mIdentity)).AllocSysString ();
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_IconIdentity"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::put_IconIdentity (BSTR IconIdentity)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::put_IconIdentity"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult;

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("put_IconIdentity"), _T("%s"), CString(IconIdentity));
#endif
	lResult = SetIconIdentity (&CGuidStr::Parse (CString (IconIdentity)));

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::put_IconIdentity"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_IconTip (BSTR *IconTip)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_IconTip"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!IconTip)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*IconTip) = mIconData.mTip.AllocSysString ();
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_IconTip"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::put_IconTip (BSTR IconTip)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::put_IconTip"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult;

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("put_IconTip"), _T("%s"), CString(IconTip));
#endif
	mIconData.mTip = IconTip;
	lResult = ShowIcon (IsIconShown());

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::put_IconTip"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_TTSEngine (VARIANT_BOOL GetDefault, IDaSvrTTSEngine **TTSEngine)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_TTSEngine"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT				lResult = E_FAIL;
	DaSvrTTSEngine *	lTTSEngine = NULL;
	IDaSvrTTSEnginePtr	lInterface;
	CSapiVoiceCache *	lVoiceCache;
	CSapi5Voices *		lSapi5Voices;
	CSapi5VoiceInfo *	lSapi5Voice;
#ifndef	_WIN64
	CSapi4Voices *		lSapi4Voices;
	CSapi4VoiceInfo *	lSapi4Voice;
#endif

	if	(!TTSEngine)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*TTSEngine) = NULL;

		if	(GetDefault)
		{
			lResult = GetDefaultTTSEngine (mFile, TTSEngine, mClientMutexName);
		}
		else
		if	(lVoiceCache = CSapiVoiceCache::GetStaticInstance ())
		{
			CSapiVoice *	lSapiVoice;

			if	(lSapiVoice = GetSapiVoice (true))
			{
				if	(
						(lSapi5Voices = lVoiceCache->GetSapi5Voices ())
					&&	(lSapi5Voice = lSapi5Voices->GetVoiceId (CString (lSapiVoice->GetUniqueId ())))
					)
				{
					lTTSEngine = DaSvrTTSEngine::CreateInstance (lSapi5Voice, mClientMutexName);
				}
#ifndef	_WIN64
				else
				if	(
						(lSapi4Voices = lVoiceCache->GetSapi4Voices ())
					&&	(lSapi4Voice = lSapi4Voices->GetModeId (CGuidStr::Parse (lSapiVoice->GetUniqueId ())))
					)
				{
					lTTSEngine = DaSvrTTSEngine::CreateInstance (lSapi4Voice, mClientMutexName);
				}
#endif
			}

			if	(lTTSEngine)
			{
				lInterface = lTTSEngine->GetControllingUnknown();
				(*TTSEngine) = lInterface.Detach();
				lResult = S_OK;
			}
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_TTSEngine"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::FindTTSEngines (long LanguageID, IDaSvrTTSEngines **TTSEngines)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::FindTTSEngines"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = E_FAIL;

	if	(!TTSEngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*TTSEngines) = NULL;

		if	(mFile)
		{
			lResult = FindTTSEngines (mFile, (LANGID)LanguageID, SpeechGender_Neutral, TTSEngines, mClientMutexName);
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::FindTTSEngines"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_SREngine (VARIANT_BOOL GetDefault, IDaSvrSREngine **SREngine)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::get_SREngine"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT				lResult = E_FAIL;
	DaSvrSREngine *		lSREngine = NULL;
	IDaSvrSREnginePtr	lInterface;
	CSapiInputCache *	lInputCache;
	CSapi5Inputs *		lSapi5Inputs;
	CSapi5InputInfo *	lSapi5Input;

	if	(!SREngine)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SREngine) = NULL;

		if	(GetDefault)
		{
			lResult = GetDefaultSREngine (mFile, SREngine, mClientMutexName);
		}
		else
		if	(lInputCache = CSapiInputCache::GetStaticInstance ())
		{
			CSapi5Input *	lSapiInput;

			if	(lSapiInput = GetSapiInput (true))
			{
				if	(
						(lSapi5Inputs = lInputCache->GetSapi5Inputs ())
					&&	(lSapi5Input = lSapi5Inputs->GetEngineId (CString (lSapiInput->GetEngineId ())))
					)
				{
					lSREngine = DaSvrSREngine::CreateInstance (lSapi5Input, mClientMutexName);
				}
			}

			if	(lSREngine)
			{
				lInterface = lSREngine->GetControllingUnknown();
				(*SREngine) = lInterface.Detach();
				lResult = S_OK;
			}
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::get_SREngine"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::FindSREngines (long LanguageID, IDaSvrSREngines **SREngines)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::FindSREngines"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = E_FAIL;

	if	(!SREngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SREngines) = NULL;

		if	(mFile)
		{
			lResult = FindSREngines (mFile, (LANGID)LanguageID, SREngines, mClientMutexName);
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::FindSREngines"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}
