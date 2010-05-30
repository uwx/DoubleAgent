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

/////////////////////////////////////////////////////////////////////////////
#pragma warning (push)
#pragma warning (disable: 4251 4275)
/////////////////////////////////////////////////////////////////////////////

class CQueuedAction;

class ATL_NO_VTABLE CAgentWndObj :
	public CComObjectRootEx<CComMultiThreadModel>,
	public IOleWindow
{
// Declarations
public:
	DECLARE_NOT_AGGREGATABLE(CAgentWndObj)
	DECLARE_GET_CONTROLLING_UNKNOWN()

	BEGIN_COM_MAP(CAgentWndObj)
		COM_INTERFACE_ENTRY(IOleWindow)
	END_COM_MAP()
};

/////////////////////////////////////////////////////////////////////////////

class _DACORE_IMPEXP ATL_NO_VTABLE CAgentWnd :
	public CAgentWndObj,
	public CDirectShowWnd,
	public CAgentFileClient,
	protected CAgentStreamUtils
{
	DECLARE_DLL_OBJECT(CAgentWnd)
protected:
	CAgentWnd ();
public:
	virtual ~CAgentWnd ();

// Interfaces
public:
	// IOleWindow
    HRESULT STDMETHODCALLTYPE GetWindow (HWND *phwnd);
    HRESULT STDMETHODCALLTYPE ContextSensitiveHelp (BOOL fEnterMode);

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

	long QueueGesture (long pCharID, LPCTSTR pGestureName, LPCTSTR pForState = NULL);
	CQueuedAction * FindQueuedGesture (long pCharID, LPCTSTR pGestureName, LPCTSTR pForState = NULL);
	tBstrPtr GetQueuedGesture (CQueuedAction * pQueuedGesture);
	bool RemoveQueuedGesture (CQueuedAction * pQueuedGesture, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);

	CQueuedAction * FindQueuedAction (long pReqID);
	CQueuedAction * NextQueuedAction (long pCharID = -1);
	UINT HasQueuedActions (long pCharID = -1);
	bool HasQueuedStates (long pCharID = -1);
	bool HasQueuedGestures (long pCharID = -1);

	bool PutQueuedAction (CQueuedAction * pQueuedAction);
	bool RemoveQueuedAction (CQueuedAction * pQueuedAction, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);
	bool ClearQueuedActions (long pCharID = -1, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);
	bool ClearQueuedStates (long pCharID = -1, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL, bool pExcludeActive = false, LPCTSTR pExcludeState = NULL, ...);
	bool ClearQueuedGestures (long pCharID = -1, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL, bool pExcludeActive = false, LPCTSTR pExcludeState = NULL, ...);

	UINT IsQueueBusy () const;
	UINT_PTR IsQueueActive () const;
	UINT_PTR ActivateQueue (bool pImmediate, DWORD pQueueTime = 0);
	UINT_PTR SuspendQueue ();

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
	bool UpdateActiveMedia ();
	bool MakeActiveMedia (bool pActive);

// Overrides
public:
	virtual bool Open (LPCTSTR pFileName);
	virtual HRESULT Start (DWORD pWaitForCompletion = 100);
	virtual HRESULT Stop (DWORD pWaitForCompletion = 100);
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

	bool DoQueuedState ();
	bool DoQueuedGesture ();
	bool DoAnimationLoop ();
	virtual bool DoAnimationQueue ();
	virtual bool DoIdle ();

	virtual void AbortQueuedAction (CQueuedAction * pQueuedAction, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);
	void AbortQueuedState (CQueuedAction * pQueuedAction, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);
	void AbortQueuedGesture (CQueuedAction * pQueuedAction, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);

	bool ShowStateGestures (long pCharID, LPCTSTR pStateName, bool pQueuedState);
	bool AnimationSequenceChanging (bool pStopNow = false, bool pStopAtEndOfStream = true);
	void AnimationSequenceChanged ();
	bool ClearAnimations ();

	long GetCurrPosMs ();
	long GetStopPosMs ();
	long GetDurationMs ();
	HRESULT PlayFromTo (long pStartPosMs, long pStopPosMs, bool pRestart, DWORD pWaitForCompletion = 1000);

	long NextReqID () const;

protected:
	CAtlPtrTypeArray <class CEventNotify>		mNotify;
	CQueuedActions								mQueue;
	UINT_PTR									mQueueTimer;
	DWORD										mQueueTime;
	const DWORD									mQueueTimeMin;
	const DWORD									mQueueTimeMax;
	const DWORD									mQueueTimeDefault;
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
