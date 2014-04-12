/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
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
#include "SapiVoice.h"

/////////////////////////////////////////////////////////////////////////////

class CAgentCharacterWnd :
	public CAgentWnd,
	public CNotifySourcesOwner2 <CSapiVoice, _ISapiVoiceEventSink, CAgentCharacterWnd>,
	public _ISapiVoiceEventSink
{
	DECLARE_DLL_OBJECT_EX(CAgentCharacterWnd, _DACORE_IMPEXP)
public:
	_DACORE_IMPEXP CAgentCharacterWnd ();
	_DACORE_IMPEXP virtual ~CAgentCharacterWnd ();

// Attributes
public:
	_DACORE_IMPEXP long GetCharID () const;
	_DACORE_IMPEXP bool IsVisible () const;
	_DACORE_IMPEXP bool IsCharShown () const;

// Operations
public:
	_DACORE_IMPEXP virtual bool Attach (long pCharID, CEventNotify* pNotify, const CAgentIconData* pIconData, bool pSetActiveCharID);
	_DACORE_IMPEXP virtual bool Detach (long pCharID, CEventNotify* pNotify);
	_DACORE_IMPEXP bool SetLastActive ();
	_DACORE_IMPEXP static HWND GetLastActive ();

	_DACORE_IMPEXP class CAgentBalloonWnd* GetBalloonWnd (bool pCreate = false);
	_DACORE_IMPEXP class CAgentListeningWnd* GetListeningWnd (bool pCreate = false);

	_DACORE_IMPEXP long QueueShow (long pCharID, bool pFast = false, int pVisibilityCause = -1);
	_DACORE_IMPEXP long QueueHide (long pCharID, bool pFast = false, int pVisibilityCause = -1);
	_DACORE_IMPEXP long QueueThink (long pCharID, LPCTSTR pText, class CAgentTextObject* pTextObject, class CAgentBalloonOptions* pBalloonOptions, UINT pSapiVersion = 5);
	_DACORE_IMPEXP long QueueSpeak (long pCharID, LPCTSTR pText, class CAgentTextObject* pTextObject, LPCTSTR pSoundUrl, class CSapiVoice* pVoice, class CAgentBalloonOptions* pBalloonOptions);
	_DACORE_IMPEXP long QueueWait (long pCharID, long pOtherCharID, long pOtherReqID);
	_DACORE_IMPEXP long QueueInterrupt (long pCharID, long pOtherCharID, long pOtherReqID);

	_DACORE_IMPEXP long IsShowingQueued ();
	_DACORE_IMPEXP long IsHidingQueued ();
	_DACORE_IMPEXP CQueuedAction* IsSpeakQueued (long pCharID = -1);
	_DACORE_IMPEXP bool UpdateQueuedSpeak (long pCharID, class CSapiVoice* pVoice);

// Overrides
public:
	_DACORE_IMPEXP virtual int IsIdle () const;
	_DACORE_IMPEXP virtual bool StopIdle (LPCTSTR pReason = NULL);
protected:
	_DACORE_IMPEXP virtual bool DoIdle ();
	_DACORE_IMPEXP virtual void OnVoiceStart (long pCharID);
	_DACORE_IMPEXP virtual void OnVoiceEnd (long pCharID);
	_DACORE_IMPEXP virtual void OnVoiceBookMark (long pCharID, long pBookMarkId);
	_DACORE_IMPEXP virtual void OnVoiceVisual (long pCharID, int pMouthOverlay);

// Implementation
protected:
	_DACORE_IMPEXP LRESULT OnTimer (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnLButtonDown (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnLButtonUp (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnLButtonDblClk (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnRButtonDown (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnRButtonUp (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnRButtonDblClk (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnMButtonDown (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnMButtonUp (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnMButtonDblClk (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnContextMenu (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnDisplayChange (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnInputLangChange (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnVoiceStartMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnVoiceEndMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnVoiceBookMarkMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnVoiceVisualMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

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
	bool KeepBalloonVisible (class CAgentBalloonWnd* pBalloon);

	_DACORE_IMPEXP virtual bool ShowQueued (CQueuedShow* pQueuedShow);
	_DACORE_IMPEXP virtual bool NotifyShown (long pForCharID, VisibilityCauseType pVisiblityCause);
	_DACORE_IMPEXP virtual bool HideQueued (CQueuedHide* pQueuedHide);
	_DACORE_IMPEXP virtual bool NotifyHidden (long pForCharID, VisibilityCauseType pVisiblityCause);

	void SetLastSpeech (CAgentText& pSpeech);
	bool StartMouthAnimation (long pSpeakingDuration = -1);
	bool StopMouthAnimation ();
	bool PlayMouthAnimation (short pMouthOverlayNdx, bool pPlayAlways);

	CQueuedAction* FindOtherRequest (long pReqID, CAgentCharacterWnd*& pRequestOwner);

protected:
	_DACORE_IMPEXP virtual void IsLastActive (bool pLastActive);

	short NotifyKeyState () const;
	void NotifyClick (short pButton, const CPoint& pPoint);
	void NotifyDblClick (short pButton, const CPoint& pPoint);

protected:
	long								mCharID;
	bool								mCharShown;
	class CAgentBalloonWnd*			mBalloonWnd;
	IUnknownPtr							mBalloonRefHolder;
	tPtr <CAgentListeningWnd>			mListeningWnd;
	CAgentText							mLastSpeech;
	UINT								mLastButtonMsg;
	static HWND							mLastActive;
	_DACORE_IMPEXP static const UINT	mVoiceStartMsg;
	_DACORE_IMPEXP static const UINT	mVoiceEndMsg;
	_DACORE_IMPEXP static const UINT	mVoiceBookMarkMsg;
	_DACORE_IMPEXP static const UINT	mVoiceVisualMsg;
};

/////////////////////////////////////////////////////////////////////////////
