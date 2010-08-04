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
#pragma once
#include "DirectShowWnd.h"
#include "AgentFile.h"
#include "AgentFileCache.h"
#include "AgentStreamUtils.h"
#include "QueuedActions.h"
#include "EventNotify.h"

/////////////////////////////////////////////////////////////////////////////
#pragma warning (push)
#pragma warning (disable: 4251 4275)
/////////////////////////////////////////////////////////////////////////////

class CQueuedAction;

/////////////////////////////////////////////////////////////////////////////

class _DACORE_IMPEXP ATL_NO_VTABLE CAgentWnd :
	public CDirectShowWnd,
	public CAgentFileClient,
	public CEventNotifiesClient<CAgentWnd>,
	public CAgentStreamUtils
{
	DECLARE_DLL_OBJECT(CAgentWnd)
protected:
	CAgentWnd ();
public:
	virtual ~CAgentWnd ();

// Attributes
public:
	CAgentFile * GetAgentFile () const {return CAgentStreamUtils::GetAgentFile ();}

// Operations
public:
	bool Open (CAgentFile * pAgentFile);

	SAFEARRAY * GetStateNames ();
	SAFEARRAY * GetGestureNames ();
	SAFEARRAY * GetAnimationNames ();

	bool ShowState (LPCTSTR pStateName, bool pStopIdle = true, bool pClearQueue = false);
	bool ShowGesture (LPCTSTR pGestureName, LPCTSTR pForState = NULL, bool pStopIdle = true, bool pClearQueue = false);
	bool ShowAnimation (LPCTSTR pAnimationName, bool pStopIdle = true, bool pClearQueue = false);

	bool IsShowingState (LPCTSTR pStateName);
	bool IsShowingGesture (LPCTSTR pGestureName, LPCTSTR pForState = NULL);
	bool IsShowingAnimation (LPCTSTR pAnimationName);

	long QueueState (long pCharID, LPCTSTR pStateName);
	CQueuedAction * FindQueuedState (long pCharID, LPCTSTR pStateName);
	tBstrPtr GetQueuedState (CQueuedAction * pQueuedState);
	bool RemoveQueuedState (CQueuedAction * pQueuedState, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);
	bool RemoveQueuedState (long pCharID, LPCTSTR pStateName, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);
	bool ShowStateGestures (long pCharID, LPCTSTR pStateName, bool pQueuedState);

	long QueueGesture (long pCharID, LPCTSTR pGestureName, LPCTSTR pForState = NULL);
	CQueuedAction * FindQueuedGesture (long pCharID, LPCTSTR pGestureName, LPCTSTR pForState = NULL);
	tBstrPtr GetQueuedGesture (CQueuedAction * pQueuedGesture);
	bool RemoveQueuedGesture (CQueuedAction * pQueuedGesture, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);

	bool PutQueuedAction (CQueuedAction * pQueuedAction);
	bool RemoveQueuedAction (CQueuedAction * pQueuedAction, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);
	CQueuedAction * FindQueuedAction (long pReqID);
	CQueuedAction * NextQueuedAction (long pCharID = -1);
	UINT HasQueuedActions (long pCharID = -1);

	bool RemoveQueuedActions (QueueAction pAction, long pCharID = -1, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL, bool pExcludeActive = false);
	bool ClearQueuedActions (long pCharID = -1, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);
	bool ClearQueuedStates (long pCharID = -1, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL, bool pExcludeActive = false, LPCTSTR pExcludeState = NULL, ...);
	bool ClearQueuedGestures (long pCharID = -1, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL, bool pExcludeActive = false, LPCTSTR pExcludeState = NULL, ...);

	UINT IsQueueBusy () const;
	UINT IsQueuePaused () const;
	UINT_PTR IsQueueActive () const;
	UINT_PTR ActivateQueue (bool pImmediate, DWORD pQueueTime = 0);
	UINT_PTR SuspendQueue ();
	UINT_PTR PauseQueue (bool pPause);

	bool IsSoundEnabled (bool pIgnoreGlobalConfig = false) const;
	bool EnableSound (bool pEnable);

	bool SetBkColor (COLORREF pBkColor);
	bool GetBkColor (COLORREF & pBkColor);
	bool ResetBkColor ();

	bool EnableIdle (bool pEnable);
	bool IsIdleEnabled () const;
	virtual int IsIdle () const;
	virtual bool StopIdle (LPCTSTR pReason = NULL);

	bool IsAnimationComplete (bool pPauseAtEndOfStream = false);
	bool ClearAnimations ();
	bool UpdateActiveMedia ();
	bool MakeActiveMedia (bool pActive);

// Overrides
public:
	virtual HWND Create (HWND hWndParent, _U_RECT rect = NULL, LPCTSTR szWindowName = NULL, DWORD dwStyle = 0, DWORD dwExStyle = 0, _U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL);
	virtual bool Open (LPCTSTR pFileName);
	virtual HRESULT Start (DWORD pWaitForCompletion = 100);
	virtual HRESULT Stop (DWORD pWaitForCompletion = 100);
	virtual DWORD GetAlphaSmoothing () const;
protected:
	virtual void Opening (LPCTSTR pFileName);
	virtual void Opened ();
	virtual void Closing ();
	virtual void Closed ();
	virtual HRESULT PrepareGraph (LPCTSTR pFileName);
	virtual COLORREF GetEraseColor ();
	virtual bool PaintWindow (HDC pDC);

// Implementation
protected:
	LRESULT OnTimer (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnMediaEvent (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);

	BEGIN_MSG_MAP(CAgentWnd)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(mEventMsg, OnMediaEvent)
		REFLECT_NOTIFICATIONS()
		CHAIN_MSG_MAP(CDirectShowWnd)
	END_MSG_MAP()

protected:
	int PreDoQueue ();
	int PostDoQueue ();
	virtual int _PreDoQueue ();
	virtual int _PostDoQueue ();

	virtual bool CanDoAnimationQueue ();
	virtual bool DoAnimationQueue (bool & pNextActivateImmediate, DWORD & pNextQueueTime);
	bool DoAnimationLoop ();
	virtual bool DoIdle ();

	virtual void PauseQueuedAction (CQueuedAction * pQueuedAction, bool pPause);
	virtual void AbortQueuedAction (CQueuedAction * pQueuedAction, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);

	bool AnimationSequenceChanging (bool pStopNow = false, bool pStopAtEndOfStream = true);
	void AnimationSequenceChanged ();

public:
	long GetCurrPosMs ();
	long GetStopPosMs ();
	long GetDurationMs ();
	HRESULT PlayFromTo (long pStartPosMs, long pStopPosMs, bool pRestart, DWORD pWaitForCompletion = 1000);

	long NextReqID () const;

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
	IUnknownPtr									mSystemClock;
};

#pragma warning (pop)
/////////////////////////////////////////////////////////////////////////////
