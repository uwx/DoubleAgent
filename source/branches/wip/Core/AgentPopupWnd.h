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
#ifndef	AGENTPOPUPWND_H_INCLUDED_
#define AGENTPOPUPWND_H_INCLUDED_
#pragma once

#include "AgentWnd.h"
#include "AgentNotifyIcon.h"
#include "SapiVoiceEventSink.h"

/////////////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable: 4251 4275)

class _DACORE_IMPEXP CAgentPopupWnd : public CAgentWnd, public ISapiVoiceEventSink
{
protected:
	CAgentPopupWnd ();
public:
	virtual ~CAgentPopupWnd ();
	DECLARE_DYNCREATE(CAgentPopupWnd)

// Attributes
public:
	long GetCharID () const;
	bool IsDragging () const;

// Operations
public:
	bool Create (HWND pParentWnd, CRect * pInitialRect = NULL);
	bool Attach (long pCharID, IDaNotify * pNotify, const CAgentIconData * pIconData, bool pSetActiveCharID);
	bool Detach (long pCharID, IDaNotify * pNotify);

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
	//{{AFX_VIRTUAL(CAgentPopupWnd)
	public:
	virtual void OnFinalRelease ();
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
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAgentPopupWnd)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg _MFC_NCHITTEST_RESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcRButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnNcRButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnNcMButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcMButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnNcMButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg LRESULT OnEnterSizeMove(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnExitSizeMove(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMoving(UINT nSide, LPRECT lpRect);
	afx_msg void OnMove(int x, int y);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnContextMenu(CWnd *pWnd, CPoint pos);
	afx_msg LRESULT OnDisplayChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInputLangChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotifyIcon(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTaskbarCreated (WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnVoiceStartMsg (WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnVoiceEndMsg (WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnVoiceBookMarkMsg (WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnVoiceVisualMsg (WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

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
	bool StartMouthAnimation (long pSpeakingDuration = 0);
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
	tPtr <class CAgentBalloonWnd>	mBalloonWnd;
	tPtr <class CAgentListeningWnd>	mListeningWnd;
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

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // AGENTPOPUPWND_H_INCLUDED_
