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
#include "AgentFile.h"
#include "AgentFileCache.h"
#include "AgentText.h"
#include "AgentBalloonOptions.h"
#include "AgentBalloonShape.h"
#include "SapiVoice.h"
#include "EventNotify.h"
#include "ImageBuffer.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CAgentBalloonWndObj :
	public CComObjectRootEx<CComMultiThreadModel>,
	public IUnknown,
	public CWindowImpl<CAgentBalloonWndObj, CWindow, CWinTraits<WS_POPUP|TTS_ALWAYSTIP|TTS_NOPREFIX|TTS_NOANIMATE|TTS_NOFADE> >
{
// Declarations
public:
	DECLARE_WND_SUPERCLASS(NULL, TOOLTIPS_CLASS)
	DECLARE_NOT_AGGREGATABLE(CAgentBalloonWndObj)
	DECLARE_GET_CONTROLLING_UNKNOWN()

	BEGIN_COM_MAP(CAgentBalloonWndObj)
		COM_INTERFACE_ENTRY(IUnknown)
	END_COM_MAP()
};

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CAgentBalloonWnd :
	public CAgentBalloonWndObj,
	public CAgentFileClient,
	public CEventNotifiesClient<CAgentBalloonWnd>,
	public CNotifySourceSink <CSapiVoice, _ISapiVoiceEventSink>
{
	DECLARE_DLL_OBJECT_EX(CAgentBalloonWnd, _DACORE_IMPEXP)
protected:
	CAgentBalloonWnd ();
public:
	_DACORE_IMPEXP virtual ~CAgentBalloonWnd ();
	_DACORE_IMPEXP static CAgentBalloonWnd * CreateInstance (long pCharID, CAtlPtrTypeArray <CEventNotify> & pNotify);

// Declarations
public:
	DECLARE_PROTECT_FINAL_RELEASE(CAgentBalloonWndObj)

// Attributes
public:
	_DACORE_IMPEXP long GetCharID () const;

	_DACORE_IMPEXP bool IsSpeechShape () const;
	_DACORE_IMPEXP bool IsThoughtShape () const;
	_DACORE_IMPEXP bool IsBusy (bool pForIdle) const;
	_DACORE_IMPEXP bool IsPaused () const;
	_DACORE_IMPEXP bool IsDrawingLayered () const;

	_DACORE_IMPEXP bool IsAutoSize () const;
	_DACORE_IMPEXP bool IsAutoPace () const;
	_DACORE_IMPEXP bool IsAutoHide () const;
	_DACORE_IMPEXP bool ClipPartialLines () const;

	const DWORD	mAutoPaceTime;
	DWORD		mAutoHideTime;

// Operations
public:
	_DACORE_IMPEXP bool ApplyOptions (CAgentBalloonOptions * pOptions = NULL);

	_DACORE_IMPEXP bool Create (CWindow * pOwnerWnd, DWORD pExStyle = 0);
	_DACORE_IMPEXP bool Attach (long pCharID, class CEventNotify * pNotify, bool pSetActiveCharID);
	_DACORE_IMPEXP bool Detach (long pCharID, class CEventNotify * pNotify);
	_DACORE_IMPEXP void FinalRelease ();

	_DACORE_IMPEXP bool ShowBalloonSpeech (LPCTSTR pText, UINT pSapiVersion = 5, bool pNoAutoPace = false);
	_DACORE_IMPEXP bool ShowBalloonSpeech (const CAgentText & pText, bool pNoAutoPace = false);
	_DACORE_IMPEXP bool ShowBalloonThought (LPCTSTR pText, UINT pSapiVersion = 5, bool pNoAutoPace = false);
	_DACORE_IMPEXP bool ShowBalloonThought (const CAgentText & pText, bool pNoAutoPace = false);
	_DACORE_IMPEXP bool ShowBalloonNow ();
	_DACORE_IMPEXP bool ShowBalloonAuto ();
	_DACORE_IMPEXP bool HideBalloon (bool pFast = false);
	_DACORE_IMPEXP bool MoveBalloon ();

	CAtlString GetDisplayText ();
	CAtlString GetSpeechText ();
	_DACORE_IMPEXP bool AbortSpeechText ();
	_DACORE_IMPEXP bool Pause (bool pPause);

// Overrides
protected:
	_DACORE_IMPEXP virtual void OnVoiceStart (long pCharID);
	_DACORE_IMPEXP virtual void OnVoiceEnd (long pCharID);
	_DACORE_IMPEXP virtual void OnVoiceWord (long pCharID, UINT pWordPos, int pWordLength);
	_DACORE_IMPEXP virtual void OnFinalMessage (HWND);
	_DACORE_IMPEXP virtual bool _PreNotify ();
	_DACORE_IMPEXP virtual bool _PostNotify ();

// Implementation
protected:
	_DACORE_IMPEXP LRESULT OnShow(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnPaint (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	_DACORE_IMPEXP LRESULT OnEraseBkgnd (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	_DACORE_IMPEXP LRESULT OnPrint (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	_DACORE_IMPEXP LRESULT OnPrintClient (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	_DACORE_IMPEXP LRESULT OnCustomDraw(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnWindowPosChanging (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	_DACORE_IMPEXP LRESULT OnWindowPosChanged (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	_DACORE_IMPEXP LRESULT OnSize (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	_DACORE_IMPEXP LRESULT OnShowWindow (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	_DACORE_IMPEXP LRESULT OnSetRedraw (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	_DACORE_IMPEXP LRESULT OnNcPaint (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	_DACORE_IMPEXP LRESULT OnNcHitTest (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	_DACORE_IMPEXP LRESULT OnTimer (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	_DACORE_IMPEXP LRESULT OnDestroy (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	_DACORE_IMPEXP LRESULT OnVoiceStartMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	_DACORE_IMPEXP LRESULT OnVoiceEndMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	_DACORE_IMPEXP LRESULT OnVoiceWordMsg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	_DACORE_IMPEXP LRESULT OnTtmActivate (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	_DACORE_IMPEXP LRESULT OnTtmTrackPosition (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	_DACORE_IMPEXP LRESULT OnTtmTrackActivate (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	_DACORE_IMPEXP LRESULT OnTtmUpdateTipText (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	_DACORE_IMPEXP LRESULT OnTtmUpdate (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);

	BEGIN_MSG_MAP(CAgentBalloonWnd)
		REFLECTED_NOTIFY_CODE_HANDLER(TTN_SHOW, OnShow)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_PRINT, OnPrint)
		MESSAGE_HANDLER(WM_PRINTCLIENT, OnPrintClient)
		REFLECTED_NOTIFY_CODE_HANDLER(NM_CUSTOMDRAW, OnCustomDraw)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGING, OnWindowPosChanging)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGED, OnWindowPosChanged)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
		MESSAGE_HANDLER(WM_SETREDRAW, OnSetRedraw)
		MESSAGE_HANDLER(WM_NCPAINT, OnNcPaint)
		MESSAGE_HANDLER(WM_NCHITTEST, OnNcHitTest)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(mVoiceStartMsg, OnVoiceStartMsg)
		MESSAGE_HANDLER(mVoiceEndMsg, OnVoiceEndMsg)
		MESSAGE_HANDLER(mVoiceWordMsg, OnVoiceWordMsg)
		MESSAGE_HANDLER(TTM_ACTIVATE, OnTtmActivate)
		MESSAGE_HANDLER(TTM_TRACKPOSITION, OnTtmTrackPosition)
		MESSAGE_HANDLER(TTM_TRACKACTIVATE, OnTtmTrackActivate)
		MESSAGE_HANDLER(TTM_UPDATETIPTEXT, OnTtmUpdateTipText)
		MESSAGE_HANDLER(TTM_UPDATE, OnTtmUpdate)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

protected:
	bool ShowingBalloon ();
	bool ShowBalloonText (const CAgentText & pText, UINT pForSpeech, bool pNoAutoPace);
	bool ShowBalloon (bool pForSpeech, bool pTextChanged);
	void ShowedBalloon (bool pWasVisible);
	bool CalcLayoutRects (CRect & pTextRect, CRect & pOwnerRect, CRect & pBounds);
	bool CalcWinRect (CRect & pWinRect, bool pOnShow = false);
	void ApplyLayout (const CRect & pWinRect, bool pOnShow = false);
	bool ApplyRegion (bool pRedraw = true);

	bool StartAutoPace ();
	bool StopAutoPace ();
	bool StartAutoHide ();
	bool StopAutoHide ();
	bool StartAutoScroll ();
	bool StopAutoScroll ();

	void DrawBalloon (HDC pDC, const CRect & pDrawRect);
	void DrawBalloonText (HDC pDC, const CRect & pDrawRect);
	DWORD ApplyFontLayout (HDC pDC);
	void ShowedVoiceWord (bool pFastRefresh);

protected:
	CAgentBalloonOptions				mOptions;
	tPtr <CAgentBalloonOptions>			mPendingOptions;
	CFontHandle							mFont;
	tSS <TOOLINFO, UINT>				mToolInfo;
	CAgentTextDraw						mText;
	bool								mAutoPaceDisabled;
	UINT_PTR							mAutoPaceTimer;
	UINT_PTR							mAutoHideTimer;
	UINT_PTR							mAutoScrollTimer;
	tPtr <class CAgentBalloonShape>		mShape;
	tPtr <CSize>						mShapeSize;
	CImageBuffer						mShapeBuffer;
	CImageBuffer						mDrawBuffer;
	long								mCharID;
	static const UINT					mVoiceStartMsg;
	static const UINT					mVoiceEndMsg;
	static const UINT					mVoiceWordMsg;
private:
	CWindow *							mOwnerWnd;
	bool								mRedrawDisabled;
	bool								mPacingSpeech;
	bool								mApplyingLayout;
	bool								mApplyingRegion;
	bool								mPaused;
	INT_PTR								mPausedWord;

private:
	UINT EnterRecursion () const;
	UINT ExitRecursion () const;
	CAtlString RecursionIndent () const;
	mutable UINT mDebugRecursionLevel;
};

/////////////////////////////////////////////////////////////////////////////
