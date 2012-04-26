/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "DaCore.h"
#include "EventNotify.h"
#include "AgentCharacterWnd.h"
#include "AgentBalloonWnd.h"
#include "AgentListeningWnd.h"
#include "AgentStreamInfo.h"
#include "AgentTextParse.h"
#include "DirectShowSource.h"
#include "DirectShowRender.h"
#include "DirectSoundLipSync.h"
#include "DaGlobalConfig.h"
#include "QueuedPrepare.h"
#include "FileDownload.h"
#include "VfwErr.h"
#include "StringArrayEx.h"
#include "Elapsed.h"
#include "MallocPtr.h"
#include "Sapi5Voice.h"
#include "Sapi5Err.h"
#include "ThreadSecurity.h"
#include "ImageTools.h"
#ifndef	_WIN64
#include "Sapi4Voice.h"
#include "Sapi4Err.h"
#endif
#include "DebugStr.h"
#ifdef	_DEBUG
#include "ImageDebugger.h"
#include "DebugWin.h"
#include "DebugProcess.h"
#endif
#ifdef	_DEBUG_NOT
#include "DebugTime.h"
#endif

#ifdef	_DEBUG
//#define	_DEBUG_MOUSE		LogNormal
#define	_DEBUG_ACTIVE			(GetProfileDebugInt(_T("DebugActive"),LogDetails,true)&0xFFFF|LogTime)
#define	_DEBUG_SPEECH			(GetProfileDebugInt(_T("DebugSpeech"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
#define	_DEBUG_SPEECH_MOUTH		(GetProfileDebugInt(_T("DebugSpeechMouth"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
#define	_DEBUG_SPEECH_EVENTS	(GetProfileDebugInt(_T("DebugSpeechEvents"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
#define	_DEBUG_BALLOON_OPTIONS	(GetProfileDebugInt(_T("DebugBalloonOptions"),LogVerbose,true)&0xFFFF|LogTime)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Popup"),LogDetails,true)&0xFFFF|LogTime)
#define	_LOG_QUEUE_OPS			(GetProfileDebugInt(_T("LogQueueOps"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
#define	_LOG_POPUP_OPS			MinLogLevel(GetProfileDebugInt(_T("LogPopupOps"),LogVerbose,true)&0xFFFF|LogTimeMs,_LOG_QUEUE_OPS)
#endif

#ifndef	_LOG_INSTANCE
#define	_LOG_INSTANCE		LogDetails
#endif
#ifndef	_LOG_QUEUE_OPS
#define	_LOG_QUEUE_OPS		LogDetails
#endif
#ifndef	_LOG_POPUP_OPS
#define	_LOG_POPUP_OPS		LogDetails
#endif

/////////////////////////////////////////////////////////////////////////////

HWND		CAgentCharacterWnd::mLastActive = NULL;
const UINT	CAgentCharacterWnd::mVoiceStartMsg = RegisterWindowMessage (_T("A444DB92-39D0-4677-8D6D-1C4032BC9DED"));
const UINT	CAgentCharacterWnd::mVoiceEndMsg = RegisterWindowMessage (_T("AD44294A-BC10-43e5-94A7-C9C392863A79"));
const UINT	CAgentCharacterWnd::mVoiceBookMarkMsg = RegisterWindowMessage (_T("8FC08C6D-E6EB-4d53-B115-8378AB001571"));
const UINT	CAgentCharacterWnd::mVoiceVisualMsg = RegisterWindowMessage (_T("242D8583-6BAC-44d5-8CF8-F6DD020F701C"));

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DLL_OBJECT(CAgentCharacterWnd)

CAgentCharacterWnd::CAgentCharacterWnd ()
:	mCharID (0),
	mCharShown (false),
	mLastButtonMsg (0),
	mBalloonWnd (NULL)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CAgentCharacterWnd::CAgentCharacterWnd"), this);
	}
#endif
	mAutoSize = false;
}

CAgentCharacterWnd::~CAgentCharacterWnd ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CAgentCharacterWnd::~CAgentCharacterWnd [%p] [%d]"), this, m_hWnd, ::IsWindow(m_hWnd));
	}
#endif
	ClearNotifySources ();
	Detach (-1, NULL);
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p] CAgentCharacterWnd::~CAgentCharacterWnd Done"), this);
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

long CAgentCharacterWnd::GetCharID () const
{
	return mCharID;
}

bool CAgentCharacterWnd::IsVisible () const
{
	if	(
			(IsWindow ())
		&&	(GetStyle () & WS_VISIBLE)
		)
	{
		return true;
	}
	return false;
}

bool CAgentCharacterWnd::IsCharShown () const
{
	if	(
			(IsWindow ())
		&&	(GetStyle () & WS_VISIBLE)
		&&	(GetCharID () > 0)
		&&	(mCharShown)
		)
	{
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentCharacterWnd::Attach (long pCharID, CEventNotify* pNotify, const CAgentIconData* pIconData, bool pSetActiveCharID)
{
	bool	lRet = false;
	long	lPrevCharID = mCharID;

	if	(
			(pNotify)
		&&	(mNotify.AddUnique (pNotify) >= 0)
		)
	{
		pNotify->LockSinks::AddNotifySink (this);
		lRet = true;
	}
	if	(
			(mBalloonWnd)
		&&	(mBalloonWnd->Attach (pCharID, pNotify, pSetActiveCharID))
		)
	{
		lRet = true;
	}
	if	(
			(pSetActiveCharID)
		&&	(mListeningWnd)
		&&	(mListeningWnd->Attach (pCharID, NULL))
		)
	{
		lRet = true;
	}

	if	(
			(pSetActiveCharID)
		&&	(mCharID != pCharID)
		)
	{
		mCharID = pCharID;
		lRet = true;
	}
	return lRet;
}

bool CAgentCharacterWnd::Detach (long pCharID, CEventNotify* pNotify)
{
	bool	lRet = false;

	try
	{
		if	(pCharID < 0)
		{
			mBalloonWnd = NULL;
			SafeFreeSafePtr (mBalloonRefHolder);
			SafeFreeSafePtr (mListeningWnd);
		}
		ClearQueuedActions (pCharID, 0, _T("Detach"));

		if	(pCharID == mCharID)
		{
			mCharID = 0;
		}

		if	(
				(mBalloonWnd)
			&&	(mBalloonWnd->Detach (pCharID, pNotify))
			)
		{
			lRet = true;
		}
		if	(
				(mListeningWnd)
			&&	(mListeningWnd->Detach (pCharID))
			)
		{
			lRet = true;
		}
		if	(
				(pNotify)
			&&	(mNotify.Remove (pNotify) >= 0)
			)
		{
			pNotify->LockSinks::RemoveNotifySink (this);
			lRet = true;
		}

		if	(
				(pCharID < 0)
			&&	(!pNotify)
			)
		{
			INT_PTR			lNotifyNdx;
			CEventNotify*	lNotify;

			for	(lNotifyNdx = (INT_PTR)mNotify.GetCount()-1; lNotify = mNotify (lNotifyNdx); lNotifyNdx--)
			{
				if	(Detach (-1, lNotify))
				{
					lRet = true;
				}
			}
			mNotify.RemoveAll ();
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentCharacterWnd::SetLastActive ()
{
	CAgentCharacterWnd*	lLastActive = NULL;
	long					lLastActiveCharID = -1;
	INT_PTR					lNotifyNdx;
	CEventNotify*			lNotify;

	if	(mLastActive != m_hWnd)
	{
		CAgentWnd*	lAgentWnd;

		for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
		{
			if	(
					(mLastActive)
				&&	(lAgentWnd = lNotify->GetAgentWnd (mLastActive))
				)
			{
				if	(lLastActive = dynamic_cast <CAgentCharacterWnd*> (lAgentWnd))
				{
					lLastActiveCharID = lLastActive->mCharID;
				}
				break;
			}
		}
	}

	if	(
			(IsWindow ())
		&&	(mLastActive != m_hWnd)
		)
	{
		mLastActive = m_hWnd;

		if	(lLastActive)
		{
			lLastActive->IsLastActive (false);
		}
		IsLastActive (true);

		if	(lLastActive)
		{
			if	(lLastActive->PreNotify ())
			{
				try
				{
#ifdef	_DEBUG_ACTIVE
					if	(LogIsActive (_DEBUG_ACTIVE))
					{
						LogMessage (_DEBUG_ACTIVE, _T("[%p] SetNotInputActive [%d] (Activate)"), this, lLastActiveCharID);
					}
#endif
					for	(lNotifyNdx = 0; lNotify = lLastActive->mNotify (lNotifyNdx); lNotifyNdx++)
					{
						lNotify->ActiveCharacterNotify (-1, -1, lLastActiveCharID, lLastActiveCharID);
					}
				}
				catch AnyExceptionDebug
			}
			lLastActive->PostNotify ();
		}

		if	(PreNotify ())
		{
			try
			{
#ifdef	_DEBUG_ACTIVE
				if	(LogIsActive (_DEBUG_ACTIVE))
				{
					LogMessage (_DEBUG_ACTIVE, _T("[%p] SetInputActive [%d] (Activate)"), this, mCharID);
				}
#endif
				for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
				{
					lNotify->ActiveCharacterNotify (mCharID, mCharID, -1, -1);
				}
				for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
				{
					if	(lNotify->ActiveCharacterChanged (mCharID, mCharID, lLastActiveCharID, lLastActiveCharID))
					{
						break;
					}
				}
			}
			catch AnyExceptionDebug
		}
		PostNotify ();

		return true;
	}
	return false;
}

HWND CAgentCharacterWnd::GetLastActive ()
{
	return mLastActive;
}

void CAgentCharacterWnd::IsLastActive (bool pLastActive)
{
	MakeActiveMedia (pLastActive);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CAgentBalloonWnd* CAgentCharacterWnd::GetBalloonWnd (bool pCreate)
{
	if	(
			(!mBalloonWnd)
		&&	(pCreate)
		&&	(mBalloonWnd = CAgentBalloonWnd::CreateInstance (mCharID, mNotify))
		)
	{
		mBalloonRefHolder = mBalloonWnd->GetControllingUnknown ();
	}

	if	(
			(mBalloonWnd)
		&&	(!mBalloonWnd->IsWindow ())
		&&	(pCreate)
		)
	{
		mBalloonWnd->Create (this);
	}
	return mBalloonWnd;
}

CAgentListeningWnd* CAgentCharacterWnd::GetListeningWnd (bool pCreate)
{
	if	(
			(!mListeningWnd)
		&&	(pCreate)
		&&	(mListeningWnd = CAgentListeningWnd::CreateInstance())
		)
	{
		mListeningWnd->Create (this, GetExStyle() & WS_EX_TOPMOST);
	}
	return mListeningWnd;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

long CAgentCharacterWnd::QueueShow (long pCharID, bool pFast, int pVisibilityCause)
{
	long			lReqID = 0;
	CQueuedShow*	lQueuedShow = NULL;

	if	(
			(IsWindow ())
		&&	(lQueuedShow = new CQueuedShow (pCharID, lReqID=NextReqID()))
		)
	{
		lQueuedShow->mFast = pFast;
		lQueuedShow->mVisibilityCause = pVisibilityCause;
		if	(pFast)
		{
			mQueue.AddHead (lQueuedShow);
		}
		else
		{
			mQueue.AddTail (lQueuedShow);
		}
	}
	else
	{
		lReqID = 0;
	}

#ifdef	_LOG_POPUP_OPS
	if	(LogIsActive (_LOG_POPUP_OPS))
	{
		LogMessage (_LOG_POPUP_OPS, _T("[%p(%d)] QueueShow [%p(%d)] [%u]"), this, pCharID, lQueuedShow, lReqID, pFast);
	}
#endif
	return lReqID;
}

/////////////////////////////////////////////////////////////////////////////

long CAgentCharacterWnd::IsShowingQueued ()
{
	CQueuedAction*	lQueuedShow;

	if	(
			(this)
		&&	(IsWindow ())
		&&	(lQueuedShow = mQueue.GetCharAction (QueueActionShow, -1))
		&&	(lQueuedShow == mQueue.FindNextAction ())
		)
	{
		return mCharID;
	}
	return 0;
}

bool CAgentCharacterWnd::ShowQueued (CQueuedShow* pQueuedShow)
{
	mCharShown = true;
	NotifyShown (pQueuedShow->mCharID, (VisibilityCauseType)pQueuedShow->mVisibilityCause);
	if	(IsWindowVisible ())
	{
		Invalidate ();
	}
	if	(GetStyle() & WS_VISIBLE)
	{
		SetLastActive ();
	}
	return true;
}

bool CAgentCharacterWnd::NotifyShown (long pForCharID, VisibilityCauseType pVisiblityCause)
{
	if	(pVisiblityCause != VisibilityCause_NeverShown)
	{
		if	(PreNotify ())
		{
			try
			{
				INT_PTR				lNotifyNdx;
				CEventNotify*		lNotify;
				long				lNotifyCharID;
				VisibilityCauseType	lVisibilityCause;

				for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
				{
					lNotifyCharID = lNotify->GetNotifyClient (mCharID);
					lVisibilityCause = ((pVisiblityCause==VisibilityCause_ProgramShowed) && (lNotifyCharID!=pForCharID)) ? VisibilityCause_OtherProgramShowed : pVisiblityCause;

					lNotify->_PutVisibilityCause (lNotifyCharID, lVisibilityCause);
					lNotify->VisibleState (lNotifyCharID, TRUE, lVisibilityCause);
				}
			}
			catch AnyExceptionDebug
		}
		PostNotify ();
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

long CAgentCharacterWnd::QueueHide (long pCharID, bool pFast, int pVisibilityCause)
{
	long			lReqID = 0;
	CQueuedHide*	lQueuedHide = NULL;

	if	(
			(IsWindow ())
		&&	(lQueuedHide = new CQueuedHide (pCharID, lReqID=NextReqID()))
		)
	{
		lQueuedHide->mFast = pFast;
		lQueuedHide->mVisibilityCause = pVisibilityCause;
		mQueue.AddTail (lQueuedHide);
	}
	else
	{
		lReqID = 0;
	}

#ifdef	_LOG_POPUP_OPS
	if	(LogIsActive (_LOG_POPUP_OPS))
	{
		LogMessage (_LOG_POPUP_OPS, _T("[%p(%d)] QueueHide [%p(%d)] [%u]"), this, pCharID, lQueuedHide, lReqID, pFast);
	}
#endif
	return lReqID;
}

/////////////////////////////////////////////////////////////////////////////

long CAgentCharacterWnd::IsHidingQueued ()
{
	CQueuedAction*	lQueuedHide;

	if	(
			(this)
		&&	(IsWindow ())
		&&	(lQueuedHide = mQueue.GetCharAction (QueueActionHide, -1))
		&&	(lQueuedHide == mQueue.FindNextAction ())
		)
	{
		return mCharID;
	}
	return 0;
}

bool CAgentCharacterWnd::HideQueued (CQueuedHide* pQueuedHide)
{
	mCharShown = false;
	Invalidate ();
	NotifyHidden (GetCharID(), (VisibilityCauseType)pQueuedHide->mVisibilityCause);
	return true;
}

bool CAgentCharacterWnd::NotifyHidden (long pForCharID, VisibilityCauseType pVisiblityCause)
{
	if	(pVisiblityCause != VisibilityCause_NeverShown)
	{
		if	(PreNotify ())
		{
			try
			{
				INT_PTR				lNotifyNdx;
				CEventNotify*		lNotify;
				long				lNotifyCharID;
				VisibilityCauseType	lVisibilityCause;

				for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
				{
					lNotifyCharID = lNotify->GetNotifyClient (mCharID);
					lVisibilityCause = ((pVisiblityCause==VisibilityCause_ProgramHid) && (lNotifyCharID!=pForCharID)) ? VisibilityCause_OtherProgramHid : pVisiblityCause;

					lNotify->_PutVisibilityCause (lNotifyCharID, lVisibilityCause);
					lNotify->VisibleState (lNotifyCharID, FALSE, lVisibilityCause);
				}

				if	(GetLastActive() == m_hWnd)
				{
					for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
					{
						lNotify->ActiveCharacterNotify (-1, -1, mCharID, mCharID);
					}
					for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
					{
						if	(lNotify->ActiveCharacterChanged (-1, -1, mCharID, mCharID))
						{
							break;
						}
					}
				}
			}
			catch AnyExceptionDebug
		}
		PostNotify ();
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

long CAgentCharacterWnd::QueueThink (long pCharID, LPCTSTR pText, class CAgentTextObject* pTextObject, CAgentBalloonOptions* pBalloonOptions, UINT pSapiVersion)
{
	long			lReqID = 0;
	CQueuedThink *	lQueuedThink = NULL;

	if	(
			(IsWindow ())
		&&	(lQueuedThink = new CQueuedThink (pBalloonOptions, pCharID, lReqID=NextReqID()))
		)
	{
		if	(pTextObject)
		{
			lQueuedThink->Initialize (pTextObject);
		}
		else
		{
			CAtlString		lText;
			CAtlStringArray	lTextParts;

			if	(MakeStringArray (pText, lTextParts, _T("|")) > 1)
			{
				lText = lTextParts [((rand() / lTextParts.GetCount()) * lTextParts.GetCount()) % lTextParts.GetCount()];
			}
			else
			{
				lText = pText;
			}

			if	(lText.CompareNoCase (_T("\\lst\\")) == 0)
			{
				lQueuedThink->Initialize (mLastSpeech);
			}
			else
			{
				CAgentTextParse	lTextParse;

				if	(pSapiVersion)
				{
					lTextParse.SetSapiVersion (pSapiVersion);
				}
				lTextParse = lText;
				lQueuedThink->Initialize (lTextParse);
			}
		}

#ifdef	_DEBUG_BALLOON_OPTIONS
		if	(
				(lQueuedThink->mBalloonOptions)
			&&	(LogIsActive (_DEBUG_BALLOON_OPTIONS))
			)
		{
			CAtlString	lPrefix;
			lPrefix.Format (_T("[%p(%d)]"), this, mCharID);
			lQueuedThink->mBalloonOptions->LogOptions (_DEBUG_BALLOON_OPTIONS, _T("QueueThink"), lPrefix);
		}
#endif
		mQueue.AddTail (lQueuedThink);
	}
	else
	{
		lReqID = 0;
	}
#ifdef	_LOG_QUEUE_OPS
	if	(LogIsActive (_LOG_QUEUE_OPS))
	{
		LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] QueueThink [%p(%d)] [%s] [%p]"), this, pCharID, lQueuedThink, lReqID, (lQueuedThink ? (LPCTSTR)DebugStr(lQueuedThink->GetFullText()) : NULL), pTextObject);
	}
#endif
	return lReqID;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

long CAgentCharacterWnd::QueueSpeak (long pCharID, LPCTSTR pText, CAgentTextObject* pTextObject, LPCTSTR pSoundUrl, CSapiVoice* pVoice, CAgentBalloonOptions* pBalloonOptions)
{
	long			lReqID = 0;
	CQueuedSpeak *	lQueuedSpeak = NULL;
#ifdef	DebugTimeStart
	DebugTimeStart
#endif

	if	(
			(IsWindow ())
		&&	(lQueuedSpeak = new CQueuedSpeak (pBalloonOptions, pCharID, lReqID=NextReqID()))
		)
	{
		lQueuedSpeak->mSoundUrl = pSoundUrl;

		if	(pTextObject)
		{
			lQueuedSpeak->Initialize (pTextObject, pVoice);
		}
		else
		{
			CAtlString		lText;
			CAtlStringArray	lTextParts;
			CAgentTextParse	lTextParse (pText, pVoice->SafeIsValid());

			if	(MakeStringArray (pText, lTextParts, _T("|")) > 1)
			{
				lText = lTextParts [((rand() / lTextParts.GetCount()) * lTextParts.GetCount()) % lTextParts.GetCount()];
			}
			else
			{
				lText = pText;
			}

			if	(!lQueuedSpeak->mSoundUrl.IsEmpty ())
			{
				lTextParts.RemoveAll ();
				lTextParse.SplitText (lText, lTextParts);
				lTextParse = lTextParts;
				lQueuedSpeak->Initialize (lTextParse, pVoice);
			}
			else
			if	(lText.CompareNoCase (_T("\\lst\\")) == 0)
			{
				lQueuedSpeak->Initialize (mLastSpeech, pVoice);
			}
			else
			{
				lTextParse = lText;
				lQueuedSpeak->Initialize (lTextParse, pVoice);
			}
		}

#ifdef	_DEBUG_SPEECH
		if	(
				(lQueuedSpeak->mSoundUrl.IsEmpty ())
			&&	(LogIsActive (_DEBUG_SPEECH))
			)
		{
			LogMessage (_DEBUG_SPEECH, _T("[%p(%d)] CAgentCharacterWnd Queue   [%s] [%p]"), this, mCharID, DebugStr(pText), pTextObject);
			LogMessage (_DEBUG_SPEECH, _T("[%p(%d)]                    Speech  [%s]"), this, mCharID, DebugStr(lQueuedSpeak->GetSpeechText()));
			LogMessage (_DEBUG_SPEECH, _T("[%p(%d)]                    Text    [%s]"), this, mCharID, DebugStr(lQueuedSpeak->GetFullText()));
			LogMessage (_DEBUG_SPEECH, _T("[%p(%d)]                    Voice   [%u] Busy [%u] Balloon [%u] Busy [%u]"), this, mCharID, lQueuedSpeak->mVoice->SafeIsValid(), lQueuedSpeak->mVoice->SafeIsSpeaking (), (mBalloonWnd!=NULL), ((mBalloonWnd!= NULL) && mBalloonWnd->IsBusy (false)));
			LogMessage (_DEBUG_SPEECH, _T("[%p(%d)]                    Queue   [%u] Busy [%u %u]"), this, mCharID, mQueue.GetCount(), IsQueueBusy(), !IsAnimationComplete());
		}
#endif
#ifdef	_DEBUG_BALLOON_OPTIONS
		if	(
				(lQueuedSpeak->mBalloonOptions)
			&&	(LogIsActive (_DEBUG_BALLOON_OPTIONS))
			)
		{
			CAtlString	lPrefix;
			lPrefix.Format (_T("[%p(%d)]"), this, mCharID);
			lQueuedSpeak->mBalloonOptions->LogOptions (_DEBUG_BALLOON_OPTIONS, _T("QueueSpeak"), lPrefix);
		}
#endif
		mQueue.AddTail (lQueuedSpeak);
	}
	else
	{
		lReqID = 0;
	}
#ifdef	_LOG_QUEUE_OPS
	if	(LogIsActive (_LOG_QUEUE_OPS))
	{
		LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] QueueSpeak [%p(%d)] [%s] [%p] [%s]"), this, pCharID, lQueuedSpeak, lReqID, DebugStr(pText), pTextObject, DebugStr(pSoundUrl));
	}
#endif

#ifdef	DebugTimeStart
	DebugTimeStop
	LogMessage (LogIfActive|LogTimeMs|LogHighVolume, _T("%f CAgentCharacterWnd::QueueSpeak"), DebugTimeElapsed);
#endif
	return lReqID;
}

/////////////////////////////////////////////////////////////////////////////

CQueuedAction* CAgentCharacterWnd::IsSpeakQueued (long pCharID)
{
	return mQueue.GetCharAction (QueueActionSpeak, pCharID);
}

bool CAgentCharacterWnd::UpdateQueuedSpeak (long pCharID, CSapiVoice* pVoice)
{
	bool			lRet = false;
	POSITION		lPos;
	CQueuedAction*	lQueuedAction;
	CQueuedSpeak *	lQueuedSpeak;

	if	(pVoice)
	{
		for	(lPos = mQueue.GetHeadPosition(); lPos;)
		{
			if	(
					(lQueuedAction = mQueue.GetNext (lPos))
				&&	(lQueuedAction->mAction == QueueActionSpeak)
				&&	(lQueuedAction->mCharID == pCharID)
				)
			{
				lQueuedSpeak = (CQueuedSpeak *) lQueuedAction;
				if	(
						(!lQueuedSpeak->mStarted)
					&&	(lQueuedSpeak->SetVoice (pVoice))
					)
				{
					lRet = true;
				}
			}
		}
	}
	return lRet;
}

void CAgentCharacterWnd::SetLastSpeech (CAgentText& pSpeech)
{
	mLastSpeech = pSpeech;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentCharacterWnd::StartMouthAnimation (long pSpeakingDuration)
{
	bool				lRet = false;
	CAgentFile*		lAgentFile;
	CAgentStreamInfo*	lStreamInfo;
	long				lAnimationNdx = -1;
	long				lSpeakingFrameNdx = -1;
#ifdef	DebugTimeStart
	DebugTimeStart
#endif

	if	(
			(lAgentFile = GetAgentFile())
		&&	(lStreamInfo = GetAgentStreamInfo())
		)
	{
#ifdef	_DEBUG_SPEECH_MOUTH
		if	(
				(LogIsActive (_DEBUG_SPEECH_MOUTH))
			&&	(lStreamInfo->GetAnimationIndex (&lAnimationNdx) == S_OK)
			&&	(lAnimationNdx >= 0)
			)
		{
			LogMessage (_DEBUG_SPEECH_MOUTH, _T("[%p(%d)]   Last Animation [%d] [%ls]"), this, mCharID, lAnimationNdx, (BSTR)(lAgentFile->GetAnimation (lAnimationNdx)->Name));
		}
#endif
		if	(
				(lStreamInfo->GetAnimationIndex (&lAnimationNdx) == S_OK)
			&&	(lAnimationNdx >= 0)
			&&	(lStreamInfo->FindAnimationSpeakingFrame (&lSpeakingFrameNdx, lAnimationNdx) == S_OK)
			&&	(lSpeakingFrameNdx >= 0)
			)
		{
			lStreamInfo->ClearAnimationSequences ();

			if	(
					(lStreamInfo->NewAnimationSequence () == S_OK)
				&&	(lStreamInfo->SequenceAnimationFrame (lAnimationNdx, lSpeakingFrameNdx) == S_OK)
				)
			{
				lStreamInfo->SetSpeakingDuration (pSpeakingDuration);
				AnimationSequenceChanged ();
#ifdef	_DEBUG_SPEECH_MOUTH
				if	(LogIsActive (_DEBUG_SPEECH_MOUTH))
				{
					LogMessage (_DEBUG_SPEECH_MOUTH, _T("[%p(%d)]   Speech MouthAnimation [%d] [%ls] Frame [%d] Duration [%d]"), this, mCharID, lAnimationNdx, (BSTR)(lAgentFile->GetAnimation (lAnimationNdx)->Name), lSpeakingFrameNdx, pSpeakingDuration);
				}
#endif
				lRet = true;
			}
			else
			{
				AnimationSequenceChanged ();
			}
		}
	}
#ifdef	DebugTimeStart
	DebugTimeStop
	LogMessage (LogIfActive|LogTimeMs|LogHighVolume, _T("%f   CAgentCharacterWnd::StartMouthAnimation"), DebugTimeElapsed);
#endif
	return lRet;
}

bool CAgentCharacterWnd::StopMouthAnimation ()
{
	bool				lRet;
	CAgentStreamInfo*	lStreamInfo;

	lRet = PlayMouthAnimation (-1, true);

	if	(
			(lStreamInfo = GetAgentStreamInfo())
		&&	(lStreamInfo->SetSpeakingDuration (0))
		)
	{
		lStreamInfo->ResetMouthOverlays ();
		AnimationSequenceChanged ();
#ifdef	_DEBUG_SPEECH_MOUTH
		if	(LogIsActive (_DEBUG_SPEECH_MOUTH))
		{
			LogMessage (_DEBUG_SPEECH_MOUTH, _T("[%p(%d)]   Speech MouthAnimation stopped"), this, mCharID);
		}
#endif
		lRet = true;
	}
	return lRet;
}

bool CAgentCharacterWnd::PlayMouthAnimation (short pMouthOverlayNdx, bool pPlayAlways)
{
	bool				lRet = false;
	CAgentStreamInfo*	lStreamInfo;
#ifdef	DebugTimeStart
	DebugTimeStart
#endif

	if	(lStreamInfo = GetAgentStreamInfo())
	{
		if	(
				(
					(
						(pMouthOverlayNdx < 0)
					?	(lStreamInfo->ResetMouthOverlays ())
					:	(lStreamInfo->SetMouthOverlay (pMouthOverlayNdx))
					)
				||	(pPlayAlways)
				)
			&&	(
					(pPlayAlways)
				||	(IsAnimationComplete ())
				)
			)
		{
			long	lStopPosition = GetDurationMs();
			long	lStartPosition = max (lStopPosition - 100000, 0);

#ifdef	_DEBUG_SPEECH_MOUTH
			if	(LogIsActive (_DEBUG_SPEECH_MOUTH))
			{
				LogMessage (_DEBUG_SPEECH_MOUTH, _T("[%p(%d)]   Speech MouthAnimation [%d] from [%d] to [%d]"), this, mCharID, pMouthOverlayNdx, lStartPosition, lStopPosition);
			}
#endif
			if	(SUCCEEDED (PlayFromTo (lStartPosition, lStopPosition, (pMouthOverlayNdx < 0), 0)))
			{
				lRet = true;
			}
		}
	}
#ifdef	DebugTimeStart
	DebugTimeStop
	LogMessage (LogIfActive|LogTimeMs|LogHighVolume, _T("%f   CAgentCharacterWnd::PlayMouthAnimation"), DebugTimeElapsed);
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentCharacterWnd::KeepBalloonVisible (CAgentBalloonWnd* pBalloon)
{
	bool				lRet = false;
	CQueuedAction*		lQueuedAction;
	CQueuedThink *		lQueuedThink;
	CQueuedSpeak *		lQueuedSpeak;
	POSITION			lPos;

	if	(
			(!mQueue.IsEmpty ())
		&&	(lPos = mQueue.GetHeadPosition ())
		&&	(lQueuedAction = mQueue.GetNext (lPos))
		)
	{
		if	(
				(lQueuedAction->mAction == QueueActionThink)
			&&	(lQueuedThink = (CQueuedThink *) lQueuedAction)
			&&	(
					(!lQueuedThink->mStarted)
				||	(
						(lQueuedThink->mStarted)
					&&	(lPos)
					&&	(lQueuedAction = mQueue.GetNext (lPos))
					&&	(lQueuedAction->mAction == QueueActionThink)
					&&	(lQueuedThink = (CQueuedThink *) lQueuedAction)
					&&	(!lQueuedThink->mStarted)
					)
				)
			)
		{
			lRet = true;
		}
		else
		if	(
				(lQueuedAction->mAction == QueueActionSpeak)
			&&	(lQueuedSpeak = (CQueuedSpeak *) lQueuedAction)
			&&	(lQueuedSpeak->ShowBalloon())
			&&	(
					(!lQueuedSpeak->mStarted)
				||	(
						(lQueuedSpeak->mStarted)
					&&	(lPos)
					&&	(lQueuedAction = mQueue.GetNext (lPos))
					&&	(lQueuedAction->mAction == QueueActionSpeak)
					&&	(lQueuedSpeak = (CQueuedSpeak *) lQueuedAction)
					&&	(lQueuedSpeak->ShowBalloon())
					&&	(!lQueuedSpeak->mStarted)
					)
				)
			)
		{
			lRet = true;
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CAgentCharacterWnd::OnVoiceStart (long pCharID)
{
#ifdef	_DEBUG_SPEECH_EVENTS
	if	(LogIsActive (_DEBUG_SPEECH_EVENTS))
	{
		LogMessage (_DEBUG_SPEECH_EVENTS, _T("[%p(%d)] CAgentCharacterWnd   OnVoiceStart"), this, mCharID);
	}
#endif
	PostMessage (mVoiceStartMsg, pCharID);
}

void CAgentCharacterWnd::OnVoiceEnd (long pCharID)
{
#ifdef	_DEBUG_SPEECH_EVENTS
	if	(LogIsActive (_DEBUG_SPEECH_EVENTS))
	{
		LogMessage (_DEBUG_SPEECH_EVENTS, _T("[%p(%d)] CAgentCharacterWnd   OnVoiceEnd"), this, mCharID);
	}
#endif
	PostMessage (mVoiceEndMsg, pCharID);
}

void CAgentCharacterWnd::OnVoiceBookMark (long pCharID, long pBookMarkId)
{
#ifdef	_DEBUG_SPEECH_EVENTS
	if	(LogIsActive (_DEBUG_SPEECH_EVENTS))
	{
		LogMessage (_DEBUG_SPEECH_EVENTS, _T("[%p(%d)] CAgentCharacterWnd   OnVoiceBookMark [%d] [%d]"), this, mCharID, pCharID, pBookMarkId);
	}
#endif
	PostMessage (mVoiceBookMarkMsg, pCharID, pBookMarkId);
}

void CAgentCharacterWnd::OnVoiceVisual (long pCharID, int pMouthOverlay)
{
#ifdef	_DEBUG_SPEECH_EVENTS
	if	(LogIsActive (_DEBUG_SPEECH))
	{
		LogMessage (_DEBUG_SPEECH_EVENTS, _T("[%p(%d)] CAgentCharacterWnd     OnVoiceVisual [%s]"), this, mCharID, MouthOverlayStr(pMouthOverlay));
	}
#endif
	PostMessage (mVoiceVisualMsg, pCharID, pMouthOverlay);
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CAgentCharacterWnd::OnVoiceStartMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#ifdef	_DEBUG_SPEECH_EVENTS
	if	(LogIsActive (_DEBUG_SPEECH_EVENTS))
	{
		LogMessage (_DEBUG_SPEECH_EVENTS, _T("[%p(%d)] CAgentCharacterWnd   OnVoiceStartMsg"), this, mCharID);
	}
#endif
	if	(StartMouthAnimation ())
	{
		PlayMouthAnimation (-1, true);
	}
	return 0;
}

LRESULT CAgentCharacterWnd::OnVoiceEndMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#ifdef	_DEBUG_SPEECH_EVENTS
	if	(LogIsActive (_DEBUG_SPEECH_EVENTS))
	{
		LogMessage (_DEBUG_SPEECH_EVENTS, _T("[%p(%d)] CAgentCharacterWnd   OnVoiceEndMsg"), this, mCharID);
	}
#endif
	StopMouthAnimation ();
	return 0;
}

LRESULT CAgentCharacterWnd::OnVoiceBookMarkMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	long	lCharID = (long)wParam;
	long	lBookMarkId = (long)lParam;
#ifdef	_DEBUG_SPEECH_EVENTS
	if	(LogIsActive (_DEBUG_SPEECH_EVENTS))
	{
		LogMessage (_DEBUG_SPEECH_EVENTS, _T("[%p(%d)] CAgentCharacterWnd   OnVoiceBookMarkMsg [%d] [%d]"), this, mCharID, lCharID, lBookMarkId);
	}
#endif
	if	(PreNotify ())
	{
		try
		{
			CEventNotify*	lNotify;

			if	(lNotify = GetNotifyClientNotify (lCharID))
			{
				lNotify->BookMark (lBookMarkId);
			}
		}
		catch AnyExceptionDebug
	}
	PostNotify ();

	return 0;
}

LRESULT CAgentCharacterWnd::OnVoiceVisualMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int	lMouthOverlay = (int)lParam;
#ifdef	_DEBUG_SPEECH_EVENTS
	if	(LogIsActive (_DEBUG_SPEECH))
	{
		LogMessage (_DEBUG_SPEECH_EVENTS, _T("[%p(%d)] CAgentCharacterWnd     OnVoiceVisualMsg [%s]"), this, mCharID, MouthOverlayStr(lMouthOverlay));
	}
#endif
	PlayMouthAnimation (lMouthOverlay, true);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

long CAgentCharacterWnd::QueueWait (long pCharID, long pOtherCharID, long pOtherReqID)
{
	long			lReqID = 0;
	CQueuedWait *	lQueuedWait = NULL;

	if	(
			(IsWindow ())
		&&	(lQueuedWait = new CQueuedWait (pOtherCharID, pOtherReqID, pCharID, lReqID=NextReqID()))
		)
	{
		mQueue.AddTail (lQueuedWait);
#ifdef	_LOG_QUEUE_OPS
		if	(LogIsActive (_LOG_QUEUE_OPS))
		{
			LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] QueueWait [%p(%d)] Other [%d] [%d]"), this, pCharID, lQueuedWait, lReqID, pOtherCharID, pOtherReqID);
		}
#endif
	}
	else
	{
		lReqID = 0;
	}
	return lReqID;
}

/////////////////////////////////////////////////////////////////////////////

long CAgentCharacterWnd::QueueInterrupt (long pCharID, long pOtherCharID, long pOtherReqID)
{
	long				lReqID = 0;
	CQueuedInterrupt *	lQueuedInterrupt = NULL;

	if	(
			(IsWindow ())
		&&	(lQueuedInterrupt = new CQueuedInterrupt (pOtherCharID, pOtherReqID, pCharID, lReqID=NextReqID()))
		)
	{
		mQueue.AddTail (lQueuedInterrupt);
#ifdef	_LOG_QUEUE_OPS
		if	(LogIsActive (_LOG_QUEUE_OPS))
		{
			LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] QueueInterrupt [%p(%d)] Other [%d] [%d]"), this, pCharID, lQueuedInterrupt, lReqID, pOtherCharID, pOtherReqID);
		}
#endif
	}
	else
	{
		lReqID = 0;
	}
	return lReqID;
}

/////////////////////////////////////////////////////////////////////////////

CQueuedAction* CAgentCharacterWnd::FindOtherRequest (long pReqID, CAgentCharacterWnd*& pRequestOwner)
{
	CQueuedAction*			lRet = NULL;
	INT_PTR					lNotifyNdx;
	CEventNotify*			lNotify;
	CAgentCharacterWnd*	lRequestOwner;

	pRequestOwner = NULL;

	for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
	{
		if	(
				(lRequestOwner = dynamic_cast <CAgentCharacterWnd*> (lNotify->GetRequestOwner (pReqID)))
			&&	(lRet = lRequestOwner->FindQueuedAction (pReqID))
			)
		{
			pRequestOwner = lRequestOwner;
			break;
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

int CAgentCharacterWnd::IsIdle () const
{
	int	lRet = 0;

	if	(IsCharShown ())
	{
		lRet = CAgentWnd::IsIdle ();

		if	(
				(lRet > 0)
			&&	(mBalloonWnd)
			&&	(mBalloonWnd->IsWindow ())
			&&	(mBalloonWnd->IsBusy (true))
			)
		{
			lRet = 0;
		}
	}
	return lRet;
}

bool CAgentCharacterWnd::StopIdle (LPCTSTR pReason)
{
	bool	lRet;
	bool	lIdleStarted = mIdleStarted;

	lRet = CAgentWnd::StopIdle (pReason);

	if	(
			(lRet)
		&&	(lIdleStarted)
		)
	{
		if	(PreNotify ())
		{
			try
			{
				INT_PTR			lNotifyNdx;
				CEventNotify*	lNotify;

				for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
				{
					lNotify->Idle (lNotify->GetNotifyClient (mCharID), FALSE);
				}
			}
			catch AnyExceptionDebug
		}
		PostNotify ();
	}
	return lRet;
}

bool CAgentCharacterWnd::DoIdle ()
{
	bool	lRet;
	bool	lIdleStarted = mIdleStarted;

	lRet = CAgentWnd::DoIdle ();

	if	(
			(lRet)
		&&	(!lIdleStarted)
		&&	(mIdleStarted)
		)
	{
		if	(PreNotify ())
		{
			try
			{
				INT_PTR			lNotifyNdx;
				CEventNotify*	lNotify;

				for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
				{
					lNotify->Idle (lNotify->GetNotifyClient (mCharID), TRUE);
				}
			}
			catch AnyExceptionDebug
		}
		PostNotify ();
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LRESULT CAgentCharacterWnd::OnTimer (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if	(!mQueue.IsEmpty ())
	{
		StopIdle (_T("Queue"));
	}
	return CAgentWnd::OnTimer (uMsg, wParam, lParam, bHandled);
}

/////////////////////////////////////////////////////////////////////////////

short CAgentCharacterWnd::NotifyKeyState () const
{
	short	lResult = 0;

	if	(GetKeyState (VK_SHIFT) < 0)
	{
		lResult |= MK_SHIFT;
	}
	if	(GetKeyState (VK_CONTROL) < 0)
	{
		lResult |= MK_CONTROL;
	}
	if	(GetKeyState (VK_MENU) < 0)
	{
		lResult |= MK_ALT;
	}
	return lResult;
}

void CAgentCharacterWnd::NotifyClick (short pButton, const CPoint& pPoint)
{
	if	(PreNotify ())
	{
		try
		{
			CEventNotify*	lNotify;

			if	(lNotify = GetNotifyClientNotify (mCharID))
			{
				lNotify->Click (mCharID, pButton|NotifyKeyState(), pPoint.x, pPoint.y);
			}
		}
		catch AnyExceptionDebug
	}
	PostNotify ();
}

void CAgentCharacterWnd::NotifyDblClick (short pButton, const CPoint& pPoint)
{
	if	(PreNotify ())
	{
		try
		{
			CEventNotify*	lNotify;

			if	(lNotify = GetNotifyClientNotify (mCharID))
			{
				lNotify->DblClick (mCharID, pButton|NotifyKeyState(), pPoint.x, pPoint.y);
			}
		}
		catch AnyExceptionDebug
	}
	PostNotify ();
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CAgentCharacterWnd::OnLButtonDown (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT	lResult = 0;
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnLButtonDown [%d %d]"), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
#endif
	if	(!(bHandled = CAgentWnd::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult)))
	{
		bHandled = TRUE;
		lResult = DefWindowProc ();
	}
	mLastButtonMsg = WM_LBUTTONDOWN;
	bHandled = FALSE;
	return 0;
}

LRESULT CAgentCharacterWnd::OnLButtonUp (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT	lResult = 0;
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnLButtonUp [%d %d]"), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
#endif
	if	(!(bHandled = CAgentWnd::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult)))
	{
		bHandled = TRUE;
		lResult = DefWindowProc ();
	}
	if	(mLastButtonMsg != WM_LBUTTONDBLCLK)
	{
		NotifyClick (MK_LBUTTON, CPoint (GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
	}
	mLastButtonMsg = WM_LBUTTONUP;
	bHandled = FALSE;
	return 0;
}

LRESULT CAgentCharacterWnd::OnLButtonDblClk (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT	lResult = 0;
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnLButtonDblClk [%d %d]"), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
#endif
	if	(!(bHandled = CAgentWnd::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult)))
	{
		bHandled = TRUE;
		lResult = DefWindowProc ();
	}
	NotifyDblClick (MK_LBUTTON, CPoint (GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
	mLastButtonMsg = WM_LBUTTONDBLCLK;
	return lResult;
}

///////////////////////////////////////////////////////////////////////////////

LRESULT CAgentCharacterWnd::OnRButtonDown (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT	lResult = 0;
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnRButtonDown [%d %d]"), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
#endif
	if	(!(bHandled = CAgentWnd::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult)))
	{
		bHandled = TRUE;
		lResult = DefWindowProc ();
	}
	mLastButtonMsg = WM_RBUTTONDOWN;
	return lResult;
}

LRESULT CAgentCharacterWnd::OnRButtonUp (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT	lResult = 0;
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnRButtonUp [%d %d]"), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
#endif
	if	(PreNotify ())
	{
		try
		{
			if	(!(bHandled = CAgentWnd::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult)))
			{
				bHandled = TRUE;
				lResult = DefWindowProc ();
			}
			if	(mLastButtonMsg != WM_RBUTTONDBLCLK)
			{
				NotifyClick (MK_RBUTTON, CPoint (GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
			}
			mLastButtonMsg = WM_RBUTTONUP;
		}
		catch AnyExceptionDebug
	}
	PostNotify ();

	return lResult;
}

LRESULT CAgentCharacterWnd::OnRButtonDblClk (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT	lResult = 0;
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnRButtonDblClk [%d %d]"), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
#endif
	if	(!(bHandled = CAgentWnd::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult)))
	{
		bHandled = TRUE;
		lResult = DefWindowProc ();
	}
	NotifyDblClick (MK_RBUTTON, CPoint (GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
	mLastButtonMsg = WM_RBUTTONDBLCLK;
	bHandled = FALSE;
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

LRESULT CAgentCharacterWnd::OnMButtonDown (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT	lResult = 0;
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnMButtonDown [%d %d]"), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
#endif
	if	(!(bHandled = CAgentWnd::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult)))
	{
		bHandled = TRUE;
		lResult = DefWindowProc ();
	}
	mLastButtonMsg = WM_MBUTTONDOWN;
	return lResult;
}

LRESULT CAgentCharacterWnd::OnMButtonUp (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT	lResult = 0;
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnMButtonUp [%d %d]"), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
#endif
	if	(!(bHandled = CAgentWnd::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult)))
	{
		bHandled = TRUE;
		lResult = DefWindowProc ();
	}
	if	(mLastButtonMsg != WM_MBUTTONDBLCLK)
	{
		NotifyClick (MK_MBUTTON, CPoint (GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
	}
	mLastButtonMsg = WM_MBUTTONUP;
	return lResult;
}

LRESULT CAgentCharacterWnd::OnMButtonDblClk (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT	lResult = 0;
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnMButtonDblClk [%d %d]"), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
#endif
	if	(!(bHandled = CAgentWnd::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult)))
	{
		bHandled = TRUE;
		lResult = DefWindowProc ();
	}
	NotifyDblClick (MK_MBUTTON, CPoint (GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
	mLastButtonMsg = WM_MBUTTONDBLCLK;
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LRESULT CAgentCharacterWnd::OnContextMenu (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnContextMenu [%d %d]"), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
#endif
	if	((HWND)wParam == m_hWnd)
	{
		if	(PreNotify ())
		{
			try
			{
				CEventNotify*	lNotify;

				if	(lNotify = GetNotifyClientNotify (mCharID))
				{
					lNotify->_ContextMenu (mCharID, m_hWnd, CPoint (GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
				}
			}
			catch AnyExceptionDebug
		}
		PostNotify ();
	}
	return 0;
}

LRESULT CAgentCharacterWnd::OnDisplayChange (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT	lResult = 0;
#ifdef	_DEBUG
	LogMessage (LogNormal, _T("CAgentCharacterWnd::OnDisplayChange"));
#endif
	bHandled = CAgentWnd::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult);
	return lResult;
}

LRESULT CAgentCharacterWnd::OnInputLangChange (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT	lResult = 0;
#ifdef	_DEBUG
	LogMessage (LogNormal, _T("CAgentCharacterWnd::OnInputLangChange [%4.4hX]"), LOWORD(GetKeyboardLayout(GetCurrentThreadId())));
#endif
	bHandled = CAgentWnd::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult);
	return lResult;
}
