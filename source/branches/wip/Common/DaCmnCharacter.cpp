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
	mNotify (NULL),
	mWnd (NULL),
	mSapiVoice (NULL),
	mSapiInput (NULL),
	mIdleEnabled (true),
	mSoundEnabled (true),
	mAutoPopupMenu (true),
	mInNotify (0)
{
}

CDaCmnCharacter::~CDaCmnCharacter()
{
}

/////////////////////////////////////////////////////////////////////////////

void CDaCmnCharacter::Initialize (long pCharID, CEventNotify * pNotify, _IListeningAnchor * pListeningAnchor)
{
	mCharID = pCharID;
	mNotify = pNotify;
	mNotify->RegisterEventReflect (this, true);
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

void CDaCmnCharacter::Terminate ()
{
	if	(this)
	{
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
			&&	(mNotify)
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
					mNotify->mAnchor->mAnchor.RemoveFileClient (mFile, mWnd->GetBalloonWnd());
				}
			}
			catch AnyExceptionDebug
			try
			{
				if	(mWnd)
				{
					mNotify->mAnchor->mAnchor.RemoveFileClient (mFile, mWnd);
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

		SafeFreeSafePtr (mWndRefHolder);

		if	(
				(mFile)
			&&	(mNotify)
			)
		{
			try
			{
				mNotify->RegisterEventReflect (this, false);
			}
			catch AnyExceptionDebug
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

		mFile = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CDaCmnCharacter::PreNotify ()
{
	if	(
			(this)
		&&	(_PreNotify ())
		)
	{
		mInNotify++;
		return true;
	}
	return false;
}

bool CDaCmnCharacter::_PreNotify ()
{
	return true;
}

bool CDaCmnCharacter::PostNotify ()
{
	if	(this)
	{
		if	(mInNotify > 0)
		{
			mInNotify--;
		}
		return _PostNotify ();
	}
	return false;
}

bool CDaCmnCharacter::_PostNotify ()
{
	return true;
}

UINT CDaCmnCharacter::IsInNotify () const
{
	if	(this)
	{
		return mInNotify;
	}
	return 0;
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

HRESULT CDaCmnCharacter::OpenFile (CAgentFile * pFile, DWORD pInitialStyle)
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

	if	(
			(SUCCEEDED (lResult))
		&&	(mNotify->mAnchor->mAnchor.GetFileClients (mFile, lFileClients))
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
		mNotify->mAnchor->mAnchor.AddFileClient (mFile, mWnd);
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

HRESULT CDaCmnCharacter::GetLoadPath (VARIANT pLoadKey, CString & pFilePath)
{
	HRESULT	lResult = S_OK;

	pFilePath.Empty ();

	if	(!IsEmptyParm (&pLoadKey))
	{
		try
		{
			pFilePath = (BSTR)(_bstr_t)_variant_t(pLoadKey);
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
		UINT	lPathNum;
		CString	lFilePath;

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

HRESULT CDaCmnCharacter::GetAgentFile (VARIANT pLoadKey, tPtr <CAgentFile> & pAgentFile)
{
	HRESULT	lResult;
	CString	lFilePath;

	if	(SUCCEEDED (lResult = GetLoadPath (pLoadKey, lFilePath)))
	{
		lResult =  GetAgentFile (lFilePath, pAgentFile);
	}
	return lResult;
}

HRESULT CDaCmnCharacter::GetAgentFile (LPCTSTR pFilePath, tPtr <CAgentFile> & pAgentFile)
{
	HRESULT	lResult;
	CString	lFilePath (pFilePath);

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

bool CDaCmnCharacter::IsShowing () const
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

bool CDaCmnCharacter::IsHiding () const
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

long CDaCmnCharacter::Show (bool pFast, bool pImmediate)
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

long CDaCmnCharacter::Hide (bool pFast, bool pImmediate)
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

bool CDaCmnCharacter::IsInputActive () const
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

bool CDaCmnCharacter::IsClientActive () const
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

long CDaCmnCharacter::GetActiveClient () const
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

bool CDaCmnCharacter::SetClientActive (bool pActive, bool pInputActive)
{
	bool			lRet = false;
	long			lPrevCharId = 0;
	CDaCmnCharacter *	lNextCharacter = NULL;
	CDaCmnBalloon *	lBalloon = NULL;

#ifdef	_DEBUG_ACTIVE
	if	(LogIsActive (_DEBUG_ACTIVE))
	{
		LogMessage (_DEBUG_ACTIVE, _T("[%d] SetClientActive [%u] InputActive [%u] - IsVisible [%u] IsClientActive [%u] IsInputActive [%u] IsListening [%u] - ClientActive [%d] InputActive [%d] Listen [%d]"), mCharID, pActive, pInputActive, IsVisible(), IsClientActive(), IsInputActive(), IsListening(), GetActiveClient(), mNotify->mAnchor->mAnchor.GetActiveCharacter(), mNotify->mAnchor->mAnchor.GetListenCharacter());
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
			if	(
					(GetBalloonWnd (false))
				&&	(lBalloon = GetBalloon (true))
				)
			{
				lBalloon->SetBalloonWndOptions (mLangID);
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
				if	(
						(lNextCharacter->GetBalloonWnd (false))
					&&	(lBalloon = lNextCharacter->GetBalloon (true))
					)
				{
					lBalloon->SetBalloonWndOptions (lNextCharacter->mLangID);
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
		LogMessage (_DEBUG_ACTIVE, _T("[%d] %u  ClientActive [%u] InputActive [%u] - IsVisible [%u] IsClientActive [%u] IsInputActive [%u] IsListening [%u] - ClientActive [%d] InputActive [%d] Listen [%d]"), mCharID, lRet, pActive, pInputActive, IsVisible(), IsClientActive(), IsInputActive(), IsListening(), mNotify->GetActiveClient(mCharID), mNotify->mAnchor->mAnchor.GetActiveCharacter(), mNotify->mAnchor->mAnchor.GetListenCharacter());
	}
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

INT_PTR CDaCmnCharacter::GetClientCount (int pSkipCharID) const
{
	INT_PTR	lRet = -1;

	try
	{
		CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
		INT_PTR								lClientNdx;
		CDaCmnCharacter *						lCharacter;

		if	(
				(mFile)
			&&	(mNotify->mAnchor->mAnchor.GetFileClients (mFile, lFileClients))
			)
		{
			lRet = 0;

			for	(lClientNdx = 0; lClientNdx < (INT_PTR)lFileClients.GetCount(); lClientNdx++)
			{
				if	(
						(lCharacter = dynamic_cast <CDaCmnCharacter *> (lFileClients [lClientNdx]))
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

HRESULT CDaCmnCharacter::SetStyle (DWORD pRemoveStyle, DWORD pAddStyle)
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
			CDaCmnCommands *			lCommands = NULL;
			CAgentBalloonWnd *		lBalloonWnd;
			CVoiceCommandsWnd *		lVoiceCommandsWnd;
			CAgentListeningWnd *	lListeningWnd;

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
					(mWnd)
				&&	(mWnd->GetCharID() == mCharID)
				)
			{
				mWnd->SetNotifyIconTip (&mIconData, mFile, mLangID);
			}

			if	(lBalloon = GetBalloon (false))
			{
				lBalloon->SetLangID (mLangID);
			}

			if	(
					(lBalloonWnd = GetBalloonWnd (false))
				&&	(lBalloonWnd->GetCharID() == mCharID)
				&&	(lBalloonWnd->GetLangID() != mLangID)
				&&	(lBalloon = GetBalloon (true))
				)
			{
				lBalloon->SetBalloonWndOptions (mLangID);
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
	if	(
			(mWnd)
		&&	(mWnd->IsWindow ())
		)
	{
		return mWnd->IsIdleEnabled ();
	}
	return mIdleEnabled;
}

bool CDaCmnCharacter::IsSoundEnabled (bool pIgnoreGlobalConfig) const
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

bool CDaCmnCharacter::ShowIcon (bool pShow)
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

void CDaCmnCharacter::ReleaseSapiVoice ()
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

bool CDaCmnCharacter::ShowHearingState (bool pShow)
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

void CDaCmnCharacter::ReleaseSapiInput ()
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

HRESULT CDaCmnCharacter::StopAll (long pStopTypes, HRESULT pReqStatus)
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

HRESULT CDaCmnCharacter::DoPrepare (long pType, LPCTSTR pName, bool pQueue, long & pReqID)
{
	HRESULT					lResult = E_FAIL;
	tPtr <CQueuedPrepare>	lPrepare;

#ifdef	_DEBUG_PREPARE
	if	(LogIsActive (_DEBUG_PREPARE))
	{
		LogMessage (_DEBUG_PREPARE, _T("[%p] [%d] CDaCmnCharacter Prepare [%d %u] [%s]"), this, mCharID, pType, pQueue, pName);
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

void CDaCmnCharacter::_OnCharacterNameChanged (long pCharID)
{
#ifdef	_DEBUG_NOTIFY_PATH
	LogMessage (_DEBUG_NOTIFY_PATH, _T("CDaCmnCharacter::_OnCharacterNameChanged [%d]"), pCharID);
#endif	
	if	(
			(mWnd)
		&&	(mWnd->IsWindow ())
		&&	(mWnd->GetCharID() == GetCharID())
		)
	{
		mWnd->SetNotifyIconTip (&mIconData, mFile, mLangID);
	}
}

void CDaCmnCharacter::_OnCharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID)
{
	CVoiceCommandsWnd *	lVoiceCommandsWnd;

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
	CDaCmnCommands *		lCommands;
	CDaCmnCommand *		lCommand;
	USHORT				lCommandId;
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
	bool			lRet = false;
	CDaCmnCommands *	lCommands;

	if	(lCommands = GetCommands (true))
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
				lVoiceCommandsWnd->Show ();
			}
			lRet = true;
		}
	}
	return lRet;
}

bool CDaCmnCharacter::NotifyVoiceCommand (USHORT pCommandId, interface ISpRecoResult * pRecoResult, bool pGlobalCommand)
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CAgentPopupWnd * CDaCmnCharacter::GetAgentWnd ()
{
	return mWnd;
}

CAgentBalloonWnd * CDaCmnCharacter::GetBalloonWnd (bool pCreateObject)
{
	CAgentBalloonWnd *	lBalloonWnd = NULL;
	CDaCmnBalloon *		lBalloon = NULL;

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
			&&	(lBalloon = GetBalloon (true))
			)
		{
			if	(
					(lBalloon->get_Enabled (NULL) == S_OK)
				&&	(lBalloonWnd = mWnd->GetBalloonWnd (true))
				&&	(lBalloonWnd->IsWindow ())
				)
			{
				lBalloon->SetBalloonWndOptions (mLangID);
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

CAgentListeningWnd * CDaCmnCharacter::GetListeningWnd (bool pCreateObject)
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

LPVOID CDaCmnCharacter::FindOtherRequest (long pReqID, CDaCmnCharacter *& pOtherCharacter)
{
	LPVOID			lRet = NULL;
	CAgentFile *	lFile;
	INT_PTR			lFileNdx;

	pOtherCharacter = NULL;

	for	(lFileNdx = 0; lFile = mNotify->mAnchor->GetCachedFile (lFileNdx); lFileNdx++)
	{
		CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
		INT_PTR								lClientNdx;
		CDaCmnCharacter *					lCharacter;

		if	(mNotify->mAnchor->GetFileClients (lFile, lFileClients))
		{
			for	(lClientNdx = lFileClients.GetCount()-1; lClientNdx >= 0; lClientNdx--)
			{
				if	(
						(lCharacter = dynamic_cast <CDaCmnCharacter *> (lFileClients [lClientNdx]))
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

void CDaCmnCharacter::_OnOptionsChanged ()
{
	CDaSettingsConfig		lSettingsConfig;
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
		&&	(lBalloon = GetBalloon (true))
		)
	{
		if	(lBalloon->get_Enabled (NULL) == S_FALSE)
		{
			lBalloonWnd->HideBalloon (true);
			mWnd->RemoveQueuedThink (mCharID, AGENTREQERR_INTERRUPTEDCODE, _T("OptionsChanged"));
		}
		lBalloon->SetBalloonWndOptions (mLangID);
	}
}

void CDaCmnCharacter::_OnDefaultCharacterChanged ()
{
	StopListening (false, ListenComplete_DefaultCharacterChanged);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::SetPosition (long Left, long Top)
{
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
#ifdef	_STRICT_COMPATIBILITY
		if	(!mWnd->MovePopup (CPoint (Left, Top), 0, MoveCause_ProgramMoved, true))
#else
		if	(!mWnd->MovePopup (CPoint (Left, Top), 0, MoveCause_ProgramMoved))
#endif
		{
			lResult = S_FALSE;
		}
	}
	return lResult;
}

HRESULT CDaCmnCharacter::GetPosition (long *Left, long *Top)
{
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
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::SetSize (long Width, long Height)
{
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
		if	(!mWnd->SizePopup (CSize (Width, Height), 0))
		{
#ifndef	_STRICT_COMPATIBILITY
			lResult = S_FALSE;
#endif
		}
	}
	return lResult;
}

HRESULT CDaCmnCharacter::GetSize (long *Width, long *Height)
{
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
	return lResult;
}

HRESULT CDaCmnCharacter::GetOriginalSize (long *Width, long *Height)
{
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
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::GetVersion(short *MajorVersion, short *MinorVersion)
{
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
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::Show (long Fast, long *RequestID)
{
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
	}

	if	(RequestID)
	{
		(*RequestID) = lReqID;
	}
	return lResult;
}

HRESULT CDaCmnCharacter::Play (BSTR Animation, long *RequestID)
{
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
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::Stop (long RequestID)
{
	HRESULT				lResult = S_OK;
	LPVOID				lRequest = NULL;
	CDaCmnCharacter *	lOtherCharacter = NULL;

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
	return lResult;
}

HRESULT CDaCmnCharacter::StopAll (long Types)
{
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
		lResult = StopAll (Types, AGENTREQERR_INTERRUPTEDCODE);
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::MoveTo (short x, short y, long Speed, long *RequestID)
{
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
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::GestureAt (short x, short y, long *RequestID)
{
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
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::Think (BSTR Text, long *RequestID)
{
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
		CDaCmnBalloon *	lBalloon = GetBalloon (true);

		if	(lBalloon->get_Enabled (NULL) == S_OK)
		{
			GetBalloonWnd (true);
			lReqID = mWnd->QueueThink (mCharID, CString (Text));
			mWnd->ActivateQueue (true);
		}
		else
		{
			lResult = AGENTERR_NOBALLOON;
		}
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
	HRESULT				lResult = S_OK;
	LPVOID				lOtherRequest = NULL;
	CDaCmnCharacter *	lOtherCharacter = NULL;
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
	return lResult;
}

HRESULT CDaCmnCharacter::Interrupt (long InterruptRequestID, long *RequestID)
{
	HRESULT				lResult = S_OK;
	LPVOID				lOtherRequest = NULL;
	CDaCmnCharacter *	lOtherCharacter = NULL;
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
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::ShowPopupMenu (short x, short y)
{
	HRESULT	lResult = S_OK;

	if	(
			(!mWnd)
		||	(!mWnd->IsWindow ())
		)
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}
	else
	if	(mNotify->mAnchor->mAnchor.GetActiveCharacter() != GetCharID())
	{
		lResult = AGENTERR_CHARACTERNOTACTIVE;
	}
	else
	{
		if	(!DoContextMenu (mWnd->m_hWnd, CPoint (x, y)))
		{
			lResult = S_FALSE;
		}
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
	return lResult;
}

HRESULT CDaCmnCharacter::Speak (BSTR Text, BSTR Url, long *RequestID)
{
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
		CDaCmnBalloon *		lBalloon = GetBalloon (true);
		bool				lShowBalloon = (lBalloon->get_Enabled (NULL) == S_OK);
		CString				lText (Text);
		CString				lSoundUrl (Url);
		CSapiVoice *		lVoice = NULL;
//
//	MS Agent shows the speech balloon silently when the character is listening.
//	For now, we'll just stop listening.
//
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
			mWnd->ActivateQueue (true);
		}
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
		lResult = AGENTERR_CHARACTERINVALID;
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
	return lResult;
}

HRESULT CDaCmnCharacter::put_Name (BSTR Name)
{
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
		if	(lFileName = mFile->FindName (mLangID))
		{
			if	(CString ((BSTR)lFileName->mName) != lName)
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
	return lResult;
}

HRESULT CDaCmnCharacter::put_Description (BSTR Description)
{
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
			lResult = AGENTERR_CHARACTERINVALID;
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
			lResult = AGENTERR_CHARACTERINVALID;
		}
		else
		{
			(*FilePath) = mFile->GetPath().Detach();
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
		lResult = AGENTERR_CHARACTERINVALID;
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
		lResult = AGENTERR_CHARACTERINVALID;
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
		(*TTSModeID) = CString().AllocSysString();
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
			CSapiVoice *	lPrevVoice = mSapiVoice;

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
		(*SRModeID) = mSapiInput->GetEngineName().Detach();
	}
	else
	{
		(*SRModeID) = CString().AllocSysString();
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
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::get_Left (short *Left)
{
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
	return lResult;
}

HRESULT CDaCmnCharacter::put_Left (short Left)
{
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
		if	(!mWnd->MovePopup (CPoint (Left, lWindowRect.top), 0, MoveCause_ProgramMoved))
		{
			lResult = S_FALSE;
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::get_Top (short *Top)
{
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
	return lResult;
}

HRESULT CDaCmnCharacter::put_Top (short Top)
{
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
		if	(!mWnd->MovePopup (CPoint (lWindowRect.left, Top), 0, MoveCause_ProgramMoved))
		{
			lResult = S_FALSE;
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::get_Width (short *Width)
{
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
	return lResult;
}

HRESULT CDaCmnCharacter::put_Width (short Width)
{
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
		if	(!mWnd->SizePopup (CSize (Width, lWindowRect.Height()), 0))
		{
			lResult = S_FALSE;
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::get_Height (short *Height)
{
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
	return lResult;
}

HRESULT CDaCmnCharacter::put_Height (short Height)
{
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
		if	(!mWnd->SizePopup (CSize (lWindowRect.Width(), Height), 0))
		{
			lResult = S_FALSE;
		}
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
		lResult = AGENTERR_CHARACTERINVALID;
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
		lResult = AGENTERR_CHARACTERINVALID;
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
	return lResult;
}

HRESULT CDaCmnCharacter::get_ActiveState (ActiveStateType *ActiveState)
{
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
	return lResult;
}

HRESULT CDaCmnCharacter::put_ActiveState (ActiveStateType ActiveState)
{
	HRESULT	lResult = S_OK;
#ifdef	_DEBUG_ACTIVE
	if	(LogIsActive (_DEBUG_ACTIVE))
	{
		LogMessage (_DEBUG_ACTIVE, _T("[%d] put_ActiveState [%hd]"), mCharID, ActiveState);
	}
#endif

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
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacter::get_IdleState (VARIANT_BOOL *IdleState)
{
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
		lResult = (mWnd->IsIdle() > 0) ? S_OK : S_FALSE;
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
#if FALSE
		if	(
				(mWnd)
			&&	(mWnd->IsWindow ())
			)
		{
			(*HasIcon) = mWnd->IsNotifyIconValid()?VARIANT_TRUE:VARIANT_FALSE;
		}
#else
		if	(
				(mFile)
			&&	(mFile->GetIcon ())
			)
		{
			(*HasIcon) = VARIANT_TRUE;
		}
#endif
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

	lResult = SetIconIdentity (&CGuidStr::Parse (CString (IconIdentity)));
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
