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
#include "AgentWnd.h"
#include "AgentNotifyIcon.h"
#include "AgentListeningWnd.h"
#include "SapiVoiceEventSink.h"

/////////////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable: 4251 4275 4150)
/////////////////////////////////////////////////////////////////////////////

class _DACORE_IMPEXP ATL_NO_VTABLE CAgentPopupWnd : public CAgentWnd, public _ISapiVoiceEventSink
{
	DECLARE_DLL_OBJECT(CAgentPopupWnd)
protected:
	CAgentPopupWnd ();
public:
	virtual ~CAgentPopupWnd ();
	static CAgentPopupWnd * CreateInstance ();

// Declarations
public:
	DECLARE_NOT_AGGREGATABLE(CAgentPreviewWnd)
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_PROTECT_FINAL_RELEASE(CAgentWnd)
	DECLARE_WND_CLASS_EX(NULL, CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW|CS_NOCLOSE, COLOR_WINDOW)

// Attributes
public:
	long GetCharID () const;
	bool IsDragging () const;

// Operations
public:
	bool Create (HWND pParentWnd, CRect * pInitialRect = NULL);
	bool Attach (long pCharID, interface _IServerNotify * pNotify, const CAgentIconData * pIconData, bool pSetActiveCharID);
	bool Detach (long pCharID, interface _IServerNotify * pNotify);
	void FinalRelease ();

	class CAgentBalloonWnd * GetBalloonWnd (bool pCreate = false);
	class CAgentListeningWnd * GetListeningWnd (bool pCreate = false);
	bool SetLastActive (HWND pLastActive);
	static HWND GetLastActive ();

	bool ShowPopup (long pForCharID, long pVisiblityCause, bool pAlwaysNotify = false);
	bool HidePopup (long pForCharID, long pVisiblityCause, bool pAlwaysNotify = false);
	bool MovePopup (const CPoint & pPosition, long pForCharID, long pMoveCause, bool pAlwaysNotify = false);
	bool SizePopup (const CSize & pSize, long pForCharID, bool pAlwaysNotify = false);

	long QueueShow (long pCharID, bool pFast = false, int pVisibilityCause = -1);
	CQueuedAction * IsShowQueued (long pCharID = -1);
	long IsShowingQueued ();
	bool RemoveQueuedShow (long pCharID = -1, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL, bool pExcludeActive = false);

	long QueueHide (long pCharID, bool pFast = false, int pVisibilityCause = -1);
	CQueuedAction * IsHideQueued (long pCharID = -1);
	long IsHidingQueued ();
	bool RemoveQueuedHide (long pCharID = -1, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL, bool pExcludeActive = false);

	long QueueMove (long pCharID, const CPoint & pPosition, DWORD pSpeed);
	CQueuedAction * IsMoveQueued (long pCharID = -1);
	bool RemoveQueuedMove (long pCharID = -1, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL, bool pExcludeActive = false);

	long QueueThink (long pCharID, LPCTSTR pText);
	CQueuedAction * IsThinkQueued (long pCharID = -1);
	bool RemoveQueuedThink (long pCharID = -1, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL, bool pExcludeActive = false);

	long QueueSpeak (long pCharID, LPCTSTR pText, LPCTSTR pSoundUrl, class CSapiVoice * pVoice, bool pShowBalloon);
	CQueuedAction * IsSpeakQueued (long pCharID = -1);
	bool RemoveQueuedSpeak (long pCharID = -1, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL, bool pExcludeActive = false);
	bool UpdateQueuedSpeak (long pCharID, class CSapiVoice * pVoice);

	long QueueWait (long pCharID, long pOtherCharID, long pOtherReqID);
	CQueuedAction * IsWaitQueued (long pCharID = -1);
	bool RemoveQueuedWait (long pCharID = -1, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL, bool pExcludeActive = false);

	long QueueInterrupt (long pCharID, long pOtherCharID, long pOtherReqID);
	CQueuedAction * IsInterruptQueued (long pCharID = -1);
	bool RemoveQueuedInterrupt (long pCharID = -1, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL, bool pExcludeActive = false);

	CQueuedAction * IsPrepareQueued (long pCharID = -1);
	bool RemoveQueuedPrepare (long pCharID = -1, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL, bool pExcludeActive = false);

	bool IsNotifyIconValid () const;
	bool IsIconVisible () const;
	bool UpdateNotifyIcon (const CAgentIconData * pIconData = NULL);
	bool SetNotifyIconTip (const CAgentIconData * pIconData, CAgentFile * pAgentFile, LANGID pLangID = 0);

// Overrides
public:
	virtual int IsIdle () const;
	virtual bool StopIdle (LPCTSTR pReason = NULL);
protected:
	virtual void Opened ();
	virtual void Closing ();
	virtual bool DoIdle ();
	virtual void AbortQueuedAction (CQueuedAction * pQueuedAction, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);
	virtual void OnVoiceStart (long pCharID);
	virtual void OnVoiceEnd (long pCharID);
	virtual void OnVoiceBookMark (long pCharID, long pBookMarkId);
	virtual void OnVoiceVisual (long pCharID, int pMouthOverlay);
	virtual int _PreDoQueue ();
	virtual int _PostDoQueue ();
	virtual void OnFinalMessage (HWND);

// Implementation
protected:
	LRESULT OnDestroy (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnTimer (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnNcHitTest (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnNcLButtonDown (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnNcLButtonUp (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnNcLButtonDblClk (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnNcRButtonDown (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnNcRButtonUp (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnNcRButtonDblClk (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnNcMButtonDown (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnNcMButtonUp (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnNcMButtonDblClk (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnEnterSizeMove (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnExitSizeMove (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnMoving (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnMove (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnMouseActivate (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnActivate (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnContextMenu (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnDisplayChange (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnInputLangChange (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnNotifyIcon (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnTaskbarCreated (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnVoiceStartMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnVoiceEndMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnVoiceBookMarkMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnVoiceVisualMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);

	BEGIN_MSG_MAP(CAgentPopupWnd)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_NCHITTEST, OnNcHitTest)
		MESSAGE_HANDLER(WM_NCLBUTTONDOWN, OnNcLButtonDown)
		MESSAGE_HANDLER(WM_NCLBUTTONUP, OnNcLButtonUp)
		MESSAGE_HANDLER(WM_NCLBUTTONDBLCLK, OnNcLButtonDblClk)
		MESSAGE_HANDLER(WM_NCRBUTTONDOWN, OnNcRButtonDown)
		MESSAGE_HANDLER(WM_NCRBUTTONUP, OnNcRButtonUp)
		MESSAGE_HANDLER(WM_NCRBUTTONDBLCLK, OnNcRButtonDblClk)
		MESSAGE_HANDLER(WM_NCMBUTTONDOWN, OnNcMButtonDown)
		MESSAGE_HANDLER(WM_NCMBUTTONUP, OnNcMButtonUp)
		MESSAGE_HANDLER(WM_NCMBUTTONDBLCLK, OnNcMButtonDblClk)
		MESSAGE_HANDLER(WM_ENTERSIZEMOVE, OnEnterSizeMove)
		MESSAGE_HANDLER(WM_EXITSIZEMOVE, OnExitSizeMove)
		MESSAGE_HANDLER(WM_MOVING, OnMoving)
		MESSAGE_HANDLER(WM_MOVE, OnMove)
		MESSAGE_HANDLER(WM_MOUSEACTIVATE, OnMouseActivate)
		MESSAGE_HANDLER(WM_ACTIVATE, OnActivate)
		MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
		MESSAGE_HANDLER(WM_DISPLAYCHANGE, OnDisplayChange)
		MESSAGE_HANDLER(WM_INPUTLANGCHANGE, OnInputLangChange)
		MESSAGE_HANDLER(CAgentNotifyIcon::mNotifyIconMsg, OnNotifyIcon)
		MESSAGE_HANDLER(CAgentNotifyIcon::mTaskbarCreatedMsg, OnTaskbarCreated)
		MESSAGE_HANDLER(mVoiceStartMsg, OnVoiceStartMsg)
		MESSAGE_HANDLER(mVoiceEndMsg, OnVoiceEndMsg)
		MESSAGE_HANDLER(mVoiceBookMarkMsg, OnVoiceBookMarkMsg)
		MESSAGE_HANDLER(mVoiceVisualMsg, OnVoiceVisualMsg)
		CHAIN_MSG_MAP(CAgentWnd)
	END_MSG_MAP()

public:
	bool KeepBalloonVisible (class CAgentBalloonWnd * pBalloon);

protected:
	bool DoQueuedShow ();
	bool DoQueuedHide ();
	bool DoQueuedMove ();
	bool DoQueuedMoveCycle (CQueuedMove * pQueuedMove = NULL);
	bool DoQueuedThink ();
	bool DoQueuedSpeak ();
	bool DoQueuedWait ();
	bool DoQueuedInterrupt ();
	bool DoQueuedPrepare ();

	void AbortQueuedShow (CQueuedAction * pQueuedAction, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);
	void AbortQueuedHide (CQueuedAction * pQueuedAction, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);
	void AbortQueuedMove (CQueuedAction * pQueuedAction, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);
	void AbortQueuedThink (CQueuedAction * pQueuedAction, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);
	void AbortQueuedSpeak (CQueuedAction * pQueuedAction, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);
	void AbortQueuedWait (CQueuedAction * pQueuedAction, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);
	void AbortQueuedInterrupt (CQueuedAction * pQueuedAction, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);
	void AbortQueuedPrepare (CQueuedAction * pQueuedAction, HRESULT pReqStatus = 0, LPCTSTR pReason = NULL);

	bool SpeechIsBusy (CQueuedSpeak * pQueuedSpeak);
	HRESULT SpeechIsReady (CQueuedSpeak * pQueuedSpeak);
	HRESULT PrepareSpeech (CQueuedSpeak * pQueuedSpeak);
	HRESULT StartSpeech (CQueuedSpeak * pQueuedSpeak);
	bool ShowSpeechAnimation (CQueuedSpeak * pQueuedSpeak);
	bool StartMouthAnimation (long pSpeakingDuration = -1);
	bool StopMouthAnimation ();
	bool PlayMouthAnimation (short pMouthOverlayNdx, bool pPlayAlways);

	CQueuedAction * FindOtherRequest (long pReqID, CAgentPopupWnd *& pRequestOwner);

	short NotifyKeyState () const;
	void NotifyClick (short pButton, const CPoint & pPoint);
	void NotifyDblClick (short pButton, const CPoint & pPoint);
	void OnIconDblClick (const CPoint & pPoint);

public:
	bool PreNotify ();
	bool PostNotify ();
	UINT IsInNotify () const;

protected:
	long							mCharID;
	class CAgentBalloonWnd *		mBalloonWnd;
	IUnknownPtr						mBalloonRefHolder;
	tPtr <CAgentListeningWnd>		mListeningWnd;
	tPtr <CPoint>					mSizeMoveStart;
	bool							mIsDragging;
	bool							mWasDragged;
	CAgentText						mLastSpeech;
	CAgentNotifyIcon				mNotifyIcon;
	static HWND						mLastActive;
	static UINT						mVoiceStartMsg;
	static UINT						mVoiceEndMsg;
	static UINT						mVoiceBookMarkMsg;
	static UINT						mVoiceVisualMsg;
private:
	UINT							mInNotify;
	UINT							mLastButtonMsg;
};

#pragma warning(pop)
/////////////////////////////////////////////////////////////////////////////
