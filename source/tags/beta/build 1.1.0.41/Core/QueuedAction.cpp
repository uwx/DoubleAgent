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
#include "QueuedAction.h"
#include "QueuedActions.h"
#include "AgentPopupWnd.h"
#include "AgentBalloonWnd.h"
#include "EventNotify.h"
#include "Elapsed.h"
#include "StringArrayEx.h"
#include "DebugStr.h"
#ifdef	_DEBUG
#include "Registry.h"
#endif

#ifdef	_DEBUG
#define	_DEBUG_REQUESTS	(GetProfileDebugInt(_T("DebugRequests"),LogVerbose,true)&0xFFFF|LogTimeMs)
#define	_LOG_QUEUE_OPS	(GetProfileDebugInt(_T("LogQueueOps"),LogVerbose,true)&0xFFFF|LogTimeMs)
#endif

#ifndef	_LOG_QUEUE_OPS
#define	_LOG_QUEUE_OPS	LogVerbose
#endif

#ifdef	_TRACE_ACTION_INSTANCE
CAtlPtrTypeArray<CQueuedAction>	CQueuedAction::mInstances;
#endif

//////////////////////////////////////////////////////////////////////

CQueuedAction::CQueuedAction (QueueAction pAction, long pCharID, long pReqID)
:	mAction (pAction), mCharID (pCharID), mReqID (pReqID), mStarted (false), mPaused (false)
{
#ifdef	_TRACE_ACTION_INSTANCE
	mInstances.AddSorted (this);
#endif	
}

CQueuedAction::~CQueuedAction ()
{
#ifdef	_TRACE_ACTION_INSTANCE
	mInstances.RemoveSorted (this);
#endif	
}

//////////////////////////////////////////////////////////////////////

void CQueuedAction::NotifyStarted (CEventNotify * pNotify)
{
	if	(
			(mReqID > 0)
		&&	(pNotify != NULL)
		&&	(pNotify->GetNotifyClient (mCharID) == mCharID)
		)
	{
#ifdef	_DEBUG_REQUESTS
		LogMessage (_DEBUG_REQUESTS, _T("[%d(%d)] RequestStart"), mCharID, mReqID);
#endif
		pNotify->RequestStart (mReqID);
	}
}

void CQueuedAction::NotifyStarted (CAtlPtrTypeArray <CEventNotify> & pNotify)
{
	mStarted = true;

#ifdef	_LOG_QUEUE_OPS
	if	(LogIsActive())
	{
		LogMessage (_LOG_QUEUE_OPS, _T("[%d(%d)] ActionStarted"), mCharID, mReqID);
	}
#endif

	if	(mReqID > 0)
	{
		INT_PTR			lNotifyNdx;
		CEventNotify *	lNotify;

#ifdef	_DEBUG_REQUESTS
		LogMessage (_DEBUG_REQUESTS, _T("[%d(%d)] NotifyStarted   [%d]"), mCharID, mReqID, pNotify.GetCount());
#endif
		for	(lNotifyNdx = 0; lNotify = pNotify (lNotifyNdx); lNotifyNdx++)
		{
			NotifyStarted (lNotify);
		}
	}
}

void CQueuedAction::NotifyComplete (CEventNotify * pNotify, HRESULT pReqStatus)
{
	if	(
			(mReqID > 0)
		&&	(pNotify != NULL)
		&&	(pNotify->GetNotifyClient (mCharID) == mCharID)
		)
	{
#ifdef	_DEBUG_REQUESTS
		LogMessage (_DEBUG_REQUESTS, _T("[%d(%d)] RequestComplete [%8.8X]"), mCharID, mReqID, pReqStatus);
#endif
		pNotify->RequestComplete (mReqID, pReqStatus);
	}
}

void CQueuedAction::NotifyComplete (CAtlPtrTypeArray <CEventNotify> & pNotify, HRESULT pReqStatus)
{
#ifdef	_LOG_QUEUE_OPS
	if	(LogIsActive())
	{
		LogMessage (_LOG_QUEUE_OPS, _T("[%d(%d)] ActionComplete  [%8.8X]"), mCharID, mReqID, pReqStatus);
	}
#endif

	if	(mReqID > 0)
	{
		INT_PTR			lNotifyNdx;
		CEventNotify *	lNotify;

#ifdef	_DEBUG_REQUESTS
		LogMessage (_DEBUG_REQUESTS, _T("[%d(%d)] NotifyComplete  [%d]"), mCharID, mReqID, pNotify.GetCount());
#endif
		for	(lNotifyNdx = 0; lNotify = pNotify (lNotifyNdx); lNotifyNdx++)
		{
			NotifyComplete (lNotify, pReqStatus);
		}
	}
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CQueuedState::CQueuedState (LPCTSTR pStateName, long pCharID, long pReqID)
:	CQueuedAction (QueueActionState, pCharID, pReqID),
	mStateName (pStateName)
{
}

CQueuedState::~CQueuedState ()
{
}

//////////////////////////////////////////////////////////////////////

bool CQueuedState::Advance (CQueuedActions & pQueue, CAgentWnd * pAgentWnd)
{
	bool	lRet = false;

	if	(mPaused)
	{
		lRet = true;
	}
	else
	{
		if	(mStarted)
		{
			pQueue.RemoveHead ();
			NotifyComplete (pAgentWnd->mNotify);
		}
		else
		{
			NotifyStarted (pAgentWnd->mNotify);

			if	(pQueue.GetNextAction (QueueActionState) == this)
			{
				if	((pAgentWnd->GetStyle () & WS_VISIBLE) == 0)
				{
					pQueue.RemoveHead ();
					NotifyComplete (pAgentWnd->mNotify, AGENTERR_CHARACTERNOTVISIBLE);
				}
				else
				{
					CAtlOwnPtrList <CQueuedAction>	lQueue;

					pQueue.RemoveHead ();
					pQueue.PushQueue (lQueue);
					if	(pAgentWnd->ShowStateGestures (mCharID, mStateName, true))
					{
						pQueue.AddTail (this);
						lRet = true;
					}
					pQueue.PopQueue (lQueue);
				}
			}
			else
			{
				lRet = true; // Was deleted during NotifyStarted
			}
		}
	}
	return lRet;
}

bool CQueuedState::Pause (CQueuedActions & pQueue, CAgentWnd * pAgentWnd, bool pPause)
{
	bool				lRet = false;
#ifdef	_LOG_QUEUE_OPS
	if	(LogIsActive (_LOG_QUEUE_OPS))
	{
		LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] Pause [%u] QueuedState [%p(%d)] Started [%u]"), pAgentWnd, mCharID, pPause, this, mReqID, mStarted);
	}
#endif
	if	(mPaused != pPause)
	{
		mPaused = pPause;
		lRet = true;
	}
	return lRet;
}

bool CQueuedState::Abort (CQueuedActions & pQueue, CAgentWnd * pAgentWnd, HRESULT pReqStatus, LPCTSTR pReason)
{
	bool				lRet = false;
	CQueuedGesture *	lQueuedGesture;

	if	(mStarted)
	{
#ifdef	_LOG_QUEUE_OPS
		if	(LogIsActive (_LOG_QUEUE_OPS))
		{
			LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] Abort [%p(%d)] QueuedState [%s]"), pAgentWnd, mCharID, this, mReqID, mStateName);
		}
#endif
		while	(
					(lQueuedGesture = (CQueuedGesture *) pQueue.GetNextAction (QueueActionGesture))
				&&	(lQueuedGesture->mCharID == mCharID)
				&&	(lQueuedGesture->mStateName.CompareNoCase (mStateName) == 0)
				&&	(pAgentWnd->RemoveQueuedGesture (lQueuedGesture, pReqStatus, pReason))
				)
		{
			lRet = true;
		}
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CQueuedGesture::CQueuedGesture (LPCTSTR pGestureName, long pCharID, long pReqID)
:	CQueuedAction (QueueActionGesture, pCharID, pReqID),
	mGestureName (pGestureName)
{
}

CQueuedGesture::~CQueuedGesture ()
{
}

//////////////////////////////////////////////////////////////////////

bool CQueuedGesture::Advance (CQueuedActions & pQueue, CAgentWnd * pAgentWnd)
{
	bool	lRet = false;

	if	(mPaused)
	{
		lRet = true;
	}
	else
	{
		if	(mStarted)
		{
			pQueue.RemoveHead ();
			NotifyComplete (pAgentWnd->mNotify);
		}
		else
		{
			NotifyStarted (pAgentWnd->mNotify);

			if	(pQueue.GetNextAction (QueueActionGesture) == this)
			{
				if	((pAgentWnd->GetStyle () & WS_VISIBLE) == 0)
				{
					pQueue.RemoveHead ();
					if	(mReqID > 0)
					{
						NotifyComplete (pAgentWnd->mNotify, AGENTERR_CHARACTERNOTVISIBLE);
					}
				}
				else
				if	(pAgentWnd->ShowGesture (mGestureName, mStateName))
				{
					lRet = true;
				}
				else
				{
					pQueue.RemoveHead ();
					if	(mReqID > 0)
					{
						NotifyComplete (pAgentWnd->mNotify, AGENTERR_ANIMATIONNOTFOUND);
					}
				}
			}
			else
			{
				lRet = true; // Was deleted during NotifyStarted
			}
		}
	}
	return lRet;
}

bool CQueuedGesture::Pause (CQueuedActions & pQueue, CAgentWnd * pAgentWnd, bool pPause)
{
	bool	lRet = false;
#ifdef	_LOG_QUEUE_OPS
	if	(LogIsActive (_LOG_QUEUE_OPS))
	{
		LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] Pause [%u] QueuedGesture [%p(%d)] Started [%u]"), pAgentWnd, mCharID, pPause, this, mReqID, mStarted);
	}
#endif
	if	(pPause)
	{
		if	(!mPaused)
		{
			lRet = true;
		}
		if	(
				(mStarted)
			&&	(pAgentWnd->IsPlaying (false))
			)
		{
			pAgentWnd->Pause ();
		}
	}
	else
	{
		if	(mPaused)
		{
			lRet = true;
		}
		if	(
				(mStarted)
			&&	(pAgentWnd->IsPaused ())
			)
		{
			pAgentWnd->Resume ();
		}
	}
	mPaused = pPause;

	return lRet;
}

bool CQueuedGesture::Abort (CQueuedActions & pQueue, CAgentWnd * pAgentWnd, HRESULT pReqStatus, LPCTSTR pReason)
{
	bool	lRet = false;

#ifdef	_LOG_QUEUE_OPS
	if	(LogIsActive (_LOG_QUEUE_OPS))
	{
		LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] Abort [%p(%d)] QueuedGesture [%s] Started [%u] Animating [%u]"), pAgentWnd, mCharID, this, mReqID, mGestureName, mStarted, !pAgentWnd->IsAnimationComplete());
	}
#endif
	if	(mStarted)
	{
		if	(!pAgentWnd->ShowGesture (NULL))
		{
			pAgentWnd->Stop ();
		}
		lRet = true;
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CQueuedShow::CQueuedShow (long pCharID, long pReqID)
:	CQueuedAction (QueueActionShow, pCharID, pReqID),
	mFast (false),
	mVisibilityCause (-1),
	mAnimationShown (false)
{
}

CQueuedShow::~CQueuedShow ()
{
}

//////////////////////////////////////////////////////////////////////

bool CQueuedShow::Advance (CQueuedActions & pQueue, CAgentWnd * pAgentWnd)
{
	bool					lRet = false;
	bool					lShown = true;
	CAgentCharacterWnd *	lCharacterWnd;

	if	(mPaused)
	{
		lRet = true;
	}
	else
	{
		if	(!mStarted)
		{
			NotifyStarted (pAgentWnd->mNotify);
		}
		if	(pQueue.GetNextAction (QueueActionShow) == this)
		{
			if	(
					(!mAnimationShown)
				&&	(lCharacterWnd = dynamic_cast <CAgentCharacterWnd *> (pAgentWnd))
				)
			{
				lShown = lCharacterWnd->ShowQueued (this);
			}

			if	(pQueue.GetNextAction (QueueActionShow) == this)
			{
				if	(
						(!mFast)
					&&	(!mAnimationShown)
					&&	(
							(lShown)
						||	(
								(mVisibilityCause < 0)
							&&	(!pAgentWnd->DidAnimations())
							)
						)
					)
				{
					CAtlOwnPtrList <CQueuedAction>	lQueue;

					mAnimationShown = true;
					pAgentWnd->ClearAnimations ();

					pQueue.PushQueue (lQueue);
					if	(pAgentWnd->ShowState (_T("SHOWING")))
					{
						lQueue.RemoveHead ();
						pQueue.AddTail (this);
#ifdef	_LOG_QUEUE_OPS
						if	(LogIsActive (_LOG_QUEUE_OPS))
						{
							LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] Requeue [%p(%d)] Show after Showing state"), pAgentWnd, mCharID, this, mReqID);
						}
#endif
						lRet = true;
					}
					pQueue.PopQueue (lQueue);
				}
				else
				{
					mAnimationShown = true;
				}

				if	(!lRet)
				{
					pQueue.RemoveHead ();
					NotifyComplete (pAgentWnd->mNotify);
				}
			}
			else
			{
				lRet = true; // Was deleted during ShowQueued
			}
		}
		else
		{
			lRet = true; // Was deleted during NotifyStarted
		}
	}
	return lRet;
}

bool CQueuedShow::Pause (CQueuedActions & pQueue, CAgentWnd * pAgentWnd, bool pPause)
{
	bool	lRet = false;
#ifdef	_LOG_QUEUE_OPS
	if	(LogIsActive (_LOG_QUEUE_OPS))
	{
		LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] Pause [%u] QueuedShow [%p(%d)] Started [%u] Animated [%d]"), pAgentWnd, mCharID, pPause, this, mReqID, mStarted, mAnimationShown);
	}
#endif
	if	(pPause)
	{
		if	(!mPaused)
		{
			lRet = true;
		}
		if	(
				(mAnimationShown)
			&&	(pAgentWnd->IsPlaying (false))
			)
		{
			pAgentWnd->Pause ();
		}
	}
	else
	{
		if	(mPaused)
		{
			lRet = true;
		}
		if	(
				(mAnimationShown)
			&&	(pAgentWnd->IsPaused ())
			)
		{
			pAgentWnd->Resume ();
		}
	}
	mPaused = pPause;

	return lRet;
}

bool CQueuedShow::Abort (CQueuedActions & pQueue, CAgentWnd * pAgentWnd, HRESULT pReqStatus, LPCTSTR pReason)
{
	bool			lRet = false;
	CQueuedState *	lQueuedState;

#ifdef	_LOG_QUEUE_OPS
	if	(LogIsActive (_LOG_QUEUE_OPS))
	{
		LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] Abort [%p(%d)] QueuedShow Started [%u]"), pAgentWnd, mCharID, this, mReqID, mStarted);
	}
#endif
	if	(
			(mFast)
		&&	(lQueuedState = (CQueuedState *) pQueue.GetNextAction (QueueActionState))
		&&	(lQueuedState->mStateName.CompareNoCase (_T("SHOWING")) == 0)
		)
	{
		lRet = pAgentWnd->RemoveQueuedAction (lQueuedState, pReqStatus, pReason);
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CQueuedHide::CQueuedHide (long pCharID, long pReqID)
:	CQueuedAction (QueueActionHide, pCharID, pReqID),
	mFast (false),
	mVisibilityCause (-1),
	mAnimationShown (false)
{
}

CQueuedHide::~CQueuedHide ()
{
}

//////////////////////////////////////////////////////////////////////

bool CQueuedHide::Advance (CQueuedActions & pQueue, CAgentWnd * pAgentWnd)
{
	bool					lRet = false;
	CAgentCharacterWnd *	lCharacterWnd;
	CAgentBalloonWnd *		lBalloonWnd;
	CAgentListeningWnd *	lListeningWnd;

	if	(mPaused)
	{
		lRet = true;
	}
	else
	{
		if	(!mStarted)
		{
			if	(lCharacterWnd = dynamic_cast <CAgentCharacterWnd *> (pAgentWnd))
			{
				if	(
						(lBalloonWnd = lCharacterWnd->GetBalloonWnd (false))
					&&	(lBalloonWnd->IsWindow ())
					)
				{
					lBalloonWnd->HideBalloon ();
				}
				if	(
						(lListeningWnd = lCharacterWnd->GetListeningWnd (false))
					&&	(lListeningWnd->IsWindow ())
					&&	(lListeningWnd->GetCharID() == lCharacterWnd->GetCharID())
					)
				{
					lListeningWnd->HideTipWnd ();
				}
			}

			NotifyStarted (pAgentWnd->mNotify);
		}

		if	(pQueue.GetNextAction (QueueActionHide) == this)
		{
			if	(
					(!mFast)
				&&	(!mAnimationShown)
				)
			{
				mAnimationShown = true;

				if	(pAgentWnd->GetStyle() & WS_VISIBLE)
				{
					CAtlOwnPtrList <CQueuedAction>	lQueue;

					pQueue.PushQueue (lQueue);
					if	(pAgentWnd->ShowState (_T("HIDING")))
					{
						lQueue.RemoveHead ();
						pQueue.AddTail (this);
						lRet = true;
#ifdef	_LOG_QUEUE_OPS
						if	(LogIsActive (_LOG_QUEUE_OPS))
						{
							LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] Requeue [%p(%d)] Hide after Hiding state"), pAgentWnd, mCharID, this, mReqID);
						}
#endif
					}
					pQueue.PopQueue (lQueue);
				}
			}
			else
			{
				mAnimationShown = true;
			}

			if	(
					(!lRet)
				&&	(mAnimationShown)
				)
			{
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] Queued [%p(%d)] Hide"), pAgentWnd, mCharID, this, mReqID);
				}
#endif
				if	(lCharacterWnd = dynamic_cast <CAgentCharacterWnd *> (pAgentWnd))
				{
					lCharacterWnd->HideQueued (this);
				}
				if	(pQueue.GetNextAction (QueueActionHide) == this)
				{
					pQueue.RemoveHead ();
					NotifyComplete (pAgentWnd->mNotify);
				}
				else
				{
					lRet = true; // Was deleted during HideQueued
				}
			}
		}
		else
		{
			lRet = true; // Was deleted during NotifyStarted
		}
	}
	return lRet;
}

bool CQueuedHide::Pause (CQueuedActions & pQueue, CAgentWnd * pAgentWnd, bool pPause)
{
	bool	lRet = false;
#ifdef	_LOG_QUEUE_OPS
	if	(LogIsActive (_LOG_QUEUE_OPS))
	{
		LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] Pause [%u] QueuedHide [%p(%d)] Started [%u] Animated [%u]"), pAgentWnd, mCharID, pPause, this, mReqID, mStarted, mAnimationShown);
	}
#endif
	if	(pPause)
	{
		if	(!mPaused)
		{
			lRet = true;
		}
		if	(
				(mAnimationShown)
			&&	(pAgentWnd->IsPlaying (false))
			)
		{
			pAgentWnd->Pause ();
		}
	}
	else
	{
		if	(mPaused)
		{
			lRet = true;
		}
		if	(
				(mAnimationShown)
			&&	(pAgentWnd->IsPaused ())
			)
		{
			pAgentWnd->Resume ();
		}
	}
	mPaused = pPause;

	return lRet;
}

bool CQueuedHide::Abort (CQueuedActions & pQueue, CAgentWnd * pAgentWnd, HRESULT pReqStatus, LPCTSTR pReason)
{
	bool			lRet = false;
	CQueuedState *	lQueuedState;

#ifdef	_LOG_QUEUE_OPS
	if	(LogIsActive (_LOG_QUEUE_OPS))
	{
		LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] Abort [%p(%d)] QueuedHide Started [%u]"), pAgentWnd, mCharID, this, mReqID, mStarted);
	}
#endif
	if	(
			(mAnimationShown)
		&&	(lQueuedState = (CQueuedState *) pQueue.GetNextAction (QueueActionState))
		&&	(lQueuedState->mStateName.CompareNoCase (_T("HIDING")) == 0)
		)
	{
		lRet = pAgentWnd->RemoveQueuedAction (lQueuedState, pReqStatus, pReason);
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CQueuedMove::CQueuedMove (long pCharID, long pReqID)
:	CQueuedAction (QueueActionMove, pCharID, pReqID),
	mAnimationShown (false),
	mEndAnimationShown (false),
	mMoveStarted (false),
	mTimeStarted (0),
	mTimeAllowed (0),
	mTimePaused (0)
{
}

CQueuedMove::~CQueuedMove ()
{
}

//////////////////////////////////////////////////////////////////////

bool CQueuedMove::IsCycling () const
{
	if	(
			(!mPaused)
		&&	(mMoveStarted)
		&&	(mTimeAllowed > 0)
		)
	{
		return true;
	}
	return false;
}

bool CQueuedMove::Cycle (CQueuedActions & pQueue, CAgentWnd * pAgentWnd)
{
	bool	lRet = false;

	if	(IsCycling ())
	{
		long	lElapsed = ElapsedTicks (mTimeStarted);
		CPoint	lOffset;
		CRect	lWinRect;

		pAgentWnd->GetWindowRect (&lWinRect);
		lOffset.x = mPosition.x - lWinRect.left;
		lOffset.y = mPosition.y - lWinRect.top;

		if	(pQueue.mTime != pQueue.mTimeMin)
		{
			pAgentWnd->ActivateQueue (false, pQueue.mTimeMin);
		}
		if	(lElapsed < (long)mTimeAllowed)
		{
			lOffset.x = mMoveStarted->x + MulDiv (mPosition.x - mMoveStarted->x, lElapsed, (long)mTimeAllowed) - lWinRect.left;
			lOffset.y = mMoveStarted->y + MulDiv (mPosition.y - mMoveStarted->y, lElapsed, (long)mTimeAllowed) - lWinRect.top;
#ifdef	_LOG_QUEUE_OPS
			if	(LogIsActive (_LOG_QUEUE_OPS))
			{
				LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] Queued [%p(%d)] Move [%d %d] to [%d %d] by [%d %d] Elapsed [%d of %u] Remaining [%d %d] [%d]"), pAgentWnd, mCharID, this, mReqID, lWinRect.left, lWinRect.top, mPosition.x, mPosition.y, lOffset.x, lOffset.y, lElapsed, mTimeAllowed, mPosition.x-lWinRect.left-lOffset.x, mPosition.y-lWinRect.top-lOffset.y, (long)mTimeAllowed-lElapsed);
			}
#endif
			lWinRect.OffsetRect (lOffset);
			pAgentWnd->MoveWindow (lWinRect);
			lRet = true;
		}
		else
		if	(pQueue.mTime != pQueue.mTimeDefault)
		{
			pAgentWnd->ActivateQueue (false, pQueue.mTimeDefault);
		}
	}
	return lRet;
}

bool CQueuedMove::Advance (CQueuedActions & pQueue, CAgentWnd * pAgentWnd)
{
	bool					lRet = false;
	CAgentCharacterWnd *	lCharacterWnd;
	CAgentPopupWnd *		lPopupWnd;
	CPoint					lOffset;
	CRect					lWinRect;

	if	(mPaused)
	{
		lRet = true;
	}
	else
	{
		pAgentWnd->GetWindowRect (&lWinRect);
		lOffset.x = mPosition.x - lWinRect.left;
		lOffset.y = mPosition.y - lWinRect.top;

		if	(!mStarted)
		{
			NotifyStarted (pAgentWnd->mNotify);
		}
		if	(pQueue.GetNextAction (QueueActionMove) == this)
		{
			if	(
					(!mAnimationShown)
				&&	(
						(!pAgentWnd->IsWindowVisible ())
					||	(
							(lOffset.x == 0)
						&&	(lOffset.y == 0)
						)
					)
				)
			{
				mTimeAllowed = 0;
			}

			if	(!mAnimationShown)
			{
				mAnimationShown = true;

				if	(mTimeAllowed > 0)
				{
					CAtlOwnPtrList <CQueuedAction>	lQueue;

					if	(lOffset.x < 0)
					{
						if	(lOffset.y < lOffset.x)
						{
							mAnimationState = _T("MOVINGUP");
						}
						else
						if	(lOffset.y > -lOffset.x)
						{
							mAnimationState = _T("MOVINGDOWN");
						}
						else
						{
							mAnimationState = _T("MOVINGRIGHT");
						}
					}
					else
					{
						if	(lOffset.y > lOffset.x)
						{
							mAnimationState = _T("MOVINGDOWN");
						}
						else
						if	(lOffset.y < -lOffset.x)
						{
							mAnimationState = _T("MOVINGUP");
						}
						else
						{
							mAnimationState = _T("MOVINGLEFT");
						}
					}

					pQueue.PushQueue (lQueue);
					if	(pAgentWnd->ShowState (mAnimationState))
					{
						lQueue.RemoveHead ();
						pQueue.AddTail (this);
						lRet = true;
#ifdef	_LOG_QUEUE_OPS
						if	(LogIsActive (_LOG_QUEUE_OPS))
						{
							LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] Requeue [%p(%d)] Move to end of queue"), pAgentWnd, mCharID, this, mReqID);
						}
#endif
					}
					pQueue.PopQueue (lQueue);
				}
			}

			if	(
					(!lRet)
				&&	(pQueue.GetNextAction (QueueActionMove) == this)
				)
			{
				if	(!mMoveStarted)
				{
					mMoveStarted = new CPoint (lWinRect.TopLeft());
					if	(mTimeAllowed > 0)
					{
						mTimeStarted = GetTickCount();
						mTimeAllowed = MulDiv (max (max (labs (lOffset.x), labs (lOffset.y)), 10), mTimeAllowed, 500);
					}
				}

				if	(mMoveStarted)
				{
					if	(
							(IsCycling ())
						&&	(Cycle (pQueue, pAgentWnd))
						)
					{
						lRet = true;
					}

					if	(!lRet)
					{
						if	(
								(!mEndAnimationShown)
							&&	(lPopupWnd = dynamic_cast <CAgentPopupWnd *> (pAgentWnd))
							)
						{
							lPopupWnd->MovePopup (mPosition, mCharID, MoveCause_ProgramMoved, true);
						}
						if	(
								(mTimeAllowed > 0)
							&&	(!mEndAnimationShown)
							&&	(pAgentWnd->ShowGesture (NULL))
							)
						{
							mEndAnimationShown = true;
							lRet = true;
						}
						if	(!lRet)
						{
							pQueue.RemoveHead ();
#ifdef	_STRICT_COMPATIBILITY
							if	(
									(lCharacterWnd = dynamic_cast <CAgentCharacterWnd *> (pAgentWnd))
								&&	(!lCharacterWnd->IsCharShown ())
								)
							{
								NotifyComplete (pAgentWnd->mNotify, AGENTERR_CHARACTERNOTVISIBLE);
							}
							else
#endif
							{
								NotifyComplete (pAgentWnd->mNotify);
							}
						}
					}
				}
			}
			else
			{
				lRet = true;
			}
		}
		else
		{
			lRet = true; // Was deleted during NotifyStarted
		}
	}
	return lRet;
}

bool CQueuedMove::Pause (CQueuedActions & pQueue, CAgentWnd * pAgentWnd, bool pPause)
{
	bool	lRet = false;
#ifdef	_LOG_QUEUE_OPS
	if	(LogIsActive (_LOG_QUEUE_OPS))
	{
		LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] Pause [%u] QueuedMove [%p(%d)] Started [%u] Animated [%u %u]"), pAgentWnd, mCharID, pPause, this, mReqID, mStarted, mAnimationShown, mEndAnimationShown);
	}
#endif
	if	(pPause)
	{
		if	(!mPaused)
		{
			lRet = true;
		}
		if	(
				(
					(mAnimationShown)
				||	(mEndAnimationShown)
				)
			&&	(pAgentWnd->IsPlaying (false))
			)
		{
			pAgentWnd->Pause ();
		}
		mTimePaused = GetTickCount();
	}
	else
	{
		if	(mPaused)
		{
			lRet = true;
		}
		if	(
				(
					(mAnimationShown)
				||	(mEndAnimationShown)
				)
			&&	(pAgentWnd->IsPaused ())
			)
		{
			pAgentWnd->Resume ();
		}
		if	(mMoveStarted)
		{
			mTimeStarted += ElapsedTicks (mTimePaused);
		}
	}
	mPaused = pPause;

	return lRet;
}

bool CQueuedMove::Abort (CQueuedActions & pQueue, CAgentWnd * pAgentWnd, HRESULT pReqStatus, LPCTSTR pReason)
{
	bool			lRet = false;
	CQueuedState *	lQueuedState;

#ifdef	_LOG_QUEUE_OPS
	if	(LogIsActive (_LOG_QUEUE_OPS))
	{
		LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] Abort [%p(%d)] QueuedMove Started [%u] Animated [%u %u]"), pAgentWnd, mCharID, this, mReqID, mStarted, mAnimationShown, mEndAnimationShown);
	}
#endif
	if	(
			(mAnimationShown)
		&&	(!mEndAnimationShown)
		)
	{
		if	(!pAgentWnd->ShowGesture (NULL))
		{
			pAgentWnd->Stop ();
		}
		lRet = true;
	}
	if	(
			(!mAnimationState.IsEmpty ())
		&&	(lQueuedState = (CQueuedState *) pQueue.GetNextAction (QueueActionState))
		&&	(lQueuedState->mStateName.CompareNoCase (mAnimationState) == 0)
		)
	{
		lRet = pAgentWnd->RemoveQueuedAction (lQueuedState, pReqStatus, pReason);
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CQueuedWait::CQueuedWait (long pOtherCharID, long pOtherReqID, long pCharID, long pReqID)
:	CQueuedAction (QueueActionWait, pCharID, pReqID),
	mOtherCharID (pOtherCharID),
	mOtherReqID (pOtherReqID)
{
}

CQueuedWait::~CQueuedWait ()
{
}

//////////////////////////////////////////////////////////////////////

bool CQueuedWait::Advance (CQueuedActions & pQueue, CAgentWnd * pAgentWnd)
{
	bool					lRet = false;
	CQueuedAction *			lOtherRequest;
	CAgentCharacterWnd *	lRequestOwner = NULL;

	if	(mPaused)
	{
		lRet = true;
	}
	else
	{
#ifdef	_LOG_QUEUE_OPS
		if	(LogIsActive (_LOG_QUEUE_OPS))
		{
			LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] QueuedWait [%p(%d)] Other [%d(%d)]"), pAgentWnd, mCharID, this, mReqID, mOtherCharID, mOtherReqID);
		}
#endif

		if	(!mStarted)
		{
			NotifyStarted (pAgentWnd->mNotify);
		}
		if	(pQueue.GetNextAction (QueueActionWait) == this)
		{
			if	(
					(lRequestOwner = dynamic_cast <CAgentCharacterWnd *> (pAgentWnd))
				&&	(lOtherRequest = lRequestOwner->FindOtherRequest (mOtherReqID, lRequestOwner))
				&&	(lRequestOwner)
				)
			{
#ifdef	_LOG_QUEUE_OPS
					if	(LogIsActive (_LOG_QUEUE_OPS))
					{
						LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)]   Waiting for [%p(%d)(%d)]"), pAgentWnd, mCharID, lOtherRequest, mOtherCharID, mOtherReqID);
					}
#endif
				mStarted = true;
				lRet = true;
			}
			else
			if	(mStarted)
			{
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)]   Wait done for [%p(%d)(%d)]"), pAgentWnd, mCharID, lOtherRequest, mOtherCharID, mOtherReqID);
				}
#endif
				pQueue.RemoveHead ();
				if	(pAgentWnd->mNotify.GetCount() > 0)
				{
					NotifyComplete (pAgentWnd->mNotify);
				}
			}
			else
			{
				pQueue.RemoveHead ();
				if	(pAgentWnd->mNotify.GetCount() > 0)
				{
					NotifyComplete (pAgentWnd->mNotify, AGENTREQERR_REMOVED);
				}
			}
		}
		else
		{
			lRet = true; // Was deleted during NotifyStarted
		}
	}
	return lRet;
}

bool CQueuedWait::Pause (CQueuedActions & pQueue, CAgentWnd * pAgentWnd, bool pPause)
{
	bool	lRet = false;

	if	(mPaused != pPause)
	{
		mPaused = pPause;
		lRet = true;
	}
	return lRet;
}

bool CQueuedWait::Abort (CQueuedActions & pQueue, CAgentWnd * pAgentWnd, HRESULT pReqStatus, LPCTSTR pReason)
{
	return false;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CQueuedInterrupt::CQueuedInterrupt (long pOtherCharID, long pOtherReqID, long pCharID, long pReqID)
:	CQueuedAction (QueueActionInterrupt, pCharID, pReqID),
	mOtherCharID (pOtherCharID),
	mOtherReqID (pOtherReqID)
{
}

CQueuedInterrupt::~CQueuedInterrupt ()
{
}

//////////////////////////////////////////////////////////////////////

bool CQueuedInterrupt::Advance (CQueuedActions & pQueue, CAgentWnd * pAgentWnd)
{
	bool					lRet = false;
	CQueuedAction *			lOtherRequest;
	CAgentCharacterWnd *	lRequestOwner = NULL;
	HRESULT					lStatus;

	if	(mPaused)
	{
		lRet = true;
	}
	else
	{
#ifdef	_LOG_QUEUE_OPS
		if	(LogIsActive (_LOG_QUEUE_OPS))
		{
			LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] QueuedInterrupt [%p(%d)] Other [%d(%d)]"), pAgentWnd, mCharID, this, mReqID, mOtherCharID, mOtherReqID);
		}
#endif
		if	(!mStarted)
		{
			NotifyStarted (pAgentWnd->mNotify);
		}
		if	(pQueue.GetNextAction (QueueActionInterrupt) == this)
		{
			if	(
					(lRequestOwner = dynamic_cast <CAgentCharacterWnd *> (pAgentWnd))
				&&	(lOtherRequest = lRequestOwner->FindOtherRequest (mOtherReqID, lRequestOwner))
				&&	(lRequestOwner)
				&&	(lRequestOwner->RemoveQueuedAction (lOtherRequest, AGENTREQERR_INTERRUPTEDCODE, _T("Interrupt")))
				)
			{
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)]   Interrupted [%p(%d)(%d)]"), pAgentWnd, mCharID, lOtherRequest, mOtherCharID, mOtherReqID);
				}
#endif
				lStatus = S_OK;
			}
			else
			{
				lStatus = AGENTREQERR_REMOVED;
			}

			pQueue.RemoveHead ();
			if	(pAgentWnd->mNotify.GetCount() > 0)
			{
				NotifyComplete (pAgentWnd->mNotify, lStatus);
			}
		}
		else
		{
			lRet = true; // Was deleted during NotifyStarted
		}
	}
	return lRet;
}

bool CQueuedInterrupt::Pause (CQueuedActions & pQueue, CAgentWnd * pAgentWnd, bool pPause)
{
	bool	lRet = false;

	if	(mPaused != pPause)
	{
		mPaused = pPause;
		lRet = true;
	}
	return lRet;
}

bool CQueuedInterrupt::Abort (CQueuedActions & pQueue, CAgentWnd * pAgentWnd, HRESULT pReqStatus, LPCTSTR pReason)
{
	return false;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

void CQueuedAction::LogAction (UINT pLogLevel, LPCTSTR pFormat, ...) const
{
#if defined(_DEBUG) || defined (_TRACE_ACTION_INSTANCE)
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString	lTitle;
			if	(pFormat)
			{
				va_list lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);
				lTitle.ReleaseBuffer ();
			}
			_LogAction (pLogLevel, lTitle, _T("Action"));
		}
		catch AnyExceptionSilent
	}
#endif
}

void CQueuedAction::_LogAction (UINT pLogLevel, LPCTSTR pTitle, LPCTSTR pActionType, LPCTSTR pFormat, ...) const
{
#if defined(_DEBUG) || defined (_TRACE_ACTION_INSTANCE)
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString	lTitle (pTitle);
			CAtlString	lIndent;
			CAtlString	lDetails;

			if	(pFormat)
			{
				va_list lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lDetails.GetBuffer(2048), 2048, pFormat, lArgPtr);
				lDetails.ReleaseBuffer ();
				if	(!lDetails.IsEmpty ())
				{
					lDetails.Insert (0, _T(' '));
				}
			}
			if	(!lTitle.IsEmpty())
			{
				lIndent = lTitle;
				lTitle.TrimLeft ();
				if	(lIndent.GetLength() > lTitle.GetLength())
				{
					lIndent = CAtlString (_T(' '), lIndent.GetLength() - lTitle.GetLength ());
				}
				else
				{
					lIndent.Empty ();
				}
			}
			if	(lTitle.IsEmpty())
			{
				lTitle.Format (_T("%-10s"), pActionType);
			}
			else
			{
				lTitle.Format (_T("%s %-10s"), CAtlString((LPCTSTR)lTitle), pActionType);
			}
			LogMessage (pLogLevel, _T("%s%s [%d %d] Started [%u (%u)]%s"), lIndent, lTitle, mCharID, mReqID, mStarted, mPaused, lDetails);
		}
		catch AnyExceptionSilent
	}
#endif
}

//////////////////////////////////////////////////////////////////////

void CQueuedState::LogAction (UINT pLogLevel, LPCTSTR pFormat, ...) const
{
#if defined(_DEBUG) || defined (_TRACE_ACTION_INSTANCE)
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString	lTitle;
			if	(pFormat)
			{
				va_list lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);
				lTitle.ReleaseBuffer ();
			}
			_LogAction (pLogLevel, lTitle, _T("State"), _T("Name [%s] Gestures [%s]"), mStateName, JoinStringArray(mGestures, _T(" ")));
		}
		catch AnyExceptionSilent
	}
#endif
}

void CQueuedGesture::LogAction (UINT pLogLevel, LPCTSTR pFormat, ...) const
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString	lTitle;
			if	(pFormat)
			{
				va_list lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);
				lTitle.ReleaseBuffer ();
			}
			_LogAction (pLogLevel, lTitle, _T("Gesture"), _T("Name [%s] State [%s]"), mGestureName, mStateName);
		}
		catch AnyExceptionSilent
	}
#endif
}

//////////////////////////////////////////////////////////////////////

void CQueuedShow::LogAction (UINT pLogLevel, LPCTSTR pFormat, ...) const
{
#if defined(_DEBUG) || defined (_TRACE_ACTION_INSTANCE)
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString	lTitle;
			if	(pFormat)
			{
				va_list lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);
				lTitle.ReleaseBuffer ();
			}
			_LogAction (pLogLevel, lTitle, _T("Show"), _T("Fast [%u] Animated [%u] Cause [%d]"), mFast, mAnimationShown, mVisibilityCause);
		}
		catch AnyExceptionSilent
	}
#endif
}

void CQueuedHide::LogAction (UINT pLogLevel, LPCTSTR pFormat, ...) const
{
#if defined(_DEBUG) || defined (_TRACE_ACTION_INSTANCE)
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString	lTitle;
			if	(pFormat)
			{
				va_list lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);
				lTitle.ReleaseBuffer ();
			}
			_LogAction (pLogLevel, lTitle, _T("Hide"), _T("Fast [%u] Animated [%u] Cause [%d]"), mFast, mAnimationShown, mVisibilityCause);
		}
		catch AnyExceptionSilent
	}
#endif
}

void CQueuedMove::LogAction (UINT pLogLevel, LPCTSTR pFormat, ...) const
{
#if defined(_DEBUG) || defined (_TRACE_ACTION_INSTANCE)
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString	lTitle;
			if	(pFormat)
			{
				va_list lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);
				lTitle.ReleaseBuffer ();
			}
			_LogAction (pLogLevel, lTitle, _T("Move"), _T("To [%d %d] Started [%d %d] Animation [%s] Shown [%u %u]"), mPosition.x, mPosition.y, mMoveStarted?mMoveStarted->x:-1, mMoveStarted?mMoveStarted->y:-1, mAnimationState, mAnimationShown, mEndAnimationShown);
		}
		catch AnyExceptionSilent
	}
#endif
}

//////////////////////////////////////////////////////////////////////

void CQueuedWait::LogAction (UINT pLogLevel, LPCTSTR pFormat, ...) const
{
#if defined(_DEBUG) || defined (_TRACE_ACTION_INSTANCE)
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString	lTitle;
			if	(pFormat)
			{
				va_list lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);
				lTitle.ReleaseBuffer ();
			}
			_LogAction (pLogLevel, lTitle, _T("Wait"), _T("OtherChar [%d %d]"), mOtherCharID, mOtherReqID);
		}
		catch AnyExceptionSilent
	}
#endif
}

void CQueuedInterrupt::LogAction (UINT pLogLevel, LPCTSTR pFormat, ...) const
{
#if defined(_DEBUG) || defined (_TRACE_ACTION_INSTANCE)
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString	lTitle;
			if	(pFormat)
			{
				va_list lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);
				lTitle.ReleaseBuffer ();
			}
			_LogAction (pLogLevel, lTitle, _T("Interrupt"), _T("OtherChar [%d %d]"), mOtherCharID, mOtherReqID);
		}
		catch AnyExceptionSilent
	}
#endif
}
