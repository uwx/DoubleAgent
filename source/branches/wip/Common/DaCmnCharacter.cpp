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
#include <AgtErr.h>
#include "DaCmnCharacter.h"
#include "DaCmnBalloon.h"
#include "DaCmnCommands.h"
#include "DaCmnAnimationNames.h"
#include "AgentAnchor.h"
#include "AgentFiles.h"
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
#define	_DEBUG_LANGUAGE			(GetProfileDebugInt(_T("DebugLanguage"),LogVerbose,true)&0xFFFF)
#define	_DEBUG_ACTIVE			(GetProfileDebugInt(_T("DebugActive"),LogVerbose,true)&0xFFFF|LogTimeMs)
#define	_DEBUG_PREPARE			(GetProfileDebugInt(_T("DebugPrepare"),LogVerbose,true)&0xFFFF|LogTimeMs)
#define	_DEBUG_LISTEN			(GetProfileDebugInt(_T("DebugListen"),LogVerbose,true)&0xFFFF|LogTimeMs)
#define	_DEBUG_REQUESTS			(GetProfileDebugInt(_T("DebugRequests"),LogVerbose,true)&0xFFFF|LogTimeMs)
#define	_DEBUG_STYLE			(GetProfileDebugInt(_T("DebugStyle"),LogVerbose,true)&0xFFFF|LogTimeMs)
#define	_DEBUG_NOTIFY_PATH		(GetProfileDebugInt(_T("DebugNotifyPath"),LogVerbose,true)&0xFFFF)
#define	_DEBUG_DEFAULT_CHAR		(GetProfileDebugInt(_T("DebugDefaultChar"),LogVerbose,true)&0xFFFF)
#define	_LOG_FILE_LOAD			(GetProfileDebugInt(_T("LogFileLoad"),LogVerbose,true)&0xFFFF)
#endif

#ifndef	_LOG_FILE_LOAD
#define	_LOG_FILE_LOAD			LogDetails
#endif

/////////////////////////////////////////////////////////////////////////////

CDaCmnCharacter::CDaCmnCharacter ()
:	mCharID (0),
	mLangID (LANG_USER_DEFAULT),
	mFile (NULL),
	mWnd (NULL),
	mIsDefault (false),
	mSapiVoice (NULL),
	mSapiInput (NULL),
	mIdleEnabled (true),
	mSoundEnabled (true),
	mAutoPopupMenu (true)
{
}

CDaCmnCharacter::~CDaCmnCharacter()
{
	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

void CDaCmnCharacter::Initialize (long pCharID, CEventNotify * pNotify, _IListeningAnchor * pListeningAnchor)
{
	mCharID = pCharID;
	mNotify = pNotify;
	mNotify->RegisterEventReflect (this, true);
	mNotify->RegisterEventLock (this, true);
	mListeningAnchor = pListeningAnchor;

	if	(FAILED (SetLangID (GetUserDefaultUILanguage ())))
	{
		SetLangID (MAKELANGID (LANG_ENGLISH, SUBLANG_DEFAULT));
	}
#ifdef	_DEBUG_LANGUAGE
	if	(LogIsActive (_DEBUG_LANGUAGE))
	{
		LogMessage (_DEBUG_LANGUAGE, _T("[%p] [%d] CDaCmnCharacter Default LangID [%4.4hX]"), this, mCharID, mLangID);
	}
#endif
}

void CDaCmnCharacter::Terminate (bool pFinal, bool pAbandonned)
{
	if	(this)
	{
		if	(!pAbandonned)
		{
			try
			{
				if	(IsClientActive ())
				{
					SetActiveClient (false, false);
				}
			}
			catch AnyExceptionDebug
		}

		if	(!mPrepares.IsEmpty ())
		{
			try
			{
				mPrepares.RemoveAll ();
			}
			catch AnyExceptionDebug
		}

		if	(mNotify)
		{
			try
			{
				if	(
						(pFinal)
					||	(!IsInNotify ())
					)
				{
					mNotify->RegisterEventLock (this, false);
				}
				mNotify->RegisterEventReflect (this, false);
			}
			catch AnyExceptionDebug

			if	(
					(pFinal)
				&&	(mFile)
				)
			{
				try
				{
					mNotify->mAnchor->RemoveFileClient (mFile, this, false);
				}
				catch AnyExceptionDebug
				try
				{
					mNotify->mAnchor->mAnchor.RemoveFileClient (mFile, this);
				}
				catch AnyExceptionDebug
			}
		}

		Unrealize (pFinal || pAbandonned);

		ReleaseSapiVoice (pAbandonned);
		ReleaseSapiInput (pAbandonned);

		if	(
				(pFinal)
			&&	(!IsInNotify ())
			)
		{
			mFile = NULL;
		}
	}
}

void CDaCmnCharacter::Unrealize (bool pForce)
{
	if	(mWnd)
	{
		if	(GetActiveClient () == mCharID)
		{
			Hide (true, true);
		}

		if	(
				(mFile)
			&&	(mNotify)
			)
		{
			INT_PTR	lClientCount = GetClientCount (mCharID);
			
			try
			{
				mNotify->mAnchor->RemoveFileClient (mFile, mWnd, false);
				if	(mWnd->GetBalloonWnd())
				{
					mNotify->mAnchor->RemoveFileClient (mFile, mWnd->GetBalloonWnd(), false);
				}
			}
			catch AnyExceptionDebug

			if	(lClientCount <= 0)
			{
				try
				{
					mNotify->mAnchor->mAnchor.RemoveFileClient (mFile, mWnd);
					if	(mWnd->GetBalloonWnd())
					{
						mNotify->mAnchor->mAnchor.RemoveFileClient (mFile, mWnd->GetBalloonWnd());
					}
				}
				catch AnyExceptionDebug
			}
		}

		if	(
				(pForce)
			||	(!IsInNotify ())
			)
		{
			try
			{
				mWnd->Detach (mCharID, mNotify);
				if	(GetClientCount (mCharID) <= 0)
				{
					mWnd->Close ();
				}
			}
			catch AnyExceptionDebug

			mWnd = NULL;
			SafeFreeSafePtr (mWndRefHolder);
		}
	}
}

bool CDaCmnCharacter::IsValid (const CAgentFile * pFile) const
{
	if	(pFile)
	{
		return mFile == pFile;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::OpenFile (CAgentFile * pFile, bool pIsDefault)
{
	HRESULT	lResult = S_OK;

	if	(mFile)
	{
		lResult = E_UNEXPECTED;
	}
	else
	if	(mFile = pFile)
	{
		mIsDefault = pIsDefault;

		if	(!mNotify->mAnchor->AddFileClient (mFile, this))
		{
			mNotify->mAnchor->CacheFile (mFile, this);
		}
		if	(!mNotify->mAnchor->mAnchor.AddFileClient (mFile, this))
		{
			mNotify->mAnchor->mAnchor.CacheFile (mFile, this);
		}
	}
	else
	{
		lResult = E_INVALIDARG;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::Realize (CAgentCharacterWnd * pCharacterWnd, DWORD pInitialStyle)
{
	HRESULT				lResult = S_OK;
	CAgentPopupWnd *	lPopupWnd;

	if	(
			(!mFile)
		||	(
				(mWnd)
			&&	(mWnd != pCharacterWnd)
			)
		)
	{
		lResult = E_UNEXPECTED;
	}
	else
	if	(pCharacterWnd)
	{
		if	(
				(
					(pCharacterWnd->GetAgentFile())
				&&	(IsEqualGUID (pCharacterWnd->GetAgentFile()->GetGuid(), mFile->GetGuid()))
				)
			||	(
					(!pCharacterWnd->GetAgentFile())
				&&	(pCharacterWnd->Open (mFile))
				)
			)
		{
			mWnd = pCharacterWnd;
			mNotify->mAnchor->AddFileClient (mFile, pCharacterWnd);
			mNotify->mAnchor->mAnchor.AddFileClient (mFile, pCharacterWnd);

			SetStyle (~pInitialStyle, pInitialStyle);
			pCharacterWnd->EnableIdle (mIdleEnabled);
			pCharacterWnd->EnableSound (mSoundEnabled);

			if	(lPopupWnd = GetPopupWnd ())
			{
				mWndRefHolder = lPopupWnd->GetControllingUnknown();
			}
			if	(GetActiveClient () <= 0)
			{
				SetActiveClient (true, false);
			}
			else
			{
				pCharacterWnd->Attach (mCharID, mNotify, &mIconData, false);
			}
		}
		else
		{
			lResult = E_FAIL;
		}
	}
	else
	{
		lResult = E_INVALIDARG;
	}
	return lResult;
}

HRESULT CDaCmnCharacter::RealizePopup (CWindow * pParentWnd, DWORD pInitialStyle, DWORD pExStyle)
{
	HRESULT								lResult = S_OK;
	CAgentPopupWnd *					lPopupWnd;
	CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
	INT_PTR								lClientNdx;

	if	(
			(!mFile)
		||	(mWnd)
		)
	{
		lResult = E_UNEXPECTED;
	}
	else
	if	(mNotify->mAnchor->mAnchor.GetFileClients (mFile, lFileClients))
	{
		for	(lClientNdx = 0; lClientNdx < (INT_PTR)lFileClients.GetCount(); lClientNdx++)
		{
			if	(
					(lPopupWnd = dynamic_cast <CAgentPopupWnd *> (lFileClients [lClientNdx]))
				&&	(lPopupWnd->IsWindow ())
				)
			{
				break;
			}
		}
	}

	if	(
			(SUCCEEDED (lResult))
		&&	(lPopupWnd)
		)
	{
		lPopupWnd->ModifyStyleEx (WS_EX_TOPMOST, pExStyle|WS_EX_TOOLWINDOW);
		lResult = Realize (lPopupWnd, pInitialStyle);
	}
	else
	if	(
			(SUCCEEDED (lResult))
		&&	(!lPopupWnd)
		)
	{
		if	(mWnd = lPopupWnd = CAgentPopupWnd::CreateInstance())
		{
			SetStyle (~pInitialStyle, pInitialStyle);

			if	(lPopupWnd->Create (pParentWnd, NULL, pExStyle|WS_EX_TOOLWINDOW))
			{
				lResult = Realize (lPopupWnd, pInitialStyle);
			}
			else
			{
				lResult = E_FAIL;
			}
			if	(FAILED (lResult))
			{
				mWnd = NULL;
				delete lPopupWnd;
			}
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
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

BSTR CDaCmnCharacter::GetName () const
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

HRESULT CDaCmnCharacter::GetLoadPath (VARIANT pProvider, CAtlString & pFilePath, bool * pIsDefault)
{
	HRESULT	lResult = S_OK;

	pFilePath.Empty ();
	if	(pIsDefault)
	{
		*pIsDefault = false;
	}

	if	(!IsEmptyParm (&pProvider))
	{
		try
		{
			pFilePath = (BSTR)(_bstr_t)_variant_t(pProvider);
	}
		catch AnyExceptionSilent
	}

	pFilePath.TrimLeft ();
	pFilePath.TrimRight ();
	PathUnquoteSpaces (pFilePath.GetBuffer (pFilePath.GetLength ()));
	pFilePath.ReleaseBuffer ();
	pFilePath.TrimLeft ();
	pFilePath.TrimRight ();

	if	(pFilePath.IsEmpty ())
	{
		pFilePath = CAgentFiles::GetDefCharPath ();
		if	(pIsDefault)
		{
			*pIsDefault = true;
		}
#ifdef	_LOG_FILE_LOAD
		if	(LogIsActive (_LOG_FILE_LOAD))
		{
			LogMessage (_LOG_FILE_LOAD, _T("Load default Path [%s]"), pFilePath);
		}
#endif
	}
	else
	if	(CAgentFile::IsRelativeFilePath (pFilePath))
	{
		UINT		lPathNum;
		CAtlString	lFilePath;

		lFilePath = CAgentFile::ParseFilePath (pFilePath);

		if	(!lFilePath.IsEmpty ())
		{
			if	(PathFileExists (lFilePath))
			{
#ifdef	_LOG_FILE_LOAD
				if	(LogIsActive (_LOG_FILE_LOAD))
				{
					LogMessage (_LOG_FILE_LOAD, _T("File [%s] try Path [%s]"), pFilePath, lFilePath);
				}
#endif
				pFilePath = lFilePath;
			}
			else
			{
				for	(lPathNum = 0; true; lPathNum++)
				{
					lFilePath = CAgentFiles::GetSystemCharsPath (lPathNum);
					if	(lFilePath.IsEmpty ())
					{
						break;
					}
					PathAppend (lFilePath.GetBuffer (MAX_PATH), pFilePath);
					lFilePath.ReleaseBuffer ();
#ifdef	_LOG_FILE_LOAD
					if	(LogIsActive (_LOG_FILE_LOAD))
					{
						LogMessage (_LOG_FILE_LOAD, _T("File [%s] try Path [%s]"), pFilePath, lFilePath);
					}
#endif
					if	(PathFileExists (lFilePath))
					{
						break;
					}
				}
				if	(!lFilePath.IsEmpty ())
				{
					pFilePath = lFilePath;
				}
			}
		}
	}

	if	(pFilePath.IsEmpty ())
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		pFilePath = CAgentFile::ParseFilePath (pFilePath);

		if	(!CAgentFile::IsProperFilePath (pFilePath))
		{
			lResult = AGENTPROVERROR_PROTOCOL;
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::GetAgentFile (VARIANT pProvider, tPtr <CAgentFile> & pAgentFile)
{
	HRESULT		lResult;
	CAtlString	lFilePath;

	if	(SUCCEEDED (lResult = GetLoadPath (pProvider, lFilePath)))
	{
		lResult =  GetAgentFile (lFilePath, pAgentFile);
	}
	return lResult;
}

HRESULT CDaCmnCharacter::GetAgentFile (LPCTSTR pFilePath, tPtr <CAgentFile> & pAgentFile)
{
	HRESULT		lResult;
	CAtlString	lFilePath (pFilePath);

	if	(lFilePath.IsEmpty ())
	{
		lResult = E_INVALIDARG;
	}
	else
	if	(!CAgentFile::IsProperFilePath (lFilePath))
	{
		lResult = AGENTPROVERROR_PROTOCOL;
	}
	else
	if	(PathIsURL (lFilePath))
	{
		lResult = E_NOTIMPL;
	}
	else
	if	(pAgentFile = CAgentFile::CreateInstance())
	{
		lResult = pAgentFile->Open (lFilePath);
	}
	else
	{
		lResult = E_OUTOFMEMORY;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CDaCmnCharacter::IsVisible (bool pOrIsShowing) const
{
	bool					lRet = false;
	CAgentCharacterWnd *	lCharacterWnd;

	if	(
			(lCharacterWnd = GetCharacterWnd ())
		&&	(
				(lCharacterWnd->IsCharShown ())
			||	(
					(pOrIsShowing)
				&&	(lCharacterWnd->IsShowingQueued ())
				)
			)
		)
	{
		lRet = true;
	}
	return lRet;
}

bool CDaCmnCharacter::IsShowing () const
{
	bool					lRet = false;
	CAgentCharacterWnd *	lCharacterWnd;

	if	(
			(lCharacterWnd = GetCharacterWnd ())
		&&	(
				(lCharacterWnd->IsShowingQueued ())
			||	(lCharacterWnd->IsShowingState (_T("SHOWING")))
			)
		)
	{
		lRet = true;
	}
	return lRet;
}

bool CDaCmnCharacter::IsHiding () const
{
	bool					lRet = false;
	CAgentCharacterWnd *	lCharacterWnd;

	if	(
			(lCharacterWnd = GetCharacterWnd ())
		&&	(
				(lCharacterWnd->IsHidingQueued ())
			||	(lCharacterWnd->IsShowingState (_T("HIDING")))
			)
		)
	{
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

long CDaCmnCharacter::Show (bool pFast, bool pImmediate)
{
	long					lReqID = 0;
	CAgentCharacterWnd *	lCharacterWnd;
	CAgentPopupWnd *		lPopupWnd;

	if	(pImmediate)
	{
		if	(
				(lPopupWnd = GetPopupWnd ())
			&&	(lPopupWnd->GetCharID() == mCharID)
			)
		{
			lPopupWnd->ShowPopup (mCharID, VisibilityCause_ProgramShowed);
		}
	}
	else
	if	(lCharacterWnd = GetCharacterWnd ())
	{
		lReqID = lCharacterWnd->QueueShow (mCharID, pFast);
		lCharacterWnd->ActivateQueue (true);
	}
	return lReqID;
}

long CDaCmnCharacter::Hide (bool pFast, bool pImmediate)
{
	long					lReqID = 0;
	CAgentCharacterWnd *	lCharacterWnd;
	CAgentPopupWnd *		lPopupWnd;
	CAgentBalloonWnd *		lBalloonWnd;

	StopListening (false, ListenComplete_CharacterClientDeactivated);

	if	(pImmediate)
	{
		if	(lCharacterWnd = GetCharacterWnd ())
		{
			lCharacterWnd->ClearQueuedActions (-1, AGENTREQERR_INTERRUPTEDCODE, _T("Hide"));
			lCharacterWnd->StopIdle (_T("Hide"));
			lCharacterWnd->Stop ();
			if	(lBalloonWnd = lCharacterWnd->GetBalloonWnd())
			{
				lBalloonWnd->HideBalloon (true);
			}
		}
		if	(
				(lPopupWnd = GetPopupWnd ())
			&&	(lPopupWnd->GetCharID() == mCharID)
			)
		{
			lPopupWnd->HidePopup (mCharID, VisibilityCause_ProgramHid);
		}
	}
	else
	if	(lCharacterWnd = GetCharacterWnd ())
	{
		lReqID = lCharacterWnd->QueueHide (mCharID, pFast);
		lCharacterWnd->ActivateQueue (true);
	}
	return lReqID;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CDaCmnCharacter::IsInputActive () const
{
	bool					lRet = false;
	CAgentCharacterWnd *	lCharacterWnd;

	if	(
			(lCharacterWnd = GetCharacterWnd ())
		&&	(lCharacterWnd->GetStyle () & WS_VISIBLE)
		&&	(lCharacterWnd->GetCharID () == GetCharID())
		&&	(lCharacterWnd->GetLastActive () == lCharacterWnd->m_hWnd)
		)
	{
		lRet = true;
	}
	return lRet;
}

bool CDaCmnCharacter::IsClientActive () const
{
	bool					lRet = false;
	CAgentCharacterWnd *	lCharacterWnd;

	if	(
			(lCharacterWnd = GetCharacterWnd ())
		&&	(lCharacterWnd->GetCharID() == GetCharID())
		)
	{
		lRet = true;
	}
	return lRet;
}

long CDaCmnCharacter::GetActiveClient () const
{
	long					lRet = 0;
	CAgentCharacterWnd *	lCharacterWnd;

	if	(lCharacterWnd = GetCharacterWnd ())
	{
		lRet = lCharacterWnd->GetCharID ();
	}
	return lRet;
}

short CDaCmnCharacter::GetActiveState () const
{
	short	lActiveState = ActiveState_Inactive;

	if	(IsClientActive ())
	{
		CAgentCharacterWnd *	lCharacterWnd;
	
		if	(
				(lCharacterWnd = GetCharacterWnd ())
			&&	(lCharacterWnd->m_hWnd == CAgentCharacterWnd::GetLastActive())
			)
		{
			lActiveState = ActiveState_InputActive;
		}
		else
		{
			lActiveState = ActiveState_Active;
		}
	}
	return lActiveState;
}

/////////////////////////////////////////////////////////////////////////////

bool CDaCmnCharacter::SetActiveClient (bool pActive, bool pInputActive)
{
	bool					lRet = false;
	long					lPrevCharId = 0;
	CAgentCharacterWnd *	lCharacterWnd;
	CDaCmnCharacter *		lNextCharacter = NULL;
	CDaCmnBalloon *			lBalloon = NULL;

#ifdef	_DEBUG_ACTIVE
	if	(LogIsActive (_DEBUG_ACTIVE))
	{
		LogMessage (_DEBUG_ACTIVE, _T("[%d] SetActiveClient [%u] InputActive [%u] - IsVisible [%u] IsClientActive [%u] IsInputActive [%u] IsListening [%u] - ClientActive [%d] InputActive [%d] Listen [%d]"), mCharID, pActive, pInputActive, IsVisible(), IsClientActive(), IsInputActive(), IsListening(), GetActiveClient(), mNotify->mAnchor->mAnchor.GetActiveCharacter(), mNotify->mAnchor->mAnchor.GetListenCharacter());
	}
#endif
	if	(lCharacterWnd = GetCharacterWnd ())
	{
		lPrevCharId = lCharacterWnd->GetCharID ();
	}

	if	(
			(pActive)
		&&	(lPrevCharId != mCharID)
		)
	{
		if	(
				(lCharacterWnd = GetCharacterWnd ())
			&&	(lCharacterWnd->Attach (mCharID, mNotify, &mIconData, true))
			)
		{
			if	(lBalloon = GetBalloon (true))
			{
				lBalloon->SetLangID (mLangID);
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
				&&	(mNotify->mAnchor->mAnchor.GetFileClients (mFile, lFileClients))
				)
			{
				for	(lClientNdx = 0; lClientNdx < (INT_PTR)lFileClients.GetCount(); lClientNdx++)
				{
					if	(
							(lNextCharacter = dynamic_cast <CDaCmnCharacter *> (lFileClients [lClientNdx]))
						&&	(lNextCharacter->IsValid (mFile))
						&&	(lNextCharacter->GetCharID() != mCharID)
						)
					{
						break;
					}
					lNextCharacter = NULL;
				}
			}

			if	(
					(lCharacterWnd = GetCharacterWnd ())
				&&	(lNextCharacter)
				&&	(lNextCharacter->GetCharacterWnd () == lCharacterWnd)
				&&	(lCharacterWnd->Attach (lNextCharacter->mCharID, lNextCharacter->mNotify, &lNextCharacter->mIconData, true))
				)
			{
				if	(lBalloon = lNextCharacter->GetBalloon (true))
				{
					lBalloon->SetLangID (lNextCharacter->mLangID);
				}
				lRet = true;
			}
		}
		catch AnyExceptionDebug
	}

	if	(
			(pActive)
		&&	(pInputActive)
		&&	(lCharacterWnd = GetCharacterWnd ())
		&&	(lCharacterWnd->IsCharShown ())
		)
	{
		lCharacterWnd->SetLastActive ();
	}
#ifdef	_DEBUG_ACTIVE
	if	(LogIsActive (_DEBUG_ACTIVE))
	{
		LogMessage (_DEBUG_ACTIVE, _T("[%d] %u  ClientActive [%u] InputActive [%u] - IsVisible [%u] IsClientActive [%u] IsInputActive [%u] IsListening [%u] - ClientActive [%d] InputActive [%d] Listen [%d]"), mCharID, lRet, pActive, pInputActive, IsVisible(), IsClientActive(), IsInputActive(), IsListening(), mNotify->GetActiveClient(mCharID), mNotify->mAnchor->mAnchor.GetActiveCharacter(), mNotify->mAnchor->mAnchor.GetListenCharacter());
	}
#endif
	return lRet;
}

HRESULT CDaCmnCharacter::SetActiveState (short pActiveState)
{
	HRESULT					lResult = S_OK;
	CAgentCharacterWnd *	lCharacterWnd;

	if	(pActiveState == ActiveState_Inactive)
	{
		if	(!SetActiveClient (false, false))
		{
			if	(GetCharacterWnd ())
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
	if	(pActiveState == ActiveState_Active)
	{
		if	(lCharacterWnd = GetCharacterWnd ())
		{
			SetActiveClient (true, false);
		}
		else
		{
			lResult = AGENTERR_CLIENTINVALID;
		}
	}
	else
	if	(pActiveState == ActiveState_InputActive)
	{
		if	(lCharacterWnd = GetCharacterWnd ())
		{
			if	(lCharacterWnd->IsCharShown ())
			{
				SetActiveClient (true, true);
			}
			else
			{
				lResult = S_FALSE;
			}
		}
		else
		{
			lResult = AGENTERR_CLIENTINVALID;
		}
	}
	else
	{
		lResult = E_INVALIDARG;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

INT_PTR CDaCmnCharacter::GetClientCount (int pSkipCharID) const
{
	INT_PTR	lRet = -1;

	try
	{
		CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
		INT_PTR								lClientNdx;
		CDaCmnCharacter *					lCharacter;

		if	(
				(mFile)
			&&	(mNotify)
			&&	(mNotify->mAnchor->mAnchor.GetFileClients (mFile, lFileClients))
			)
		{
			lRet = 0;

			for	(lClientNdx = 0; lClientNdx < (INT_PTR)lFileClients.GetCount(); lClientNdx++)
			{
				if	(
						(lCharacter = dynamic_cast <CDaCmnCharacter *> (lFileClients [lClientNdx]))
					&&	(lCharacter->IsValid (mFile))
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

DWORD CDaCmnCharacter::GetStyle () const
{
	DWORD					lStyle = 0;
	CAgentCharacterWnd *	lCharacterWnd;

	if	(IsIdleEnabled ())
	{
		lStyle |= CharacterStyle_IdleEnabled;
	}
	if	(IsSoundEnabled ())
	{
		lStyle |= CharacterStyle_SoundEffects;
	}
	if	(IsAutoPopupMenu ())
	{
		lStyle |= CharacterStyle_AutoPopupMenu;
	}
	if	(IsIconShown ())
	{
		lStyle |= CharacterStyle_IconShown;
	}
	if	(lCharacterWnd = GetCharacterWnd (false))
	{
		if	((lCharacterWnd->GetAlphaSmoothing() & (RenderSmoothEdges|RenderSmoothAll)) == RenderSmoothAll)
		{
			lStyle |= CharacterStyle_Smoothed;
		}
		else
		if	((lCharacterWnd->GetAlphaSmoothing() & (RenderSmoothEdges|RenderSmoothAll)) == RenderSmoothEdges)
		{
			lStyle |= CharacterStyle_SmoothEdges;
		}
	}
	return lStyle;
}

HRESULT CDaCmnCharacter::SetStyle (DWORD pRemoveStyle, DWORD pAddStyle)
{
	HRESULT					lResult = S_FALSE;
	CAgentCharacterWnd *	lCharacterWnd;
	CAgentPopupWnd *		lPopupWnd;

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
				(lCharacterWnd = GetCharacterWnd ())
			&&	(lCharacterWnd->GetCharID() == mCharID)
			)
		{
			lCharacterWnd->EnableIdle (mIdleEnabled);
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
				(lCharacterWnd = GetCharacterWnd ())
			&&	(lCharacterWnd->GetCharID() == mCharID)
			)
		{
			lCharacterWnd->EnableSound (mSoundEnabled);
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
				(lPopupWnd = GetPopupWnd ())
			&&	(lPopupWnd->GetCharID() == GetCharID())
			)
		{
			lPopupWnd->UpdateNotifyIcon (&mIconData);
			lResult = S_OK;
		}
	}

	if	(
			(lCharacterWnd = GetCharacterWnd (false))
		&&	(!lCharacterWnd->m_hWnd)
		)
	{
		if	((pAddStyle & CharacterStyle_Smoothed) == CharacterStyle_Smoothed)
		{
			lCharacterWnd->SetAlphaSmoothing (RenderSmoothAll);
		}
		else
		if	((pAddStyle & CharacterStyle_SmoothEdges) == CharacterStyle_SmoothEdges)
		{
			lCharacterWnd->SetAlphaSmoothing (RenderSmoothEdges);
		}
		else
		{
			lCharacterWnd->SetAlphaSmoothing (0);
		}
	}
#ifdef	_DEBUG_STYLE
	if	(LogIsActive (_DEBUG_STYLE))
	{
		long lStyle;
		get_Style (&lStyle);
		LogMessage (_DEBUG_STYLE, _T("[%p] [%d] CDaCmnCharacter Style [%8.8X]"), this, mCharID, lStyle);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::SetLangID (LANGID pLangID)
{
	HRESULT					lResult = S_OK;
	LANGID					lLangID;
	CAgentCharacterWnd *	lCharacterWnd;

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
			LogMessage (_DEBUG_LANGUAGE, _T("[%p] [%d] CDaCmnCharacter LangID [%4.4hX] not supported"), this, mCharID, pLangID);
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
			LogMessage (_DEBUG_LANGUAGE, _T("[%p] [%d] CDaCmnCharacter LangID from [%4.4hX] to [%4.4hX]"), this, mCharID, mLangID, lLangID);
		}
#endif
		if	(
				(lCharacterWnd = GetCharacterWnd (false))
			&&	(lCharacterWnd->GetCharID() == mCharID)
			)
		{
			lCharacterWnd->RemoveQueuedActions (QueueActionSpeak, mCharID, AGENTREQERR_INTERRUPTEDCODE, _T("SetLangID"));
			lCharacterWnd->RemoveQueuedActions (QueueActionThink, mCharID, AGENTREQERR_INTERRUPTEDCODE, _T("SetLangID"));
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
			LogMessage (_DEBUG_LANGUAGE, _T("[%p] [%d] CDaCmnCharacter LangID [%4.4hX] not installed"), this, mCharID, pLangID);
		}
#endif
		lResult = AGENTERR_LANGUAGENOTFOUND;
	}
	return lResult;
}

void CDaCmnCharacter::PropagateLangID ()
{
	if	(mFile)
	{
		try
		{
			CDaCmnBalloon *			lBalloon = NULL;
			CDaCmnCommands *		lCommands = NULL;
			CVoiceCommandsWnd *		lVoiceCommandsWnd;
			CAgentListeningWnd *	lListeningWnd;
			CAgentPopupWnd *		lPopupWnd;

			if	(lCommands = GetCommands (false))
			{
				lCommands->SetLangID (mLangID);
			}

			if	(
					(mListeningAnchor)
				&&	(lVoiceCommandsWnd = mListeningAnchor->GetVoiceCommandsWnd (false))
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
					(lPopupWnd = GetPopupWnd (false))
				&&	(lPopupWnd->GetCharID() == mCharID)
				)
			{
				lPopupWnd->SetNotifyIconTip (&mIconData, mFile, mLangID);
			}

			if	(lBalloon = GetBalloon (false))
			{
				lBalloon->SetLangID (mLangID);
			}
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CDaCmnCharacter::IsIdleEnabled () const
{
	CAgentCharacterWnd *	lCharacterWnd;

	if	(lCharacterWnd = GetCharacterWnd ())
	{
		return lCharacterWnd->IsIdleEnabled ();
	}
	return mIdleEnabled;
}

bool CDaCmnCharacter::IsSoundEnabled (bool pIgnoreGlobalConfig) const
{
	CAgentCharacterWnd *	lCharacterWnd;

	if	(lCharacterWnd = GetCharacterWnd ())
	{
		return lCharacterWnd->IsSoundEnabled (pIgnoreGlobalConfig);
	}
	return	(
				(mSoundEnabled)
			&&	(
					(pIgnoreGlobalConfig)
				||	(CDaSettingsConfig().LoadConfig().mEffectsEnabled)
				)
			);
}

bool CDaCmnCharacter::IsAutoPopupMenu () const
{
	return mAutoPopupMenu;
}

/////////////////////////////////////////////////////////////////////////////

bool CDaCmnCharacter::IsIconShown () const
{
	return mIconData.mShowIcon;
}

bool CDaCmnCharacter::IsIconVisible () const
{
	CAgentPopupWnd *	lPopupWnd;

	if	(
			(lPopupWnd = GetPopupWnd ())
		&&	(lPopupWnd->IsIconVisible ())
		)
	{
		return true;
	}
	return false;
}

bool CDaCmnCharacter::ShowIcon (bool pShow)
{
	bool				lRet = false;
	CAgentPopupWnd *	lPopupWnd;

	if	(mIconData.mShowIcon != pShow)
	{
		mIconData.mShowIcon = pShow;
		lRet = true;
	}
	if	(
			(lPopupWnd = GetPopupWnd ())
		&&	(lPopupWnd->GetCharID() == GetCharID())
		)
	{
		lPopupWnd->UpdateNotifyIcon (&mIconData);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CDaCmnCharacter::IsSpeaking () const
{
	bool			lRet = false;
	CSapiVoice *	lVoice;

//
//	Note that if the voice is shared by multiple characters, we don't check
//	which character is speaking.  That's ok because this method is used
//	to check if ANY character is speaking, so it doesn't matter.
//
	if	(
			(lVoice = const_cast <CDaCmnCharacter *> (this)->GetSapiVoice (false))
		&&	(lVoice->SafeIsSpeaking ())
		)
	{
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

CSapiVoice * CDaCmnCharacter::GetSapiVoice (bool pCreateObject, LPCTSTR pVoiceName)
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
			CAtlString			lVoiceName (pVoiceName);

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

void CDaCmnCharacter::ReleaseSapiVoice (bool pAbandonned)
{
	if	(mSapiVoice)
	{
		try
		{
			CSapiVoiceCache *	lVoiceCache;

			mSapiVoice->Stop ();

			if	(lVoiceCache = CSapiVoiceCache::GetStaticInstance ())
			{
				lVoiceCache->RemoveVoiceClient (mSapiVoice, this, !pAbandonned);
			}
		}
		catch AnyExceptionDebug
	}
	mSapiVoice = NULL;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CDaCmnCharacter::IsListening () const
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

bool CDaCmnCharacter::IsHearing () const
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

HRESULT CDaCmnCharacter::StartListening (bool pManual)
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
			LogMessage (_DEBUG_LISTEN, _T("[%d] StartListening Manual [%u] Listening [%u] Active [%u] Enabled [%u]"), mCharID, pManual, IsListening(), (mNotify->mAnchor->mAnchor.GetActiveCharacter()==GetCharID()), CDaSettingsConfig().LoadConfig().mSrEnabled);
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
		if	(!GetCharacterWnd ())
		{
			lResult = AGENTERR_CLIENTINVALID;
		}
		else
		if	(
				(pManual)
			?	(mNotify->mAnchor->mAnchor.GetActiveCharacter() != GetCharID())
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

HRESULT CDaCmnCharacter::StopListening (bool pManual, long pCause)
{
	HRESULT	lResult = S_FALSE;

	try
	{
		if	(mListeningState)
		{
#ifdef	_DEBUG_LISTEN
			if	(LogIsActive (_DEBUG_LISTEN))
			{
				LogMessage (_DEBUG_LISTEN, _T("[%d] StopListening Manual [%u] Listening [%u] Active [%u]"), mCharID, pManual, IsListening(), (mNotify->mAnchor->mAnchor.GetActiveCharacter()==GetCharID()));
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

void CDaCmnCharacter::TransferListeningState (CDaCmnCharacter * pOtherCharacter)
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

bool CDaCmnCharacter::ShowListeningState (bool pShow)
{
	bool					lRet = false;
	CAgentCharacterWnd *	lCharacterWnd;

#ifdef	_DEBUG_LISTEN
	if	(LogIsActive (_DEBUG_LISTEN))
	{
		LogMessage (_DEBUG_LISTEN, _T("[%d] ShowListeningState [%u] IsVisible [%u] IsShowing [%u] IsHiding {%u]"), mCharID, pShow, IsVisible(false), IsShowing(), IsHiding());
	}
#endif
	if	(lCharacterWnd = GetCharacterWnd ())
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
				if	(lCharacterWnd->QueueState (0, _T("LISTENING")))
				{
					lCharacterWnd->ActivateQueue (true);
					lRet = true;
				}
			}
		}
		else
		{
			if	(lCharacterWnd->RemoveQueuedState (0, _T("LISTENING")))
			{
				lRet = true;
			}
			if	(lCharacterWnd->IsShowingState (_T("LISTENING")))
			{
				lCharacterWnd->ShowGesture (NULL);
				lRet = true;
			}
			lCharacterWnd->ActivateQueue (true);
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

bool CDaCmnCharacter::ShowHearingState (bool pShow)
{
	bool					lRet = false;
	CAgentCharacterWnd *	lCharacterWnd;

#ifdef	_DEBUG_LISTEN
	if	(LogIsActive (_DEBUG_LISTEN))
	{
		LogMessage (_DEBUG_LISTEN, _T("[%d] ShowHearingState [%u] IsVisible [%u] IsShowing [%u] IsHiding {%u]"), mCharID, pShow, IsVisible(false), IsShowing(), IsHiding());
	}
#endif
	if	(lCharacterWnd = GetCharacterWnd ())
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

				if	(lCharacterWnd->QueueState (0, _T("HEARING")))
				{
					lCharacterWnd->ActivateQueue (true);
					lRet = true;
				}
			}
		}
		else
		{
			if	(lCharacterWnd->RemoveQueuedState (0, _T("HEARING")))
			{
				lRet = true;
			}
			if	(lCharacterWnd->IsShowingState (_T("HEARING")))
			{
				lCharacterWnd->ShowGesture (NULL);
				lRet = true;
			}
			lCharacterWnd->ActivateQueue (true);
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

CSapi5Input * CDaCmnCharacter::GetSapiInput (bool pCreateObject, LPCTSTR pEngineName)
{
	if	(
			(!mSapiInput)
		&&	(pCreateObject)
		)
	{
		try
		{
			CSapiInputCache *	lInputCache;
			CAtlString			lEngineName (pEngineName);

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

void CDaCmnCharacter::ReleaseSapiInput (bool pAbandonned)
{
	SafeFreeSafePtr (mListeningState);

	if	(mSapiInput)
	{
		try
		{
			CSapiInputCache *	lInputCache;

			if	(lInputCache = CSapiInputCache::GetStaticInstance ())
			{
				lInputCache->RemoveInputClient (mSapiInput, this, !pAbandonned);
			}
		}
		catch AnyExceptionDebug
	}
	mSapiInput = NULL;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::StopAll (long pStopTypes, HRESULT pReqStatus)
{
	HRESULT					lResult = S_FALSE;
	CAgentCharacterWnd *	lCharacterWnd;
	bool					lExcludeActive = false;

	if	(lCharacterWnd = GetCharacterWnd ())
	{
		if	(pStopTypes & StopAll_Play)
		{
			if	(lCharacterWnd->ClearQueuedStates (mCharID, pReqStatus, _T("StopAll"), lExcludeActive, _T("SHOWING"), _T("HIDING"), NULL))
			{
				lResult = S_OK;
			}
			if	(lCharacterWnd->ClearQueuedGestures (mCharID, pReqStatus, _T("StopAll"), lExcludeActive, _T("SHOWING"), _T("HIDING"), NULL))
			{
				lResult = S_OK;
			}
		}

		if	(pStopTypes & StopAll_Speak)
		{
			if	(lCharacterWnd->RemoveQueuedActions (QueueActionSpeak, mCharID, pReqStatus, _T("StopAll"), lExcludeActive))
			{
				lResult = S_OK;
			}
			if	(lCharacterWnd->RemoveQueuedActions (QueueActionThink, mCharID, pReqStatus, _T("StopAll"), lExcludeActive))
			{
				lResult = S_OK;
			}
//
//	The balloon may still be visible even if there are no more Speak or Think actions queued.  Either the balloon
//	is not AUTOHIDE, or it's AUTOHIDE timer is set and it will disappear soon.  We could hide it immediately if
//	we wanted, but for now we won't.
//
//			CAgentBalloonWnd *	lBalloonWnd;
//			if	(
//					(lBalloonWnd = GetBalloonWnd (false))
//				&&	(lBalloonWnd->IsAutoHide ())
//				)
//			{
//				lBalloonWnd->HideBalloon (true);
//			}
		}

		if	(pStopTypes & StopAll_Move)
		{
			if	(lCharacterWnd->RemoveQueuedActions (QueueActionMove, mCharID, pReqStatus, _T("StopAll"), lExcludeActive))
			{
				lResult = S_OK;
			}
		}

		if	(pStopTypes & StopAll_Visibility)
		{
			if	(lCharacterWnd->RemoveQueuedActions (QueueActionShow, mCharID, pReqStatus, _T("StopAll"), lExcludeActive))
			{
				lResult = S_OK;
			}
			if	(lCharacterWnd->RemoveQueuedActions (QueueActionHide, mCharID, pReqStatus, _T("StopAll"), lExcludeActive))
			{
				lResult = S_OK;
			}
		}

		if	(pStopTypes & StopAll_QueuedPrepare)
		{
			if	(lCharacterWnd->RemoveQueuedActions (QueueActionPrepare, mCharID, pReqStatus, _T("StopAll"), lExcludeActive))
			{
				lResult = S_OK;
			}
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

HRESULT CDaCmnCharacter::DoPrepare (long pType, LPCTSTR pName, bool pQueue, long & pReqID)
{
	HRESULT					lResult = E_FAIL;
	CAgentCharacterWnd *	lCharacterWnd;
	tPtr <CQueuedPrepare>	lPrepare;

#ifdef	_DEBUG_PREPARE
	if	(LogIsActive (_DEBUG_PREPARE))
	{
		LogMessage (_DEBUG_PREPARE, _T("[%p] [%d] CDaCmnCharacter Prepare [%d %u] [%s]"), this, mCharID, pType, pQueue, pName);
	}
#endif

	if	(
			(lCharacterWnd = GetCharacterWnd ())
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
						lCharacterWnd->PutQueuedAction (lPrepare.Detach ());
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
							LogMessage (_DEBUG_PREPARE, _T("[%p] [%d] CDaCmnCharacter Preparing [%p] [%d] [%s]"), this, mCharID, lPrepare.Ptr(), lPrepare->mReqID, (BSTR)lPrepare->GetAnimationNames());
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

bool CDaCmnCharacter::_OnDownloadComplete (CFileDownload * pDownload)
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
				LogMessage (_DEBUG_PREPARE, _T("[%p] [%d] CDaCmnCharacter _OnDownloadComplete [%ls]"), this, mCharID, (BSTR)pDownload->GetURL());
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
						LogComErrAnon (MinLogLevel(_DEBUG_PREPARE,LogAlways), lResult, _T("[%p] [%d] CDaCmnCharacter PutAnimation [%ls] [%ls]"), this, mCharID, (BSTR)pDownload->GetURL(), (BSTR)pDownload->GetCacheName());
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
						LogMessage (_DEBUG_PREPARE, _T("[%p] [%d] CDaCmnCharacter Prepare Complete [%p] [%d] [%ls]"), this, mCharID, lPrepare, lPrepare->mReqID, (BSTR)lPrepare->GetAnimationNames());
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

CFileDownload * CDaCmnCharacter::_FindSoundDownload (LPCTSTR pSoundUrl)
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
				&&	(CAtlString (lDownload->GetURL()).CompareNoCase (pSoundUrl) == 0)
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

void CDaCmnCharacter::_OnCharacterNameChanged (long pCharID)
{
	CAgentPopupWnd *	lPopupWnd;
#ifdef	_DEBUG_NOTIFY_PATH
	LogMessage (_DEBUG_NOTIFY_PATH, _T("CDaCmnCharacter::_OnCharacterNameChanged [%d]"), pCharID);
#endif
	if	(
			(lPopupWnd = GetPopupWnd ())
		&&	(lPopupWnd->GetCharID() == GetCharID())
		)
	{
		lPopupWnd->SetNotifyIconTip (&mIconData, mFile, mLangID);
	}
}

void CDaCmnCharacter::_OnCharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID)
{
	CAgentCharacterWnd *	lCharacterWnd;
	CVoiceCommandsWnd *		lVoiceCommandsWnd;

#ifdef	_DEBUG_NOTIFY_PATH
	LogMessage (_DEBUG_NOTIFY_PATH, _T("CDaCmnCharacter::_OnCharacterActivated [%d] {%d] [%d] [%d]"), pActiveCharID, pInputActiveCharID, pInactiveCharID, pInputInactiveCharID);
#endif
#ifdef	_DEBUG_ACTIVE
	if	(LogIsActive (_DEBUG_ACTIVE))
	{
		LogMessage (_DEBUG_ACTIVE, _T("[%d] OnCharacterActive [%d] InputActive [%d] Inactive [%d] InputInactive [%d] - IsVisible [%u] IsClientActive [%u] IsInputActive [%u] IsListening [%u] - ActiveClient [%d] InputActive [%d] Listen [%d]"), mCharID, pActiveCharID, pInputActiveCharID, pInactiveCharID, pInputInactiveCharID, IsVisible(), IsClientActive(), IsInputActive(), IsListening(), GetActiveClient(), mNotify->mAnchor->mAnchor.GetActiveCharacter(), mNotify->mAnchor->mAnchor.GetListenCharacter());
	}
#endif

	if	(pActiveCharID == mCharID)
	{
		PropagateLangID ();
		if	(lCharacterWnd = GetCharacterWnd ())
		{
			lCharacterWnd->EnableIdle (mIdleEnabled);
			lCharacterWnd->EnableSound (mSoundEnabled);
		}
	}

	if	(
			(pInactiveCharID == mCharID)
		&&	(pActiveCharID != GetActiveClient ())
		)
	{
		CDaCmnCharacter *	lListenCharacter = NULL;

		if	(
				(IsListening ())
			&&	(
					(
						(pInputActiveCharID > 0)
					&&	(pInputActiveCharID == pActiveCharID)
					&&	(lListenCharacter = mNotify->mAnchor->mAnchor.GetGlobalCharacter (pInputActiveCharID))
					)
				||	(lListenCharacter = mNotify->mAnchor->mAnchor.GetGlobalCharacter (mNotify->mAnchor->mAnchor.GetListenCharacter ()))
				)
			&&	(lListenCharacter != this)
			)
		{
			TransferListeningState (lListenCharacter);
		}

		if	(lListenCharacter != this)
		{
			StopListening (false, ListenComplete_CharacterClientDeactivated);
		}
	}

	if	(
			(
				(pActiveCharID == mCharID)
			||	(pInactiveCharID == mCharID)
			)
		&&	(mListeningAnchor)
		&&	(lVoiceCommandsWnd = mListeningAnchor->GetVoiceCommandsWnd (false))
		&&	(lVoiceCommandsWnd->IsWindow ())
		)
	{
		CDaCmnCommands *	lCommands;

		if	(
				(pActiveCharID == mCharID)
			&&	(pInputActiveCharID == mCharID)
			)
		{
			if	(
					(mListeningAnchor)
				&&	(lVoiceCommandsWnd = mListeningAnchor->GetVoiceCommandsWnd (true, mCharID))
				)
			{
				lVoiceCommandsWnd->SetLangID (mLangID);

				if	(lCommands = GetCommands (true))
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

bool CDaCmnCharacter::_OnContextMenu (long pCharID, HWND pOwner, const CPoint & pPosition)
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

bool CDaCmnCharacter::_OnDefaultCommand (long pCharID, HWND pOwner, const CPoint & pPosition)
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

bool CDaCmnCharacter::DoContextMenu (HWND pOwner, const CPoint & pPosition)
{
	CDaCmnCommands *	lCommands;
	CDaCmnCommand *		lCommand;
	long				lCommandId;
	CVoiceCommandsWnd *	lVoiceCommandsWnd = NULL;

	if	(mListeningAnchor)
	{
		lVoiceCommandsWnd = mListeningAnchor->GetVoiceCommandsWnd (false);
	}
	if	(
			(lCommands = GetCommands (true))
		&&	(lCommandId = lCommands->DoContextMenu (pOwner, pPosition, lVoiceCommandsWnd))
		)
	{
		if	(HIWORD (lCommandId))
		{
			CDaCmnCharacter *	lCharacter;

			if	(
					(mNotify)
				&&	(mNotify->mAnchor)
				&&	(lCharacter = mNotify->mAnchor->mAnchor.GetGlobalCharacter (LOWORD (lCommandId)))
				)
			{
				lCharacter->DoMenuActivate ();
			}
		}
		else
		{
			if	(
					(!DoMenuCommand (LOWORD (lCommandId)))
				&&	(lCommand = lCommands->GetCommand (LOWORD (lCommandId)))
				&&	(lCommand->mEnabled)
				)
			{
				PreNotify ();
				try
				{
					mNotify->Command (lCommandId, NULL);
				}
				catch AnyExceptionDebug
				PostNotify ();
			}
		}
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

bool CDaCmnCharacter::DoDefaultCommand (HWND pOwner, const CPoint & pPosition)
{
//
//	MS Agent leaves default command processing to the calling application
//
#ifndef	_STRICT_COMPATIBILITY
	CDaCmnCommands *	lCommands;
	USHORT			lCommand;

	if	(
			(lCommands = GetCommands (true))
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

bool CDaCmnCharacter::DoMenuCommand (USHORT pCommandId)
{
	bool					lRet = false;
	CAgentCharacterWnd *	lCharacterWnd;
	CDaCmnCommands *		lCommands;

	if	(lCommands = GetCommands (true))
	{
		if	(pCommandId == lCommands->mHideCharacterCmdId)
		{
			StopAll (StopAll_Play|StopAll_Move|StopAll_Speak|StopAll_QueuedPrepare, AGENTREQERR_INTERRUPTEDUSER);
			if	(lCharacterWnd = GetCharacterWnd ())
			{
				lCharacterWnd->QueueHide (mCharID, false, VisibilityCause_UserHid);
				lCharacterWnd->ActivateQueue (true);
			}
			lRet = true;
		}
		else
		if	(pCommandId == lCommands->mShowCharacterCmdId)
		{
			StopAll (StopAll_Play|StopAll_Move|StopAll_Speak|StopAll_QueuedPrepare, AGENTREQERR_INTERRUPTEDUSER);
			if	(lCharacterWnd = GetCharacterWnd ())
			{
				lCharacterWnd->QueueShow (mCharID, false, VisibilityCause_UserShowed);
				lCharacterWnd->ActivateQueue (true);
			}
			lRet = true;
		}
		else
		if	(pCommandId == lCommands->mHideCommandsCmdId)
		{
			CVoiceCommandsWnd *	lVoiceCommandsWnd;

			if	(
					(mListeningAnchor)
				&&	(lVoiceCommandsWnd = mListeningAnchor->GetVoiceCommandsWnd (false))
				)
			{
				lVoiceCommandsWnd->Hide ();
			}
			lRet = true;
		}
		else
		if	(pCommandId == lCommands->mShowCommandsCmdId)
		{
			CVoiceCommandsWnd *	lVoiceCommandsWnd;

			if	(
					(mListeningAnchor)
				&&	(lVoiceCommandsWnd = mListeningAnchor->GetVoiceCommandsWnd (true, mCharID))
				)
			{
				lVoiceCommandsWnd->SetLangID (mLangID);
				lCommands->ShowVoiceCommands (lVoiceCommandsWnd);
				lVoiceCommandsWnd->ShowOtherClients (mCharID);
				lVoiceCommandsWnd->Show (true);
			}
			lRet = true;
		}
	}
	return lRet;
}

bool CDaCmnCharacter::DoMenuActivate ()
{
	if	(IsVisible ())
	{
		if	(
				(!IsInputActive ())
			||	(!IsClientActive ())
			)
		{
			SetActiveClient (true, true);
		}
	}
	else
	{
		if	(!IsClientActive ())
		{
			SetActiveClient (true, false);
		}
		Show (false);
	}
	return true;
}

bool CDaCmnCharacter::NotifyVoiceCommand (USHORT pCommandId, interface ISpRecoResult * pRecoResult, bool pGlobalCommand)
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CAgentCharacterWnd * CDaCmnCharacter::GetCharacterWnd (bool pMustExist) const
{
	if	(
			(mWnd)
		&&	(
				(!pMustExist)
			||	(mWnd->IsWindow ())
			)
		)
	{
		return mWnd;
	}
	return NULL;
}

CAgentPopupWnd * CDaCmnCharacter::GetPopupWnd (bool pMustExist) const
{
	if	(
			(mWnd)
		&&	(
				(!pMustExist)
			||	(mWnd->IsWindow ())
			)
		)
	{
		return dynamic_cast <CAgentPopupWnd *> (mWnd);
	}
	return NULL;
}

CAgentBalloonWnd * CDaCmnCharacter::GetBalloonWnd (bool pCreateObject)
{
	CAgentCharacterWnd *	lCharacterWnd;
	CAgentBalloonWnd *		lBalloonWnd = NULL;
	CDaCmnBalloon *			lBalloon = NULL;

	if	(lCharacterWnd = GetCharacterWnd ())
	{
		lBalloonWnd = lCharacterWnd->GetBalloonWnd ();

		if	(
				(
					(!lBalloonWnd)
				||	(!lBalloonWnd->IsWindow ())
				)
			&&	(pCreateObject)
			&&	(lBalloon = GetBalloon (true))
			&&	(lBalloon->get_Enabled (NULL) == S_OK)
			)
		{
			lBalloonWnd = lCharacterWnd->GetBalloonWnd (true);
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

CAgentListeningWnd * CDaCmnCharacter::GetListeningWnd (bool pCreateObject)
{
	CAgentCharacterWnd *	lCharacterWnd;
	CAgentListeningWnd *	lListeningWnd = NULL;

	if	(
			(lCharacterWnd = GetCharacterWnd ())
		&&	(
				(!pCreateObject)
			||	(CDaSettingsConfig().LoadConfig().mSrListeningTip)
			)
		)
	{
		lListeningWnd = lCharacterWnd->GetListeningWnd (pCreateObject);

		if	(
				(lListeningWnd)
			&&	(lListeningWnd->IsWindow ())
			)
		{
			if	(pCreateObject)
			{
				lListeningWnd->Attach (mCharID, CAtlString (GetName()));
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

LPVOID CDaCmnCharacter::FindOtherRequest (long pReqID, CDaCmnCharacter *& pOtherCharacter)
{
	LPVOID			lRet = NULL;
	CAgentFile *	lFile;
	INT_PTR			lFileNdx;

	pOtherCharacter = NULL;

	for	(lFileNdx = 0; lFile = mNotify->mAnchor->mAnchor.GetCachedFile (lFileNdx); lFileNdx++)
	{
		CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
		INT_PTR								lClientNdx;
		CDaCmnCharacter *					lCharacter;
		CAgentCharacterWnd *				lCharacterWnd;

		if	(mNotify->mAnchor->mAnchor.GetFileClients (lFile, lFileClients))
		{
			for	(lClientNdx = lFileClients.GetCount()-1; lClientNdx >= 0; lClientNdx--)
			{
				if	(
						(lCharacter = dynamic_cast <CDaCmnCharacter *> (lFileClients [lClientNdx]))
					&&	(lCharacter->IsValid (lFile))
					&&	(lCharacterWnd = lCharacter->GetCharacterWnd ())
					&&	(lRet = lCharacterWnd->FindQueuedAction (pReqID))
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

void CDaCmnCharacter::_OnOptionsChanged ()
{
	CDaSettingsConfig		lSettingsConfig;
	CAgentCharacterWnd *	lCharacterWnd;
	CAgentListeningWnd *	lListeningWnd;
	CAgentBalloonWnd *		lBalloonWnd;
	CDaCmnBalloon *			lBalloon;

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
			(lCharacterWnd = GetCharacterWnd ())
		&&	(lCharacterWnd->GetCharID() == GetCharID())
		)
	{
		lCharacterWnd->EnableSound (mSoundEnabled);
	}

	if	(
			(mFile)
		&&	(lBalloon = GetBalloon (true))
		)
	{
		lBalloon->SetLangID (mLangID);

		if	(lBalloon->get_Enabled (NULL) == S_FALSE)
		{
			if	(
					(lBalloonWnd = GetBalloonWnd (false))
				&&	(lBalloonWnd->GetCharID() == GetCharID())
				)
			{
				lBalloonWnd->HideBalloon (true);
			}
			if	(lCharacterWnd = GetCharacterWnd ())
			{
				lCharacterWnd->RemoveQueuedActions (QueueActionThink, mCharID, AGENTREQERR_INTERRUPTEDCODE, _T("OptionsChanged"));
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

void CDaCmnCharacter::_OnDefaultCharacterChanged (REFGUID pCharGuid, LPCTSTR pFilePath)
{
#ifdef	_DEBUG_DEFAULT_CHAR
	if	(
			(IsDefault ())
		&&	(LogIsActive (_DEBUG_DEFAULT_CHAR))
		)
	{
		LogMessage (_DEBUG_DEFAULT_CHAR, _T("[%p] [%d] CDaCmnCharacter::_OnDefaultCharacterChanged [%s] to [%s]"), this, mCharID, GetName(), pFilePath);
	}
#endif
	StopListening (false, ListenComplete_DefaultCharacterChanged);

	if	(IsDefault ())
	{
		try
		{
			HRESULT					lResult;
			CAgentCharacterWnd *	lCharacterWnd = GetCharacterWnd (false);
			CAgentPopupWnd *		lPopupWnd = GetPopupWnd (false);
			CAgentFile *			lOldFile = GetFile ();
			tPtr <CAgentFile>		lLoadFile;
			CAgentFile *			lNewFile;

			if	(
					(lCharacterWnd)
				&&	(lOldFile)
				&&	(!IsEqualGUID (lOldFile->GetGuid(), pCharGuid))
				&&	(
						(lNewFile = mNotify->mAnchor->mAnchor.FindCachedFile (pCharGuid))
					||	(
							(lLoadFile = CAgentFile::CreateInstance())
						&&	(SUCCEEDED (LogComErr (LogNormal, lLoadFile->Open (pFilePath))))
						&&	(lNewFile = lLoadFile)
						)
					)
				)
			{
				try
				{
					bool	lVisible = IsVisible ();
					bool	lClientActive = IsClientActive ();
					bool	lIsInputActive = IsInputActive ();
					long	lActiveClient = GetActiveClient ();
					DWORD	lStyle = GetStyle ();
					LANGID	lLangID = GetLangID ();
					CPoint	lPosition;
					CSize	lSize;
					CSize	lDefaultSize;

					GetPosition (&lPosition.x, &lPosition.y);
					GetSize (&lSize.cx, &lSize.cy);
					GetOriginalSize (&lDefaultSize.cx, &lDefaultSize.cy);
#ifdef	_DEBUG_DEFAULT_CHAR
					if	(LogIsActive (_DEBUG_DEFAULT_CHAR))
					{
						LogMessage (_DEBUG_DEFAULT_CHAR, _T("[%p] [%d] Reinitialize   [%u %u %u %d %8.8X %4.4X] [%s] [%s] [%s]"), this, mCharID, lVisible, lClientActive, lIsInputActive, lActiveClient, lStyle, lLangID, FormatPoint(lPosition), FormatSize(lSize), FormatSize(lDefaultSize));
					}
#endif
					Terminate (false);

					if	(SUCCEEDED (lResult = LogComErr (LogNormal, OpenFile (lNewFile, true))))
					{
						Initialize (mCharID, mNotify, mListeningAnchor);
						lLoadFile.Detach ();
					}
					if	(
							(SUCCEEDED (lResult))
						&&	(
								(lPopupWnd)
							?	(SUCCEEDED (lResult = LogComErr (LogNormal, RealizePopup (mNotify->mAnchor->mOwnerWnd, lStyle))))
							:	(SUCCEEDED (lResult = LogComErr (LogNormal, Realize (lCharacterWnd, lStyle))))
							)
						)
					{
						CSize	lOldSize (lSize);
						CSize	lOldDefaultSize (lDefaultSize);

						GetOriginalSize (&lDefaultSize.cx, &lDefaultSize.cy);
						if	(lSize == lOldDefaultSize)
						{
							lSize = lDefaultSize;
						}
#ifdef	_DEBUG_DEFAULT_CHAR
						if	(LogIsActive (_DEBUG_DEFAULT_CHAR))
						{
							LogMessage (_DEBUG_DEFAULT_CHAR, _T("[%p] [%d] Reinitializing [%u %u %u %d %8.8X %4.4X] [%s] [%s] [%s]"), this, mCharID, IsVisible(), IsClientActive(), IsInputActive(), GetActiveClient(), GetStyle(), GetLangID(), FormatPoint(lPosition), FormatSize(lSize), FormatSize(lDefaultSize));
						}
#endif
						if	(lPopupWnd)
						{
							SetPosition (lPosition.x, lPosition.y);
							SetSize (lSize.cx, lSize.cy);
						}
						else
						{
							CRect	lVideoRect (lPosition, lSize);
							CRect	lOldRect (lPosition, lOldSize);
							CRect	lClientRect;

							lCharacterWnd->GetClientRect (&lClientRect);
							lVideoRect.OffsetRect (lOldRect.CenterPoint().x-lVideoRect.CenterPoint().x, lOldRect.CenterPoint().y-lVideoRect.CenterPoint().y);
							lVideoRect.OffsetRect (min (lClientRect.right-lVideoRect.right, 0), min (lClientRect.bottom-lVideoRect.bottom, 0));
							lVideoRect.OffsetRect (max (lClientRect.left-lVideoRect.left, 0), max (lClientRect.top-lVideoRect.top, 0));
							lCharacterWnd->SetVideoRect (lVideoRect);
						}

						if	(
								(FAILED (SetLangID (lLangID)))
							&&	(FAILED (SetLangID (GetUserDefaultUILanguage ())))
							)
						{
							SetLangID (MAKELANGID (LANG_ENGLISH, SUBLANG_DEFAULT));
						}
						mNotify->mGlobal->_CharacterNameChanged (mCharID);

						if	(lVisible)
						{
							Show (true, true);
							if	(lIsInputActive)
							{
								SetActiveClient (true, true);
							}
							else
							if	(lClientActive)
							{
								SetActiveClient (true, false);
							}
							Show (false);
						}
#ifdef	_DEBUG_DEFAULT_CHAR
						if	(LogIsActive (_DEBUG_DEFAULT_CHAR))
						{
							GetPosition (&lPosition.x, &lPosition.y);
							GetSize (&lSize.cx, &lSize.cy);
							LogMessage (_DEBUG_DEFAULT_CHAR, _T("[%p] [%d] Reinitialized  [%u %u %u %d %8.8X %4.4X] [%s] [%s] [%s]"), this, mCharID, IsVisible(), IsClientActive(), IsInputActive(), GetActiveClient(), GetStyle(), GetLangID(), FormatPoint(lPosition), FormatSize(lSize), FormatSize(lDefaultSize));
						}
#endif
					}
					else
					{
						Terminate (false);
#ifdef	_DEBUG_DEFAULT_CHAR
						if	(LogIsActive (_DEBUG_DEFAULT_CHAR))
						{
							LogMessage (_DEBUG_DEFAULT_CHAR, _T("[%p] [%d] Reinitialize failed"), this, mCharID);
						}
#endif
					}
				}
				catch AnyExceptionDebug
			}
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::SetPosition (long Left, long Top)
{
	HRESULT					lResult = S_OK;
	CAgentCharacterWnd *	lCharacterWnd;
	CAgentPopupWnd *		lPopupWnd;

	if	(lPopupWnd = GetPopupWnd ())
	{
#ifdef	_STRICT_COMPATIBILITY
		if	(!lPopupWnd->MovePopup (CPoint (Left, Top), 0, MoveCause_ProgramMoved, true))
#else
		if	(!lPopupWnd->MovePopup (CPoint (Left, Top), 0, MoveCause_ProgramMoved))
#endif
		{
			lResult = S_FALSE;
		}
	}
	else
	if	(lCharacterWnd = GetCharacterWnd ())
	{
		lResult = lCharacterWnd->SetVideoRect (CRect (CPoint (Left, Top), lCharacterWnd->GetVideoRect().Size()));
	}
	else
	{
		lResult = AGENTERR_CLIENTINVALID;
	}
	return lResult;
}

HRESULT CDaCmnCharacter::GetPosition (long *Left, long *Top)
{
	HRESULT					lResult = S_OK;
	CAgentCharacterWnd *	lCharacterWnd;
	CAgentPopupWnd *		lPopupWnd;
	CRect					lRect (0,0,0,0);

	if	(lPopupWnd = GetPopupWnd ())
	{
		lPopupWnd->GetWindowRect (&lRect);
	}
	else
	if	(lCharacterWnd = GetCharacterWnd ())
	{
		lRect = lCharacterWnd->GetVideoRect ();
	}
	else
	{
		lResult = AGENTERR_CLIENTINVALID;
	}
	if	(Left)
	{
		(*Left) = lRect.left;
	}
	if	(Top)
	{
		(*Top) = lRect.top;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::SetSize (long Width, long Height)
{
	HRESULT					lResult = S_OK;
	CAgentCharacterWnd *	lCharacterWnd;
	CAgentPopupWnd *		lPopupWnd;

	if	(lPopupWnd = GetPopupWnd ())
	{
		if	(!lPopupWnd->SizePopup (CSize (Width, Height), 0))
		{
#ifndef	_STRICT_COMPATIBILITY
			lResult = S_FALSE;
#endif
		}
	}
	else
	if	(lCharacterWnd = GetCharacterWnd ())
	{
		lResult = lCharacterWnd->SetVideoRect (CRect (lCharacterWnd->GetVideoRect().TopLeft(), CSize (Width, Height)));
	}
	else
	{
		lResult = AGENTERR_CLIENTINVALID;
	}
	return lResult;
}

HRESULT CDaCmnCharacter::GetSize (long *Width, long *Height)
{
	HRESULT					lResult = S_OK;
	CAgentCharacterWnd *	lCharacterWnd;
	CAgentPopupWnd *		lPopupWnd;
	CRect					lRect (0,0,0,0);

	if	(lPopupWnd = GetPopupWnd ())
	{
		lPopupWnd->GetWindowRect (&lRect);
	}
	else
	if	(lCharacterWnd = GetCharacterWnd ())
	{
		lRect = lCharacterWnd->GetVideoRect ();
	}
	else
	{
		lResult = AGENTERR_CLIENTINVALID;
	}
	if	(Width)
	{
		(*Width) = lRect.Width();
	}
	if	(Height)
	{
		(*Height) = lRect.Height();
	}
	return lResult;
}

HRESULT CDaCmnCharacter::GetOriginalSize (long *Width, long *Height)
{
	HRESULT	lResult = S_OK;
	CSize	lImageSize (0,0);

	if	(!mFile)
	{
		lResult = AGENTERR_CLIENTINVALID;
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
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::GetVersion(short *MajorVersion, short *MinorVersion)
{
	HRESULT	lResult = S_OK;

	if	(!mFile)
	{
		lResult = AGENTERR_CLIENTINVALID;
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
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::Show (long Fast, long *RequestID)
{
	HRESULT	lResult = S_OK;
	long	lReqID = 0;

	if	(GetCharacterWnd ())
	{
		lReqID = Show (Fast!=0);
	}
	else
	{
		lResult = AGENTERR_CLIENTINVALID;
	}
	if	(RequestID)
	{
		(*RequestID) = lReqID;
	}
	return lResult;
}

HRESULT CDaCmnCharacter::Hide (long Fast, long *RequestID)
{
	HRESULT	lResult = S_OK;
	long	lReqID = 0;

	if	(GetCharacterWnd ())
	{
		lReqID = Hide (Fast!=0);
	}
	else
	{
		lResult = AGENTERR_CLIENTINVALID;
	}
	if	(RequestID)
	{
		(*RequestID) = lReqID;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::Prepare (long Type, BSTR Name, long Queue, long *RequestID)
{
	HRESULT	lResult;
	long	lReqID = 0;

	if	(!Name)
	{
		lResult = E_POINTER;
	}
	else
	if	(GetCharacterWnd ())
	{
		lResult = DoPrepare (Type, CAtlString (Name), (Queue != 0), lReqID);
	}
	else
	{
		lResult = AGENTERR_CLIENTINVALID;
	}

	if	(RequestID)
	{
		(*RequestID) = lReqID;
	}
	return lResult;
}

HRESULT CDaCmnCharacter::Play (BSTR Animation, long *RequestID)
{
	HRESULT					lResult = S_OK;
	CAgentCharacterWnd *	lCharacterWnd;
	long					lReqID = 0;

	if	(!Animation)
	{
		lResult = E_POINTER;
	}
	else
	if	(lCharacterWnd = GetCharacterWnd ())
	{
		lReqID = lCharacterWnd->QueueGesture (mCharID, Animation);
		if	(lReqID)
		{
			lCharacterWnd->ActivateQueue (true);
		}
		else
		{
			lResult = AGENTERR_ANIMATIONNOTFOUND;
		}
	}
	else
	{
		lResult = AGENTERR_CLIENTINVALID;
	}
	if	(RequestID)
	{
		(*RequestID) = lReqID;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::Stop (long RequestID)
{
	HRESULT					lResult = S_OK;
	CAgentCharacterWnd *	lCharacterWnd;
	LPVOID					lRequest = NULL;
	CDaCmnCharacter *		lOtherCharacter = NULL;

	if	(RequestID <= 0)
	{
		lResult = AGENTREQERR_OBJECTINVALID;
	}
	else
	if	(lCharacterWnd = GetCharacterWnd ())
	{
		if	(lRequest = lCharacterWnd->FindQueuedAction (RequestID))
		{
			if	(!lCharacterWnd->RemoveQueuedAction ((CQueuedAction *) lRequest, AGENTREQERR_INTERRUPTEDCODE, _T("Stop")))
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
	else
	{
		lResult = AGENTERR_CLIENTINVALID;
	}
	return lResult;
}

HRESULT CDaCmnCharacter::StopAll (long Types)
{
	HRESULT	lResult = S_FALSE;

	if	(GetCharacterWnd ())
	{
		lResult = StopAll (Types, AGENTREQERR_INTERRUPTEDCODE);
	}
	else
	{
		lResult = AGENTERR_CLIENTINVALID;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::MoveTo (short X, short Y, long Speed, long *RequestID)
{
	HRESULT				lResult = S_OK;
	CAgentPopupWnd *	lPopupWnd;
	long				lReqID = 0;

	if	(lPopupWnd = GetPopupWnd ())
	{
		if	(lPopupWnd->IsDragging())
		{
			lResult = AGENTERR_CANTMOVEDURINGDRAG;
		}
		else
		if	(Speed > 0)
		{
			lReqID = lPopupWnd->QueueMove (mCharID, CPoint (X, Y), Speed);
			if	(lReqID)
			{
				lPopupWnd->ActivateQueue (true);
			}
			else
			{
				lResult = SetPosition (X, Y);
			}
		}
		else
		{
			lResult = SetPosition (X, Y);
		}
	}
	else
	{
		lResult = AGENTERR_CLIENTINVALID;
	}
	if	(RequestID)
	{
		(*RequestID) = lReqID;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::GestureAt (short X, short Y, long *RequestID)
{
	HRESULT					lResult = S_OK;
	CAgentCharacterWnd *	lCharacterWnd;
	long					lReqID = 0;

	if	(lCharacterWnd = GetCharacterWnd ())
	{
		CPoint		lOffset (X, Y);
		CRect		lWinRect;
		CAtlString	lStateName;

		lCharacterWnd->GetWindowRect (&lWinRect);
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

		lReqID = lCharacterWnd->QueueState (mCharID, lStateName);
		if	(lReqID)
		{
			lCharacterWnd->ActivateQueue (true);
		}
#ifndef	_STRICT_COMPATIBILITY
		else
		{
			lResult = AGENTERR_STATENOTFOUND;
		}
#endif
	}
	else
	{
		lResult = AGENTERR_CLIENTINVALID;
	}
	if	(RequestID)
	{
		(*RequestID) = lReqID;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::Think (BSTR Text, class CAgentTextObject * pTextObject, long *RequestID)
{
	HRESULT					lResult = S_OK;
	CAgentCharacterWnd *	lCharacterWnd;
	long					lReqID = 0;

	if	(lCharacterWnd = GetCharacterWnd ())
	{
		CDaCmnBalloon *				lBalloon = GetBalloon (true);
		tPtr <CAgentBalloonOptions>	lBalloonOptions;

		if	(lBalloon->get_Enabled (NULL) == S_OK)
		{
			GetBalloonWnd (true);
			lBalloonOptions = lBalloon->GetBalloonOptions ();
			lReqID = lCharacterWnd->QueueThink (mCharID, CAtlString (Text), pTextObject, lBalloonOptions, GetSapiVoice(true)->SafeIsValid());
			lCharacterWnd->ActivateQueue (true);
		}
		else
		{
			lResult = AGENTERR_NOBALLOON;
		}
	}
	else
	{
		lResult = AGENTERR_CLIENTINVALID;
	}
	if	(RequestID)
	{
		(*RequestID) = lReqID;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::Wait (long WaitForRequestID, long *RequestID)
{
	HRESULT					lResult = S_OK;
	CAgentCharacterWnd *	lCharacterWnd;
	LPVOID					lOtherRequest = NULL;
	CDaCmnCharacter *		lOtherCharacter = NULL;
	long					lReqID = 0;

	if	(RequestID <= 0)
	{
		lResult = AGENTREQERR_OBJECTINVALID;
	}
	else
	if	(lCharacterWnd = GetCharacterWnd ())
	{
		if	(lOtherRequest = FindOtherRequest (WaitForRequestID, lOtherCharacter))
		{
			if	(
					(lOtherCharacter == this)
				||	(lOtherCharacter->GetCharacterWnd () == lCharacterWnd)
				)
			{
				lResult = AGENTREQERR_CANTWAITONSELF;
			}
			else
			{
				lReqID = lCharacterWnd->QueueWait (mCharID, lOtherCharacter->mCharID, WaitForRequestID);
				lCharacterWnd->ActivateQueue (true);
			}
		}
		else
		{
#ifndef	_STRICT_COMPATIBILITY
			lResult = AGENTREQERR_OBJECTNOTFOUND;
#endif
		}
	}
	else
	{
		lResult = AGENTERR_CLIENTINVALID;
	}
	if	(RequestID)
	{
		(*RequestID) = lReqID;
	}
	return lResult;
}

HRESULT CDaCmnCharacter::Interrupt (long InterruptRequestID, long *RequestID)
{
	HRESULT					lResult = S_OK;
	CAgentCharacterWnd *	lCharacterWnd;
	LPVOID					lOtherRequest = NULL;
	CDaCmnCharacter *		lOtherCharacter = NULL;
	long					lReqID = 0;

	if	(RequestID <= 0)
	{
		lResult = AGENTREQERR_OBJECTINVALID;
	}
	else
	if	(lCharacterWnd = GetCharacterWnd ())
	{
		if	(lOtherRequest = FindOtherRequest (InterruptRequestID, lOtherCharacter))
		{
			if	(
					(lOtherCharacter == this)
				||	(lOtherCharacter->GetCharacterWnd () == lCharacterWnd)
				)
			{
				lResult = AGENTREQERR_CANTINTERRUPTSELF;
			}
			else
			{
				lReqID = lCharacterWnd->QueueInterrupt (mCharID, lOtherCharacter->mCharID, InterruptRequestID);
				lCharacterWnd->ActivateQueue (true);
			}
		}
		else
		{
#ifndef	_STRICT_COMPATIBILITY
			lResult = AGENTREQERR_OBJECTNOTFOUND;
#endif
		}
	}
	else
	{
		lResult = AGENTERR_CLIENTINVALID;
	}
	if	(RequestID)
	{
		(*RequestID) = lReqID;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::ShowPopupMenu (short X, short Y)
{
	HRESULT					lResult = S_OK;
	CAgentCharacterWnd *	lCharacterWnd;

	if	(mNotify->mAnchor->mAnchor.GetActiveCharacter() != GetCharID())
	{
		lResult = AGENTERR_CHARACTERNOTACTIVE;
	}
	else
	if	(lCharacterWnd = GetCharacterWnd ())
	{
		if	(!DoContextMenu (lCharacterWnd->m_hWnd, CPoint (X, Y)))
		{
			lResult = S_FALSE;
		}
	}
	else
	{
		lResult = AGENTERR_CLIENTINVALID;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::GetTTSSpeed (long *Speed)
{
	HRESULT	lResult = S_OK;

	if	(!Speed)
	{
		lResult = E_POINTER;
	}
	else
	if	(!mFile)
	{
		lResult = AGENTERR_CLIENTINVALID;
	}
	else
#ifndef	_STRICT_COMPATIBILITY
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
#endif
	{
		(*Speed) = CDaSettingsConfig().ApplyVoiceRate (mFile->GetTts().mSpeed);
	}
	return lResult;
}

HRESULT CDaCmnCharacter::GetTTSPitch (short *Pitch)
{
	HRESULT	lResult = S_OK;

	if	(!Pitch)
	{
		lResult = E_POINTER;
	}
	else
	if	(!mFile)
	{
		lResult = AGENTERR_CLIENTINVALID;
	}
	else
#ifndef	_STRICT_COMPATIBILITY
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
#endif
	{
		(*Pitch) = mFile->GetTts().mPitch;
	}
	return lResult;
}

HRESULT CDaCmnCharacter::Speak (BSTR Text, class CAgentTextObject * pTextObject, BSTR Url, long *RequestID)
{
	HRESULT					lResult = S_OK;
	CAgentCharacterWnd *	lCharacterWnd;
	long					lReqID = 0;

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
	if	(lCharacterWnd = GetCharacterWnd ())
	{
		CDaCmnBalloon *				lBalloon = GetBalloon (true);
		tPtr <CAgentBalloonOptions>	lBalloonOptions;
		CAtlString					lText (Text);
		CAtlString					lSoundUrl (Url);
		CSapiVoice *				lVoice = NULL;
//
//	MS Agent shows the speech balloon silently when the character is listening.
//	For now, we'll just stop listening.
//
		StopListening (false, ListenComplete_CharacterClientDeactivated);

		if	(lSoundUrl.IsEmpty ())
		{
			lVoice = GetSapiVoice (true);
			if	(lVoice)
			{
				lBalloonOptions = lBalloon->GetBalloonOptions ();
			}
			else
			{
				lResult = AGENTERR_TTSLANGUAGENOTFOUND;
			}
		}
		else
		{
			lVoice = GetSapiVoice (false);
			if	(!lText.IsEmpty ())
			{
				lBalloonOptions = lBalloon->GetBalloonOptions ();
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
			if	(lBalloonOptions)
			{
				GetBalloonWnd (true);
			}
			lReqID = lCharacterWnd->QueueSpeak (mCharID, lText, pTextObject, lSoundUrl, lVoice, lBalloonOptions);
			lCharacterWnd->ActivateQueue (true);
		}
	}
	else
	{
		lResult = AGENTERR_CLIENTINVALID;
	}
	if	(RequestID)
	{
		(*RequestID) = lReqID;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::Listen (long Listen)
{
	HRESULT	lResult;

	if	(Listen)
	{
		lResult = StartListening (true);
	}
	else
	{
		lResult = StopListening (true, ListenComplete_ProgramDisabled);
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::get_CharacterID (long *CharacterID)
{
	HRESULT	lResult = S_OK;

	if	(!CharacterID)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*CharacterID) = mCharID;
	}
	return lResult;
}

HRESULT CDaCmnCharacter::get_UniqueID (BSTR *CharGUID)
{
	HRESULT	lResult = S_OK;

	if	(!CharGUID)
	{
		lResult = E_POINTER;
	}
	else
	if	(!mFile)
	{
		lResult = AGENTERR_CLIENTINVALID;
	}
	else
	{
		(*CharGUID) = ((CString)CGuidStr (mFile->GetGuid())).AllocSysString();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::get_Name (BSTR *Name)
{
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
			lResult = AGENTERR_CLIENTINVALID;
		}
		else
		if	(
				(mFile->GetNames().GetCount() <= 0)
			&&	(!mFile->ReadNames ())
			)
		{
			lResult = AGENTPROVERROR_CHARACTERVERSION;
		}
		else
		if	(lFileName = mFile->FindName (mLangID))
		{
			(*Name) = tBstrPtr (lFileName->mName).Detach();
		}
		else
		{
			lResult = AGENTPROVERROR_CHARACTERVERSION;
		}
	}
	return lResult;
}

HRESULT CDaCmnCharacter::put_Name (BSTR Name)
{
	HRESULT				lResult = S_OK;
	CAtlString			lName (Name);
	CAgentFileName *	lFileName;

	lName.TrimLeft ();
	lName.TrimRight ();

	if	(!mFile)
	{
		lResult = AGENTERR_CLIENTINVALID;
	}
	else
	if	(lName.IsEmpty ())
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		if	(lFileName = mFile->FindName (mLangID))
		{
			if	(CAtlString ((BSTR)lFileName->mName) != lName)
			{
				lFileName->mName = lName.AllocSysString ();
				mNotify->mGlobal->_CharacterNameChanged (mCharID);
			}
		}
		else
		{
			lResult = S_FALSE;
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::get_Description (BSTR *Description)
{
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
			lResult = AGENTERR_CLIENTINVALID;
		}
		else
		if	(
				(mFile->GetNames().GetCount() <= 0)
			&&	(!mFile->ReadNames ())
			)
		{
			lResult = AGENTPROVERROR_CHARACTERVERSION;
		}
		else
		if	(lFileName = mFile->FindName (mLangID))
		{
			(*Description) = tBstrPtr (lFileName->mDesc1).Detach();
		}
		else
		{
			lResult = AGENTPROVERROR_CHARACTERVERSION;
		}
	}
	return lResult;
}

HRESULT CDaCmnCharacter::put_Description (BSTR Description)
{
	HRESULT				lResult = S_OK;
	CAtlString			lDescription (Description);
	CAgentFileName *	lFileName;

	lDescription.TrimLeft ();
	lDescription.TrimRight ();

	if	(!mFile)
	{
		lResult = AGENTERR_CLIENTINVALID;
	}
	else
	{
		if	(lFileName = mFile->FindName (mLangID))
		{
			lFileName->mDesc1 = lDescription.AllocSysString ();
		}
		else
		{
			lResult = S_FALSE;
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::get_ExtraData (BSTR *ExtraData)
{
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
			lResult = AGENTERR_CLIENTINVALID;
		}
		else
		if	(
				(mFile->GetNames().GetCount() <= 0)
			&&	(!mFile->ReadNames ())
			)
		{
			lResult = AGENTPROVERROR_CHARACTERVERSION;
		}
		else
		if	(lFileName = mFile->FindName (mLangID))
		{
			(*ExtraData) = tBstrPtr (lFileName->mDesc2).Detach();
		}
		else
		{
			lResult = AGENTPROVERROR_CHARACTERVERSION;
		}
	}
	return lResult;
}

HRESULT CDaCmnCharacter::get_FileName (BSTR *FileName)
{
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
			lResult = AGENTERR_CLIENTINVALID;
		}
		else
		{
			(*FileName) = mFile->GetFileName().Detach();
		}
	}
	return lResult;
}

HRESULT CDaCmnCharacter::get_FilePath (BSTR *FilePath)
{
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
			lResult = AGENTERR_CLIENTINVALID;
		}
		else
		{
			(*FilePath) = mFile->GetPath().Detach();
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::get_Animations (SAFEARRAY **Animations)
{
	HRESULT	lResult = S_OK;

	if	(!Animations)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Animations) = NULL;

		if	(!mFile)
		{
			lResult = AGENTERR_CLIENTINVALID;
		}
		else
		{
			(*Animations) = mFile->GetGestureNames ();

			if	(!(*Animations))
			{
				lResult = E_OUTOFMEMORY;
			}
		}
	}
	return lResult;
}

HRESULT CDaCmnCharacter::get_States (SAFEARRAY **States)
{
	HRESULT	lResult = S_OK;

	if	(!States)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*States) = NULL;

		if	(!mFile)
		{
			lResult = AGENTERR_CLIENTINVALID;
		}
		else
		{
			(*States) = mFile->GetStateNames ();

			if	(!(*States))
			{
				lResult = E_OUTOFMEMORY;
			}
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::get_LanguageID (long *LanguageID)
{
	HRESULT	lResult = S_OK;

	if	(!LanguageID)
	{
		lResult = E_POINTER;
	}
	else
	if	(!mFile)
	{
		lResult = AGENTERR_CLIENTINVALID;
	}
	else
	{
		(*LanguageID) = (long)mLangID;
	}
	return lResult;
}

HRESULT CDaCmnCharacter::put_LanguageID (long LanguageID)
{
	HRESULT	lResult = S_OK;
	LANGID	lLangID = LOWORD (LanguageID);

#ifdef	_DEBUG_LANGUAGE
	if	(LogIsActive (_DEBUG_LANGUAGE))
	{
		LogMessage (_DEBUG_LANGUAGE, _T("[%p] [%d] CDaCmnCharacter put_LanguageID [%4.4hX]"), this, mCharID, LanguageID);
	}
#endif

	if	(!mFile)
	{
		lResult = AGENTERR_CLIENTINVALID;
	}
	else
	{
		if	(mLangID != lLangID)
		{
			lResult = SetLangID (lLangID);
			if	(SUCCEEDED (lResult))
			{
				mNotify->mGlobal->_CharacterNameChanged (mCharID);
			}
		}
#ifndef	_STRICT_COMPATIBILITY
		else
		{
			lResult = S_FALSE;
		}
#endif
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::get_TTSModeID (BSTR *TTSModeID)
{
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
		(*TTSModeID) = CAtlString().AllocSysString();
		lResult = S_FALSE;
	}
	return lResult;
}

HRESULT CDaCmnCharacter::put_TTSModeID (BSTR TTSModeID)
{
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
			CAgentCharacterWnd *	lCharacterWnd;
			CSapiVoice *			lPrevVoice = mSapiVoice;

#ifdef	_DEBUG_LANGUAGE
			if	(LogIsActive (_DEBUG_LANGUAGE))
			{
				LogMessage (_DEBUG_LANGUAGE, _T("[%p] [%d] CDaCmnCharacter put_TTSModeID [%ls]"), this, mCharID, TTSModeID);
			}
#endif
			try
			{
				if	(
						(lCharacterWnd = GetCharacterWnd ())
					&&	(lCharacterWnd->NextQueuedAction (mCharID))
					&&	(lCharacterWnd->IsSpeakQueued (mCharID) == mWnd->NextQueuedAction (mCharID))
					)
				{
					lCharacterWnd->RemoveQueuedAction (lCharacterWnd->NextQueuedAction (mCharID), AGENTREQERR_INTERRUPTEDCODE, _T("put_TTSModeID"));
				}
				if	(mSapiVoice)
				{
					mSapiVoice->Stop ();
				}
			}
			catch AnyExceptionDebug

			mSapiVoice = NULL;

			if	(GetSapiVoice (true, CAtlString (TTSModeID)))
			{
				if	(lCharacterWnd = GetCharacterWnd ())
				{
					lCharacterWnd->UpdateQueuedSpeak (mCharID, mSapiVoice);
				}
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
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::get_SRModeID (BSTR *SRModeID)
{
	HRESULT	lResult = S_OK;

	if	(!SRModeID)
	{
		lResult = E_POINTER;
	}
	else
	if	(GetSapiInput (true))
	{
		(*SRModeID) = mSapiInput->ShortEngineId (mSapiInput->GetEngineId()).Detach();
	}
	else
	{
		(*SRModeID) = CAtlString().AllocSysString();
		lResult = S_FALSE;
	}
	return lResult;
}

HRESULT CDaCmnCharacter::put_SRModeID (BSTR SRModeID)
{
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

#ifdef	_DEBUG_LANGUAGE
			if	(LogIsActive (_DEBUG_LANGUAGE))
			{
				LogMessage (_DEBUG_LANGUAGE, _T("[%p] [%d] CDaCmnCharacter put_SRModeID [%ls]"), this, mCharID, SRModeID);
			}
#endif
			mSapiInput = NULL;

			if	(GetSapiInput (true, CAtlString (SRModeID)))
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
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::get_Left (short *Left)
{
	HRESULT					lResult = S_OK;
	CAgentCharacterWnd *	lCharacterWnd;
	CAgentPopupWnd *		lPopupWnd;
	CRect					lRect (0,0,0,0);

	if	(!Left)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(lPopupWnd = GetPopupWnd ())
		{
			lPopupWnd->GetWindowRect (&lRect);
		}
		else
		if	(lCharacterWnd = GetCharacterWnd ())
		{
			lRect = lCharacterWnd->GetVideoRect ();
		}
		else
		{
			lResult = AGENTERR_CLIENTINVALID;
		}
		(*Left) = (short)lRect.left;
	}
	return lResult;
}

HRESULT CDaCmnCharacter::put_Left (short Left)
{
	HRESULT					lResult = S_OK;
	CAgentCharacterWnd *	lCharacterWnd;
	CAgentPopupWnd *		lPopupWnd;
	CRect					lRect;

	if	(lPopupWnd = GetPopupWnd ())
	{
		lPopupWnd->GetWindowRect (&lRect);
		if	(!lPopupWnd->MovePopup (CPoint (Left, lRect.top), 0, MoveCause_ProgramMoved))
		{
			lResult = S_FALSE;
		}
	}
	else
	if	(lCharacterWnd = GetCharacterWnd ())
	{
		lRect = lCharacterWnd->GetVideoRect();
		lRect.OffsetRect (Left - lRect.left, 0);
		lResult = lCharacterWnd->SetVideoRect (lRect);
	}
	else
	{
		lResult = AGENTERR_CLIENTINVALID;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::get_Top (short *Top)
{
	HRESULT					lResult = S_OK;
	CAgentCharacterWnd *	lCharacterWnd;
	CAgentPopupWnd *		lPopupWnd;
	CRect					lRect (0,0,0,0);

	if	(!Top)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(lPopupWnd = GetPopupWnd ())
		{
			lPopupWnd->GetWindowRect (&lRect);
		}
		else
		if	(lCharacterWnd = GetCharacterWnd ())
		{
			lRect = lCharacterWnd->GetVideoRect ();
		}
		else
		{
			lResult = AGENTERR_CLIENTINVALID;
		}
		(*Top) = (short)lRect.top;
	}
	return lResult;
}

HRESULT CDaCmnCharacter::put_Top (short Top)
{
	HRESULT					lResult = S_OK;
	CAgentCharacterWnd *	lCharacterWnd;
	CAgentPopupWnd *		lPopupWnd;
	CRect					lRect;

	if	(lPopupWnd = GetPopupWnd ())
	{
		lPopupWnd->GetWindowRect (&lRect);
		if	(!lPopupWnd->MovePopup (CPoint (lRect.left, Top), 0, MoveCause_ProgramMoved))
		{
			lResult = S_FALSE;
		}
	}
	else
	if	(lCharacterWnd = GetCharacterWnd ())
	{
		lRect = lCharacterWnd->GetVideoRect();
		lRect.OffsetRect (0, Top - lRect.top);
		lResult = lCharacterWnd->SetVideoRect (lRect);
	}
	else
	{
		lResult = AGENTERR_CLIENTINVALID;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::get_Width (short *Width)
{
	HRESULT					lResult = S_OK;
	CAgentCharacterWnd *	lCharacterWnd;
	CAgentPopupWnd *		lPopupWnd;
	CRect					lRect (0,0,0,0);

	if	(!Width)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(lPopupWnd = GetPopupWnd ())
		{
			lPopupWnd->GetWindowRect (&lRect);
		}
		else
		if	(lCharacterWnd = GetCharacterWnd ())
		{
			lRect = lCharacterWnd->GetVideoRect ();
		}
		else
		{
			lResult = AGENTERR_CLIENTINVALID;
		}
		(*Width) = (short)lRect.Width();
	}
	return lResult;
}

HRESULT CDaCmnCharacter::put_Width (short Width)
{
	HRESULT					lResult = S_OK;
	CAgentCharacterWnd *	lCharacterWnd;
	CAgentPopupWnd *		lPopupWnd;
	CRect					lRect;

	if	(lPopupWnd = GetPopupWnd ())
	{
		lPopupWnd->GetWindowRect (&lRect);
		if	(!lPopupWnd->SizePopup (CSize (Width, lRect.Height()), 0))
		{
			lResult = S_FALSE;
		}
	}
	else
	if	(lCharacterWnd = GetCharacterWnd ())
	{
		lRect = lCharacterWnd->GetVideoRect();
		lRect.InflateRect (0, 0, Width - lRect.Width(), 0);
		lResult = lCharacterWnd->SetVideoRect (lRect);
	}
	else
	{
		lResult = AGENTERR_CLIENTINVALID;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::get_Height (short *Height)
{
	HRESULT					lResult = S_OK;
	CAgentCharacterWnd *	lCharacterWnd;
	CAgentPopupWnd *		lPopupWnd;
	CRect					lRect (0,0,0,0);

	if	(!Height)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(lPopupWnd = GetPopupWnd ())
		{
			lPopupWnd->GetWindowRect (&lRect);
		}
		else
		if	(lCharacterWnd = GetCharacterWnd ())
		{
			lRect = lCharacterWnd->GetVideoRect ();
		}
		else
		{
			lResult = AGENTERR_CLIENTINVALID;
		}
		(*Height) = (short)lRect.Height();
	}
	return lResult;
}

HRESULT CDaCmnCharacter::put_Height (short Height)
{
	HRESULT					lResult = S_OK;
	CAgentCharacterWnd *	lCharacterWnd;
	CAgentPopupWnd *		lPopupWnd;
	CRect					lRect;

	if	(lPopupWnd = GetPopupWnd ())
	{
		lPopupWnd->GetWindowRect (&lRect);
		if	(!lPopupWnd->SizePopup (CSize (lRect.Width(), Height), 0))
		{
			lResult = S_FALSE;
		}
	}
	else
	if	(lCharacterWnd = GetCharacterWnd ())
	{
		lRect = lCharacterWnd->GetVideoRect();
		lRect.InflateRect (0, 0, 0, Height - lRect.Height());
		lResult = lCharacterWnd->SetVideoRect (lRect);
	}
	else
	{
		lResult = AGENTERR_CLIENTINVALID;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::get_OriginalWidth (short *OriginalWidth)
{
	HRESULT	lResult = S_OK;

	if	(!OriginalWidth)
	{
		lResult = E_POINTER;
	}
	else
	if	(!mFile)
	{
		(*OriginalWidth) = 0;
		lResult = AGENTERR_CLIENTINVALID;
	}
	else
	{
		(*OriginalWidth) = (short)mFile->GetImageSize ().cx;
	}
	return lResult;
}

HRESULT CDaCmnCharacter::get_OriginalHeight (short *OriginalHeight)
{
	HRESULT	lResult = S_OK;

	if	(!OriginalHeight)
	{
		lResult = E_POINTER;
	}
	else
	if	(!mFile)
	{
		(*OriginalHeight) = 0;
		lResult = AGENTERR_CLIENTINVALID;
	}
	else
	{
		(*OriginalHeight) = (short)mFile->GetImageSize ().cy;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::get_Visible (VARIANT_BOOL *Visible)
{
	HRESULT	lResult = S_OK;

	if	(!Visible)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Visible) = VARIANT_FALSE;

		if	(GetCharacterWnd ())
		{
			(*Visible) = IsVisible (true) ? VARIANT_TRUE : VARIANT_FALSE;
		}
		else
		{
			lResult = AGENTERR_CLIENTINVALID;
		}
	}
	return lResult;
}

HRESULT CDaCmnCharacter::get_ActiveState (ActiveStateType *ActiveState)
{
	HRESULT	lResult = S_OK;
	short	lActiveState = ActiveState_Inactive;

	if	(!mFile)
	{
		lResult = AGENTERR_CLIENTINVALID;
	}
	else
	{
		if	(IsClientActive ())
		{
			lActiveState = GetActiveState ();
		}
		else
		{
			lResult = S_FALSE;
		}
	}
	if	(ActiveState)
	{
		(*ActiveState) = (ActiveStateType)lActiveState;
	}
	return lResult;
}

HRESULT CDaCmnCharacter::put_ActiveState (ActiveStateType ActiveState)
{
#ifdef	_DEBUG_ACTIVE
	if	(LogIsActive (_DEBUG_ACTIVE))
	{
		LogMessage (_DEBUG_ACTIVE, _T("[%d] put_ActiveState [%hd]"), mCharID, ActiveState);
	}
#endif
	HRESULT	lResult = SetActiveState ((short)ActiveState);

#ifdef	_STRICT_COMPATIBILITY
	if	(lResult == AGENTERR_CLIENTINVALID)
	{
		lResult = E_INVALIDARG;
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::get_IdleState (VARIANT_BOOL *IdleState)
{
	HRESULT					lResult;
	CAgentCharacterWnd *	lCharacterWnd;

	if	(lCharacterWnd = GetCharacterWnd ())
	{
		lResult = (lCharacterWnd->IsIdle() > 0) ? S_OK : S_FALSE;
	}
	else
	{
		lResult = AGENTERR_CLIENTINVALID;
	}
	if	(IdleState)
	{
		(*IdleState) = (lResult == S_OK) ? VARIANT_TRUE : VARIANT_FALSE;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::get_OtherClientCount (long *OtherClientCount)
{
	HRESULT	lResult = S_OK;
	long	lClientCount = 0;

	if	(!mFile)
	{
		lResult = AGENTERR_CLIENTINVALID;
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
	return lResult;
}

HRESULT CDaCmnCharacter::get_MoveCause (MoveCauseType *MoveCause)
{
	HRESULT	lResult = S_OK;

	if	(!MoveCause)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*MoveCause) = MoveCause_NeverMoved;

		if	(GetCharacterWnd ())
		{
			(*MoveCause) = mNotify->_GetMoveCause (mCharID);
		}
		else
		{
			lResult = AGENTERR_CLIENTINVALID;
		}
	}
	return lResult;
}

HRESULT CDaCmnCharacter::get_VisibilityCause (VisibilityCauseType *VisibilityCause)
{
	HRESULT	lResult = S_OK;

	if	(!VisibilityCause)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*VisibilityCause) = VisibilityCause_NeverShown;

		if	(GetCharacterWnd ())
		{
			(*VisibilityCause) = mNotify->_GetVisibilityCause (mCharID);
		}
		else
		{
			lResult = AGENTERR_CLIENTINVALID;
		}
	}
	return lResult;
}

HRESULT CDaCmnCharacter::get_ListeningStatus (ListeningStatusType *ListeningStatus)
{
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
		if	(mNotify->mAnchor->mAnchor.GetActiveCharacter() != GetCharID())
		{
			(*ListeningStatus) = ListeningStatus_CharacterInactive;
		}
		else
		if	(!GetSapiInput (true))
		{
			(*ListeningStatus) = ListeningStatus_InitializeFailed;
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::get_HasIcon (VARIANT_BOOL *HasIcon)
{
	HRESULT	lResult = S_OK;

	if	(HasIcon)
	{
		(*HasIcon) = VARIANT_FALSE;

		if	(
				(mFile)
			&&	(mFile->GetIcon ())
			)
		{
			(*HasIcon) = VARIANT_TRUE;
		}
	}
	else
	{
		lResult = E_POINTER;
	}
	return lResult;
}

HRESULT CDaCmnCharacter::get_Style (long *Style)
{
	HRESULT	lResult = S_OK;

	if	(Style)
	{
		(*Style) = GetStyle ();
	}
	else
	{
		lResult = E_POINTER;
	}
	return lResult;
}

HRESULT CDaCmnCharacter::put_Style (long Style)
{
	HRESULT	lResult = S_OK;

	lResult = SetStyle (~Style, Style);
	return lResult;
}

HRESULT CDaCmnCharacter::get_IconVisible (VARIANT_BOOL *IconVisible)
{
	HRESULT	lResult = S_OK;

	if	(IconVisible)
	{
		(*IconVisible) = IsIconVisible()?VARIANT_TRUE:VARIANT_FALSE;
	}
	else
	{
		lResult = E_POINTER;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::GenerateIcon (long ClipLeft, long ClipTop, long ClipWidth, long ClipHeight)
{
	HRESULT	lResult = S_OK;

	if	(!mIconData.mGenerateIcon)
	{
		bool	lIconShown = IsIconShown();

		mIconData.mGenerateIcon = true;
		mIconData.mGenerateIconClip = CRect (CPoint (ClipLeft, ClipTop), CSize (ClipWidth, ClipHeight));
		ShowIcon (false);
		lResult = ShowIcon (lIconShown);
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::GetIconIdentity (GUID *IconIdentity)
{
	HRESULT	lResult = S_OK;

	if	(!IconIdentity)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*IconIdentity) = mIconData.mIdentity;
	}
	return lResult;
}

HRESULT CDaCmnCharacter::SetIconIdentity (const GUID *IconIdentity)
{
	HRESULT	lResult;

	if	(IconIdentity)
	{
		mIconData.mIdentity = (*IconIdentity);
	}
	else
	{
		mIconData.mIdentity = GUID_NULL;
	}
	lResult = ShowIcon (IsIconShown());
	return lResult;
}

HRESULT CDaCmnCharacter::get_IconIdentity (BSTR *IconIdentity)
{
	HRESULT	lResult = S_OK;

	if	(!IconIdentity)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*IconIdentity) = ((CString)CGuidStr(mIconData.mIdentity)).AllocSysString ();
	}
	return lResult;
}

HRESULT CDaCmnCharacter::put_IconIdentity (BSTR IconIdentity)
{
	HRESULT	lResult;

	lResult = SetIconIdentity (&CGuidStr::Parse (CAtlString (IconIdentity)));
	return lResult;
}

HRESULT CDaCmnCharacter::get_IconTip (BSTR *IconTip)
{
	HRESULT	lResult = S_OK;

	if	(!IconTip)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*IconTip) = mIconData.mTip.AllocSysString ();
	}
	return lResult;
}

HRESULT CDaCmnCharacter::put_IconTip (BSTR IconTip)
{
	HRESULT	lResult;

	mIconData.mTip = IconTip;
	lResult = ShowIcon (IsIconShown());
	return lResult;
}
