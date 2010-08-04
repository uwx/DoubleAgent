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
#include "AgentText.h"
#include "SapiVoiceEventSink.h"

/////////////////////////////////////////////////////////////////////////////
#pragma warning (push)
#pragma warning (disable: 4251 4275 4150)
/////////////////////////////////////////////////////////////////////////////

class _DACORE_IMPEXP CAgentCharacterWnd :
	public CAgentWnd,
	public _ISapiVoiceEventSink
{
	DECLARE_DLL_OBJECT(CAgentCharacterWnd)
public:
	CAgentCharacterWnd ();
	virtual ~CAgentCharacterWnd ();

// Attributes
public:
	long GetCharID () const;
	bool IsVisible () const;
	bool IsCharShown () const;

// Operations
public:
	virtual bool Attach (long pCharID, CEventNotify * pNotify, const CAgentIconData * pIconData, bool pSetActiveCharID);
	virtual bool Detach (long pCharID, CEventNotify * pNotify);
	bool SetLastActive ();
	static HWND GetLastActive ();

	class CAgentBalloonWnd * GetBalloonWnd (bool pCreate = false);
	class CAgentListeningWnd * GetListeningWnd (bool pCreate = false);

	long QueueShow (long pCharID, bool pFast = false, int pVisibilityCause = -1);
	long QueueHide (long pCharID, bool pFast = false, int pVisibilityCause = -1);
	long QueueThink (long pCharID, LPCTSTR pText, class CAgentTextObject * pTextObject, UINT pSapiVersion = 5);
	long QueueSpeak (long pCharID, LPCTSTR pText, class CAgentTextObject * pTextObject, LPCTSTR pSoundUrl, class CSapiVoice * pVoice, bool pShowBalloon);
	long QueueWait (long pCharID, long pOtherCharID, long pOtherReqID);
	long QueueInterrupt (long pCharID, long pOtherCharID, long pOtherReqID);

	long IsShowingQueued ();
	long IsHidingQueued ();
	CQueuedAction * IsSpeakQueued (long pCharID = -1);
	bool UpdateQueuedSpeak (long pCharID, class CSapiVoice * pVoice);

// Overrides
public:
	virtual int IsIdle () const;
	virtual bool StopIdle (LPCTSTR pReason = NULL);
protected:
	virtual bool DoIdle ();
	virtual void OnVoiceStart (long pCharID);
	virtual void OnVoiceEnd (long pCharID);
	virtual void OnVoiceBookMark (long pCharID, long pBookMarkId);
	virtual void OnVoiceVisual (long pCharID, int pMouthOverlay);

// Implementation
protected:
	LRESULT OnTimer (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnLButtonDown (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnLButtonUp (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnLButtonDblClk (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnRButtonDown (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnRButtonUp (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnRButtonDblClk (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnMButtonDown (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnMButtonUp (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnMButtonDblClk (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnContextMenu (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnDisplayChange (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnInputLangChange (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnVoiceStartMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnVoiceEndMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnVoiceBookMarkMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnVoiceVisualMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);

	BEGIN_MSG_MAP(CAgentCharacterWnd)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClk)
		MESSAGE_HANDLER(WM_RBUTTONDOWN, OnRButtonDown)
		MESSAGE_HANDLER(WM_RBUTTONUP, OnRButtonUp)
		MESSAGE_HANDLER(WM_RBUTTONDBLCLK, OnRButtonDblClk)
		MESSAGE_HANDLER(WM_MBUTTONDOWN, OnMButtonDown)
		MESSAGE_HANDLER(WM_MBUTTONUP, OnMButtonUp)
		MESSAGE_HANDLER(WM_MBUTTONDBLCLK, OnMButtonDblClk)
		MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
		MESSAGE_HANDLER(WM_DISPLAYCHANGE, OnDisplayChange)
		MESSAGE_HANDLER(WM_INPUTLANGCHANGE, OnInputLangChange)
		MESSAGE_HANDLER(mVoiceStartMsg, OnVoiceStartMsg)
		MESSAGE_HANDLER(mVoiceEndMsg, OnVoiceEndMsg)
		MESSAGE_HANDLER(mVoiceBookMarkMsg, OnVoiceBookMarkMsg)
		MESSAGE_HANDLER(mVoiceVisualMsg, OnVoiceVisualMsg)
		CHAIN_MSG_MAP(CAgentWnd)
	END_MSG_MAP()

public:
	bool KeepBalloonVisible (class CAgentBalloonWnd * pBalloon);

	virtual bool ShowQueued (CQueuedShow * pQueuedShow);
	virtual bool NotifyShown (long pForCharID, VisibilityCauseType pVisiblityCause);
	virtual bool HideQueued (CQueuedHide * pQueuedHide);
	virtual bool NotifyHidden (long pForCharID, VisibilityCauseType pVisiblityCause);

	void SetLastSpeech (CAgentText & pSpeech);
	bool StartMouthAnimation (long pSpeakingDuration = -1);
	bool StopMouthAnimation ();
	bool PlayMouthAnimation (short pMouthOverlayNdx, bool pPlayAlways);

	CQueuedAction * FindOtherRequest (long pReqID, CAgentCharacterWnd *& pRequestOwner);

protected:
	virtual void IsLastActive (bool pLastActive);

	short NotifyKeyState () const;
	void NotifyClick (short pButton, const CPoint & pPoint);
	void NotifyDblClick (short pButton, const CPoint & pPoint);

protected:
	long							mCharID;
	bool							mCharShown;
	class CAgentBalloonWnd *		mBalloonWnd;
	IUnknownPtr						mBalloonRefHolder;
	tPtr <CAgentListeningWnd>		mListeningWnd;
	CAgentText						mLastSpeech;
	UINT							mLastButtonMsg;
	static HWND						mLastActive;
	static const UINT				mVoiceStartMsg;
	static const UINT				mVoiceEndMsg;
	static const UINT				mVoiceBookMarkMsg;
	static const UINT				mVoiceVisualMsg;
};

#pragma warning (pop)
/////////////////////////////////////////////////////////////////////////////
