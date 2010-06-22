/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
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
//#define	_DEBUG_ACTIVATE		LogNormal
#define	_DEBUG_SPEECH			(GetProfileDebugInt(_T("DebugSpeech"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
#define	_DEBUG_SPEECH_EVENTS	(GetProfileDebugInt(_T("DebugSpeechEvents"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Popup"),LogDetails,true)&0xFFFF)
#define	_LOG_POPUP_OPS			(GetProfileDebugInt(_T("LogPopupOps"),LogVerbose,true)&0xFFFF|LogTimeMs)
#define	_LOG_QUEUE_OPS			(GetProfileDebugInt(_T("LogQueueOps"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
#endif

#ifndef	_LOG_INSTANCE
#define	_LOG_INSTANCE		LogDetails
#endif
#ifndef	_LOG_POPUP_OPS
#define	_LOG_POPUP_OPS		LogDetails
#endif
#ifndef	_LOG_QUEUE_OPS
#define	_LOG_QUEUE_OPS		LogDetails
#endif

/////////////////////////////////////////////////////////////////////////////

HWND	CAgentCharacterWnd::mLastActive = NULL;
UINT	CAgentCharacterWnd::mVoiceStartMsg = RegisterWindowMessage (_T("A444DB92-39D0-4677-8D6D-1C4032BC9DED"));
UINT	CAgentCharacterWnd::mVoiceEndMsg = RegisterWindowMessage (_T("AD44294A-BC10-43e5-94A7-C9C392863A79"));
UINT	CAgentCharacterWnd::mVoiceBookMarkMsg = RegisterWindowMessage (_T("8FC08C6D-E6EB-4d53-B115-8378AB001571"));
UINT	CAgentCharacterWnd::mVoiceVisualMsg = RegisterWindowMessage (_T("242D8583-6BAC-44d5-8CF8-F6DD020F701C"));

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

bool CAgentCharacterWnd::Attach (long pCharID, CEventNotify * pNotify, const CAgentIconData * pIconData, bool pSetActiveCharID)
{
	bool	lRet = false;
	long	lPrevCharID = mCharID;

	if	(
			(pNotify)
		&&	(mNotify.AddUnique (pNotify) >= 0)
		)
	{
		pNotify->RegisterEventLock (this, true);
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

bool CAgentCharacterWnd::Detach (long pCharID, CEventNotify * pNotify)
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
			pNotify->RegisterEventLock (this, false);
			lRet = true;
		}

		if	(
				(pCharID < 0)
			&&	(!pNotify)
			)
		{
			INT_PTR			lNotifyNdx;
			CEventNotify *	lNotify;

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
	CAgentCharacterWnd *	lLastActive = NULL;
	long					lLastActiveCharID = -1;
	INT_PTR					lNotifyNdx;
	CEventNotify *			lNotify;

	if	(mLastActive != m_hWnd)
	{
		CAgentWnd *	lAgentWnd;

		for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
		{
			if	(
					(mLastActive)
				&&	(lAgentWnd = lNotify->GetAgentWnd (mLastActive))
				)
			{
				if	(lLastActive = dynamic_cast <CAgentCharacterWnd *> (lAgentWnd))
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

		if	(
				(lLastActive)
			&&	(lLastActive->PreNotify ())
			)
		{
			try
			{
#ifdef	_DEBUG_ACTIVATE
				LogMessage (_DEBUG_ACTIVATE, _T("[%p] SetNotInputActive [%d] (Activate)"), this, lLastActiveCharID);
#endif
				for	(lNotifyNdx = 0; lNotify = lLastActive->mNotify (lNotifyNdx); lNotifyNdx++)
				{
					lNotify->ActiveCharacterNotify (-1, -1, lLastActiveCharID, lLastActiveCharID);
				}
			}
			catch AnyExceptionDebug
			lLastActive->PostNotify ();
		}

		if	(PreNotify ())
		{
			try
			{
#ifdef	_DEBUG_ACTIVATE
				LogMessage (_DEBUG_ACTIVATE, _T("[%p] SetInputActive [%d] (Activate)"), this, mCharID);
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
			PostNotify ();
		}
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

CAgentBalloonWnd * CAgentCharacterWnd::GetBalloonWnd (bool pCreate)
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

CAgentListeningWnd * CAgentCharacterWnd::GetListeningWnd (bool pCreate)
{
	if	(
			(!mListeningWnd)
		&&	(pCreate)
		&&	(mListeningWnd = CAgentListeningWnd::CreateInstance())
		)
	{
		mListeningWnd->Create (m_hWnd);
	}
	return mListeningWnd;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

long CAgentCharacterWnd::QueueShow (long pCharID, bool pFast, int pVisibilityCause)
{
	long			lReqID = 0;
	CQueuedShow *	lQueuedShow = NULL;

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
		LogMessage (_LOG_POPUP_OPS, _T("[%p] [%d] QueueShow [%p] [%d] [%d]"), this, mCharID, lQueuedShow, pCharID, lReqID);
	}
#endif
	return lReqID;
}

bool CAgentCharacterWnd::DoQueuedShow ()
{
	tPtr <CQueuedShow>	lQueuedShow;

	if	(lQueuedShow = (CQueuedShow *) mQueue.GetNextAction (QueueActionShow))
	{
		bool	lShown = true;

		if	(!lQueuedShow->mStarted)
		{
			lQueuedShow->NotifyStarted (mNotify);
		}
		if	(mQueue.GetNextAction (QueueActionShow) == lQueuedShow)
		{
			if	(!lQueuedShow->mAnimationShown)
			{
				lShown = ShowQueued (lQueuedShow);
			}

			if	(mQueue.GetNextAction (QueueActionShow) == lQueuedShow)
			{
				mQueue.RemoveHead (); // Wait until window visible so (IsWindowVisible || IsShowingQueued) works

				if	(
						(lShown)
					&&	(!lQueuedShow->mFast)
					&&	(!lQueuedShow->mAnimationShown)
					)
				{
					CAtlOwnPtrList <CQueuedAction>	lQueue;

					lQueuedShow->mAnimationShown = true;
					ClearAnimations ();
					mQueue.PushQueue (lQueue);
					if	(ShowState (_T("SHOWING")))
					{
						mQueue.AddTail (lQueuedShow.Detach());
#ifdef	_LOG_QUEUE_OPS
						if	(LogIsActive (_LOG_QUEUE_OPS))
						{
							LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d] Requeue show to end of queue"), this, mCharID);
						}
#endif
					}
					mQueue.PopQueue (lQueue);
				}
				else
				{
					lQueuedShow->mAnimationShown = true;
				}

				if	(lQueuedShow)
				{
					lQueuedShow->NotifyComplete (mNotify);
				}
			}
			else
			{
				lQueuedShow.Detach (); // Was deleted during ShowPopup
			}
		}
		else
		{
			lQueuedShow.Detach (); // Was deleted during NotifyStarted
		}
		return true;
	}
	return false;
}

void CAgentCharacterWnd::AbortQueuedShow (CQueuedAction * pQueuedAction, HRESULT pReqStatus, LPCTSTR pReason)
{
	CQueuedShow *	lQueuedShow;
	CQueuedState *	lQueuedState;

	if	(
			(lQueuedShow = (CQueuedShow *) pQueuedAction)
		&&	(!lQueuedShow->mFast)
		&&	(lQueuedState = (CQueuedState *) mQueue.GetNextAction (QueueActionState))
		&&	(lQueuedState->mStateName.CompareNoCase (_T("SHOWING")) == 0)
		)
	{
		RemoveQueuedAction (lQueuedState, pReqStatus, pReason);
	}
}

/////////////////////////////////////////////////////////////////////////////

CQueuedAction * CAgentCharacterWnd::IsShowQueued (long pCharID)
{
	return mQueue.GetCharAction (QueueActionShow, pCharID);
}

long CAgentCharacterWnd::IsShowingQueued ()
{
	if	(
			(this)
		&&	(IsWindow ())
		&&	(IsShowQueued ())
		&&	(IsShowQueued () == mQueue.FindNextAction ())
		)
	{
		return mCharID;
	}
	return 0;
}

bool CAgentCharacterWnd::RemoveQueuedShow (long pCharID, HRESULT pReqStatus, LPCTSTR pReason, bool pExcludeActive)
{
	bool			lRet = false;
	CQueuedShow *	lQueuedShow;

	while	(
				(lQueuedShow = (CQueuedShow *) mQueue.GetCharAction (QueueActionShow, pCharID, true))
			&&	(
					(!pExcludeActive)
				||	(!lQueuedShow->mStarted)
				)
			&&	(RemoveQueuedAction (lQueuedShow, pReqStatus, pReason))
			)
	{
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentCharacterWnd::ShowQueued (CQueuedShow * pQueuedShow)
{
	if	(IsWindowVisible ())
	{
		mCharShown = true;
		Invalidate ();
		NotifyShown (pQueuedShow->mCharID, (VisibilityCauseType)pQueuedShow->mVisibilityCause);
		SetLastActive ();
		return true;
	}
	return false;
}

bool CAgentCharacterWnd::NotifyShown (long pForCharID, VisibilityCauseType pVisiblityCause)
{
	if	(
			(pVisiblityCause != VisibilityCause_NeverShown)
		&&	(PreNotify ())
		)
	{
		try
		{
			INT_PTR				lNotifyNdx;
			CEventNotify *		lNotify;
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
	CQueuedHide *	lQueuedHide = NULL;

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
		LogMessage (_LOG_POPUP_OPS, _T("[%p] [%d] QueueHide [%p] [%d] [%d]"), this, mCharID, lQueuedHide, pCharID, lReqID);
	}
#endif
	return lReqID;
}

bool CAgentCharacterWnd::DoQueuedHide ()
{
	tPtr <CQueuedHide>	lQueuedHide;

	if	(lQueuedHide = (CQueuedHide *) mQueue.GetNextAction (QueueActionHide))
	{
		if	(!lQueuedHide->mStarted)
		{
			if	(
					(mBalloonWnd)
				&&	(mBalloonWnd->IsWindow ())
				)
			{
				mBalloonWnd->HideBalloon ();
			}
			if	(
					(mListeningWnd)
				&&	(mListeningWnd->IsWindow ())
				&&	(mListeningWnd->GetCharID() == mCharID)
				)
			{
				mListeningWnd->HideTipWnd ();
			}

			lQueuedHide->NotifyStarted (mNotify);
		}
		if	(mQueue.GetNextAction (QueueActionHide) == lQueuedHide)
		{
			if	(
					(!lQueuedHide->mFast)
				&&	(!lQueuedHide->mAnimationShown)
				)
			{
				lQueuedHide->mAnimationShown = true;

				if	(IsWindowVisible ())
				{
					CAtlOwnPtrList <CQueuedAction>	lQueue;

					mQueue.PushQueue (lQueue);
					if	(ShowState (_T("HIDING")))
					{
						lQueue.RemoveHead ();
						mQueue.AddTail (lQueuedHide.Detach());
#ifdef	_LOG_QUEUE_OPS
						if	(LogIsActive (_LOG_QUEUE_OPS))
						{
							LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d] Requeue hide to end of queue"), this, mCharID);
						}
#endif
					}
					mQueue.PopQueue (lQueue);
				}
			}
			else
			{
				lQueuedHide->mAnimationShown = true;
			}
			if	(
					(lQueuedHide)
				&&	(lQueuedHide->mAnimationShown)
				)
			{
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d] Queued hide"), this, mCharID);
				}
#endif
				HideQueued (lQueuedHide);

				if	(mQueue.GetNextAction (QueueActionHide) == lQueuedHide)
				{
					mQueue.RemoveHead ();
					lQueuedHide->NotifyComplete (mNotify);
				}
				else
				{
					lQueuedHide.Detach (); // Was deleted during HidePopup
				}
			}
		}
		else
		{
			lQueuedHide.Detach (); // Was deleted during NotifyStarted
		}
		return true;
	}
	return false;
}

void CAgentCharacterWnd::AbortQueuedHide (CQueuedAction * pQueuedAction, HRESULT pReqStatus, LPCTSTR pReason)
{
	CQueuedHide *	lQueuedHide;
	CQueuedState *	lQueuedState;

	if	(
			(lQueuedHide = (CQueuedHide *) pQueuedAction)
		&&	(lQueuedHide->mAnimationShown)
		&&	(lQueuedState = (CQueuedState *) mQueue.GetNextAction (QueueActionState))
		&&	(lQueuedState->mStateName.CompareNoCase (_T("HIDING")) == 0)
		)
	{
		RemoveQueuedAction (lQueuedState, pReqStatus, pReason);
	}
}

/////////////////////////////////////////////////////////////////////////////

CQueuedAction * CAgentCharacterWnd::IsHideQueued (long pCharID)
{
	return mQueue.GetCharAction (QueueActionHide, pCharID);
}

long CAgentCharacterWnd::IsHidingQueued ()
{
	if	(
			(this)
		&&	(IsWindow ())
		&&	(IsHideQueued ())
		&&	(IsHideQueued () == mQueue.FindNextAction ())
		)
	{
		return mCharID;
	}
	return 0;
}

bool CAgentCharacterWnd::RemoveQueuedHide (long pCharID, HRESULT pReqStatus, LPCTSTR pReason, bool pExcludeActive)
{
	bool			lRet = false;
	CQueuedHide *	lQueuedHide;

	while	(
				(lQueuedHide = (CQueuedHide *) mQueue.GetCharAction (QueueActionHide, pCharID, true))
			&&	(
					(!pExcludeActive)
				||	(!lQueuedHide->mStarted)
				)
			&&	(RemoveQueuedAction (lQueuedHide, pReqStatus, pReason))
			)
	{
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentCharacterWnd::HideQueued (CQueuedHide * pQueuedHide)
{
	mCharShown = false;
	Invalidate ();
	NotifyHidden (GetCharID(), (VisibilityCauseType)pQueuedHide->mVisibilityCause);
	return true;
}

bool CAgentCharacterWnd::NotifyHidden (long pForCharID, VisibilityCauseType pVisiblityCause)
{
	if	(
			(pVisiblityCause != VisibilityCause_NeverShown)
		&&	(PreNotify ())
		)
	{
		try
		{
			INT_PTR				lNotifyNdx;
			CEventNotify *		lNotify;
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
		PostNotify ();
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

long CAgentCharacterWnd::QueueThink (long pCharID, LPCTSTR pText)
{
	long			lReqID = 0;
	CQueuedThink *	lQueuedThink = NULL;

	if	(
			(IsWindow ())
		&&	(lQueuedThink = new CQueuedThink (pCharID, lReqID=NextReqID()))
		)
	{
		lQueuedThink->mText = pText;
		if	(GetBalloonWnd (true))
		{
			lQueuedThink->mBalloonOptions = mBalloonWnd->GetNextOptions ();
		}
		mQueue.AddTail (lQueuedThink);
	}
	else
	{
		lReqID = 0;
	}
#ifdef	_LOG_QUEUE_OPS
	if	(LogIsActive (_LOG_QUEUE_OPS))
	{
		LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d] QueueThink [%p] [%d] [%d] [%s]"), this, mCharID, lQueuedThink, pCharID, lReqID, (lQueuedThink ? (LPCTSTR)DebugStr(lQueuedThink->mText) : NULL));
	}
#endif
	return lReqID;
}

bool CAgentCharacterWnd::DoQueuedThink ()
{
	tPtr <CQueuedThink>	lQueuedThink;

	if	(lQueuedThink = (CQueuedThink *) mQueue.GetNextAction (QueueActionThink))
	{
#ifdef	_LOG_QUEUE_OPS
		if	(LogIsActive (_LOG_QUEUE_OPS))
		{
			LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d] QueuedThink [%p] [%d] [%d] [%s]"), this, mCharID, lQueuedThink.Ptr(), lQueuedThink->mCharID, lQueuedThink->mReqID, DebugStr(lQueuedThink->mText));
		}
#endif
		if	(!lQueuedThink->mStarted)
		{
			if	(
					(mBalloonWnd)
				&&	(mBalloonWnd->IsWindow ())
				&&	(mBalloonWnd->IsBusy (true))
				)
			{
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d] QueuedThink [%p] [%d] [%d] is still busy"), this, mCharID, lQueuedThink.Ptr(), lQueuedThink->mCharID, lQueuedThink->mReqID);
				}
#endif
				lQueuedThink.Detach();
			}
			else
			{
				lQueuedThink->NotifyStarted (mNotify);

				if	(mQueue.GetNextAction (QueueActionThink) == lQueuedThink)
				{
					if	(!IsCharShown ())
					{
						mQueue.RemoveHead ();
						lQueuedThink->NotifyComplete (mNotify, AGENTERR_CHARACTERNOTVISIBLE);
					}
					else
					if	(
							(GetBalloonWnd (true))
						&&	(mBalloonWnd->IsWindow ())
						)
					{
//
//	MS Agent doesn't do this
//
//						ShowGesture (_T("Think"));
#ifdef	_LOG_QUEUE_OPS
						if	(LogIsActive (_LOG_QUEUE_OPS))
						{
							LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d] Show QueuedThink [%p] [%d] [%d] [%s]"), this, mCharID, lQueuedThink.Ptr(), lQueuedThink->mCharID, lQueuedThink->mReqID, DebugStr(lQueuedThink->mText));
						}
#endif
						mBalloonWnd->ApplyOptions (lQueuedThink->mBalloonOptions);
						if	(mBalloonWnd->ShowBalloonThought (lQueuedThink->mText))
						{
							mBalloonWnd->ShowBalloonAuto ();
						}
						lQueuedThink.Detach ();
					}
					else
					{
						mQueue.RemoveHead ();
						lQueuedThink->NotifyComplete (mNotify, AGENTERR_NOBALLOON);
					}
				}
				else
				{
					lQueuedThink.Detach (); // Was deleted during NotifyStarted
				}
			}
		}
		else
		{
			if	(
					(mBalloonWnd)
				&&	(mBalloonWnd->IsWindow ())
				&&	(mBalloonWnd->IsBusy (false))
				)
			{
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d] QueuedThink [%p] [%d] [%d] is busy"), this, mCharID, lQueuedThink.Ptr(), lQueuedThink->mCharID, lQueuedThink->mReqID);
				}
#endif
				lQueuedThink.Detach();
			}
			else
			{
				mQueue.RemoveHead ();

				if	(
						(mBalloonWnd)
					&&	(mBalloonWnd->IsWindow ())
					)
				{
					mBalloonWnd->AbortSpeechText ();
				}
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d] Done QueuedThink [%p] [%d] [%d]"), this, mCharID, lQueuedThink.Ptr(), lQueuedThink->mCharID, lQueuedThink->mReqID);
				}
#endif
				lQueuedThink->NotifyComplete (mNotify);
			}
		}
		return true;
	}
	return false;
}

void CAgentCharacterWnd::AbortQueuedThink (CQueuedAction * pQueuedAction, HRESULT pReqStatus, LPCTSTR pReason)
{
	CQueuedThink *	lQueuedThink;

	if	(
			(lQueuedThink = (CQueuedThink *) pQueuedAction)
		&&	(lQueuedThink->mStarted)
		&&	(mBalloonWnd)
		&&	(mBalloonWnd->IsWindow ())
		)
	{
#ifdef	_STRICT_COMPATIBILITY
		mBalloonWnd->HideBalloon (true);
#else
		mBalloonWnd->AbortSpeechText ();
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////

CQueuedAction * CAgentCharacterWnd::IsThinkQueued (long pCharID)
{
	return mQueue.GetCharAction (QueueActionThink, pCharID);
}

bool CAgentCharacterWnd::RemoveQueuedThink (long pCharID, HRESULT pReqStatus, LPCTSTR pReason, bool pExcludeActive)
{
	bool			lRet = false;
	CQueuedThink *	lQueuedThink;

	while	(
				(lQueuedThink = (CQueuedThink *) mQueue.GetCharAction (QueueActionThink, pCharID, true))
			&&	(
					(!pExcludeActive)
				||	(!lQueuedThink->mStarted)
				)
			&&	(RemoveQueuedAction (lQueuedThink, pReqStatus, pReason))
			)
	{
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

long CAgentCharacterWnd::QueueSpeak (long pCharID, LPCTSTR pText, LPCTSTR pSoundUrl, CSapiVoice * pVoice, bool pShowBalloon)
{
	long			lReqID = 0;
	CQueuedSpeak *	lQueuedSpeak = NULL;
	CAtlString		lText;
	CAtlStringArray	lTextParts;
#ifdef	DebugTimeStart
	DebugTimeStart
#endif

	if	(
			(IsWindow ())
		&&	(lQueuedSpeak = new CQueuedSpeak (pVoice, pShowBalloon, pCharID, lReqID=NextReqID()))
		)
	{
		lQueuedSpeak->mSoundUrl = pSoundUrl;

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
			lQueuedSpeak->mText.SplitText (lText, lTextParts);
			lQueuedSpeak->mText = lTextParts;
		}
		else
		{
			if	(lText.CompareNoCase (_T("\\lst\\")) == 0)
			{
				lQueuedSpeak->mText = mLastSpeech;
			}
			else
			{
				lQueuedSpeak->mText = lText;
			}
		}
		if	(lQueuedSpeak->mText.GetFullText().IsEmpty ())
		{
			lQueuedSpeak->mShowBalloon = false;
		}
		if	(
				(lQueuedSpeak->mShowBalloon)
			&&	(GetBalloonWnd (true))
			)
		{
			lQueuedSpeak->mBalloonOptions = mBalloonWnd->GetNextOptions ();
		}

#ifdef	_DEBUG_SPEECH
		if	(
				(lQueuedSpeak->mSoundUrl.IsEmpty ())
			&&	(LogIsActive (_DEBUG_SPEECH))
			)
		{
			LogMessage (_DEBUG_SPEECH, _T("[%p] [%d] CAgentCharacterWnd Queue   [%s]"), this, mCharID, DebugStr(pText));
			LogMessage (_DEBUG_SPEECH, _T("[%p] [%d]                    Speech  [%s]"), this, mCharID, DebugStr(lQueuedSpeak->mText.GetSpeechText()));
			LogMessage (_DEBUG_SPEECH, _T("[%p] [%d]                    Text    [%s]"), this, mCharID, DebugStr(lQueuedSpeak->mText.GetFullText()));
			LogMessage (_DEBUG_SPEECH, _T("[%p] [%d]                    Voice   [%u] Busy [%u] Balloon [%u] Busy [%u]"), this, mCharID, lQueuedSpeak->mVoice->SafeIsValid(), lQueuedSpeak->mVoice->SafeIsSpeaking (), (mBalloonWnd!=NULL), ((mBalloonWnd!= NULL) && mBalloonWnd->IsBusy (false)));
			LogMessage (_DEBUG_SPEECH, _T("[%p] [%d]                    Queue   [%u] Busy [%u %u]"), this, mCharID, mQueue.GetCount(), IsQueueBusy(), !IsAnimationComplete());
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
		LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d] QueueSpeak [%p] [%d] [%d] [%s] [%s]"), this, mCharID, lQueuedSpeak, pCharID, lReqID, DebugStr(pText), DebugStr(pSoundUrl));
	}
#endif

#ifdef	DebugTimeStart
	DebugTimeStop
	LogMessage (LogIfActive|LogHighVolume|LogTimeMs, _T("%f CAgentCharacterWnd::QueueSpeak"), DebugTimeElapsed);
#endif
	return lReqID;
}

bool CAgentCharacterWnd::DoQueuedSpeak ()
{
	tPtr <CQueuedSpeak>	lQueuedSpeak;
	HRESULT				lResult = S_OK;

	if	(lQueuedSpeak = (CQueuedSpeak *) mQueue.GetNextAction (QueueActionSpeak))
	{
#ifdef	DebugTimeStart
		DebugTimeStart
#endif
#ifdef	_LOG_QUEUE_OPS
		if	(LogIsActive (_LOG_QUEUE_OPS))
		{
			LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d] QueuedSpeak [%p] [%d] [%d] [%s] [%s]"), this, mCharID, lQueuedSpeak.Ptr(), lQueuedSpeak->mCharID, lQueuedSpeak->mReqID, DebugStr(lQueuedSpeak->mText.GetFullText()), DebugStr(lQueuedSpeak->mSoundUrl));
		}
#endif
		if	(!lQueuedSpeak->mStarted)
		{
			lResult = SpeechIsReady (lQueuedSpeak);

			if	(lResult == S_FALSE)
			{
				lQueuedSpeak.Detach();
			}
		}

		if	(
				(lResult == S_OK)
			&&	(lQueuedSpeak->mStarted)
			)
		{
			if	(SpeechIsBusy (lQueuedSpeak))
			{
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d] QueuedSpeak [%p] [%d] [%d] is busy"), this, mCharID, lQueuedSpeak.Ptr(), lQueuedSpeak->mCharID, lQueuedSpeak->mReqID);
				}
#endif
				lQueuedSpeak.Detach();
			}
		}
		else
		if	(
				(lResult == S_OK)
			&&	(!lQueuedSpeak->mStarted)
			)
		{
			if	(SpeechIsBusy (lQueuedSpeak))
			{
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d] QueuedSpeak [%p] [%d] [%d] still busy"), this, mCharID, lQueuedSpeak.Ptr(), lQueuedSpeak->mCharID, lQueuedSpeak->mReqID);
				}
#endif
				lQueuedSpeak.Detach();
			}
			else
			if	(
					(!lQueuedSpeak->mAnimated)
				&&	(ShowSpeechAnimation (lQueuedSpeak))
				)
			{
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d] QueuedSpeak [%p] [%d] [%d] animated"), this, mCharID, lQueuedSpeak.Ptr(), lQueuedSpeak->mCharID, lQueuedSpeak->mReqID);
				}
#endif
				lQueuedSpeak.Detach();
			}
			else
			if	(
					(lQueuedSpeak->mAnimated)
				&&	(FAILED (lResult = PrepareSpeech (lQueuedSpeak)))
				)
			{
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d] QueuedSpeak [%p] [%d] [%d] failed"), this, mCharID, lQueuedSpeak.Ptr(), lQueuedSpeak->mCharID, lQueuedSpeak->mReqID);
				}
#endif
			}
			else
			{
				lQueuedSpeak->NotifyStarted (mNotify);

				if	(mQueue.GetNextAction (QueueActionSpeak) == lQueuedSpeak)
				{
					if	(!IsCharShown ())
					{
						lResult = AGENTERR_CHARACTERNOTVISIBLE;
					}
					else
					{
#ifdef	_LOG_QUEUE_OPS
						if	(LogIsActive (_LOG_QUEUE_OPS))
						{
							LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d] Show QueuedSpeak [%p] [%d] [%d] [%s] [%s]"), this, mCharID, lQueuedSpeak.Ptr(), lQueuedSpeak->mCharID, lQueuedSpeak->mReqID, DebugStr(lQueuedSpeak->mText.GetFullText()), DebugStr(lQueuedSpeak->mSoundUrl));
						}
#endif
						mLastSpeech = lQueuedSpeak->mText;
						if	(SUCCEEDED (lResult = StartSpeech (lQueuedSpeak)))
						{
							lQueuedSpeak.Detach();
						}
					}
				}
				else
				{
					lQueuedSpeak.Detach (); // Was deleted during NotifyStarted
				}
			}
		}

		if	(
				(lQueuedSpeak)
			&&	(mQueue.GetNextAction (QueueActionSpeak) == lQueuedSpeak)
			)
		{
			CDirectSoundLipSync *	lLipSync;
#ifdef	_DEBUG_SPEECH
			if	(LogIsActive (_DEBUG_SPEECH))
			{
				LogMessage (_DEBUG_SPEECH, _T("[%p] EndQueuedSpeak [%d] [%d] Started [%u] Animated [%u] Balloon [%u]"), this, lQueuedSpeak->mCharID, lQueuedSpeak->mReqID, lQueuedSpeak->mStarted, lQueuedSpeak->mAnimated, lQueuedSpeak->mShowBalloon);
			}
#endif
			mQueue.RemoveHead ();

			if	(lQueuedSpeak->mVoice)
			{
				lQueuedSpeak->mVoice->RemoveEventSink (this);
			}
			if	(
					(!lQueuedSpeak->mSoundUrl.IsEmpty ())
				&&	(lLipSync = static_cast <CDirectSoundLipSync *> (lQueuedSpeak->mSoundFilter.GetInterfacePtr()))
				)
			{
				lLipSync->Disconnect ();
			}
			if	(
					(lQueuedSpeak->mShowBalloon)
				&&	(mBalloonWnd)
				&&	(mBalloonWnd->IsWindow ())
				)
			{
				mBalloonWnd->AbortSpeechText ();
			}
			StopMouthAnimation ();

#ifdef	_LOG_QUEUE_OPS
			if	(LogIsActive (_LOG_QUEUE_OPS))
			{
				LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d] Done QueuedSpeak [%p] [%d] [%d] [%s] [%s] [%8.8X]"), this, mCharID, lQueuedSpeak.Ptr(), lQueuedSpeak->mCharID, lQueuedSpeak->mReqID, DebugStr(lQueuedSpeak->mText.GetFullText()), DebugStr(lQueuedSpeak->mSoundUrl), lResult);
			}
#endif
			lQueuedSpeak->NotifyComplete (mNotify, lResult);
		}
#ifdef	DebugTimeStart
		DebugTimeStop
		LogMessage (LogIfActive|LogHighVolume|LogTimeMs, _T("%f CAgentCharacterWnd::DoQueuedSpeak"), DebugTimeElapsed);
#endif
		return true;
	}
	return false;
}

void CAgentCharacterWnd::AbortQueuedSpeak (CQueuedAction * pQueuedAction, HRESULT pReqStatus, LPCTSTR pReason)
{
	CQueuedSpeak *	lQueuedSpeak;

	if	(lQueuedSpeak = (CQueuedSpeak *) pQueuedAction)
	{
#ifdef	_DEBUG_SPEECH
		if	(LogIsActive (_DEBUG_SPEECH))
		{
			LogMessage (_DEBUG_SPEECH, _T("[%p] AbortQueuedSpeak [%d] [%d] Started [%u] Animated [%u] Balloon [%u]"), this, lQueuedSpeak->mCharID, lQueuedSpeak->mReqID, lQueuedSpeak->mStarted, lQueuedSpeak->mAnimated, lQueuedSpeak->mShowBalloon);
		}
#endif
		if	(lQueuedSpeak->mStarted)
		{
			CDirectSoundLipSync *	lLipSync;

			if	(lQueuedSpeak->mVoice->SafeIsValid())
			{
				lQueuedSpeak->mVoice->Stop ();
				lQueuedSpeak->mVoice->ClearEventSinks ();
			}
			if	(
					(!lQueuedSpeak->mSoundUrl.IsEmpty ())
				&&	(lLipSync = static_cast <CDirectSoundLipSync *> (lQueuedSpeak->mSoundFilter.GetInterfacePtr()))
				)
			{
				lLipSync->Stop ();
				lLipSync->Disconnect ();
			}
			StopMouthAnimation ();

			if	(
					(lQueuedSpeak->mShowBalloon)
				&&	(mBalloonWnd)
				&&	(mBalloonWnd->IsWindow ())
				)
			{
#ifdef	_STRICT_COMPATIBILITY
				mBalloonWnd->HideBalloon (true);
#else
				mBalloonWnd->AbortSpeechText ();
#endif
			}
		}
		else
		if	(lQueuedSpeak->mAnimated)
		{
			if	(!ShowGesture (NULL))
			{
				Stop ();
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentCharacterWnd::SpeechIsBusy (CQueuedSpeak * pQueuedSpeak)
{
	bool	lRet = false;
#ifdef	DebugTimeStart
	DebugTimeStart
#endif

	if	(pQueuedSpeak)
	{
		if	(pQueuedSpeak->mStarted)
		{
			if	(
					(
						(pQueuedSpeak->mSoundUrl.IsEmpty ())
					&&	(pQueuedSpeak->mVoice->SafeIsValid ())
					&&	(pQueuedSpeak->mVoice->SafeIsSpeaking ())
					)
				||	(
						(pQueuedSpeak->mShowBalloon)
					&&	(mBalloonWnd)
					&&	(mBalloonWnd->IsWindow ())
					&&	(mBalloonWnd->IsBusy (false))
					)
				)
			{
				lRet = true;
			}
		}
		else
		{
			if	(
					(
						(pQueuedSpeak->mVoice->SafeIsValid ())
					&&	(pQueuedSpeak->mVoice->SafeIsSpeaking ())
					)
				||	(
						(!pQueuedSpeak->mSoundUrl.IsEmpty ())
					&&	(PathIsURL (pQueuedSpeak->mSoundUrl))
					)
				||	(
						(pQueuedSpeak->mShowBalloon)
					&&	(mBalloonWnd)
					&&	(mBalloonWnd->IsWindow ())
#ifdef	_STRICT_COMPATIBILITY
					&&	(mBalloonWnd->IsBusy (false))
#else
					&&	(mBalloonWnd->IsBusy (mBalloonWnd->IsAutoSize()))
#endif
					)
				)
			{
				lRet = true;
			}
		}
	}

#ifdef	DebugTimeStart
	DebugTimeStop
	LogMessage (LogIfActive|LogHighVolume|LogTimeMs, _T("%f   CAgentCharacterWnd::SpeechIsBusy [%u]"), DebugTimeElapsed, lRet);
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CAgentCharacterWnd::SpeechIsReady (CQueuedSpeak * pQueuedSpeak)
{
	HRESULT	lResult = S_OK;
#ifdef	DebugTimeStart
	DebugTimeStart
#endif

	if	(pQueuedSpeak)
	{
		if	(!pQueuedSpeak->mSoundUrl.IsEmpty ())
		{
			if	(PathIsURL (pQueuedSpeak->mSoundUrl))
			{
				INT_PTR				lNotifyNdx;
				CEventNotify *		lNotify;
				CFileDownload *		lSoundDownload = NULL;

				for	(lNotifyNdx = 0; lNotifyNdx < (INT_PTR)mNotify.GetCount(); lNotifyNdx++)
				{
					if	(
							(lNotify = mNotify [lNotifyNdx])
						&&	(lSoundDownload = lNotify->_FindSoundDownload (pQueuedSpeak->mSoundUrl))
						)
					{
						break;
					}
				}

				if	(
						(lSoundDownload)
					&&	(lSoundDownload->IsDownloadComplete () == S_OK)
					)
				{
					pQueuedSpeak->mSoundUrl = lSoundDownload->GetCacheName ();
				}
				else
				if	(
						(lSoundDownload)
					&&	(
							(lSoundDownload->IsDownloadStarted ())
						||	(lSoundDownload->IsDownloadStarting ())
						)
					&&	(!lSoundDownload->IsDownloadCancelling ())
					)
				{
#ifdef	_LOG_QUEUE_OPS
					if	(LogIsActive (_LOG_QUEUE_OPS))
					{
						LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d] QueuedSpeak [%p] [%d] [%d] waiting for [%s]"), this, mCharID, pQueuedSpeak, pQueuedSpeak->mCharID, pQueuedSpeak->mReqID, DebugStr(pQueuedSpeak->mSoundUrl));
					}
#endif
					lResult = S_FALSE;
				}
				else
				{
#ifdef	_LOG_QUEUE_OPS
					if	(LogIsActive (_LOG_QUEUE_OPS))
					{
						LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d] QueuedSpeak [%p] [%d] [%d] failed for [%s]"), this, mCharID, pQueuedSpeak, pQueuedSpeak->mCharID, pQueuedSpeak->mReqID, DebugStr(pQueuedSpeak->mSoundUrl));
					}
#endif
					lResult = AGENTPROVERROR_WAVEINVALID;
				}
			}
		}
		else
		if	(!pQueuedSpeak->mVoice->SafeIsValid())
		{
			lResult = AGENTWARNING_TTSENGINENOTFOUND;
		}
	}

#ifdef	DebugTimeStart
	DebugTimeStop
	LogMessage (LogIfActive|LogHighVolume|LogTimeMs, _T("%f   CAgentCharacterWnd::SpeechIsReady [%8.8X]"), DebugTimeElapsed, lResult);
#endif
	return lResult;
}

HRESULT CAgentCharacterWnd::PrepareSpeech (CQueuedSpeak * pQueuedSpeak)
{
	HRESULT	lResult = S_FALSE;
#ifdef	DebugTimeStart
	DebugTimeStart
#endif

	if	(pQueuedSpeak)
	{
		if	(!pQueuedSpeak->mSoundUrl.IsEmpty ())
		{
			if	(!pQueuedSpeak->mSoundFilter)
			{
				tPtr <CComObject <CDirectSoundLipSync> >	lLipSync;

				if	(SUCCEEDED (CComObject <CDirectSoundLipSync>::CreateInstance (lLipSync.Free())))
				{
					lResult = lLipSync->Connect (mGraphBuilder, pQueuedSpeak->mSoundUrl, GetAgentStreamInfo());
					if	(SUCCEEDED (lResult))
					{
						pQueuedSpeak->mSoundFilter = lLipSync.Detach ();
					}
				}
				else
				{
					lResult = E_OUTOFMEMORY;
				}
			}
		}
		else
		if	(pQueuedSpeak->mVoice->SafeIsValid())
		{
#ifndef	_WIN64
			CSapi4Voice *	lSapi4Voice;
			CAgentFile *	lAgentFile;

			if	(lSapi4Voice = dynamic_cast <CSapi4Voice *> (pQueuedSpeak->mVoice))
			{
				if	(
						(lAgentFile = GetAgentFile())
					&&	(lAgentFile->GetTts().mPitch > 0)
					)
				{
					LogSapi4Err (LogNormal, lSapi4Voice->SetPitch (lAgentFile->GetTts().mPitch));
				}
				if	(
						(lAgentFile)
					&&	(lAgentFile->GetTts().mSpeed > 0)
					)
				{
					LogSapi4Err (LogNormal, lSapi4Voice->SetRate (CDaSettingsConfig().ApplyVoiceRate (lAgentFile->GetTts().mSpeed, 4)));
				}
				else
				{
					LogSapi4Err (LogNormal, lSapi4Voice->SetRate (CDaSettingsConfig().ApplyVoiceRate (lSapi4Voice->GetDefaultRate(), 4)));
				}
			}
			else
#endif
			{
				LogSapi5Err (LogNormal, pQueuedSpeak->mVoice->SetRate (CDaSettingsConfig().CalcVoiceRate()));
			}
			lResult = S_OK;
		}
	}

#ifdef	DebugTimeStart
	DebugTimeStop
	LogMessage (LogIfActive|LogHighVolume|LogTimeMs, _T("%f   CAgentCharacterWnd::PrepareSpeech"), DebugTimeElapsed);
#endif
	return lResult;
}

HRESULT CAgentCharacterWnd::StartSpeech (CQueuedSpeak * pQueuedSpeak)
{
	HRESULT	lResult = S_FALSE;
#ifdef	DebugTimeStart
	DebugTimeStart
#endif

	if	(pQueuedSpeak)
	{
		bool					lBalloonShown = false;
		CDirectSoundLipSync *	lLipSync;

		if	(!pQueuedSpeak->mSoundUrl.IsEmpty ())
		{
			if	(lLipSync = static_cast <CDirectSoundLipSync *> (pQueuedSpeak->mSoundFilter.GetInterfacePtr()))
			{
				StartMouthAnimation ((long)(lLipSync->GetDuration() / MsPer100Ns));
			}
			if	(
					(mQueue.GetNextAction (QueueActionSpeak) == pQueuedSpeak)
				&&	(pQueuedSpeak->mShowBalloon)
				&&	(GetBalloonWnd (true))
				&&	(mBalloonWnd->IsWindow ())
				)
			{
				mBalloonWnd->ApplyOptions (pQueuedSpeak->mBalloonOptions);
				lBalloonShown = mBalloonWnd->ShowBalloonSpeech (pQueuedSpeak->mText, true);
			}

			if	(mQueue.GetNextAction (QueueActionSpeak) == pQueuedSpeak)
			{
#ifdef	_DEBUG_SPEECH
				if	(LogIsActive (_DEBUG_SPEECH))
				{
					LogMessage (_DEBUG_SPEECH, _T("[%p] [%d] CAgentCharacterWnd Speak [%s] [%s]"), this, mCharID, DebugStr(pQueuedSpeak->mText.GetFullText()), DebugStr(pQueuedSpeak->mSoundUrl));
				}
#endif
				lResult = PlayFromTo (0, GetDurationMs(), true);
			}
		}
		else
		if	(pQueuedSpeak->mVoice->SafeIsValid())
		{
			pQueuedSpeak->mVoice->ClearEventSinks ();
			pQueuedSpeak->mVoice->AddEventSink (this);
			pQueuedSpeak->mVoice->SetEventCharID (pQueuedSpeak->mCharID);

			if	(
					(mQueue.GetNextAction (QueueActionSpeak) == pQueuedSpeak)
				&&	(pQueuedSpeak->mShowBalloon)
				&&	(GetBalloonWnd (true))
				&&	(mBalloonWnd->IsWindow ())
				)
			{
				pQueuedSpeak->mVoice->AddEventSink (mBalloonWnd);
				mBalloonWnd->ApplyOptions (pQueuedSpeak->mBalloonOptions);
				lBalloonShown = mBalloonWnd->ShowBalloonSpeech (pQueuedSpeak->mText);
			}

			if	(mQueue.GetNextAction (QueueActionSpeak) == pQueuedSpeak)
			{
#ifdef	_DEBUG_SPEECH
				if	(LogIsActive (_DEBUG_SPEECH))
				{
					LogMessage (_DEBUG_SPEECH, _T("[%p] [%d] CAgentCharacterWnd Speak   [%s]"), this, mCharID, DebugStr(pQueuedSpeak->mText.GetSpeechText()));
					LogMessage (_DEBUG_SPEECH, _T("[%p] [%d]                    Text    [%s]"), this, mCharID, DebugStr(pQueuedSpeak->mText.GetFullText()));
					LogMessage (_DEBUG_SPEECH, _T("[%p] [%d]                    Voice   [%u] Rate [%u]"), this, mCharID, pQueuedSpeak->mVoice->SafeIsValid (), pQueuedSpeak->mVoice->GetRate());
					if	(
							(pQueuedSpeak->mShowBalloon)
						&&	(mBalloonWnd)
						&&	(mBalloonWnd->IsWindow ())
						)
					{
						LogMessage (_DEBUG_SPEECH, _T("[%p] [%d]                    Balloon [%u] AutoSize [%u] AutoPace [%u] AutoHide [%u]"), this, mCharID, mBalloonWnd->IsWindowVisible(), mBalloonWnd->IsAutoSize(), mBalloonWnd->IsAutoPace(), mBalloonWnd->IsAutoHide());
					}
				}
#endif
				lResult = LogComErr (LogNormal, pQueuedSpeak->mVoice->Speak (pQueuedSpeak->mText.GetSpeechText()));
			}
		}
		else
		{
			lResult = E_FAIL;
		}

		if	(FAILED (lResult))
		{
			if	(
					(mBalloonWnd)
				&&	(mBalloonWnd->IsWindow ())
				)
			{
				mBalloonWnd->AbortSpeechText ();
				mBalloonWnd->HideBalloon (true);
			}
		}
		else
		if	(lBalloonShown)
		{
			mBalloonWnd->ShowBalloonAuto ();
		}
	}

#ifdef	DebugTimeStart
	DebugTimeStop
	LogMessage (LogIfActive|LogHighVolume|LogTimeMs, _T("%f   CAgentCharacterWnd::StartSpeech"), DebugTimeElapsed);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentCharacterWnd::ShowSpeechAnimation (CQueuedSpeak * pQueuedSpeak)
{
	bool	lRet = false;
#ifdef	DebugTimeStart
	DebugTimeStart
#endif

	if	(
			(pQueuedSpeak)
		&&	(!pQueuedSpeak->mAnimated)
		)
	{
		CAgentStreamInfo *		lStreamInfo;
		long					lAnimationNdx = -1;
		long					lSpeakingFrameNdx = -1;
		const CAtlStringArray *	lGestures;
		LPCTSTR					lGesture;

		mQueue.RemoveHead ();
		pQueuedSpeak->mAnimated = true;

		if	(
				(lStreamInfo = GetAgentStreamInfo())
			&&	(lStreamInfo->GetAnimationIndex (&lAnimationNdx) == S_OK)
			&&	(lAnimationNdx >= 0)
			&&	(lStreamInfo->FindAnimationSpeakingFrame (&lSpeakingFrameNdx, lAnimationNdx) == S_OK)
			&&	(lSpeakingFrameNdx >= 0)
			)
		{
			mQueue.AddHead (pQueuedSpeak);
#ifdef	_DEBUG_SPEECH
			if	(LogIsActive (_DEBUG_SPEECH))
			{
				LogMessage (_DEBUG_SPEECH, _T("[%p] [%d]   Speech Animation [%d] [%ls] Frame [%d]"), this, mCharID, lAnimationNdx, (BSTR)(GetAgentFile()->GetAnimation (lAnimationNdx)->mName), lSpeakingFrameNdx);
			}
#endif
		}
		else
		if	(
				(lGestures = GetFileStates() (_T("SPEAKING")))
			&&	(lGestures->GetCount() > 0)
			)
		{
			CAtlOwnPtrList <CQueuedAction>	lQueue;

			mQueue.PushQueue (lQueue);
			ShowState (_T("SPEAKING"));
			mQueue.AddTail (pQueuedSpeak);
			mQueue.PopQueue (lQueue);
#ifdef	_DEBUG_SPEECH
			if	(LogIsActive (_DEBUG_SPEECH))
			{
				LogMessage (_DEBUG_SPEECH, _T("[%p] [%d]   Showed SPEAKING state"), this, mCharID);
			}
#endif
			lRet = true;
		}
		else
		if	(
				(ShowGesture (lGesture = _T("RestPose")))
			||	(ShowGesture (lGesture = _T("Explain")))
			)
		{
			mQueue.AddHead (pQueuedSpeak);
#ifdef	_DEBUG_SPEECH
			if	(LogIsActive (_DEBUG_SPEECH))
			{
				LogMessage (_DEBUG_SPEECH, _T("[%p] [%d]   Showed %s for speech"), this, mCharID, lGesture);
			}
#endif
			lRet = true;
		}
		else
		{
			mQueue.AddHead (pQueuedSpeak);
		}
	}

#ifdef	DebugTimeStart
	DebugTimeStop
	LogMessage (LogIfActive|LogHighVolume|LogTimeMs, _T("%f   CAgentCharacterWnd::ShowSpeechAnimation"), DebugTimeElapsed);
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentCharacterWnd::StartMouthAnimation (long pSpeakingDuration)
{
	bool				lRet = false;
	CAgentFile *		lAgentFile;
	CAgentStreamInfo *	lStreamInfo;
	long				lAnimationNdx = -1;
	long				lSpeakingFrameNdx = -1;

	if	(
			(lAgentFile = GetAgentFile())
		&&	(lStreamInfo = GetAgentStreamInfo())
		)
	{
#ifdef	_DEBUG_SPEECH
		if	(
				(LogIsActive (_DEBUG_SPEECH))
			&&	(lStreamInfo->GetAnimationIndex (&lAnimationNdx) == S_OK)
			&&	(lAnimationNdx >= 0)
			)
		{
			LogMessage (_DEBUG_SPEECH, _T("[%p] [%d]   Last Animation [%d] [%ls]"), this, mCharID, lAnimationNdx, (BSTR)(lAgentFile->GetAnimation (lAnimationNdx)->mName));
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
#ifdef	_DEBUG_SPEECH
				if	(LogIsActive (_DEBUG_SPEECH))
				{
					LogMessage (_DEBUG_SPEECH, _T("[%p] [%d]   Speech MouthAnimation [%d] [%ls] Frame [%d] started [%d]"), this, mCharID, lAnimationNdx, (BSTR)(lAgentFile->GetAnimation (lAnimationNdx)->mName), lSpeakingFrameNdx, pSpeakingDuration);
				}
#endif
				PlayMouthAnimation (-1, true);
				lRet = true;
			}
		}
	}
	return lRet;
}

bool CAgentCharacterWnd::StopMouthAnimation ()
{
	bool				lRet;
	CAgentStreamInfo *	lStreamInfo;

	lRet = PlayMouthAnimation (-1, true);

	if	(
			(lStreamInfo = GetAgentStreamInfo())
		&&	(lStreamInfo->SetSpeakingDuration (0))
		)
	{
		lStreamInfo->ResetMouthOverlays ();
		AnimationSequenceChanged ();
#ifdef	_DEBUG_SPEECH
		if	(LogIsActive (_DEBUG_SPEECH))
		{
			LogMessage (_DEBUG_SPEECH, _T("[%p] [%d]   Speech MouthAnimation stopped"), this, mCharID);
		}
#endif
		lRet = true;
	}
	return lRet;
}

bool CAgentCharacterWnd::PlayMouthAnimation (short pMouthOverlayNdx, bool pPlayAlways)
{
	bool				lRet = false;
	CAgentStreamInfo *	lStreamInfo;
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

#ifdef	_DEBUG_SPEECH
			if	(LogIsActive (_DEBUG_SPEECH))
			{
				LogMessage (_DEBUG_SPEECH, _T("[%p] [%d]   Speech MouthAnimation [%d] from [%d] to [%d]"), this, mCharID, pMouthOverlayNdx, lStartPosition, lStopPosition);
			}
#endif
			if	(SUCCEEDED (PlayFromTo (lStartPosition, lStopPosition, (pMouthOverlayNdx < 0))))
			{
				lRet = true;
			}
		}
	}
#ifdef	DebugTimeStart
	DebugTimeStop
	LogMessage (LogIfActive|LogHighVolume|LogTimeMs, _T("%f   CAgentCharacterWnd::PlayMouthAnimation"), DebugTimeElapsed);
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

CQueuedAction * CAgentCharacterWnd::IsSpeakQueued (long pCharID)
{
	return mQueue.GetCharAction (QueueActionSpeak, pCharID);
}

bool CAgentCharacterWnd::RemoveQueuedSpeak (long pCharID, HRESULT pReqStatus, LPCTSTR pReason, bool pExcludeActive)
{
	bool			lRet = false;
	CQueuedSpeak *	lQueuedSpeak;

#ifdef	_DEBUG_SPEECH
	if	(LogIsActive (_DEBUG_SPEECH))
	{
		LogMessage (_DEBUG_SPEECH, _T("[%p] RemoveQueuedSpeak for [%d] [%8.8X] [%s]"), this, pCharID, pReqStatus, pReason);
	}
#endif
	while	(
				(lQueuedSpeak = (CQueuedSpeak *) mQueue.GetCharAction (QueueActionSpeak, pCharID, true))
			&&	(
					(!pExcludeActive)
				||	(!lQueuedSpeak->mStarted)
				)
			&&	(RemoveQueuedAction (lQueuedSpeak, pReqStatus, pReason))
			)
	{
		lRet = true;
	}
	return lRet;
}

bool CAgentCharacterWnd::UpdateQueuedSpeak (long pCharID, CSapiVoice * pVoice)
{
	bool			lRet = false;
	POSITION		lPos;
	CQueuedAction *	lQueuedAction;
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

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentCharacterWnd::KeepBalloonVisible (CAgentBalloonWnd * pBalloon)
{
	bool				lRet = false;
	CQueuedAction *		lQueuedAction;
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
			&&	(lQueuedSpeak->mShowBalloon)
			&&	(
					(!lQueuedSpeak->mStarted)
				||	(
						(lQueuedSpeak->mStarted)
					&&	(lPos)
					&&	(lQueuedAction = mQueue.GetNext (lPos))
					&&	(lQueuedAction->mAction == QueueActionSpeak)
					&&	(lQueuedSpeak = (CQueuedSpeak *) lQueuedAction)
					&&	(lQueuedSpeak->mShowBalloon)
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
		LogMessage (_DEBUG_SPEECH_EVENTS, _T("[%p] [%d] CAgentCharacterWnd   OnVoiceStart"), this, mCharID);
	}
#endif
	PostMessage (mVoiceStartMsg, pCharID);
}

void CAgentCharacterWnd::OnVoiceEnd (long pCharID)
{
#ifdef	_DEBUG_SPEECH_EVENTS
	if	(LogIsActive (_DEBUG_SPEECH_EVENTS))
	{
		LogMessage (_DEBUG_SPEECH_EVENTS, _T("[%p] [%d] CAgentCharacterWnd   OnVoiceEnd"), this, mCharID);
	}
#endif
	PostMessage (mVoiceEndMsg, pCharID);
}

void CAgentCharacterWnd::OnVoiceBookMark (long pCharID, long pBookMarkId)
{
#ifdef	_DEBUG_SPEECH_EVENTS
	if	(LogIsActive (_DEBUG_SPEECH_EVENTS))
	{
		LogMessage (_DEBUG_SPEECH_EVENTS, _T("[%p] [%d] CAgentCharacterWnd   OnVoiceBookMark [%d] [%d]"), this, mCharID, pCharID, pBookMarkId);
	}
#endif
	PostMessage (mVoiceBookMarkMsg, pCharID, pBookMarkId);
}

void CAgentCharacterWnd::OnVoiceVisual (long pCharID, int pMouthOverlay)
{
#ifdef	_DEBUG_SPEECH_EVENTS
	if	(LogIsActive (_DEBUG_SPEECH))
	{
		LogMessage (_DEBUG_SPEECH_EVENTS, _T("[%p] [%d] CAgentCharacterWnd     OnVoiceVisual [%s]"), this, mCharID, MouthOverlayStr(pMouthOverlay));
	}
#endif
	PostMessage (mVoiceVisualMsg, pCharID, pMouthOverlay);
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CAgentCharacterWnd::OnVoiceStartMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
#ifdef	_DEBUG_SPEECH_EVENTS
	if	(LogIsActive (_DEBUG_SPEECH_EVENTS))
	{
		LogMessage (_DEBUG_SPEECH_EVENTS, _T("[%p] [%d] CAgentCharacterWnd   OnVoiceStartMsg"), this, mCharID);
	}
#endif
	StartMouthAnimation ();
	return 0;
}

LRESULT CAgentCharacterWnd::OnVoiceEndMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
#ifdef	_DEBUG_SPEECH_EVENTS
	if	(LogIsActive (_DEBUG_SPEECH_EVENTS))
	{
		LogMessage (_DEBUG_SPEECH_EVENTS, _T("[%p] [%d] CAgentCharacterWnd   OnVoiceEndMsg"), this, mCharID);
	}
#endif
	StopMouthAnimation ();
	return 0;
}

LRESULT CAgentCharacterWnd::OnVoiceBookMarkMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	long	lCharID = (long)wParam;
	long	lBookMarkId = (long)lParam;
#ifdef	_DEBUG_SPEECH_EVENTS
	if	(LogIsActive (_DEBUG_SPEECH_EVENTS))
	{
		LogMessage (_DEBUG_SPEECH_EVENTS, _T("[%p] [%d] CAgentCharacterWnd   OnVoiceBookMarkMsg [%d] [%d]"), this, mCharID, lCharID, lBookMarkId);
	}
#endif
	if	(PreNotify ())
	{
		try
		{
			int				lNotifyNdx;
			CEventNotify *	lNotify;

			for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
			{
				if	(lNotify->GetNotifyClient (mCharID) == lCharID)
				{
					lNotify->BookMark (lBookMarkId);
				}
			}
		}
		catch AnyExceptionDebug
		PostNotify ();
	}
	return 0;
}

LRESULT CAgentCharacterWnd::OnVoiceVisualMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	int	lMouthOverlay = (int)lParam;
#ifdef	_DEBUG_SPEECH_EVENTS
	if	(LogIsActive (_DEBUG_SPEECH))
	{
		LogMessage (_DEBUG_SPEECH_EVENTS, _T("[%p] [%d] CAgentCharacterWnd     OnVoiceVisualMsg [%s]"), this, mCharID, MouthOverlayStr(lMouthOverlay));
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
			LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d] QueueWait [%p] [%d] [%d] Other [%d] [%d]"), this, mCharID, lQueuedWait, pCharID, lReqID, pOtherCharID, pOtherReqID);
		}
#endif
	}
	else
	{
		lReqID = 0;
	}
	return lReqID;
}

bool CAgentCharacterWnd::DoQueuedWait ()
{
	tPtr <CQueuedWait>	lQueuedWait;

	if	(lQueuedWait = (CQueuedWait *) mQueue.GetNextAction (QueueActionWait))
	{
		CQueuedAction *		lOtherRequest;
		CAgentCharacterWnd *	lRequestOwner = NULL;

#ifdef	_LOG_QUEUE_OPS
		if	(LogIsActive (_LOG_QUEUE_OPS))
		{
			LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d] QueuedWait [%p] [%d] [%d] Other [%d] [%d]"), this, mCharID, lQueuedWait.Ptr(), lQueuedWait->mCharID, lQueuedWait->mReqID, lQueuedWait->mOtherCharID, lQueuedWait->mOtherReqID);
		}
#endif

		if	(!lQueuedWait->mStarted)
		{
			lQueuedWait->NotifyStarted (mNotify);
		}
		if	(mQueue.GetNextAction (QueueActionWait) == lQueuedWait)
		{
			mQueue.RemoveHead ();

			if	(
					(lOtherRequest = FindOtherRequest (lQueuedWait->mOtherReqID, lRequestOwner))
				&&	(lRequestOwner)
				)
			{
#ifdef	_LOG_QUEUE_OPS
					if	(LogIsActive (_LOG_QUEUE_OPS))
					{
						LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d]   Waiting for [%p] [%d] [%d]"), this, mCharID, lOtherRequest, lQueuedWait->mOtherCharID, lQueuedWait->mOtherReqID);
					}
#endif
				lQueuedWait->mStarted = true;
				mQueue.AddHead (lQueuedWait.Detach ());
			}
			else
			if	(lQueuedWait->mStarted)
			{
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d]   Wait done for [%p] [%d] [%d]"), this, mCharID, lOtherRequest, lQueuedWait->mOtherCharID, lQueuedWait->mOtherReqID);
				}
#endif
				if	(mNotify.GetCount() > 0)
				{
					lQueuedWait->NotifyComplete (mNotify);
				}
			}
			else
			{
				if	(mNotify.GetCount() > 0)
				{
					lQueuedWait->NotifyComplete (mNotify, AGENTREQERR_REMOVED);
				}
			}
		}
		else
		{
			lQueuedWait.Detach (); // Was deleted during NotifyStarted
		}
		return true;
	}
	return false;
}

void CAgentCharacterWnd::AbortQueuedWait (CQueuedAction * pQueuedAction, HRESULT pReqStatus, LPCTSTR pReason)
{
}

/////////////////////////////////////////////////////////////////////////////

CQueuedAction * CAgentCharacterWnd::IsWaitQueued (long pCharID)
{
	return mQueue.GetCharAction (QueueActionWait, pCharID);
}

bool CAgentCharacterWnd::RemoveQueuedWait (long pCharID, HRESULT pReqStatus, LPCTSTR pReason, bool pExcludeActive)
{
	bool			lRet = false;
	CQueuedWait *	lQueuedWait;

	while	(
				(lQueuedWait = (CQueuedWait *) mQueue.GetCharAction (QueueActionWait, pCharID, true))
			&&	(
					(!pExcludeActive)
				||	(!lQueuedWait->mStarted)
				)
			&&	(RemoveQueuedAction (lQueuedWait, pReqStatus, pReason))
			)
	{
		lRet = true;
	}
	return lRet;
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
			LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d] QueueInterrupt [%p] [%d] [%d] Other [%d] [%d]"), this, mCharID, lQueuedInterrupt, pCharID, lReqID, pOtherCharID, pOtherReqID);
		}
#endif
	}
	else
	{
		lReqID = 0;
	}
	return lReqID;
}

bool CAgentCharacterWnd::DoQueuedInterrupt ()
{
	tPtr <CQueuedInterrupt>	lQueuedInterrupt;

	if	(lQueuedInterrupt = (CQueuedInterrupt *) mQueue.GetNextAction (QueueActionInterrupt))
	{
		CQueuedAction *		lOtherRequest;
		CAgentCharacterWnd *	lRequestOwner = NULL;
		HRESULT				Status;

#ifdef	_LOG_QUEUE_OPS
		if	(LogIsActive (_LOG_QUEUE_OPS))
		{
			LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d] QueuedInterrupt [%p] [%d] [%d] Other [%d] [%d]"), this, mCharID, lQueuedInterrupt.Ptr(), lQueuedInterrupt->mCharID, lQueuedInterrupt->mReqID, lQueuedInterrupt->mOtherCharID, lQueuedInterrupt->mOtherReqID);
		}
#endif
		if	(!lQueuedInterrupt->mStarted)
		{
			lQueuedInterrupt->NotifyStarted (mNotify);
		}
		if	(mQueue.GetNextAction (QueueActionInterrupt) == lQueuedInterrupt)
		{
			mQueue.RemoveHead ();

			if	(
					(lOtherRequest = FindOtherRequest (lQueuedInterrupt->mOtherReqID, lRequestOwner))
				&&	(lRequestOwner)
				&&	(lRequestOwner->RemoveQueuedAction (lOtherRequest, AGENTREQERR_INTERRUPTEDCODE, _T("Interrupt")))
				)
			{
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p] [%d]   Interrupted [%p] [%d] [%d]"), this, mCharID, lOtherRequest, lQueuedInterrupt->mOtherCharID, lQueuedInterrupt->mOtherReqID);
				}
#endif
				Status = S_OK;
			}
			else
			{
				Status = AGENTREQERR_REMOVED;
			}

			if	(mNotify.GetCount() > 0)
			{
				lQueuedInterrupt->NotifyComplete (mNotify, Status);
			}
		}
		else
		{
			lQueuedInterrupt.Detach (); // Was deleted during NotifyStarted
		}
		return true;
	}
	return false;
}

void CAgentCharacterWnd::AbortQueuedInterrupt (CQueuedAction * pQueuedAction, HRESULT pReqStatus, LPCTSTR pReason)
{
}

/////////////////////////////////////////////////////////////////////////////

CQueuedAction * CAgentCharacterWnd::IsInterruptQueued (long pCharID)
{
	return mQueue.GetCharAction (QueueActionInterrupt, pCharID);
}

bool CAgentCharacterWnd::RemoveQueuedInterrupt (long pCharID, HRESULT pReqStatus, LPCTSTR pReason, bool pExcludeActive)
{
	bool				lRet = false;
	CQueuedInterrupt *	lQueuedInterrupt;

	while	(
				(lQueuedInterrupt = (CQueuedInterrupt *) mQueue.GetCharAction (QueueActionInterrupt, pCharID, true))
			&&	(
					(!pExcludeActive)
				||	(!lQueuedInterrupt->mStarted)
				)
			&&	(RemoveQueuedAction (lQueuedInterrupt, pReqStatus, pReason))
			)
	{
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

CQueuedAction * CAgentCharacterWnd::FindOtherRequest (long pReqID, CAgentCharacterWnd *& pRequestOwner)
{
	CQueuedAction *		lRet = NULL;
	INT_PTR				lNotifyNdx;
	CEventNotify *		lNotify;
	CAgentCharacterWnd *	lRequestOwner;

	pRequestOwner = NULL;

	for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
	{
		if	(
				(lRequestOwner = dynamic_cast <CAgentCharacterWnd *> (lNotify->GetRequestOwner (pReqID)))
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

bool CAgentCharacterWnd::DoQueuedPrepare ()
{
	tPtr <CQueuedPrepare>	lQueuedPrepare;

	if	(lQueuedPrepare = (CQueuedPrepare *) mQueue.GetNextAction (QueueActionPrepare))
	{
		if	(!lQueuedPrepare->IsStarted ())
		{
			lQueuedPrepare->NotifyStarted (mNotify);
		}
		if	(mQueue.GetNextAction (QueueActionPrepare) == lQueuedPrepare)
		{
			if	(lQueuedPrepare->IsSoundDownload ())
			{
				lQueuedPrepare->FinishDownloads ();
			}
			else
			{
				lQueuedPrepare->PutAnimations (GetAgentFile ());
			}

			if	(lQueuedPrepare->IsComplete ())
			{
				mQueue.RemoveHead ();

				if	(mNotify.GetCount() > 0)
				{
					HRESULT			lPrepareResult = S_OK;
					CFileDownload *	lFileDownload;

					if	(lFileDownload = lQueuedPrepare->GetDownload ())
					{
						lPrepareResult = lFileDownload->IsDownloadComplete ();
					}
					lQueuedPrepare->NotifyComplete (mNotify, lPrepareResult);
				}
			}
			else
			{
				lQueuedPrepare->StartDownloads ();
				lQueuedPrepare.Detach ();
			}
		}
		else
		{
			lQueuedPrepare.Detach (); // Was deleted during NotifyStarted
		}

		return true;
	}
	return false;
}

void CAgentCharacterWnd::AbortQueuedPrepare (CQueuedAction * pQueuedAction, HRESULT pReqStatus, LPCTSTR pReason)
{
	CQueuedPrepare *	lQueuedPrepare;

	if	(lQueuedPrepare = (CQueuedPrepare *) pQueuedAction)
	{
		lQueuedPrepare->CancelDownloads ();
	}
}

/////////////////////////////////////////////////////////////////////////////

CQueuedAction * CAgentCharacterWnd::IsPrepareQueued (long pCharID)
{
	return mQueue.GetCharAction (QueueActionPrepare, pCharID);
}

bool CAgentCharacterWnd::RemoveQueuedPrepare (long pCharID, HRESULT pReqStatus, LPCTSTR pReason, bool pExcludeActive)
{
	bool				lRet = false;
	CQueuedPrepare *	lQueuedPrepare;

	while	(
				(lQueuedPrepare = (CQueuedPrepare *) mQueue.GetCharAction (QueueActionPrepare, pCharID, true))
			&&	(
					(!pExcludeActive)
				||	(!lQueuedPrepare->IsStarted ())
				)
			&&	(RemoveQueuedAction (lQueuedPrepare, pReqStatus, pReason))
			)
	{
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

void CAgentCharacterWnd::AbortQueuedAction (CQueuedAction * pQueuedAction, HRESULT pReqStatus, LPCTSTR pReason)
{
	CAgentWnd::AbortQueuedAction (pQueuedAction, pReqStatus, pReason);

	if	(pQueuedAction)
	{
		if	(pQueuedAction->mAction == QueueActionShow)
		{
			AbortQueuedShow (pQueuedAction, pReqStatus, pReason);
		}
		else
		if	(pQueuedAction->mAction == QueueActionHide)
		{
			AbortQueuedHide (pQueuedAction, pReqStatus, pReason);
		}
		else
		if	(pQueuedAction->mAction == QueueActionThink)
		{
			AbortQueuedThink (pQueuedAction, pReqStatus, pReason);
		}
		else
		if	(pQueuedAction->mAction == QueueActionSpeak)
		{
			AbortQueuedSpeak (pQueuedAction, pReqStatus, pReason);
		}
		else
		if	(pQueuedAction->mAction == QueueActionWait)
		{
			AbortQueuedWait (pQueuedAction, pReqStatus, pReason);
		}
		else
		if	(pQueuedAction->mAction == QueueActionInterrupt)
		{
			AbortQueuedInterrupt (pQueuedAction, pReqStatus, pReason);
		}
		else
		if	(pQueuedAction->mAction == QueueActionPrepare)
		{
			AbortQueuedPrepare (pQueuedAction, pReqStatus, pReason);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentCharacterWnd::DoAnimationQueue (bool & pNextActivateImmediate, DWORD & pNextQueueTime)
{
	bool			lRet = CAgentWnd::DoAnimationQueue (pNextActivateImmediate, pNextQueueTime);
	CQueuedAction *	lNextAction = mQueue.GetNextAction ();

	if	(
			(!lRet)
		&&	(
				(DoQueuedShow ())
			||	(DoQueuedHide ())
			||	(DoQueuedThink ())
			||	(DoQueuedSpeak ())
			||	(DoQueuedWait ())
			||	(DoQueuedInterrupt ())
			||	(DoQueuedPrepare ())
			)
		)
	{
		pNextActivateImmediate = (mQueue.GetNextAction () != lNextAction);
		pNextQueueTime = mQueueTime;
		lRet = true;
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
		&&	(PreNotify ())
		)
	{
		try
		{
			INT_PTR			lNotifyNdx;
			CEventNotify *	lNotify;

			for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
			{
				lNotify->Idle (lNotify->GetNotifyClient (mCharID), FALSE);
			}
		}
		catch AnyExceptionDebug
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
		&&	(PreNotify ())
		)
	{
		try
		{
			INT_PTR			lNotifyNdx;
			CEventNotify *	lNotify;

			for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
			{
				lNotify->Idle (lNotify->GetNotifyClient (mCharID), TRUE);
			}
		}
		catch AnyExceptionDebug
		PostNotify ();
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LRESULT CAgentCharacterWnd::OnTimer (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
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

void CAgentCharacterWnd::NotifyClick (short pButton, const CPoint & pPoint)
{
	if	(PreNotify ())
	{
		try
		{
			INT_PTR			lNotifyNdx;
			CEventNotify *	lNotify;

			for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
			{
				if	(lNotify->GetNotifyClient (mCharID) == mCharID)
				{
					lNotify->Click (mCharID, pButton|NotifyKeyState(), pPoint.x, pPoint.y);
				}
			}
		}
		catch AnyExceptionDebug
		PostNotify ();
	}
}

void CAgentCharacterWnd::NotifyDblClick (short pButton, const CPoint & pPoint)
{
	if	(PreNotify ())
	{
		try
		{
			INT_PTR			lNotifyNdx;
			CEventNotify *	lNotify;

			for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
			{
				if	(lNotify->GetNotifyClient (mCharID) == mCharID)
				{
					lNotify->DblClick (mCharID, pButton|NotifyKeyState(), pPoint.x, pPoint.y);
				}
			}
		}
		catch AnyExceptionDebug
		PostNotify ();
	}
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CAgentCharacterWnd::OnLButtonDown (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
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

LRESULT CAgentCharacterWnd::OnLButtonUp (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
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

LRESULT CAgentCharacterWnd::OnLButtonDblClk (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
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

LRESULT CAgentCharacterWnd::OnRButtonDown (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
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

LRESULT CAgentCharacterWnd::OnRButtonUp (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
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
		PostNotify ();
	}
	return lResult;
}

LRESULT CAgentCharacterWnd::OnRButtonDblClk (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
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

LRESULT CAgentCharacterWnd::OnMButtonDown (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
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

LRESULT CAgentCharacterWnd::OnMButtonUp (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
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

LRESULT CAgentCharacterWnd::OnMButtonDblClk (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
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

LRESULT CAgentCharacterWnd::OnContextMenu (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnContextMenu [%d %d]"), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
#endif
	if	(
			((HWND)wParam == m_hWnd)
		&&	(PreNotify ())
		)
	{
		try
		{
			INT_PTR			lNotifyNdx;
			CEventNotify *	lNotify;

			for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
			{
				if	(
						(lNotify->GetNotifyClient (mCharID) == mCharID)
					&&	(lNotify->_ContextMenu (mCharID, m_hWnd, CPoint (GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))))
					)
				{
					break;
				}
			}
		}
		catch AnyExceptionDebug
		PostNotify ();
	}
	return 0;
}

LRESULT CAgentCharacterWnd::OnDisplayChange (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LRESULT	lResult = 0;
#ifdef	_DEBUG
	LogMessage (LogNormal, _T("CAgentCharacterWnd::OnDisplayChange"));
#endif
	bHandled = CAgentWnd::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult);
	return lResult;
}

LRESULT CAgentCharacterWnd::OnInputLangChange (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LRESULT	lResult = 0;
#ifdef	_DEBUG
	LogMessage (LogNormal, _T("CAgentCharacterWnd::OnInputLangChange [%4.4hX]"), LOWORD(GetKeyboardLayout(GetCurrentThreadId())));
#endif
	bHandled = CAgentWnd::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult);
	return lResult;
}
