/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
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
#pragma once
#include "DirectShowWnd.h"
#include "AgentFile.h"
#include "AgentFileCache.h"
#include "AgentStreamUtils.h"
#include "QueuedActions.h"
#include "EventNotify.h"

/////////////////////////////////////////////////////////////////////////////

class CQueuedAction;

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CAgentWnd :
	public CDirectShowWnd,
	public CAgentFileClient,
	public CEventNotifiesClient<CAgentWnd>,
	public CAgentStreamUtils
{
	DECLARE_DLL_OBJECT_EX(CAgentWnd, _DACORE_IMPEXP)
protected:
	CAgentWnd ();
public:
	_DACORE_IMPEXP virtual ~CAgentWnd ();

// Attributes
public:
	_DACORE_IMPEXP CAgentFile * GetAgentFile () const {return CAgentStreamUtils::GetAgentFile ();}

// Operations
public:
	_DACORE_IMPEXP bool Open (CAgentFile * pAgentFile);

	_DACORE_IMPEXP SAFEARRAY * GetStateNames ();
	_DACORE_IMPEXP SAFEARRAY * GetGestureNames ();
	_DACORE_IMPEXP SAFEARRAY * GetAnimationNames ();

	_DACORE_IMPEXP bool ShowState (LPCTSTR pStateName, bool pStopIdle = true, bool pClearQueue = false);
	_DACORE_IMPEXP bool ShowGesture (LPCTSTR pGestureName, LPCTSTR pForState = NULL, bool pStopIdle = true, bool pClearQueue = false);
	_DACORE_IMPEXP bool ShowAnimation (LPCTSTR pAnimationName, bool pStopIdle = true, bool pClearQueue = false);

	_DACORE_IMPEXP bool IsShowingState (LPCTSTR pStateName);
	_DACORE_IMPEXP bool IsShowingGesture (LPCTSTR pGestureName, LPCTSTR pForState = NULL);
	_DACORE_IMPEXP bool IsShowingAnimation (LPCTSTR pAnimationName);

	_DACORE_IMPEXP long QueueState (long pCharID, LPCTSTR pStateName);
	_DACORE_IMPEXP CQueuedAction * FindQueuedState (long pCharID, LPCTSTR pStateName);
	_DACORE_IMPEXP tBstrPtr GetQueuedState (CQueuedAction * pQueuedState);
	_DACORE_IMPEXP bool RemoveQueuedState (CQueuedAction * pQueuedState, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);
	_DACORE_IMPEXP bool RemoveQueuedState (long pCharID, LPCTSTR pStateName, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);
	_DACORE_IMPEXP bool ShowStateGestures (long pCharID, LPCTSTR pStateName, bool pQueuedState);

	_DACORE_IMPEXP long QueueGesture (long pCharID, LPCTSTR pGestureName, LPCTSTR pForState = NULL);
	_DACORE_IMPEXP CQueuedAction * FindQueuedGesture (long pCharID, LPCTSTR pGestureName, LPCTSTR pForState = NULL);
	_DACORE_IMPEXP tBstrPtr GetQueuedGesture (CQueuedAction * pQueuedGesture);
	_DACORE_IMPEXP bool RemoveQueuedGesture (CQueuedAction * pQueuedGesture, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);

	_DACORE_IMPEXP bool PutQueuedAction (CQueuedAction * pQueuedAction);
	_DACORE_IMPEXP bool RemoveQueuedAction (CQueuedAction * pQueuedAction, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);
	_DACORE_IMPEXP CQueuedAction * FindQueuedAction (long pReqID);
	_DACORE_IMPEXP CQueuedAction * NextQueuedAction (long pCharID = -1);
	_DACORE_IMPEXP UINT HasQueuedActions (long pCharID = -1);

	_DACORE_IMPEXP bool RemoveQueuedActions (QueueAction pAction, long pCharID = -1, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL, bool pExcludeActive = false);
	_DACORE_IMPEXP bool ClearQueuedActions (long pCharID = -1, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);
	_DACORE_IMPEXP bool ClearQueuedStates (long pCharID = -1, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL, bool pExcludeActive = false, LPCTSTR pExcludeState = NULL, ...);
	_DACORE_IMPEXP bool ClearQueuedGestures (long pCharID = -1, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL, bool pExcludeActive = false, LPCTSTR pExcludeState = NULL, ...);

	_DACORE_IMPEXP UINT IsQueueBusy () const;
	_DACORE_IMPEXP bool IsQueuePaused () const;
	_DACORE_IMPEXP UINT_PTR IsQueueActive () const;
	_DACORE_IMPEXP UINT_PTR ActivateQueue (bool pImmediate, DWORD pQueueTime = 0);
	_DACORE_IMPEXP UINT_PTR SuspendQueue ();
	_DACORE_IMPEXP bool PauseQueue (bool pPause);

	_DACORE_IMPEXP bool IsSoundEnabled (bool pIgnoreGlobalConfig = false) const;
	_DACORE_IMPEXP bool EnableSound (bool pEnable);

	_DACORE_IMPEXP bool SetBkColor (COLORREF pBkColor);
	_DACORE_IMPEXP bool GetBkColor (COLORREF & pBkColor);
	_DACORE_IMPEXP bool ResetBkColor ();

	_DACORE_IMPEXP bool EnableIdle (bool pEnable);
	_DACORE_IMPEXP bool IsIdleEnabled () const;
	_DACORE_IMPEXP virtual int IsIdle () const;
	_DACORE_IMPEXP virtual bool StopIdle (LPCTSTR pReason = NULL);

	_DACORE_IMPEXP bool IsAnimationComplete (bool pPauseAtEndOfStream = false);
	_DACORE_IMPEXP bool ClearAnimations ();
	_DACORE_IMPEXP bool DidAnimations ();
	_DACORE_IMPEXP bool UpdateActiveMedia ();
	_DACORE_IMPEXP bool MakeActiveMedia (bool pActive);

// Overrides
public:
	_DACORE_IMPEXP virtual HWND Create (HWND hWndParent, _U_RECT rect = NULL, LPCTSTR szWindowName = NULL, DWORD dwStyle = 0, DWORD dwExStyle = 0, _U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL);
	_DACORE_IMPEXP virtual bool Open (LPCTSTR pFileName);
	_DACORE_IMPEXP virtual HRESULT Start (DWORD pWaitForCompletion = 100);
	_DACORE_IMPEXP virtual HRESULT Stop (DWORD pWaitForCompletion = 100);
	_DACORE_IMPEXP virtual DWORD GetAlphaSmoothing () const;
protected:
	_DACORE_IMPEXP virtual void Opening (LPCTSTR pFileName);
	_DACORE_IMPEXP virtual void Opened ();
	_DACORE_IMPEXP virtual void Closing ();
	_DACORE_IMPEXP virtual void Closed ();
	_DACORE_IMPEXP virtual HRESULT PrepareGraph (LPCTSTR pFileName);
	_DACORE_IMPEXP virtual COLORREF GetEraseColor ();
	_DACORE_IMPEXP virtual bool PaintWindow (HDC pDC);

// Implementation
protected:
	_DACORE_IMPEXP LRESULT OnTimer (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	_DACORE_IMPEXP LRESULT OnMediaEvent (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);

	BEGIN_MSG_MAP(CAgentWnd)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(mEventMsg, OnMediaEvent)
		REFLECT_NOTIFICATIONS()
		CHAIN_MSG_MAP(CDirectShowWnd)
	END_MSG_MAP()

protected:
	_DACORE_IMPEXP int PreDoQueue ();
	_DACORE_IMPEXP int PostDoQueue ();
	_DACORE_IMPEXP virtual int _PreDoQueue ();
	_DACORE_IMPEXP virtual int _PostDoQueue ();

	_DACORE_IMPEXP virtual bool CanDoAnimationQueue ();
	_DACORE_IMPEXP virtual bool DoAnimationQueue (bool & pNextActivateImmediate, DWORD & pNextQueueTime);
	_DACORE_IMPEXP bool DoAnimationLoop ();
	_DACORE_IMPEXP virtual bool DoIdle ();

	_DACORE_IMPEXP virtual void PauseQueuedAction (CQueuedAction * pQueuedAction, bool pPause);
	_DACORE_IMPEXP virtual void AbortQueuedAction (CQueuedAction * pQueuedAction, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);

	_DACORE_IMPEXP bool AnimationSequenceChanging (bool pStopNow = false, bool pStopAtEndOfStream = true);
	_DACORE_IMPEXP void AnimationSequenceChanged ();

public:
	_DACORE_IMPEXP long GetCurrPosMs ();
	_DACORE_IMPEXP long GetStopPosMs ();
	_DACORE_IMPEXP long GetDurationMs ();
	_DACORE_IMPEXP HRESULT PlayFromTo (long pStartPosMs, long pStopPosMs, bool pRestart, DWORD pWaitForCompletion = 1000);

	_DACORE_IMPEXP long NextReqID () const;

protected:
	CQueuedActions								mQueue;
	int											mIdleLevel;
	UINT_PTR									mIdleTimer;
	bool										mIdleStarted;
	CAtlStringArray								mIdleQueue;
	bool										mEnableSoundFlag;
	_IAgentStreamSourcePtr						mSourceFilter;
	_IAgentStreamRenderPtr						mRenderFilter;
	tPtr <COLORREF>								mBkColor;
	GUID										mVideoRenderType;
	mutable LARGE_INTEGER						mStateTraceData;

private:
	mutable volatile UINT						mQueueBusy;
	bool										mQueuePaused;
	IUnknownPtr									mSystemClock;
};

/////////////////////////////////////////////////////////////////////////////
