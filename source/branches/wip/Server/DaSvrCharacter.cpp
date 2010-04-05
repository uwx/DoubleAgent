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
#include "DaSvrSpeechEngine.h"
#include "DaSvrSpeechEngines.h"
#include "DaSvrRecognitionEngine.h"
#include "DaSvrRecognitionEngines.h"
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

DaSvrCharacter * DaSvrCharacter::CreateInstance (long pCharID, CAgentFile * pFile, CAgentFileCache * pUsedFileCache, _IServerNotify * pNotify, LPCTSTR pClientMutexName)
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
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%d] DaSvrCharacter::CreateInstance (%d) [%ls]"), lInstance, lInstance->m_dwRef, lInstance->mCharID, _AtlModule.GetLockCount(), ((pFile) ? (BSTR)pFile->GetPath() : NULL));
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
		if	(lInstance->mFile = pFile)
		{
			if	(!lInstance->mUsedFileCache->AddFileClient (lInstance->mFile, lInstance))
			{
				lInstance->mUsedFileCache->CacheFile (lInstance->mFile, lInstance);
			}
			if	(!_AtlModule.AddFileClient (lInstance->mFile, lInstance))
			{
				_AtlModule.CacheFile (lInstance->mFile, lInstance);
			}
			lInstance->ShowIcon (lInstance->mIconData.mShowIcon);
			lInstance->OpenFile ();
		}

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

void DaSvrCharacter::OpenFile ()
{
	tPtr <CAgentPopupWnd>				lNewWnd;
	CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
	int									lClientNdx;

	if	(
			(mFile)
		&&	(_AtlModule.GetFileClients (mFile, lFileClients))
		)
	{
		for	(lClientNdx = 0; lClientNdx <= lFileClients.GetUpperBound(); lClientNdx++)
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
			(mFile)
		&&	(
				(!mWnd)
			||	(!mWnd->IsWindow ())
			)
		&&	(lNewWnd = CAgentPopupWnd::CreateInstance())
		&&	(lNewWnd->Create (NULL))
		)
	{
		lNewWnd->ModifyStyle (WS_CAPTION|WS_THICKFRAME|WS_SYSMENU, 0, SWP_FRAMECHANGED);
		lNewWnd->ModifyStyleEx (0, WS_EX_TOOLWINDOW);

		if	(SUCCEEDED (lNewWnd->Open (mFile)))
		{
			lNewWnd->EnableIdle (mIdleEnabled);
			lNewWnd->EnableSound (mSoundEnabled);
			mWnd = lNewWnd.Detach ();
		}
	}

	if	(
			(mFile)
		&&	(mWnd)
		&&	(mWnd->IsWindow ())
		)
	{
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
			int									lClientNdx;

			if	(
					(mFile)
				&&	(_AtlModule.GetFileClients (mFile, lFileClients))
				)
			{
				for	(lClientNdx = 0; lClientNdx <= lFileClients.GetUpperBound(); lClientNdx++)
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

int DaSvrCharacter::GetClientCount (int pSkipCharID) const
{
	int	lRet = -1;

	try
	{
		CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
		int									lClientNdx;
		DaSvrCharacter *					lCharacter;

		if	(
				(mFile)
			&&	(_AtlModule.GetFileClients (mFile, lFileClients))
			)
		{
			lRet = 0;

			for	(lClientNdx = 0; lClientNdx <= lFileClients.GetUpperBound(); lClientNdx++)
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
				||	(CDaAudioOutputConfig().LoadConfig().mEffectsEnabled)
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
			LogMessage (_DEBUG_LISTEN, _T("[%d] StartListening Manual [%u] Listening [%u] Active [%u] Enabled [%u]"), mCharID, pManual, IsListening(), (mNotify->_GetActiveCharacter()==GetCharID()), CDaSpeechInputConfig().LoadConfig().mEnabled);
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
				StopAll (StopType_Play|StopType_Move|StopType_Speak|StopType_QueuedPrepare, AGENTREQERR_INTERRUPTEDLISTENKEY);
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
				StopAll (StopType_Play|StopType_Move|StopType_Speak|StopType_QueuedPrepare, AGENTREQERR_INTERRUPTEDHEARING);

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

long DaSvrCharacter::GetListeningStatus ()
{
	long	lStatus = ListenStatus_Available;

	if	(!CDaSpeechInputConfig().LoadConfig().mEnabled)
	{
		lStatus = ListenStatus_SpeechDisabled;
	}
	else
	if	(mNotify->_GetActiveCharacter() != GetCharID())
	{
		lStatus = ListenStatus_CharacterInactive;
	}
	else
	if	(!GetSapiInput (true))
	{
		lStatus = ListenStatus_InitializeFailed;
	}
	return lStatus;
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

	if	(pStopTypes & StopType_Play)
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

	if	(pStopTypes & StopType_Speak)
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

	if	(pStopTypes & StopType_Move)
	{
		if	(mWnd->RemoveQueuedMove (mCharID, pReqStatus, _T("StopAll"), lExcludeActive))
		{
			lResult = S_OK;
		}
	}

	if	(pStopTypes & StopType_Visibility)
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

	if	(pStopTypes & StopType_QueuedPrepare)
	{
		if	(mWnd->RemoveQueuedPrepare (mCharID, pReqStatus, _T("StopAll"), lExcludeActive))
		{
			lResult = S_OK;
		}
	}

	if	(
			(pStopTypes & StopType_ImmediatePrepate)
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
					(pType == PrepareType_Animation)
				||	(pType == PrepareType_State)
				)
			&&	(!mFile->IsAcfFile ())
			)
		{
			lResult = S_FALSE;
		}
		else
		if	(
				(pType != PrepareType_Animation)
			&&	(pType != PrepareType_State)
			&&	(pType != PrepareType_Wave)
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
				if	(pType == PrepareType_Animation)
				{
					lResult = lPrepare->PutAnimationNames (mFile, pName, mNotify);
				}
				else
				if	(pType == PrepareType_State)
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
			StopAll (StopType_Play|StopType_Move|StopType_Speak|StopType_QueuedPrepare, AGENTREQERR_INTERRUPTEDUSER);
			mWnd->QueueHide (mCharID, false, VisibilityCause_UserHid);
			mWnd->ActivateQueue (true);
			lRet = true;
		}
		else
		if	(pCommandId == lCommands->mShowCharacterCmdId)
		{
			StopAll (StopType_Play|StopType_Move|StopType_Speak|StopType_QueuedPrepare, AGENTREQERR_INTERRUPTEDUSER);
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

DaSvrBalloon * DaSvrCharacter::GetBalloonObj (bool pCreateObject)
{
	DaSvrBalloon *		lRet = NULL;
	IDaSvrBalloonPtr	lInterface;

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

IDaSvrBalloon * DaSvrCharacter::GetBalloonInterface (bool pCreateObject)
{
	IDaSvrBalloonPtr	lInterface;

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
			IDaSvrBalloonPtr	lBalloonInterface (GetBalloonInterface (true));

			if	(
					(lBalloonInterface->GetEnabled (NULL) == S_OK)
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
			||	(CDaSpeechInputConfig().LoadConfig().mListeningTip)
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
			for	(lClientNdx = lFileClients.GetUpperBound(); lClientNdx >= 0; lClientNdx--)
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
	CDaSpeechInputConfig	lInputConfig;
	CAgentListeningWnd *	lListeningWnd;
	CAgentBalloonWnd *		lBalloonWnd;

	lInputConfig.LoadConfig ();

	if	(!lInputConfig.mEnabled)
	{
		StopListening (false, ListenComplete_UserDisabled);
		ReleaseSapiVoice ();
	}
	if	(
			(!lInputConfig.mListeningTip)
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
		IDaSvrBalloonPtr	lBalloonInterface (GetBalloonInterface (true));

		if	(lBalloonInterface->GetEnabled (NULL) == S_FALSE)
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

HRESULT DaSvrCharacter::GetDefaultSpeechEngine (CAgentFile * pFile, IDaSvrSpeechEngine ** pSpeechEngine, LPCTSTR pClientMutexName)
{
	HRESULT					lResult = S_FALSE;
	DaSvrSpeechEngine *		lSpeechEngine = NULL;
	IDaSvrSpeechEnginePtr	lInterface;
	CSapiVoiceCache *		lVoiceCache;
	CSapi5Voices *			lSapi5Voices;
	CSapi5VoiceInfo *		lSapi5Voice;
#ifndef	_WIN64
	CSapi4Voices *			lSapi4Voices;
	CSapi4VoiceInfo *		lSapi4Voice;
#endif

	if	(
			(pFile)
		&&	(pSpeechEngine)
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
				lSpeechEngine = DaSvrSpeechEngine::CreateInstance (lSapi5Voice, pClientMutexName);
			}
#ifndef	_WIN64
			else
			if	(
					(lSapi4Voices = lVoiceCache->GetSapi4Voices ())
				&&	(lSapi4Voice = lSapi4Voices->GetModeId (CGuidStr::Parse (lSapiVoice->GetUniqueId ())))
				)
			{
				lSpeechEngine = DaSvrSpeechEngine::CreateInstance (lSapi4Voice, pClientMutexName);
			}
#endif
		}

		if	(lSpeechEngine)
		{
			lInterface = lSpeechEngine->GetControllingUnknown ();
			(*pSpeechEngine) = lInterface.Detach();
			lResult = S_OK;
		}
	}
	else
	{
		lResult = E_FAIL;
	}
	return lResult;
}

HRESULT DaSvrCharacter::FindSpeechEngines (CAgentFile * pFile, LANGID pLangId, short pGender, IDaSvrSpeechEngines ** pSpeechEngines, LPCTSTR pClientMutexName)
{
	HRESULT					lResult = S_FALSE;
	DaSvrSpeechEngines *	lSpeechEngines;
	IDaSvrSpeechEnginesPtr	lInterface;
	CSapiVoiceCache *		lVoiceCache;
	CSapi5Voices *			lSapi5Voices;
	INT_PTR					lSapi5VoiceNdx = -1;
#ifndef	_WIN64
	CSapi4Voices *			lSapi4Voices;
	INT_PTR					lSapi4VoiceNdx = -1;
#endif

	if	(
			(pSpeechEngines)
		&&	(lVoiceCache = CSapiVoiceCache::GetStaticInstance ())
		)
	{
		if	(lSpeechEngines = DaSvrSpeechEngines::CreateInstance (pClientMutexName))
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
					lSpeechEngines->mSapi5Voices.Add (lSapi5Voices->GetAt (lSapi5VoiceNdx));
				}
			}
#ifndef	_WIN64
			if	(lSapi4Voices = lVoiceCache->GetSapi4Voices ())
			{
				while ((lSapi4VoiceNdx = lSapi4Voices->FindVoice (lFileTts, (pLangId==0), lSapi4VoiceNdx)) >= 0)
				{
					lSpeechEngines->mSapi4Voices.Add (lSapi4Voices->GetAt (lSapi4VoiceNdx));
				}
			}
#endif
			lInterface = lSpeechEngines->GetControllingUnknown();
			(*pSpeechEngines) = lInterface.Detach();
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

HRESULT DaSvrCharacter::GetDefaultRecognitionEngine (CAgentFile * pFile, IDaSvrRecognitionEngine ** pRecognitionEngine, LPCTSTR pClientMutexName)
{
	HRESULT						lResult = S_FALSE;
	DaSvrRecognitionEngine *	lRecognitionEngine = NULL;
	IDaSvrRecognitionEnginePtr	lInterface;
	CSapiInputCache *			lInputCache;
	CSapi5Inputs *				lSapi5Inputs;
	CSapi5InputInfo *			lSapi5Input;

	if	(
			(pFile)
		&&	(pRecognitionEngine)
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
			lRecognitionEngine = DaSvrRecognitionEngine::CreateInstance (lSapi5Input, pClientMutexName);
		}

		if	(lRecognitionEngine)
		{
			lInterface = lRecognitionEngine->GetControllingUnknown();
			(*pRecognitionEngine) = lInterface.Detach();
			lResult = S_OK;
		}
	}
	else
	{
		lResult = E_FAIL;
	}
	return lResult;
}

HRESULT DaSvrCharacter::FindRecognitionEngines (CAgentFile * pFile, LANGID pLangId, IDaSvrRecognitionEngines ** pRecognitionEngines, LPCTSTR pClientMutexName)
{
	HRESULT						lResult = S_FALSE;
	DaSvrRecognitionEngines *	lRecognitionEngines;
	IDaSvrRecognitionEnginesPtr	lInterface;
	CSapiInputCache *			lInputCache;
	CSapi5Inputs *				lSapi5Inputs;
	INT_PTR						lSapi5InputNdx = -1;

	if	(
			(pRecognitionEngines)
		&&	(lInputCache = CSapiInputCache::GetStaticInstance ())
		)
	{
		if	(lRecognitionEngines = DaSvrRecognitionEngines::CreateInstance (pClientMutexName))
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
					lRecognitionEngines->mSapi5Inputs.Add (lSapi5Inputs->GetAt (lSapi5InputNdx));
				}
			}

			lInterface = lRecognitionEngines->GetControllingUnknown();
			(*pRecognitionEngines) = lInterface.Detach();
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
	LogMessage (_DEBUG_COM, _T("[%p(%d)] DaSvrCharacter::GetClassForHandler [%8.8X] [%s]"), this, m_dwRef, dwDestContext, CGuidStr::GuidName(*pClsid));
#endif
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetPosition (long lLeft, long lTop)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::SetPosition [%d %d]"), this, m_dwRef, mCharID, lLeft, lTop);
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
		_AtlModule.TraceCharacterAction (mCharID, _T("SetPosition"), _T("%d\t%d"), lLeft, lTop);
#endif
#ifdef	_STRICT_COMPATIBILITY
		if	(!mWnd->MovePopup (CPoint (lLeft, lTop), 0, MoveCause_ProgramMoved, true))
#else
		if	(!mWnd->MovePopup (CPoint (lLeft, lTop), 0, MoveCause_ProgramMoved))
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetPosition (long *plLeft, long *plTop)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetPosition"), this, m_dwRef, mCharID);
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
		mWnd->GetWindowRect (&lWindowRect);
		if	(plLeft)
		{
			(*plLeft) = lWindowRect.left;
		}
		if	(plTop)
		{
			(*plTop) = lWindowRect.top;
		}
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetSize (long lWidth, long lHeight)
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
		_AtlModule.TraceCharacterAction (mCharID, _T("SetSize"), _T("%d\t%d"), lWidth, lHeight);
#endif
		if	(!mWnd->SizePopup (CSize (lWidth, lHeight), 0))
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetSize (long *plWidth, long *plHeight)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetSize"), this, m_dwRef, mCharID);
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
		mWnd->GetWindowRect (&lWindowRect);
		if	(plWidth)
		{
			(*plWidth) = lWindowRect.Width();
		}
		if	(plHeight)
		{
			(*plHeight) = lWindowRect.Height();
		}
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetOriginalSize (long *plWidth, long *plHeight)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetOriginalSize"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
		CSize	lImageSize = mFile->GetImageSize ();

		if	(plWidth)
		{
			(*plWidth) = lImageSize.cx;
		}
		if	(plHeight)
		{
			(*plHeight) = lImageSize.cy;
		}
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetGUID (BSTR *pbszID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetGUID"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!pbszID)
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
		(*pbszID) = ((CString)CGuidStr (mFile->GetGuid())).AllocSysString();
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetGUID"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetVersion(short *psMajor, short *psMinor)
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
		if	(psMajor)
		{
			(*psMajor) = HIWORD (mFile->GetVersion());
		}
		if	(psMinor)
		{
			(*psMinor) = LOWORD (mFile->GetVersion());
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetName (BSTR *pbszName)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetName"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT				lResult = S_OK;
	CAgentFileName *	lFileName;

	if	(!mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	if	(!pbszName)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(
				(mFile->GetNames().GetSize() <= 0)
			&&	(!mFile->ReadNames ())
			)
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
		else
		if	(lFileName = mFile->FindName (mLangID))
		{
			(*pbszName) = tBstrPtr (lFileName->mName).Detach();
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetName"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetName (BSTR bszName)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::SetName"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT				lResult = S_OK;
	CString				lName (bszName);
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
		_AtlModule.TraceCharacterAction (mCharID, _T("SetName"), _T("%ls"), bszName);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::SetName"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetDescription (BSTR *pbszDescription)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetDescription"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT				lResult = S_OK;
	CAgentFileName *	lFileName;

	if	(!mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	if	(!pbszDescription)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(
				(mFile->GetNames().GetSize() <= 0)
			&&	(!mFile->ReadNames ())
			)
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
		else
		if	(lFileName = mFile->FindName (mLangID))
		{
			(*pbszDescription) = tBstrPtr (lFileName->mDesc1).Detach();
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetDescription"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetDescription (BSTR bszDescription)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::SetDescription"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT				lResult = S_OK;
	CString				lDescription (bszDescription);
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
		_AtlModule.TraceCharacterAction (mCharID, _T("SetDescription"), _T("%ls"), bszDescription);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::SetDescription"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetLanguageID (long langid)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::SetLanguageID"), this, m_dwRef, mCharID);
	}
#endif
#ifdef	_DEBUG_LANGUAGE
	if	(LogIsActive (_DEBUG_LANGUAGE))
	{
		LogMessage (_DEBUG_LANGUAGE, _T("[%p] [%d] DaSvrCharacter SetLanguageID [%4.4hX]"), this, mCharID, langid);
	}
#endif
	HRESULT	lResult = S_OK;
	LANGID	lLangID = LOWORD (langid);

	if	(!mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("SetLanguageID"), _T("%d"), langid);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::SetLanguageID"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetLanguageID (long *plangid)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetLanguageID"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!plangid)
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
		(*plangid) = (long)mLangID;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetLanguageID"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetIdleOn (long bOn)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::SetIdleOn [%d]"), this, m_dwRef, mCharID, bOn);
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
		_AtlModule.TraceCharacterAction (mCharID, _T("SetIdleOn"), _T("%d"), bOn);
#endif
		if	(bOn)
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetIdleOn (long *pbOn)
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
	if	(!pbOn)
	{
#ifdef _STRICT_COMPATIBILITY
		lResult = E_POINTER;
#endif
	}
	else
	{
		(*pbOn) = (IsIdleEnabled()!=false);
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetSoundEffectsOn (long bOn)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::SetSoundEffectsOn [%d]"), this, m_dwRef, mCharID, bOn);
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
		_AtlModule.TraceCharacterAction (mCharID, _T("SetSoundEffectsOn"), _T("%d"), bOn);
#endif
		if	(bOn)
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::SetSoundEffectsOn [%d]"), this, m_dwRef, mCharID, bOn);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetSoundEffectsOn (long *pbOn)
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
	if	(!pbOn)
	{
#ifdef _STRICT_COMPATIBILITY
		lResult = E_POINTER;
#endif
	}
	else
	{
		(*pbOn) = (IsSoundEnabled(true)!=false);
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetExtraData (BSTR *pbszExtraData)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetExtraData"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT				lResult = S_OK;
	CAgentFileName *	lFileName;

	if	(!mFile)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	if	(!pbszExtraData)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(
				(mFile->GetNames().GetSize() <= 0)
			&&	(!mFile->ReadNames ())
			)
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
		else
		if	(lFileName = mFile->FindName (mLangID))
		{
			(*pbszExtraData) = tBstrPtr (lFileName->mDesc2).Detach();
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetExtraData"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
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
	DaSvrAnimationNames *	lAnimationNames;

	if	(!punkEnum)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*punkEnum) = NULL;

		if	(!mFile)
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
		else
		if	(lAnimationNames = DaSvrAnimationNames::CreateInstance (*mFile))
		{
			lAnimationNames->AddRef ();
			(*punkEnum) = lAnimationNames->GetControllingUnknown ();
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetAnimationNames"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Activate (short sState)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::Activate [%hd]"), this, m_dwRef, mCharID, sState);
	}
#endif
#ifdef	_DEBUG_ACTIVE
	if	(LogIsActive (_DEBUG_ACTIVE))
	{
		LogMessage (_DEBUG_ACTIVE, _T("[%d] Activate [%hd]"), mCharID, sState);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(sState == ActiveType_Inactive)
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
	if	(sState == ActiveType_Active)
	{
		SetClientActive (true, false);
	}
	else
	if	(sState == ActiveType_InputActive)
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::Activate"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetActive (short *psState)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetActive"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;
	short	lState = ActiveType_Inactive;

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
				lState = ActiveType_InputActive;
			}
			else
			{
				lState = ActiveType_Active;
			}
		}
		else
		{
			lResult = S_FALSE;
		}
		if	(psState)
		{
			(*psState) = lState;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetActive"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::HasOtherClients (long *plNumOtherClients)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::HasOtherClients"), this, m_dwRef, mCharID);
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
		lClientCount = GetClientCount (mCharID);
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
		if	(plNumOtherClients)
		{
			(*plNumOtherClients) = lClientCount;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::HasOtherClients"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Show (long bFast, long *pdwReqID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::Show [%d]"), this, m_dwRef, mCharID, bFast);
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
		lReqID = Show (bFast!=0);
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("Show"), _T("%d\t%d"), bFast, lReqID);
#endif
	}
	if	(pdwReqID)
	{
		(*pdwReqID) = lReqID;
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Hide (long bFast, long *pdwReqID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::Hide [%d]"), this, m_dwRef, mCharID, bFast);
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
		lReqID = Hide (bFast!=0);
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("Hide"), _T("%d\t%d"), bFast, lReqID);
#endif
	}
	if	(pdwReqID)
	{
		(*pdwReqID) = lReqID;
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetVisible (long *pbVisible)
{
#ifdef	_DEBUG_INTERFACE_NOT
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetVisible"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!pbVisible)
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
		(*pbVisible) = IsVisible (true) ? TRUE : FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetVisible"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetVisibilityCause (long *pdwCause)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetVisibilityCause"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!pdwCause)
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
		(*pdwCause) = mNotify->_GetVisibilityCause (mCharID);
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetVisibilityCause"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Prepare (long dwType, BSTR bszName, long bQueue, long *pdwReqID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::Prepare [%d] [%ls] [%d]"), this, m_dwRef, mCharID, dwType, bszName, bQueue);
	}
#endif
	HRESULT	lResult;
	long	lReqID = 0;

	if	(!bszName)
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
		lResult = DoPrepare (dwType, CString (bszName), (bQueue != 0), lReqID);
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("Prepare"), _T("%d\t%ls\t%d\t%d"), dwType, bszName, bQueue, lReqID);
#endif
	}

	if	(pdwReqID)
	{
		(*pdwReqID) = lReqID;
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Play (BSTR bszAnimation, long *pdwReqID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::Play [%s]"), this, m_dwRef, mCharID, DebugStr(CString(bszAnimation)));
	}
#endif
	HRESULT	lResult = S_OK;
	long	lReqID = 0;

	if	(!bszAnimation)
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
		lReqID = mWnd->QueueGesture (mCharID, bszAnimation);
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("Play"), _T("%ls\t%d"), bszAnimation, lReqID);
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
	if	(pdwReqID)
	{
		(*pdwReqID) = lReqID;
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Stop (long dwReqID)
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
	if	(dwReqID <= 0)
	{
		lResult = AGENTREQERR_OBJECTINVALID;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("Stop"), _T("%d"), dwReqID);
#endif
		if	(lRequest = mWnd->FindQueuedAction (dwReqID))
		{
			if	(!mWnd->RemoveQueuedAction ((CQueuedAction *) lRequest, AGENTREQERR_INTERRUPTEDCODE, _T("Stop")))
			{
				lResult = AGENTREQERR_REMOVED;
			}
		}
		else
		if	(
				(lRequest = FindOtherRequest (dwReqID, lOtherCharacter))
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::StopAll (long lTypes)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::StopAll [%8.8X]"), this, m_dwRef, mCharID, lTypes);
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
		_AtlModule.TraceCharacterAction (mCharID, _T("StopAll"), _T("0x%8.8X"), lTypes);
#endif
		lResult = StopAll (lTypes, AGENTREQERR_INTERRUPTEDCODE);
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::MoveTo (short x, short y, long lSpeed, long *pdwReqID)
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
	if	(lSpeed > 0)
	{
		lReqID = mWnd->QueueMove (mCharID, CPoint (x, y), lSpeed);
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("MoveTo"), _T("%hd\t%hd\t%d\t%d"), x, y, lSpeed, lReqID);
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
	if	(pdwReqID)
	{
		(*pdwReqID) = lReqID;
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetMoveCause (long *pdwCause)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetMoveCause"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!pdwCause)
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
		(*pdwCause) = mNotify->_GetMoveCause (mCharID);
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetMoveCause"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GestureAt (short x, short y, long *pdwReqID)
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
	if	(pdwReqID)
	{
		(*pdwReqID) = lReqID;
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Think (BSTR bszText, long *pdwReqID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::Think [%s]"), this, m_dwRef, mCharID, DebugStr(bszText));
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
		IDaSvrBalloonPtr	lAgentBalloon (GetBalloonInterface (true));

		if	(lAgentBalloon->GetEnabled (NULL) == S_OK)
		{
			GetBalloonWnd (true);
			lReqID = mWnd->QueueThink (mCharID, CString (bszText));
			mWnd->ActivateQueue (true);
		}
		else
		{
			lResult = AGENTERR_NOBALLOON;
		}
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("Think"), _T("%s\t%d"), EncodeTraceString(bszText), lReqID);
#endif
	}
	if	(pdwReqID)
	{
		(*pdwReqID) = lReqID;
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Wait (long dwReqID, long *pdwReqID)
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
	if	(dwReqID <= 0)
	{
		lResult = AGENTREQERR_OBJECTINVALID;
	}
	else
	{
		if	(lOtherRequest = FindOtherRequest (dwReqID, lOtherCharacter))
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
				lReqID = mWnd->QueueWait (mCharID, lOtherCharacter->mCharID, dwReqID);
				mWnd->ActivateQueue (true);
			}
#ifdef	_TRACE_CHARACTER_ACTIONS
			_AtlModule.TraceCharacterAction (mCharID, _T("Wait"), _T("%d\t%d"), dwReqID, lReqID);
#endif
		}
		else
		{
#ifndef	_STRICT_COMPATIBILITY
			lResult = AGENTREQERR_OBJECTNOTFOUND;
#endif
		}
	}
	if	(pdwReqID)
	{
		(*pdwReqID) = lReqID;
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Interrupt (long dwReqID, long *pdwReqID)
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
	if	(dwReqID <= 0)
	{
		lResult = AGENTREQERR_OBJECTINVALID;
	}
	else
	{
		if	(lOtherRequest = FindOtherRequest (dwReqID, lOtherCharacter))
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
				lReqID = mWnd->QueueInterrupt (mCharID, lOtherCharacter->mCharID, dwReqID);
				mWnd->ActivateQueue (true);
			}
#ifdef	_TRACE_CHARACTER_ACTIONS
			_AtlModule.TraceCharacterAction (mCharID, _T("Interrupt"), _T("%d\t%d"), dwReqID, lReqID);
#endif
		}
		else
		{
#ifndef	_STRICT_COMPATIBILITY
			lResult = AGENTREQERR_OBJECTNOTFOUND;
#endif
		}
	}
	if	(pdwReqID)
	{
		(*pdwReqID) = lReqID;
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetAutoPopupMenu (long bAutoPopupMenu)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::SetAutoPopupMenu [%d]"), this, m_dwRef, mCharID, bAutoPopupMenu);
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
		_AtlModule.TraceCharacterAction (mCharID, _T("SetAutoPopupMenu"), _T("%d"), bAutoPopupMenu);
#endif
		if	(bAutoPopupMenu)
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetAutoPopupMenu (long *pbAutoPopupMenu)
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
	if	(!pbAutoPopupMenu)
	{
#ifdef _STRICT_COMPATIBILITY
		lResult = E_POINTER;
#endif
	}
	else
	{
		(*pbAutoPopupMenu) = (IsAutoPopupMenu()!=false);
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetHelpFileName (BSTR *pbszName)
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
	if	(pbszName)
	{
		(*pbszName) = NULL;
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetHelpFileName (BSTR bszName)
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetHelpModeOn (long bHelpModeOn)
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetHelpModeOn (long *pbHelpModeOn)
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
	if	(pbHelpModeOn)
	{
		(*pbHelpModeOn) = FALSE;
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetHelpContextID (long ulID)
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetHelpContextID (long *pulID)
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
	if	(pulID)
	{
		(*pulID) = 0;
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetTTSSpeed (long *pdwSpeed)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetTTSSpeed"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!pdwSpeed)
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
		(*pdwSpeed) = CDaAudioOutputConfig().ApplyVoiceRate (mFile->GetTts().mSpeed, 4);
	}
	else
#endif
	{
		(*pdwSpeed) = CDaAudioOutputConfig().ApplyVoiceRate (mFile->GetTts().mSpeed);
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetTTSPitch (short *pwPitch)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetTTSPitch"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!pwPitch)
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
		(*pwPitch) = mSapiVoice->GetPitch ();
	}
	else
#endif
	{
		(*pwPitch) = mFile->GetTts().mPitch;
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetTTSModeID (BSTR *pbszModeID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetTTSModeID"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!pbszModeID)
	{
		lResult = E_POINTER;
	}
	else
	if	(GetSapiVoice (true))
	{
		(*pbszModeID) = mSapiVoice->GetUniqueId().Detach();
	}
	else
	{
		(*pbszModeID) = CString().AllocSysString();
#ifndef	_STRICT_COMPATIBILITY
		lResult = AGENTWARNING_TTSENGINENOTFOUND;
#endif
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetTTSModeID"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetTTSModeID (BSTR bszModeID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::SetTTSModeID [%ls]"), this, m_dwRef, mCharID, bszModeID);
	}
#endif
	HRESULT				lResult = E_FAIL;
	CSapiVoiceCache *	lVoiceCache;

	if	(!CDaAudioOutputConfig().LoadConfig().mTtsEnabled)
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
			_AtlModule.TraceCharacterAction (mCharID, _T("SetTTSModeID"), _T("%ls"), bszModeID);
#endif
#ifdef	_STRICT_COMPATIBILITY
			try
			{
				if	(
						(mWnd->NextQueuedAction (mCharID))
					&&	(mWnd->IsSpeakQueued (mCharID) == mWnd->NextQueuedAction (mCharID))
					)
				{
					mWnd->RemoveQueuedAction (mWnd->NextQueuedAction (mCharID), AGENTREQERR_INTERRUPTEDCODE, _T("SetTTSModeID"));
				}
				if	(mSapiVoice)
				{
					mSapiVoice->Stop ();
				}
			}
			catch AnyExceptionDebug
#endif
			mSapiVoice = NULL;

			if	(GetSapiVoice (true, CString (bszModeID)))
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::SetTTSModeID"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Speak (BSTR bszText, BSTR bszUrl, long *pdwReqID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::Speak [%s] [%s]"), this, m_dwRef, mCharID, DebugStr(CString(bszText)), DebugStr(CString(bszUrl)));
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
	if	(!CDaAudioOutputConfig().LoadConfig().mTtsEnabled)
	{
		lResult = AGENTERR_SPEAKINGDISABLED;
	}
	else
#if	FALSE
	if	(
			(
				(bszUrl == NULL)
			||	(*bszUrl == NULL)
			)
		&&	(
				(bszText == NULL)
			||	(*bszText == NULL)
			)
		)
	{
		lResult = E_INVALIDARG;
	}
	else
#endif
	{
		IDaSvrBalloonPtr	lAgentBalloon (GetBalloonInterface (true));
		bool				lShowBalloon = (lAgentBalloon->GetEnabled (NULL) == S_OK);
		CString				lText (bszText);
		CString				lSoundUrl (bszUrl);
		CSapiVoice *		lVoice = NULL;
//
//	MS Agent shows the speech balloon silently when the character is listening.
//	For now, we'll just stop listening.
//
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("PreSpeak"), _T("%s\t%s"), EncodeTraceString(bszText), DebugStr(bszUrl));
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
					lResult = DoPrepare (PrepareType_Wave, lSoundUrl, false, lReqID);
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
			_AtlModule.TraceCharacterAction (mCharID, _T("Speak"), _T("%s\t%s\t%d"), EncodeTraceString(bszText), DebugStr(bszUrl), lReqID);
#endif
			mWnd->ActivateQueue (true);
		}
	}
	if	(pdwReqID)
	{
		(*pdwReqID) = lReqID;
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetSRModeID (BSTR *pbszModeID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetSRModeID"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!pbszModeID)
	{
		lResult = E_POINTER;
	}
	else
	if	(GetSapiInput (true))
	{
		(*pbszModeID) = mSapiInput->GetEngineName().Detach();
	}
	else
	{
		(*pbszModeID) = CString().AllocSysString();
		lResult = S_FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetSRModeID"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::SetSRModeID (BSTR bszModeID)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::SetSRModeID"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT				lResult = E_FAIL;
	CSapiInputCache *	lInputCache;

	if	(!CDaAudioOutputConfig().LoadConfig().mTtsEnabled)
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
			_AtlModule.TraceCharacterAction (mCharID, _T("SetSRModeID"), _T("%ls"), bszModeID);
#endif
			mSapiInput = NULL;

			if	(GetSapiInput (true, CString (bszModeID)))
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::SetSRModeID"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::Listen (long bListen)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::Listen [%d]"), this, m_dwRef, mCharID, bListen);
	}
#endif
	HRESULT	lResult;

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("Listen"), _T("%d"), bListen);
#endif
	if	(bListen)
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::Listen [%d]"), this, m_dwRef, mCharID, bListen);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetSRStatus (long *plStatus)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetSRStatus"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = S_OK;

	if	(!plStatus)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*plStatus) = GetListeningStatus ();
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetSRStatus"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_HasIcon (boolean *HasIcon)
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
			(*HasIcon) = mWnd->IsNotifyIconValid()?TRUE:FALSE;
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::get_IconVisible (boolean *IconVisible)
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
		(*IconVisible) = IsIconVisible()?TRUE:FALSE;
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

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetSpeechEngine (boolean GetDefault, IDaSvrSpeechEngine **SpeechEngine)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetSpeechEngine"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT					lResult = E_FAIL;
	DaSvrSpeechEngine *	lSpeechEngine = NULL;
	IDaSvrSpeechEnginePtr	lInterface;
	CSapiVoiceCache *		lVoiceCache;
	CSapi5Voices *			lSapi5Voices;
	CSapi5VoiceInfo *		lSapi5Voice;
#ifndef	_WIN64
	CSapi4Voices *			lSapi4Voices;
	CSapi4VoiceInfo *		lSapi4Voice;
#endif

	if	(!SpeechEngine)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SpeechEngine) = NULL;

		if	(GetDefault)
		{
			lResult = GetDefaultSpeechEngine (mFile, SpeechEngine, mClientMutexName);
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
					lSpeechEngine = DaSvrSpeechEngine::CreateInstance (lSapi5Voice, mClientMutexName);
				}
#ifndef	_WIN64
				else
				if	(
						(lSapi4Voices = lVoiceCache->GetSapi4Voices ())
					&&	(lSapi4Voice = lSapi4Voices->GetModeId (CGuidStr::Parse (lSapiVoice->GetUniqueId ())))
					)
				{
					lSpeechEngine = DaSvrSpeechEngine::CreateInstance (lSapi4Voice, mClientMutexName);
				}
#endif
			}

			if	(lSpeechEngine)
			{
				lInterface = lSpeechEngine->GetControllingUnknown();
				(*SpeechEngine) = lInterface.Detach();
				lResult = S_OK;
			}
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetSpeechEngine"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::FindSpeechEngines (long LanguageID, IDaSvrSpeechEngines **SpeechEngines)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::FindSpeechEngines"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = E_FAIL;

	if	(!SpeechEngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SpeechEngines) = NULL;

		if	(mFile)
		{
			lResult = FindSpeechEngines (mFile, (LANGID)LanguageID, SpeechGender_Neutral, SpeechEngines, mClientMutexName);
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::FindSpeechEngines"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacter::GetRecognitionEngine (boolean GetDefault, IDaSvrRecognitionEngine **RecognitionEngine)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::GetRecognitionEngine"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT						lResult = E_FAIL;
	DaSvrRecognitionEngine *	lRecognitionEngine = NULL;
	IDaSvrRecognitionEnginePtr	lInterface;
	CSapiInputCache *			lInputCache;
	CSapi5Inputs *				lSapi5Inputs;
	CSapi5InputInfo *			lSapi5Input;

	if	(!RecognitionEngine)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*RecognitionEngine) = NULL;

		if	(GetDefault)
		{
			lResult = GetDefaultRecognitionEngine (mFile, RecognitionEngine, mClientMutexName);
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
					lRecognitionEngine = DaSvrRecognitionEngine::CreateInstance (lSapi5Input, mClientMutexName);
				}
			}

			if	(lRecognitionEngine)
			{
				lInterface = lRecognitionEngine->GetControllingUnknown();
				(*RecognitionEngine) = lInterface.Detach();
				lResult = S_OK;
			}
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::GetRecognitionEngine"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacter::FindRecognitionEngines (long LanguageID, IDaSvrRecognitionEngines **RecognitionEngines)
{
#ifdef	_DEBUG_INTERFACE
	if	(LogIsActive (_DEBUG_INTERFACE))
	{
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrCharacter::FindRecognitionEngines"), this, m_dwRef, mCharID);
	}
#endif
	HRESULT	lResult = E_FAIL;

	if	(!RecognitionEngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*RecognitionEngines) = NULL;

		if	(mFile)
		{
			lResult = FindRecognitionEngines (mFile, (LANGID)LanguageID, RecognitionEngines, mClientMutexName);
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacter2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrCharacter::FindRecognitionEngines"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}
